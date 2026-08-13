/**
 * @file lv_port_disp.h
 */

#ifndef LV_PORT_DISP_H
#define LV_PORT_DISP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "gd32w51x.h"

#define XSIZE_PHYS 320
#define YSIZE_PHYS 240

void lv_port_disp_init(void);
void disp_enable_update(void);
void disp_disable_update(void);

#ifdef __cplusplus
}
#endif

#endif