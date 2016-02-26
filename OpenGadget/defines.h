
/* This file is part of OpenGadget.
*
* OpenGadget is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option)
* any later version.
*
* OpenGadget is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with OpenGadget.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <SDL_log.h>
#include <assert.h>

#include "dep/bstrlib.h"

typedef uint8_t OG_RETVAL;
typedef uint8_t OG_BOOL;

#define OG_TRUE  1
#define OG_FALSE 0

#define OG_LOG_ERROR( ... ) SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__ )

#endif /* DEFINES_H */
