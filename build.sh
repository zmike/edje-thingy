#!/bin/bash
CFLAGS="-O0 -pipe -Wall -Wextra -g -ggdb3 $(pkg-config --cflags --libs ecore-file) -DHAVE_ALLOCA_H -D_GNU_SOURCE"

[[ -f lemon ]] || gcc -o lemon lemon.c -g

re2c -bi -o edje_parser.y edje_parser.yre
./lemon -q edje_parser.y
gcc $CFLAGS -I. -o edje_parser edje*.c
