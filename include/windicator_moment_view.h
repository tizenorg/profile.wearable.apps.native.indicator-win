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

#ifndef __WINDICATOR_MOMENT_VIEW_H__
#define __WINDICATOR_MOMENT_VIEW_H__

Eina_Bool windicator_moment_view_timer_cb(void *data);
windicator_error_e windicator_moment_view_init(void *data);
windicator_error_e windicator_moment_view_show(void *data,int type);
windicator_error_e windicator_moment_view_hide(void *data);

typedef enum
{
	WINDICATOR_MOMENT_VIEW_TYPE_INDICATOR,
	WINDICATOR_MOMENT_VIEW_TYPE_CHARGING_ANI
} windicator_moment_view_type;

#endif /* __WINDICATOR_MOMENT_VIEW_H__ */
