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

#include "windicator.h"
#include "log.h"
#include "windicator_flight_mode.h"
#include "windicator_util.h"
#include "windicator_moment_bar.h"

void on_pressed_flight_mode_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->flight_mode_layout, "flight.pressed", "img.flight.bg");
}

void on_released_flight_mode_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->flight_mode_layout, "flight.released", "img.flight.bg");
}

Evas_Object *windicator_flight_mode_layout_create(Evas_Object *parent, void *data)
{
        retv_if(parent== NULL, NULL);

        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

        Evas_Object *layout = elm_layout_add(parent);
        retv_if(layout == NULL, NULL);

        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/flight_mode") != EINA_TRUE) {
        		_E("Failed to set layout");
                return NULL;
        }
        evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_fill_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);

        evas_object_show(layout);

        elm_object_part_content_set(parent, "flight.swallow", layout);
        /* for press effect */
        elm_object_signal_callback_add(layout, "mouse,down,1", "img.flight.icon", on_pressed_flight_mode_icon, ad);
        elm_object_signal_callback_add(layout, "mouse,up,1", "img.flight.icon", on_released_flight_mode_icon, ad);

        return layout;
}

void windicator_flight_mode_layout_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->flight_mode_layout != NULL) {
                _D("Destroy Flight Mode Layout");
                evas_object_del(ad->flight_mode_layout);
                ad->flight_mode_layout = NULL;
        }
}

windicator_error_e windicator_flight_mode_update(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        _I("Enabled Flight Mode image");
        elm_object_signal_emit(ad->flight_mode_layout, "flight.enable", "flight.bg.icon");
        return WINDICATOR_ERROR_OK;
}
