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

#include <vconf.h>
#include <efl_extension.h>
#include <app_preference.h>

#include "windicator.h"
#include "log.h"
#include "windicator_dnd_btn.h"
#include "windicator_util.h"
#include "windicator_moment_bar.h"


#define PREKEY_WINDICATOR_DND_POPUP_ENABLED "windicator/dnd/popup_enabled"

void on_pressed_dnd_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->dnd_btn_layout, "dnd.pressed", "img.disturb.bg");
}

void on_released_dnd_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->dnd_btn_layout, "dnd.released", "img.disturb.bg");
}

Evas_Object *windicator_dnd_btn_layout_create(Evas_Object *parent, void *data)
{
        retv_if(parent== NULL, NULL);
        bool exist = false;

        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);

        Evas_Object *layout = elm_layout_add(parent);
        retv_if(layout == NULL, NULL);

        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/dnd_btn/bt") != EINA_TRUE) {
                _E("Failed to set layout");
                return NULL;
        }

        preference_is_existing(PREKEY_WINDICATOR_DND_POPUP_ENABLED,&exist);
        _I("exist = %d",exist);
        if(exist==false)
        {
                int ret = 0;
                ret = preference_set_int(PREKEY_WINDICATOR_DND_POPUP_ENABLED,1);
                if(ret != PREFERENCE_ERROR_NONE)
                {
                	_E("set preference error");
                }
        }

        evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_fill_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
        elm_object_part_content_set(parent, "dnd.swallow", layout);
        /* for press effect */
        elm_object_signal_callback_add(layout, "mouse,down,1", "img.disturb.icon", on_pressed_dnd_icon, ad);
        elm_object_signal_callback_add(layout, "mouse,up,1", "img.disturb.icon", on_released_dnd_icon, ad);

        evas_object_show(layout);
        return layout;
}

void windicator_dnd_btn_layout_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->dnd_btn_layout != NULL) {
                _D("Destroy DND Layout");
                evas_object_del(ad->dnd_btn_layout);
                ad->dnd_btn_layout = NULL;
        }
}

windicator_error_e windicator_dnd_btn_update(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);
        _I("update dnd button");
        elm_object_signal_emit(ad->dnd_btn_layout, "dnd.enable", "dnd.bg.icon");
        //img.disturb.icon
        /*const char *color_class = "AO012";
        edje_object_color_class_set(ad->dnd_btn_layout, color_class,
                                0, 0, 0, 128,
                                NULL, NULL, NULL, NULL,
                                NULL, NULL, NULL, NULL);*/
        return WINDICATOR_ERROR_OK;
}

