# Makefile.in generated by automake 1.14.1 from Makefile.am.
# cipher/Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994-2013 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



# Makefile for cipher modules
# Copyright (C) 1998, 1999, 2000, 2001, 2002,
#               2003, 2009 Free Software Foundation, Inc.
#
# This file is part of Libgcrypt.
#
# Libgcrypt is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of
# the License, or (at your option) any later version.
#
# Libgcrypt is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program; if not, see <http://www.gnu.org/licenses/>.

# Process this file with automake to produce Makefile.in


am__is_gnu_make = test -n '$(MAKEFILE_LIST)' && test -n '$(MAKELEVEL)'
am__make_running_with_option = \
  case $${target_option-} in \
      ?) ;; \
      *) echo "am__make_running_with_option: internal error: invalid" \
              "target option '$${target_option-}' specified" >&2; \
         exit 1;; \
  esac; \
  has_opt=no; \
  sane_makeflags=$$MAKEFLAGS; \
  if $(am__is_gnu_make); then \
    sane_makeflags=$$MFLAGS; \
  else \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        bs=\\; \
        sane_makeflags=`printf '%s\n' "$$MAKEFLAGS" \
          | sed "s/$$bs$$bs[$$bs $$bs	]*//g"`;; \
    esac; \
  fi; \
  skip_next=no; \
  strip_trailopt () \
  { \
    flg=`printf '%s\n' "$$flg" | sed "s/$$1.*$$//"`; \
  }; \
  for flg in $$sane_makeflags; do \
    test $$skip_next = yes && { skip_next=no; continue; }; \
    case $$flg in \
      *=*|--*) continue;; \
        -*I) strip_trailopt 'I'; skip_next=yes;; \
      -*I?*) strip_trailopt 'I';; \
        -*O) strip_trailopt 'O'; skip_next=yes;; \
      -*O?*) strip_trailopt 'O';; \
        -*l) strip_trailopt 'l'; skip_next=yes;; \
      -*l?*) strip_trailopt 'l';; \
      -[dEDm]) skip_next=yes;; \
      -[JT]) skip_next=yes;; \
    esac; \
    case $$flg in \
      *$$target_option*) has_opt=yes; break;; \
    esac; \
  done; \
  test $$has_opt = yes
