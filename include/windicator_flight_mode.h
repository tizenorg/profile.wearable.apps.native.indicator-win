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

#ifndef __WINDICATOR_FLIGHT_MODE_H__
#define __WINDICATOR_FLIGHT_MODE_H__

Evas_Object *windicator_flight_mode_layout_create(Evas_Object *parent, void *data);
void windicator_flight_mode_layout_destroy(void *data);

windicator_error_e windicator_flight_mode_update(void *data);

void _flight_mode_clicked_cb(void *data, Evas_Object *obj, const char *emission, const char *source);

#endif /* __WINDICATOR_FLIGHT_MODE_H__ */
