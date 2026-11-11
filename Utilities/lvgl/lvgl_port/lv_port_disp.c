/**
 * @file lv_port_disp.c
 */

#include "lv_port_disp.h"
#include <stdbool.h>

#include "lcd_driver.h"

#define MY_DISP_HOR_RES XSIZE_PHYS
#define MY_DISP_VER_RES YSIZE_PHYS

#define LV_BUF_TYPE 2

#if LV_BUF_TYPE == 1
__attribute__((aligned(8))) uint16_t buf_2[320 * 120];
#elif LV_BUF_TYPE == 2
__attribute__((aligned(8))) uint16_t buf_2[320 * 120];
__attribute__((aligned(8))) uint16_t buf_3[320 * 120];
#endif

lv_display_t * volatile g_disp_drv = NULL;

static void disp_init(void);
static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);

void lv_port_disp_init(void)
{
    lv_display_t *disp;

    disp_init();

    disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(disp, disp_flush);

#if LV_BUF_TYPE == 1
    lv_display_set_buffers(disp, buf_2, NULL, sizeof(buf_2), LV_DISPLAY_RENDER_MODE_PARTIAL);
#elif LV_BUF_TYPE == 2
    lv_display_set_buffers(disp, buf_2, buf_3, sizeof(buf_2), LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
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

void lcd_copy(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *src_data)
{
    uint16_t *data = src_data;

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

static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    if(disp_flush_enabled) {
#if USE_DMA_LCD == 1
        while(g_disp_drv != NULL) {
        }
        g_disp_drv = disp;
        lcd_copy(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)px_map);
#else
        lcd_copy(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint16_t *)px_map);
        lv_display_flush_ready(disp);
#endif
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
            lv_display_flush_ready((lv_display_t *)g_disp_drv);
            g_disp_drv = NULL;
        }
    }
}
#endif