am__make_dryrun = (target_option=n; $(am__make_running_with_option))
am__make_keepgoing = (target_option=k; $(am__make_running_with_option))
pkgdatadir = $(datadir)/libgcrypt
pkgincludedir = $(includedir)/libgcrypt
pkglibdir = $(libdir)/libgcrypt
pkglibexecdir = $(libexecdir)/libgcrypt
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = x86_64-unknown-linux-gnu
host_triplet = x86_64-unknown-linux-gnu
subdir = cipher
DIST_COMMON = $(srcdir)/Makefile.in $(srcdir)/Makefile.am \
	$(top_srcdir)/build-aux/depcomp
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/m4/gpg-error.m4 \
	$(top_srcdir)/m4/libtool.m4 $(top_srcdir)/m4/lock.m4 \
	$(top_srcdir)/m4/ltoptions.m4 $(top_srcdir)/m4/ltsugar.m4 \
	$(top_srcdir)/m4/ltversion.m4 $(top_srcdir)/m4/lt~obsolete.m4 \
	$(top_srcdir)/m4/noexecstack.m4 $(top_srcdir)/m4/onceonly.m4 \
	$(top_srcdir)/m4/socklen.m4 $(top_srcdir)/m4/sys_socket_h.m4 \
	$(top_srcdir)/m4/threadlib.m4 $(top_srcdir)/acinclude.m4 \
	$(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
LTLIBRARIES = $(noinst_LTLIBRARIES)
am__DEPENDENCIES_1 =
am_libcipher_la_OBJECTS = cipher.lo cipher-cbc.lo cipher-cfb.lo \
	cipher-ofb.lo cipher-ctr.lo cipher-aeswrap.lo cipher-ccm.lo \
	cipher-cmac.lo cipher-gcm.lo cipher-selftest.lo pubkey.lo \
	pubkey-util.lo md.lo mac.lo mac-hmac.lo mac-cmac.lo \
	mac-gmac.lo kdf.lo hmac-tests.lo primegen.lo hash-common.lo \
	dsa-common.lo rsa-common.lo
libcipher_la_OBJECTS = $(am_libcipher_la_OBJECTS)
AM_V_lt = $(am__v_lt_$(V))
am__v_lt_ = $(am__v_lt_$(AM_DEFAULT_VERBOSITY))
am__v_lt_0 = --silent
am__v_lt_1 = 
AM_V_P = $(am__v_P_$(V))
am__v_P_ = $(am__v_P_$(AM_DEFAULT_VERBOSITY))
am__v_P_0 = false
am__v_P_1 = :
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN     " $@;
am__v_GEN_1 = 
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
am__v_at_1 = 
DEFAULT_INCLUDES = -I. -I$(top_builddir)
depcomp = $(SHELL) $(top_srcdir)/build-aux/depcomp
am__depfiles_maybe = depfiles
am__mv = mv -f
CPPASCOMPILE = $(CCAS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CCASFLAGS) $(CCASFLAGS)
LTCPPASCOMPILE = $(LIBTOOL) $(AM_V_lt) $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CCAS) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CCASFLAGS) $(CCASFLAGS)
AM_V_CPPAS = $(am__v_CPPAS_$(V))
am__v_CPPAS_ = $(am__v_CPPAS_$(AM_DEFAULT_VERBOSITY))
am__v_CPPAS_0 = @echo "  CPPAS   " $@;
am__v_CPPAS_1 = 
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CC) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CFLAGS) $(CFLAGS)
AM_V_CC = $(am__v_CC_$(V))
am__v_CC_ = $(am__v_CC_$(AM_DEFAULT_VERBOSITY))
am__v_CC_0 = @echo "  CC      " $@;
am__v_CC_1 = 
CCLD = $(CC)
LINK = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
AM_V_CCLD = $(am__v_CCLD_$(V))
am__v_CCLD_ = $(am__v_CCLD_$(AM_DEFAULT_VERBOSITY))
am__v_CCLD_0 = @echo "  CCLD    " $@;
am__v_CCLD_1 = 
SOURCES = $(libcipher_la_SOURCES) $(EXTRA_libcipher_la_SOURCES)
DIST_SOURCES = $(libcipher_la_SOURCES) $(EXTRA_libcipher_la_SOURCES)
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
# Read a list of newline-separated strings from the standard input,
# and print each of them once, without duplicates.  Input order is
# *not* preserved.
am__uniquify_input = $(AWK) '\
  BEGIN { nonempty = 0; } \
  { items[$$0] = 1; nonempty = 1; } \
  END { if (nonempty) { for (i in items) print i; }; } \
'
# Make sure the list of sources is unique.  This is necessary because,
# e.g., the same source file might be shared among _SOURCES variables
# for different programs/libraries.
am__define_uniq_tagged_files = \
  list='$(am__tagged_files)'; \
  unique=`for i in $$list; do \
    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
  done | $(am__uniquify_input)`
