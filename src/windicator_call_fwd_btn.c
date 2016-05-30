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
#include "windicator_call_fwd_btn.h"
#include "windicator_util.h"
#include "windicator_moment_bar.h"

void on_pressed_call_fwd_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->callfwd_btn_layout, "callfwd.pressed", "img.callfwd.bg");
}

void on_released_call_fwd_icon(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        elm_object_signal_emit(ad->callfwd_btn_layout, "callfwd.released", "img.callfwd.bg");
}

Evas_Object *windicator_callfwd_btn_layout_create(Evas_Object *parent, void *data)
{
        retv_if(parent== NULL, NULL);

        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);
        _D("");

//      Evas_Object *btn_callfwd = NULL;

        Evas_Object *layout = elm_layout_add(parent);
        retv_if(layout == NULL, NULL);

        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/callfwd_btn") != EINA_TRUE) {
        		_E("Failed to set layout");
                return NULL;
        }
        evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_fill_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);

        elm_object_part_content_set(parent, "callfwd.swallow", layout);
        //Get call_fwd_button_show from a function which detects the sim is present or not
        int call_fwd_button_show = 0;
        if(call_fwd_button_show)
        {
			/* for press effect */
			elm_object_signal_callback_add(layout, "mouse,down,1", "img.callfwd.icon", on_pressed_call_fwd_icon, ad);
			elm_object_signal_callback_add(layout, "mouse,up,1", "img.callfwd.icon", on_released_call_fwd_icon, ad);
        }

        ad->callfwd_btn_layout = layout;
        evas_object_show(layout);
        return layout;
}

windicator_error_e windicator_call_fwd_btn_update(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        if(ad->callfwd_btn_layout == NULL) {
                _E("Failed to update Call Forward button");
                return WINDICATOR_ERROR_FAIL;
        }
        //Get call_fwd_button_show from a function which detects the sim is present or not
        int call_fwd_button_show = 0;
        if(call_fwd_button_show)
        	elm_object_signal_emit(ad->callfwd_btn_layout, "img.callfwd.enable", "img.callfwd.icon");
        else
        	elm_object_signal_emit(ad->callfwd_btn_layout, "img.callfwd.disable", "img.callfwd.icon");

        return WINDICATOR_ERROR_OK;
}

void windicator_callfwd_btn_layout_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->callfwd_btn_layout != NULL) {
                _D("Destroy CALL FORWARDING Layout");
                evas_object_del(ad->callfwd_btn_layout);
                ad->callfwd_btn_layout = NULL;
        }
}
