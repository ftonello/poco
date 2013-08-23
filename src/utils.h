#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define	poco_log_debug(...)	          \
		fprintf(stdout, __VA_ARGS__)

#define	poco_log_warning(...)         \
		fprintf(stderr, __VA_ARGS__)

#define	poco_log_fatal(...)	          \
	do {                              \
		fprintf(stderr, __VA_ARGS__); \
		abort();                      \
	} while (false)

#endif /* _UTILS_H_ */
