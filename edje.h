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

#if !defined(strdupa)
# define strdupa(str) strcpy(alloca(strlen(str) + 1), str)
#endif
#if !defined(strndupa)
# define strndupa(str, len) strncpy(alloca(len + 1), str, len)
#endif
typedef enum Edje_Type
{
  EDJE_TYPE_INT,
  EDJE_TYPE_DOUBLE,
  EDJE_TYPE_STR
} Edje_Type;

typedef enum Edje_Compression_Type
{
   EDJE_COMPRESSION_TYPE_RAW,
   EDJE_COMPRESSION_TYPE_COMP,
   EDJE_COMPRESSION_TYPE_LOSSY,
   EDJE_COMPRESSION_TYPE_USER
} Edje_Compression_Type;

typedef enum Edje_Image_Middle
{
   EDJE_IMAGE_MIDDLE_DEFAULT,
   EDJE_IMAGE_MIDDLE_NONE,
   EDJE_IMAGE_MIDDLE_SOLID
} Edje_Image_Middle;

typedef enum Edje_Image_Scale_Hint
{
   EDJE_IMAGE_SCALE_HINT_NONE,
   EDJE_IMAGE_SCALE_HINT_DYNAMIC,
   EDJE_IMAGE_SCALE_HINT_STATIC
} Edje_Image_Scale_Hint;

typedef struct Edje_Style
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   const char *base;
   Eina_Hash *tags;
} Edje_Style;

typedef struct Edje_Color_Class
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   int color[4];
   int color2[4];
   int color3[4];
} Edje_Color_Class;

typedef struct Edje_Data
{
   EINA_INLIST;
   const char *doc;
   Eina_Hash *items;
   Eina_Hash *files;
} Edje_Data;


typedef struct Edje_Property
{
   Edje_Type type;
   union
   {
      int i;
      double d;
      const char *s;
   } data;
} Edje_Property;

typedef struct Edje_External
{
   EINA_INLIST;
   const char *doc;
   const char *external;
} Edje_External;

typedef struct Edje_Image
{
   EINA_INLIST;
   const char *doc;
   const char *normal;
   Eina_List *tween; /* stringshared tween filenames */
   int border[4];
   Edje_Image_Middle middle;
   Eina_Bool border_scale : 1;
   Edje_Image_Scale_Hint scale_hint;
} Edje_Image;

typedef struct Edje_Set_Image
{
   EINA_INLIST;
   const char *filename;
   Edje_Compression_Type comp;
   int comp_amount;
   int size[4];
} Edje_Set_Image;

typedef struct Edje_Set
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   Edje_Set_Image *set_images;
} Edje_Set;

typedef struct Edje_Images
{
   EINA_INLIST;
   const char *doc;
   Edje_Set_Image *set_images;
   Edje_Set *sets;
} Edje_Images;

typedef struct Edje_Font
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   const char *alias;
} Edje_Font;

typedef struct Edje_Part
{
   EINA_INLIST;
   const char *doc;
   Edje_Set_Image *set_images;
   Edje_Set *sets;
   Edje_Images *images;
   Edje_Font *fonts;
   Edje_Style *styles;
} Edje_Part;

typedef struct Edje_Parts
{
   const char *doc;
   Eina_Hash *aliases;
   Edje_Set_Image *set_images;
   Edje_Set *sets;
   Edje_Images *images;
   Edje_Font *fonts;
   Edje_Style *styles;
   Edje_Part *parts;
} Edje_Parts;

typedef struct Edje_Group
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   const char *alias;
   int min[2];
   int max[2];
   Eina_Bool script_only : 1;
   Edje_Color_Class *color_classes;
   Edje_Data *data;
   Edje_Style *styles;
   Edje_Images *images;
   Edje_Parts *parts;
} Edje_Group;

typedef struct Edje_Collection
{
   EINA_INLIST;
   const char *doc;
   Edje_Group *groups;
   Edje_Color_Class *color_classes;
} Edje_Collection;

typedef struct Edje
{
   Edje_Collection    *collections;
   Edje_External      *externals;
   Edje_Images        *images;
   Edje_Set           *sets;
   Edje_Font          *fonts;
   Edje_Data          *data;
   Edje_Color_Class   *color_classes;
   Edje_Style         *styles;
} Edje;
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
Edje_Property *edje_property_new(Edje_Type type);

const char *edje_stringshare_toupper(const char *str);
Eina_Inlist *edje_inlist_join(Eina_Inlist *a, Eina_Inlist *b);
#endif
