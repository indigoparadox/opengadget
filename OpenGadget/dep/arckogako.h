
#ifndef ARCKOGADO_H
#define ARCKOGADO_H


/*
using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Security.Cryptography;
using System.Diagnostics;
using System.Text;
using GameRes.Formats.Strings;
using GameRes.Utility;
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
struct pak_entry {
   //namespace GameRes.Formats.Kogado
   //{
   //    public class KogadoEntry : PackedEntry
   //    {
   // 0 : Not compressed
   // 1 : Mariel compression
   // 2 : Cocotte compression
   // 3 : Xor 0xff encryption
   /*
   public byte     CompressionType;
   public bool     HasCheckSum;
   public ushort   CheckSum;
   public long     FileTime;
   */
   //}
   char name[0x15];
   char ext[3];
   int64_t offset;
   uint32_t unpacked_size;
   uint32_t size;
   uint8_t compression_type;
   uint8_t is_packed;
   uint8_t has_checksum;
   uint16_t checksum;
   int64_t file_time;
};

//[Export(typeof(ArchiveFormat))]
//public class PakOpener : ArchiveFormat
struct pak_file {
//   int test;
//public override string         Tag{ get{ return "KOGADO"; } }
//public override string Description{ get{ return arcStrings.KogadoDescription; } }
//public override uint     Signature{ get{ return 0x61507948; } } // 'HyPa'
//public override bool  IsHierarchic{ get{ return false; } }
//public override bool     CanCreate{ get{ return true; } }

   int32_t count;
   struct pak_entry entries[];
};

/*
public sealed class Crc16 : ICheckSum
{
   private ushort m_value = 0xffff;

   public uint Value{ get{ return m_value; } }
}
*/

struct pak_file* pakopener_try_open( FILE* file );
uint8_t* pakopener_open_entry( struct pak_file* pak, FILE* file, struct pak_entry* entry );

#endif /* ARCKOGAKO_H */
