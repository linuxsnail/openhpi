/*
 * marshaling/demarshaling
 *
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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <endian.h>
#include <byteswap.h>
#include "marshal.h"


cMarshalType VoidType =
{
  .m_type = eMtVoid
};

cMarshalType Uint8Type =
{
  .m_type = eMtUint8
};

cMarshalType Uint16Type =
{
  .m_type = eMtUint16
};

cMarshalType Uint32Type =
{
  .m_type = eMtUint32
};

cMarshalType Uint64Type =
{
  .m_type = eMtUint64
};

cMarshalType Int8Type =
{
  .m_type = eMtInt8
};

cMarshalType Int16Type =
{
  .m_type = eMtInt16
};

cMarshalType Int32Type =
{
  .m_type = eMtInt32
};

cMarshalType Int64Type =
{
  .m_type = eMtInt64
};

cMarshalType Float32Type =
{
  .m_type = eMtFloat32
};

cMarshalType Float64Type =
{
  .m_type = eMtFloat64
};


int
MarshalByteOrder()
{
  if ( __BYTE_ORDER == __LITTLE_ENDIAN )
       return 1;

  return 0;
}


int
IsSimpleType( tMarshalType type )
{
  assert( type != eMtUnknown );
  
  switch( type )
     {
       case eMtUnknown:
            assert( type != eMtUnknown );
            return 0;

       case eMtVoid:
       case eMtUint8:
       case eMtUint16:
       case eMtUint32:
       case eMtUint64:
       case eMtInt8:
       case eMtInt16:
       case eMtInt32:
       case eMtInt64:
       case eMtFloat32:
       case eMtFloat64:
            return 1;

       case eMtArray:
       case eMtVarArray:
       case eMtStruct:
       case eMtStructElement:
       case eMtUnion:
       case eMtUnionElement:
       case eMtUserDefined:
            return 0;
     }

  // not reached
  assert( 0 );
  return 0;
}


int
MarshalSize( const cMarshalType *type )
{
  switch( type->m_type )
     {
       case eMtUnknown:
	    assert( 0 );
	    return 0;

       case eMtVoid:
	    return 0;

       case eMtUint8:
       case eMtInt8:
	    return sizeof( tUint8 );

       case eMtUint16:
       case eMtInt16:
	    return sizeof( tUint16 );

       case eMtUint32:
       case eMtInt32:
	    return sizeof( tUint32 );

       case eMtUint64:
       case eMtInt64:
	    return sizeof( tUint64 );

       case eMtFloat32:
	    return sizeof( tFloat32 );

       case eMtFloat64:
	    return sizeof( tFloat64 );

       case eMtArray:
	    assert( type->m_u.m_array.m_size > 0 );
	    assert( type->m_u.m_array.m_type );

	    return type->m_u.m_array.m_size * MarshalSize( type->m_u.m_array.m_type );

       case eMtVarArray:
            return 0xffff;

       case eMtStruct:
	    {
	      assert( type->m_u.m_struct.m_elements );

	      int i;
	      int size = 0;

	      for( i = 0; type->m_u.m_struct.m_elements[i].m_type == eMtStructElement; i++ )
		 {
		   cMarshalType *elem = &type->m_u.m_struct.m_elements[i];
		   int s = MarshalSize( elem->m_u.m_struct_element.m_type );

		   if ( s < 0 )
			return -1;
		   
		   size += s;
		 }

	      return size;
	    }

       case eMtUnion:
            {
	      assert( type->m_u.m_union.m_elements );

	      int i;
	      int max = 0;

	      for( i = 0; type->m_u.m_union.m_elements[i].m_type == eMtUnionElement; i++ )
		 {
		   cMarshalType *elem = &type->m_u.m_union.m_elements[i];

                   int s = MarshalSize( elem->m_u.m_union_element.m_type );

		   if ( s < 0 )
			return -1;

                   if ( max < s )
                        max = s;
		 }

	      return max;
            }

       case eMtUserDefined:
            return 0xffff;

       case eMtStructElement:
       case eMtUnionElement:
	    assert( 0 );
	    return -1;
     }

  // not reached
  return -1;
}


int
MarshalSizeArray( const cMarshalType **types )
{
  int size = 0;
  int i;

  for( i = 0; types[i]; i++ )
     {
       int s = MarshalSize( types[i] );

       if ( s < 0 )
	    return -1;
       
       size += s;
     }

  return size;
}


int
MarshalSimpleTypes( tMarshalType type, const void *data,
                    void *buffer )
{
  switch( type )
     {
       case eMtVoid:
	    return 0;

       case eMtUint8:
       case eMtInt8:
            {
              tUint8 v = *(const tUint8 *)data;
              *(tUint8 *)buffer = v;
            }

            return sizeof( tUint8 );

       case eMtInt16:
       case eMtUint16:
            {
              tUint16 v = *(const tUint16 *)data;
              *(tUint16 *)buffer = v;
            }

            return sizeof( tUint16 );

       case eMtUint32:
       case eMtInt32:
            {
              tUint32 v = *(const tUint32 *)data;
              *(tUint32 *)buffer = v;
            }

            return sizeof( tUint32 );

       case eMtUint64:
       case eMtInt64:
            {
              tUint64 v = *(const tUint64 *)data;
              *(tUint64 *)buffer = v;
            }

            return sizeof( tUint64 );

       case eMtFloat32:
            {
              tFloat32 v = *(const tFloat32 *)data;
              *(tFloat32 *)buffer = v;
            }

            return sizeof( tFloat32 );

       case eMtFloat64:
            {
              tFloat64 v = *(const tFloat64 *)data;
              *(tFloat64 *)buffer = v;
            }

            return sizeof( tFloat64 );

       default:
            break;
     }

  assert( 0 );

  return -1;
}


static const cMarshalType *
FindUnionModifierType( const cMarshalType *type, cMarshalType *st_type, const void *d )
{
  cMarshalType *mod_struct_element = &type->m_u.m_struct.m_elements[st_type->m_u.m_union.m_offset];
  assert( mod_struct_element->m_type == eMtStructElement );
  cMarshalType *mod_type = mod_struct_element->m_u.m_struct_element.m_type;
  const void *so = (const unsigned char *)d + mod_struct_element->m_u.m_struct_element.m_offset;

  tUint32 m;

  switch( mod_type->m_type )
     {
       case eMtUint8:
       case eMtInt8:
	    m = (tUint32)*(const tUint8 *)so;
	    break;

       case eMtUint16:
       case eMtInt16:
	    m = (tUint32)*(const tUint16 *)so;
	    break;

       case eMtUint32:
       case eMtInt32:
	    m = *(const tUint32 *)so;
	    break;

       default:
	    assert( 0 );
	    m = 0;

	    return 0;
     }

  int i;

  for( i = 0; st_type->m_u.m_union.m_elements[i].m_type == eMtUnionElement; i++ )
       if ( st_type->m_u.m_union.m_elements[i].m_u.m_union_element.m_mod == m )
	    return st_type->m_u.m_union.m_elements[i].m_u.m_union_element.m_type;

  return 0;
}


static int
FindArraySize( const cMarshalType *type, cMarshalType *st_type, const void *d )
{
  cMarshalType *size_struct_element = &type->m_u.m_struct.m_elements[st_type->m_u.m_var_array.m_size];
  assert( size_struct_element->m_type == eMtStructElement );
  cMarshalType *size_type = size_struct_element->m_u.m_struct_element.m_type;
  const void *so = (const unsigned char *)d + size_struct_element->m_u.m_struct_element.m_offset;

  tUint32 size;

  switch( size_type->m_type )
     {
       case eMtUint8:
       case eMtInt8:
	    size = (tUint32)*(const tUint8 *)so;
	    break;

       case eMtUint16:
       case eMtInt16:
	    size = (tUint32)*(const tUint16 *)so;
	    break;

       case eMtUint32:
       case eMtInt32:
	    size = *(const tUint32 *)so;
	    break;

       default:
	    assert( 0 );
	    return -1;
     }

  return size;
}


int
Marshal( const cMarshalType *type, const void *d, void *b )
{
  if ( IsSimpleType( type->m_type ) )
       return MarshalSimpleTypes( type->m_type, d, b );

  int                  size   = 0;
  const unsigned char *data   = d;
  unsigned char       *buffer = b;

  switch( type->m_type )
     {
       case eMtArray:
            {
              int i;

              //assert( IsSimpleType( type->m_u.m_array.m_type->m_type ) );

              for( i = 0; i < type->m_u.m_array.m_size; i++ )
                 {
                   int s = Marshal( type->m_u.m_array.m_type, data, buffer );

		   if ( s < 0 )
			return -1;

                   data   += s;
                   buffer += s;
                   size   += s;
                 }
            }
            break;

       case eMtStruct:
	    {
	      int i;
 
	      for( i = 0; type->m_u.m_struct.m_elements[i].m_type == eMtStructElement; i++ )
		 {
		   cMarshalType *struct_element = &type->m_u.m_struct.m_elements[i];
                   assert( struct_element->m_type == eMtStructElement );

                   cMarshalType *st_type = struct_element->m_u.m_struct_element.m_type;

                   int s = 0;

                   if ( st_type->m_type == eMtUnion )
                      {
                        // the mod must be before this entry.
                        // this is a limitation of demarshaling of unions
                        assert( st_type->m_u.m_union.m_offset < i );
			const cMarshalType *mod = FindUnionModifierType( type, st_type, data );

			if ( mod )
			   {
			     s = Marshal( mod, data + struct_element->m_u.m_struct_element.m_offset, buffer );

			     if ( s < 0 )
				  return -1;
			   }
			else
			     return -1;
                      }
                   else if ( st_type->m_type == eMtVarArray )
                      {
                        // the array size must be before this entry.
                        // this is a limitation of demarshaling of var arrays
                        assert( st_type->m_u.m_var_array.m_size < i );

			int array_size = FindArraySize( type, st_type, data );

                        // only simple types can be array elements
                        assert( IsSimpleType( st_type->m_u.m_var_array.m_type->m_type ) );

                        unsigned char *bb = buffer;
                        const unsigned char *dd = data + struct_element->m_u.m_struct_element.m_offset;
                        tUint32 j;

                        for( j = 0; j < array_size; j++ )
                           {
                             int si = Marshal( st_type->m_u.m_var_array.m_type,
					       dd, bb );

			     if ( si < 0 )
				  return -1;

                             bb += si;
                             dd += si;
                             s  += si;
                           }
                      }
                   else
		      {
                        s = Marshal( st_type, data + struct_element->m_u.m_struct_element.m_offset,
                                     buffer );

			if ( s < 0 )
			     return -1;
		      }

		   buffer += s;
                   size   += s;
		 }
	    }
	    break;

       case eMtUnion:
            assert( 0 );
	    return -1;

       case eMtUserDefined:
	    assert( type->m_u.m_user_defined.m_marshal );
	    size = type->m_u.m_user_defined.m_marshal( type, d, b, type->m_u.m_user_defined.m_user_data );
	    break;

       default:
            assert( 0 );
            return -1;
     }

  return size;
}


int
MarshalArray( const cMarshalType **types, 
	      const void **data, void *b )
{
  int            i;
  int            size = 0;
  unsigned char *buffer = b;

  for( i = 0; types[i]; i++ )
     {
       int s = Marshal( types[i], data[i], buffer );

       if ( s < 0 )
	    return -1;

       size   += s;
       buffer += s;
     }

  return size;
}


// for byte swap float 32
typedef union
{
  tUint32  m_u32;
  tFloat32 m_f32;
} tFloat32Uint32;


// for byte swap float 64
typedef union
{
  tUint64  m_u64;
  tFloat64 m_f64;
} tFloat64Uint64;


int
DemarshalSimpleTypes( int byte_order, tMarshalType type,
                      void *data, const void *buffer )
{
  switch( type )
     {
       case eMtVoid:
	    return 0;

       case eMtUint8:
       case eMtInt8:
            {
              tUint8 v = *(const tUint8 *)buffer;
              *(tUint8 *)data = v;
            }

            return sizeof( tUint8 );

       case eMtInt16:
       case eMtUint16:
            {
              tUint16 v = *(const tUint16 *)buffer;

              if ( MarshalByteOrder() != byte_order )
                   v = bswap_16( v );
              
              *(tUint16 *)data = v;
            }            

            return sizeof( tUint16 );

       case eMtUint32:
       case eMtInt32:
            {
              tUint32 v = *(const tUint32 *)buffer;

              if ( MarshalByteOrder() != byte_order )
                   v = bswap_32( v );

              *(tUint32 *)data = v;
            }

            return sizeof( tUint32 );

       case eMtUint64:
       case eMtInt64:
            {
              tUint64 v = *(const tUint64 *)buffer;

              if ( MarshalByteOrder() != byte_order )
                   v = bswap_64( v );

              *(tUint64 *)data = v;
            }

            return sizeof( tUint64 );

       case eMtFloat32:
            {
              // this has been tested for i386 and PPC
              tFloat32Uint32 v;
              v.m_f32 = *(const tFloat32 *)buffer;

              if ( MarshalByteOrder() != byte_order )
                   v.m_u32 = bswap_32( v.m_u32 );

              *(tFloat32 *)data = v.m_f32;
            }

            return sizeof( tFloat32 );

       case eMtFloat64:
            {
              // this has been tested for i386 and PPC
              tFloat64Uint64 v;
              v.m_f64 = *(const tFloat64 *)buffer;

              if ( MarshalByteOrder() != byte_order )
                   v.m_u64 = bswap_64( v.m_u64 );

              *(tFloat64 *)data = v.m_f64;
            }

            return sizeof( tFloat64 );

       default:
            break;
     }

  assert( 0 );

  return -1;
}


int
Demarshal( int byte_order, const cMarshalType *type, 
           void *d, const void *b )
{
  if ( IsSimpleType( type->m_type ) )
       return DemarshalSimpleTypes( byte_order, type->m_type, d, b );

  int                  size = 0;
  unsigned char       *data  = d;
  const unsigned char *buffer = b;

  switch( type->m_type )
     {
       case eMtArray:
            {
              int i;

              for( i = 0; i < type->m_u.m_array.m_size; i++ )
                 {
                   int s = Demarshal( byte_order, type->m_u.m_array.m_type,
				      data, buffer );

		   if ( s < 0 )
			return -1;

                   data   += s;
                   buffer += s;
                   size   += s;
                 }
            }
            break;

       case eMtStruct:
	    {
	      int i;
 
	      for( i = 0; type->m_u.m_struct.m_elements[i].m_type == eMtStructElement; i++ )
		 {
		   cMarshalType *struct_element = &type->m_u.m_struct.m_elements[i];
                   assert( struct_element->m_type == eMtStructElement );

                   cMarshalType *st_type = struct_element->m_u.m_struct_element.m_type;
		   int s = 0;

		   if ( st_type->m_type == eMtUnion )
		      {
                        // the mod must be before this entry.
                        // this is a limitation of demarshaling of unions
                        assert( st_type->m_u.m_union.m_offset < i );
			const cMarshalType *mod = FindUnionModifierType( type, st_type, data );

			if ( mod )
			   {
			     s = Demarshal( byte_order, mod, data + struct_element->m_u.m_struct_element.m_offset, buffer );

			     if ( s < 0 )
				  return -1;
			   }
			else
			     return -1;
		      }
		   else if ( st_type->m_type == eMtVarArray )
		      {
                        // the array size must be before this entry.
                        // this is a limitation of demarshaling of var arrays
                        assert( st_type->m_u.m_var_array.m_size < i );

			tUint32 array_size = FindArraySize( type, st_type, data );

                        // only simple types can be array elements
                        assert( IsSimpleType( st_type->m_u.m_var_array.m_type->m_type ) );

                        const unsigned char *bb = buffer;
                        unsigned char       *dd = data + struct_element->m_u.m_struct_element.m_offset;
                        tUint32 j;

                        for( j = 0; j < array_size; j++ )
                           {
                             int si = Demarshal( byte_order, st_type->m_u.m_var_array.m_type,
						 dd, bb );

			     if ( si < 0 )
				  return -1;

                             bb += si;
                             dd += si;
                             s  += si;
                           }
                      }
                   else
		      {
			s = Demarshal( byte_order, st_type,
				       data + struct_element->m_u.m_struct_element.m_offset,
				       buffer );

			if ( s < 0 )
			     return -1;
		      }

		   buffer += s;
                   size   += s;
		 }
	    }

	    break;

       case eMtUnion:
	    // unions must me encapsulate in structs
            assert( 0 );
	    return -1;

       case eMtStructElement:
       case eMtUnionElement:
	    assert( 0 );
	    return -1;

       case eMtUserDefined:
	    assert( type->m_u.m_user_defined.m_demarshal );
	    size = type->m_u.m_user_defined.m_demarshal( byte_order, type, d, b,
							 type->m_u.m_user_defined. m_user_data );

	    if ( size < 0 )
		 return -1;

	    break;

       default:
            assert( 0 );
            return -1;
     }

  return size;
}


int
DemarshalArray( int byte_order, const cMarshalType **types,
                void **data, const void *b )
{
  int i;
  int size = 0;
  const unsigned char *buffer = b;

  for( i = 0; types[i]; i++ )
     {
       int s = Demarshal( byte_order, types[i], data[i], buffer );

       if ( s < 0 )
	    return -1;

       size   += s;
       buffer += s;
     }

  return size;
}
