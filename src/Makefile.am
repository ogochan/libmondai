# libmondai -- MONTSUQI data access library
# Copyright (C) 2005-2009 Ogochan.
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

lib_LTLIBRARIES = libmondai.la rubymondai.la

if SOAP
TESTSOAP  =  testsoap
else
TESTSOAP  =
endif
noinst_PROGRAMS = testfixed testconv testvalue testmem testothers $(TESTSOAP)

libmondai_la_LDFLAGS	=	\
	-version-info $(LTVERSION)	\
	-export-dynamic

AM_CFLAGS	=	-Wall -Wunused -Wredundant-decls -funsigned-char \
	-Wno-pointer-sign

EXTRA_DIST = libmondai.h.in

BUILT_SOURCES = libmondai.h
libmondai.h: libmondai.h.in

libmondaiincludedir = $(includedir)/libmondai

libmondaiinclude_HEADERS =	\
	lock.h					\
	misc_v.h				\
	LBSfunc.h				\
	OpenCOBOL_v.h			\
	dotCOBOL_v.h			\
	Native_v.h				\
	Text_v.h				\
	XML_v.h					\
	php_v.h					\
	json_v.h				\
	SOAP_v.h				\
	cobolvalue.h			\
	others.h				\
	numerici.h				\
	fixed_v.h				\
	hash_v.h				\
	getset.h				\
	value.h					\
	valueconv.h				\
	libmondai.h				\
	Lex.h					\
	RecParser.h				\
	types.h					\
	monstring.h				\
	memory_v.h				\
	numeric.h

INCLUDES 			= 		\
	$(GLIB_CFLAGS) 			\
	$(XML_CFLAGS)			\
	$(RUBY_CFLAGS)			\
	-I$(top_srcdir)

libmondai_la_SOURCES	=	\
	misc.c							\
	lock.c			lock.h			\
	LBSfunc.c		LBSfunc.h		\
	cobolvalue.c	cobolvalue.h	\
	OpenCOBOL.c		OpenCOBOL_v.h	\
	dotCOBOL.c		dotCOBOL_v.h	\
	Native.c		Native_v.h		\
	Text.c			Text_v.h		\
	php.c			php_v.h			\
	json.c			json_v.h		\
	XML.c			XML_v.h			\
	SOAP.c			SOAP_v.h		\
	hash.c			hash_v.h		\
	getset.c		getset.h		\
	value.c			value.h			\
	valueconv.c		valueconv.h		\
	fixed.c			fixed_v.h		\
	numerici.c		numerici.h		\
	others.c		others.h		\
	RecParser.c		RecParser.h		\
	Lex.c			Lex.h			\
	memory.c		memory_v.h		\
	monstring.c		monstring.h		\
	numeric.c		numeric.h		\
	debug.h

rubymondai_la_LDFLAGS	=	\
	-version-info $(LTVERSION)	\
	-module						\
	-export-dynamic
rubymondai_la_SOURCES	=	\
	ruby.c
rubymondai_la_LIBADD 	=	\
	$(GLIB_LIBS)			\
	$(XML_LIBS)				\
	$(RUBY_LIBS)			\
	$(top_builddir)/src/libmondai.la

libmondai_la_LIBADD	=		\
	$(GLIB_LIBS)			\
	$(XML_LIBS)

LDADD 				=		\
	$(GLIB_LIBS)			\
	$(XML_LIBS)				\
	$(top_builddir)/src/libmondai.la

testconv_SOURCES	=		\
	testconv.c

testvalue_SOURCES	=		\
	testvalue.c

if SOAP
testsoap_SOURCES	=		\
	testsoap.c
endif

testmem_SOURCES		=		\
	testmem.c

testfixed_SOURCES		=		\
	testfixed.c

testothers_SOURCES		=		\
	testothers.c
