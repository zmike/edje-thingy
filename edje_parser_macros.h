#ifndef EDJE_PARSER_MACROS_H
#define EDJE_PARSER_MACROS_H

#define COLLECTION  ep->collection
#define COLOR_CLASS ep->color_class
#define IMAGES      ep->images
#define SET         ep->set
#define SET_IMAGE   ep->set_image
#define GROUP       ep->group
#define PARTS       ep->parts
#define PART       ep->part
#define DESCRIPTION ep->description

#define EDJE        ep->edje
#define EDJE_CREATE if (!EDJE) EDJE = edje_new()
/* retrieve current TYPE from NAME in parser struct */
#define PARSER_CURRENT(NAME, TYPE)                                                    \
  EINA_INLIST_CONTAINER_GET(                                                          \
    EINA_INLIST_GET(NAME)->last ? EINA_INLIST_GET(NAME)->last : EINA_INLIST_GET(NAME) \
    , TYPE)

/* stringshares "string" without quotes */
#define STRINGSHARE_NOQUOTES(TEXT) \
  eina_stringshare_add_length(TEXT + 1, strlen(TEXT + 1) - 1)
/*************************** APPEND MACROS ********************************/
/* append to parser for temp storage */
#define PARSER_APPEND(NAME, APPEND, TYPE)                                    \
  if (NAME)                                                                  \
    {                                                                        \
       Eina_Inlist *l;                                                       \
       l = edje_inlist_join(EINA_INLIST_GET(NAME), EINA_INLIST_GET(APPEND)); \
       NAME = EINA_INLIST_CONTAINER_GET(l, TYPE);                            \
    }                                                                        \
  else                                                                       \
    NAME = APPEND
/* append to edje struct */
#define EDJE_APPEND(NAME, APPEND, TYPE)                                            \
  if (EDJE->NAME)                                                                  \
    {                                                                              \
       Eina_Inlist *l;                                                             \
       l = edje_inlist_join(EINA_INLIST_GET(EDJE->NAME), EINA_INLIST_GET(APPEND)); \
       EDJE->NAME = EINA_INLIST_CONTAINER_GET(l, TYPE);                            \
    }                                                                              \
  else                                                                             \
    EDJE->NAME = APPEND
/* append to current (struct->name) */
#define EDJE_STRUCT_APPEND(STRUCT, NAME, APPEND, TYPE)              \
  if (STRUCT->NAME)                                                 \
    {                                                               \
       Eina_Inlist *l;                                              \
       l = edje_inlist_join(EINA_INLIST_GET(STRUCT->NAME), APPEND); \
       STRUCT->NAME = EINA_INLIST_CONTAINER_GET(l, TYPE);           \
    }                                                               \
  else                                                              \
    STRUCT->NAME = EINA_INLIST_CONTAINER_GET(APPEND, TYPE)
/**************************** ERROR MACROS **********************************/
#define ERROR_SYNTAX(TOKEN) do {                                                           \
       if ((!TOKEN) || (!TOKEN->type))                                                     \
         eina_stringshare_replace(&ep->error, "Unexpected EOF!\n");                        \
       else                                                                                \
         {                                                                                 \
            eina_stringshare_del(ep->error);                                               \
            ep->error = eina_stringshare_printf(                                           \
                "Syntax error on line %d column %d: unexpected token %s:'%s'\n",           \
                TOKEN->sline + 1, TOKEN->scol + 1, yyTokenName[TOKEN->type], TOKEN->text); \
         }                                                                                 \
  } while (0)
#define ERROR_RANGE(TOKEN)  do {                                           \
       eina_stringshare_del(ep->error);                                    \
       ep->error = eina_stringshare_printf(                                \
           "Syntax error on line %d column %d: Out of range value '%s'\n", \
           TOKEN->sline + 1, TOKEN->scol + 1, TOKEN->text);                \
  } while (0)

#endif
