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
#include "windicator_moment_bar.h"
#include "windicator_moment_view.h"
#include "windicator_util.h"
#include "windicator_call_fwd_btn.h"
#include "windicator_music_btn.h"
#include "windicator_dnd_btn.h"
#include "windicator_flight_mode.h"
#include "windicator_brightness.h"
#include "windicator_volume.h"
#include "windicator_dynamic.h"
#include "windicator_battery.h"
#include "windicator_scs.h"
#include "windicator_connection.h"

#define KEY_BACK "XF86Back"

Evas_Object *windicator_moment_bar_first_page_layout_create(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, NULL);
        _I("moment bar first layout create");

        /* create the first page layout */
        Evas_Object *layout = elm_layout_add(ad->moment_bar_conformant);

        retv_if(layout == NULL, NULL);
        _I("main %x first %x",ad->moment_bar_main_layout,layout );
#ifdef _TIZEN_3G_DISABLE
        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/moment_bar/first_page/bt") != EINA_TRUE) {
                _E("Failed to create moment bar first page layout");
                evas_object_del(layout);
                return NULL;
        }
#else
        if(elm_layout_file_set(layout, EDJ_FILE, "windicator/moment_bar/first_page/3g") != EINA_TRUE) {
        	_E("Failed to create moment bar first page layout");
                evas_object_del(layout);
                return NULL;
        }

        //elm_object_signal_callback_add(layout, "mouse,clicked,1", "img.volume.touch.area", on_clicked_small_volume_icon, ad);
        //elm_object_signal_callback_add(layout, "mouse,clicked,1", "img.callfwd.touch.area", _call_fwd_btn_clicked_cb, ad);
#endif
        /* 2015-09-01 */
        elm_object_signal_callback_add(layout, "mouse,clicked,1", "img.brightness.touch.area", on_clicked_small_brightness_icon, ad);
        elm_object_signal_callback_add(layout, "mouse,clicked,1", "img.dnd.touch.area", _dnd_btn_clicked_cb, ad);

        /* 2016-02-01 */
        elm_object_signal_callback_add(layout, "mouse,clicked,1", "img.flight.touch.area", _flight_mode_clicked_cb, ad);

        elm_object_signal_callback_add(layout, "mouse,clicked,1", "img.music.touch.area", _music_btn_clicked_cb, ad);


//      ecore_event_handler_add(ECORE_EVENT_MOUSE_BUTTON_DOWN, windicator_moment_bar_rotary_icon_clicked_cb, ad);
        evas_object_resize(layout, ad->win_w, ad->win_h);
        ad->scs_layout = windicator_scs_layout_create(layout, ad);
        if(ad->scs_layout == NULL) {
        		_E("Failed to create scs icon");
        }

        /* create dynamic layout */
        ad->moment_bar_dynamic_layout = windicator_dynamic_layout_create(layout, ad);
        if(ad->moment_bar_dynamic_layout == NULL) {
        		_E("Failed to create dynamic layout");
        }

        /* create battery icon */
        ad->moment_bar_battery_icon = windicator_battery_icon_create(ad->moment_bar_dynamic_layout, ad);
        if(ad->moment_bar_battery_icon == NULL) {
        		_E("Failed to create battery icon");
        }

        ad->dnd_btn_layout = windicator_dnd_btn_layout_create(layout,ad);
        if(ad->dnd_btn_layout == NULL) {
        		_E("Failed to create dnd layout");
        }

        ad->flight_mode_layout = windicator_flight_mode_layout_create(layout,ad);
        if(ad->flight_mode_layout == NULL) {
        		_E("Failed to create flight_mode layout");
        }

        ad->music_btn_layout = windicator_music_btn_layout_create(layout,ad);
        if(ad->music_btn_layout == NULL) {
        		_E("Failed to create music layout");
        }

        ad->brightness_small_layout = windicator_brightness_small_layout_create(layout,ad);
        if(ad->brightness_small_layout == NULL) {
        		_E("Failed to create brightness layout");
        }

#ifndef _TIZEN_3G_DISABLE
        ad->volume_small_icon = windicator_volume_small_icon_create(layout,ad);
        if(ad->volume_small_icon == NULL) {
        		_E("Failed to create volume layout");
        }
        ad->callfwd_btn_layout = windicator_callfwd_btn_layout_create(layout,ad);
        if(ad->callfwd_btn_layout == NULL) {
        		_E("Failed to create callfwd layout");
        }
#endif

        evas_object_show(layout);
        _I("moment bar first layout create END");
        return layout;
}

windicator_error_e windicator_moment_bar_update_main_view(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);
        _I("moment bar update main view start");
        windicator_brightness_update(ad);
        windicator_music_btn_update(ad);
        windicator_dnd_btn_update(ad);
        windicator_flight_mode_update(ad);
#ifndef _TIZEN_3G_DISABLE
        windicator_volume_update(ad);
        windicator_call_fwd_btn_update(ad);
#endif
        if(WINDICATOR_ERROR_OK != windicator_scs_update(ad)) {
        		_E("Failed to update scs status");
        }
        _I("moment bar update main view end");
        return WINDICATOR_ERROR_OK;
}

