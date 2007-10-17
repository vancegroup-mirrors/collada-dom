// File generated by CPPExt (Transient)
//
//
//                     Copyright (C) 1991 - 2000 by  
//                      Matra Datavision SA.  All rights reserved.
//  
//                     Copyright (C) 2001 - 2004 by
//                     Open CASCADE SA.  All rights reserved.
// 
// This file is part of the Open CASCADE Technology software.
//
// This software may be distributed and/or modified under the terms and
// conditions of the Open CASCADE Public License as defined by Open CASCADE SA
// and appearing in the file LICENSE included in the packaging of this file.
//  
// This software is distributed on an "AS IS" basis, without warranty of any
// kind, and Open CASCADE SA hereby disclaims all such warranties,
// including without limitation, any warranties of merchantability, fitness
// for a particular purpose or non-infringement. Please see the License for
// the specific terms and conditions governing rights and limitations under the
// License.

#ifndef _Geom_Plane_HeaderFile
#define _Geom_Plane_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_Geom_Plane_HeaderFile
#include <Handle_Geom_Plane.hxx>
#endif

#ifndef _Geom_ElementarySurface_HeaderFile
#include <Geom_ElementarySurface.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_Geom_Curve_HeaderFile
#include <Handle_Geom_Curve.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_Geom_Geometry_HeaderFile
#include <Handle_Geom_Geometry.hxx>
#endif
class Standard_ConstructionError;
class Standard_RangeError;
class gp_Ax3;
class gp_Pln;
class gp_Pnt;
class gp_Dir;
class gp_Trsf;
class gp_GTrsf2d;
class Geom_Curve;
class gp_Vec;
class Geom_Geometry;


