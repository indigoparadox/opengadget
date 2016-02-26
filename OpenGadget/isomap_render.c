
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

#include "isomap_render.h"

static SDL_Texture* isomap_render_terrain_textures[0xff];
static SDL_Texture* isomap_render_unit_textures[0xff];
static SDL_Texture* isomap_render_ui_textures[0xff];
static uint8_t isomap_render_textures_loaded = 0;

OG_RETVAL isomap_render_load_textures( const bstring data_path ) {
   OG_RETVAL retval = 0;
   const char* gfx_terrain_name_format = "MP%d_%02d_%d";
   const char* gfx_bldg_name_format = "MB%d_%02d_%d";
   const char* gfx_unit_name_format = "MU_%d%02d00";
   bstring gfx_tile_name = NULL;
   bstring gfx_data_path = NULL;
   FILE* gfx_data_file = NULL;
   struct pak_file* gfx_data_pak = NULL;
   int i, j;
   struct tagbstring ui_cursor = bsStatic( "RS_MAPCUR" );
   struct tagbstring ui_marker = bsStatic( "RS_MAPMARK" );

   if( 0 != isomap_render_textures_loaded ) {
      goto cleanup;
   }

   gfx_data_path = bformat( "%s\\graphic.pak", bdata( data_path ) );
   gfx_tile_name = bfromcstr( "" );

   gfx_data_file = fopen( bdata( gfx_data_path ), "rb" );
   if( NULL == gfx_data_file ) {
      retval = 1;
      goto cleanup;
   }
   gfx_data_pak = pakopener_try_open( gfx_data_file );
   if( NULL == gfx_data_pak ) {
      retval = 1;
      goto cleanup;
   }

   memset( isomap_render_terrain_textures, '\0', 0xff * sizeof( SDL_Texture* ) );
   memset( isomap_render_unit_textures, '\0', 0xff * sizeof( SDL_Texture* ) );
   memset( isomap_render_ui_textures, '\0', 0xff * sizeof( SDL_Texture* ) );

   /* Load terrain tiles. */
   for( i = 0 ; ISOMAP_BUILDINGS_TILES_OFFSET > i ; i++ ) {
      bassignformat( gfx_tile_name, gfx_terrain_name_format, 1, i, 0 );
      isomap_render_terrain_textures[i] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   }

   /* Load buildings tiles. */
   for( i = 0 ; (0x1B - ISOMAP_BUILDINGS_TILES_OFFSET) > i ; i++ ) {
      bassignformat( gfx_tile_name, gfx_bldg_name_format, 0, i, 0 );
      isomap_render_terrain_textures[ISOMAP_BUILDINGS_TILES_OFFSET + i] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   }

   /* Load the white units. */
   for( i = 0 ; ISOMAP_RENDER_UNIT_TEXTURES_MAX > i ; i++ ) {
      bassignformat( gfx_tile_name, gfx_unit_name_format, 0, i );
      isomap_render_unit_textures[i] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   }

   /* Load the black units. */
   for( i = ISOMAP_RENDER_UNIT_TEXTURES_MAX ; (2 * ISOMAP_RENDER_UNIT_TEXTURES_MAX) > i ; i++ ) {
      bassignformat( gfx_tile_name, gfx_unit_name_format, 1, (i - ISOMAP_RENDER_UNIT_TEXTURES_MAX) );
      isomap_render_unit_textures[i] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   }

   isomap_render_ui_textures[ISOMAP_RENDER_UI_MAPCURSOR] = graphics_image_load( &ui_cursor, gfx_data_pak );
   isomap_render_ui_textures[ISOMAP_RENDER_UI_MAPMARKER] = graphics_image_load( &ui_marker, gfx_data_pak );

   isomap_render_textures_loaded = 1;

cleanup:

   if( NULL != gfx_data_path ) {
      bdestroy( gfx_data_path );
   }

   if( NULL != gfx_tile_name ) {
      bdestroy( gfx_tile_name );
   }

   return retval;
}

