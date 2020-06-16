#

dist_man_MANS = \
	varnish-cli.7 \
	varnish-counters.7 \
	vcl.7 \
	vsl.7 \
	vsl-query.7 \
	varnishadm.1 \
	varnishd.1 \
	varnishhist.1 \
	varnishlog.1 \
	varnishncsa.1 \
	varnishstat.1 \
	varnishtest.1 \
	vtc.7 \
	varnishtop.1 \
	vmod_cookie.3 \
	vmod_directors.3 \
	vmod_purge.3 \
	vmod_std.3 \
	vmod_vtc.3 \
	vmod_blob.3 \
	vmod_unix.3 \
	vmod_proxy.3

MAINTAINERCLEANFILES = $(dist_man_MANS)

if MAINTAINER_MODE
# hack to enable suffix rules across directories
VPATH += :$(top_builddir)/doc/sphinx/reference:
VPATH += :$(top_builddir)/lib/libvmod_blob:
VPATH += :$(top_builddir)/lib/libvmod_cookie:
VPATH += :$(top_builddir)/lib/libvmod_directors:
VPATH += :$(top_builddir)/lib/libvmod_proxy:
VPATH += :$(top_builddir)/lib/libvmod_purge:
VPATH += :$(top_builddir)/lib/libvmod_std:
VPATH += :$(top_builddir)/lib/libvmod_unix:
VPATH += :$(top_builddir)/lib/libvmod_vtc:

# generic build rules
RST2ANY_FLAGS = --halt=2

BUILD_MAN = $(AM_V_GEN) $(RST2MAN) $(RST2ANY_FLAGS)

SUFFIXES = .rst .man.rst .1 .3 .7

.rst.1:
	$(BUILD_MAN) $< $*.1

.man.rst.3:
	$(BUILD_MAN) $< $*.3

.rst.7:
	$(BUILD_MAN) $< $*.7

# target-specific dependencies
varnish-cli.7: $(top_builddir)/doc/sphinx/include/cli.rst

varnish-counters.7: $(top_builddir)/doc/sphinx/include/counters.rst

vcl.7: \
	$(top_builddir)/doc/sphinx/reference/vcl_var.rst \
	$(top_srcdir)/bin/varnishd/builtin.vcl

vsl.7: $(top_builddir)/doc/sphinx/include/vsl-tags.rst

varnishd.1: $(top_builddir)/doc/sphinx/include/params.rst

varnishncsa.1: \
	$(top_builddir)/doc/sphinx/include/varnishncsa_options.rst \
	$(top_builddir)/doc/sphinx/include/varnishncsa_synopsis.rst

varnishlog.1: \
	$(top_builddir)/doc/sphinx/include/varnishlog_options.rst \
	$(top_builddir)/doc/sphinx/include/varnishlog_synopsis.rst

varnishstat.1: \
	$(top_builddir)/doc/sphinx/include/varnishstat_options.rst \
	$(top_builddir)/doc/sphinx/include/varnishstat_synopsis.rst \
	$(top_builddir)/doc/sphinx/include/varnishstat_bindings.rst

vtc.7: $(top_builddir)/doc/sphinx/include/vtc-syntax.rst

varnishtop.1: \
	$(top_builddir)/doc/sphinx/include/varnishtop_options.rst \
	$(top_builddir)/doc/sphinx/include/varnishtop_synopsis.rst

varnishhist.1: \
	$(top_builddir)/doc/sphinx/include/varnishhist_options.rst \
	$(top_builddir)/doc/sphinx/include/varnishhist_synopsis.rst

endif # MAINTAINER_MODE
