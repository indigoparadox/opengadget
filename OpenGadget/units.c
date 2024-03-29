
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

#include "units.h"

const int units_damage_table[UNITS_TYPE_MAX][UNITS_TYPE_MAX] = {
   {  50, 40,  60, 10, 10, 10,  20,  20,  20,  -1,  -1,  -1, -1, -1, 10, -2, -2, -2, -2 },
   {  70, 50,  80, 50, 30, 40,  40,  40,  -1,  -1,  -1,  -1, -1, -1, 10, -2, -2, -2, -2 },
   {  -1, -1,  -1, -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -2, -2, -2, -2 },
   {  90, 70, 100, 50, 60, 60,  70,  70,  70,  -1,  -1,  -1, -1, -1, 60, -2, -2, -2, -2 },
   {  70, 60,  80, 10, 50, 30,  30,  30,  30,  -1,  -1,  -1, -1, -1, 10, -2, -2, -2, -2 },
   {  50, 40,  60, 10, 30, 30,  20,  20,  20,  -1,  -1,  -1, -1, -1, 10, -2, -2, -2, -2 },
   {  90, 70, 100, 50, 50, 50,  60,  60,  60,  -1,  -1,  -1, -1, 30, 50, -2, -2, -2, -2 },
   { 100, 70, 110, 60, 60, 60,  70,  70,  70,  -1,  -1,  -1, -1, 30, 60, -2, -2, -2, -2 },
   {  -1, -1,  -1, -1, -1, -1,  -1,  -1,  -1, 100, 100, 100, -1, -1, -1, -2, -2, -2, -2 },
   {  -1, -1,  -1, -1, -1, -1,  -1,  -1,  -1,  60,  90,  90, -1, -1, -1, -2, -2, -2, -2 },
   { 100, 90, 110, 90, 90, 90, 100, 100, 100,  -1,  -1,  -1, 60, 30, 90, -2, -2, -2, -2 },
   {  50, 50,  60, 10, 10, 10,  10,  10,  10,  -1,  -1,  -1, -1, -1, -1, -2, -2, -2, -2 },
   {  -1, -1,  -1, -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1, 60, 70, 90, -2, -2, -2, -2 },
   { 100, 90, 100, 90, 90, 90,  90,  90,  90,  -1,  -1,  -1, -1, 30, 90, -2, -2, -2, -2 },
   {  50, 50,  60, 10, 10, 10,  10,  10,  10,  -1,  -1,  -1, -1, 10, 10, -2, -2, -2, -2 },
};

const int units_mobility_table[UNITS_TYPE_MAX] = {
   40, 30, 40, 70, 90, 80, 50, 50, 50, 110, 100, 80, 70, 70, 80
};