void isomap_render_cleanup( void ) {
   int i;

   for( i = 0 ; 0xff > i ; i++ ) {
      if( NULL != isomap_render_terrain_textures[i] ) {
         SDL_DestroyTexture( isomap_render_terrain_textures[i] );
      }
      if( NULL != isomap_render_unit_textures[i] ) {
         SDL_DestroyTexture( isomap_render_unit_textures[i] );
      }
   }

   memset( isomap_render_terrain_textures, '\0', 0xff * sizeof( SDL_Texture* ) );
   memset( isomap_render_unit_textures, '\0', 0xff * sizeof( SDL_Texture* ) );
   memset( isomap_render_ui_textures, '\0', 0xff * sizeof( SDL_Texture* ) );

   isomap_render_textures_loaded = 0;
}

static void isomap_render_select_terrain(
   const struct isomap_tile* tile,
   const GRAPHICS_ROTATE rotation,
   struct isomap_render_texture* texture_selection
) {
   ISOMAP_RENDER_BITWISE sides_sum = 0;
   ISOMAP_RENDER_BITWISE test_sum = 0;
   ISOMAP_RENDER_BITWISE temp_sum = 0;
   struct isomap_tile* current_tile;
   struct isomap_tile* test_tile;
   uint32_t current_x = tile->x;
   uint32_t current_y = tile->map->width - tile->y - 1;

   texture_selection->texture_index = tile->terrain;
   current_tile = tile;

   /* Above */
   test_tile = &(tile->map->tiles[isomap_get_tile( current_x + 1, current_y, tile->map )]);
   if( isomap_render_adjacent( current_tile, test_tile ) ) {
      sides_sum += 1;
   }

   /* Left */
   test_tile = &(tile->map->tiles[isomap_get_tile( current_x, current_y - 1, tile->map )]);
   if( isomap_render_adjacent( current_tile, test_tile ) ) {
      sides_sum += 2;
   }

   /* Below */
   test_tile = &(tile->map->tiles[isomap_get_tile( current_x - 1, current_y, tile->map )]);
   if( isomap_render_adjacent( current_tile, test_tile ) ) {
      sides_sum += 4;
   }

   /* Right */
   test_tile = &(tile->map->tiles[isomap_get_tile( current_x, current_y + 1, tile->map )]);
   if( isomap_render_adjacent( current_tile, test_tile ) ) {
      sides_sum += 8;
   }

   /* Translate the side sums for the current rotation. Maybe there are       *
    * better ways to do this, but we can find them some other time.           */
   switch( rotation ) {
      case GRAPHICS_ROTATE_270:
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_UP;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_RIGHT; }
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_LEFT;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_UP; }
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_DOWN;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_LEFT; }
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_RIGHT;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_DOWN; }
         sides_sum = temp_sum;
         break;

      case GRAPHICS_ROTATE_180:
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_UP;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_DOWN;
         }
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_LEFT;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_RIGHT;
         }
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_DOWN;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_UP;
         }
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_RIGHT;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_LEFT;
         }
         sides_sum = temp_sum;
         break;


      case GRAPHICS_ROTATE_90:
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_UP;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_LEFT;
         }
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_LEFT;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_DOWN;
         }
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_DOWN;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_RIGHT;
         }
         test_sum = sides_sum & ISOMAP_RENDER_BITWISE_RIGHT;
         if( 0 != test_sum ) {
            temp_sum += ISOMAP_RENDER_BITWISE_UP;
         }
         sides_sum = temp_sum;
         break;
   }

   if( 0 != sides_sum ) {
      texture_selection->sprite_rect.x =
         (sides_sum % GRAPHICS_TILES_X_COUNT) * GRAPHICS_TILE_WIDTH;
      texture_selection->sprite_rect.y =
         floor( (sides_sum / GRAPHICS_TILES_X_COUNT) ) * GRAPHICS_TILE_HEIGHT;
   } else {
      texture_selection->sprite_rect.x = 0;
      texture_selection->sprite_rect.y = 0;
   }

#ifdef DEBUG
   assert( 15 >= sides_sum );
   assert( 30 >= texture_selection->texture_index );
   assert( 0 <= texture_selection->texture_index );
   assert( 0 <= texture_selection->sprite_rect.x );
   assert( 144 >= texture_selection->sprite_rect.y );
   assert( 144 >= texture_selection->sprite_rect.x );
#endif /* DEBUG */
}

