
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>

#include "dep\bstrlib.h"
#include "dep\arckogako.h"

SDL_Surface* graphics_image_load( bstring image_name, struct pak_file* pak );

#endif /* GRAPHICS_H */

