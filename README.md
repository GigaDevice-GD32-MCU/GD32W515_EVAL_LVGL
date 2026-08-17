# LVGL V8.3 ported to the GD32W515_EVAL

This project ports LVGL V8.3.11 to the GD32W515 EVAL platform for GUI demonstration.

## Hardware Information

The `GD32W515_EVAL` is based on:

- `GD32W515PIQ6` microcontroller (`ARM Cortex-M33` core, `180MHz`, FPU, DSP, TrustZone security)
- `2048 KB` on-chip Flash memory and `448 KB` on-chip RAM
- `2.2 寸` `<TFT>` display (`<320*240>`, `<SPI>`)
- no input device
- `On-board GD-LINK debugger/programmer for one-click download and debugging`
- `USB / I2C / I2S / IFRP / ADC / LED / KEY`

## Project Information

- GUI framework: `LVGL V8.3.11`
- Toolchain: `< Keil MDK / IAR / GD32EmbeddedBuilder>`
- Target board: `<GD32W515_EVAL V1.1>`
- Display configuration: `<320*240 / 16bit / 横向>`

## Third-Party Components

| Category | In use | Component | Version    | License |
| -------- | ------ | --------- | ---------- | ------- |
| GUI      | `Yes`  | `LVGL`    | `V 8.3.11` | `MIT`   |

> When adding new third-party libraries, update this table simultaneously and preserve the corresponding license texts and copyright statements.

