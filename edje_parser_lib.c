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

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "edje_parser_lib.h"
#include "edje_parser_macros.h"

static void
edje_parser_stream_free(Edje_Stream *s)
{
   if (!s) return;
   if (s->buffer) eina_stringshare_del(s->buffer);
   free(s);
}

/* handle errno more easily */
Eina_Bool
edje_parser_strtol(const char *text,
                   int        *i)
{
   errno = 0;
   *i = strtol(text, NULL, 10);
   if (errno) return EINA_FALSE;

   return EINA_TRUE;
}

Eina_Bool
edje_parser_strtof(const char *text,
                   float      *f)
{
   errno = 0;
   *f = strtof(text, NULL);
   if (errno) return EINA_FALSE;
   if ((*f > 1.0) || (*f < 0.0)) return EINA_FALSE;

   return EINA_TRUE;
}

Eina_Bool
edje_parser_strtod(const char *text,
                   double     *d)
{
   errno = 0;
   *d = strtod(text, NULL);
   if (errno) return EINA_FALSE;
   if (*d < 0.0) return EINA_FALSE;

   return EINA_TRUE;
}

Eina_Bool
edje_parser_strtobool(const char *text,
                      int        *i)
{
   if (!edje_parser_strtol(text, i) || ((*i != 1) && (*i != 0)))
     return EINA_FALSE;
   return EINA_TRUE;
}

Eina_Bool
edje_parser_parse_color(Edje_Parser *ep,
                        Edje_Token  *P,
                        Edje_Token  *I[],
                        int         *color[])
{
   size_t len;
   int *c, i;
   const char *text[4];

   text[0] = I[0]->text;
   text[1] = I[1]->text;
   text[2] = I[2]->text;
   text[3] = I[3]->text;
   len = strlen(P->text); /* check for color*: */
   if ((len < 6) || (len > 7) || (strncmp(P->text, "color", 5)))
     return EINA_FALSE;  /* must set ERROR_SYNTAX in parser */

   if (len == 6) /* color: */
     c = color[0];
   else if (P->text[5] == '2') /* color2: */
     c = color[1];
   else if (P->text[5] == '3') /* color3: */
     c = color[2];
   else
     return EINA_FALSE;  /* must set ERROR_SYNTAX in parser */

   for (i = 0; i < 4; i++)
     {
        if (!edje_parser_strtol(text[i], &c[i]) || (c[i] < 0) || (c[i] > 255))
          {
             ERROR_RANGE(I[i]);
             return EINA_FALSE;
          }
     }
   return EINA_TRUE;
}

Eina_Bool
edje_parser_parse_min(Edje_Parser *ep,
                      Edje_Token  *I,
                      Edje_Token  *J,
                      int         *min,
                      int         *max)
{
   if ((!I) || (!J) || (!min) || (!max)) return EINA_FALSE;

   if ((!edje_parser_strtol(I->text, &min[0])) || (min[0] < 0))
     {
        ERROR_RANGE(I);
        return EINA_FALSE;
     }
   else if ((max[0] && (max[0] < min[0])) || (max[1] && (max[1] < min[1])))
     {
        ep->error = eina_stringshare_printf(
            "Syntax error on line %d column %d: max cannot be smaller than min!\n",
            I->sline + 1, I->scol + 1);
        ERR("%s", ep->error);
        return EINA_FALSE;
     }
   if ((!edje_parser_strtol(J->text, &min[1])) || (min[1] < 0))
     {
        ERROR_RANGE(J);
        return EINA_FALSE;
     }
   else if ((max[0] && (max[0] < min[0])) || (max[1] && (max[1] < min[1])))
     {
        ep->error = eina_stringshare_printf(
            "Syntax error on line %d column %d: max cannot be smaller than min!\n",
            J->sline + 1, J->scol + 1);
        ERR("%s", ep->error);
        return EINA_FALSE;
     }
   return EINA_TRUE;
}

Eina_Bool
edje_parser_parse_max(Edje_Parser *ep,
                      Edje_Token  *I,
                      Edje_Token  *J,
                      int         *min,
                      int         *max)
{
   if ((!I) || (!J) || (!min) || (!max)) return EINA_FALSE;

   if ((!edje_parser_strtol(I->text, &max[0])) || (*max < 0))
     {
        ERROR_RANGE(I);
        return EINA_FALSE;
     }
   else if ((max[0] && (max[0] < min[0])) || (max[1] && (max[1] < min[1])))
     {
        ep->error = eina_stringshare_printf(
            "Syntax error on line %d column %d: max cannot be smaller than min!\n",
            I->sline + 1, I->scol + 1);
        ERR("%s", ep->error);
        return EINA_FALSE;
     }
   if ((!edje_parser_strtol(J->text, &max[1])) || (max[1] < 0))
     {
        ERROR_RANGE(J);
        return EINA_FALSE;
     }
   else if ((max[0] && (max[0] < min[0])) || (max[1] && (max[1] < min[1])))
     {
        ep->error = eina_stringshare_printf(
            "Syntax error on line %d column %d: max cannot be smaller than min!\n",
            J->sline + 1, J->scol + 1);
        ERR("%s", ep->error);
        return EINA_FALSE;
     }
   return EINA_TRUE;
}

