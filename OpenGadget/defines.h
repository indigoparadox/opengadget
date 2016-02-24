
#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <SDL_log.h>

#include "dep/bstrlib.h"

typedef uint8_t OG_RETVAL;
typedef uint8_t OG_BOOL;

#define OG_TRUE  1
#define OG_FALSE 0

#define OG_LOG_ERROR( ... ) SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__ )

#endif /* DEFINES_H */
