#ifdef ESP_PLATFORM

#include "ESP32Board.h"

#if defined(ADMIN_PASSWORD) && !defined(DISABLE_WIFI_OTA)   // Repeater or Room Server only
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include <SPIFFS.h>

namespace {
AsyncWebServer* ota_server = nullptr;
char ota_id_buf[60];
char ota_home_buf[90];
volatile bool ota_start_pending = false;
constexpr uint8_t kOtaStartAttempts = 60;
constexpr uint32_t kOtaStartRetryDelayMs = 500;

bool startOTAServerNow() {
  if (ota_server != nullptr) {
    ota_server->end();
    delete ota_server;
    ota_server = nullptr;
  }

  ota_server = new AsyncWebServer(80);

  ota_server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", ota_home_buf);
  });
  ota_server->on("/log", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/packet_log", "text/plain");
  });

  AsyncElegantOTA.setID(ota_id_buf);
  AsyncElegantOTA.begin(ota_server);    // Start ElegantOTA
  ota_server->begin();
  if (ota_server->state() != LISTEN) {
    ota_server->end();
    delete ota_server;
    ota_server = nullptr;
    return false;
  }
  return true;
}

void startOTAServerTask(void*) {
  vTaskDelay(pdMS_TO_TICKS(1500));
  for (uint8_t attempt = 1; attempt <= kOtaStartAttempts; ++attempt) {
    if (startOTAServerNow()) {
      MESH_DEBUG_PRINTLN("OTA server listening on port 80");
      ota_start_pending = false;
      vTaskDelete(nullptr);
      return;
    }
    MESH_DEBUG_PRINTLN("OTA server port 80 busy, retry %u/%u", attempt, kOtaStartAttempts);
    vTaskDelay(pdMS_TO_TICKS(kOtaStartRetryDelayMs));
  }
  MESH_DEBUG_PRINTLN("OTA server failed to bind port 80");
  ota_start_pending = false;
  vTaskDelete(nullptr);
}
}

bool ESP32Board::startOTAUpdate(const char* id, char reply[]) {
  inhibit_sleep = true;   // prevent sleep during OTA

  IPAddress ota_ip;
  if (WiFi.status() == WL_CONNECTED) {
    ota_ip = WiFi.localIP();
  } else {
    WiFi.mode(WIFI_AP_STA);
    if (!WiFi.softAP("MeshCore-OTA", NULL)) {
      strcpy(reply, "Error - OTA AP start failed");
      return false;
    }
    ota_ip = WiFi.softAPIP();
  }

  sprintf(reply, "Started: http://%s/update", ota_ip.toString().c_str());
  MESH_DEBUG_PRINTLN("startOTAUpdate: %s", reply);

  snprintf(ota_id_buf, sizeof(ota_id_buf), "%s (%s)", id, getManufacturerName());
  snprintf(ota_home_buf, sizeof(ota_home_buf), "<H2>Hi! I am a MeshCore Repeater. ID: %s</H2>", id);

  if (!ota_start_pending) {
    ota_start_pending = true;
    if (xTaskCreate(startOTAServerTask, "ota-start", 4096, nullptr, 1, nullptr) != pdPASS) {
      const bool started = startOTAServerNow();
      ota_start_pending = false;
      if (!started) {
        strcpy(reply, "Error - OTA listener start failed");
        return false;
      }
    }
  }

  return true;
}

#else
bool ESP32Board::startOTAUpdate(const char* id, char reply[]) {
  return false; // not supported
}
#endif

#endif
