
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

#include "graphics.h"

#ifdef USE_SDL
SDL_Renderer* opengadget_renderer = NULL;
SDL_Window* opengadget_window = NULL;

TTF_Font* graphics_fonts[GRAPHICS_FONT_MAX];
#elif defined USE_ALLEGRO
#include <loadpng.h>

BITMAP* graphics_buffer;

typedef struct _OG_PAK_INFO {
   AL_CONST unsigned char *block;
   long length;
   long offset;
} OG_PAK_INFO;

static int og_pak_fclose( void* userdata ) {
   return 0;
}

static int og_pak_getc( void* userdata ) {
   OG_PAK_INFO* info = userdata;
   ASSERT( info );
   ASSERT( info->offset <= info->length );

   if( info->offset == info->length ) {
      return EOF;
   } else {
      return info->block[info->offset++];
   }
}

static int og_pak_ungetc( int c, void* userdata ) {
   OG_PAK_INFO* info = userdata;
   unsigned char ch = c;

   if( (0 < info->offset) && (info->block[info->offset - 1] == ch) ) {
      return ch;
   } else {
      return EOF;
   }
}

static long og_pak_fread( void* p, long n, void* userdata ) {
   OG_PAK_INFO* info = userdata;
   size_t actual;
   ASSERT( info );
   ASSERT( info->offset <= info->length );

   actual = MIN( n, info->length - info->offset );

   memcpy( p, info->block + info->offset, actual );
   info->offset += actual;

   ASSERT( info->offset <= info->length );

   return actual;
}

static int og_pak_putc( int c, void* userdata ) {
    /* STUB */
    return EOF;
}

static long og_pak_fwrite( const void* p, long n, void* userdata ) {
    /* STUB */
    return 0;
}

static int og_pak_fseek( void* userdata, int offset ) {
   OG_PAK_INFO* info = userdata;
   long actual;

   ASSERT( info );
   ASSERT( info->offset <= info->length );

   actual = MIN( offset, info->length - info->offset );

   info->offset += actual;

   ASSERT( info->offset <= info->length );

   if( offset == actual ) {
      return 0;
   } else {
      return -1;
   }
}

static int og_pak_feof( void* userdata ) {
   OG_PAK_INFO* info = userdata;

   return info->offset >= info->length;
}

static int og_pak_ferror( void* userdata ) {
    /* STUB */
    return FALSE;
}

const PACKFILE_VTABLE og_pak_vtable = {
   og_pak_fclose,
   og_pak_getc,
   og_pak_ungetc,
   og_pak_fread,
   og_pak_putc,
   og_pak_fwrite,
   og_pak_fseek,
   og_pak_feof,
   og_pak_ferror,
};
#endif /* USE_SDL */

