// File generated by CPPExt (Enum)
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

#ifndef _BRepBuilderAPI_WireError_HeaderFile
#define _BRepBuilderAPI_WireError_HeaderFile

//! Indicates the outcome of wire <br>
//! construction, i.e. whether it is successful or not, as explained below: <br>
//! -      BRepBuilderAPI_WireDone No <br>
//!    error occurred. The wire is correctly built. <br>
//! -      BRepBuilderAPI_EmptyWire No <br>
//! initialization of the algorithm. Only an empty constructor was used. <br>
//! -      BRepBuilderAPI_DisconnectedWire <br>
//! The last edge which you attempted to add was not connected to the wire. <br>
//! -      BRepBuilderAPI_NonManifoldWire <br>
//!    The wire with some singularity. <br>
enum BRepBuilderAPI_WireError { 
 BRepBuilderAPI_WireDone,
BRepBuilderAPI_EmptyWire,
BRepBuilderAPI_DisconnectedWire,
BRepBuilderAPI_NonManifoldWire
};


#ifndef _Standard_PrimitiveTypes_HeaderFile
#include <Standard_PrimitiveTypes.hxx>
#endif

#endif