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

typedef enum
{
  EDJE_TYPE_INT,
  EDJE_TYPE_DOUBLE,
  EDJE_TYPE_STR
} Edje_Type;

typedef enum
{
   EDJE_COMPRESSION_TYPE_RAW,
   EDJE_COMPRESSION_TYPE_COMP,
   EDJE_COMPRESSION_TYPE_LOSSY,
   EDJE_COMPRESSION_TYPE_USER
} Edje_Compression_Type;

typedef enum
{
   EDJE_IMAGE_MIDDLE_DEFAULT,
   EDJE_IMAGE_MIDDLE_NONE,
   EDJE_IMAGE_MIDDLE_SOLID
} Edje_Image_Middle;

typedef enum
{
   EDJE_IMAGE_SCALE_HINT_NONE,
   EDJE_IMAGE_SCALE_HINT_DYNAMIC,
   EDJE_IMAGE_SCALE_HINT_STATIC
} Edje_Image_Scale_Hint;

typedef struct
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   const char *base;
   Eina_Hash *tags;
} Edje_Style;

typedef struct
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   int color[4];
   int color2[4];
   int color3[4];
} Edje_Color_Class;

typedef struct
{
   EINA_INLIST;
   const char *doc;
   Eina_Hash *items;
   Eina_Hash *files;
} Edje_Data;


typedef struct
{
   Edje_Type type;
   union
   {
      int i;
      double d;
      const char *s;
   } data;
} Edje_Property;

typedef struct
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
} Edje_Group;

typedef struct
{
   EINA_INLIST;
   const char *doc;
   const char *external;
} Edje_External;

typedef struct
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

typedef struct
{
   EINA_INLIST;
   const char *filename;
   Edje_Compression_Type comp;
   int comp_amount;
   int size[4];
} Edje_Set_Image;

typedef struct
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   Edje_Set_Image *set_images;
} Edje_Set;

typedef struct
{
   EINA_INLIST;
   const char *doc;
   Edje_Set_Image *set_images;
   Edje_Set *sets;
} Edje_Images;

typedef struct
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   const char *alias;
} Edje_Font;

typedef struct
{
   EINA_INLIST;
   const char *doc;
   Edje_Group *groups;
   Edje_Color_Class *color_classes;
} Edje_Collection;

typedef struct
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
Edje_Property *edje_property_new(Edje_Type type);

const char *edje_stringshare_toupper(const char *str);

#endif
