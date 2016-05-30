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
#include "windicator_battery.h"
#include "windicator_util.h"
#include "windicator_moment_view.h"

Evas_Object *windicator_battery_icon_create(Evas_Object *parent, void *data)
{
        retv_if(parent == NULL, NULL);
        _I("moment dynamic view battery initialize");
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

        Evas_Object *icon = elm_layout_add(parent);
        retv_if(icon == NULL, NULL);

        if(elm_layout_file_set(icon, EDJ_FILE, "windicator/battery_icon") != EINA_TRUE) {
                _E("Failed to set battery icon");
                evas_object_del(icon);
                return NULL;
        }

        evas_object_size_hint_weight_set (icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(icon, EVAS_HINT_FILL, EVAS_HINT_FILL);

        elm_object_part_content_set(parent, "sw.icon.battery", icon);
        evas_object_show(icon);
        _I("moment dynamic view battery initialize END");

        return icon;
}

void windicator_battery_icon_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->moment_bar_battery_icon != NULL)
        {
                evas_object_del(ad->moment_bar_battery_icon);
                ad->moment_bar_battery_icon = NULL;
        }
}
