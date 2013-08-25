#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define	poco_log_debug(...)	\
		fprintf(stdout, __VA_ARGS__)

#define	poco_log_warning(...) \
		fprintf(stderr, "Warning: " __VA_ARGS__)

#define	poco_log_fatal(...)	                    \
	do {                                        \
		fprintf(stderr, "Fatal: " __VA_ARGS__); \
		abort();                                \
	} while (false)

#define	ARRAY_LENGTH(x)	(sizeof(x) / sizeof(*x))

#endif /* _UTILS_H_ */
