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

#ifndef __WINDICATOR_MOMENT_BAR_H__
#define __WINDICATOR_MOMENT_BAR_H__

Evas_Object *windicator_moment_bar_first_page_layout_create(void *data);
Evas_Object *windicator_moment_bar_first_page_layout_create_for_3g(void *data);
windicator_error_e windicator_moment_bar_init(void *data);
windicator_error_e windicator_moment_bar_show(void *data);
windicator_error_e windicator_moment_bar_hide(void *data);
//Eina_Bool windicator_moment_bar_timer_cb(void *data);
void windicator_moment_bar_refresh_main_view(void *data);
//void windicator_handle_fade_in_animation(void* data);
windicator_error_e windicator_moment_bar_update_main_view(void *data);

windicator_error_e windicator_show_moment_bar_directly(void* data);
windicator_error_e windicator_hide_moment_bar_directly(void* data);
//void windicator_handle_clicked_animation(void* data,int index);
//void change_theme_callback(void *data);
void windicator_util_back_key_ungrab(void *data);
void windicator_util_down_key_ungrab(void *data);

#endif /* __WINDICATOR_MOMENT_BAR_H__ */
