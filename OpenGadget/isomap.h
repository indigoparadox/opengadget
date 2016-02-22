
#ifndef ISOMAP_H
#define ISOMAP_H

#include "defines.h"
#include "dep/arckogako.h"

struct isomap {
   uint16_t width;
   uint16_t height;
   uint16_t* tiles;
};

struct isomap* isomap_load_map( uint8_t* map_data, uint32_t map_data_len );

#endif /* ISOMAP_H */