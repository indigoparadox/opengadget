
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

//[Export(typeof(ArchiveFormat))]
//public class PakOpener : ArchiveFormat
struct pak_file {
   int test;
//public override string         Tag{ get{ return "KOGADO"; } }
//public override string Description{ get{ return arcStrings.KogadoDescription; } }
//public override uint     Signature{ get{ return 0x61507948; } } // 'HyPa'
//public override bool  IsHierarchic{ get{ return false; } }
//public override bool     CanCreate{ get{ return true; } }
};

/*
public sealed class Crc16 : ICheckSum
{
   private ushort m_value = 0xffff;

   public uint Value{ get{ return m_value; } }
}
*/


void try_open( FILE*, struct pak_file* );

#endif /* ARCKOGAKO_H */
