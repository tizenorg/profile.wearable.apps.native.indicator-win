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

//#include "csc-feature.h"
#include "windicator.h"
#include "log.h"
#include "windicator_connection.h"
#include "windicator_dynamic.h"
//#include "windicator_sim_state.h"

typedef enum {
        PACKET_NONE = 0x00,
        PACKET_IN,
        PACKET_OUT,
        PACKET_INOUT,
        PACKET_MAX,
} packet_type_e;

typedef enum {
        TYPE_NONE = 0x00,
        TYPE_EDGE_E,
        TYPE_GPRS_G,
        TYPE_2G,
        TYPE_HSDPA_H,
        TYPE_HSPA_H_PLUS,
        TYPE_LTE_4G,
        TYPE_UMTS_3G,
        TYPE_UMTS_3G_PLUS,
        TYPE_NO_SIM,
        TYPE_WIFI_00,
        TYPE_WIFI_01,
        TYPE_WIFI_02,
        TYPE_WIFI_03,
        TYPE_WIFI_04,
        TYPE_NO_SIGNAL,
        TYPE_CONN_MAX,
} connection_type_e;

static const char *packet_name[PACKET_MAX] = {
        "packet_no_inout_connected",
        "packet_in_connected",
        "packet_out_connected",
        "packet_inout_connected",
};

static const char *type_name[TYPE_CONN_MAX] = {
        "hide,connection_icon",             // Hide Connection icon.
        "type_edge_connected",
        "type_gprs_connected",
        "type_2g_connected",
        "type_hsdpa_connected",
        "type_hspa_connected",
        "type_lte_connected",
        "type_umts_connected",
        "type_3g_plus_connected",
        "type_no_sim",
        "type_wifi_connected_00",
        "type_wifi_connected_01",
        "type_wifi_connected_02",
        "type_wifi_connected_03",
        "type_wifi_connected_04",
        "type_no_signal",
};

static struct info {
        int conn_type;
    	int packet_type;
} s_info = {
        .conn_type = TYPE_NONE,
    	.packet_type = PACKET_NONE,
};

#if 0
enum {
        RSSI_NONE = 0x00,
        RSSI_LEVEL_0 = RSSI_NONE,
        RSSI_LEVEL_1,
        RSSI_LEVEL_2,
        RSSI_LEVEL_3,
        RSSI_LEVEL_4,
        RSSI_LEVEL_5,
        RSSI_LEVEL_6,
        RSSI_FLIGHT,
        RSSI_NO_SIGNAL,
        RSSI_HIDE,                         // Hide RSSI icon.
        RSSI_MAX,
};

static void _rssi_icon_set(Evas_Object *icon)
{
        struct appdata *ad = (struct appdata *)windicator_appdata_get();
        ret_if(ad == NULL);

        ret_if(icon == NULL);

        if(s_info.rssi_level >= RSSI_MAX) {
                _E("Rssi level error (%d)", s_info.rssi_level);
                return;
        }

        _W("RSSI level : %d/%d, (%p)", s_info.rssi_level, RSSI_ANT_BAR, icon);
        if(s_info.rssi_level == RSSI_HIDE) {
                _E("Set RSSI HIDE sw.icon_0 (rssi_level : %d) (rssi_hide : %d)", s_info.rssi_level,ad->rssi_hide);
                ad->rssi_hide = 1;
        }
        else
        {
                _E("Set RSSI SHOW sw.icon_0 (rssi_level : %d) (rssi_hide : %d)(%x)", s_info.rssi_level,ad->rssi_hide,ad->moment_bar_rssi_icon);
                ad->rssi_hide = 0;
        }
        if(rssi_name[s_info.rssi_level][s_info.is_roaming] == NULL) {
                _E("Failed to get rssi name. Set NO_SIGNAL icon : name : %s, (%p)", rssi_name[RSSI_NO_SIGNAL][0], icon);
                elm_object_signal_emit(icon, rssi_name[RSSI_NO_SIGNAL][0], "img.rssi");
                return;
        }

        _W("rssi name : %s (%p)", rssi_name[s_info.rssi_level][s_info.is_roaming], icon);
        elm_object_signal_emit(icon, rssi_name[s_info.rssi_level][s_info.is_roaming], "img.rssi");
}

