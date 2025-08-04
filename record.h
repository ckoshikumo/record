#ifndef INCLUDE_RECORD_H
#define INCLUDE_RECORD_H

#include <errno.h>
#include <stdarg.h>
#include <string.h> // IWYU pragma: keep.

#ifndef REC_API
#define REC_API
#endif

//
// API
//
#ifdef NDEBUG
#define rec_debug(msg, ...)
#else
#define rec_debug(msg, ...)                                                    \
	_rec_write(REC_DEBUG, __FILE__, __LINE__, REC_ERRNO,                       \
	           msg __VA_OPT__(, ) __VA_ARGS__)
#endif // NDEBUG

#define rec_error(msg, ...)                                                    \
	_rec_write(REC_ERROR, __FILE__, __LINE__, REC_ERRNO,                       \
	           msg __VA_OPT__(, ) __VA_ARGS__)

#define rec_warn(msg, ...)                                                     \
	_rec_write(REC_WARN, __FILE__, __LINE__, REC_ERRNO,                        \
	           msg __VA_OPT__(, ) __VA_ARGS__)

#define rec_info(msg, ...)                                                     \
	_rec_write(REC_INFO, __FILE__, __LINE__, REC_ERRNO,                        \
	           msg __VA_OPT__(, ) __VA_ARGS__)

#define rec_log(msg, ...)                                                      \
	_rec_write(REC_LOG, __FILE__, __LINE__, REC_ERRNO,                         \
	           msg __VA_OPT__(, ) __VA_ARGS__)

//
// Helpers and implementation
//
#define REC_ERRNO (errno == 0 ? NULL : strerror(errno))

#define REC_ENUM_STR(apply)                                                    \
	apply(REC_DEBUG, "Debug") apply(REC_ERROR, "Error")                        \
	    apply(REC_WARN, "Warn") apply(REC_INFO, "Info") apply(REC_LOG, "Log")

#define REC_AS_ENUM(type, str, ...) type,

typedef enum { REC_ENUM_STR(REC_AS_ENUM) } rec_type_e;

extern const char *record_strs[];

REC_API void _rec_write(rec_type_e type, char *file, int line, char *errnostr,
                        char const *msg, ...);

#endif // INCLUDE_RECORD_H

#ifdef RECORD_IMPLEMENTATION

#include <stdio.h>

#define REC_AS_STR(type, str, ...) str,

const char *record_strs[] = {REC_ENUM_STR(REC_AS_STR)};

REC_API void _rec_write(rec_type_e type, char *file, int line, char *errnostr,
                        const char *msg, ...) {

	fprintf(stderr, "[%s] (%s:%d", record_strs[type], file, line);
	if (errnostr) fprintf(stderr, ":errno[%s]", errnostr);

	fprintf(stderr, ") ");

	va_list(arg);
	va_start(arg, msg);
	vfprintf(stderr, msg, arg);
	va_end(arg);

	fprintf(stderr, "\n");
}

#endif // RECORD_IMPLEMENTATION
