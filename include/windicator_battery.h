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

#ifndef __WINDICATOR_BATTERY_H__
#define __WINDICATOR_BATTERY_H__

Evas_Object *windicator_battery_layout_create(Evas_Object *parent, void *data);
void windicator_battery_layout_destroy(void *data);

Evas_Object *windicator_battery_icon_create(Evas_Object *parent, void *data);
void windicator_battery_icon_destroy(void *data);

windicator_error_e windicator_battery_icon_update(Evas_Object *layout, void *data);
extern void _battery_update(void* data);
extern char *windicator_util_str_from_icu_get(int level);

windicator_error_e windicator_battery_vconfkey_register(void *data);
void windicator_battery_vconfkey_unregister(void);

extern windicator_error_e windicator_battery_init(void *data);
extern void windicator_battery_fini(void);

void windicator_battery_show(void* data);
void windicator_battery_hide(void* data);

#endif /*  __WINDICATOR_BATTERY_H__ */
