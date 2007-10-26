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

#ifndef _Handle_Geom_OffsetCurve_HeaderFile
#define _Handle_Geom_OffsetCurve_HeaderFile

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
class Geom_OffsetCurve;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(Geom_OffsetCurve);

class Handle(Geom_OffsetCurve) : public Handle(Geom_Curve) {
  public:
    Handle(Geom_OffsetCurve)():Handle(Geom_Curve)() {} 
    Handle(Geom_OffsetCurve)(const Handle(Geom_OffsetCurve)& aHandle) : Handle(Geom_Curve)(aHandle) 
     {
     }

    Handle(Geom_OffsetCurve)(const Geom_OffsetCurve* anItem) : Handle(Geom_Curve)((Geom_Curve *)anItem) 
     {
     }

    Handle(Geom_OffsetCurve)& operator=(const Handle(Geom_OffsetCurve)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(Geom_OffsetCurve)& operator=(const Geom_OffsetCurve* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    Geom_OffsetCurve* operator->() const
     {
      return (Geom_OffsetCurve *)ControlAccess();
     }

//   Standard_EXPORT ~Handle(Geom_OffsetCurve)();
 
   Standard_EXPORT static const Handle(Geom_OffsetCurve) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif