#include "lv_widgets_slideshow.h"
#include "lvgl.h"

static lv_obj_t * g_tabview = NULL;
static lv_anim_t   g_anim;

/* ── recursive find tabview ── */
static lv_obj_t * find_tabview(lv_obj_t * root)
{
    if (root == NULL) {
        return NULL;
    }

    if (lv_obj_check_type(root, &lv_tabview_class)) {
        return root;
    }

    uint32_t cnt = lv_obj_get_child_cnt(root);
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        lv_obj_t * c = lv_obj_get_child(root, i);
        lv_obj_t * t = find_tabview(c);
        if (t != NULL) {
            return t;
        }
    }

    return NULL;
}

/* ── animation callbacks ── */
static void scroll_anim_y_cb(void * var, int32_t v)
{
    lv_obj_scroll_to_y((lv_obj_t *)var, v, LV_ANIM_OFF);
}

static void slideshow_start_tab(lv_obj_t * tab);

static void slideshow_anim_completed_cb(lv_anim_t * a)
{
    lv_obj_t * tab  = (lv_obj_t *)a->var;
    lv_obj_t * cont = lv_obj_get_parent(tab);
    if (cont == NULL) return;

    lv_obj_t * tv = lv_obj_get_parent(cont);
    if ((tv == NULL) || !lv_obj_check_type(tv, &lv_tabview_class)) return;

    uint32_t tab_cnt = lv_obj_get_child_cnt(cont);
    if (tab_cnt < 2U) return;

    /* switch to next tab */
    uint16_t act  = lv_tabview_get_tab_act(tv);
    uint16_t next = (uint16_t)((act + 1U) % tab_cnt);
    lv_tabview_set_act(tv, next, LV_ANIM_ON);

    /* start scrolling the new tab */
    lv_obj_t * new_tab = lv_obj_get_child(cont, next);
    slideshow_start_tab(new_tab);
}

static void slideshow_start_tab(lv_obj_t * tab)
{
    if (tab == NULL) return;

    /* refresh layout so scroll dimensions are correct */
    lv_obj_t * cont = lv_obj_get_parent(tab);
    if (cont != NULL) {
        lv_obj_t * tv = lv_obj_get_parent(cont);
        if (tv != NULL) {
            lv_obj_update_layout(tv);
        }
    }

    /* duration: ~2ms per pixel, clamped to a reasonable range */
    int32_t  v = lv_obj_get_scroll_bottom(tab);
    if (v <= 0) {
        /* nothing to scroll – skip to next tab directly */
        g_anim.var = tab;
        slideshow_anim_completed_cb(&g_anim);
        return;
    }
    uint32_t t = (uint32_t)(v * 8);
    if (t > 8000) t = 8000;
    if (t < 2000) t = 2000;

    lv_anim_init(&g_anim);
    lv_anim_set_exec_cb(&g_anim, scroll_anim_y_cb);
    lv_anim_set_time(&g_anim, t);
    lv_anim_set_playback_time(&g_anim, t);
    lv_anim_set_values(&g_anim, 0, v);
    lv_anim_set_var(&g_anim, tab);
    lv_anim_set_ready_cb(&g_anim, slideshow_anim_completed_cb);
    lv_anim_start(&g_anim);
}

/* ── public API ── */
void lv_demo_widgets_start_slideshow(void)
{
    lv_demo_widgets_stop_slideshow();

    g_tabview = find_tabview(lv_scr_act());
    if (g_tabview == NULL) {
        return;
    }

    lv_obj_t * cont = lv_tabview_get_content(g_tabview);
    if (cont == NULL) return;

    lv_obj_t * tab = lv_obj_get_child(cont, 0);
    if (tab == NULL) return;

    slideshow_start_tab(tab);
}

void lv_demo_widgets_stop_slideshow(void)
{
    lv_anim_del(NULL, scroll_anim_y_cb);
}
