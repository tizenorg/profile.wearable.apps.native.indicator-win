#include <pkgmgr-info.h>
#include <aul.h>
#include <app_control.h>

#include "windicator.h"
#include "log.h"

#define CLOCK_SETTING_BRIGHTNESS_PKG_NAME  "org.tizen.watch-setting"

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

windicator_error_e windicator_util_win_notification_property_set(Evas_Object *win)
{
		retv_if(win == NULL, WINDICATOR_ERROR_FAIL);

		/* set notification's priority */
		efl_util_set_notification_window_level(win, EFL_UTIL_NOTIFICATION_LEVEL_DEFAULT);

        return WINDICATOR_ERROR_OK;
}

#if 0
static void _aux_hint_set(Evas_Object *obj)
{
        ret_if(obj == NULL);

        const Eina_List *l, *ll;
        int id = -1;
        char *hint;
        Ecore_X_Window xid;
        char buffer[128]={0,};

        xid = elm_win_xwindow_get(obj);
        l = elm_win_aux_hints_supported_get(obj);

        id = elm_win_aux_hint_add(obj, "wm.comp.win.effect.enable", "1");
        if (-1 == id) _E("Failed to get effect enable id");

        EINA_LIST_FOREACH(l, ll, hint) {
                _D("aux hint : %s", hint);
                if (!strncmp(hint, "wm.comp.win.effect.launch.pos", strlen("wm.comp.win.effect.launch.pos"))) {
                        /*source format is WIN_ID.ROTATION_ANGLE.X.Y.W.H*/
                        sprintf(buffer, "%d.0.0.%d.0.0", xid, -1);
                        _D("pos buffer : %s", buffer);
                        id = elm_win_aux_hint_add(obj, "wm.comp.win.effect.launch.pos", buffer);
                        if (id != -1) {
                                //ad->launch_id = id; // save id to enable effect
                        }
                } else if (!strncmp(hint, "wm.comp.win.effect.close.pos", strlen("wm.comp.win.effect.close.pos"))) {
                        id = elm_win_aux_hint_add(obj, "wm.comp.win.effect.close.pos", buffer); // close effect direction.
                        if (id != -1) {
                                //ad->close_id = id; // save id to enable effect
                        }
                }
        }
}
#endif

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
            //unsigned int opaque_val = 0;
            //Ecore_X_Atom opaque_atom;
            //Ecore_X_Window xwin;
            //Ecore_X_Window root_win;
            Evas_Object *win = NULL;

            retv_if(!name, NULL);

            win = elm_win_add(NULL, name, ELM_WIN_BASIC);
            retv_if(!win, NULL);

            elm_win_title_set(win, name);

            elm_win_role_set(win, "no-effect");

            //_aux_hint_set(win);

            //opaque_atom = ecore_x_atom_get("_E_ILLUME_WINDOW_REGION_OPAQUE");
            //xwin = elm_win_xwindow_get(win);
            //ecore_x_window_prop_card32_set(xwin, opaque_atom, &opaque_val, 1);

            if (elm_win_wm_rotation_supported_get(win)) {
                    const int rots[1] = {0};
                    elm_win_wm_rotation_available_rotations_set(win, rots, 1);
            }

            evas_object_color_set(win, 0, 0, 0, 0);

            //xwin = elm_win_xwindow_get(win);

            //ecore_x_e_illume_quickpanel_set(xwin, 1);

            elm_win_quickpanel_priority_major_set(win, 300);

            Ecore_Evas *ee;
            ee = ecore_evas_ecore_evas_get(evas_object_evas_get(win));
            ecore_evas_name_class_set(ee,"QUICKPANEL","QUICKPANEL");

            evas_object_move(win, 0, -360);
            evas_object_show(win);

            //elm_win_title_set(win, title);
            elm_win_borderless_set(win, EINA_TRUE);

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
                        app_control_set_package(ac, CLOCK_SETTING_BRIGHTNESS_PKG_NAME);
                        app_control_add_extra_data(ac, "launch-type", "brightness");
                }

                app_control_send_launch_request(ac, NULL, NULL);
                app_control_destroy(ac);
                ad->launch_setting_trigger = 0;
        }

}
