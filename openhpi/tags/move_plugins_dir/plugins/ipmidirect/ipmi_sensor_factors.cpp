/*
 * ipmi_sensor_factors.cpp
 *
 * Copyright (c) 2004 by FORCE Computers
 *
 * Note that this file is based on parts of OpenIPMI
 * written by Corey Minyard <minyard@mvista.com>
 * of MontaVista Software. Corey's code was helpful
 * and many thanks go to him. He gave the permission
 * to use this code in OpenHPI under BSD license.
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


#include "ipmi_sensor_factors.h"
#include <math.h>


static const char *analoge_data_format[] =
{
  "unsigned",
  "1compl",
  "2compl",
  "analog"
};


const char *
IpmiAnalogeDataFormatToString( tIpmiAnalogeDataFormat fmt )
{
  if ( (int)fmt <= eIpmiAnalogDataFormatNotAnalog )
       return analoge_data_format[fmt];

  return "invalid";
}


static const char *linearization_map[] =
{
  "linear",
  "ln",
  "log10",
  "log2",
  "e",
  "exp10",
  "exp2",
  "1overX",
  "sqr",
  "cube",
  "sqrt",
  "1 over cube"
};


const char *
IpmiLinearizationToString( tIpmiLinearization val )
{
  if ( val == eIpmiLinearizationNonlinear )
       return "non linear";

  if ( val <= eIpmiLinearization1OverCube )
       return linearization_map[val];

  return "invalid";
}


cIpmiSensorFactors::cIpmiSensorFactors()
  : m_analog_data_format( eIpmiAnalogDataFormatUnsigned ),
    m_linearization( eIpmiLinearizationLinear ),
    m_m( 0 ),
    m_tolerance( 0 ),
    m_b( 0 ),
    m_r_exp( 0 ),
    m_accuracy_exp( 0 ),
    m_accuracy( 0 ),
    m_b_exp( 0 )
{
}


cIpmiSensorFactors::~cIpmiSensorFactors()
{
}


bool
cIpmiSensorFactors::GetDataFromSdr( cIpmiSdr *sdr )
{
  m_analog_data_format = (tIpmiAnalogeDataFormat)((sdr->m_data[20] >> 6) & 3);
  m_linearization = (tIpmiLinearization)(sdr->m_data[23] & 0x7f);

  if ( m_linearization <= 11 )
     {
       m_m            = sdr->m_data[24] | ((sdr->m_data[25] & 0xc0) << 2);
       m_tolerance    = sdr->m_data[25] & 0x3f;
       m_b            = sdr->m_data[26] | ((sdr->m_data[27] & 0xc0) << 2);
       m_accuracy     = ((sdr->m_data[27] & 0x3f)
                            | ((sdr->m_data[28] & 0xf0) << 2));
       m_accuracy_exp = (sdr->m_data[28] >> 2) & 0x3;
       m_r_exp        = (sdr->m_data[29] >> 4) & 0xf;
       m_b_exp        = sdr->m_data[29] & 0xf;
     }

  return true;
}


bool
cIpmiSensorFactors::Cmp( const cIpmiSensorFactors &sf )
{
  if ( m_analog_data_format != sf.m_analog_data_format )
       return false;

  if ( m_linearization != sf.m_linearization )
       return false;

  if ( m_linearization <= 11 )
     {
       if ( m_m            != sf.m_m            )
            return false;

       if ( m_tolerance    != sf.m_tolerance    )
            return false;

       if ( m_b            != sf.m_b            )
            return false;

       if ( m_accuracy     != sf.m_accuracy     )
            return false;

       if ( m_accuracy_exp != sf.m_accuracy_exp )
            return false;

       if ( m_r_exp        != sf.m_r_exp        )
            return false;

       if ( m_b_exp        != sf.m_b_exp        )
            return false;
     }

  return true;
}


static double
c_linear( double val )
{
  return val;
}


static double
c_exp10( double val )
{
  return pow( 10.0, val );
}


static double
c_exp2( double val )
{
  return pow( 2.0, val );
}


static double
c_1_over_x( double val )
{
  return 1.0 / val;
}


static double
c_sqr( double val )
{
  return pow( val, 2.0 );
}


static double
c_cube( double val )
{
  return pow( val, 3.0 );
}


static double
c_1_over_cube( double val )
{
  return 1.0 / pow( val, 3.0 );
}


typedef double (*linearizer)( double val );
static linearizer linearize[11] =
{
  c_linear,
  log,
  log10,
  exp,
  c_exp10,
  c_exp2,
  c_1_over_x,
  c_sqr,
  c_cube,
  sqrt,
  c_1_over_cube
};


static int
sign_extend( int m, int bits )
{
    if ( m & (1 << (bits-1)) )
	return m | (-1 << bits);
    else
	return m & (~(-1 << bits));
}


bool
cIpmiSensorFactors::ConvertFromRaw( unsigned int val,
                                    double      &result )
{
  double m, b, b_exp, r_exp, fval;
  linearizer c_func;

  if ( m_linearization == eIpmiLinearizationNonlinear )
       c_func = c_linear;
  else if ( m_linearization <= 11 )
       c_func = linearize[m_linearization];
  else
       return false;

  val &= 0xff;

  m     = m_m;
  b     = m_b;
  r_exp = m_r_exp;
  b_exp = m_b_exp;

  switch( m_analog_data_format )
     {
       case eIpmiAnalogDataFormatUnsigned:
	    fval = val;
	    break;

       case eIpmiAnalogDataFormat1Compl:
	    val = sign_extend( val, 8 );
	    if ( val == 0xffffffff )
                 val += 1;

	    fval = val;
	    break;

       case eIpmiAnalogDataFormat2Compl:
	    fval = sign_extend( val, 8 );
	    break;

       default:
	    return false;
     }

  result = c_func( ((m * fval) + (b * pow(10, b_exp))) * pow(10, r_exp) );

  return true;
}


bool
cIpmiSensorFactors::ConvertToRaw( tIpmiRound    rounding,
                                  double        val,
                                  unsigned int &result )
{
  bool rv;
  double cval;
  int    lowraw, highraw, raw, maxraw, minraw, next_raw;

  switch( m_analog_data_format )
     {
       case eIpmiAnalogDataFormatUnsigned:
	    lowraw   = 0;
	    highraw  = 255;
	    minraw   = 0;
	    maxraw   = 255;
	    next_raw = 128;
	    break;

       case eIpmiAnalogDataFormat1Compl:
	    lowraw   = -127;
	    highraw  = 127;
	    minraw   = -127;
	    maxraw   = 127;
	    next_raw = 0;
	    break;

       case eIpmiAnalogDataFormat2Compl:
	    lowraw   = -128;
	    highraw  = 127;
	    minraw   = -128;
	    maxraw   = 127;
	    next_raw = 0;
	    break;

       default:
	    return false;
    }

  // We do a binary search for the right value.  Yuck, but I don't
  // have a better plan that will work with non-linear sensors.
  do
     {
       raw = next_raw;
       rv = ConvertFromRaw( raw, cval );

       if ( !rv )
	    return false;

       if ( cval < val )
          {
	    next_raw = ((highraw - raw) / 2) + raw;
	    lowraw = raw;
          } 
       else
          {
	    next_raw = ((raw - lowraw) / 2) + lowraw;
	    highraw = raw;
          }
     }
  while( raw != next_raw );

  // The above loop gets us to within 1 of what it should be, we
  // have to look at rounding to make the final decision.
  switch( rounding )
     {
       case eRoundNormal:
	    if ( val > cval )
               {
                 if ( raw < maxraw )
                    {
                      double nval;

                      rv = ConvertFromRaw( raw + 1, nval );
                      if ( !rv )
                           return false;

                      nval = cval + ((nval - cval) / 2.0);
                      if ( val >= nval )
                           raw++;
                    }
               }
            else
               {
                 if ( raw > minraw )
                    {
                      double pval;
                      rv = ConvertFromRaw( raw-1, pval );
                      if ( !rv )
                           return false;

                      pval = pval + ((cval - pval) / 2.0);
                      if ( val < pval )
                           raw--;
                    }
               }
	    break;

       case eRoundUp:
	    if ((val > cval) && (raw < maxraw))
                 raw++;

	    break;

       case eRoundDown:
	    if ( ( val < cval) && (raw > minraw ) )
                 raw--;

	    break;
     }

  if ( m_analog_data_format == eIpmiAnalogDataFormat1Compl )
       if ( raw < 0 )
	    raw -= 1;

  result = raw & 0xff;
  return true;
}


bool
cIpmiSensorFactors::CreateDataFormat( SaHpiSensorDataFormatT &df )
{
  df.ReadingFormats          = SAHPI_SRF_RAW | SAHPI_SRF_INTERPRETED;
  df.IsNumeric               = SAHPI_TRUE;
  df.SignFormat              = (SaHpiSensorSignFormatT)AnalogDataFormat();
  df.FactorsStatic           = SAHPI_TRUE;
  df.Factors.M_Factor        = (SaHpiInt16T)M();
  df.Factors.B_Factor        = (SaHpiInt16T)B();
  df.Factors.AccuracyFactor  = (SaHpiUint16T)Accuracy();
  df.Factors.ToleranceFactor = (SaHpiUint8T)Tolerance();
  df.Factors.ExpA            = (SaHpiUint8T)AccuracyExp();
  df.Factors.ExpR            = (SaHpiUint8T)RExp();
  df.Factors.ExpB            = (SaHpiUint8T)BExp();
  df.Factors.Linearization   = (SaHpiSensorLinearizationT)Linearization();

  return true;
}