ETAGS = etags
CTAGS = ctags
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
ACLOCAL = ${SHELL} /source_depot/libgcrypt-1.6.4/build-aux/missing aclocal-1.14
AMTAR = $${TAR-tar}
AM_DEFAULT_VERBOSITY = 1
AR = ar
AS = as
AUTOCONF = ${SHELL} /source_depot/libgcrypt-1.6.4/build-aux/missing autoconf
AUTOHEADER = ${SHELL} /source_depot/libgcrypt-1.6.4/build-aux/missing autoheader
AUTOMAKE = ${SHELL} /source_depot/libgcrypt-1.6.4/build-aux/missing automake-1.14
AWK = gawk
BUILD_FILEVERSION = 1,6,4,29416
BUILD_REVISION = 72e8c27
BUILD_TIMESTAMP = 2015-09-11T05:38+0000
CC = gcc
CCAS = gcc
CCASDEPMODE = depmode=gcc3
CCASFLAGS = -g -O2
CCDEPMODE = depmode=gcc3
CC_FOR_BUILD = gcc
CFLAGS = -g -O2 -fvisibility=hidden -Wall
CPP = gcc -E
CPPFLAGS = 
CYGPATH_W = echo
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DLLTOOL = false
DL_LIBS = 
DSYMUTIL = 
DUMPBIN = 
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /bin/grep -E
EXEEXT = 
FALLBACK_SOCKLEN_T = typedef socklen_t gcry_socklen_t;
FGREP = /bin/grep -F
GCRYPT_CIPHERS =  arcfour.lo blowfish.lo blowfish-amd64.lo cast5.lo cast5-amd64.lo des.lo rijndael.lo rijndael-amd64.lo twofish.lo twofish-amd64.lo serpent.lo serpent-sse2-amd64.lo serpent-avx2-amd64.lo rfc2268.lo seed.lo camellia.lo camellia-glue.lo camellia-aesni-avx-amd64.lo camellia-aesni-avx2-amd64.lo idea.lo salsa20.lo salsa20-amd64.lo gost28147.lo
GCRYPT_DIGESTS =  crc.lo gostr3411-94.lo stribog.lo md4.lo md5.lo sha256.lo sha256-ssse3-amd64.lo sha512.lo sha512-ssse3-amd64.lo sha512-avx-amd64.lo sha512-avx2-bmi2-amd64.lo tiger.lo whirlpool.lo rmd160.lo sha1.lo sha1-ssse3-amd64.lo
GCRYPT_HWF_MODULES = hwf-x86.lo
GCRYPT_KDFS =  scrypt.lo
GCRYPT_PUBKEY_CIPHERS =  dsa.lo rsa.lo elgamal.lo                           ecc.lo ecc-curves.lo ecc-misc.lo                           ecc-ecdsa.lo ecc-eddsa.lo ecc-gost.lo
GCRYPT_RANDOM =  rndlinux.lo
GPG_ERROR_CFLAGS = -I/opt/local/include
GPG_ERROR_CONFIG = /opt/local/bin/gpg-error-config
GPG_ERROR_LIBS = -L/opt/local/lib -lgpg-error
GREP = /bin/grep
INSERT_SYS_SELECT_H =  include <sys/select.h>
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
LD = /usr/bin/ld -m elf_x86_64
LDFLAGS = 
LIBGCRYPT_CIPHERS = arcfour blowfish cast5 des aes twofish serpent rfc2268 seed camellia idea salsa20 gost28147
LIBGCRYPT_CONFIG_API_VERSION = 1
LIBGCRYPT_CONFIG_CFLAGS = 
LIBGCRYPT_CONFIG_HOST = x86_64-unknown-linux-gnu
LIBGCRYPT_CONFIG_LIBS = -lgcrypt
LIBGCRYPT_DIGESTS = crc gostr3411-94 md4 md5 rmd160 sha1 sha256 sha512 tiger whirlpool stribog
LIBGCRYPT_LT_AGE = 0
LIBGCRYPT_LT_CURRENT = 20
LIBGCRYPT_LT_REVISION = 4
LIBGCRYPT_PUBKEY_CIPHERS = dsa elgamal rsa ecc
LIBGCRYPT_THREAD_MODULES = 
LIBMULTITHREAD = -lpthread
LIBOBJS = 
LIBS = 
LIBTHREAD = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIPO = 
LN_S = ln -s
LTLIBMULTITHREAD = -lpthread
LTLIBOBJS = 
LTLIBTHREAD = 
MAINT = #
MAKEINFO = ${SHELL} /source_depot/libgcrypt-1.6.4/build-aux/missing makeinfo
MANIFEST_TOOL = :
MKDIR_P = /bin/mkdir -p
MPI_SFLAGS = 
NM = /usr/bin/nm -B
NMEDIT = 
NOEXECSTACK_FLAGS = -Wa,--noexecstack
OBJDUMP = objdump
OBJEXT = o
OTOOL = 
OTOOL64 = 
PACKAGE = libgcrypt
PACKAGE_BUGREPORT = http://bugs.gnupg.org
PACKAGE_NAME = libgcrypt
PACKAGE_STRING = libgcrypt 1.6.4
PACKAGE_TARNAME = libgcrypt
PACKAGE_URL = 
PACKAGE_VERSION = 1.6.4
PATH_SEPARATOR = :
PTH_CFLAGS = 
PTH_CONFIG = 
PTH_LIBS = 
RANLIB = ranlib
RC = 
RUN_LARGE_DATA_TESTS = no
SED = /bin/sed
SET_MAKE = 
SHELL = /bin/sh
STRIP = strip
SYS_SOCKET_H = 
VERSION = 1.6.4
VERSION_NUMBER = 0x010604
abs_builddir = /source_depot/libgcrypt-1.6.4/cipher
abs_srcdir = /source_depot/libgcrypt-1.6.4/cipher
abs_top_builddir = /source_depot/libgcrypt-1.6.4
abs_top_srcdir = /source_depot/libgcrypt-1.6.4
ac_ct_AR = ar
ac_ct_CC = gcc
ac_ct_DUMPBIN = 
am__include = include
am__leading_dot = .
am__quote = 
am__tar = $${TAR-tar} chof - "$$tardir"
am__untar = $${TAR-tar} xf -
bindir = ${exec_prefix}/bin
build = x86_64-unknown-linux-gnu
build_alias = 
build_cpu = x86_64
build_os = linux-gnu
build_vendor = unknown
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
emacs_local_vars_begin = Local Variables:
emacs_local_vars_end = End:
emacs_local_vars_read_only = buffer-read-only: t
exec_prefix = /opt/local
host = x86_64-unknown-linux-gnu
host_alias = 
host_cpu = x86_64
host_os = linux-gnu
host_vendor = unknown
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /source_depot/libgcrypt-1.6.4/build-aux/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
mandir = ${datarootdir}/man
mkdir_p = $(MKDIR_P)
oldincludedir = /usr/include
pdfdir = ${docdir}
prefix = /opt/local
program_transform_name = s,x,x,
psdir = ${docdir}
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = ${prefix}/etc
target_alias = 
top_build_prefix = ../
top_builddir = ..
top_srcdir = ..
EXTRA_DIST = Manifest

