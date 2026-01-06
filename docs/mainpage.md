# EQ3 BLE valves integration with ESPHome

This project provides an experimental integration of EQ3 BLE thermostatic valves
into Home Assistant using ESPHome.

An ESP device running a custom ESPHome configuration directly manages BLE
communication with EQ3 valves.  
The EQ3 BLE protocol is implemented directly on the ESP device, which exposes
valve controls and sensor data to Home Assistant as native ESPHome entities.

No custom Home Assistant integration is required: all logic runs on the ESP device.

## Project Status  
This project is experimental and relies on ESPHome internals that may change  
between releases. It has been tested with ESPHome **2025.11.5**  .
  
---

## Getting started
- [ESPHome configuration](config.md)

## Architecture and internals
- [Architecture overview](architecture.md)
- [BLE transport layer](ble_transport.md)
- [EQ3 protocol](protocol.md)

## Troubleshooting and notes
- [Troubleshooting](troubleshooting.md)

---

## Additional project information
For a complete project overview, repository structure, and design notes,
see the full project README:

- [Project README](../README.md)