static void _rssi_changed_cb(keynode_t *node, void *data)
{
        int status = 0;
        int ret = 0;

        Evas_Object *rssi_icon = (Evas_Object*) data;
        ret_if(rssi_icon == NULL);

        /* flight mode */
        ret = vconf_get_bool(VCONFKEY_TELEPHONY_FLIGHT_MODE, &status);
        if(ret == 0 && status == true) {
                _D("RSSI status : Flight mode");

                /* set flight mode icon */
                s_info.rssi_level = RSSI_FLIGHT;
                _rssi_icon_set(rssi_icon);

                return;
        }

        ret = vconf_get_int(VCONFKEY_TELEPHONY_SVC_ROAM, &status);
        if(ret == 0) {
                _D("roaming status : %d", status);
                s_info.is_roaming = status;
        }

        /* RSSI level */
        ret = vconf_get_int(VCONFKEY_TELEPHONY_RSSI, &status);
        if(ret == 0) {
                _D("rssi level : %d", status);
                /* set rssi icon */
                s_info.rssi_level = status;
        }

        _rssi_icon_set(rssi_icon);
}
#endif

static void _packet_icon_set(packet_type_e packet, void *data)
{
        Evas_Object *connect_icon = (Evas_Object *)data;
        ret_if(connect_icon == NULL);

        if(s_info.conn_type == TYPE_NONE) {
                _E("connection type : TYPE_NONE");
                return;
        } else if(s_info.conn_type == TYPE_NO_SIM) {
                _D("No SIM MODE");
                elm_object_signal_emit(connect_icon, "hide,packet_icon", "img.connection_packet");
                return;
        }

        _W("packet : %d / signal : %s", packet, packet_name[packet]);

        elm_object_signal_emit(connect_icon, packet_name[packet], "img.connection_packet");
}

static void _packet_type_changed_cb(keynode_t *node, void *data)
{
        _D("_packet_type_changed_cb");
        int state = 0;
        int type = PACKET_NONE;

        Evas_Object *connect_icon = (Evas_Object *)data;
        ret_if(connect_icon == NULL);

        /* Check Wifi */
        if(s_info.conn_type > TYPE_NO_SIM) {
                _E("WIFI MODE");
                return;
        }

        if(s_info.conn_type == TYPE_NONE) {
                _E("connection type : TYPE_NONE");
                return;
        }

        if(vconf_get_int(VCONFKEY_PACKET_STATE, &state) < 0) {
                _E("Failed to get vconfkey : %s", VCONFKEY_PACKET_STATE);
                state = VCONFKEY_PACKET_NORMAL;
        }
        _I("packet state : %d", state);

        switch(state) {
        case VCONFKEY_PACKET_RX :
                type = PACKET_IN;
                break;
        case VCONFKEY_PACKET_TX :
                type = PACKET_OUT;
                break;
        case VCONFKEY_PACKET_RXTX :
                type = PACKET_INOUT;
                break;
        case VCONFKEY_PACKET_NORMAL :
        default :
                type = PACKET_NONE;
                break;
        }

        s_info.packet_type = type;
        _packet_icon_set(type, connect_icon);
}

static void _connection_icon_set(connection_type_e type, void *data)
{
        struct appdata *ad = (struct appdata *)windicator_appdata_get();
        ret_if(ad == NULL);

        Evas_Object *connect_icon = (Evas_Object *)data;
        ret_if(connect_icon == NULL);

        _W("type : %d / signal : %s", type, type_name[type]);

        if(type == TYPE_NONE) {
                if(ad->connection_hide == 1)
                {
                        ad->connection_hide = 1;
                        _E("Set Connection (TYPE_NONE), hide sw.icon_1 (type : %d) / (hide : %d)", type, ad->connection_hide);
                }
                else
                {
                        _W("Connection status changed");
                        ad->connection_hide = 1;
                        _E("Set Connection (TYPE_NONE), hide sw.icon_1 (type : %d) / (hide : %d)", type, ad->connection_hide);
                        dynamic_event_icon_refresh_position_for_3g(ad->moment_bar_dynamic_layout, 0);
                }
        }
        else
        {
                if(ad->connection_hide == 0)
                {
                        ad->connection_hide = 0;
                        _E("Set Connection / show sw.icon_1 (type : %d) / (hide : %d)", type, ad->connection_hide);
                }
                else
                {
                        _W("Connection status changed");
                        ad->connection_hide = 0;
                        _E("Set Connection / show sw.icon_1 (type : %d) / (hide : %d)", type, ad->connection_hide);
                        dynamic_event_icon_refresh_position_for_3g(ad->moment_bar_dynamic_layout, 0);
                }
        }
        elm_object_signal_emit(connect_icon, type_name[type], "img.connection_type");
        _packet_type_changed_cb(NULL, connect_icon);
}

