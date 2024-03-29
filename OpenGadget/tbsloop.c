
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

static void tbsloop_unit_move_step( struct tbsloop_config* config ) {
   struct isomap_tile** tile_list_temp;
   int i;

   if( NULL == config->moving_unit || NULL == config->moving_unit->path_list ) {
      /* No moving unit or the unit hasn't started moving yet. */
      return;
   }

   if(
      100 <= config->moving_unit->path_next_percent_x ||
      100 <= config->moving_unit->path_next_percent_y
   ) {
      /* The unit has finished moving one tile. */
      config->moving_unit->tile->unit = NULL;
      config->moving_unit->tile = config->moving_unit->path_list[0];
      config->moving_unit->tile->unit = config->moving_unit;

      if( 2 > config->moving_unit->path_list_count ) {
         /* This was the last move for this unit, so reset it. */
         config->moving_unit->path_next_percent_x = 0;
         config->moving_unit->path_next_percent_y = 0;
         free( config->moving_unit->path_list );
         config->moving_unit->path_list = NULL;
         config->moving_unit->path_list_count = 0;
         config->moving_unit = NULL;
      } else {
         /* The unit has finished moving onto this tile, so pop it. */
         for( i = 1 ; config->moving_unit->path_list_count > i ; i++ ) {
            config->moving_unit->path_list[i - 1] = config->moving_unit->path_list[i];
         }
         config->moving_unit->path_list_count--;
         config->moving_unit->path_list = realloc( config->moving_unit->path_list, config->moving_unit->path_list_count * sizeof( struct isomap_tile* ) );
         config->moving_unit->path_next_percent_x = 0;
         config->moving_unit->path_next_percent_y = 0;
      }
   } else {
      /* The unit is still moving onto its next tile. */
      if( config->moving_unit->path_list[0]->x != config->moving_unit->tile->x ) {
         config->moving_unit->path_next_percent_x += ISOMAP_RENDER_MOVE_STEP;
      } else if( config->moving_unit->path_list[0]->y != config->moving_unit->tile->y ) {
         config->moving_unit->path_next_percent_y += ISOMAP_RENDER_MOVE_STEP;
      } else {
         /* Dummy path. */
         config->moving_unit->path_next_percent_x = 100;
         config->moving_unit->path_next_percent_y = 100;
      }
   }
}

static void tbsloop_unit_move_start( struct tbsloop_config* config, struct isomap_tile* tile_dst ) {
   if( !tile_dst->movable ) {
      /* Cancel if an invalid tile is selected. */
      isomap_reset_movable( config->map );
      config->current_state = TBSLOOP_STATE_FREE;
      config->moving_unit = NULL;
      return;
   }

   isomap_reset_movable( config->map );
   units_move( config->moving_unit, config->moving_unit->tile, NULL, 0, 0, tile_dst );
}

#ifdef USE_SDL
static void tbsloop_handle_mouseup( SDL_Event* event, struct tbsloop_config* config ){
   int mouse_tile_x = 0, mouse_tile_y = 0, active_tile_index = -1;
   struct units_unit* unit_test = NULL;

   graphics_transform_isometric_reverse(
      &mouse_tile_x,
      &mouse_tile_y,
      event->button.x,
      event->button.y,
      config->map->width,
      config->map->height,
      &(config->viewport),
      config->rotation
   );

   active_tile_index = (mouse_tile_y * config->map->width) + mouse_tile_x;

   if( 0 > active_tile_index || config->map->tiles_count <= active_tile_index ) {
      return;
   }

   SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Mouse clicked on tile: %d, %d", config->map->tiles[active_tile_index].x, config->map->tiles[active_tile_index].y );

   switch( config->current_state ) {
      case TBSLOOP_STATE_FREE:
         if( NULL != config->moving_unit ) {
            /* A unit is still moving, so wait. */
            break;
         }

         unit_test = config->map->tiles[active_tile_index].unit;
         if( NULL == unit_test ) {
            break;
         }

         /* Start the movement selection process. */
         isomap_reset_movable( config->map );
         units_walk_range(
            unit_test->tile,
            unit_test,
            units_get_mobility_range( unit_test )
         );
         //units_select( unit_test );
         config->moving_unit = unit_test;
         config->current_state = TBSLOOP_STATE_MOVING;
         break;

      case TBSLOOP_STATE_MOVING:
         if( NULL != config->moving_unit ) {
            tbsloop_unit_move_start( config, &(config->map->tiles[active_tile_index]) );
            config->current_state = TBSLOOP_STATE_FREE;
         }
         break;
   }
}
#endif /* USE_SDL */

