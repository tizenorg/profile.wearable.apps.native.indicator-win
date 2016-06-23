#include <pkgmgr-info.h>
#include <aul.h>
#include <app_control.h>
#include <app_common.h>

#include "windicator.h"
#include "log.h"

#define CLOCK_SETTING_BRIGHTNESS_PKG_NAME  "org.tizen.watch-setting"

void _get_resource(const char *file_in, char *file_path_out, int file_path_max)
{
        char *res_path = app_get_resource_path();
        if (res_path) {
                snprintf(file_path_out, file_path_max, "%s%s", res_path, file_in);
                free(res_path);
        }
}

Evas_Object *windicator_util_bg_create(Evas_Object *parent)
{
        retv_if(parent == NULL, NULL);

        Evas_Object *bg = elm_bg_add(parent);
        retv_if(bg == NULL, NULL);

        evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_win_resize_object_add(parent, bg);
        evas_object_color_set(bg, 0, 0, 0, 0);
        evas_object_show(bg);

        return bg;
}

Evas_Object *windicator_util_conformant_create(Evas_Object *parent)
{
        retv_if(parent == NULL, NULL);
        Evas_Object *conform;

        conform = elm_conformant_add(parent);
        evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_win_resize_object_add(parent, conform);
        evas_object_show(conform);

        return conform;

}

Evas_Object *windicator_util_layout_create(Evas_Object *parent)
{
        Evas_Object *layout;

        if (parent == NULL) return NULL;

        layout = elm_layout_add(parent);

        if (layout == NULL) {
                _D("Failed elm_layout_add.\n");
                return NULL;
        }

        elm_layout_theme_set(layout, "layout", "application", "default");
        evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        elm_win_resize_object_add(parent, layout);

        evas_object_show(layout);

        return layout;
}

Evas_Object *windicator_util_naviframe_create(Evas_Object *parent)
{
        Evas_Object *nf = NULL;

        if (parent == NULL) return NULL;

        nf = elm_naviframe_add(parent);
        elm_object_part_content_set(parent, "elm.swallow.content", nf);

        evas_object_show(nf);

        return nf;
}

windicator_error_e windicator_util_win_notification_property_set(Evas_Object *win)
{
		retv_if(win == NULL, WINDICATOR_ERROR_FAIL);

		/* set notification's priority */
		efl_util_set_notification_window_level(win, EFL_UTIL_NOTIFICATION_LEVEL_DEFAULT);

        return WINDICATOR_ERROR_OK;
}

static void _quit_cb(void *data, Evas_Object *obj, void *ei)
{
        elm_exit();
}

Evas_Object* windicator_util_win_create(const char *name, Eina_Bool is_alpha, windicator_type type)
{
        retv_if(name == NULL, NULL);

        _I("win name(%s), alpha(%d), type(%d)", name, is_alpha, type);

        if(type == TYPE_MOMENT_VIEW) {
                Evas_Object *win = elm_win_add(NULL, name, ELM_WIN_NOTIFICATION);
                retv_if(win == NULL, NULL);

                elm_win_title_set(win, name);
                elm_win_borderless_set(win, EINA_TRUE);
                elm_win_autodel_set(win, EINA_TRUE);
                elm_win_role_set(win, "no-dim");

                if (elm_win_wm_rotation_supported_get(win)) {
                        int rots[4] = { 0, 90, 180, 270 };
                        elm_win_wm_rotation_available_rotations_set(win, (const int*)(&rots), 4);
                }

                evas_object_smart_callback_add(win, "delete,request", _quit_cb, NULL);

                return win;
        }
        else
        {
            Evas_Object *win = NULL;

            retv_if(!name, NULL);

            win = elm_win_add(NULL, name, ELM_WIN_BASIC);
            retv_if(!win, NULL);

            elm_win_title_set(win, name);
	    elm_win_autodel_set(win, EINA_TRUE);

            if (elm_win_wm_rotation_supported_get(win)) {
            	int rots[4] = { 0, 90, 180, 270 };
                elm_win_wm_rotation_available_rotations_set(win, (const int*)(&rots), 4);
            }

            return win;
        }
}

void windicator_util_launch_app(void *data)
{
        _I("");
        struct appdata *ad = data;
        int ret = -1;

        if(ad->launch_setting_trigger != 0)
        {
                app_control_h ac = NULL;

                _E("launch setting %d",ad->launch_setting_trigger);
                if ((ret = app_control_create(&ac)) != APP_CONTROL_ERROR_NONE) {
                        _E("Failed to create app control:%d", ret);
                }
                //launch setting
                if(ad->launch_setting_trigger==1)//brightness
                {
                        app_control_set_app_id(ac, CLOCK_SETTING_BRIGHTNESS_PKG_NAME);
                        app_control_add_extra_data(ac, "launch-type", "brightness");
                }

                app_control_send_launch_request(ac, NULL, NULL);
                app_control_destroy(ac);
                //ad->launch_setting_trigger = 0;
        }

}

void windicator_util_event_register(void *data)
{
	 struct appdata *ad = (struct appdata *)data;
	 ret_if(ad == NULL);


    /* register dynamic vconfkey event */
    if(WINDICATOR_ERROR_OK != windicator_dynamic_vconfkey_register(ad)) {
            _E("Failed to register dynamic vconfkey event");
    }


	if(WINDICATOR_ERROR_OK != windicator_battery_init(ad)) {
		_E("Failed to register battery vconfkey event");
	}

}


void windicator_util_event_unregister()
{
        windicator_dynamic_vconfkey_unregister();
        windicator_battery_fini();
}

void windicator_util_display_lock(void)
{
	 device_power_request_lock(POWER_LOCK_DISPLAY_DIM, 0);
}

