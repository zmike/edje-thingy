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

#include <Eina.h>
#include <Ecore.h>
#include "edje_parser_lib.h"

int edje_parser_log_dom = 0;

static Edje *edje;
static char *edje_file;

#if 0
static const Ecore_Getopt opts = {
   "Edje_Parser",
   "Edje_Parser file.edje -o destination_directory/",
   "1.0alpha",
   "(C) 2011 Mike Blumenkrantz, previously others, see AUTHORS",
   "LGPL",
   "Parse an edje file into *.{c,h} files\n\n",
   1,
   {
      ECORE_GETOPT_STORE_STR('m', "modes", "Parser modes: all, server-impl, server-headers,\n"
                                           "\t\t\tclient-impl, client-headers,\n"
                                           "\t\t\tcommon-impl, common-headers"),
      ECORE_GETOPT_STORE_STR('o', "output", "Output directory (default is .)"),
      ECORE_GETOPT_STORE_TRUE('d', "debug", "Print debugging output"),
      ECORE_GETOPT_STORE_TRUE('H', "Hash", "Do not generate hash functions"),
      ECORE_GETOPT_STORE_TRUE('n', "null", "Do not generate isnull functions"),
      ECORE_GETOPT_STORE_TRUE('p', "print", "Do not generate print functions"),
      ECORE_GETOPT_STORE_TRUE('e', "eq", "Do not generate eq functions"),
      ECORE_GETOPT_VERSION('V', "version"),
      ECORE_GETOPT_COPYRIGHT('R', "copyright"),
      ECORE_GETOPT_LICENSE('L', "license"),
      ECORE_GETOPT_HELP('h', "help"),
      ECORE_GETOPT_SENTINEL
   }
};
#endif

extern void edje_parser_Trace(FILE *,
                              char *);


int
main(int   argc,
     char *argv[])
{
   Eina_Bool err;
#if 0
   Eina_Bool debug = EINA_FALSE;
   Eina_Bool exit_option = EINA_FALSE;
   int args;
   char *modes = "all";

   Ecore_Getopt_Value values[] =
   {
      ECORE_GETOPT_VALUE_STR(modes),
      ECORE_GETOPT_VALUE_STR(out_dir),
      ECORE_GETOPT_VALUE_BOOL(debug),
      ECORE_GETOPT_VALUE_BOOL(hash_funcs),
      ECORE_GETOPT_VALUE_BOOL(isnull_funcs),
      ECORE_GETOPT_VALUE_BOOL(print_funcs),
      ECORE_GETOPT_VALUE_BOOL(eq_funcs),
      ECORE_GETOPT_VALUE_BOOL(exit_option),
      ECORE_GETOPT_VALUE_BOOL(exit_option),
      ECORE_GETOPT_VALUE_BOOL(exit_option),
      ECORE_GETOPT_VALUE_BOOL(exit_option)
   };
#endif
   eina_init();
   ecore_init();
   ecore_app_args_set(argc, (const char **)argv);
   edje_parser_log_dom = eina_log_domain_register("edje_parser", EINA_COLOR_YELLOW);
   eina_log_domain_level_set("edje_parser", EINA_LOG_LEVEL_DBG);
/*
   args = ecore_getopt_parse(&opts, values, argc, argv);
   if (args < 0)
     return 1;

   if (exit_option)
     return 0;
 */
   edje_file = argv[1];

   if (!edje_file)
     {
        printf("You must specify the .edc file.\n");
        return 1;
     }

   edje_parser_Trace(stdout, "Edje_Parser: ");
   err = EINA_FALSE;
   edje = edje_parse_file(edje_file, &err);
   if ((!edje) || (err))
     {
        printf("Error parsing file!\n");
        exit(1);
     }

   //if (debug)
   printf("edje-parser: Done!!\n");
   return 0;
}