static void tbsloop_scrolling_state( TBSLOOP_STATE new_state, struct tbsloop_config* config ) {
   if(
      TBSLOOP_STATE_SCROLLING_X_RIGHT != config->current_state &&
      TBSLOOP_STATE_SCROLLING_X_LEFT != config->current_state &&
      TBSLOOP_STATE_SCROLLING_Y_UP != config->current_state &&
      TBSLOOP_STATE_SCROLLING_Y_DOWN != config->current_state
   ) {
      /* Hold on to the previous state in case we're moving a unit. */
      config->previous_state = config->current_state;
   }

   config->current_state = new_state;
}

OG_RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   OG_RETVAL retval = 0;
   int i = 0;
#ifdef USE_SDL
   SDL_Event event;
#endif /* USE_SDL */
   bstring new_title = NULL;
   int mouse_tile_x = 0, mouse_tile_y = 0;

   new_title = bfromcstr( "" );
   bassignformat( new_title, "%s - %d", bdata( config->map_name ), config->rotation );

   graphics_set_title( new_title );

   //memset( &viewport, '\0', sizeof( SDL_Rect ) );

   retval = isomap_render_load_textures( data_path );
   if( retval ) {
      goto cleanup;
   }

   config->viewport.w = GRAPHICS_SCREEN_WIDTH;
   config->viewport.h = GRAPHICS_SCREEN_HEIGHT;
   config->viewport.x = -200;
   config->viewport.y = 200;

#ifdef USE_SDL
   SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Started strategy mode with map: %d x %d", config->map->width, config->map->height );
#endif /* USE_SDL */

   while( 1 ) {

#ifdef USE_SDL
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
                  &(config->viewport),
                  config->rotation
               );
#ifdef DEBUG
               bassignformat( new_title, "%s - %d - %d, %d", bdata( config->map_name ), config->rotation, mouse_tile_x, mouse_tile_y );
               graphics_set_title( new_title );
#endif
               break;

            case SDL_MOUSEBUTTONUP:
               tbsloop_handle_mouseup( &event, config );
               break;

            case SDL_KEYUP:
               if(
                  ((TBSLOOP_STATE_SCROLLING_X_RIGHT == config->current_state &&
                     SDLK_RIGHT == event.key.keysym.sym) ||
                  (TBSLOOP_STATE_SCROLLING_X_LEFT == config->current_state &&
                     SDLK_LEFT == event.key.keysym.sym) ||
                  (TBSLOOP_STATE_SCROLLING_Y_UP == config->current_state &&
                     SDLK_UP == event.key.keysym.sym) ||
                  (TBSLOOP_STATE_SCROLLING_Y_DOWN == config->current_state &&
                     SDLK_DOWN == event.key.keysym.sym ))
               ) {
                  config->current_state = config->previous_state;
               }
               break;

            case SDL_KEYDOWN:
               switch( event.key.keysym.sym ) {
                  case SDLK_r:
                     config->rotation++;
                     if( 3 < config->rotation ) {
                        config->rotation = 0;
                     }
                     bassignformat( new_title, "%s - %d", bdata( config->map_name ), config->rotation );
                     graphics_set_title( new_title );
                     break;

                  case SDLK_RIGHT:
                     tbsloop_scrolling_state( TBSLOOP_STATE_SCROLLING_X_RIGHT, config );
                     break;

                  case SDLK_LEFT:
                     tbsloop_scrolling_state( TBSLOOP_STATE_SCROLLING_X_LEFT, config );
                     break;

                  case SDLK_UP:
                     tbsloop_scrolling_state( TBSLOOP_STATE_SCROLLING_Y_UP, config );
                     break;

                  case SDLK_DOWN:
                     tbsloop_scrolling_state( TBSLOOP_STATE_SCROLLING_Y_DOWN, config );
                     break;

                  case SDLK_ESCAPE:
                     goto cleanup;
               }
               break;
         }
      }
#endif /* USE_SDL */

      /* Perform any animated actions. */
      tbsloop_unit_move_step( config );

      switch( config->current_state ) {
         case TBSLOOP_STATE_SCROLLING_X_LEFT:
            config->viewport.x += TBSLOOP_VIEWPORT_STEP;
            break;
         case TBSLOOP_STATE_SCROLLING_X_RIGHT:
            config->viewport.x -= TBSLOOP_VIEWPORT_STEP;
            break;
         case TBSLOOP_STATE_SCROLLING_Y_UP:
            config->viewport.y += TBSLOOP_VIEWPORT_STEP;
            break;
         case TBSLOOP_STATE_SCROLLING_Y_DOWN:
            config->viewport.y -= TBSLOOP_VIEWPORT_STEP;
            break;
      }

      /* Draw the current frame. */
      isomap_render_loop( config->map, &(config->viewport), config->rotation, mouse_tile_x, mouse_tile_y );
      graphics_end_draw();

      graphics_sleep( 50 );
   }

cleanup:

   isomap_render_cleanup();

   return retval;
}
