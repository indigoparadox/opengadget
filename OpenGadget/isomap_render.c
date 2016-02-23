
#include "isomap_render.h"

static SDL_Texture* isomap_render_textures[0xff];
static SDL_Texture* isomap_render_textures_loaded = 0;

void isomap_render_load_textures( bstring data_path ) {
   const char* gfx_tile_name_format = "MP%d_%02d_%d";
   bstring gfx_tile_name = NULL;
   bstring gfx_data_path = NULL;
   FILE* gfx_data_file = NULL;
   struct pak_file* gfx_data_pak = NULL;
   int i;

   gfx_data_path = bformat( "%s\\graphic.pak", bdata( data_path ) );
   gfx_tile_name = bfromcstr( "" );

   gfx_data_file = fopen( bdata( gfx_data_path ), "rb" );
   if( NULL == gfx_data_file ) {
      goto cleanup;
   }
   gfx_data_pak = pakopener_try_open( gfx_data_file );
   if( NULL == gfx_data_pak ) {
      goto cleanup;
   }

   memset( isomap_render_textures, '\0', 0xFF * sizeof( SDL_Texture* ) );

   for( i = 0 ; ISOMAP_TERRAIN_HQ > i ; i++ ) {
      isomap_render_load_texture( i );
   }

   isomap_render_textures_loaded = 1;

cleanup:

   return;
}

void isomap_render_cleanup( void ) {
   int i;

   for( i = 0; 0xFF > i; i++ ) {
      if( NULL != isomap_render_textures[i] ) {
         SDL_DestroyTexture( isomap_render_textures[i] );
      }
   }
}

void isomap_render_draw_tile( uint8_t* tiles, int map_width, int map_height, SDL_Rect* viewport, ISOMAP_RENDER_ROTATE rotation ) {
   int x_y_index;
   int i = 0, x, y, projected_x, projected_y, tile_index;
   SDL_Texture* sprite_texture = NULL;

   for( x = map_width - 1 ; 0 <= x ; x-- ) {
      for( y = 0 ; map_height > y ; y++ ) {

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

         x_y_index = (x * map_height) + y;
         tile_index = tiles[x_y_index];
         sprite_texture = isomap_render_textures[tile_index];
         if( NULL == sprite_texture ) {
            continue;
         }

         graphics_draw_tile(
            sprite_texture,
            0, 0,
            viewport->x + (projected_y * GRAPHICS_TILE_WIDTH / 2) + (projected_x * GRAPHICS_TILE_WIDTH / 2),
            viewport->y + ((projected_x * GRAPHICS_TILE_OFFSET_X / 2) - (projected_y * GRAPHICS_TILE_OFFSET_X / 2))
         );
      }
   }
}