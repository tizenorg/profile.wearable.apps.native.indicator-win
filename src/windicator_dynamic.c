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
#include "windicator_dynamic.h"
#include "windicator_battery.h"
#include "windicator_moment_view.h"

#define IMG_HEADSET_PATH IMAGEDIR"/Indicator/indi_headset_on.png"
#define IMG_ALARM_PATH IMAGEDIR"/Indicator/indi_alarm.png"
#define IMG_HEADSET_MOMENT_VIEW_PATH IMAGEDIR"/Indicator/indi_headset_on.png"
#define IMG_CALL_FORWARDING_PATH IMAGEDIR"/Indicator/indi_call_forwarding.png"
#define ICON_EVENT_TYPE "_event_type_"

enum {
        EVENT_TYPE_BLUETOOTH,
        EVENT_TYPE_HEADSET,
        EVENT_TYPE_CALL_FORWARDING,
        EVENT_TYPE_GPS,
        EVENT_TYPE_SOUND,
        EVENT_TYPE_ALARM,
        EVENT_TYPE_MAX,
};
Evas_Object* bluetooth_icon = NULL;
Evas_Object* headset_icon = NULL;
Evas_Object* call_forwarding_icon = NULL;
Evas_Object* gps_icon = NULL;
Evas_Object* sound_icon = NULL;
Evas_Object* alarm_icon = NULL;

Evas_Object* view_bluetooth_icon = NULL;
Evas_Object* view_headset_icon = NULL;
Evas_Object* view_call_forwarding_icon = NULL;
Evas_Object* view_gps_icon = NULL;
Evas_Object* view_sound_icon = NULL;
Evas_Object* view_alarm_icon = NULL;

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
#ifdef _TIZEN_3G_DISABLE
        elm_object_part_content_set(parent, "sw.icon_2", icon);
#else
        elm_object_part_content_set(parent, "sw.icon_1", icon);
#endif
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
#ifdef _TIZEN_3G_DISABLE
        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/dynamic/layout/bt") != EINA_TRUE) {
                _E("Failed to create dynamic layout");
                evas_object_del(layout);
                return NULL;
        }
#else
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
#endif
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

static Evas_Object *_dynamic_event_icon_create(Evas_Object *layout, int event_type, int isForMomentView)
{
        Evas_Object *icon = NULL;
        _D("isForMomentView? (%d)", isForMomentView);

        switch(event_type) {
        case EVENT_TYPE_BLUETOOTH :
                if(isForMomentView == 0 && bluetooth_icon != NULL)
                {
                        _D("bluetooth_icon is already created");
                        return bluetooth_icon;
                }
                else if(isForMomentView == 1 && view_bluetooth_icon != NULL)
                {
                        _D("view_bluetooth_icon is already created");
                        return view_bluetooth_icon;
                }

                _D("Create BLUETOOTH icon");
                icon = elm_layout_add(layout);
                retv_if(icon == NULL, NULL);

                if(isForMomentView == 0) bluetooth_icon = icon;
                else view_bluetooth_icon = icon;

                if(elm_layout_file_set(icon, EDJ_FILE, "windicator/dynamic/bluetooth_icon") != EINA_TRUE) {
                        _E("Failed to set layout file : event_type(%d)", event_type);
                        return NULL;
                }
                evas_object_size_hint_min_set(icon, ELM_SCALE_SIZE(DYNAMIC_ICON_WIDTH), ELM_SCALE_SIZE(DYNAMIC_ICON_HEIGHT));
                break;
        case EVENT_TYPE_HEADSET :
                if(isForMomentView == 0 && headset_icon != NULL)
                {
                        _D("headset_icon is already created");
                        //ea_theme_object_color_set(headset_icon,"AO016");
                        return headset_icon;
                }
                else if(isForMomentView == 1 && view_headset_icon != NULL)
                {
                        _D("view_headset_icon is already created");
                        //ea_theme_object_color_set(view_headset_icon,"AO016");
                        return view_headset_icon;
                }
                _D("Create HEADSET icon");
                icon = elm_image_add(layout);
                retv_if(icon == NULL, NULL);

                if(isForMomentView == 0) headset_icon = icon;
                else view_headset_icon = icon;

                if(elm_image_file_set(icon, IMG_HEADSET_PATH, NULL) != EINA_TRUE) {
                        _E("Failed to set image file : event_type(%d)", event_type);
                        return NULL;
                }
                //ea_theme_object_color_set(icon,"AO016");
                evas_object_size_hint_min_set(icon, ELM_SCALE_SIZE(DYNAMIC_ICON_WIDTH), ELM_SCALE_SIZE(DYNAMIC_ICON_HEIGHT));
                break;
        default :
                _E("Failed to add dynamic icon. type error(%d)", event_type);
                return NULL;
        }

        evas_object_show(icon);
        evas_object_data_set(icon, ICON_EVENT_TYPE, (void *)event_type);

        return icon;
}

