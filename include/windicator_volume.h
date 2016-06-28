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

#ifndef __WINDICATOR_VOLUME_H__
#define __WINDICATOR_VOLUME_H__

Evas_Object *windicator_volume_small_icon_create(Evas_Object *parent, void *data);
void windicator_volume_small_icon_destroy(void *data);

void windicator_volume_icon_set_by_level(void *data);
windicator_error_e windicator_volume_update(void *data);

void on_clicked_small_volume_icon(void *data, Evas_Object *obj, const char *emission, const char *source);

typedef enum
{
        SETTING_SOUND_MODE_SND  = 0,    // Sound on, Vibrate off
        SETTING_SOUND_MODE_VIB,                 // Sound off, Vibrate on
        SETTING_SOUND_MODE_MUTE,                // Sound off, Vibrate off
        SETTING_SOUND_MODE_UNKNOW
} SettingSoundMode;

#endif /*  __WINDICATOR_VOLUME_H__ */
