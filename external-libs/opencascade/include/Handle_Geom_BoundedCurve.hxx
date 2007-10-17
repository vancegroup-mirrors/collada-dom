// File generated by CPPExt (Transient)
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

#ifndef _Handle_Geom_BoundedCurve_HeaderFile
#define _Handle_Geom_BoundedCurve_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_Geom_Curve_HeaderFile
#include <Handle_Geom_Curve.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(Geom_Curve);
class Geom_BoundedCurve;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(Geom_BoundedCurve);

class Handle(Geom_BoundedCurve) : public Handle(Geom_Curve) {
  public:
    Handle(Geom_BoundedCurve)():Handle(Geom_Curve)() {} 
    Handle(Geom_BoundedCurve)(const Handle(Geom_BoundedCurve)& aHandle) : Handle(Geom_Curve)(aHandle) 
     {
     }

    Handle(Geom_BoundedCurve)(const Geom_BoundedCurve* anItem) : Handle(Geom_Curve)((Geom_Curve *)anItem) 
     {
     }

    Handle(Geom_BoundedCurve)& operator=(const Handle(Geom_BoundedCurve)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(Geom_BoundedCurve)& operator=(const Geom_BoundedCurve* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    Geom_BoundedCurve* operator->() const
     {
      return (Geom_BoundedCurve *)ControlAccess();
     }

//   Standard_EXPORT ~Handle(Geom_BoundedCurve)();
 
   Standard_EXPORT static const Handle(Geom_BoundedCurve) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif