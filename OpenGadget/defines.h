
#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <SDL_log.h>

#include "dep/bstrlib.h"

typedef uint8_t RETVAL;

#define LOG_ERROR( ... ) SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__ )

#endif /* DEFINES_H */
