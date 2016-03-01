
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

#include <SDL.h>
#include <stdio.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dep/arckogako.h"
#include "isomap.h"
#include "tbsloop.h"

bstring data_path = NULL;

int main( int argc, char* argv[] ) {
   OG_RETVAL retval = 0;
   FILE* pak_file = NULL;
   FILE* gfx_file = NULL;
   struct pak_file* map_pak = NULL;
   struct isomap* map = NULL;
   struct tbsloop_config map_config;
   uint8_t* map_data = NULL;
   bstring map_data_path = NULL;
   struct stat status;
   OG_BOOL use_data_directory = OG_FALSE;
   long data_size;
   int i;
   struct pak_entry* entry;

#ifdef DEBUG
   SDL_LogSetAllPriority( SDL_LOG_PRIORITY_DEBUG );
#endif

   SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "OpenGadget started..." );

   data_path = bfromcstr( argv[1] );

   if( 0 == _access( bdata( data_path ), 0 ) ) {
      stat( bdata( data_path ), &status );
      use_data_directory = ((status.st_mode & S_IFDIR) != 0);
   } else {
      retval = 16;
      goto cleanup;
   }
   
   /* TODO: Abstract PAK loading so that we can standardize logging. */
   if( use_data_directory ) {
      map_data_path = bformat( "%s\\map.pak", argv[1] );

      pak_file = fopen( bdata( map_data_path ), "rb" );
      if( NULL == pak_file ) {
         SDL_LogCritical( SDL_LOG_CATEGORY_APPLICATION, "Unable to open map. Aborting." );
         retval = 8;
         goto cleanup;
      }

      map_pak = pakopener_try_open( pak_file );
      if( NULL == map_pak ) {
         SDL_LogCritical( SDL_LOG_CATEGORY_APPLICATION, "Unable to open map. Aborting." );
         retval = 8;
         goto cleanup;
      }

      entry = &(map_pak->entries[16]);

      map_data = pakopener_open_entry( map_pak, entry );
      if( NULL == map_data ) {
         SDL_LogCritical( SDL_LOG_CATEGORY_APPLICATION, "Unable to open map. Aborting." );
         retval = 8;
         goto cleanup;
      }

      map_config.map_name = bfromcstr( entry->name );
      map_config.map = isomap_load_map( map_data, entry->unpacked_size );
   } else {
      pak_file = fopen( bdata( data_path ), "rb" );
      if( NULL == pak_file ) {
         SDL_LogCritical( SDL_LOG_CATEGORY_APPLICATION, "Unable to open map. Aborting." );
         retval = 8;
         goto cleanup;
      }

      fseek( pak_file, 0, SEEK_END );
      data_size = ftell( pak_file );
      map_data = calloc( data_size, sizeof( uint8_t ) );
      fseek( pak_file, 0, SEEK_SET );
      fread( map_data, sizeof( uint8_t ), data_size, pak_file );
      fclose( pak_file );
      pak_file = NULL;

      map_config.map_name = bstrcpy( data_path );
      map_config.map = isomap_load_map( map_data, data_size );

      /* TODO: Universal path separator. */
      i = bstrrchr( data_path, '\\' );
      btrunc( data_path, i );
   }

   /* Free this outside of cleanup since it's been loaded and we can save     *
    * some memory.                                                            */
   free( map_data );
   map_data = NULL;
   pakopener_free( map_pak );
   map_pak = NULL;

   retval = graphics_init();
   if( 0 != retval ) {
      goto cleanup;
   }

   retval = tbsloop_loop( &map_config );

cleanup:
   
   if( NULL != pak_file ) {
      fclose( pak_file );
   }

   if( NULL != map_config.map_name ) {
      bdestroy( map_config.map_name );
   }

   if( NULL != map_pak ) {
      /* This frees pak_file above, too. */
      pakopener_free( map_pak );
   }

   if( NULL != map_data ) {
      free( map_data );
   }

   graphics_cleanup();

   SDL_Quit();

   return retval;
}
