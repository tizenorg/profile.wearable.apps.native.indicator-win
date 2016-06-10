/*
 * Samsung API
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Flora License, Version 1.1 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://floralicense.org/license/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <efl_extension.h>
#include <device.h>
#include <vconf.h>

#include "windicator.h"
#include "log.h"
#include "windicator_brightness.h"
#include "windicator_util.h"

#define BRIGHTNESS_LEVEL_MAX 10

static int bOutdoorMode = 0;

void on_clicked_small_brightness_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("on_clicked_small_brightness_icon");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        //launch setting
        ad->launch_setting_trigger = 1;
        windicator_util_launch_app(ad);
}

void on_pressed_small_brightness_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->brightness_small_layout, "brightness.pressed", "img.brightness.bg");
}

void on_released_small_brightness_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->brightness_small_layout, "brightness.released", "img.brightness.bg");
}

Evas_Object *windicator_brightness_small_layout_create(Evas_Object *parent, void *data)
{
        retv_if(parent == NULL, NULL);

        _D("windicator_brightness_small_layout_create");
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

        Evas_Object *layout = elm_layout_add(parent);
        retv_if(layout == NULL, NULL);

        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/brightness/small_layout") != EINA_TRUE) {
        		_E("Failed to set brightness small layout");
                evas_object_del(layout);
                return NULL;
        }
        evas_object_size_hint_weight_set (layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

        elm_object_part_content_set(parent, "brightness.swallow", layout);
        /* for press effect */
        elm_object_signal_callback_add(layout, "mouse,down,1", "img.brightness.icon", on_pressed_small_brightness_icon, ad);
        elm_object_signal_callback_add(layout, "mouse,up,1", "img.brightness.icon", on_released_small_brightness_icon, ad);
        evas_object_show(layout);
        return layout;
}

static int _brightness_level_to_index(int level, int max)
{
        int index = 1;

        if(level <= 1) {
                index = 1;
        } else if(level <= 20) {
                index = 2;
        } else if(level <= 40) {
                index = 3;
        } else if(level <= 50) {
                index = 4;
        } else if(level <= 60) {
                index = 5;
        } else if(level <= 70) {
                index = 6;
        } else if(level <= 80) {
                index = 7;
        } else if(level <= 90) {
                index = 8;
        } else if(level <= 95) {
                index = 9;
        } else if(level <= 100) {
                index = 10;
        } else {
        		_E("Failed to change level to index");
                index = 1;
        }

        _I("brightness level: %d -> index: %d", level, index);

        return index;
}

char *windicator_util_str_from_icu_get(int level)
{
         char *ret_str = NULL;
         char locale[32] = {0,};

		 snprintf(locale, sizeof(locale)-1, "%d", level);
		 ret_str = strdup(locale);
		 if(ret_str == NULL) {
			   _E("Failed to duplicate string");
			   return NULL;
		 }
		 return ret_str;
}

void windicator_brightness_icon_set_by_level(Evas_Object *layout, void *data)
{
        ret_if(layout == NULL);

        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        char value[1024];

        _D("Brightness index, set (%s)", value);

		snprintf(value,sizeof(value),"%02d",ad->brightness_index);
		elm_object_part_text_set(layout,"txt.brightness",value);
}

static int _current_device_brightness_level_get(void)
{
        int level = 0;

        if(bOutdoorMode == 1)
        {
                _D("Outdoor mode, so get VCONFKEY_SETAPPL_LCD_BRIGHTNESS");
                if(vconf_get_int(VCONFKEY_SETAPPL_LCD_BRIGHTNESS, &level) < 0) {
                        _SECURE_E("Failed to get vconfkey : %s", VCONFKEY_SETAPPL_LCD_BRIGHTNESS);
                        device_get_brightness(0, &level);
                }
        }
        else
        {
                _D("NOT Outdoor mode, so use device_get_brightness()");
                device_get_brightness(0, &level);
        }

        _I("Brightness level : %d", level);
        return level;
}

windicator_error_e windicator_brightness_update(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        int level = 50;

        _I("current device brightness level : %d / isOutdoorMode", level);

        bOutdoorMode = 1;//display_get_hbm();

        level = _current_device_brightness_level_get();
        _D("current device brightness level : %d / isOutdoorMode : %d", level, bOutdoorMode);

        /* update icon & slider */
        ad->brightness_index = _brightness_level_to_index(level, BRIGHTNESS_LEVEL_MAX);
        windicator_brightness_icon_set_by_level(ad->brightness_small_layout, ad);
//      windicator_brightness_slider_set_by_level(ad);

        return WINDICATOR_ERROR_OK;
}

void windicator_brightness_small_layout_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->brightness_small_layout != NULL) {
                _D("Destroy BRIGHTNESS Layout");
                evas_object_del(ad->brightness_small_layout);
                ad->brightness_small_layout = NULL;
        }
}

