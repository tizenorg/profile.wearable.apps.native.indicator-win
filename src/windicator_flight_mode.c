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

#include "windicator.h"
#include "log.h"
#include "windicator_flight_mode.h"
#include "windicator_util.h"
#include "windicator_moment_bar.h"

enum {
        FLIGHT_MODE_ENABLED,
        FLIGHT_MODE_DISABLED,
        FLIGHT_MODE_DIM,
};

static struct _s_info {
        int flight_mode_status;
} s_info = {
        .flight_mode_status = FLIGHT_MODE_ENABLED,
};

static Evas_Object* __flignt_mode_create_win(const char *name)
{
        _W("API() entered.");

        Evas_Object     *pObj = NULL;

        pObj = elm_win_add(NULL, name, ELM_WIN_BASIC);
        elm_win_alpha_set(pObj, EINA_TRUE);

        if (!pObj)
                return NULL;

        elm_win_title_set(pObj, name);
        elm_win_autodel_set(pObj, EINA_TRUE);

        if (elm_win_wm_rotation_supported_get(pObj))
        {
                int rots[4] = { 0, 90, 180, 270 };
                elm_win_wm_rotation_available_rotations_set(pObj, (const int*)(&rots), 4);
        }

        elm_win_indicator_mode_set(pObj, ELM_WIN_INDICATOR_HIDE);

        elm_win_aux_hint_add(pObj, "wm.comp.win.effect.enable", "0");

        return pObj;
}

static void _popup_hide_cb(void *data, Evas_Object *obj, void *event_info)
{
        if(!obj) return;
        elm_popup_dismiss(obj);
}

static void _popup_response_cb(void *data, Evas_Object *obj, void *event_info)
{
        if(!data) return;
        int btn_val = 0;
        int ret = -1;

        btn_val = (int)evas_object_data_get(obj,"btn_val");

        _W("btn_val = %d",btn_val);

        if(btn_val==1)//OK
        {
            ret = vconf_set_bool(VCONFKEY_TELEPHONY_FLIGHT_MODE, EINA_TRUE);
            if(ret < 0)
            {
                    _E("Failed to set VCONFKEY_TELEPHONY_FLIGHT_MODE : %d", ret);
            }

            s_info.flight_mode_status = FLIGHT_MODE_ENABLED;
        }

	struct appdata *ad = (struct appdata *)windicator_appdata_get();
	windicator_flight_mode_update(ad);

	if(!obj) return;
	evas_object_del(obj);

	evas_object_del(ad->flight_mode_win);
	ad->flight_mode_win = NULL;
}

static void _popup_hide_finished_cb(void *data, Evas_Object *obj, void *event_info)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(!obj) return;
        evas_object_del(obj);

        evas_object_del(ad->flight_mode_win);
        ad->flight_mode_win = NULL;
}

static void _flight_mode_popup_launch(void* data)
{

        _W("_flight_mode_popup_launch");

        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        Evas_Object *popup;
        Evas_Object *btn;
        Evas_Object *icon;
        Evas_Object *layout;

        if(ad->flight_mode_win != NULL)
                evas_object_del(ad->flight_mode_win);
        ad->flight_mode_win = __flignt_mode_create_win("FLIGHT_MODE");

        popup = elm_popup_add(ad->flight_mode_win);
        elm_object_style_set(popup, "circle");
        evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _popup_hide_cb, NULL);
        evas_object_smart_callback_add(popup, "dismissed", _popup_hide_finished_cb, ad);

        layout = elm_layout_add(popup);
        elm_layout_theme_set(layout, "layout", "popup", "content/circle/buttons2");
        elm_object_part_text_set(layout, "elm.text.title", "Flight Mode");

        //need to translate
		_W("[Flight Mode] popup");
		elm_object_part_text_set(layout, "elm.text", "Flight mode disables calls, messaging and all connections. To use Wi-Fi and Bluetooth, go to Settings.");

        elm_object_content_set(popup, layout);

        btn = elm_button_add(popup);
        elm_object_style_set(btn, "popup/circle/left");
        evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_object_part_content_set(popup, "button1", btn);
        evas_object_data_set(btn,"btn_val",(void*)0);
        evas_object_smart_callback_add(btn, "clicked", _popup_response_cb, popup);

        icon = elm_image_add(btn);
        elm_image_file_set(icon, ICON_PATH"/tw_ic_popup_btn_delete.png", NULL);
        evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_object_part_content_set(btn, "elm.swallow.content", icon);
        evas_object_show(icon);

        btn = elm_button_add(popup);
        elm_object_style_set(btn, "popup/circle/right");
        evas_object_data_set(btn,"btn_val",(void*)1);
        evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_object_part_content_set(popup, "button2", btn);
        evas_object_smart_callback_add(btn, "clicked", _popup_response_cb, popup);

        icon = elm_image_add(btn);
        elm_image_file_set(icon, ICON_PATH"/tw_ic_popup_btn_check.png", NULL);
        evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_object_part_content_set(btn, "elm.swallow.content", icon);
        evas_object_show(icon);

        evas_object_show(ad->flight_mode_win);
        evas_object_show(popup);
}

void _flight_mode_clicked_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
        _I("(%d)", s_info.flight_mode_status);

        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);
        int ret = -1;

         if(s_info.flight_mode_status == FLIGHT_MODE_ENABLED) {
        	 ret = vconf_set_bool(VCONFKEY_TELEPHONY_FLIGHT_MODE, EINA_FALSE);
			 if(ret < 0)
			 {
					 _E("Failed to set VCONFKEY_TELEPHONY_FLIGHT_MODE : %d", ret);
			 }

			 s_info.flight_mode_status = FLIGHT_MODE_DISABLED;

			 elm_object_signal_emit(ad->flight_mode_layout, "flight.disable", "flight.bg.icon");
         } else {
			 windicator_hide_moment_bar_directly(ad);
			 _flight_mode_popup_launch(ad);
         }
}

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
#ifndef _TIZEN_3G_DISABLE
        //Initialize the tele init
#endif
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
#ifndef _TIZEN_3G_DISABLE
        //Init tele
#endif
        int flight_mode_state = 0;
        int ret = vconf_get_bool(VCONFKEY_TELEPHONY_FLIGHT_MODE, &flight_mode_state);
        if(ret < 0) {
                _E("Failed to get vconfkey(%s) : %d", VCONFKEY_TELEPHONY_FLIGHT_MODE, ret);
                return WINDICATOR_ERROR_FAIL;
        }
        _D("Flight Mode %d", flight_mode_state);
        if(flight_mode_state) {
                s_info.flight_mode_status = FLIGHT_MODE_ENABLED;
                _D("Enabled Flight Mode image");
                elm_object_signal_emit(ad->flight_mode_layout, "flight.enable", "flight.bg.icon");
        } else {
                s_info.flight_mode_status = FLIGHT_MODE_DISABLED;
                _D("Disabled Flight Mode image");
                elm_object_signal_emit(ad->flight_mode_layout, "flight.disable", "flight.bg.icon");
        }

        return WINDICATOR_ERROR_OK;
}
