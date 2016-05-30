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
#include "windicator_moment_view.h"
#include "windicator_util.h"
#include "windicator_dynamic.h"
#include "windicator_battery.h"

Evas_Object *windicator_moment_view_battery_number_create(void *data)
{
        struct appdata *ad =(struct appdata *)data;
        retv_if(ad == NULL, NULL);

        _I("moment battery initialize");
        //int battery_level = 50;

        Evas_Object *layout = elm_layout_add(ad->moment_view_battery_layout);
        retv_if(layout == NULL, NULL);

        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/moment_view/battery/number") != EINA_TRUE) {
        	_E("Failed to create moment view layout");
                evas_object_del(layout);
                return NULL;
        }

        evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
        ad->moment_view_battery_number_layout = layout;
        elm_object_part_content_set(ad->moment_view_battery_layout,"image.number.swallow",layout);
        evas_object_show(layout);

        return layout;
}

Evas_Object *windicator_moment_view_battery_layout_create(void *data)
{
        struct appdata *ad =(struct appdata *)data;
        retv_if(ad == NULL, NULL);

        _I("moment battery initialize");
        int battery_level = 50;

        Evas_Object *layout = elm_layout_add(ad->moment_view_win);
        retv_if(layout == NULL, NULL);

        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/moment_view/battery") != EINA_TRUE) {
        	_E("Failed to create moment view layout");
                evas_object_del(layout);
                return NULL;
        }
        ad->moment_view_battery_layout = layout;

        ad->battery_level = battery_level;

        evas_object_resize(layout, ad->win_w, ad->win_h);

//      wnidicator_moment_view_battery_image_create(ad);

        evas_object_show(layout);

        windicator_moment_view_battery_number_create(data);

        _I("moment battery initialize END %x",ad->moment_view_battery_layout);
        return layout;
}

Evas_Object *windicator_dynamic_layout_for_moment_view_create(Evas_Object *parent, void *data)
{
        retv_if(parent == NULL, NULL);
        _I("moment dynamic view initialize");
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

        Evas_Object *layout = elm_layout_add(parent);
        retv_if(layout == NULL, NULL);

        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/dynamic/layout/3g") != EINA_TRUE) {
                _E("Failed to create dynamic layout");
                evas_object_del(layout);
                return NULL;
        }
        /* initialize battery icon */
        ad->moment_view_battery_icon = windicator_battery_icon_create(layout, ad);
        if(ad->moment_view_battery_icon == NULL) {
                _E("Failed to create battery icon");
			}

        elm_object_part_content_set(parent, "sw.dynamic", layout);
        _I("moment dynamic view initialize END");
        return layout;
}

Evas_Object *windicator_moment_view_layout_create(void *data)
{
        struct appdata *ad =(struct appdata *)data;
        retv_if(ad == NULL, NULL);

        _I("moment view initialize");

        Evas_Object *layout = elm_layout_add(ad->moment_view_win);
        retv_if(layout == NULL, NULL);

        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/moment_view/full") != EINA_TRUE) {
        		_E("Failed to create moment view layout");
                evas_object_del(layout);
                return NULL;
        }

        evas_object_resize(layout, ad->win_w, ad->win_h);

        /* create dynamic layout */
        ad->moment_view_dynamic_layout = windicator_dynamic_layout_for_moment_view_create(layout, ad);
        if(ad->moment_view_dynamic_layout == NULL) {
                _E("Failed to create dynamic layout");
        }

        /* create rssi icon */
        ad->moment_view_rssi_icon = windicator_rssi_icon_create(ad->moment_view_dynamic_layout, ad);
        if(ad->moment_view_rssi_icon == NULL) {
                _E("Failed to create RSSI icon for moment view");
                return NULL;
        }

        /* create connection icon */
        ad->moment_view_connection_icon = windicator_connection_icon_create(ad->moment_view_dynamic_layout, ad);
        if(ad->moment_view_connection_icon == NULL) {
                _E("Failed to create CONNECTION icon for moment view");
                return NULL;
        }

        evas_object_show(layout);

        _I("moment view initialize END");
        return layout;

}

windicator_error_e windicator_moment_view_init(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        /* create moment view window */
        ad->moment_view_win = windicator_util_win_create("__MOMENT_VIEW__", EINA_TRUE, TYPE_MOMENT_VIEW);
        retv_if(ad->moment_view_win == NULL, WINDICATOR_ERROR_FAIL);

        windicator_moment_view_battery_layout_create(ad);

        /* create moment view layout */
        ad->moment_view_layout = windicator_moment_view_layout_create(ad);
        retv_if(ad->moment_view_layout == NULL, WINDICATOR_ERROR_FAIL);

        //evas_object_resize(ad->moment_view_win, ad->win_w, ad->moment_view_h);
        evas_object_resize(ad->moment_view_win, ad->win_w, ad->moment_view_h);
        ad->moment_view_evas = evas_object_evas_get(ad->moment_view_win);
        ad->moment_view_ee = ecore_evas_ecore_evas_get(ad->moment_view_evas);

        if(WINDICATOR_ERROR_OK != windicator_util_win_notification_property_set(ad->moment_view_win)) {
                _E("Failed to set window notification property");
        }

        elm_win_prop_focus_skip_set(ad->moment_view_win, EINA_TRUE);
        return WINDICATOR_ERROR_OK;
}
