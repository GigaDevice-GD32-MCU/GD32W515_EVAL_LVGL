#ifndef LV_WIDGETS_SLIDESHOW_H
#define LV_WIDGETS_SLIDESHOW_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Start auto-slideshow on the widgets demo tabview.
 * @note   Each tab scrolls from top to bottom and back, then switches
 *         to the next tab. The cycle repeats indefinitely.
 */
void lv_demo_widgets_start_slideshow(void);

/**
 * @brief  Stop the running slideshow animation.
 */
void lv_demo_widgets_stop_slideshow(void);

#ifdef __cplusplus
}
#endif

#endif