static void isomap_render_select_unit(
   const struct units_unit* unit,
   int animation_frame,
   GRAPHICS_ROTATE rotation,
   struct isomap_render_texture* texture_selection
) {
   int new_direction;

   new_direction = unit->facing + rotation;
   if( GRAPHICS_ROTATE_270 < new_direction ) {
      new_direction = unit->facing % new_direction;
   }

   texture_selection->sprite_rect.x = GRAPHICS_TILE_WIDTH * animation_frame;
   texture_selection->sprite_rect.y = GRAPHICS_TILE_HEIGHT * (unit->facing + 1);
   
   if( unit->team ) {
      texture_selection->texture_index = ISOMAP_RENDER_UNIT_TEXTURES_MAX + unit->type;
   } else {
      texture_selection->texture_index = unit->type;
   }
   
}

void isomap_render_draw_tile(
   const struct isomap_tile* tile,
   const SDL_Rect* viewport, 
   const GRAPHICS_ROTATE rotation
) {
   int i = 0;
   uint32_t draw_x = tile->x;
   uint32_t draw_y = tile->y;
   SDL_Texture* sprite_texture = NULL;
   struct isomap_render_texture texture_selection;

   isomap_render_select_terrain( tile, rotation, &texture_selection );

   graphics_transform_isometric(
      tile->x,
      tile->y,
      &draw_x,
      &draw_y,
      tile->map->width,
      tile->map->height,
      viewport,
      rotation
   );

   sprite_texture = isomap_render_terrain_textures[texture_selection.texture_index];
   if( NULL == sprite_texture ) {
      /* TODO: Draw a placeholder. */
      goto cleanup;
   }

   graphics_draw_tile(
      sprite_texture,
      texture_selection.sprite_rect.x,
      texture_selection.sprite_rect.y,
      draw_x,
      draw_y
   );

cleanup:

   return;
}

void isomap_render_draw_unit(
   const struct units_unit* unit,
   const uint8_t ani_frame,
   const SDL_Rect* viewport,
   const GRAPHICS_ROTATE rotation
) {
   int i = 0, x, y;
   SDL_Texture* sprite_texture = NULL;
   struct isomap_render_texture texture_selection;

   isomap_render_select_unit( unit, ani_frame, rotation, &texture_selection );
   
   graphics_transform_isometric(
      unit->tile->x,
      unit->tile->y,
      &x,
      &y,
      unit->tile->map->width,
      unit->tile->map->height,
      viewport,
      rotation
   );

   sprite_texture = isomap_render_unit_textures[texture_selection.texture_index];
   if( NULL == sprite_texture ) {
      goto cleanup;
   }

   graphics_draw_tile(
      sprite_texture,
      texture_selection.sprite_rect.x,
      texture_selection.sprite_rect.y,
      x,
      y
   );

cleanup:

   return;
}

void isomap_render_loop( const struct isomap* map, const SDL_Rect* viewport, GRAPHICS_ROTATE rotation ) {
   int i, j, render_x, render_y;
   static int animation_frame = 0;

   for( i = map->tiles_count - 1 ; 0 <= i ; i-- ) {
      switch( rotation ) {
         case GRAPHICS_ROTATE_0:
            j = i;
            break;
         case GRAPHICS_ROTATE_90:
            render_x = i % map->width;
            render_y = i / map->width;
            j = (render_y * map->width) + (map->width - render_x - 1);
            break;
         case GRAPHICS_ROTATE_180:
            j = map->tiles_count - i - 1;
            break;
         case GRAPHICS_ROTATE_270:
            render_x = i % map->width;
            render_y = i / map->width;
            j = map->tiles_count - ((render_y * map->width) + (map->width - render_x - 1)) - 1;
            break;
      }

      isomap_render_draw_tile(
         &(map->tiles[j]),
         viewport,
         rotation
         );
      if( NULL != map->tiles[j].unit ) {
         isomap_render_draw_unit(
            map->tiles[j].unit,
            animation_frame,
            viewport,
            rotation
            );
      }

   }

   animation_frame++;
   if( 3 < animation_frame ) {
      animation_frame = 0;
   }
}

SDL_Texture* isomap_render_get_texture( ISOMAP_RENDER_TEXTURE_TYPE type, int index ) {
   switch( type ) {
      case ISOMAP_RENDER_TEXTURE_TYPE_TERRAIN:
         return isomap_render_terrain_textures[index];
      case ISOMAP_RENDER_TEXTURE_TYPE_UNIT:
         return isomap_render_unit_textures[index];
      case ISOMAP_RENDER_TEXTURE_TYPE_UI:
         return isomap_render_ui_textures[index];
   }
}