void dynamic_event_icon_refresh_position_for_3g(Evas_Object *layout, int isForMomentView)
{
        struct appdata *ad = (struct appdata *)windicator_appdata_get();
        ret_if(ad == NULL);

        Evas_Object *icon = NULL;
        char swallow_name[256] = { 0, };
        int i = 0;

        /* Unset "All" non-fixed icons. (sw.icon_0, sw.icon_1 are Fixed icon. So don't need to check) */
        for(i = 0; i <= 7; i++)
        {
                snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i);
                _D("Try to unset sw.icon_%d 's icon", i);
                icon = elm_object_part_content_unset(layout, swallow_name);
                if(icon != NULL)
                {
                        _D("Hide sw.icon_%d 's icon (%p)", i, icon);
                        evas_object_hide(icon);
                        icon = NULL;
                }
                else
                {
                        _D("There is no icon on sw.icon_%d", i);
                        break;
                }
        }

    	if(isForMomentView == 0)  // For MomentBar.
    	{
    		/* Set icons (icon's pointer was allocated by "_dynamic_event_icon_create()". */
    		int b_jump_connection = 0;
    		i = 0;

    		/* [P150903-04314] Only for 3G */
    		/* Check RSSI, Connection */
    		_E("rssi %d connection %d",ad->rssi_hide,ad->connection_hide);

    		if(ad->rssi_hide == 0 && ad->connection_hide == 0)
    		{
    			i = 2;
    			elm_object_part_content_set(layout, "sw.icon_0",ad->moment_bar_rssi_icon);
    			elm_object_part_content_set(layout, "sw.icon_1",ad->moment_bar_connection_icon);

    		}
    		else if(ad->rssi_hide == 1 && ad->connection_hide == 0)
    		{
    			i = 0;
    			b_jump_connection = 1;
    			elm_object_part_content_set(ad->moment_bar_dynamic_layout, "sw.icon_1",ad->moment_bar_connection_icon);
    		}

    		else if(ad->rssi_hide == 0 && ad->connection_hide == 1)
    		{
    			i = 1;
    			elm_object_part_content_set(layout, "sw.icon_0",ad->moment_bar_rssi_icon);
    		}
    		else
    		{
    			i = 0;
    		}

    		_D("Set icon from (sw.icon_%d)", i);
    		if(bluetooth_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Bluetooth icon ==> %s (%p)", swallow_name, bluetooth_icon);
    			elm_object_part_content_set(layout, swallow_name, bluetooth_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if(headset_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Headset icon ==> %s (%p)", swallow_name, headset_icon);
    			elm_object_part_content_set(layout, swallow_name, headset_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if(call_forwarding_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Call forwarding icon ==> %s (%p)", swallow_name, call_forwarding_icon);
    			elm_object_part_content_set(layout, swallow_name, call_forwarding_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if (gps_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Gps icon ==> %s (%p)", swallow_name, gps_icon);
    			elm_object_part_content_set(layout, swallow_name, gps_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if (sound_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Sound icon ==> %s (%p)", swallow_name, sound_icon);
    			elm_object_part_content_set(layout, swallow_name, sound_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if (alarm_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Alarm icon ==> %s (%p)", swallow_name, alarm_icon);
    			elm_object_part_content_set(layout, swallow_name, alarm_icon);
    		}

    	}
    	else   // For MomentView.
    	{
    		// Set icons (icon's pointer was allocated by "_dynamic_event_icon_create()".

    		int b_jump_connection = 0;
    		i = 0;

    		/* [P150903-04314] Only for 3G */
    		/* Check RSSI, Connection */
    		_E("rssi %d connection %d",ad->rssi_hide,ad->connection_hide);

    		if(ad->rssi_hide == 0 && ad->connection_hide == 0)
    		{
    			i = 2;
    			elm_object_part_content_set(layout, "sw.icon_1",ad->moment_view_connection_icon);
    			elm_object_part_content_set(layout, "sw.icon_0",ad->moment_view_rssi_icon);

    		}
    		else if(ad->rssi_hide == 1 && ad->connection_hide == 0)
    		{
    			i = 0;
    			b_jump_connection = 1;
    			elm_object_part_content_set(layout, "sw.icon_1",ad->moment_view_connection_icon);
    		}

    		else if(ad->rssi_hide == 0 && ad->connection_hide == 1)
    		{
    			i = 1;
    			elm_object_part_content_set(layout, "sw.icon_0",ad->moment_view_rssi_icon);
    		}
    		else
    		{
    			i = 0;
    		}

    		if(view_bluetooth_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Bluetooth icon ==> %s (%p)", swallow_name, view_bluetooth_icon);
    			elm_object_part_content_set(layout, swallow_name, view_bluetooth_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if(view_headset_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Headset icon ==> %s (%p)", swallow_name, view_headset_icon);
    			elm_object_part_content_set(layout, swallow_name, view_headset_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if(view_call_forwarding_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Call forwarding icon ==> %s (%p)", swallow_name, view_call_forwarding_icon);
    			elm_object_part_content_set(layout, swallow_name, view_call_forwarding_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if (view_gps_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Gps icon ==> %s (%p)", swallow_name, view_gps_icon);
    			elm_object_part_content_set(layout, swallow_name, view_gps_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if (view_sound_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Sound icon ==> %s (%p)", swallow_name, view_sound_icon);
    			elm_object_part_content_set(layout, swallow_name, view_sound_icon);
    		}

    		if(i==1&&b_jump_connection==1)
    		{
    			i++;
    			b_jump_connection = 0;
    		}

    		if (view_alarm_icon != NULL)
    		{
    			snprintf(swallow_name, sizeof(swallow_name)-1, "sw.icon_%d", i++);
    			_D("Show Alarm icon ==> %s (%p)", swallow_name, view_alarm_icon);
    			elm_object_part_content_set(layout, swallow_name, view_alarm_icon);
    		}
    	}
    	_D("Refresh DONE");
    }
static void _dynamic_event_item_pack(int event_type, int isForMomentView, void *data)
{
        Evas_Object *layout = (Evas_Object *)data;
        ret_if(layout == NULL);

        _D("layout(%p), IsforMomentView? %d", layout, isForMomentView);

        Evas_Object *item = _dynamic_event_icon_create(layout, event_type, isForMomentView);
        ret_if(item == NULL);

#ifdef _TIZEN_3G_DISABLE
        struct appdata *ad = (struct appdata *)windicator_appdata_get();
        ret_if(ad == NULL);

        switch(event_type) {
        case EVENT_TYPE_BLUETOOTH :
                _D("Show Bluetooth icon");
                elm_object_part_content_set(layout, "sw.icon_0", item);
                elm_object_signal_emit(layout, "show.icon_0", "sw.icon_0");

                /* Move wi-fi icon's position */
                elm_object_signal_emit(ad->moment_bar_dynamic_layout, "WiFi.move.1", "sw.icon_2");
                elm_object_signal_emit(ad->moment_view_dynamic_layout, "WiFi.move.1", "sw.icon_2");

//              elm_object_signal_emit(ad->moment_view_dynamic_layout, "odd.icon.position", "sw.icon_0");
                break;
        case EVENT_TYPE_HEADSET :
                _D("Show Headset icon");
                elm_object_part_content_set(layout, "sw.icon_1", item);
                elm_object_signal_emit(layout, "show.icon_1", "sw.icon_1");

                /* Move wi-fi icon's position */
                elm_object_signal_emit(ad->moment_bar_dynamic_layout, "WiFi.move.2", "sw.icon_2");
                elm_object_signal_emit(ad->moment_view_dynamic_layout, "WiFi.move.2", "sw.icon_2");

//              elm_object_signal_emit(ad->c, "even.icon.position", "sw.icon_0");
                break;
        default :
                _E("Failed to add dynamic icon. type error(%d)", event_type);
                evas_object_del(item);
                return;
        }
#else
        dynamic_event_icon_refresh_position_for_3g(layout, isForMomentView);
#endif
}

static void _dynamic_event_icon_del(Evas_Object *item, int event_type)
{
        ret_if(item == NULL);

        _D("(Delete icon : %p)", item);
        evas_object_data_del(item, ICON_EVENT_TYPE);
        evas_object_del(item);
        item = NULL;
}

static void _dynamic_event_item_unpack(int event_type, int isForMomentView, void *data)
{
	Evas_Object *layout = (Evas_Object *)data;
	ret_if(layout == NULL);

	_D("layout(%p)", layout);
	Evas_Object *icon = NULL;

#ifdef _TIZEN_3G_DISABLE
	struct appdata *ad = (struct appdata *)windicator_appdata_get();
	ret_if(ad == NULL);

	switch(event_type) {
	case EVENT_TYPE_BLUETOOTH :
		_D("Hide Bluetooth icon : layout(%p)", layout);
		elm_object_signal_emit(layout, "hide.icon_0", "sw.icon_0");
		icon = elm_object_part_content_get(layout, "sw.icon_0");
		_dynamic_event_icon_del(icon, event_type);
		if(isForMomentView == 0) bluetooth_icon = NULL;
		else view_bluetooth_icon = NULL;

		/* Move wi-fi icon's position */
		elm_object_signal_emit(ad->moment_bar_dynamic_layout, "WiFi.move.0", "sw.icon_2");
		elm_object_signal_emit(ad->moment_view_dynamic_layout, "WiFi.move.0", "sw.icon_2");

		break;
	case EVENT_TYPE_HEADSET :
		_D("Hide Headset icon : layout(%p)", layout);
		elm_object_signal_emit(layout, "hide.icon_1", "sw.icon_1");
		icon = elm_object_part_content_get(layout, "sw.icon_1");
		_dynamic_event_icon_del(icon, event_type);
		if(isForMomentView == 0) headset_icon = NULL;
		else view_headset_icon = NULL;

		/* Move wi-fi icon's position */
		if(bluetooth_icon)
		{
			elm_object_signal_emit(ad->moment_bar_dynamic_layout, "WiFi.move.1", "sw.icon_2");
			elm_object_signal_emit(ad->moment_view_dynamic_layout, "WiFi.move.1", "sw.icon_2");
		}
		else
		{
			elm_object_signal_emit(ad->moment_bar_dynamic_layout, "WiFi.move.0", "sw.icon_2");
			elm_object_signal_emit(ad->moment_view_dynamic_layout, "WiFi.move.0", "sw.icon_2");
		}
//		elm_object_signal_emit(ad->moment_view_dynamic_layout, "odd.icon.position", "sw.icon_0");

		break;
	default :
		_E("Failed to remove dynamic icon. type error(%d)", event_type);
		return;
	}

#else
	switch(event_type) {
	case EVENT_TYPE_BLUETOOTH :
		_D("Hide Bluetooth icon : layout(%p)", layout);
		_dynamic_event_icon_del(bluetooth_icon, event_type);
		if(isForMomentView == 0) bluetooth_icon = NULL;
		else view_bluetooth_icon = NULL;

		break;
	case EVENT_TYPE_HEADSET :
		_D("Hide Headset icon : layout(%p)", layout);
		_dynamic_event_icon_del(headset_icon, event_type);
		if(isForMomentView == 0) headset_icon = NULL;
		else view_headset_icon = NULL;

		break;
	default :
		_E("Failed to remove dynamic icon. type error(%d)", event_type);
		return;
	}
	dynamic_event_icon_refresh_position_for_3g(layout, isForMomentView);
#endif
}

static void _dynamic_bluetooth_state_changed_cb(void *node, void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        int isBTOn = 0;
        int sap_connected = 0;
        int bt_bonded = 0;

        if(vconf_get_int(VCONFKEY_BT_STATUS, &isBTOn) < 0) {
                _E("Failed to get vconfkey : %s", VCONFKEY_BT_STATUS);
                isBTOn = 0;
        }

        _W("isBTOn : %d", isBTOn);

        if(vconf_get_int("memory/wms/wmanager_connected", &sap_connected) < 0) {
                _SECURE_E("Failed to get vconfkey : %s", VCONFKEY_WMS_WMANAGER_CONNECTED);
                sap_connected = 0;
        }
        _W("sap connected : %d", sap_connected);

        if(isBTOn) {
                _dynamic_event_item_pack(EVENT_TYPE_BLUETOOTH, 0, ad->moment_bar_dynamic_layout);
                _dynamic_event_item_pack(EVENT_TYPE_BLUETOOTH, 1, ad->moment_view_dynamic_layout);

                Evas_Object *moment_bar_bt_icon = bluetooth_icon;

                if(moment_bar_bt_icon == NULL) {
                        _E("Failed to get moment bar bt icon");
                }

                Evas_Object *moment_view_bt_icon = view_bluetooth_icon;

                if(moment_view_bt_icon == NULL) {
                        _E("Failed to get moment view bt icon");
                }
                if(vconf_get_int(VCONFKEY_BT_DEVICE, &bt_bonded) < 0) {
                        _SECURE_E("Failed to get vconfkey : %s", VCONFKEY_BT_DEVICE);
                        bt_bonded = 0;
                }

                if(sap_connected) {
						_W("display connected BT icon");
						elm_object_signal_emit(moment_bar_bt_icon, "showConnectedBTicon", "img.bluetooth");
						elm_object_signal_emit(moment_view_bt_icon, "showConnectedBTicon", "img.bluetooth");
                } else {
                        _D("display unconnected BT icon");
                        elm_object_signal_emit(moment_bar_bt_icon, "showUnconnectedBTicon", "img.bluetooth");
                        elm_object_signal_emit(moment_view_bt_icon, "showUnconnectedBTicon", "img.bluetooth");
                }
        } else {
                _D("remove a BT icon");
                _dynamic_event_item_unpack(EVENT_TYPE_BLUETOOTH, 0, ad->moment_bar_dynamic_layout);
                _dynamic_event_item_unpack(EVENT_TYPE_BLUETOOTH, 1, ad->moment_view_dynamic_layout);
        }


}

static void _dynamic_headset_state_changed_cb(keynode_t *node, void *data)
{
        _D("");
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        /* headset */
        int isBTOn = 0;
        int bt_bonded = 0;
        if(vconf_get_int(VCONFKEY_BT_STATUS, &isBTOn) != 0) {
                _E("Failed to get vconfkey : %s", VCONFKEY_BT_STATUS);
                isBTOn = 0;
        }
        _D("isBTOn : %d", isBTOn);

        if(isBTOn) {
                /* headset */
                if(vconf_get_int(VCONFKEY_BT_DEVICE, &bt_bonded) < 0) {
                        _SECURE_E("Failed to get vconfkey : %s", VCONFKEY_BT_DEVICE);
                        bt_bonded = 0;
                }

                //bt_bonded &= VCONFKEY_BT_DEVICE_A2DP_HEADSET_CONNECTED;
                if(bt_bonded & VCONFKEY_BT_DEVICE_A2DP_HEADSET_CONNECTED || bt_bonded & VCONFKEY_BT_DEVICE_HEADSET_CONNECTED) {
                        _D("headset is connected. add headset icon");
                        _dynamic_event_item_pack(EVENT_TYPE_HEADSET, 0, ad->moment_bar_dynamic_layout);
                        _dynamic_event_item_pack(EVENT_TYPE_HEADSET, 1, ad->moment_view_dynamic_layout);
                } else {
                        _D("headset is not connected. remove headset icon");
                        _dynamic_event_item_unpack(EVENT_TYPE_HEADSET, 0, ad->moment_bar_dynamic_layout);
                        _dynamic_event_item_unpack(EVENT_TYPE_HEADSET, 1, ad->moment_view_dynamic_layout);
                }
        } else {
                _D("remove a headset icon");
                _dynamic_event_item_unpack(EVENT_TYPE_HEADSET, 0, ad->moment_bar_dynamic_layout);
                _dynamic_event_item_unpack(EVENT_TYPE_HEADSET, 1, ad->moment_view_dynamic_layout);
        }
}

windicator_error_e windicator_dynamic_update(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);
        _D("");

        _dynamic_bluetooth_state_changed_cb(NULL, ad);
        _dynamic_headset_state_changed_cb(NULL, ad);

        return WINDICATOR_ERROR_OK;
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


windicator_error_e windicator_dynamic_vconfkey_register(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        /* check BT for Bluetooth icon */
        _dynamic_bluetooth_state_changed_cb(NULL, ad);
        vconf_notify_key_changed(VCONFKEY_BT_STATUS, (vconf_callback_fn)_dynamic_bluetooth_state_changed_cb, ad);
        vconf_notify_key_changed(VCONFKEY_WMS_WMANAGER_CONNECTED, (vconf_callback_fn)_dynamic_bluetooth_state_changed_cb, ad);

        /* check BT for Headset icon */
        _dynamic_headset_state_changed_cb(NULL, ad);
        vconf_notify_key_changed(VCONFKEY_BT_STATUS, (vconf_callback_fn)_dynamic_headset_state_changed_cb, ad);
        vconf_notify_key_changed(VCONFKEY_BT_DEVICE, (vconf_callback_fn)_dynamic_headset_state_changed_cb, ad);

        return WINDICATOR_ERROR_OK;
}

void windicator_dynamic_vconfkey_unregister(void)
{
        /* ignore BT for Bluetooth icon */
        vconf_ignore_key_changed(VCONFKEY_BT_STATUS, (vconf_callback_fn)_dynamic_bluetooth_state_changed_cb);
        vconf_ignore_key_changed(VCONFKEY_WMS_WMANAGER_CONNECTED, (vconf_callback_fn)_dynamic_bluetooth_state_changed_cb);

        /* ignore BT for Headset icon */
        vconf_ignore_key_changed(VCONFKEY_BT_STATUS, (vconf_callback_fn)_dynamic_headset_state_changed_cb);
        vconf_ignore_key_changed(VCONFKEY_BT_DEVICE, (vconf_callback_fn)_dynamic_headset_state_changed_cb);
}
