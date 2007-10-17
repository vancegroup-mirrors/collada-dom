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

#ifndef _TColgp_Array2OfPnt2d_HeaderFile
#define _TColgp_Array2OfPnt2d_HeaderFile

#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
class Standard_RangeError;
class Standard_OutOfRange;
class Standard_OutOfMemory;
class Standard_DimensionMismatch;
class gp_Pnt2d;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif


class TColgp_Array2OfPnt2d  {

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


Standard_EXPORT TColgp_Array2OfPnt2d(const Standard_Integer R1,const Standard_Integer R2,const Standard_Integer C1,const Standard_Integer C2);


Standard_EXPORT TColgp_Array2OfPnt2d(const gp_Pnt2d& Item,const Standard_Integer R1,const Standard_Integer R2,const Standard_Integer C1,const Standard_Integer C2);


Standard_EXPORT   void Init(const gp_Pnt2d& V) ;


Standard_EXPORT   void Destroy() ;
~TColgp_Array2OfPnt2d()
{
  Destroy();
}



Standard_EXPORT  const TColgp_Array2OfPnt2d& Assign(const TColgp_Array2OfPnt2d& Other) ;
 const TColgp_Array2OfPnt2d& operator =(const TColgp_Array2OfPnt2d& Other) 
{
  return Assign(Other);
}


  Standard_Integer ColLength() const;

  Standard_Integer RowLength() const;

  Standard_Integer LowerCol() const;

  Standard_Integer LowerRow() const;

  Standard_Integer UpperCol() const;

  Standard_Integer UpperRow() const;

  void SetValue(const Standard_Integer Row,const Standard_Integer Col,const gp_Pnt2d& Value) ;

 const gp_Pnt2d& Value(const Standard_Integer Row,const Standard_Integer Col) const;
 const gp_Pnt2d& operator()(const Standard_Integer Row,const Standard_Integer Col) const
{
  return Value(Row,Col);
}


  gp_Pnt2d& ChangeValue(const Standard_Integer Row,const Standard_Integer Col) ;
  gp_Pnt2d& operator()(const Standard_Integer Row,const Standard_Integer Col) 
{
  return ChangeValue(Row,Col);
}






protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


Standard_EXPORT TColgp_Array2OfPnt2d(const TColgp_Array2OfPnt2d& AnArray);


Standard_EXPORT   void Allocate() ;


 // Fields PRIVATE
 //
Standard_Integer myLowerRow;
Standard_Integer myLowerColumn;
Standard_Integer myUpperRow;
Standard_Integer myUpperColumn;
Standard_Boolean myDeletable;
Standard_Address myData;


};

#define Array2Item gp_Pnt2d
#define Array2Item_hxx <gp_Pnt2d.hxx>
#define TCollection_Array2 TColgp_Array2OfPnt2d
#define TCollection_Array2_hxx <TColgp_Array2OfPnt2d.hxx>

#include <TCollection_Array2.lxx>

#undef Array2Item
#undef Array2Item_hxx
#undef TCollection_Array2
#undef TCollection_Array2_hxx


// other Inline functions and methods (like "C++: function call" methods)
//


#endif