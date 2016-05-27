/*
 * Copyright (c) 2000-2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __WINDICATOR_DBUS__
#define __WINDICATOR_DBUS__

void windi_set_dbus_handler(void *data);
void windi_register_lcd_state_changed_cb(void *data);
void windi_register_wrist_up_cb(void *data, void(*dbus_handler_cb)(void*data, DBusMessage *msg));
void windi_register_movement_cb(void *data, void(*dbus_handler_cb)(void*data, DBusMessage *msg));
void windi_register_charge_changed_cb(void *data, void(*dbus_handler_cb)(void*data, DBusMessage *msg));
void windi_register_home_button_cb(void *data, void(*dbus_handler_cb)(void*data, DBusMessage *msg));
void windi_register_cool_down_cb(void *data);

void windicator_dbus_init(void *data);
void windicator_dbus_fini(void *data);

int windicator_dbus_method_async_call(const char *dest, const char *path,
		const char *interface, const char *method,
		const char *sig, char *param[]);
#endif
