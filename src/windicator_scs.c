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
#include "windicator_util.h"
#include "windicator_scs.h"
#include "windicator_dynamic.h"

windicator_error_e windicator_scs_update(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        int sap_connected = 0;
        int headset_connected = 0;
        int isBTOn = 0;

        if (vconf_get_int(VCONFKEY_BT_STATUS, &isBTOn) != 0) {
                _E("vconf_get_int() failed");
                isBTOn = 0;
                goto done;
        }
        _W("isBTOn: %d", isBTOn);

        if(isBTOn)
        {
			if(vconf_get_bool(VCONFKEY_WMS_WMANAGER_CONNECTED, &sap_connected) < 0) {
					_E("Failed to get vconfkey : %s", VCONFKEY_WMS_WMANAGER_CONNECTED);
					sap_connected = 0;
					goto done;
			}
			_W("sap connected : %d", sap_connected);

			if(sap_connected) {
				_W("connected via bluetooth");
				elm_object_signal_emit(ad->scs_layout, "icon,scs,connected,bt", "icon.scs");
				elm_object_domain_translatable_part_text_set(ad->scs_layout, "txt.scs", PROJECT, "WDS_QP_TMBODY_CONNECTED_VIA_BLUETOOTH_ABB");
				return WINDICATOR_ERROR_OK;
			}
		}

done:
		_W("not connected icon");
		elm_object_signal_emit(ad->scs_layout, "icon,scs,not,connected", "icon.scs");
		elm_object_domain_translatable_part_text_set(ad->scs_layout, "txt.scs", PROJECT, "WDS_ST_BODY_STANDALONE_ABB");
		return WINDICATOR_ERROR_OK;
}

Evas_Object *windicator_scs_layout_create(Evas_Object *parent, void *data)
{
        retv_if(parent == NULL, NULL);
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);
        _I("moment bar first layout scs create");

        /* create layout */
        Evas_Object *layout = elm_layout_add(parent);
        retv_if(layout == NULL, NULL);
        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/scs") != EINA_TRUE) {
                _E("Failed to set scs layout");
                evas_object_del(layout);
                return NULL;
        }
        evas_object_size_hint_weight_set (layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);

        elm_object_part_content_set(parent, "sw.main.view", layout);
        evas_object_show(layout);
#ifdef _TIZEN_3G_DISABLE
        elm_object_signal_emit(layout, "txt.sim_state.hide", "txt.sim_state");
#endif
        _I("moment bar first layout scs create END");
        return layout;
}

void windicator_scs_layout_destroy(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        if(ad->scs_layout != NULL) {
                evas_object_del(ad->scs_layout);
                ad->scs_layout = NULL;
        }
}