OG_RETVAL graphics_init( void ) {
   OG_RETVAL retval = 0;

#ifdef USE_SDL
   memset( graphics_fonts, '\0', sizeof( TTF_Font* ) * GRAPHICS_FONT_MAX );

   retval = SDL_Init( SDL_INIT_VIDEO );
   if( 0 != retval ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_Init: %s", SDL_GetError() );
      goto cleanup;
   }

   opengadget_window = SDL_CreateWindow( "OpenGadget", 100, 100, GRAPHICS_SCREEN_WIDTH, GRAPHICS_SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
   if( NULL == opengadget_window ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_CreateWindow: %s", SDL_GetError() );
      retval = 2;
      goto cleanup;
   }

#ifdef WIN32
   opengadget_renderer = SDL_CreateRenderer( opengadget_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
#else
   opengadget_renderer = SDL_CreateRenderer( opengadget_window, -1, SDL_RENDERER_SOFTWARE );
#endif /* WIN32 */
   if( NULL == opengadget_renderer ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_CreateRenderer: %s", SDL_GetError() );
      retval = 4;
      goto cleanup;
   }

   retval = TTF_Init();
   if( 0 != retval ) {
      /* TODO: Error message. */
      goto cleanup;
   }

cleanup:

   if( 0 != retval && NULL != opengadget_window ) {
      SDL_DestroyWindow( opengadget_window );
   }
#elif defined USE_ALLEGRO
    set_color_depth( 32 );
    if( set_gfx_mode( GFX_AUTODETECT_WINDOWED, GRAPHICS_SCREEN_WIDTH, GRAPHICS_SCREEN_HEIGHT, 0, 0 ) != 0 ) {
        allegro_message( "Could not set a 32 bit color resolution." );
        retval = 1;
        goto cleanup;
    }

    graphics_buffer = create_bitmap( GRAPHICS_SCREEN_WIDTH, GRAPHICS_SCREEN_HEIGHT );

cleanup:
#endif /* USE_SDL */

   return retval;
}

void graphics_cleanup( void ) {
#ifdef USE_SDL
   if( NULL != opengadget_renderer ) {
      SDL_DestroyRenderer( opengadget_renderer );
   }

   if( NULL != opengadget_window ) {
      SDL_DestroyWindow( opengadget_window );
   }
#elif defined USE_ALLEGRO
   if( NULL != graphics_buffer ) {
      destroy_bitmap( graphics_buffer );
   }
#endif /* USE_SDL */
}

void graphics_set_title( const bstring title ) {
#ifdef USE_SDL
   SDL_SetWindowTitle( opengadget_window, bdata( title ) );
#endif /* USE_SDL */
}

void graphics_clear( void ) {
#ifdef USE_SDL
   SDL_RenderClear( opengadget_renderer );
#elif defined USE_ALLEGRO
    clear_bitmap( graphics_buffer );
#endif /* USE_SDL */
}

void graphics_end_draw( void ) {
#ifdef USE_SDL
   SDL_RenderPresent( opengadget_renderer );
#elif defined USE_ALLEGRO
    blit( graphics_buffer, screen, 0, 0, 0, 0, GRAPHICS_SCREEN_WIDTH, GRAPHICS_SCREEN_HEIGHT );
#endif /* USE_SDL */
}

void graphics_sleep( const int milliseconds ) {
#ifdef USE_SDL
   SDL_Delay( milliseconds );
#elif defined USE_ALLEGRO
   usleep( 1000 );
#endif /* USE_SDL */
}

static void graphics_texture_colorkey( OG_Texture* texture, int h, uint8_t r, uint8_t g, uint8_t b ) {
#ifdef USE_SDL
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
#endif /* USE_SDL */
}

OG_Texture* graphics_image_load( const bstring image_name, const struct pak_file* pak ) {
   OG_Texture* texture_out = NULL;
   int i;
   const struct pak_entry* entry = NULL;
#ifdef USE_SDL
   SDL_Surface* surface_temp = NULL;
   SDL_Surface* image_opt = NULL;
   SDL_RWops* image_rw = NULL;
   SDL_Surface* surface_formatted = NULL;
#elif defined USE_ALLEGRO
   PALETTE pal;
   OG_PAK_INFO pak_info;
   PACKFILE* pak_mem;
#endif /* USE_SDL */
   uint8_t* image_data = NULL;

   for( i = 0 ; pak->count > i ; i++ ) {
      /* TODO: Pick the shortest length possible so we don't overflow. */
      if( 0 == strncmp( (char*)bdata( image_name ), pak->entries[i].name, blength( image_name ) ) ) {
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

#ifdef USE_SDL
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
#elif defined USE_ALLEGRO
   pak_info.block = image_data;
   pak_info.length = entry->unpacked_size;
   pak_info.offset = 0;
   pak_mem = pack_fopen_vtable( &og_pak_vtable, &pak_info );
   if( NULL == pak_mem ) {
      goto cleanup;
   }
   texture_out = load_bmp_pf( pak_mem, NULL );
   //texture_out = load_memory_png( image_data, entry->unpacked_size, pal );
   if( NULL == texture_out ) {
      //allegro_message( "Error opening image file." );
      goto cleanup;
   }
   //set_palette( pal );

cleanup:
#endif /* USE_SDL */

   return texture_out;
}

OG_Texture* graphics_text_create( bstring text, GRAPHICS_FONT font_index, int size ) {
   OG_Texture* message_texture = NULL;

#ifdef USE_SDL
   SDL_Color white = { 0, 0, 0 };
   SDL_Surface* message_surface = NULL;

   if( NULL == graphics_fonts[font_index] ) {
      switch( font_index ) {
         case GRAPHICS_FONT_SANS:
            graphics_fonts[font_index] = TTF_OpenFont( "C:\\Windows\\Fonts\\Arial.ttf", size );
            break;
      }
   }

   message_surface = TTF_RenderText_Solid( graphics_fonts[font_index], bdata( text ), white );
   if( NULL == message_surface ) {
      /* TODO: Error message. */
      goto cleanup;
   }

   message_texture = SDL_CreateTextureFromSurface( opengadget_renderer, message_surface );

cleanup:

   if( NULL != message_surface ) {
      SDL_FreeSurface( message_surface );
   }
#endif /* USE_SDL */

   return message_texture;
}

void graphics_draw( OG_Texture* texture, OG_Rect* src_rect, OG_Rect* dst_rect ) {
#ifdef USE_SDL
   SDL_RenderCopy( opengadget_renderer, texture, src_rect, dst_rect );
#elif defined USE_ALLEGRO
   blit( texture, graphics_buffer, src_rect->x, src_rect->y, dst_rect->x,dst_rect->y, dst_rect->w, dst_rect->h );
#endif /* USE_SDL */
}

void graphics_draw_tile( const OG_Texture* texture, const int src_x, const int src_y, const int dest_x, const int dest_y ) {
   static OG_Rect tile_rect;
   static OG_Rect screen_rect;
   static uint8_t rects_init = 0;

   if( 0 == rects_init ) {
      /* Setup rectangles for the first time. */
      memset( &tile_rect, '\0', sizeof( OG_Rect ) );
      memset( &screen_rect, '\0', sizeof( OG_Rect ) );

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

   graphics_draw( texture, &tile_rect, &screen_rect );
}

void graphics_draw_bg( OG_Texture* background ) {
   OG_Rect bg_src;
   OG_Rect bg_dst;

   bg_src.x = 0;
   bg_src.y = 0;
#ifdef USE_SDL
   SDL_QueryTexture( background, NULL, NULL, &bg_src.w, &bg_src.h );
#elif defined USE_ALLEGRO
   bg_src.w = background->w;
   bg_src.h = background->h;
#endif /* USE_SDL */
   bg_dst.w = bg_src.w;
   bg_dst.h = bg_src.h;
   for( bg_dst.x = 0 ; GRAPHICS_SCREEN_WIDTH > bg_dst.x ; bg_dst.x += bg_src.w ) {
      for( bg_dst.y = 0 ; GRAPHICS_SCREEN_HEIGHT > bg_dst.y ; bg_dst.y += bg_src.h ) {
         graphics_draw( background, &bg_src, &bg_dst );
      }
   }
}

void graphics_transform_isometric_reverse(
   int* tile_x,
   int* tile_y,
   int screen_x,
   int screen_y,
   int map_width,
   int map_height,
   const OG_Rect* viewport,
   int rotation
) {
   screen_x -= viewport->x;
   screen_y -= viewport->y;

   screen_x /= (GRAPHICS_TILE_WIDTH / 2);
   screen_y /= -1 * (GRAPHICS_TILE_HEIGHT / 4);

   *tile_x = ((screen_x + screen_y) / 2) + 1;
   *tile_y = ((screen_x - screen_y) / 2) - 2;

   /* I'm not quite sure what I'm doing, but I discovered by trial and error  *
    * that width and height are reversed in these formulae when dealing with  *
    * the mouse cursor. So they're fully expanded and modified, here.         */
   switch( rotation ) {
      case GRAPHICS_ROTATE_90:
         *tile_x = *tile_x ^ *tile_y;
         *tile_y = *tile_x ^ *tile_y;
         *tile_x = *tile_x ^ *tile_y;
         *tile_x = map_height - *tile_x;
         break;

      case GRAPHICS_ROTATE_180:
         *tile_x = map_width - *tile_x;
         *tile_y = map_height - *tile_y;
         break;

      case GRAPHICS_ROTATE_270:
         *tile_x = *tile_x ^ *tile_y;
         *tile_y = *tile_x ^ *tile_y;
         *tile_x = *tile_x ^ *tile_y;
         *tile_y = map_width - *tile_y;
         break;
   }
}

/* TODO: Maybe make this smarter with XOR at some point, but don't            *
 * overcomplicate right now.                                                  */
#ifdef USE_INLINE_HELPERS
#ifdef _MSC_VER
__inline
#else
inline
#endif /* _MSC_VER */
#endif /* USE_INLINE_HELPERS */
void graphics_isometric_tile_rotate(
   int* x, int* y, int width, int height, GRAPHICS_ROTATE rotation
) {
   switch( rotation ) {
      case GRAPHICS_ROTATE_90:
         *x = *x ^ *y;
         *y = *x ^ *y;
         *x = *x ^ *y;
         *y = height - *y;
         break;
      case GRAPHICS_ROTATE_180:
         *x = width - *x;
         *y = height - *y;
         break;
      case GRAPHICS_ROTATE_270:
         *x = *x ^ *y;
         *y = *x ^ *y;
         *x = *x ^ *y;
         *x = width - *x;
         break;
   }
}

#ifdef USE_INLINE_HELPERS
#ifdef _MSC_VER
__inline
#else
inline
#endif /* _MSC_VER */
#endif /* USE_INLINE_HELPERS */
void graphics_transform_isometric(
   float tile_x, float tile_y, int* screen_x, int* screen_y, const OG_Rect* viewport
) {
   *screen_x = viewport->x + (tile_x * GRAPHICS_TILE_WIDTH / 2) + \
      (tile_y * GRAPHICS_TILE_WIDTH / 2); \
   *screen_y = viewport->y + ((tile_y * GRAPHICS_TILE_OFFSET_X / 2) - \
         (tile_x * GRAPHICS_TILE_OFFSET_X / 2));
}

void graphics_destroy_texture( OG_Texture* texture ) {
#ifdef USE_SDL
    SDL_DestroyTexture( texture );
#elif defined USE_ALLEGRO
    destroy_bitmap( texture );
#endif /* USE_SDL */
}
