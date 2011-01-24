/*
 * Copyright 2011 Mike Blumenkrantz <mike@zentific.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EDJE_H
#define EDJE_H

#include <Eina.h>
#include "edje_types.h"

#if !defined(strdupa)
# define strdupa(str) strcpy(alloca(strlen(str) + 1), str)
#endif
#if !defined(strndupa)
# define strndupa(str, len) strncpy(alloca(len + 1), str, len)
#endif

Edje *edje_new(void);
Edje_Collection *edje_collection_new(void);
Edje_Color_Class *edje_color_class_new(void);
Edje_External *edje_external_new(void);
Edje_Images *edje_images_new(void);
Edje_Image *edje_image_new(void);
Edje_Set_Image *edje_set_image_new(void);
Edje_Set *edje_set_new(void);
Edje_Font *edje_font_new(void);
Edje_Data *edje_data_new(void);
Edje_Color_Class *edje_color_class_new(void);
Edje_Style *edje_style_new(void);
Edje_Group *edje_group_new(void);
Edje_Parts *edje_parts_new(void);
Edje_Part *edje_part_new(void);
Edje_Part_Description *edje_part_description_new(void);
Edje_Property *edje_property_new(Edje_Type type);

const char *edje_stringshare_toupper(const char *str);
Eina_Inlist *edje_inlist_join(Eina_Inlist *a, Eina_Inlist *b);
#endif
