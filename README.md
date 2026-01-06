# EQ3 BLE valves integration with ESPHome

## What is this?

This project provides an experimental integration of EQ3 BLE thermostatic valves into Home Assistant using ESPHome.

An ESP device running a custom ESPHome configuration directly manages BLE communication with EQ3 valves.

The BLE protocol is implemented on the ESP device, which exposes valve controls and sensor data to Home Assistant as native ESPHome entities.

No custom Home Assistant integration is required. All logic runs on the ESP device.

---

## Project status

This project is currently in an experimental stage.

It relies on specific ESPHome features and APIs, which may change between ESPHome releases.
The project has been tested with ESPHome version 2025.11.5.

---

## Repository structure

This repository is organized as follows:

esphome/
ESPHome configuration files and custom components

homeassistant/
Notes and examples related to Home Assistant usage

docs/
Additional documentation and design notes

examples/
Example configurations and reference files


---

## What this project is not

- This is not an official EQ3 integration
- This is not a Home Assistant custom component
- This project does not use the Home Assistant BLE proxy
- Home Assistant does not handle BLE communication directly

---

## Notes

This project is intended for users familiar with ESPHome and Home Assistant.
Breaking changes may occur as ESPHome evolves and as this project develops.