//! Describes a plane in 3D space. <br>
//! A plane is positioned in space by a coordinate system <br>
//! (a gp_Ax3 object) such that the plane is defined by <br>
//! the origin, "X Direction" and "Y Direction" of this <br>
//! coordinate system. <br>
//! This coordinate system is the "local coordinate <br>
//! system" of the plane. The following apply: <br>
//! - Its "X Direction" and "Y Direction" are respectively <br>
//!   the u and v parametric directions of the plane. <br>
//! - Its origin is the origin of the u and v parameters <br>
//!   (also called the "origin" of the plane). <br>
//! - Its "main Direction" is a vector normal to the plane. <br>
//!   This normal vector gives the orientation of the <br>
//!   plane only if the local coordinate system is "direct". <br>
//!   (The orientation of the plane is always defined by <br>
//!   the "X Direction" and the "Y Direction" of its local <br>
//!   coordinate system.) <br>
//!   The parametric equation of the plane is: <br>
//! P(u, v) = O + u*XDir + v*YDir <br>
//! where O, XDir and YDir are respectively the <br>
//! origin, the "X Direction" and the "Y Direction" of the <br>
//! local coordinate system of the plane. <br>
//! The parametric range of the two parameters u and v <br>
//! is ] -infinity, +infinity [. <br>
class Geom_Plane : public Geom_ElementarySurface {

public:
 // Methods PUBLIC
 // 


//!  Creates a plane located in 3D space with an axis placement <br>
//!  three axis.  The "ZDirection" of "A3" is the direction normal <br>
//!  to the plane.  The "Location" point of "A3" is the origin of <br>
//!  the plane. The "XDirection" and "YDirection" of "A3" define <br>
//!  the directions of the U isoparametric and V isoparametric <br>
//!  curves. <br>
Standard_EXPORT Geom_Plane(const gp_Ax3& A3);


//!  Creates a plane from a non transient plane from package gp. <br>
Standard_EXPORT Geom_Plane(const gp_Pln& Pl);


//!  P is the "Location" point or origin of the plane. <br>
//!  V is the direction normal to the plane. <br>
Standard_EXPORT Geom_Plane(const gp_Pnt& P,const gp_Dir& V);


//!  Creates a plane from its cartesian equation : <br>
//!  Ax + By + Cz + D = 0.0 <br>
//!  Raised if Sqrt (A*A + B*B + C*C) <= Resolution from gp <br>
Standard_EXPORT Geom_Plane(const Standard_Real A,const Standard_Real B,const Standard_Real C,const Standard_Real D);


//!  Set <me> so that <me> has the same geometric properties as Pl. <br>
Standard_EXPORT   void SetPln(const gp_Pln& Pl) ;

//! Converts this plane into a gp_Pln plane. <br>
Standard_EXPORT   gp_Pln Pln() const;


//! Changes the orientation of this plane in the u (or v) <br>
//! parametric direction. The bounds of the plane are not <br>
//! changed but the given parametric direction is <br>
//! reversed. Hence the orientation of the surface is reversed. <br>
Standard_EXPORT virtual  void UReverse() ;

//! Computes the u  parameter on the modified <br>
//! plane, produced when reversing the u <br>
//! parametric of this plane, for any point of u parameter U  on this plane. <br>
//! In the case of a plane, these methods return - -U. <br>
Standard_EXPORT   Standard_Real UReversedParameter(const Standard_Real U) const;


//! Changes the orientation of this plane in the u (or v) <br>
//! parametric direction. The bounds of the plane are not <br>
//! changed but the given parametric direction is <br>
//! reversed. Hence the orientation of the surface is reversed. <br>
Standard_EXPORT virtual  void VReverse() ;

//! Computes the v parameter on the modified <br>
//! plane, produced when reversing the  v <br>
//! parametric of this plane, for any point of v parameter V on this plane. <br>
//! In the case of a plane, these methods return -V. <br>
Standard_EXPORT   Standard_Real VReversedParameter(const Standard_Real V) const;

//! Computes the  parameters on the  transformed  surface for <br>
//!          the transform of the point of parameters U,V on <me>. <br>
//!          me->Transformed(T)->Value(U',V') <br>
//!          is the same point as <br>
//!          me->Value(U,V).Transformed(T) <br>
//!          Where U',V' are the new values of U,V after calling <br>
//!          me->TranformParameters(U,V,T) <br>
//!          This methods multiplies U and V by T.ScaleFactor() <br>
Standard_EXPORT virtual  void TransformParameters(Standard_Real& U,Standard_Real& V,const gp_Trsf& T) const;

//! Returns a 2d transformation  used to find the  new <br>
//!          parameters of a point on the transformed surface. <br>
//!          me->Transformed(T)->Value(U',V') <br>
//!          is the same point as <br>
//!          me->Value(U,V).Transformed(T) <br>
//!          Where U',V' are  obtained by transforming U,V with <br>
//!          th 2d transformation returned by <br>
//!          me->ParametricTransformation(T) <br>
//!          This  methods  returns  a scale  centered  on  the <br>
//!          origin with T.ScaleFactor <br>
Standard_EXPORT virtual  gp_GTrsf2d ParametricTransformation(const gp_Trsf& T) const;

//! Returns the parametric bounds U1, U2, V1 and V2 of this plane. <br>
//! Because a plane is an infinite surface, the following is always true: <br>
//! - U1 = V1 =   Standard_Real::RealFirst() <br>
//! - U2 = V2 =   Standard_Real::RealLast(). <br>
Standard_EXPORT   void Bounds(Standard_Real& U1,Standard_Real& U2,Standard_Real& V1,Standard_Real& V2) const;


//!  Computes the normalized coefficients of the plane's <br>
//!  cartesian equation : Ax + By + Cz + D = 0.0 <br>
Standard_EXPORT   void Coefficients(Standard_Real& A,Standard_Real& B,Standard_Real& C,Standard_Real& D) const;

//! return False <br>
Standard_EXPORT   Standard_Boolean IsUClosed() const;

//! return False <br>
Standard_EXPORT   Standard_Boolean IsVClosed() const;

//! return False. <br>
Standard_EXPORT   Standard_Boolean IsUPeriodic() const;

//! return False. <br>
Standard_EXPORT   Standard_Boolean IsVPeriodic() const;


//!  Computes the U isoparametric curve. <br>
//!  This is a Line parallel to the YAxis of the plane. <br>
Standard_EXPORT   Handle_Geom_Curve UIso(const Standard_Real U) const;


//!  Computes the V isoparametric curve. <br>
//!  This is a Line parallel to the XAxis of the plane. <br>
Standard_EXPORT   Handle_Geom_Curve VIso(const Standard_Real V) const;


//!  Computes the point P (U, V) on <me>. <br>
//!  P = O + U * XDir + V * YDir. <br>
//!  where O is the "Location" point of the plane, XDir the <br>
//!  "XDirection" and YDir the "YDirection" of the plane's local <br>
//!  coordinate system. <br>
Standard_EXPORT   void D0(const Standard_Real U,const Standard_Real V,gp_Pnt& P) const;


//!  Computes the current point and the first derivatives in the <br>
//!  directions U and V. <br>
Standard_EXPORT   void D1(const Standard_Real U,const Standard_Real V,gp_Pnt& P,gp_Vec& D1U,gp_Vec& D1V) const;


//!  Computes the current point, the first and the second <br>
//!  derivatives in the directions U and V. <br>
Standard_EXPORT   void D2(const Standard_Real U,const Standard_Real V,gp_Pnt& P,gp_Vec& D1U,gp_Vec& D1V,gp_Vec& D2U,gp_Vec& D2V,gp_Vec& D2UV) const;


//!  Computes the current point, the first,the second and the <br>
//!  third derivatives in the directions U and V. <br>
Standard_EXPORT   void D3(const Standard_Real U,const Standard_Real V,gp_Pnt& P,gp_Vec& D1U,gp_Vec& D1V,gp_Vec& D2U,gp_Vec& D2V,gp_Vec& D2UV,gp_Vec& D3U,gp_Vec& D3V,gp_Vec& D3UUV,gp_Vec& D3UVV) const;


//!  Computes the derivative of order Nu in the direction u <br>
//!  and Nv in the direction v. <br>//! Raised if Nu + Nv < 1 or Nu < 0 or Nv < 0. <br>
Standard_EXPORT   gp_Vec DN(const Standard_Real U,const Standard_Real V,const Standard_Integer Nu,const Standard_Integer Nv) const;

//! Applies the transformation T to this plane. <br>
Standard_EXPORT   void Transform(const gp_Trsf& T) ;

//! Creates a new object which is a copy of this plane. <br>
Standard_EXPORT   Handle_Geom_Geometry Copy() const;
//Standard_EXPORT ~Geom_Plane();




 // Type management
 //
 Standard_EXPORT const Handle(Standard_Type)& DynamicType() const;
 //Standard_EXPORT Standard_Boolean	       IsKind(const Handle(Standard_Type)&) const;

protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif