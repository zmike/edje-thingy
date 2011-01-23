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
#include "edje_parser_lib.h"

static void
edje_parser_stream_free(Edje_Stream *s)
{
   if (!s) return;
   if (s->buffer) eina_stringshare_del(s->buffer);
   free(s);
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
                int         length)
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
              int     type,
              int     length)
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
edje_parser_parse(Edje_Parser *parser,
        Edje_Parser_Cb          cb,
        Edje_Parser_New_Cb    new_cb,
        Edje_Parser_Free_Cb     free_cb,
        Edje_Token_Cb           token_cb)
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
edje_parser_string_parse(const char  *str,
               Eina_Bool   *err,
               Edje_Parser_Cb       cb,
               Edje_Parser_New_Cb new_cb,
               Edje_Parser_Free_Cb  free_cb,
               Edje_Token_Cb        token_cb)
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
edje_parser_file_parse(const char  *path,
             Eina_Bool   *err,
             Edje_Parser_Cb       cb,
             Edje_Parser_New_Cb new_cb,
             Edje_Parser_Free_Cb  free_cb,
             Edje_Token_Cb        token_cb)
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

