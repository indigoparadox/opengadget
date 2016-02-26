
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

//! \file       ArcKogado.cs
//! \date       Sun Aug 24 22:01:05 2014
//! \brief      Kogado game engine archive implementation.
//
// Copyright (C) 2014 by morkt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#ifndef ARCKOGADO_H
#define ARCKOGADO_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
struct pak_entry {
   char name[0x15];
   char ext[3];
   long offset; /* fseek() uses long. */
   uint32_t unpacked_size;
   uint32_t size;
   uint8_t compression_type;
   uint8_t is_packed;
   uint8_t has_checksum;
   uint16_t checksum;
   int64_t file_time;
};

struct pak_file {
   int32_t count;
   FILE* file;
   struct pak_entry entries[];
};

struct pak_file* pakopener_try_open( FILE* file );
uint8_t* pakopener_open_entry( const struct pak_file* pak, const struct pak_entry* entry );
void pakopener_free( struct pak_file* pak );
uint8_t* mariel_unpack( uint8_t* input, uint32_t dest_size );

#endif /* ARCKOGAKO_H */
