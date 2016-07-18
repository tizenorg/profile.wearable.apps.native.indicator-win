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

enum {
	DND_BTN_ENABLED,
	DND_BTN_DISABLED,
	DND_BTN_DIM,
};

static struct _s_info {
	int dnd_btn_status;
} s_info = {
	.dnd_btn_status = DND_BTN_ENABLED,
};

static Evas_Object* __dnd_create_win(const char *name)
{
	_W("API() entered.");

	Evas_Object *pObj = NULL;

	pObj = elm_win_add(NULL, name, ELM_WIN_BASIC);
	if (!pObj)
		return NULL;

	elm_win_alpha_set(pObj, EINA_TRUE);
	elm_win_title_set(pObj, name);
	elm_win_autodel_set(pObj, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(pObj)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(pObj, (const int*)(&rots), 4);
	}

	elm_win_indicator_mode_set(pObj, ELM_WIN_INDICATOR_HIDE);

	elm_win_aux_hint_add(pObj, "wm.comp.win.effect.enable", "0");

	return pObj;
}

static void _popup_hide_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (!obj) return;
	elm_popup_dismiss(obj);
}

static void _popup_response_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (!obj) return;
	int btn_val = 0;
	btn_val = (int)evas_object_data_get(obj, "btn_val");
	_D("btn_val = %d", btn_val);
	if (btn_val == 1) {
		vconf_set_bool(VCONFKEY_SETAPPL_BLOCKMODE_WEARABLE_BOOL, EINA_TRUE);
		s_info.dnd_btn_status = DND_BTN_ENABLED;
	}

	/* TODO: Suppose to call elm_popup_dismiss(data); */

	struct appdata *ad = (struct appdata *)windicator_appdata_get();
	windicator_dnd_btn_update(ad);

	evas_object_del(obj);

	evas_object_del(ad->dnd_win);
	ad->dnd_win = NULL;
	windicator_show_moment_bar_directly(ad);
}

static void _popup_hide_finished_cb(void *data, Evas_Object *obj, void *event_info)
{
	_D("_popup_hide_finished_cb");
	struct appdata *ad = (struct appdata *)data;
	ret_if(ad == NULL);

	if (!obj) return;
	evas_object_del(obj);

	evas_object_del(ad->dnd_win);
	ad->dnd_win = NULL;
	windicator_show_moment_bar_directly(ad);
}

static void _dnd_popup_launch(void* data)
{

	_W("blockingmode");

	struct appdata *ad = (struct appdata *)data;
	ret_if(ad == NULL);

	Evas_Object *popup;
	Evas_Object *btn;
	Evas_Object *icon;
	Evas_Object *layout;

	if (ad->dnd_win != NULL)
		evas_object_del(ad->dnd_win);
	ad->dnd_win = __dnd_create_win("DND");

	popup = elm_popup_add(ad->dnd_win);
	elm_object_style_set(popup, "circle");
	//uxt_popup_set_rotary_event_enabled(popup, EINA_TRUE);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _popup_hide_cb, NULL);
	evas_object_smart_callback_add(popup, "dismissed", _popup_hide_finished_cb, ad);

	layout = elm_layout_add(popup);
	elm_layout_theme_set(layout, "layout", "popup", "content/circle/buttons2");
	elm_object_part_text_set(layout, "elm.text.title", "Do not disturb");
	//need to translate
#ifdef _TIZEN_3G_DISABLE
	elm_object_part_text_set(layout, "elm.text", "Vibration will be turned off and screen will remain turned off for incoming calls and alerts, but not alarms.");
#else
	elm_object_part_text_set(layout, "elm.text", "Sounds will be muted and screen will remain turned off for incoming calls and alerts, but not alarms.");
#endif
	elm_object_content_set(popup, layout);

	btn = elm_button_add(popup);
	elm_object_style_set(btn, "popup/circle/left");
	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_content_set(popup, "button1", btn);
	evas_object_data_set(btn, "btn_val", (void*)0);
	evas_object_smart_callback_add(btn, "clicked", _popup_response_cb, popup);

	icon = elm_image_add(btn);

	char full_path[PATH_MAX] = { 0, };
	_get_resource(ICON_PATH"/tw_ic_popup_btn_delete.png", full_path, sizeof(full_path));
	_D("full_path:%s", full_path);

	elm_image_file_set(icon, full_path, NULL);
	evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_content_set(btn, "elm.swallow.content", icon);
	evas_object_show(icon);

	btn = elm_button_add(popup);
	elm_object_style_set(btn, "popup/circle/right");
	evas_object_data_set(btn, "btn_val", (void*)1);
	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_content_set(popup, "button2", btn);
	evas_object_smart_callback_add(btn, "clicked", _popup_response_cb, popup);

	icon = elm_image_add(btn);

	_get_resource(ICON_PATH"/tw_ic_popup_btn_check.png", full_path, sizeof(full_path));
	_D("full_path:%s", full_path);

	elm_image_file_set(icon, full_path, NULL);
	evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_content_set(btn, "elm.swallow.content", icon);
	evas_object_show(icon);

	evas_object_show(ad->dnd_win);
	evas_object_show(popup);
}