# Need to include ../src in addition to top_srcdir because gcrypt.h is
# a built header.
AM_CPPFLAGS = -I../src -I$(top_srcdir)/src
AM_CFLAGS = $(GPG_ERROR_CFLAGS)
AM_CCASFLAGS = $(NOEXECSTACK_FLAGS)
noinst_LTLIBRARIES = libcipher.la
GCRYPT_MODULES =  arcfour.lo blowfish.lo blowfish-amd64.lo cast5.lo cast5-amd64.lo des.lo rijndael.lo rijndael-amd64.lo twofish.lo twofish-amd64.lo serpent.lo serpent-sse2-amd64.lo serpent-avx2-amd64.lo rfc2268.lo seed.lo camellia.lo camellia-glue.lo camellia-aesni-avx-amd64.lo camellia-aesni-avx2-amd64.lo idea.lo salsa20.lo salsa20-amd64.lo gost28147.lo  dsa.lo rsa.lo elgamal.lo                           ecc.lo ecc-curves.lo ecc-misc.lo                           ecc-ecdsa.lo ecc-eddsa.lo ecc-gost.lo \
                  crc.lo gostr3411-94.lo stribog.lo md4.lo md5.lo sha256.lo sha256-ssse3-amd64.lo sha512.lo sha512-ssse3-amd64.lo sha512-avx-amd64.lo sha512-avx2-bmi2-amd64.lo tiger.lo whirlpool.lo rmd160.lo sha1.lo sha1-ssse3-amd64.lo  scrypt.lo

libcipher_la_DEPENDENCIES = $(GCRYPT_MODULES)
libcipher_la_LIBADD = $(GCRYPT_MODULES)
libcipher_la_SOURCES = \
cipher.c cipher-internal.h \
cipher-cbc.c cipher-cfb.c cipher-ofb.c cipher-ctr.c cipher-aeswrap.c \
cipher-ccm.c cipher-cmac.c cipher-gcm.c \
cipher-selftest.c cipher-selftest.h \
pubkey.c pubkey-internal.h pubkey-util.c \
md.c \
mac.c mac-internal.h \
mac-hmac.c mac-cmac.c mac-gmac.c \
kdf.c kdf-internal.h \
hmac-tests.c \
bithelp.h  \
bufhelp.h  \
primegen.c  \
hash-common.c hash-common.h \
dsa-common.c rsa-common.c \
rmd.h

