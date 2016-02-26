
#include "graphics.h"

SDL_Renderer* opengadget_renderer = NULL;
SDL_Window* opengadget_window = NULL;

OG_RETVAL graphics_init( void ) {
   OG_RETVAL retval = 0;

   retval = SDL_Init( SDL_INIT_VIDEO );
   if( 0 != retval ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_Init: %s", SDL_GetError() );
      goto cleanup;
   }

   opengadget_window = SDL_CreateWindow( "OpenGadget", 100, 100, 640, 480, SDL_WINDOW_SHOWN );
   if( NULL == opengadget_window ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_CreateWindow: %s", SDL_GetError() );
      retval = 2;
      goto cleanup;
   }

   opengadget_renderer = SDL_CreateRenderer( opengadget_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
   if( NULL == opengadget_renderer ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_CreateRenderer: %s", SDL_GetError() );
      retval = 4;
      goto cleanup;
   }

cleanup:

   if( 0 != retval && NULL != opengadget_window ) {
      SDL_DestroyWindow( opengadget_window );
   }

   return retval;
}

void graphics_cleanup( void ) {
   if( NULL != opengadget_renderer ) {
      SDL_DestroyRenderer( opengadget_renderer );
   }

   if( NULL != opengadget_window ) {
      SDL_DestroyWindow( opengadget_window );
   }
}

void graphics_set_title( const bstring title ) {
   SDL_SetWindowTitle( opengadget_window, bdata( title ) );
}

void graphics_clear( void ) {
   SDL_RenderClear( opengadget_renderer );
}

void graphics_end_draw( void ) {
   SDL_RenderPresent( opengadget_renderer );
}

void graphics_sleep( const int milliseconds ) {
   SDL_Delay( milliseconds );
}

static void graphics_texture_colorkey( SDL_Texture* texture, int h, uint8_t r, uint8_t g, uint8_t b ) {
   uint32_t* pixels;
   int pitch;
   uint32_t color_key;
   uint32_t transparent;
   int i;
   int pixel_count;

   SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );

   SDL_LockTexture( texture, NULL, &pixels, &pitch );

   pixel_count = (pitch / 4) * h;

   color_key = SDL_MapRGB( SDL_GetWindowSurface( opengadget_window )->format, r, g, b );
   transparent = SDL_MapRGBA( SDL_GetWindowSurface( opengadget_window )->format, 0xFF, 0xFF, 0xFF, 0x00 );
   for( i = 0 ; pixel_count > i ; ++i ) {
      if( pixels[i] >= color_key ) {
         pixels[i] = transparent;
      }
   }

   SDL_UnlockTexture( texture );
}

SDL_Texture* graphics_image_load( const bstring image_name, const struct pak_file* pak ) {
   int i;
   const struct pak_entry* entry = NULL;
   SDL_Surface* surface_temp = NULL;
   SDL_Surface* image_opt = NULL;
   uint8_t* image_data = NULL;
   SDL_RWops* image_rw = NULL;
   SDL_Texture* texture_out = NULL;
   SDL_Surface* surface_formatted = NULL;
   
   for( i = 0 ; pak->count > i ; i++ ) {
      /* TODO: Pick the shortest length possible so we don't overflow. */
      if( 0 == strncmp( bdata( image_name ), pak->entries[i].name, blength( image_name ) ) ) {
         entry = &(pak->entries[i]);
         break;
      }
   }
   if( NULL == entry ) {
      /* TODO: Error message. */
      goto cleanup;
   }

   image_data = pakopener_open_entry( pak, entry );
   if( NULL == image_data ) {
      /* TODO: Error message. */
      goto cleanup;
   }

   image_rw = SDL_RWFromConstMem( image_data, entry->unpacked_size );

   surface_temp = IMG_Load_RW( image_rw, 0 );
   if( NULL == surface_temp ) {
      /* TODO: Error message. */
      goto cleanup;
   }

   SDL_SetColorKey( surface_temp, SDL_TRUE, SDL_MapRGB( surface_temp->format, 0xfd, 0x00, 0xff ) );

   texture_out = SDL_CreateTextureFromSurface( opengadget_renderer, surface_temp );
   SDL_SetTextureBlendMode( texture_out, SDL_BLENDMODE_BLEND );

cleanup:

   if( NULL != image_rw ) {
      SDL_RWclose( image_rw );
   }

   if( NULL != image_data ) {
      free( image_data );
   }

   if( NULL != surface_temp ) {
      SDL_FreeSurface( surface_temp );
   }

   if( NULL != surface_formatted ) {
      SDL_FreeSurface( surface_formatted );
   }

   return texture_out;
}

void graphics_draw_tile( const SDL_Texture* texture, const int src_x, const int src_y, const int dest_x, const int dest_y ) {
   static SDL_Rect tile_rect;
   static SDL_Rect screen_rect;
   static uint8_t rects_init = 0;

   if( 0 == rects_init ) {
      /* Setup rectangles for the first time. */
      memset( &tile_rect, '\0', sizeof( SDL_Rect ) );
      memset( &screen_rect, '\0', sizeof( SDL_Rect ) );

      screen_rect.w = GRAPHICS_TILE_WIDTH;
      screen_rect.h = GRAPHICS_TILE_HEIGHT;
      tile_rect.w = GRAPHICS_TILE_WIDTH;
      tile_rect.h = GRAPHICS_TILE_HEIGHT;

      rects_init = 1;
   }

   tile_rect.x = src_x;
   tile_rect.y = src_y;
   screen_rect.x = dest_x;
   screen_rect.y = dest_y;

   SDL_RenderCopy( opengadget_renderer, texture, &tile_rect, &screen_rect );
}

void graphics_transform_isometric( 
   int tile_x,
   int tile_y,
   int* screen_x,
   int* screen_y,
   int map_width,
   int map_height,
   const SDL_Rect* viewport,
   int rotation
) {

   int i = 0, transformed_x, transformed_y;

   switch( rotation ) {
      case GRAPHICS_ROTATE_0:
         transformed_y = tile_y;
         transformed_x = tile_x;
         break;
      case GRAPHICS_ROTATE_90:
         transformed_y = map_height - tile_x;
         transformed_x = tile_x;
         break;
      case GRAPHICS_ROTATE_180:
         transformed_x = map_width - tile_x;
         transformed_y = map_height - tile_y;
         break;
      case GRAPHICS_ROTATE_270:
         transformed_x = map_width - tile_y;
         transformed_y = tile_x;
         break;
   }

   *screen_x = viewport->x + (transformed_x * GRAPHICS_TILE_WIDTH / 2) +
      (transformed_y * GRAPHICS_TILE_WIDTH / 2);
   *screen_y = viewport->y + ((transformed_y * GRAPHICS_TILE_OFFSET_X / 2) -
      (transformed_x * GRAPHICS_TILE_OFFSET_X / 2));
}