void _dnd_btn_clicked_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
		int ret = 0;
	_I("(%d)", s_info.dnd_btn_status);

	struct appdata *ad = (struct appdata *)data;
	ret_if(ad == NULL);

	if (s_info.dnd_btn_status == DND_BTN_ENABLED) {
		vconf_set_bool(VCONFKEY_SETAPPL_BLOCKMODE_WEARABLE_BOOL, EINA_FALSE);
			 if (ret < 0) {
					 _E("Failed to set VCONFKEY_SETAPPL_BLOCKMODE_WEARABLE_BOOL : %d", ret);
			 }

	    s_info.dnd_btn_status = DND_BTN_DISABLED;

			elm_object_signal_emit(ad->dnd_btn_layout, "dnd.disable", "dnd.bg.icon");
	} else {
		windicator_hide_moment_bar_directly(ad);
		_dnd_popup_launch(ad);
	}
}

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
	retv_if(parent == NULL, NULL);

	struct appdata *ad = (struct appdata *)data;
	retv_if(ad == NULL, NULL);

	Evas_Object *layout = elm_layout_add(parent);
	retv_if(layout == NULL, NULL);

	char full_path[PATH_MAX] = { 0, };
	_get_resource(EDJ_FILE, full_path, sizeof(full_path));
	_D("full_path:%s", full_path);

	if (elm_layout_file_set(layout, full_path, "windicator/dnd_btn/bt") != EINA_TRUE) {
		_E("Failed to set layout");
		evas_object_del(layout);
		return NULL;
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

	if (ad->dnd_btn_layout != NULL) {
		_D("Destroy DND Layout");
		evas_object_del(ad->dnd_btn_layout);
		ad->dnd_btn_layout = NULL;
	}
}

windicator_error_e windicator_dnd_btn_update(void *data)
{
	struct appdata *ad = (struct appdata *)data;
	retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

	int dnd_state = 0;
	int ret = vconf_get_bool(VCONFKEY_SETAPPL_BLOCKMODE_WEARABLE_BOOL, &dnd_state);
	if (ret < 0) {
		_E("Failed to get vconfkey(%s) : %d", VCONFKEY_SETAPPL_BLOCKMODE_WEARABLE_BOOL, ret);
		return WINDICATOR_ERROR_FAIL;
	}

	_I("update dnd button");
	if (dnd_state) {
			s_info.dnd_btn_status = DND_BTN_ENABLED;
			_D("Enabled DnD image (BT model)");
			elm_object_signal_emit(ad->dnd_btn_layout, "dnd.enable", "dnd.bg.icon");
		} else {
			s_info.dnd_btn_status = DND_BTN_DISABLED;
			_D("Disabled DnD image (BT model)");
			elm_object_signal_emit(ad->dnd_btn_layout, "dnd.disable", "dnd.bg.icon");
	}

	return WINDICATOR_ERROR_OK;
}

