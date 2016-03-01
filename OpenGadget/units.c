
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

void units_select( struct units_unit* unit ) {
   unit->tile->movable = OG_TRUE;
}

void units_walk_range( struct isomap_tile* tile, struct units_unit* unit, int mobility ) {
   int x_add, y_add, tile_test_index;
   struct isomap_tile* tile_test;
   int new_mobility;
   
   if( 0 > units_terrain_cost_table[unit->type][tile->terrain] ) {
      /* Negative cost means a blocking tile. */
      return;
   }

   new_mobility = mobility - units_terrain_cost_table[unit->type][tile->terrain];

   if( 0 >= new_mobility ) {
      /* TODO: This might provide too much mobility. */
      return;
   }

   tile->movable = OG_TRUE;

   for( x_add = -1 ; x_add < 2 ; x_add ++ ) {
      for( y_add = -1 ; y_add < 2 ; y_add ++ ) {
         tile_test_index = isomap_get_tile( tile->x + x_add, tile->y + y_add, tile->map );

         if( 0 > tile_test_index || tile->map->tiles_count <= tile_test_index ) {
            /* This tile does not exist. */
            continue;
         }

         tile_test = &(tile->map->tiles[tile_test_index]);

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

int units_get_mobility_range( struct units_unit* unit ) {
   return units_mobility_table[unit->type];
}

int units_get_mobility_cost( struct units_unit* unit, struct isomap_tile* tile ) {
   return units_terrain_cost_table[unit->type][tile->terrain];
}
