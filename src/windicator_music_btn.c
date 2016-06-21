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

#include <aul.h>

#include "windicator.h"
#include "log.h"
#include "windicator_music_btn.h"
#include "windicator_util.h"
#include "windicator_moment_bar.h"
#include "windicator_volume.h"

void _music_btn_clicked_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("_music_btn_clicked_cb");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        //launch music app
	if(ad->music_status == MUSIC_BTN_ENABLED)
	{
		ad->launch_music_trigger = 1;
		//windicator_util_launch_app(ad);
	}
	else
	{
		ad->launch_music_trigger = 0;
		("Music app is NOT ongoing, so skip this click event");
	}
}

void on_pressed_music_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->music_status == MUSIC_BTN_ENABLED)
        {
                elm_object_signal_emit(ad->music_btn_layout, "music.pressed", "img.music.bg");
        }
}

void on_released_music_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->music_status == MUSIC_BTN_ENABLED)
        {
                elm_object_signal_emit(ad->music_btn_layout, "music.released", "img.music.bg");
        }
}

Evas_Object *windicator_music_btn_layout_create(Evas_Object *parent, void *data)
{
        retv_if(parent== NULL, NULL);

        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

        Evas_Object *layout = elm_layout_add(parent);
        retv_if(layout == NULL, NULL);

        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/music_btn") != EINA_TRUE) {
        		_E("Failed to set layout");
                return NULL;
        }
        evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_fill_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
        elm_object_part_content_set(parent, "music.swallow", layout);
        /* for press effect */
        elm_object_signal_callback_add(layout, "mouse,down,1", "img.music.icon", on_pressed_music_icon, ad);
        elm_object_signal_callback_add(layout, "mouse,up,1", "img.music.icon", on_released_music_icon, ad);

        ad->music_btn_layout = layout;

        Evas_Object* icon = elm_image_add(ad->music_btn_layout);
        elm_image_file_set(icon, ICON_PATH"/Controls/music.png", NULL);
        evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_object_part_content_set(ad->music_btn_layout, "img.music.icon", icon);
        evas_object_show(icon);
        ad->music_icon = icon;
        if(_music_icon_update(ad) != WINDICATOR_ERROR_OK) {
        		_E("Failed to update MUSIC button");
        }
        evas_object_show(layout);
        return layout;
}

void windicator_music_btn_layout_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->music_btn_layout != NULL) {
                _D("Destroy MUSIC Layout");
                evas_object_del(ad->music_btn_layout);
                ad->music_btn_layout = NULL;
        }
}

windicator_error_e _music_icon_update(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        if(ad->music_btn_layout == NULL) {
        		_E("Failed to create MUSIC button");
                return WINDICATOR_ERROR_FAIL;
        }

        if(ad->music_status == MUSIC_BTN_ENABLED) {
		_D("Enabled Music image (BT model)");
                elm_object_signal_emit(ad->music_btn_layout, "img.music.enable", "img.music.icon");
        } else {
		_D("Disabled Music image (BT model)");
                elm_object_signal_emit(ad->music_btn_layout, "img.music.disable", "img.music.icon");
        }

        return WINDICATOR_ERROR_OK;
}

windicator_error_e windicator_music_btn_update(void *data)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);
        _I("update MUSIC button");
        if(_music_icon_update(ad) != WINDICATOR_ERROR_OK)
        {
        		_E("Failed to update MUSIC button");
		}
        return WINDICATOR_ERROR_OK;
}
