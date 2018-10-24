AUTOMAKE_OPTIONS = gnu

lib_LTLIBRARIES = libflowtuple.la
include_HEADERS = flowtuple.h

libflowtuple_la_SOURCES = \
	class.c \
	data.c \
	flowtuple.h flowtuple.c \
	fttypes.h \
	header.c \
	interval.c \
	record.h record.c \
	trailer.c \
	util.h util.c

libflowtuple_la_LDFLAGS = -version-info $(LIB_VERSION_INFO)
