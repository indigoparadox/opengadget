
#include "graphics.h"

SDL_Renderer* opengadget_renderer = NULL;
SDL_Window* opengadget_window = NULL;

RETVAL graphics_init( void ) {
   RETVAL retval = 0;

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

   //SDL_SetRenderDrawBlendMode( opengadget_renderer, SDL_BLENDMODE_BLEND );

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

void graphics_set_title( bstring title ) {
   SDL_SetWindowTitle( opengadget_window, bdata( title ) );
}

void graphics_begin_draw( void ) {
   SDL_RenderClear( opengadget_renderer );
}

void graphics_end_draw( void ) {
   SDL_RenderPresent( opengadget_renderer );
}

void graphics_sleep( int milliseconds ) {
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

SDL_Texture* graphics_image_load( bstring image_name, struct pak_file* pak ) {
   int i;
   struct pak_entry* entry = NULL;
   SDL_Surface* surface_temp = NULL;
   SDL_Surface* image_opt = NULL;
   uint8_t* image_data = NULL;
   SDL_RWops* image_rw = NULL;
   SDL_Texture* texture_out = NULL;
   SDL_Surface* surface_formatted = NULL;
   uint32_t* pixels;
   int pitch;
   
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

#if 0
   surface_formatted = SDL_ConvertSurface( 
      surface_temp, 
      SDL_GetWindowSurface( opengadget_window )->format,
      NULL
   );
   if( NULL == surface_formatted ) {
      /* TODO: Error message. */
      goto cleanup;
   }
#endif

   texture_out = SDL_CreateTextureFromSurface( opengadget_renderer, surface_temp );
   //SDL_SetSurfaceBlendMode( image_temp, SDL_BLENDMODE_BLEND );
   SDL_SetTextureBlendMode( texture_out, SDL_BLENDMODE_BLEND );

#if 0
   texture_out = SDL_CreateTexture(
      opengadget_renderer,
      SDL_GetWindowPixelFormat( opengadget_window ), 
      SDL_TEXTUREACCESS_STREAMING,
      surface_formatted->w,
      surface_formatted->h
   );

   SDL_LockTexture( texture_out, NULL, &pixels, &pitch );
   memcpy( pixels, surface_formatted->pixels, surface_formatted->pitch * surface_formatted->h );
   SDL_UnlockTexture( texture_out );
   pixels = NULL;
#endif
   
   //SDL_SetTextureBlendMode( texture_out, SDL_BLENDMODE_BLEND );

   //graphics_texture_colorkey( texture_out, surface_formatted->h, 0xfd, 0xff, 0xff );

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

static void graphics_draw( SDL_Texture* texture, const SDL_Rect* src_rect, const SDL_Rect* dest_rect ) {
   SDL_RenderCopy( opengadget_renderer, texture, src_rect, dest_rect );
}

void graphics_draw_tile( SDL_Texture* texture, int src_x, int src_y, int dest_x, int dest_y ) {
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
   }

   tile_rect.x = src_x;
   tile_rect.y = src_y;
   screen_rect.x = dest_x;
   screen_rect.y = dest_y;

   graphics_draw( texture, &tile_rect, &screen_rect );
}