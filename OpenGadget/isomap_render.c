
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
   int x,
   int y,
   const uint8_t* tiles,
   int map_width,
   int map_height,
   const ISOMAP_RENDER_ROTATE rotation,
   struct isomap_render_texture* texture_selection
) {
   ISOMAP_RENDER_BITWISE sides_sum = 0;
   ISOMAP_RENDER_BITWISE test_sum = 0;
   ISOMAP_RENDER_BITWISE temp_sum = 0;
   uint8_t current_tile;
   uint8_t test_tile;
   int tiles_count = map_width * map_height;

   texture_selection->texture_index = tiles[isomap_get_tile( x, y, map_height )];
   current_tile = tiles[isomap_get_tile( x, y, map_height )];

   /* Above */
   test_tile = tiles[isomap_get_tile( x, y + 1, map_height )];
   if( isomap_render_adjacent( current_tile, test_tile, isomap_get_tile( x, y + 1, map_height ), tiles_count ) ) {
      sides_sum += 1;
   }

   /* Left */
   test_tile = tiles[isomap_get_tile( x - 1, y, map_height )];
   if( isomap_render_adjacent( current_tile, test_tile, isomap_get_tile( x - 1, y, map_height ), tiles_count ) ) {
      sides_sum += 8;
   }

   /* Below */
   test_tile = tiles[isomap_get_tile( x, y - 1, map_height )];
   if( isomap_render_adjacent( current_tile, test_tile, isomap_get_tile( x, y - 1, map_height ), tiles_count ) ) {
      sides_sum += 4;
   }

   /* Right */
   test_tile = tiles[isomap_get_tile( x + 1, y, map_height )];
   if( isomap_render_adjacent( current_tile, test_tile, isomap_get_tile( x + 1, y, map_height ), tiles_count ) ) {
      sides_sum += 2;
   }

   /* Translate the side sums for the current rotation. Maybe there are       *
    * better ways to do this, but we can find them some other time.           */
   switch( rotation ) {
      case ISOMAP_RENDER_ROTATE_90:
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
   }

   if( 0 != sides_sum ) {
      //sides_sum = 15 - sides_sum;
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

void isomap_render_draw_tile( 
   int x,
   int y,
   const uint8_t* tiles, 
   int map_width, 
   int map_height, 
   const SDL_Rect* viewport, 
   const ISOMAP_RENDER_ROTATE rotation
) {
   int i = 0;
   SDL_Texture* sprite_texture = NULL;
   struct isomap_render_texture texture_selection;

   isomap_render_select_tile(
      x, y, tiles, map_width, map_height, rotation, &texture_selection
   );

   isomap_render_tile_rotate( x, y, map_width, map_height, i, rotation );

   sprite_texture = isomap_render_textures[texture_selection.texture_index];
   if( NULL == sprite_texture ) {
      goto cleanup;
   }

   graphics_draw_tile(
      sprite_texture,
      texture_selection.sprite_rect.x,
      texture_selection.sprite_rect.y,
      viewport->x + (y * GRAPHICS_TILE_WIDTH / 2) + (x * GRAPHICS_TILE_WIDTH / 2),
      viewport->y + ((x * GRAPHICS_TILE_OFFSET_X / 2) - (y * GRAPHICS_TILE_OFFSET_X / 2))
   );

cleanup:

   return;
}