EXTRA_libcipher_la_SOURCES = \
arcfour.c \
blowfish.c blowfish-amd64.S blowfish-arm.S \
cast5.c cast5-amd64.S cast5-arm.S \
crc.c \
des.c \
dsa.c \
elgamal.c \
ecc.c ecc-curves.c ecc-misc.c ecc-common.h \
ecc-ecdsa.c ecc-eddsa.c ecc-gost.c \
idea.c \
gost28147.c gost.h \
gostr3411-94.c \
md4.c \
md5.c \
rijndael.c rijndael-tables.h rijndael-amd64.S rijndael-arm.S \
rmd160.c \
rsa.c \
salsa20.c salsa20-amd64.S salsa20-armv7-neon.S \
scrypt.c \
seed.c \
serpent.c serpent-sse2-amd64.S serpent-avx2-amd64.S serpent-armv7-neon.S \
sha1.c sha1-ssse3-amd64.S \
sha256.c sha256-ssse3-amd64.S \
sha512.c sha512-ssse3-amd64.S sha512-avx-amd64.S sha512-avx2-bmi2-amd64.S \
  sha512-armv7-neon.S \
stribog.c \
tiger.c \
whirlpool.c \
twofish.c twofish-amd64.S twofish-arm.S \
rfc2268.c \
camellia.c camellia.h camellia-glue.c camellia-aesni-avx-amd64.S \
  camellia-aesni-avx2-amd64.S camellia-arm.S

#o_flag_munging = cat
o_flag_munging = sed -e 's/-O\([2-9s][2-9s]*\)/-O1/' -e 's/-Ofast/-O1/g'
all: all-am

.SUFFIXES:
.SUFFIXES: .S .c .lo .o .obj
$(srcdir)/Makefile.in: # $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --gnu cipher/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --gnu cipher/Makefile
.PRECIOUS: Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe);; \
	esac;

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh

$(top_srcdir)/configure: # $(am__configure_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(ACLOCAL_M4): # $(am__aclocal_m4_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(am__aclocal_m4_deps):

clean-noinstLTLIBRARIES:
	-test -z "$(noinst_LTLIBRARIES)" || rm -f $(noinst_LTLIBRARIES)
	@list='$(noinst_LTLIBRARIES)'; \
	locs=`for p in $$list; do echo $$p; done | \
	      sed 's|^[^/]*$$|.|; s|/[^/]*$$||; s|$$|/so_locations|' | \
	      sort -u`; \
	test -z "$$locs" || { \
	  echo rm -f $${locs}; \
	  rm -f $${locs}; \
	}

libcipher.la: $(libcipher_la_OBJECTS) $(libcipher_la_DEPENDENCIES) $(EXTRA_libcipher_la_DEPENDENCIES) 
	$(AM_V_CCLD)$(LINK)  $(libcipher_la_OBJECTS) $(libcipher_la_LIBADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT)

distclean-compile:
	-rm -f *.tab.c

