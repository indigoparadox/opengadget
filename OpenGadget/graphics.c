
#include "graphics.h"

static SDL_Renderer* opengadget_renderer = NULL;

RETVAL graphics_init( void ) {
   RETVAL retval = 0;
   SDL_Window* win = NULL;

   retval = SDL_Init( SDL_INIT_VIDEO );
   if( 0 != retval ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_Init: %s", SDL_GetError() );
      goto cleanup;
   }

   win = SDL_CreateWindow( "OpenGadget", 100, 100, 640, 480, SDL_WINDOW_SHOWN );
   if( NULL == win ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_CreateWindow: %s", SDL_GetError() );
      retval = 2;
      goto cleanup;
   }

   opengadget_renderer = SDL_CreateRenderer( win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
   if( NULL == opengadget_renderer ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_CreateRenderer: %s", SDL_GetError() );
      retval = 4;
      goto cleanup;
   }

cleanup:

   if( 0 != retval && NULL != win ) {
      SDL_DestroyWindow( win );
   }

   return retval;
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

SDL_Texture* graphics_image_load( bstring image_name, struct pak_file* pak ) {
   int i;
   struct pak_entry* entry = NULL;
   SDL_Surface* image_out = NULL;
   uint8_t* image_data = NULL;
   SDL_RWops* image_rw = NULL;
   SDL_Texture* texture_out = NULL;

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

   image_out = IMG_Load_RW( image_rw, 0 );
   if( NULL == image_out ) {
      /* TODO: Error message. */
      goto cleanup;
   }

   texture_out = SDL_CreateTextureFromSurface( opengadget_renderer, image_out );

cleanup:

   if( NULL != image_rw ) {
      SDL_RWclose( image_rw );
   }

   if( NULL != image_data ) {
      free( image_data );
   }

   if( NULL != image_out ) {
      SDL_FreeSurface( image_out );
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
