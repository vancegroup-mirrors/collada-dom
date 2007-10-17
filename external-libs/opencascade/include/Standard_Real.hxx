#ifndef _Standard_Real_HeaderFile
#define _Standard_Real_HeaderFile

#include <float.h>
#include <math.h>
#ifndef _Standard_values_HeaderFile
# include <Standard_values.h>
#endif

#ifdef WNT
# include <Standard_math.hxx>
#endif

#ifndef _Standard_TypeDef_HeaderFile
#include <Standard_TypeDef.hxx>
#endif

class Handle_Standard_Type;

__Standard_API Handle_Standard_Type& Standard_Real_Type_();

//==== In this version "PI" is a global variables.
#ifdef PI
#undef PI
#endif 

#if defined(WNT) && !defined(__CYGWIN32__) && !defined(__MINGW32__)
extern "C" __Standard_API Standard_Real PI;
extern "C" __Standard_API Standard_Real PI180;
extern "C" __Standard_API Standard_Real Standard_PI;
extern "C" __Standard_API Standard_Real Standard_PI180;
#else

extern const Standard_Real PI;
extern const Standard_Real PI180;

extern const Standard_Real Standard_PI;
extern const Standard_Real Standard_PI180;
#endif  /* WNT */
// ===============================================
// Methods from Standard_Entity class which are redefined:  
//    - Hascode
//    - IsEqual
//    - IsSimilar
//    - Shallowcopy
//    - ShallowDump
// ===============================================

// ==================================
// Methods implemeted in Standard_Real.cxx
// ==================================
//....             Herited from Standard_Storable
__Standard_API Standard_Integer HashCode    (const Standard_Real, const Standard_Integer);  
__Standard_API Standard_Real    ShallowCopy (const Standard_Real );  

__Standard_API Standard_Real    ACos        (const Standard_Real );
__Standard_API Standard_Real    ASin        (const Standard_Real );
__Standard_API Standard_Real    ATan2       (const Standard_Real , const Standard_Real );
__Standard_API Standard_Real    NextAfter   (const Standard_Real , const Standard_Real );
__Standard_API Standard_Real    Sign        (const Standard_Real , const Standard_Real );
__Standard_API Standard_Real    ATanh       (const Standard_Real );
__Standard_API Standard_Real    ACosh       (const Standard_Real );
__Standard_API Standard_Real    Log         (const Standard_Real );
__Standard_API Standard_Real    Sqrt        (const Standard_Real );

//class  Standard_OStream;
//void             ShallowDump(const Standard_Real, Standard_OStream& );

//-------------------------------------------------------------------
// RealSmall : Returns the smallest positive real
//-------------------------------------------------------------------
inline Standard_Real     RealSmall() 
{ return DBL_MIN; }

//-------------------------------------------------------------------
// Abs : Returns the absolute value of a real
//-------------------------------------------------------------------
inline Standard_Real     Abs(const Standard_Real Value) 
{ return fabs(Value); }


//-------------------------------------------------------------------
// IsEqual : Returns Standard_True if two reals are equal
//-------------------------------------------------------------------
inline Standard_Boolean  IsEqual (const Standard_Real Value1, 
				  const Standard_Real Value2) 
{ return Abs((Value1 - Value2)) < RealSmall(); }

//-------------------------------------------------------------------
// IsSimilar : Returns Standard_True if two reals are equal
//-------------------------------------------------------------------
inline Standard_Boolean  IsSimilar(const Standard_Real One, 
				  const Standard_Real Two)
{ return IsEqual (One,Two); }



         //  *********************************** //
         //       Class methods                  //
         //                                      //
         //  Machine-dependant values            //
         //  Should be taken from include file   //
         //  *********************************** //


//-------------------------------------------------------------------
// RealDigit : Returns the number of digits of precision in a real
//-------------------------------------------------------------------
inline Standard_Integer  RealDigits() 
{ return DBL_DIG; }

//-------------------------------------------------------------------
// RealEpsilon : Returns the minimum positive real such that 
//               1.0 + x is not equal to 1.0
//-------------------------------------------------------------------
inline Standard_Real     RealEpsilon() 
{ return DBL_EPSILON; }

//-------------------------------------------------------------------
// RealFirst : Returns the minimum negative value of a real
//-------------------------------------------------------------------
inline Standard_Real     RealFirst() 
{Standard_Real aValue = -DBL_MAX; return aValue; }
  
//-------------------------------------------------------------------
// RealFirst10Exp : Returns the minimum value of exponent(base 10) of
//                  a real.
//-------------------------------------------------------------------
inline Standard_Integer  RealFirst10Exp() 
{ return DBL_MIN_10_EXP; }

//-------------------------------------------------------------------
// RealLast : Returns the maximum value of a real
//-------------------------------------------------------------------
inline Standard_Real     RealLast() 
{ return  DBL_MAX; }

//-------------------------------------------------------------------
// RealLast10Exp : Returns the maximum value of exponent(base 10) of
//                 a real.
//-------------------------------------------------------------------
inline Standard_Integer  RealLast10Exp() 
{ return  DBL_MAX_10_EXP; }

//-------------------------------------------------------------------
// RealMantissa : Returns the size in bits of the matissa part of a 
//                real.
//-------------------------------------------------------------------
inline Standard_Integer  RealMantissa() 
{ return  DBL_MANT_DIG; }

//-------------------------------------------------------------------
// RealRadix : Returns the radix of exponent representation
//-------------------------------------------------------------------
inline Standard_Integer  RealRadix() 
{ return  FLT_RADIX; }

