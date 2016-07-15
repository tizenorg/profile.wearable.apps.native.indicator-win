#include <Elementary.h>
#include <Eina.h>
#include <efl_extension.h>
#include <device/power.h>
#include <feedback.h>
#include <efl_assist.h>
#include <efl_util.h>

#if !defined(PACKAGE)
#  define PACKAGE "windicator"
#endif

#if !defined(RESDIR)
#  define RESDIR "/usr/apps/org.tizen.windicator/res"
#endif

#if !defined(LOCALEDIR)
#  define LOCALEDIR RESDIR"/locale"
#endif

#if !defined(EDJDIR)
#  define EDJDIR RESDIR"/edje"
#endif

//#define EDJ_FILE EDJDIR"/"PACKAGE".edj"
#define EDJ_FILE "edje/"PACKAGE".edj"

#ifndef _TIZEN_3G_ENABLE
#define _TIZEN_3G_DISABLE
#endif
#define MAX_PATH_LENGTH 1024
#define DYNAMIC_ICON_WIDTH 24
#define DYNAMIC_ICON_HEIGHT 26

//#define ICON_PATH "/usr/apps/org.tizen.windicator/res/image/windicator/"
#define ICON_PATH "image/windicator"

#define PATH_MAX	4096

#ifdef _TIZEN_3G_DISABLE
#define ROTARY_INDEX_MAX 5
#else
#define ROTARY_INDEX_MAX 7
#endif

typedef enum {
        TYPE_MOMENT_NONE = 0,
        TYPE_MOMENT_BAR,
        TYPE_MOMENT_VIEW,
        TYPE_MAX,
} windicator_type;

typedef enum {
        WINDICATOR_ERROR_OK = 0,
        WINDICATOR_ERROR_FAIL = -1,
        WINDICATOR_ERROR_INVALID_PARAMETER = -2,
        WINDICATOR_ERROR_NO_DATA = -3
} windicator_error_e;

enum {
        MUSIC_BTN_DISABLED,
        MUSIC_BTN_ENABLED,
};

struct appdata
{
    /* Common variables */
    int win_w;
    int win_h;
    int is_charged;
    int is_full;
    int pre_charge_state;
    int is_connected;
    int is_cooldown;
    int is_tutorial;
    int is_poweroff_state;
    int moment_bar_rotary_index;
    int is_getting_back;

    /* RSSI, Connection */
    int rssi_hide;
    int connection_hide;

    /* common view */
    Evas_Object *common_sim_state;

    /* Two finger double tap */
    Ecore_X_Window input_win;
    Ecore_Event_Handler *two_finger_handler;
    Ecore_Event_Handler *back_key_handler;
    Ecore_Event_Handler *down_key_handler;

    /* Moment Bar */
    Evas *moment_bar_evas;
    Ecore_Evas *moment_bar_ee;
    Evas_Object *moment_bar_win;
    Evas_Object *moment_bar_bg;
    Evas_Object *moment_bar_conformant;
    Eext_Circle_Surface *moment_bar_circle_surface;
    Evas_Object *moment_bar_first_page_layout;
    Evas_Object *nf;
    Evas_Object *moment_bar_second_page_layout;
    Evas_Object *moment_bar_main_layout;
    Evas_Object *moment_bar_scroller;
    Evas_Object *moment_bar_box;
    Evas_Object *moment_bar_index;
    Ecore_Timer *moment_bar_timer;
    Ecore_Timer *brightness_bar_timer;
    Ecore_Timer *volume_bar_timer;
    Ecore_Timer *common_timer;
    Ecore_Timer *launch_current_view_timer;
    Eina_Bool is_scrolled;
    int cur_page;

    /* Moment Bar - Brightness */
    Evas_Object *brightness_small_layout;
    Evas_Object *brightness_large_layout;
    Evas_Object *brightness_circle_slider;
    int brightness_index;
    int brightness_status;

    /* Moment Bar - Volume */
    Evas_Object *volume_small_icon;
    Evas_Object *volume_large_icon;
    Evas_Object *volume_circle_slider;
    int volume_index;
    int volume_status;

    /* Moment Bar - Dynamic */
    Evas_Object *moment_bar_dynamic_layout;
    Evas_Object *moment_bar_connection_icon;
    Evas_Object *moment_bar_rssi_icon;

    /* Moment Bar - Battery */
    Evas_Object *moment_bar_battery_icon;

    /* Moment Bar - DND button */
    Evas_Object *dnd_btn_layout;
    Evas_Object *dnd_btn_checkbox;
    Evas_Object *dnd_win;

    /* Moment Bar - Flight Mode button */
    Evas_Object *flight_mode_layout;
    Evas_Object *flight_mode_win;

    /* Moment Bar - POWER SAVING button */
    Evas_Object *powersaving_layout;
    Evas_Object *powersaving_popup;
    Evas_Object *powersaving_win;

    /* Moment Bar - Music button */
    Evas_Object *music_btn_layout;
    Evas_Object *music_icon;
    Evas_Object *music_side_icon;
    int music_status;
    int music_is_playing;
    int music_port_id;
    int milk_port_id;
    char* port_app_id;
    char* main_icon_path;
//      char* main_icon_bg_path;
    char* sub_icon_path;
    int music_app_type;
    int launch_music_trigger;
    int launch_setting_trigger;

    /* Moment Bar - Call forward button */
    Evas_Object *callfwd_btn_layout;
    int launch_call_fwd_trigger;

    /* Moment Bar - SCS layout */
    Evas_Object *scs_layout;

    /* Moment View */
    int moment_view_h;
    int moment_view_type;
    Evas *moment_view_evas;
    Ecore_Evas *moment_view_ee;
    Evas_Object *moment_view_win;
    Evas_Object *moment_view_layout;
    Evas_Object *moment_view_batt_img;
    Evas_Object *moment_view_batt_charging_img;
    Evas_Object *moment_view_batt_label;
    Ecore_Animator *batt_animator;
    double start_loop_time;
    int battery_level;
    Evas_Object *moment_view_battery_layout;
    Evas_Object *moment_view_battery_number_layout;
    Ecore_Timer *moment_view_battery_disp_timer;
    Ecore_Timer *moment_view_timer;

    /* Moment View - Dynamic */
    Evas_Object *moment_view_dynamic_layout;
    Evas_Object *moment_view_dynamic_box;
    Evas_Object *moment_view_connection_icon;
    Evas_Object *moment_view_rssi_icon;

    /* Moment View - Battery */
    Evas_Object *moment_view_battery_icon;

    /* Color Theme */
    Eina_List *font_theme;
    Elm_Theme *theme;
    Ecore_Event_Handler *hdl_client_message;
    int momentbar_showing;
    void* msg_handle;
};
struct appdata *windicator_appdata_get(void);
void windicator_lo_update(void* data);
void windicator_util_display_lock(void);
