// File generated by CPPExt (Value)
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

#ifndef _gp_HeaderFile
#define _gp_HeaderFile

#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
class gp_Pnt;
class gp_Dir;
class gp_Ax1;
class gp_Ax2;
class gp_Pnt2d;
class gp_Dir2d;
class gp_Ax2d;
class gp_XYZ;
class gp_Mat;
class gp_Trsf;
class gp_GTrsf;
class gp_Pnt;
class gp_Vec;
class gp_Dir;
class gp_Ax1;
class gp_Ax2;
class gp_Ax3;
class gp_Lin;
class gp_Circ;
class gp_Elips;
class gp_Hypr;
class gp_Parab;
class gp_Pln;
class gp_Cylinder;
class gp_Sphere;
class gp_Torus;
class gp_Cone;
class gp_XY;
class gp_Mat2d;
class gp_Trsf2d;
class gp_GTrsf2d;
class gp_Pnt2d;
class gp_Vec2d;
class gp_Dir2d;
class gp_Ax2d;
class gp_Ax22d;
class gp_Lin2d;
class gp_Circ2d;
class gp_Elips2d;
class gp_Hypr2d;
class gp_Parab2d;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif


//!  The geometric processor package, called gp, provides an <br>
//!  implementation of entities used  : <br>
//!  . for algebraic calculation such as "XYZ" coordinates, "Mat" <br>
//!    matrix <br>
//!  . for basis analytic geometry such as Transformations, point, <br>
//!    vector, line, plane, axis placement, conics, and elementary <br>
//!    surfaces. <br>
//!  These entities are defined in 2d and 3d space. <br>
//!  All the classes of this package are non-persistent. <br>
class gp  {

public:

    void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
 // Methods PUBLIC
 // 

//!  In geometric computations, defines the tolerance criterion <br>
//! used to determine when two numbers can be considered equal. <br>
//! Many class functions use this tolerance criterion, for <br>
//! example, to avoid division by zero in geometric <br>
//! computations. In the documentation, tolerance criterion is <br>
//! always referred to as gp::Resolution(). <br>
static  Standard_Real Resolution() ;

//! Identifies a Cartesian point with coordinates X = Y = Z = 0.0.0 <br>
Standard_EXPORT static const gp_Pnt& Origin() ;

//! Returns a unit vector with the combination (1,0,0) <br>
Standard_EXPORT static const gp_Dir& DX() ;

//! Returns a unit vector with the combination (0,1,0) <br>
Standard_EXPORT static const gp_Dir& DY() ;

//! Returns a unit vector with the combination (0,0,1) <br>
Standard_EXPORT static const gp_Dir& DZ() ;

//!Identifies an axis where its origin is Origin <br>
//! and its unit vector coordinates  X = 1.0,  Y = Z = 0.0 <br>
Standard_EXPORT static const gp_Ax1& OX() ;

//!Identifies an axis where its origin is Origin <br>
//! and its unit vector coordinates Y = 1.0,  X = Z = 0.0 <br>
Standard_EXPORT static const gp_Ax1& OY() ;

//!Identifies an axis where its origin is Origin <br>
//! and its unit vector coordinates Z = 1.0,  Y = X = 0.0 <br>
Standard_EXPORT static const gp_Ax1& OZ() ;

//!Identifies a coordinate system where its origin is Origin, <br>
//! and its "main Direction" and "X Direction" coordinates <br>
//!  Z = 1.0, X = Y =0.0 and X direction coordinates X = 1.0, Y = Z = 0.0 <br>
Standard_EXPORT static const gp_Ax2& XOY() ;

//!Identifies a coordinate system where its origin is Origin, <br>
//! and its "main Direction" and "X Direction" coordinates <br>
//!  Y = 1.0, X = Z =0.0 and X direction coordinates Z = 1.0, X = Y = 0.0 <br>
Standard_EXPORT static const gp_Ax2& ZOX() ;

//!Identifies a coordinate system where its origin is Origin, <br>
//! and its "main Direction" and "X Direction" coordinates <br>
//!  X = 1.0, Z = Y =0.0 and X direction coordinates Y = 1.0, X = Z = 0.0 <br>//! In 2D space <br>
Standard_EXPORT static const gp_Ax2& YOZ() ;

//! Identifies a Cartesian point with coordinates X = Y = 0.0 <br>
Standard_EXPORT static const gp_Pnt2d& Origin2d() ;

//! Returns a unit vector with the combinations (1,0) <br>
Standard_EXPORT static const gp_Dir2d& DX2d() ;

//! Returns a unit vector with the combinations (0,1) <br>
Standard_EXPORT static const gp_Dir2d& DY2d() ;

//! Identifies an axis where its origin is Origin2d <br>
//! and its unit vector coordinates are: X = 1.0,  Y = 0.0 <br>
Standard_EXPORT static const gp_Ax2d& OX2d() ;

//!  Identifies an axis where its origin is Origin2d <br>
//! and its unit vector coordinates are Y = 1.0,  X = 0.0 <br>
Standard_EXPORT static const gp_Ax2d& OY2d() ;





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

friend class gp_XYZ;
friend class gp_Mat;
friend class gp_Trsf;
friend class gp_GTrsf;
friend class gp_Pnt;
friend class gp_Vec;
friend class gp_Dir;
friend class gp_Ax1;
friend class gp_Ax2;
friend class gp_Ax3;
friend class gp_Lin;
friend class gp_Circ;
friend class gp_Elips;
friend class gp_Hypr;
friend class gp_Parab;
friend class gp_Pln;
friend class gp_Cylinder;
friend class gp_Sphere;
friend class gp_Torus;
friend class gp_Cone;
friend class gp_XY;
friend class gp_Mat2d;
friend class gp_Trsf2d;
friend class gp_GTrsf2d;
friend class gp_Pnt2d;
friend class gp_Vec2d;
friend class gp_Dir2d;
friend class gp_Ax2d;
friend class gp_Ax22d;
friend class gp_Lin2d;
friend class gp_Circ2d;
friend class gp_Elips2d;
friend class gp_Hypr2d;
friend class gp_Parab2d;

};


#include <gp.lxx>



// other Inline functions and methods (like "C++: function call" methods)
//


#endif