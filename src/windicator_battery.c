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
#include <device.h>
//#include <bincfg.h>

#include "windicator.h"
#include "log.h"
#include "windicator_battery.h"
#include "windicator_util.h"
#include "windicator_moment_view.h"

int windicator_battery_charge_now_get(void)
{
	int ret = 0;
	int is_charged = 0;

	ret = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW, &is_charged);
	if (ret < 0) {
		_SECURE_E("Failed to get vconfkey(%d) : %s", ret, is_charged);
		return 0;
	}
	_W("VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW : %d", is_charged);

	return is_charged;
}

void _charging_ui_update(void* data)
{

	struct appdata *ad = data;
	ret_if(ad == NULL);

	_W("ad->is_connected : %d // ad->is_full : %d // ad->is_charged : %d", ad->is_connected, ad->is_full, ad->is_charged);
	int bat_lev  = 6;
	 vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CAPACITY, &bat_lev);
	if (ad->is_connected == 1 && (ad->is_full != VCONFKEY_SYSMAN_BAT_FULL) && (ad->is_charged != -1)) {
		if (bat_lev < 5) {
			elm_object_signal_emit(ad->moment_bar_battery_icon, "hide.img.battery", "img.battery");
			elm_object_signal_emit(ad->moment_bar_battery_icon, "Show.Battery.Charge", "img.battery.charge");
		} else {
			_W("[SHOW charging icon] Connected / not Full / not charge -1");
			elm_object_signal_emit(ad->moment_bar_battery_icon, "Show.Battery.Charge", "img.battery.charge");
		}

	} else {
		if (bat_lev <  5) {
			_W("[HIDE charging icon] Not connected or Battery full (5)");
			elm_object_signal_emit(ad->moment_bar_battery_icon, "Hide.Battery.Charge", "img.battery.charge");
			elm_object_signal_emit(ad->moment_bar_battery_icon, "change_charging_level_00", "img.battery");

		} else {
			_W("[HIDE charging icon] Not connected or Battery full (5)");
			elm_object_signal_emit(ad->moment_bar_battery_icon, "Hide.Battery.Charge", "img.battery.charge");
		}
	}
}

static void _battery_status_changed_cb(keynode_t *node, void *data)
{
	_W("");

	struct appdata *ad = (struct appdata *)data;
	ret_if(ad == NULL);

	int fully_charged = 0;
	vconf_get_int(VCONFKEY_SYSMAN_BATTERY_STATUS_LOW, &fully_charged);

	ad->is_full = fully_charged;

	_charging_ui_update(ad);

/*	if (fully_charged == VCONFKEY_SYSMAN_BAT_FULL) {
		_W("Show Moment View to display battery full");
		if (WINDICATOR_ERROR_OK != windicator_moment_view_show(ad,1)) {
			_E("Failed to show moment view");
		}
	}*/
}


void _battery_update(void* data)
{
	//by default getting the battery at the start
	 vconf_set_bool(VCONFKEY_SETAPPL_BATTERY_PERCENTAGE_BOOL, EINA_TRUE);
	_W("");

	struct appdata *ad = data;
	ret_if(ad == NULL);

	/* Moment Bar */
	if (WINDICATOR_ERROR_OK != windicator_battery_icon_update(ad->moment_bar_dynamic_layout, ad)) {
		_E("Failed to update battery icon");
	}

	/* Moment View */
	/*if (WINDICATOR_ERROR_OK != windicator_battery_icon_update(ad->moment_view_dynamic_layout, ad)) {
		_E("Failed to update battery icon");
	}*/
}

static void _battery_charging_changed_cb(keynode_t *node, void *data)
{


	struct appdata *ad = data;
	ret_if(ad == NULL);

	ad->is_charged = windicator_battery_charge_now_get();

	int ret = 0;
	ret = vconf_get_int(VCONFKEY_SYSMAN_CHARGER_STATUS, &ad->is_connected);
	if (ret < 0) {
		_SECURE_E("Failed to get vconfkey(%d) : %s", ret, VCONFKEY_SYSMAN_CHARGER_STATUS);
		ad->is_connected = 0;
	}

	_D("battery charge state(%d) TA state(%d)", ad->is_charged, ad->is_connected);

	_charging_ui_update(ad);
	_battery_update(data);

/*
	if (WINDICATOR_ERROR_OK != windicator_battery_icon_update(ad->moment_bar_dynamic_layout,ad)) {
			_E("Failed to update battery");
	}

	if (ad->is_charged != -1) {
		elm_object_signal_emit(ad->moment_bar_dynamic_layout, "showBattery", "sw.icon.battery");
		elm_object_signal_emit(ad->moment_view_dynamic_la	//elm_object_signal_emit(icon,"Show.Battery.Charge","img.battery.charge");yout, "showBattery", "sw.icon.battery");
	} else {
		_W("battery level changed abnormal");
//		if (WINDICATOR_ERROR_OK != windicator_moment_view_show(ad,0)) {
//			_E("Failed to show moment view");
//		}
		elm_object_signal_emit(ad->moment_bar_dynamic_layout, "hideBattery,blink", "sw.icon.battery");
		elm_object_signal_emit(ad->moment_view_dynamic_layout, "hideBattery,blink", "sw.icon.battery");

		return;
	}*/
}

static void _battery_charger_status_changed_cb(keynode_t *node, void *data)
{

	_W("");

	struct appdata *ad = data;
	ret_if(ad == NULL);

	int ret = 0;
	ret = vconf_get_int(VCONFKEY_SYSMAN_CHARGER_STATUS, &ad->is_connected);
	if (ret < 0) {
		_SECURE_E("Failed to get vconfkey(%d) : %s", ret, VCONFKEY_SYSMAN_CHARGER_STATUS);
		ad->is_connected = 0;
	}

	_D("battery charge state(%d) TA state(%d)", ad->is_charged, ad->is_connected);

	_charging_ui_update(ad);

}

Evas_Object *windicator_battery_icon_create(Evas_Object *parent, void *data)
{
	retv_if(parent == NULL, NULL);
	_I("moment dynamic view battery initialize");
	struct appdata *ad = (struct appdata *)data;
	retv_if(ad == NULL, NULL);

	Evas_Object *icon = elm_layout_add(parent);
	retv_if(icon == NULL, NULL);

	char full_path[PATH_MAX] = { 0, };
	_get_resource(EDJ_FILE, full_path, sizeof(full_path));
	_D("full_path:%s", full_path);

	if (elm_layout_file_set(icon, full_path, "windicator/battery_icon") != EINA_TRUE) {
		_E("Failed to set battery icon");
		evas_object_del(icon);
		return NULL;
	}

	evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(icon, EVAS_HINT_FILL, EVAS_HINT_FILL);

	elm_object_part_content_set(parent, "sw.icon.battery", icon);

	elm_object_part_text_set(parent, "txt.battery", "50");

	evas_object_show(icon);
	_I("moment dynamic view battery initialize END");

	return icon;
}
windicator_error_e windicator_battery_icon_update(Evas_Object *layout, void *data)
{

	retv_if(layout == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

	struct appdata *ad = (struct appdata *)data;
	retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

	int battery_level = 0;
	int is_battery_display = -1;
	char buf[MAX_PATH_LENGTH] = {0,};
	char buf2[5] = {0,};
	Evas_Object *icon = NULL;
	icon = elm_object_part_content_get(layout, "sw.icon.battery");
	if (icon == NULL) {
		icon = windicator_battery_icon_create(layout, ad);
		if (icon == NULL) {
			_E("Failed to create battery icon");
			return WINDICATOR_ERROR_FAIL;
		}
	}

	/* Battery Text */
	if (device_battery_get_percent(&battery_level) != DEVICE_ERROR_NONE) {
		_E("Battery percent get error");
	}

	vconf_get_bool(VCONFKEY_SETAPPL_BATTERY_PERCENTAGE_BOOL, &is_battery_display);

	if (is_battery_display) {
		char *level = windicator_util_str_from_icu_get(battery_level);
		if (level == NULL) {
			_E("Failed to get string from icu");
			snprintf(buf, sizeof(buf)-1, "%d%%", battery_level);
		} else {
			_W("battery level(%s), length(%d)", level, strlen(level));
			if (strlen(level) > 6) {
				snprintf(buf, sizeof(buf)-1, "%d%%", battery_level);
			} else {
				snprintf(buf, sizeof(buf)-1, "%s%%", level);
			}
		}
		_SECURE_D("the buf is %s", buf);

		elm_object_part_text_set(ad->moment_bar_battery_icon, "txt.battery", buf);

		free(level);
	} else {
		elm_object_part_text_set(ad->moment_bar_battery_icon, "txt.battery", "");

	}

	ad->is_charged = windicator_battery_charge_now_get();
	_W("battery_level: %d, isCharging: %d", battery_level, ad->is_charged);

	/* Battery Icon */
	if (ad->is_charged == 1) {
		snprintf(buf, sizeof(buf)-1, "change_charging_level_");
		elm_object_signal_emit(ad->moment_bar_battery_icon, "Show.Battery.Charge", "img.battery.charge");
	} else if (ad->is_charged == 0) {
		snprintf(buf, sizeof(buf)-1, "change_level_");
		elm_object_signal_emit(ad->moment_bar_battery_icon, "Hide.Battery.Charge", "img.battery.charge");
	} else if (ad->is_charged == -1) {
		_E("Battery charging has problem");
		snprintf(buf, sizeof(buf)-1, "change_level_");
		elm_object_signal_emit(ad->moment_bar_battery_icon, "Hide.Battery.Charge", "img.battery.charge");
	}

	if (battery_level <= 5) {
		battery_level /= 5;
		battery_level *= 5;
	} else {
		battery_level += 4;
		battery_level /= 5;
		battery_level *= 5;
	}

	snprintf(buf2, sizeof(buf2)-1, "%02d", battery_level);
	strncpy(buf, "change_charging_level_", 100);
	strncat(buf, buf2, sizeof(buf)-1);

	if ((ad->is_charged == 1) && (battery_level <= 4)) {
		/* Show "battery_00 */
		elm_object_signal_emit(ad->moment_bar_battery_icon, "hide.img.battery", "img.battery");
	} else {
		/* OPEN */
		elm_object_signal_emit(ad->moment_bar_battery_icon, buf, "img.battery");
	}

	/*ad->pre_charge_state = 0;
	if (ad->is_charged == -1) {
		_W("Abnormal charging status !!");
		if (ad->pre_charge_state != -1)
		{
			elm_object_signal_emit(layout, "showBattery", "sw.icon.battery");
			elm_object_signal_emit(layout, "hideBattery,blink", "sw.icon.battery");
		}
		else
		{
			_W("already blinking");
		}
	}
	else {
		_W("Normal charging status !!");
		elm_object_signal_emit(layout, "showBattery", "sw.icon.battery");
	}
	ad->pre_charge_state = ad->is_charged;*/

	return WINDICATOR_ERROR_OK;
}

void windicator_battery_icon_destroy(void *data)
{
	struct appdata *ad = (struct appdata *)data;
	ret_if(ad == NULL);

	if (ad->moment_bar_battery_icon != NULL) {
		evas_object_del(ad->moment_bar_battery_icon);
		ad->moment_bar_battery_icon = NULL;
	}
}

windicator_error_e windicator_battery_init(void *data)
{

	struct appdata *ad = (struct appdata *)data;
	retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

	/* battery charge connected/disconnected */
	 vconf_get_int(VCONFKEY_SYSMAN_CHARGER_STATUS, &ad->is_connected);
	 ad->is_charged = windicator_battery_charge_now_get();
	 if (ad->is_charged == -1) {
		 _E("battery error");
		 windicator_util_display_lock();
	 }

	 vconf_notify_key_changed(VCONFKEY_SYSMAN_CHARGER_STATUS, _battery_charger_status_changed_cb, ad);

	 vconf_notify_key_changed(VCONFKEY_SYSMAN_BATTERY_CAPACITY, _battery_charging_changed_cb, ad);

	 /* battery fully charged */
	 vconf_notify_key_changed(VCONFKEY_SYSMAN_BATTERY_STATUS_LOW, _battery_status_changed_cb, ad);

	return WINDICATOR_ERROR_OK;
}
void windicator_battery_finish(void)
{
	/* battery charge connected/disconnected */
	vconf_ignore_key_changed(VCONFKEY_SYSMAN_CHARGER_STATUS, _battery_charging_changed_cb);
	vconf_ignore_key_changed(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW, _battery_charging_changed_cb);

	/*  battery fully charged */
	vconf_ignore_key_changed(VCONFKEY_SYSMAN_BATTERY_STATUS_LOW, _battery_status_changed_cb);

}
