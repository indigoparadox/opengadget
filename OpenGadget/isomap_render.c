
#include "isomap_render.h"

static SDL_Texture* isomap_render_textures[0xff];
static uint8_t isomap_render_textures_loaded = 0;

OG_RETVAL isomap_render_load_textures( const bstring data_path ) {
   OG_RETVAL retval = 0;
   const char* gfx_tile_name_format = "MP%d_%02d_%d";
   bstring gfx_tile_name = NULL;
   bstring gfx_data_path = NULL;
   FILE* gfx_data_file = NULL;
   struct pak_file* gfx_data_pak = NULL;
   int i;

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

   memset( isomap_render_textures, '\0', 0xFF * sizeof( SDL_Texture* ) );

   for( i = 0 ; ISOMAP_TERRAIN_HQ > i ; i++ ) {
      isomap_render_load_texture( i );
   }

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

   for( i = 0; 0xFF > i; i++ ) {
      if( NULL != isomap_render_textures[i] ) {
         SDL_DestroyTexture( isomap_render_textures[i] );
      }
   }
}

static void isomap_render_select_tile(
   const int x,
   const int y,
   const uint8_t* tiles,
   const int map_width,
   const int map_height,
   const ISOMAP_RENDER_ROTATE rotation,
   struct isomap_render_texture* texture_selection
) {
#if 1
   int sides_sum = 0;

   /* Above */
   if (
      tiles[isomap_get_tile( x, y + 1, map_height )] ==
      tiles[isomap_get_tile( x, y, map_height )]
   ) {
      sides_sum += 1;
   }

   /* Left */
   if(
      tiles[isomap_get_tile( x - 1, y, map_height )] ==
      tiles[isomap_get_tile( x, y, map_height )]
   ) {
      sides_sum += 2;
   }

   /* Below */
   if(
      tiles[isomap_get_tile( x, y - 1, map_height )] ==
      tiles[isomap_get_tile( x, y, map_height )]
   ) {
      sides_sum += 4;
   }

   /* Right */
   if(
      tiles[isomap_get_tile( x + 1, y, map_height )] ==
      tiles[isomap_get_tile( x, y, map_height )]
   ) {
      sides_sum += 8;
   }

   sides_sum = 15 - sides_sum;

   texture_selection->texture_index = tiles[isomap_get_tile( x, y, map_height )];
   texture_selection->sprite_rect.x = (sides_sum % 4) * GRAPHICS_TILE_WIDTH;
   texture_selection->sprite_rect.y = floor( (sides_sum / 4) ) * GRAPHICS_TILE_HEIGHT;

#ifdef DEBUG
   assert( 15 >= sides_sum );
   assert( 30 >= texture_selection->texture_index );
   assert( 0 <= texture_selection->texture_index );
   assert( 0 <= texture_selection->sprite_rect.x );
   assert( 144 >= texture_selection->sprite_rect.y );
   assert( 144 >= texture_selection->sprite_rect.x );
#endif /* DEBUG */

#else
   texture_selection->texture_index = tiles[isomap_get_tile( x, y, map_height )];
   texture_selection->sprite_rect.x = 0;
   texture_selection->sprite_rect.y = 0;
#endif
}

void isomap_render_draw_tile( 
   const int x,
   const int y,
   const uint8_t* tiles, 
   const int map_width, 
   const int map_height, 
   const SDL_Rect* viewport, 
   const ISOMAP_RENDER_ROTATE rotation
) {
   int i = 0, projected_x, projected_y;
   SDL_Texture* sprite_texture = NULL;
   struct isomap_render_texture texture_selection;

   switch( rotation ) {
      case ISOMAP_RENDER_ROTATE_0:
         projected_x = y;
         projected_y = map_height - x - 1;
         break;

      case ISOMAP_RENDER_ROTATE_90:
         projected_x = map_width - x - 1;
         projected_y = map_height - y - 1;
         break;

      case ISOMAP_RENDER_ROTATE_180:
         projected_x = map_width - y - 1;
         projected_y = x;
         break;

      case ISOMAP_RENDER_ROTATE_270:
         projected_x = x;
         projected_y = y;
         break;
   }

   isomap_render_select_tile(
      x, y, tiles, map_width, map_height, rotation, &texture_selection
   );
   sprite_texture = isomap_render_textures[texture_selection.texture_index];
   if( NULL == sprite_texture ) {
      goto cleanup;
   }

   graphics_draw_tile(
      sprite_texture,
      texture_selection.sprite_rect.x,
      texture_selection.sprite_rect.y,
      viewport->x + (projected_y * GRAPHICS_TILE_WIDTH / 2) + (projected_x * GRAPHICS_TILE_WIDTH / 2),
      viewport->y + ((projected_x * GRAPHICS_TILE_OFFSET_X / 2) - (projected_y * GRAPHICS_TILE_OFFSET_X / 2))
   );

cleanup:

   return;
}