static Eina_Bool _key_release_event_cb(void *data, int type, void *event)
{
         struct appdata *ad = (struct appdata *)data;
         retv_if(ad == NULL, EINA_FALSE);

         Evas_Event_Key_Up *ev = event;
         retv_if(ev == NULL, EINA_FALSE);

         if(!strcmp(ev->keyname, KEY_BACK))
         {
             {
                     if(ad->is_getting_back == 0)
                     {
                             _D("Moment bar status -> idle. (Hide Moment bar)");
                             ad->is_getting_back = 1; // ad->is_getting_back will be set 0 in moment_bar_show
                             //windicator_hide_moment_bar_directly(ad);
                             //elm_exit() is bad to use it here. Replace it asap once alternative way is found.
                             elm_exit();
                     }
                     else
                     {
                             _D("Back key animation is operating, so skip back key event");
                     }

             }
     }

     return EINA_FALSE;
}

windicator_error_e windicator_util_back_key_grab(void *data)
{
        _D("");

        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        if(ad->back_key_handler == NULL) {
                ad->back_key_handler = ecore_event_handler_add(ECORE_EVENT_KEY_UP, _key_release_event_cb, ad);
                if(ad->back_key_handler == NULL) {
                		_E("Failed to add back key handler");
                        return WINDICATOR_ERROR_FAIL;
                }
        }

        return WINDICATOR_ERROR_OK;
}

windicator_error_e windicator_moment_bar_show(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);
        _I("Show Moment Bar : dynamic_layout(%p)", ad->moment_bar_dynamic_layout);

        ad->is_getting_back = 0;

        /* update LO layout */
        windicator_lo_update(ad);
        /* register back key event */
        if(WINDICATOR_ERROR_OK != windicator_util_back_key_grab(ad)) {
                _E("Failed to register back key handler");
        }

        evas_object_show(ad->moment_bar_win);

        return WINDICATOR_ERROR_OK;
}

windicator_error_e windicator_show_moment_bar_directly(void* data)
{


        _D("windicator_show_moment_bar_directly");
        struct appdata *ad = (struct appdata *)data;

        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        //Show the indicator window
        return WINDICATOR_ERROR_OK;
}

windicator_error_e windicator_hide_moment_bar_directly(void* data)
{
        _D("windicator_hide_moment_bar_directly");
        struct appdata *ad = (struct appdata *)data;

        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);

        //Hide the indicator window
        return WINDICATOR_ERROR_OK;
}

windicator_error_e windicator_moment_bar_init(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        retv_if(ad == NULL, WINDICATOR_ERROR_INVALID_PARAMETER);
        /* create moment bar window */
        _I("initialize moment bar");
        //ea_theme_event_callback_add(EA_THEME_CALLBACK_TYPE_COLOR, change_theme_callback, (void*)ad);

        ad->moment_bar_win = windicator_util_win_create("__MOMENT_BAR__", EINA_TRUE, TYPE_MOMENT_BAR);
        retv_if(ad->moment_bar_win == NULL, WINDICATOR_ERROR_FAIL);

        ad->moment_bar_evas = evas_object_evas_get(ad->moment_bar_win);
        ad->moment_bar_ee = ecore_evas_ecore_evas_get(ad->moment_bar_evas);

        /* create moment bar bg */
        ad->moment_bar_bg = windicator_util_bg_create(ad->moment_bar_win);
        retv_if(ad->moment_bar_bg == NULL, WINDICATOR_ERROR_FAIL);

        /* create moment bar conformant */
        ad->moment_bar_conformant = windicator_util_conformant_create(ad->moment_bar_win);
        retv_if(ad->moment_bar_conformant == NULL, WINDICATOR_ERROR_FAIL);

        // Eext Circle Surface Creation
        ad->moment_bar_circle_surface = eext_circle_surface_conformant_add(ad->moment_bar_conformant);
        retv_if(ad->moment_bar_circle_surface == NULL, WINDICATOR_ERROR_FAIL);

        // Indicator
        elm_win_indicator_mode_set(ad->moment_bar_win, ELM_WIN_INDICATOR_SHOW);

        /* create moment bar's layout to main layout's scroller */
        ad->moment_bar_first_page_layout = windicator_moment_bar_first_page_layout_create(ad);
        retv_if(ad->moment_bar_first_page_layout == NULL, WINDICATOR_ERROR_FAIL);

        windicator_moment_bar_update_main_view(ad);

        if(WINDICATOR_ERROR_OK != windicator_dynamic_update(ad)) {
                _E("Failed to update dynamic layout");
        }

        windi_connection_update(ad->moment_bar_rssi_icon, ad->moment_bar_connection_icon);

        elm_object_content_set(ad->moment_bar_conformant, ad->moment_bar_first_page_layout);
        elm_win_conformant_set(ad->moment_bar_win, EINA_TRUE);

        windicator_moment_bar_show(data);

        _I("initialize moment bar END");
        return WINDICATOR_ERROR_OK;
}