//-------------------------------------------------------------------
// RealSize : Returns the size in bits of an integer
//-------------------------------------------------------------------
inline Standard_Integer  RealSize() 
{ return BITS(Standard_Real); }



         //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
         //   End of machine-dependant values   //
         //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//


//-------------------------------------------------------------------
// IntToReal : Converts an integer in a real
//-------------------------------------------------------------------
inline Standard_Real     IntToReal(const Standard_Integer Value) 
{ return Value; }

//-------------------------------------------------------------------
// ATan : Returns the value of the arc tangent of a real
//-------------------------------------------------------------------
inline Standard_Real     ATan(const Standard_Real Value) 
{ return atan(Value); }


//-------------------------------------------------------------------
// Ceiling : Returns the smallest integer not less than a real
//-------------------------------------------------------------------
inline Standard_Real     Ceiling (const Standard_Real Value) 
{ return ceil(Value); }

//-------------------------------------------------------------------
// Cos : Returns the cosine of a real
//-------------------------------------------------------------------
inline Standard_Real     Cos (const Standard_Real Value) 
{ return cos(Value); }

//-------------------------------------------------------------------
// Cosh : Returns the hyperbolic cosine of a real
//-------------------------------------------------------------------
inline Standard_Real     Cosh (const Standard_Real Value) 
{ return cosh(Value); }


//-------------------------------------------------------------------
// Epsilon : Returns a real + the smallest real positive value.
//-------------------------------------------------------------------
inline Standard_Real     Epsilon (const Standard_Real Value) 
{
  Standard_Real aEpsilon;

  if (Value>=0.0){
    aEpsilon = NextAfter(Value, RealLast()) - Value;
  } else {
    aEpsilon = Value - NextAfter(Value, RealFirst());
  }
  return aEpsilon;
}

//-------------------------------------------------------------------
// Exp : Returns the exponential function of a real
//-------------------------------------------------------------------
inline Standard_Real     Exp (const Standard_Real Value) 
{ return exp(Value); }

//-------------------------------------------------------------------
// Floor : Return the largest integer not greater than a real
//-------------------------------------------------------------------
inline Standard_Real     Floor (const Standard_Real Value) 
{ return floor(Value); }

//-------------------------------------------------------------------
// IntegerPart : Returns the integer part of a real
//-------------------------------------------------------------------
inline Standard_Real     IntegerPart (const Standard_Real Value) 
{ return ( (Value>0) ? floor(Value) : ceil(Value) ); }


//-------------------------------------------------------------------
// Log10 : Returns the base-10 logarithm of a real 
//-------------------------------------------------------------------
inline Standard_Real     Log10 (const Standard_Real Value) 
{ return log10(Value); }

//-------------------------------------------------------------------
// Max : Returns the maximum value of two reals
//-------------------------------------------------------------------
inline Standard_Real     Max (const Standard_Real Val1, 
                              const Standard_Real Val2) 
{
  if (Val1 >= Val2) {
    return Val1;
  } else {
    return Val2;
  }
}

//-------------------------------------------------------------------
// Min : Returns the minimum value of two reals
//-------------------------------------------------------------------
inline Standard_Real     Min (const Standard_Real Val1, 
                              const Standard_Real Val2)
{
  if (Val1 <= Val2) {
    return Val1;
  } else {
    return Val2;
  }
}

//-------------------------------------------------------------------
// Pow : Returns a real to a given power
//-------------------------------------------------------------------
inline Standard_Real     Pow (const Standard_Real Value, const Standard_Real P)
{ return pow(Value,P); }

//-------------------------------------------------------------------
// RealPart : Returns the fractional part of a real.
//-------------------------------------------------------------------
inline  Standard_Real    RealPart (const Standard_Real Value) 
{ return fabs(IntegerPart(Value) - Value); }

//-------------------------------------------------------------------
// RealToInt : Returns the integer part of a real (in an integer)
//-------------------------------------------------------------------
inline  Standard_Integer RealToInt (const Standard_Real Value) 
{ 
//#if defined(WNT) || defined(LIN)
return ( (Value>0) ? (Standard_Integer)floor(Value) : (Standard_Integer)ceil(Value) );
//#else
//return ( (Value>0) ? floor(Value) : ceil(Value) ); 
//#endif
}

//-------------------------------------------------------------------
// Round : Returns the nearest integer of a real
//-------------------------------------------------------------------
inline Standard_Real     Round (const Standard_Real Value) 
{ return IntegerPart(Value + (Value > 0 ? 0.5 : -0.5)); }

//-------------------------------------------------------------------
// Sin : Returns the sine of a real
//-------------------------------------------------------------------
inline Standard_Real     Sin (const Standard_Real Value) 
{ return sin(Value); }

//-------------------------------------------------------------------
// Sinh : Returns the hyperbolic sine of a real
//-------------------------------------------------------------------
inline Standard_Real     Sinh(const Standard_Real Value) 
{ return sinh(Value); }

//-------------------------------------------------------------------
// ASinh : Returns the hyperbolic arc sine of a real
//-------------------------------------------------------------------
inline Standard_Real     ASinh(const Standard_Real Value) 
{ return asinh(Value); }

//-------------------------------------------------------------------
// Square : Returns a real to the power 2
//-------------------------------------------------------------------
inline Standard_Real     Square(const Standard_Real Value) 
{ return Value * Value; }

//-------------------------------------------------------------------
// Tan : Returns the tangent of a real
//-------------------------------------------------------------------
inline Standard_Real     Tan (const Standard_Real Value) 
{ return tan(Value); }

//-------------------------------------------------------------------
// Tanh : Returns the hyperbolic tangent of a real
//-------------------------------------------------------------------
inline Standard_Real     Tanh (const Standard_Real Value) 
{ return tanh(Value); }

#endif