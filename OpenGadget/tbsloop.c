
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

#include "tbsloop.h"

extern bstring data_path;

OG_RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   OG_RETVAL retval = 0;
   int i = 0;
   SDL_Rect viewport;
   GRAPHICS_ROTATE rotation = GRAPHICS_ROTATE_0;
   SDL_Event event;
   bstring new_title = NULL;
   int mouse_tile_x = 0, mouse_tile_y = 0, mouse_draw_x, mouse_draw_y;

   new_title = bfromcstr( "" );
   bassignformat( new_title, "%s - %d", bdata( config->map_name ), rotation );

   graphics_set_title( new_title );

   memset( &viewport, '\0', sizeof( SDL_Rect ) );

   retval = isomap_render_load_textures( data_path );
   if( retval ) {
      goto cleanup;
   }

   viewport.w = GRAPHICS_SCREEN_WIDTH;
   viewport.h = GRAPHICS_SCREEN_HEIGHT;
   viewport.x = 0;
   viewport.y = 200;

   while( 1 ) {

      while( SDL_PollEvent( &event ) ) {
         switch( event.type ) {
            case SDL_QUIT:
               goto cleanup;

            case SDL_MOUSEMOTION:
               graphics_transform_isometric_reverse(
                  &mouse_tile_x,
                  &mouse_tile_y,
                  event.motion.x,
                  event.motion.y,
                  config->map->width,
                  config->map->height,
                  &viewport,
                  rotation
               );
               break;

            case SDL_KEYDOWN:
               switch( event.key.keysym.sym ) {
                  case SDLK_r:
                     rotation++;
                     if( 3 < rotation ) {
                        rotation = 0;
                     }
                     bassignformat( new_title, "%s - %d", bdata( config->map_name ), rotation );
                     graphics_set_title( new_title );
                     break;

                  case SDLK_RIGHT:
                     viewport.x -= 100;
                     break;

                  case SDLK_LEFT:
                     viewport.x += 100;
                     break;

                  case SDLK_UP:
                     viewport.y += 100;
                     break;

                  case SDLK_DOWN:
                     viewport.y -= 100;
                     break;

                  case SDLK_ESCAPE:
                     goto cleanup;
               }
               break;
         }

      }

      graphics_clear();

      isomap_render_loop( config->map, &viewport, rotation );

      graphics_transform_isometric(
         mouse_tile_x,
         mouse_tile_y,
         &mouse_draw_x,
         &mouse_draw_y,
         config->map->width,
         config->map->height,
         &viewport,
         rotation
      );

      graphics_draw_tile(
         isomap_render_get_texture( ISOMAP_RENDER_TEXTURE_TYPE_UI, ISOMAP_RENDER_UI_MAPCURSOR ),
         0,
         0,
         mouse_draw_x,
         mouse_draw_y
      );

      graphics_end_draw();

      graphics_sleep( 250 );
   }

cleanup:

   isomap_render_cleanup();

   return retval;
}
