#ifndef EDJE_TYPES_H
#define EDJE_TYPES_H

#include <Eina.h>
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

typedef enum Edje_Part_Type
{
   EDJE_PART_TYPE_RECT,
   EDJE_PART_TYPE_TEXT,
   EDJE_PART_TYPE_IMAGE,
   EDJE_PART_TYPE_SWALLOW,
   EDJE_PART_TYPE_TEXTBLOCK,
   EDJE_PART_TYPE_GROUP,
   EDJE_PART_TYPE_BOX,
   EDJE_PART_TYPE_TABLE,
   EDJE_PART_TYPE_EXTERNAL
} Edje_Part_Type;

typedef enum Edje_Part_Ignore_Flag
{
   EDJE_IGNORE_FLAG_NONE,
   EDJE_IGNORE_FLAG_ON_HOLD
} Edje_Part_Ignore_Flag;

typedef enum Edje_Part_Pointer_Mode
{
   EDJE_POINTER_MODE_AUTOGRAB,
   EDJE_POINTER_MODE_NOGRAB
} Edje_Part_Pointer_Mode;

typedef enum Edje_Part_Effect
{
   EDJE_PART_EFFECT_PLAIN,
   EDJE_PART_EFFECT_OUTLINE,
   EDJE_PART_EFFECT_SOFT_OUTLINE,
   EDJE_PART_EFFECT_SHADOW,
   EDJE_PART_EFFECT_SOFT_SHADOW,
   EDJE_PART_EFFECT_OUTLINE_SHADOW,
   EDJE_PART_EFFECT_OUTLINE_SOFT_SHADOW,
   EDJE_PART_EFFECT_FAR_SHADOW,
   EDJE_PART_EFFECT_FAR_SOFT_SHADOW,
   EDJE_PART_EFFECT_GLOW
} Edje_Part_Effect;

typedef enum Edje_Part_Entry_Mode
{
   EDJE_PART_ENTRY_MODE_NONE,
   EDJE_PART_ENTRY_MODE_PLAIN,
   EDJE_PART_ENTRY_MODE_EDITABLE,
   EDJE_PART_ENTRY_MODE_PASSWORD
} Edje_Part_Entry_Mode;

typedef enum Edje_Part_Select_Mode
{
   EDJE_PART_SELECT_MODE_DEFAULT,
   EDJE_PART_SELECT_MODE_EXPLICIT_DEFAULT
} Edje_Part_Select_Mode;

typedef enum Edje_Part_Description_Aspect_Pref
{
   EDJE_PART_DESCRIPTION_ASPECT_PREF_NONE,
   EDJE_PART_DESCRIPTION_ASPECT_PREF_HORIZONTAL,
   EDJE_PART_DESCRIPTION_ASPECT_PREF_VERTICAL,
   EDJE_PART_DESCRIPTION_ASPECT_PREF_BOTH
} Edje_Part_Description_Aspect_Pref;

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

typedef struct Edje_Part_Description
{
   EINA_INLIST;
   const char *doc;
   struct
   {
      const char *name;
      float index;
   } inherit;
   struct
   {
      const char *name;
      float index;
   } state;

   float align[2];
   float aspect[2];
   int min[2];
   int max[2];
   int step[2];
   Eina_Bool fixed[2];
   Eina_Bool visible : 1;
   Edje_Part_Description_Aspect_Pref aspect_preference;
   const char *color_class;
   int color[4];
   int color2[4];
   int color3[4];
   struct
   {
      float relative[2];
      int offset[2];
      const char *to;
      const char *to_x;
      const char *to_y;
   } rel[2];

   Edje_Image *images;
   Edje_Images *imageses;
   Edje_Color_Class *color_classes;
} Edje_Part_Description;

typedef struct Edje_Part
{
   EINA_INLIST;
   const char *doc;
   const char *name;
   const char *clip_to;
   const char *source;
   const char *source2;
   const char *source3;
   const char *source4;
   const char *source5;
   const char *source6;
   Edje_Part_Ignore_Flag ignore_flags;
   Edje_Part_Type type;
   Edje_Part_Effect effect;
   Edje_Part_Pointer_Mode pointer_mode;
   Edje_Part_Entry_Mode entry_mode;
   Edje_Part_Select_Mode select_mode;
   Eina_Bool mouse_events : 1;
   Eina_Bool repeat_events : 1;
   Eina_Bool scale : 1;
   Eina_Bool precise_is_inside : 1;
   Eina_Bool use_alternate_font_metrics : 1;
   Eina_Bool multiline : 1;
   struct
   {
      const char *confine;
      const char *events;
      int x[3];
      int y[3];
   } dragable;
   Edje_Part_Description *descriptions;
   Edje_Set_Image *set_images;
   Edje_Color_Class *color_classes;
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
   Edje_Color_Class *color_classes;
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
#endif