const int units_terrain_cost_table[UNITS_TYPE_MAX][ISOMAP_TERRAIN_MAX] = {
   { 10, 10, -2, 20, 15, 10, -1, -1, -2, 15, 25, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 10, 10, -2, 20, 15, 10, -1, -1, -2, 15, 25, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 10, 10, -2, 20, 15, 10, -1, -1, -2, 15, 25, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 15, 10, -2, 20, 15, 15, -1, -1, -2, -1, -1, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 15, 10, -2, 20, 20, 15, -1, -1, -2, -1, -1, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 15, 10, -2, 20, 15, 15, -1, -1, -2, -1, -1, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 15, 10, -2, 20, 15, 15, -1, -1, -2, -1, -1, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 15, 10, -2, 20, 15, 15, -1, -1, -2, -1, -1, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 15, 10, -2, 20, 15, 15, -1, -1, -2, -1, -1, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 15, 10, -2, 20, 15, 15, -1, -1, -2, -1, -1, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 10, 10, -2, 10, 10, 10, 10, 10, -2, 10, 10, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 10, 10, -2, 10, 10, 10, 10, 10, -2, 10, 10, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { 10, 10, -2, 10, 10, 10, 10, 10, -2, 10, 10, -1, 10, -2, -2, -2, -2, -2, -2, -2, 10, 10, 10, 10, 10, 10, 10 },
   { -1, -1, -1, -1, -1, -1, 10, 10, -2, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, 10, -1, -1 },
   { -1, -1, -1, -1, -1, -1, 10, 10, -2, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, 10, -1, -1 },
   { -1, -1, -1, -1, -1, 10, 10, 10, -2, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, 10, -1, -1 },
};

static OG_BOOL units_terrain_passable( struct units_unit* unit, struct isomap_tile* tile ) {
   if(
      0 > units_terrain_cost_table[unit->type][tile->terrain] ||
      (NULL != tile->unit && unit != tile->unit)
   ) {
      return OG_FALSE;
   }

   return OG_TRUE;
}

void units_walk_range( struct isomap_tile* tile, struct units_unit* unit, int mobility ) {
   int x_add, y_add, tile_test_index;
   struct isomap_tile* tile_test;
   int new_mobility;

   new_mobility = mobility - units_terrain_cost_table[unit->type][tile->terrain];

   if( 0 >= new_mobility ) {
      /* TODO: This might provide too much mobility. */
      return;
   }

   tile->movable = OG_TRUE;

   for( x_add = -1 ; x_add < 2 ; x_add++ ) {
      for( y_add = -1 ; y_add < 2 ; y_add++ ) {
         tile_test_index = isomap_get_tile( tile->x + x_add, tile->y + y_add, tile->map );
         tile_test = &(tile->map->tiles[tile_test_index]);

         if( 0 != x_add && 0 != y_add ) {
            /* Skip diagonals. */
            continue;
         }

         if( 0 > tile_test_index || tile->map->tiles_count <= tile_test_index ) {
            /* This tile does not exist. */
            continue;
         }

         if( !units_terrain_passable( unit, tile_test ) ) {
            /* Skip blocking tiles. */
            continue;
         }

#if 0
         if( tile_test->movable ) {
            /* This tile has already been walked. */
            continue;
         }
#endif

         /* TODO: Account for tile costs. */

         units_walk_range( tile_test, unit, new_mobility );
      }
   }
}

OG_BOOL units_move( struct units_unit* unit, struct isomap_tile* tile_current, struct isomap_tile*** tiles_previous, int* tiles_previous_count, int distance, struct isomap_tile* tile_dst ) {
   int x_add, y_add, tile_test_index, i, x_inc, y_inc, x_max, y_max, x_start, y_start;
   struct isomap_tile* tile_test;
   OG_BOOL success = OG_FALSE;
   int directions_count = 0;
   int direction_index = 0;
   struct isomap_tile** tiles_previous_root = NULL;
   int tiles_previous_root_count = 0;
   OG_BOOL loop_found = OG_FALSE;
   OG_BOOL root_call = OG_FALSE;

   if( 50 < distance ) {
      goto cleanup;
   } else {
      distance++;
   }

   if( NULL == tiles_previous ) {
      root_call = OG_TRUE;
      tiles_previous = &tiles_previous_root;
      tiles_previous_count = &tiles_previous_root_count;
      (*tiles_previous_count)++;
      *tiles_previous = calloc( *tiles_previous_count, sizeof( struct isomap_tile* ) );
   } else {
      (*tiles_previous_count)++;
      *tiles_previous = realloc( *tiles_previous, (*tiles_previous_count) * sizeof( struct isomap_tile* ) );
   }

   if( NULL == *tiles_previous ) {
#ifdef USE_SDL
      SDL_LogCritical( SDL_LOG_CATEGORY_APPLICATION, "Unable to allocate memory for previous tiles list." );
#endif /* USE_SDL */
      goto cleanup;
   }

   (*tiles_previous)[(*tiles_previous_count) - 1] = tile_current;

   /* Try to move in the general direction of the destination. */
   if( tile_current->x < tile_dst->x ) {
      x_inc = -1;
      x_max = -2;
      x_start = 1;
   } else {
      x_inc = 1;
      x_max = 2;
      x_start = -1;
   }

   if( tile_current->y < tile_dst->y ) {
      y_inc = -1;
      y_max = -2;
      y_start = 1;
   } else {
      y_inc = 1;
      y_max = 2;
      y_start = -1;
   }

   for( x_add = x_start ; x_add != x_max ; x_add += x_inc ) {
      for( y_add = y_start ; y_add != y_max ; y_add += y_inc ) {
         loop_found = OG_FALSE;
         tile_test_index = isomap_get_tile( tile_current->x + x_add, tile_current->y + y_add, tile_current->map );
         tile_test = &(tile_current->map->tiles[tile_test_index]);

         for( i = 0 ; (*tiles_previous_count) > i ; i++ ) {
            if( tile_test->x == (*tiles_previous)[i]->x && tile_test->y == (*tiles_previous)[i]->y ) {
               /* No loops. */
               loop_found = OG_TRUE;
               break;
            }
         }
         if( loop_found ) {
            continue;
         }

         if( 0 != x_add && 0 != y_add ) {
            /* Skip diagonals. */
            continue;
         }

         if( 0 > tile_test_index || tile_current->map->tiles_count <= tile_test_index ) {
            /* This tile does not exist. */
            continue;
         }

         if( !units_terrain_passable( unit, tile_test ) ) {
            /* Skip blocking tiles. */
            continue;
         }

#if 0
         if( tile_test->movable ) {
            /* This tile has already been walked. */
            continue;
         }
#endif

         if( tile_test == tile_dst ) {
            /* We've found our path, so get started actually moving. */
            unit->path_list_count++;
            if( NULL == unit->path_list ) {
               unit->path_list = calloc( (unit->path_list_count), sizeof( struct isomap_tile* ) );
            } else {
               unit->path_list = realloc( unit->path_list, (unit->path_list_count) * sizeof( struct isomap_tile* ) );
            }
            for( i = 0 ; (unit->path_list_count - 1) > i ; i++ ) {
               /* Move the whole list up by one. */
               unit->path_list[i + i] = unit->path_list[i];
            }
            unit->path_list[0] = tile_test;
            //free( tiles_previous );
            success = OG_TRUE;
            goto cleanup;
         } else if( units_move( unit, tile_test, tiles_previous, tiles_previous_count, distance, tile_dst ) ) {
            unit->path_list_count++;
            if( NULL == unit->path_list ) {
               unit->path_list = calloc( (unit->path_list_count), sizeof( struct isomap_tile* ) );
            } else {
               unit->path_list = realloc( unit->path_list, (unit->path_list_count) * sizeof( struct isomap_tile* ) );
            }
            for( i = (unit->path_list_count - 1) ; 0 <= i ; i-- ) {
               /* Move the whole list up by one. */
               unit->path_list[i + 1] = unit->path_list[i];
            }
            unit->path_list[0] = tile_test;
            success = OG_TRUE;
            goto cleanup;
         }

         //direction_index++;
      }
   }

#if 0
   for( i = 0 ; directions_count > i ; i++ ) {
      tile_test = directions[i];
      unit->path_list_count++;
      if( NULL != unit->path_list ) {
         /* This is just another step in the journey. */
         unit->path_list = realloc( unit->path_list, (unit->path_list_count) * sizeof( struct isomap_tile* ) );
      } else {
         /* This is the first step! */
         unit->path_list = calloc( unit->path_list_count, sizeof( struct isomap_tile* ) );
      }
      unit->path_list[unit->path_list_count - 1] = tile_test;
      break;
   }
#endif

cleanup:

   if( root_call ) {
      free( *tiles_previous );
   }

   return success;
}

int units_get_mobility_range( struct units_unit* unit ) {
   return units_mobility_table[unit->type];
}

int units_get_mobility_cost( struct units_unit* unit, struct isomap_tile* tile ) {
   return units_terrain_cost_table[unit->type][tile->terrain];
}
