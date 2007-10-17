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

#ifndef _Handle_TopLoc_Datum3D_HeaderFile
#define _Handle_TopLoc_Datum3D_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_MMgt_TShared_HeaderFile
#include <Handle_MMgt_TShared.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(MMgt_TShared);
class TopLoc_Datum3D;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(TopLoc_Datum3D);

class Handle(TopLoc_Datum3D) : public Handle(MMgt_TShared) {
  public:
    Handle(TopLoc_Datum3D)():Handle(MMgt_TShared)() {} 
    Handle(TopLoc_Datum3D)(const Handle(TopLoc_Datum3D)& aHandle) : Handle(MMgt_TShared)(aHandle) 
     {
     }

    Handle(TopLoc_Datum3D)(const TopLoc_Datum3D* anItem) : Handle(MMgt_TShared)((MMgt_TShared *)anItem) 
     {
     }

    Handle(TopLoc_Datum3D)& operator=(const Handle(TopLoc_Datum3D)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(TopLoc_Datum3D)& operator=(const TopLoc_Datum3D* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    TopLoc_Datum3D* operator->() const
     {
      return (TopLoc_Datum3D *)ControlAccess();
     }

//   Standard_EXPORT ~Handle(TopLoc_Datum3D)();
 
   Standard_EXPORT static const Handle(TopLoc_Datum3D) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif