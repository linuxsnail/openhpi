/*
 * Copyright (c) 2004 by FORCE Computers.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *     Thomas Kanngieser <thomas.kanngieser@fci.com>
 */

#include "marshal.h"


int
main( int argc, char *argv[] )
{
  tUint64       value = 0xfedc12345678abcdLL;
  tUint64       result;
  unsigned char buffer[256];

  unsigned int s1 = Marshal( &Marshal_Uint64Type, &value, buffer );

  if ( s1 != sizeof( tUint64 ) )
       return 1;

  unsigned int s2 = Demarshal( MarshalByteOrder(), &Marshal_Uint64Type, &result, buffer );

  if ( s2 != sizeof( tUint64 ) )
       return 1;

  if ( value != result )
       return 1;

  return 0;
}
