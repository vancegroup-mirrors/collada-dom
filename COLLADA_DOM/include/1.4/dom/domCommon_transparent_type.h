/*
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the SCEA Shared Source License, Version 1.0 (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the License at:
 * http://research.scea.com/scea_shared_source_license.html
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing permissions and limitations under the
 * License.
 */

#ifndef __domCommon_transparent_type_h__
#define __domCommon_transparent_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCommon_color_or_texture_type.h>
class DAE;

class domCommon_transparent_type_complexType : public domCommon_color_or_texture_type_complexType
{
protected:  // Attribute
	domFx_opaque_enum attrOpaque;


public:	//Accessors and Mutators
	/**
	 * Gets the opaque attribute.
	 * @return Returns a domFx_opaque_enum of the opaque attribute.
	 */
	domFx_opaque_enum getOpaque() const { return attrOpaque; }
	/**
	 * Sets the opaque attribute.
	 * @param atOpaque The new value for the opaque attribute.
	 */
	void setOpaque( domFx_opaque_enum atOpaque ) { attrOpaque = atOpaque; }

protected:
	/**
	 * Constructor
	 */
	domCommon_transparent_type_complexType(DAE& dae) : attrOpaque() {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_transparent_type_complexType() {}
	/**
	 * Copy Constructor
	 */
	domCommon_transparent_type_complexType( const domCommon_transparent_type_complexType &cpy ) : domCommon_color_or_texture_type_complexType() { (void)cpy; }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_transparent_type_complexType &operator=( const domCommon_transparent_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCommon_transparent_type_complexType.
 */
class domCommon_transparent_type : public daeElement, public domCommon_transparent_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COMMON_TRANSPARENT_TYPE; }
	static COLLADA_TYPE::TypeEnum getTypeStatic() const { return COLLADA_TYPE::COMMON_TRANSPARENT_TYPE; }

public:	//Accessors and Mutators
	/**
	 * Gets the opaque attribute.
	 * @return Returns a domFx_opaque_enum of the opaque attribute.
	 */
	domFx_opaque_enum getOpaque() const { return attrOpaque; }
	/**
	 * Sets the opaque attribute.
	 * @param atOpaque The new value for the opaque attribute.
	 */
	void setOpaque( domFx_opaque_enum atOpaque ) { attrOpaque = atOpaque; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domCommon_transparent_type(DAE& dae) {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_transparent_type() {}
	/**
	 * Copy Constructor
	 */
	domCommon_transparent_type( const domCommon_transparent_type &cpy ) : daeElement(), domCommon_transparent_type_complexType() { (void)cpy; }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_transparent_type &operator=( const domCommon_transparent_type &cpy ) { (void)cpy; return *this; }

public: // STATIC METHODS
	/**
	 * Creates an instance of this class and returns a daeElementRef referencing it.
	 * @return a daeElementRef referencing an instance of this object.
	 */
	static DLLSPEC daeElementRef create();
	/**
	 * Creates a daeMetaElement object that describes this element in the meta object reflection framework.
	 * If a daeMetaElement already exists it will return that instead of creating a new one. 
	 * @return A daeMetaElement describing this COLLADA element.
	 */
	static DLLSPEC daeMetaElement* registerElement(DAE& dae);
};


#endif
