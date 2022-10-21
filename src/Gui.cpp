#include <TaskSchedulerDeclarations.h>
#include <TFT_eSPI.h>
#include "Gui.hpp"
#include "lvgl.h"
#include "images/networkconnected.c"
#include "images/networkdisconnected.c"

/* TFT instance and display buffers*/
TFT_eSPI tft = TFT_eSPI();
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

bool _isconnected = false;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint16_t c;

    tft.startWrite();                                                                            /* Start new TFT transaction */
    tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
    for (int y = area->y1; y <= area->y2; y++)
    {
        for (int x = area->x1; x <= area->x2; x++)
        {
            c = color_p->full;
            tft.writeColor(c, 1);
            color_p++;
        }
    }
    tft.endWrite();            /* terminate TFT transaction */
    lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

void render(lv_task_t *param)
{
    lv_obj_clean(lv_scr_act());
    lv_obj_t *img1 = lv_img_create(lv_scr_act(), NULL);

    if (_isconnected == false)
    {
        lv_img_set_src(img1, &networkdisconnected);
    }
    else
    {
        lv_img_set_src(img1, &networkconnected);
    }

    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void Gui::connected()
{
    _isconnected = true;
}

void Gui::setup(Scheduler &ts)
{
    lv_init();

    tft.begin();        /* TFT init */
    tft.setRotation(3); /* Landscape orientation */

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    lv_task_create(render, 50, LV_TASK_PRIO_MID, NULL);

    // Render the screen once
    lv_tick_inc(50);
    lv_task_handler();
}