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


#ifndef __WINDICATOR_DYNAMIC_H__
#define __WINDICATOR_DYNAMIC_H__

Evas_Object *windicator_dynamic_layout_create(Evas_Object *parent, void *data);
void windicator_dynamic_layout_destroy(void *data);

windicator_error_e windicator_dynamic_update(void *data);

Evas_Object *windicator_dynamic_layout_for_moment_view_create(Evas_Object *parent, void *data);
Evas_Object *windicator_rssi_icon_create(Evas_Object *parent, void *data);
Evas_Object *windicator_connection_icon_create(Evas_Object *parent, void *data);

void windicator_set_dynamic_icon_layout(void* data, int isBTConnected);
void dynamic_event_icon_refresh_position_for_3g(Evas_Object *layout, int isForMomentView);

windicator_error_e windicator_dynamic_vconfkey_register(void *data);

#endif /* __WINDICATOR_DYNAMIC_H__ */
