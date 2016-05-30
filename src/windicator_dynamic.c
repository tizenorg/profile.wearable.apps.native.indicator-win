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
#include "windicator_dynamic.h"
#include "windicator_battery.h"
#include "windicator_moment_view.h"

windicator_error_e windicator_dynamic_update(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);
        _D("");

        return WINDICATOR_ERROR_OK;
}

Evas_Object *windicator_rssi_icon_create(Evas_Object *parent, void *data)
{
        retv_if(parent == NULL, NULL);
        _I("moment dynamic view rssi initialize");
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

        Evas_Object *icon = elm_layout_add(parent);
        retv_if(icon == NULL, NULL);

        if(elm_layout_file_set(icon, EDJ_FILE, "windicator/rssi_icon") != EINA_TRUE) {
        		_E("Failed to set rssi icon");
                evas_object_del(icon);
                return NULL;
        }

        evas_object_size_hint_weight_set (icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(icon, EVAS_HINT_FILL, EVAS_HINT_FILL);

        _D("rssi create %x",icon);
        _I("moment dynamic view rssi initialize END");
        elm_object_part_content_set(parent, "sw.icon_0", icon);

        return icon;
}

Evas_Object *windicator_connection_icon_create(Evas_Object *parent, void *data)
{
        retv_if(parent == NULL, NULL);
        _I("moment dynamic view connection initialize");
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

        Evas_Object *icon = elm_layout_add(parent);
        retv_if(icon == NULL, NULL);

        if(elm_layout_file_set(icon, EDJ_FILE, "windicator/connection_icon") != EINA_TRUE) {
        		_E("Failed to set connection icon");
                evas_object_del(icon);
                return NULL;
        }

        evas_object_size_hint_weight_set (icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(icon, EVAS_HINT_FILL, EVAS_HINT_FILL);
        elm_object_part_content_set(parent, "sw.icon_1", icon);
        evas_object_show(icon);
        _I("moment dynamic view connection initialize END");

        return icon;
}

void windicator_rssi_icon_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->moment_bar_rssi_icon != NULL)
        {
                evas_object_del(ad->moment_bar_rssi_icon);
                ad->moment_bar_rssi_icon = NULL;
        }
}

Evas_Object *windicator_dynamic_layout_create(Evas_Object *parent, void *data)
{
        retv_if(parent == NULL, NULL);
        _I("moment bar first layout dynamic layout creates");
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

        Evas_Object *layout = elm_layout_add(parent);
        retv_if(layout == NULL, NULL);
        _I("Set Default dynamic layout (icon position)");
        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/dynamic/layout/3g") != EINA_TRUE) {
                _E("Failed to create dynamic layout");
                evas_object_del(layout);
                return NULL;
        }
        /* create rssi icon */
        ad->moment_bar_rssi_icon = windicator_rssi_icon_create(layout, ad);
        if(ad->moment_bar_rssi_icon == NULL) {
        	_E("Failed to create rssi icon");
                return NULL;
        }
        /* create connection icon */
        ad->moment_bar_connection_icon = windicator_connection_icon_create(layout, ad);
        if(ad->moment_bar_connection_icon == NULL) {
        	_E("Failed to create connection icon");
                return NULL;
        }

        elm_object_part_content_set(parent, "sw.dynamic", layout);
        evas_object_show(layout);
        _I("moment bar first layout dynamic layout create END");
        return layout;
}

void windicator_connection_icon_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->moment_bar_connection_icon != NULL)
        {
                evas_object_del(ad->moment_bar_connection_icon);
                ad->moment_bar_connection_icon = NULL;
        }
}

void windicator_dynamic_layout_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        _D("");

        /* Destroy connection icon */
        if(ad->moment_bar_connection_icon != NULL) {
                windicator_connection_icon_destroy(ad);
                ad->moment_bar_connection_icon = NULL;
        }
}
