# LVGL V8.3 ported to the GD32W515_EVAL

本工程将 `LVGL V8.3` 移植至 `GD32W515` 平台，用于 `GUI演示`。

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

> 引入新的第三方库时，请同步更新本表，并保留其许可证文本与版权声明。
