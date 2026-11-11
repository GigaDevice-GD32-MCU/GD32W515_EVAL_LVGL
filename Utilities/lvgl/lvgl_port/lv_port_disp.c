/**
 * @file lv_port_disp.c
 */

#include "lv_port_disp.h"
#include <stdbool.h>

#include "lcd_driver.h"

#define MY_DISP_HOR_RES XSIZE_PHYS
#define MY_DISP_VER_RES YSIZE_PHYS
#define LV_BUF_LINES 120

#define LV_BUF_TYPE 2

#if LV_BUF_TYPE == 1
__attribute__((aligned(8))) static lv_color_t buf_2[MY_DISP_HOR_RES * LV_BUF_LINES];
#elif LV_BUF_TYPE == 2
__attribute__((aligned(8))) static lv_color_t buf_2[MY_DISP_HOR_RES * LV_BUF_LINES];
__attribute__((aligned(8))) static lv_color_t buf_3[MY_DISP_HOR_RES * LV_BUF_LINES];
#endif

static lv_disp_drv_t * volatile g_disp_drv = NULL;

static void disp_init(void);
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

void lv_port_disp_init(void)
{
    static lv_disp_draw_buf_t draw_buf;
    static lv_disp_drv_t disp_drv;

    disp_init();

    lv_disp_draw_buf_init(&draw_buf, buf_2,
#if LV_BUF_TYPE == 2
                          buf_3,
#else
                          NULL,
#endif
                          MY_DISP_HOR_RES * LV_BUF_LINES);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
}

static void disp_init(void)
{
    lcd_init();
}

volatile bool disp_flush_enabled = true;

void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

static void lv_port_lcd_copy(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *src_data)
{
#if USE_DMA_LCD == 1
    dma_channel_disable(DMA0, DMA_CH3);
    dma_channel_disable(DMA0, DMA_CH4);
    spi_disable(SPI1);
    SPI_CTL0(SPI1) &= ~SPI_CTL0_FF16;
    spi_enable(SPI1);
    lcd_set_region(x, y, x + w - 1, y + h - 1);
    spi_disable(SPI1);
    SPI_CTL0(SPI1) |= SPI_CTL0_FF16;
    spi_enable(SPI1);
    LCD_RS_SET;
    LCD_CS_CLR;
    dma_flag_clear(DMA0, DMA_CH3, DMA_INT_FLAG_FTF);
    dma_flag_clear(DMA0, DMA_CH4, DMA_INT_FLAG_FTF);
    dma_memory_address_generation_config(DMA0, DMA_CH4, DMA_MEMORY_INCREASE_ENABLE);
    dma_memory_address_config(DMA0, DMA_CH4, DMA_MEMORY_0, (uint32_t)src_data);
    dma_transfer_number_config(DMA0, DMA_CH4, w * h);
    dma_transfer_number_config(DMA0, DMA_CH3, w * h);
    dma_channel_enable(DMA0, DMA_CH3);
    dma_channel_enable(DMA0, DMA_CH4);
#else
    uint16_t row;
    uint16_t column;
    uint16_t *data = src_data;

    lcd_set_region(x, y, x + w - 1, y + h - 1);
    LCD_RS_SET;
    LCD_CS_CLR;
    for(row = 0; row < h; row++) {
        for(column = 0; column < w; column++) {
            spi_write_byte(SPI1, (uint8_t)(*data >> 8));
            spi_write_byte(SPI1, (uint8_t)*data);
            data++;
        }
    }
    LCD_CS_SET;
#endif
}

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if(disp_flush_enabled) {
#if USE_DMA_LCD == 1
        while(g_disp_drv != NULL) {
        }
        g_disp_drv = disp_drv;
        lv_port_lcd_copy((uint16_t)area->x1, (uint16_t)area->y1,
                 (uint16_t)(area->x2 - area->x1 + 1),
                 (uint16_t)(area->y2 - area->y1 + 1),
                 (uint16_t *)color_p);
#else
        lv_port_lcd_copy((uint16_t)area->x1, (uint16_t)area->y1,
                 (uint16_t)(area->x2 - area->x1 + 1),
                 (uint16_t)(area->y2 - area->y1 + 1),
                 (uint16_t *)color_p);
        lv_disp_flush_ready(disp_drv);
#endif
    }
    else {
        lv_disp_flush_ready(disp_drv);
    }
}

#if USE_DMA_LCD == 1
void DMA0_Channel3_IRQHandler(void)
{
    if(SET == dma_interrupt_flag_get(DMA0, DMA_CH3, DMA_INT_FLAG_FTF)) {
        dma_interrupt_flag_clear(DMA0, DMA_CH3, DMA_INT_FLAG_FTF);
    }
}

void DMA0_Channel4_IRQHandler(void)
{
    if(SET == dma_interrupt_flag_get(DMA0, DMA_CH4, DMA_INT_FLAG_FTF)) {
        dma_interrupt_flag_clear(DMA0, DMA_CH4, DMA_INT_FLAG_FTF);
        if(g_disp_drv != NULL) {
            while(SET == spi_i2s_flag_get(SPI1, SPI_FLAG_TRANS)) {
            }
            LCD_CS_SET;
            lv_disp_flush_ready((lv_disp_drv_t *)g_disp_drv);
            g_disp_drv = NULL;
        }
    }
}
#endif