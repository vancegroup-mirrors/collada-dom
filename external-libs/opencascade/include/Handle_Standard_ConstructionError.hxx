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

#ifndef _Handle_Standard_ConstructionError_HeaderFile
#define _Handle_Standard_ConstructionError_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_Standard_DomainError_HeaderFile
#include <Handle_Standard_DomainError.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(Standard_DomainError);
class Standard_ConstructionError;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(Standard_ConstructionError);

class Handle(Standard_ConstructionError) : public Handle(Standard_DomainError) {
  public:
    Handle(Standard_ConstructionError)():Handle(Standard_DomainError)() {} 
    Handle(Standard_ConstructionError)(const Handle(Standard_ConstructionError)& aHandle) : Handle(Standard_DomainError)(aHandle) 
     {
     }

    Handle(Standard_ConstructionError)(const Standard_ConstructionError* anItem) : Handle(Standard_DomainError)((Standard_DomainError *)anItem) 
     {
     }

    Handle(Standard_ConstructionError)& operator=(const Handle(Standard_ConstructionError)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(Standard_ConstructionError)& operator=(const Standard_ConstructionError* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    Standard_ConstructionError* operator->() const
     {
      return (Standard_ConstructionError *)ControlAccess();
     }

//   Standard_EXPORT ~Handle(Standard_ConstructionError)();
 
   Standard_EXPORT static const Handle(Standard_ConstructionError) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif