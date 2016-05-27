/*
 * Copyright (c) 2000-2015 Samsung Electronics Co., Ltd.
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

#ifndef __WINDICATOR_UTIL_H__
#define __WINDICATOR_UTIL_H__

Evas_Object* windicator_util_win_create(const char *name, Eina_Bool is_alpha, windicator_type type);
void windicator_util_win_status_change(Evas_Object *win);
Evas_Object *windicator_util_bg_create(Evas_Object *parent);
Evas_Object *windicator_util_conformant_create(Evas_Object *parent);
windicator_error_e windicator_util_win_notification_property_set(Evas_Object *win);

#endif /*  __WINDICATOR_UTIL_H_ */
