# Copyright (C) 2019  Ortega Froysa, Nicolás <nicolas@ortegas.org>
# Author: Ortega Froysa, Nicolás <nicolas@ortegas.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=gcc
DEBUG=0
PREFIX=/usr/local
INCFLAGS=
LDFLAGS=-lgmp
DEFS=-DVERSION=\"2.0\" -DAPP_NAME=\"Indivisible\"
CFLAGS=$(INCFLAGS) $(DEFS) -std=c99 -Wall -Wextra -Wfatal-errors -Werror
HDRS=src/globals.h
OBJS=src/main.o

ifeq ($(DEBUG),1)
	CFLAGS+=-g -O0 -DDEBUG
else
	CFLAGS+=-O2 -DNDEBUG
endif

%.o:%.c $(HDRS)
	$(CC) -c -o $@ $< $(CFLAGS)

indivisible: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean distclean install

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) indivisible

install: indivisible
	install -m 755 indivisible $(PREFIX)/bin/
