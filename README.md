# LVGL Ported to GD32W515P EVAL

## Overview

This project ports `LVGL` to the `GD32W515P EVAL` evaluation board for GUI demonstrations.

The board features a `GD32W515PI` microcontroller based on the `ARM Cortex-M33` core, along with a `320 x 240` TFT  LCD display (SPI interface, no touch panel).
The project uses landscape orientation with the `RGB565` pixel format and 16-bit color depth.

Project toolchains include `Keil MDK-ARM (ARM Compiler 6)`, `IAR`, and `GD32EmbeddedBuilder`.

## Benchmark

The following benchmark results were obtained using `LVGL 9.6.0` with software rendering and double buffering.

| Name                      | Avg. CPU | Avg. FPS | Avg. time | render time | flush time |
|:------------------------- | --------:| --------:| ---------:| -----------:| ----------:|
| Empty screen              | 82%      | 20       | 49        | 5           | 44         |
| Moving wallpaper          | 99%      | 18       | 53        | 10          | 43         |
| Single rectangle          | 18%      | 49       | <1        | <1          | <1         |
| Multiple rectangles       | 89%      | 41       | 21        | 4           | 17         |
| Multiple RGB images       | 23%      | 50       | 1         | 1           | <1         |
| Multiple ARGB images      | 22%      | 50       | 4         | 4           | <1         |
| Rotated ARGB images       | 81%      | 41       | 22        | 21          | 1          |
| Multiple labels           | 54%      | 52       | 9         | 6           | 3          |
| Screen sized text         | 98%      | 18       | 52        | 30          | 22         |
| Multiple arcs             | 15%      | 50       | <1        | <1          | <1         |
| Containers                | 21%      | 50       | 8         | 5           | 3          |
| Containers with overlay   | 93%      | 19       | 47        | 16          | 31         |
| Containers with opa       | 35%      | 49       | 11        | 9           | 2          |
| Containers with opa_layer | 43%      | 49       | 16        | 12          | 4          |
| Containers with scrolling | 97%      | 18       | 52        | 16          | 36         |
| Widgets demo              | 99%      | 24       | 36        | 15          | 21         |
| All scenes avg.           | 60%      | 37       | 23        | 9           | 14         |

## Specification

- **MCU:** GD32W515PI (Cortex-M33, 180MHz)
- **RAM:** 448KB internal SRAM
- **Flash:** 2048KB internal
- **GPU:** None 
- **Resolution:** 320*240
- **Display Size:** 2.2 inch
- **Interface:** SPI
- **Color Depth:** 16-bit RGB565
- **Technology:** a-Si TFT
- **DPI:** 180 px/inch
- **Touch Pad:** None

## Getting Started

The following steps describe a general workflow. Confirm the project paths, software versions,
power supply requirements, and jumper settings against the actual project and the board manual.
This project currently supports three integrated development environments (IDEs): Keil MDK, IAR, and GD32EmbeddedBuilder. This document uses GD32EmbeddedBuilder to demonstrate how to build, download, and debug the project.

### Hardware Setup

1. Confirm that the board model is `GD32W515P EVAL`.
2. Check the TFT LCD display connections according to the board manual.
3. Connect the on-board GD-Link debug interface to your PC, and configure the power supply and jumpers as required.

### Software Setup

1. Install `GD32EmbeddedBuilder` for your operating system. Refer to the GD32EmbeddedBuilder software requirements for details.
2. Install the appropriate GD-Link driver software and confirm that your PC recognizes the debugger.

### Build the Project

1. Clone this repository.
   
   ```shell
   git clone "Repository URL"
   ```

2. Open `GD32EmbeddedBuilder` and click **Browse...** to select a local directory as your workspace.

3. Import the project by selecting **File > Import...** and choosing the project directory (`./Template/GD32EBuilder_project`).

4. Build the project by right-clicking the project in the project explorer and selecting **Build Project**.

5. To download and debug the project, right-click the project in the project explorer and select
    **Debug As > Debug Configurations... > GDB General Debugging**. Configure the settings for your setup,
    then click **Debug** to download the firmware and start debugging.

### Debugging

Start a debug session using your IDE and GD-Link to set breakpoints, step through code, and inspect variables.
If display  issues occur, first check peripheral initialization, SPI/DMA transfer, and display refresh.
The available debug options depend on your IDE and project configuration.

## Third-Party Components

| Category | In Use | Component | Version  | License |
| -------- | ------ | --------- | -------- | ------- |
| GUI      | Yes    | `LVGL`    | `V9.6.0` | `MIT`   |

When adding a third-party library, update this table and retain its license text and copyright notices.

## Contribution and Support

If you find any issues with the development board feel free to open an Issue in this repository. For LVGL related issues (features, bugs, etc) please use the main [lvgl repository](https://github.com/lvgl/lvgl).

If you found a bug and found a solution too please send a Pull request. If you are new to Pull requests refer to [Our Guide](https://docs.lvgl.io/master/CONTRIBUTING.html#pull-request) to learn the basics.
