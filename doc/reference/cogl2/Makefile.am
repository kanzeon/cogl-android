## Process this file with automake to produce Makefile.in

# We require automake 1.6 at least.
AUTOMAKE_OPTIONS = 1.6

# This is a blank Makefile.am for using gtk-doc.
# Copy this to your project's API docs directory and modify the variables to
# suit your project. See the GTK+ Makefiles in gtk+/docs/reference for examples
# of using the various options.

# The name of the module, e.g. 'glib'.
DOC_MODULE=cogl2

# The top-level SGML file. You can change this if you want to.
DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.xml

# The directory containing the source code. Relative to $(srcdir).
# gtk-doc will search all .c & .h files beneath here for inline comments
# documenting the functions and macros.
# e.g. DOC_SOURCE_DIR=../../../gtk
DOC_SOURCE_DIR=../../../cogl

# Extra options to pass to gtkdoc-scangobj. Not normally needed.
SCANGOBJ_OPTIONS=--type-init-func="g_type_init()"

# Extra options to supply to gtkdoc-scan.
# e.g. SCAN_OPTIONS=--deprecated-guards="GTK_DISABLE_DEPRECATED"
SCAN_OPTIONS=--deprecated-guards="COGL_DISABLE_DEPRECATED"

# Extra options to supply to gtkdoc-mkdb.
# e.g. MKDB_OPTIONS=--sgml-mode --output-format=xml
MKDB_OPTIONS=--sgml-mode --output-format=xml --name-space=cogl

# Extra options to supply to gtkdoc-mktmpl
# e.g. MKTMPL_OPTIONS=--only-section-tmpl
MKTMPL_OPTIONS=

# Extra options to supply to gtkdoc-fixref. Not normally needed.
# e.g. FIXXREF_OPTIONS=--extra-dir=../gdk-pixbuf/html --extra-dir=../gdk/html
FIXXREF_OPTIONS=\
	--extra-dir=$(GLIB_PREFIX)/share/gtk-doc/html/glib \
	--extra-dir=$(GDKPIXBUF_PREFIX)/share/gtk-doc/html/gdk-pixbuf

# Used for dependencies. The docs will be rebuilt if any of these change.
# e.g. HFILE_GLOB=$(top_srcdir)/gtk/*.h
# e.g. CFILE_GLOB=$(top_srcdir)/gtk/*.c
HFILE_GLOB=\
	$(top_srcdir)/cogl/*.h \
	$(top_builddir)/cogl/*.h \
	$(top_srcdir)/cogl/winsys/*.h
CFILE_GLOB=$(top_srcdir)/cogl/*.c

# Header files to ignore when scanning.
# e.g. IGNORE_HFILES=gtkdebug.h gtkintl.h
cogl_private_headers := \
	$(shell cd $(top_srcdir)/cogl && echo *-private.h && cd - && \
	cd $(top_srcdir)/cogl/winsys && echo *-private.h && cd -)

IGNORE_HFILES=\
	driver 					\
	tesselator				\
	cogl.h					\
	$(cogl_private_headers)			\
	cogl-atlas.h				\
	cogl-bitmap-packing.h			\
	cogl-bitmask.h				\
	cogl-blend-string.h 			\
	cogl-blit.h				\
	cogl-boxed-value.h			\
	cogl-clip-stack.h 			\
	cogl-debug.h 				\
	cogl-debug-options.h 			\
	cogl-defines.h 				\
	cogl-driver.h				\
	cogl-enum-types.h			\
	cogl-flags.h				\
	cogl-gles2-types.h			\
	cogl-gl-header.h			\
	cogl-glsl-shader-boilerplate.h		\
	cogl-profile.h				\
	cogl-queue.h				\
	cogl-rectangle-map.h			\
	cogl-spans.h 				\
	cogl-texture-driver.h			\
	cogl-util.h 				\
	$(NULL)

EXTRA_HFILES=

# Images to copy into HTML directory.
# e.g. HTML_IMAGES=$(top_srcdir)/gtk/stock-icons/stock_about_24.png
HTML_IMAGES = \
	fill-rule-non-zero.png \
	fill-rule-even-odd.png \
	quad-indices-order.png \
	quad-indices-triangles.png \
	cogl_ortho.png

# Extra SGML files that are included by $(DOC_MAIN_SGML_FILE).
# e.g. content_files=running.sgml building.sgml changes-2.0.sgml
content_files = \
	blend-strings.xml

# SGML files where gtk-doc abbrevations (#GtkWidget) are expanded
# These files must be listed here *and* in content_files
# e.g. expand_content_files=running.sgml
expand_content_files = \
	blend-strings.xml

# CFLAGS and LDFLAGS for compiling gtkdoc-scangobj with your library.
# Only needed if you are using gtkdoc-scangobj to dynamically query widget
# signals and properties.
# e.g. AM_CPPFLAGS=-I$(top_srcdir) -I$(top_builddir) $(GTK_DEBUG_FLAGS)
# e.g. GTKDOC_LIBS=$(top_builddir)/gtk/$(gtktargetlib)

AM_CPPFLAGS=-I$(top_srcdir) -I$(top_builddir)/cogl -DCOGL_ENABLE_EXPERIMENTAL_API $(COGL_DEP_CFLAGS)
GTKDOC_LIBS=$(top_builddir)/cogl/libcogl2.la $(COGL_DEP_LIBS)

# This includes the standard gtk-doc make rules, copied by gtkdocize.
if BUILD_GTK_DOC
include $(top_srcdir)/gtk-doc.make
else
EXTRA_DIST =
endif

# Other files to distribute
# e.g. EXTRA_DIST += version.xml.in

EXTRA_DIST += $(HTML_IMAGES) $(content_files)
