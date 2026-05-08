# MeshCore EastMesh Docs

MeshCore-EastMesh keeps the upstream MeshCore firmware intact and publishes four firmware tracks, depending on how the device needs to connect:

- `companion-wifi`: use this for Wi-Fi-connected companion devices. It stays closest to upstream MeshCore and adds the EastMesh Wi-Fi rescue/configuration commands.
- `repeater-bridge-espnow`: use this when you need a plain upstream-style repeater ESP-NOW bridge without MQTT uplink or the EastMesh web panel.
- `repeater-mqtt`: use this for a Wi-Fi repeater that should publish to an MQTT broker and, on supported ESP32 boards, offer the optional local web panel for setup and troubleshooting.
- `repeater-mqtt-bridge`: use this when one repeater needs both MQTT uplink and ESP-NOW bridge duties, including bridge channel/secret controls for keeping the bridge aligned with Wi-Fi.

!!! note "Bridge tracks are local radio bridges"

    The bridge tracks are for bridging two nearby repeaters that operate on different MeshCore radio configs, for example one repeater on `Australia (Narrow)` and another on `Australia (Mid)`.

    They are not MQTT-over-WAN, VPN, or internet bridge releases. MQTT is still the uplink/visibility path for MQTT firmware; it is not used to tunnel mesh traffic between distant sites.

If you are just trying to pick a board or download firmware, start with:

- [Compare Boards](./boards.md)
- [Download and Flash Releases](./releases.md)
- [Flash NOW!](https://flasher.eastmesh.au)

## End User Guides

- [Compare Boards](./boards.md)
- [Download and Flash Releases](./releases.md)
- [Migration From xJARiD/MeshCore](./migration.md)
- [Use the Repeater Web Panel](./web-panel.md)
- [Use the Repeater Web API](./api.md)
- [Custom CLI Commands](./custom-cli.md)

## Developer Notes

- [Build Locally With uv](./local-builds.md)

## Current Scope

This docs site only covers the EastMesh-specific pieces in this repository.

For general MeshCore behavior, radio operation, and upstream firmware concepts, refer to the upstream project:

- [meshcore-dev/MeshCore](https://github.com/meshcore-dev/MeshCore)

## Australian Regional References

These references are community-maintained rather than official project documentation, but they are still useful for Australian regional setup notes, local conventions, and area-specific MeshCore references.

### ACT / NSW / QLD / SA / TAS / VIC

- [wiki.eastmesh.au](https://wiki.eastmesh.au/)
- [wiki.meshcoreaus.org](https://wiki.meshcoreaus.org/)

### Sydney

- [nswmesh.au](https://nswmesh.au/)
- [github.com/nswmesh](https://github.com/nswmesh/)

### Brisbane

- [wiki.mbug.com.au/en/Meshcore/Settings](https://wiki.mbug.com.au/en/Meshcore/Settings)
