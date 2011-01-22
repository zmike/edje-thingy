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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <string.h>
#include <stdio.h>

#include "edje.h"
#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#elif defined __GNUC__
#define alloca __builtin_alloca
#elif defined _AIX
#define alloca __alloca
#else
#include <stddef.h>
void *alloca (size_t);
#endif

const char *
edje_stringshare_toupper(const char *str)
{
   char *tmp;

   tmp = strdupa(str);
   eina_str_toupper(&tmp);
   return eina_stringshare_add(tmp);
}


#define DEF(TYPE, name) \
TYPE * \
name##_new(void) \
{ \
   return calloc(1, sizeof(TYPE)); \
}

DEF(Edje, edje)
DEF(Edje_Collection, edje_collection)
DEF(Edje_Color_Class, edje_color_class)
DEF(Edje_External, edje_external)
DEF(Edje_Images, edje_images)
DEF(Edje_Image, edje_image)
DEF(Edje_Set, edje_set)
DEF(Edje_Font, edje_font)
DEF(Edje_Data, edje_data)
DEF(Edje_Style, edje_style)
DEF(Edje_Group, edje_group)

Edje_Property *
edje_property_new(Edje_Type type)
{
   Edje_Property *p;

   p = calloc(1, sizeof(Edje_Property));
   EINA_SAFETY_ON_NULL_RETURN_VAL(p, NULL);
   p->type = type;
   return p;
}