include ./$(DEPDIR)/arcfour.Plo
include ./$(DEPDIR)/blowfish-amd64.Plo
include ./$(DEPDIR)/blowfish-arm.Plo
include ./$(DEPDIR)/blowfish.Plo
include ./$(DEPDIR)/camellia-aesni-avx-amd64.Plo
include ./$(DEPDIR)/camellia-aesni-avx2-amd64.Plo
include ./$(DEPDIR)/camellia-arm.Plo
include ./$(DEPDIR)/camellia-glue.Plo
include ./$(DEPDIR)/camellia.Plo
include ./$(DEPDIR)/cast5-amd64.Plo
include ./$(DEPDIR)/cast5-arm.Plo
include ./$(DEPDIR)/cast5.Plo
include ./$(DEPDIR)/cipher-aeswrap.Plo
include ./$(DEPDIR)/cipher-cbc.Plo
include ./$(DEPDIR)/cipher-ccm.Plo
include ./$(DEPDIR)/cipher-cfb.Plo
include ./$(DEPDIR)/cipher-cmac.Plo
include ./$(DEPDIR)/cipher-ctr.Plo
include ./$(DEPDIR)/cipher-gcm.Plo
include ./$(DEPDIR)/cipher-ofb.Plo
include ./$(DEPDIR)/cipher-selftest.Plo
include ./$(DEPDIR)/cipher.Plo
include ./$(DEPDIR)/crc.Plo
include ./$(DEPDIR)/des.Plo
include ./$(DEPDIR)/dsa-common.Plo
include ./$(DEPDIR)/dsa.Plo
include ./$(DEPDIR)/ecc-curves.Plo
include ./$(DEPDIR)/ecc-ecdsa.Plo
include ./$(DEPDIR)/ecc-eddsa.Plo
include ./$(DEPDIR)/ecc-gost.Plo
include ./$(DEPDIR)/ecc-misc.Plo
include ./$(DEPDIR)/ecc.Plo
include ./$(DEPDIR)/elgamal.Plo
include ./$(DEPDIR)/gost28147.Plo
include ./$(DEPDIR)/gostr3411-94.Plo
include ./$(DEPDIR)/hash-common.Plo
include ./$(DEPDIR)/hmac-tests.Plo
include ./$(DEPDIR)/idea.Plo
include ./$(DEPDIR)/kdf.Plo
include ./$(DEPDIR)/mac-cmac.Plo
include ./$(DEPDIR)/mac-gmac.Plo
include ./$(DEPDIR)/mac-hmac.Plo
include ./$(DEPDIR)/mac.Plo
include ./$(DEPDIR)/md.Plo
include ./$(DEPDIR)/md4.Plo
include ./$(DEPDIR)/md5.Plo
include ./$(DEPDIR)/primegen.Plo
include ./$(DEPDIR)/pubkey-util.Plo
include ./$(DEPDIR)/pubkey.Plo
include ./$(DEPDIR)/rfc2268.Plo
include ./$(DEPDIR)/rijndael-amd64.Plo
include ./$(DEPDIR)/rijndael-arm.Plo
include ./$(DEPDIR)/rijndael.Plo
include ./$(DEPDIR)/rmd160.Plo
include ./$(DEPDIR)/rsa-common.Plo
include ./$(DEPDIR)/rsa.Plo
include ./$(DEPDIR)/salsa20-amd64.Plo
include ./$(DEPDIR)/salsa20-armv7-neon.Plo
include ./$(DEPDIR)/salsa20.Plo
include ./$(DEPDIR)/scrypt.Plo
include ./$(DEPDIR)/seed.Plo
include ./$(DEPDIR)/serpent-armv7-neon.Plo
include ./$(DEPDIR)/serpent-avx2-amd64.Plo
include ./$(DEPDIR)/serpent-sse2-amd64.Plo
include ./$(DEPDIR)/serpent.Plo
include ./$(DEPDIR)/sha1-ssse3-amd64.Plo
include ./$(DEPDIR)/sha1.Plo
include ./$(DEPDIR)/sha256-ssse3-amd64.Plo
include ./$(DEPDIR)/sha256.Plo
include ./$(DEPDIR)/sha512-armv7-neon.Plo
include ./$(DEPDIR)/sha512-avx-amd64.Plo
include ./$(DEPDIR)/sha512-avx2-bmi2-amd64.Plo
include ./$(DEPDIR)/sha512-ssse3-amd64.Plo
include ./$(DEPDIR)/sha512.Plo
include ./$(DEPDIR)/stribog.Plo
include ./$(DEPDIR)/tiger.Plo
include ./$(DEPDIR)/twofish-amd64.Plo
include ./$(DEPDIR)/twofish-arm.Plo
include ./$(DEPDIR)/twofish.Plo
include ./$(DEPDIR)/whirlpool.Plo

.S.o:
	$(AM_V_CPPAS)$(CPPASCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CPPAS)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCASDEPMODE) $(depcomp) \
#	$(AM_V_CPPAS_no)$(CPPASCOMPILE) -c -o $@ $<

