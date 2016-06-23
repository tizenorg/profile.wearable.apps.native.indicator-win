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

#include "windicator.h"
#include "log.h"
#include "windicator_volume.h"
#include "windicator_music_btn.h"
#include "windicator_util.h"

void on_pressed_volume_small_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->volume_small_icon, "volume.pressed", "img.volume.bg");
}

void on_released_volume_small_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->volume_small_icon, "volume.released", "img.volume.bg");
}

Evas_Object *windicator_volume_small_icon_create(Evas_Object *parent, void *data)
{
        retv_if(parent == NULL, NULL);

        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

//      Evas_Object *btn = NULL;
        Evas_Object *layout = elm_layout_add(parent);
        retv_if(layout == NULL, NULL);

        char full_path[PATH_MAX] = { 0, };
        _get_resource(EDJ_FILE, full_path, sizeof(full_path));
        _D("full_path:%s",full_path);

        if(elm_layout_file_set(layout, full_path, "windicator/volume/small_icon") != EINA_TRUE) {
        		_E("Failed to set volume small layout");
                evas_object_del(layout);
                return NULL;
        }

        evas_object_size_hint_weight_set (layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);

        elm_object_part_content_set(parent, "volume.swallow", layout);
        /* for press effect */
        elm_object_signal_callback_add(layout, "mouse,down,1", "img.volume.icon", on_pressed_volume_small_icon, ad);
        elm_object_signal_callback_add(layout, "mouse,up,1", "img.volume.icon", on_released_volume_small_icon, ad);
        evas_object_show(layout);

        return layout;
}

void windicator_volume_small_icon_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->volume_small_icon != NULL) {
                _D("Destroy VOLUME Layout");
                evas_object_del(ad->volume_small_icon);
                ad->volume_small_icon = NULL;
        }
}

void windicator_volume_icon_set_by_level(Evas_Object *icon, void *data)
{
        ret_if(icon == NULL);

        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        char value[4] = {5, 5, 5, 5};
        _I("volume index = (%s)", value);
        elm_object_part_text_set(icon,"txt.volume", value);
        elm_object_signal_emit(icon, "volume.icon.ringtone", "img.volume.icon");
}

windicator_error_e windicator_volume_update(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        int level = 10;
        //level = _current_device_volume_level_get(ad);
        _I("current device volume level : %d", level);

        /* update icon & slider */
        ad->volume_index = level;
        windicator_volume_icon_set_by_level(ad->volume_small_icon, ad);

        return WINDICATOR_ERROR_OK;
}