Edje_Stream *
edje_parser_stream_new(const char *str)
{
   Edje_Stream *s;

   if (!str) return NULL;

   s = calloc(1, sizeof(Edje_Stream));
   EINA_SAFETY_ON_NULL_RETURN_VAL(s, NULL);
   s->buffer = eina_stringshare_add(str);
   s->length = eina_stringshare_strlen(s->buffer);
   return s;
}

void
edje_parser_stream_next(Edje_Stream *s,
                        int          length)
{
   int i = 0;

   EINA_SAFETY_ON_NULL_RETURN(s);

   for (; i < length && s->index < s->length; i++, s->index++)
     {
        s->col++;

        if (s->buffer[s->index] == '\n')
          {
             s->col = 0;
             s->line++;
          }
     }
}

Edje_Token *
edje_parser_token_new(Edje_Stream *s,
                      int          type,
                      int          length)
{
   int i;
   Edje_Token *t;

   EINA_SAFETY_ON_NULL_RETURN_VAL(s, NULL);

   t = calloc(1, sizeof(Edje_Token));
   EINA_SAFETY_ON_NULL_RETURN_VAL(t, NULL);

   t->type = type;
   t->text = eina_stringshare_add_length(s->buffer + s->index, length);
   t->length = length;
   t->sline = t->eline = s->line;
   t->scol = t->ecol = s->col;

   for (i = 0; i < length; i++)
     {
        t->ecol++;

        if (t->text[i] == '\n')
          {
             t->ecol = 0;
             t->eline++;
          }
     }

   edje_parser_stream_next(s, length);
   return t;
}

void
edje_parser_token_free(Edje_Token *t)
{
   EINA_SAFETY_ON_NULL_RETURN(t);

   eina_stringshare_del(t->text);
   free(t);
}

static int
edje_parser_parse(Edje_Parser        *parser,
                  Edje_Parser_Cb      cb,
                  Edje_Parser_New_Cb  new_cb,
                  Edje_Parser_Free_Cb free_cb,
                  Edje_Token_Cb       token_cb)
{
   void *p;
   Edje_Token *t;
   Edje_Stream *s;
   int retval = -1;

   if ((!parser) || (parser->error) || (!parser->stream))
     return retval;

   s = parser->stream;
   p = new_cb(malloc);

   while ((t = token_cb(s)))
     {
        if (t->type < 0)
          {
             parser->error = eina_stringshare_printf("Unknown token '%s' at line %d char %d\n", t->text, t->sline + 1, t->scol + 1);
             ERR("%s", parser->error);
             edje_parser_token_free(t);
             goto err;
          }
        else if (!t->type)
          {
             edje_parser_token_free(t);
             cb(p, 0, NULL, parser);
             break;
          }

        cb(p, t->type, t, parser);

        if (parser->error)
          goto err;
     }

   retval = 0;
err:
   free_cb(p, free);
   return retval;
}

Edje *
edje_parser_string_parse(const char         *str,
                         Eina_Bool          *err,
                         Edje_Parser_Cb      cb,
                         Edje_Parser_New_Cb  new_cb,
                         Edje_Parser_Free_Cb free_cb,
                         Edje_Token_Cb       token_cb)
{
   Edje_Parser *parser;
   Edje *ret;

   EINA_SAFETY_ON_NULL_RETURN_VAL(str, NULL);

   parser = calloc(1, sizeof(Edje_Parser));
   EINA_SAFETY_ON_NULL_RETURN_VAL(parser, NULL);

   parser->stream = edje_parser_stream_new(str);

   if (!parser->stream)
     {
        free(parser);
        fprintf(stderr, "Stream can't be created!");
        *err = EINA_TRUE;
        return NULL;
     }

   edje_parser_parse(parser, cb, new_cb, free_cb, token_cb);

   if (parser->error)
     {
        fprintf(stderr, "%s\n", parser->error);
        *err = EINA_TRUE;
     }

   ret = parser->edje;
   edje_parser_stream_free(parser->stream);
   free(parser);
   return ret;
}

Edje *
edje_parser_file_parse(const char         *path,
                       Eina_Bool          *err,
                       Edje_Parser_Cb      cb,
                       Edje_Parser_New_Cb  new_cb,
                       Edje_Parser_Free_Cb free_cb,
                       Edje_Token_Cb       token_cb)
{
   void *buf;
   struct stat s;
   int fd;
   Edje *data;

   if (stat(path, &s) < 0) return NULL;
   if ((fd = open(path, O_RDONLY)) < 0) return NULL;

   buf = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
   close(fd);
   if (buf == MAP_FAILED) return NULL;

   data = edje_parser_string_parse(buf, err, cb, new_cb, free_cb, token_cb);
   munmap(buf, s.st_size);
   return data;
}

