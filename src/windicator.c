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

#include <Ecore.h>
#include <Elementary.h>
#include <appcore-efl.h>
#include <app.h>

#include "windicator.h"
#include "log.h"
#include "windicator_util.h"
#include "windicator_moment_bar.h"
#include "windicator_moment_view.h"
#include "windicator_dynamic.h"
#include "windicator_scs.h"
#include "windicator_battery.h"
#include "windicator_call_fwd_btn.h"

#define MOMENT_VIEW_HEIGHT 105;

struct appdata *g_ad = NULL;

static void _init_variables(void *data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        /* Common */
        ad->is_charged = 0;
        ad->is_full = 0;
        ad->pre_charge_state = 0;
        ad->is_cooldown = 0;
        ad->is_tutorial = 0;
        ad->moment_bar_rotary_index = 0;
        ad->is_getting_back = 0;

        /* RSSI, Connection */
        ad->rssi_hide = 1;
        ad->connection_hide = 1;

        /* Moment Bar */
        ad->moment_bar_win = NULL;
        ad->moment_bar_bg = NULL;
        ad->moment_bar_conformant = NULL;
        ad->moment_bar_circle_surface = NULL;
        ad->moment_bar_first_page_layout = NULL;
        ad->moment_bar_second_page_layout = NULL;
        ad->moment_bar_main_layout = NULL;
        ad->moment_bar_scroller = NULL;
        ad->moment_bar_box = NULL;
        ad->moment_bar_index = NULL;
        ad->moment_bar_timer = NULL;
        ad->brightness_bar_timer = NULL;
        ad->volume_bar_timer = NULL;
        ad->common_timer = NULL;
        ad->launch_current_view_timer = NULL;
        ad->is_scrolled = EINA_FALSE;
        ad->cur_page = -1;
        ad->hdl_client_message = NULL;
        ad->momentbar_showing = 0;
        /* Moment Bar - Brightness */
        ad->brightness_small_layout = NULL;
        ad->brightness_large_layout = NULL;
        ad->brightness_circle_slider = NULL;
        ad->brightness_index = 0;
        ad->brightness_status = 0;

        /* Moment Bar - Volume */
        ad->volume_small_icon = NULL;
        ad->volume_large_icon = NULL;
        ad->volume_circle_slider = NULL;
        ad->volume_index = 0;
        ad->volume_status = 0;

        /* Moment Bar - Dynamic */
        ad->moment_bar_dynamic_layout = NULL;
        ad->moment_bar_connection_icon = NULL;
        ad->moment_bar_rssi_icon = NULL;

        /* Moment Bar - Battery */
        ad->moment_bar_battery_icon = NULL;

        /* Moment Bar DND Button */
        ad->dnd_btn_layout = NULL;
        ad->dnd_btn_checkbox = NULL;
        ad->dnd_win = NULL;

        /* Moment Bar - Flight Mode button */
        ad->flight_mode_layout = NULL;
        ad->flight_mode_win = NULL;

        /* Moment Bar POWERSAVING Button */
        ad->powersaving_layout = NULL;
        ad->powersaving_popup = NULL;
        ad->powersaving_win = NULL;

        /* Moment Bar Music Button */
        ad->music_btn_layout = NULL;
        ad->music_status = 0;
        ad->music_is_playing = 0;
        ad->launch_music_trigger = 0;
        ad->music_app_type = 0;

        /* Moment Bar - Call forward button */
        ad->callfwd_btn_layout = NULL;
        ad->launch_call_fwd_trigger = 0;

        /* Moment Bar - SCS layout */
        ad->scs_layout = NULL;

        /* Moment View */
        ad->moment_view_h = MOMENT_VIEW_HEIGHT;
        ad->moment_view_win = NULL;
        ad->moment_view_layout = NULL;
        ad->moment_view_timer = NULL;

        /* Moment View - Dynamic */
        ad->moment_view_dynamic_layout = NULL;
        ad->moment_view_dynamic_box = NULL;
        ad->moment_view_connection_icon = NULL;
        ad->moment_view_rssi_icon = NULL;

        /* Moment View - Battery */
        ad->moment_view_battery_icon = NULL;
}

void windicator_lo_update(void* data)
{
        struct appdata *ad = (struct appdata *)data;
        ret_if(ad == NULL);

        //ad->layout_status = 0;
        elm_object_signal_emit(ad->moment_bar_first_page_layout, "set_default_layout", "sw.all");
}

static bool app_create(void *data)
{
    struct appdata *ad = (struct appdata *)data;
    retv_if(ad == NULL, false);

#ifdef _TIZEN_3G_ENABLE
        _D("_TIZEN_3G_ENABLE");
#else
        _D("_TIZEN_BT");
#endif

    elm_app_base_scale_set(1.3);

    elm_config_preferred_engine_set("opengl_x11");
    _init_variables(ad);

    Evas_Object *win = elm_win_add(NULL, "dummy_window", ELM_WIN_BASIC);
    elm_win_screen_size_get(win, NULL, NULL, &ad->win_w, &ad->win_h);
    _D("win_w(%d) win_h(%d)", ad->win_w, ad->win_h);

    /* initialize Moment Bar */
    if(WINDICATOR_ERROR_OK != windicator_moment_bar_init(ad)) {
            _E("Failed to initialize moment bar");
    }

    /* initialize Moment View */
    if(WINDICATOR_ERROR_OK != windicator_moment_view_init(ad)) {
            _E("Failed to intialize moment view");
    }
    //evas_object_show(ad->moment_view_win);
    //evas_object_show(ad->moment_bar_win);
    /* Disable vsync */
    ecore_animator_source_set(ECORE_ANIMATOR_SOURCE_TIMER);
	return true;
}

/**
 * @brief This callback function is called when another application.
 * sends the launch request to the application
 */
static void app_control(app_control_h app_control, void *user_data)
{
	/* Handle the launch request. */
}

/**
 * @brief This callback function is called each time.
 * the application is completely obscured by another application
 * and becomes invisible to the user
 */
static void app_pause(void *user_data)
{
	/* Take necessary actions when application becomes invisible. */
}

/**
 * @brief This callback function is called each time.
 * the application becomes visible to the user
 */
static void app_resume(void *user_data)
{
	/* Take necessary actions when application becomes visible. */
}

/**
 * @brief This callback function is called once after the main loop of the application exits.
 */
static void app_terminate(void *user_data)
{
	/*
	 * Release all resources.
	 */
    _D("");

    struct appdata *ad = (struct appdata *)user_data;
    ret_if(ad == NULL);

}

/**
 * @brief Main function of the application.
 */
int main(int argc, char *argv[])
{
	int ret;
	struct appdata ad;
	_I("Indicator Started");
	ui_app_lifecycle_callback_s event_callback = {0, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	/*
	 * If you want to handling more events,
	 * please check the application lifecycle guide
	 */

	memset(&ad, 0x0, sizeof(struct appdata));
	g_ad = &ad;
	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		_E("ui_app_main() is failed. err = %d", ret);
	}

	return ret;
}