static void _wifi_state_changed_cb(keynode_t *node, void *data)
{
	Evas_Object *connect_icon = (Evas_Object *)data;
	ret_if(connect_icon == NULL);

	int wifi_state = 0;
	int wifi_strength = 0;
	int connection_type = 0;

	if(vconf_get_int(VCONFKEY_WIFI_STATE, &wifi_state) < 0) {
		_SECURE_E("Failed to get vconfkey : %s", VCONFKEY_WIFI_STATE);
		return;
	}
	_I("wifi state : %d", wifi_state);

#ifdef _TIZEN_3G_DISABLE
	if(wifi_state != VCONFKEY_WIFI_CONNECTED)
	{
		elm_object_signal_emit(connect_icon, "hide,connection_icon", "img.connection_type");
		return;
	}
#endif

	ret_if(wifi_state != VCONFKEY_WIFI_CONNECTED);

	/* set connection_type */
	if(vconf_get_int(VCONFKEY_WIFI_STRENGTH, &wifi_strength) < 0) {
		_SECURE_E("Failed to get vconfkey : %s", VCONFKEY_WIFI_STRENGTH);
		return;
	}

	_D("wifi strength : %d", wifi_strength);

	switch(wifi_strength) {
	case 0:
		connection_type = TYPE_WIFI_00;
		break;
	case 1:
		connection_type = TYPE_WIFI_01;
		break;
	case 2:
		connection_type = TYPE_WIFI_02;
		break;
	case 3:
		connection_type = TYPE_WIFI_03;
		break;
	case 4:
		connection_type = TYPE_WIFI_04;
		break;
	default:
		_E("Failed to set connection icon(%d)", wifi_strength);
		connection_type = TYPE_WIFI_00;
		break;
	}
	s_info.conn_type = connection_type;
	_connection_icon_set(s_info.conn_type, connect_icon);

	int transfer_state = 0;
	int packet_type = 0;

	/* set connection_inout */
	if(vconf_get_int(VCONFKEY_WIFI_TRANSFER_STATE, &transfer_state) < 0) {
		_SECURE_E("Failed to get vconfkey : %s", VCONFKEY_WIFI_TRANSFER_STATE);
		return;
	}
	_D("wifi transfer state : %d", transfer_state);

	switch(transfer_state) {
	case VCONFKEY_WIFI_TRANSFER_STATE_NONE:
		packet_type = PACKET_NONE;
		break;
	case VCONFKEY_WIFI_TRANSFER_STATE_RX: //DOWN
		packet_type = PACKET_IN;
		break;
	case VCONFKEY_WIFI_TRANSFER_STATE_TX: //UP
		packet_type = PACKET_OUT;
		break;
	case VCONFKEY_WIFI_TRANSFER_STATE_TXRX: //UPDOWN
		packet_type = PACKET_INOUT;
		break;
	default:
		_E("Failed to set transfer state : type error(%d)", transfer_state);
		packet_type = PACKET_NONE;
		return;
	}

	s_info.packet_type = packet_type;
	_packet_icon_set(packet_type, connect_icon);
}

void windi_connection_update(Evas_Object *rssi_icon, Evas_Object *connect_icon)
{
        _D("windi_connection_update");
        /* check rssi level */
        /* check connection status */
        _wifi_state_changed_cb(NULL, connect_icon);
}