.S.obj:
	$(AM_V_CPPAS)$(CPPASCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `$(CYGPATH_W) '$<'`
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CPPAS)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCASDEPMODE) $(depcomp) \
#	$(AM_V_CPPAS_no)$(CPPASCOMPILE) -c -o $@ `$(CYGPATH_W) '$<'`

.S.lo:
	$(AM_V_CPPAS)$(LTCPPASCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
#	$(AM_V_CPPAS)source='$<' object='$@' libtool=yes \
#	DEPDIR=$(DEPDIR) $(CCASDEPMODE) $(depcomp) \
#	$(AM_V_CPPAS_no)$(LTCPPASCOMPILE) -c -o $@ $<

.c.o:
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CC)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(COMPILE) -c -o $@ $<

.c.obj:
	$(AM_V_CC)$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `$(CYGPATH_W) '$<'`
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	$(AM_V_CC)source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(COMPILE) -c -o $@ `$(CYGPATH_W) '$<'`

.c.lo:
	$(AM_V_CC)$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
#	$(AM_V_CC)source='$<' object='$@' libtool=yes \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(AM_V_CC_no)$(LTCOMPILE) -c -o $@ $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

ID: $(am__tagged_files)
	$(am__define_uniq_tagged_files); mkid -fID $$unique
tags: tags-am
TAGS: tags

tags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	set x; \
	here=`pwd`; \
	$(am__define_uniq_tagged_files); \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: ctags-am

CTAGS: ctags
ctags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	$(am__define_uniq_tagged_files); \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"
cscopelist: cscopelist-am

cscopelist-am: $(am__tagged_files)
	list='$(am__tagged_files)'; \
	case "$(srcdir)" in \
	  [\\/]* | ?:[\\/]*) sdir="$(srcdir)" ;; \
	  *) sdir=$(subdir)/$(srcdir) ;; \
	esac; \
	for i in $$list; do \
	  if test -f "$$i"; then \
	    echo "$(subdir)/$$i"; \
	  else \
	    echo "$$sdir/$$i"; \
	  fi; \
	done >> $(top_builddir)/cscope.files

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags

distdir: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(LTLIBRARIES)
installdirs:
install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
mostlyclean-generic:

clean-generic:

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-libtool clean-noinstLTLIBRARIES \
	mostlyclean-am

distclean: distclean-am
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
distclean-am: clean-am distclean-compile distclean-generic \
	distclean-tags

dvi: dvi-am

dvi-am:

html: html-am

html-am:

info: info-am

info-am:

install-data-am:

install-dvi: install-dvi-am

install-dvi-am:

install-exec-am:

install-html: install-html-am

install-html-am:

install-info: install-info-am

install-info-am:

install-man:

install-pdf: install-pdf-am

install-pdf-am:

install-ps: install-ps-am

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-am
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

pdf: pdf-am

pdf-am:

ps: ps-am

ps-am:

uninstall-am:

.MAKE: install-am install-strip

.PHONY: CTAGS GTAGS TAGS all all-am check check-am clean clean-generic \
	clean-libtool clean-noinstLTLIBRARIES cscopelist-am ctags \
	ctags-am distclean distclean-compile distclean-generic \
	distclean-libtool distclean-tags distdir dvi dvi-am html \
	html-am info info-am install install-am install-data \
	install-data-am install-dvi install-dvi-am install-exec \
	install-exec-am install-html install-html-am install-info \
	install-info-am install-man install-pdf install-pdf-am \
	install-ps install-ps-am install-strip installcheck \
	installcheck-am installdirs maintainer-clean \
	maintainer-clean-generic mostlyclean mostlyclean-compile \
	mostlyclean-generic mostlyclean-libtool pdf pdf-am ps ps-am \
	tags tags-am uninstall uninstall-am


# We need to lower the optimization for this module.
tiger.o: $(srcdir)/tiger.c
	`echo $(COMPILE) -c $(srcdir)/tiger.c | $(o_flag_munging) `

tiger.lo: $(srcdir)/tiger.c
	`echo $(LTCOMPILE) -c $(srcdir)/tiger.c | $(o_flag_munging) `

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
