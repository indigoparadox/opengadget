
#include <stdio.h>

#include "dep/arckogako.h"

int main( int argc, char* argv[] ) {

   struct pak_file* pak;
   struct pak_entry* entry;
   FILE* file;
   FILE* file_out;
   char* contents;
   int i;
   int extract;

   extract = atoi( argv[2] );
   
   file = fopen( argv[1], "rb" );

   pak = pakopener_try_open( file );

   /*
   for( i = 0 ; i < pak->count ; i++ ) {
      printf( "%s\n", pak->entries[i].name );
   }
   */

   entry = &(pak->entries[extract]);

   contents = pakopener_open_entry( pak, entry );

   file_out = fopen( entry->name, "wb" );

   fwrite( contents, entry->unpacked_size, 1, file_out );

   fclose( file_out );

   return 0;
}
