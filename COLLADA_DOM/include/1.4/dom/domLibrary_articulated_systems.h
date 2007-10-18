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
#ifndef __domLibrary_articulated_systems_h__
#define __domLibrary_articulated_systems_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domArticulated_system.h>
#include <dom/domExtra.h>

class domLibrary_articulated_systems : public daeElement
{
public:
	COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LIBRARY_ARTICULATED_SYSTEMS; }
protected:  // Attributes
	xsID attrId;
	xsNCName attrName;

protected:  // Elements
	domAssetRef elemAsset;
	domArticulated_system_Array elemArticulated_system_array;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the id attribute.
	 * @return Returns a xsID of the id attribute.
	 */
	xsID getId() const { return attrId; }
	/**
	 * Sets the id attribute.
	 * @param atId The new value for the id attribute.
	 */
	void setId( xsID atId ) { *(daeStringRef*)&attrId = atId; _validAttributeArray[0] = true; 
		if( _document != NULL ) _document->changeElementID( this, attrId );
	}

	/**
	 * Gets the name attribute.
	 * @return Returns a xsNCName of the name attribute.
	 */
	xsNCName getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsNCName atName ) { *(daeStringRef*)&attrName = atName; _validAttributeArray[1] = true; }

	/**
	 * Gets the asset element.
	 * @return a daeSmartRef to the asset element.
	 */
	const domAssetRef getAsset() const { return elemAsset; }
	/**
	 * Gets the articulated_system element array.
	 * @return Returns a reference to the array of articulated_system elements.
	 */
	domArticulated_system_Array &getArticulated_system_array() { return elemArticulated_system_array; }
	/**
	 * Gets the articulated_system element array.
	 * @return Returns a constant reference to the array of articulated_system elements.
	 */
	const domArticulated_system_Array &getArticulated_system_array() const { return elemArticulated_system_array; }
	/**
	 * Gets the extra element array.
	 * @return Returns a reference to the array of extra elements.
	 */
	domExtra_Array &getExtra_array() { return elemExtra_array; }
	/**
	 * Gets the extra element array.
	 * @return Returns a constant reference to the array of extra elements.
	 */
	const domExtra_Array &getExtra_array() const { return elemExtra_array; }
protected:
	/**
	 * Constructor
	 */
	domLibrary_articulated_systems() : attrId(), attrName(), elemAsset(), elemArticulated_system_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domLibrary_articulated_systems() {}
	/**
	 * Copy Constructor
	 */
	domLibrary_articulated_systems( const domLibrary_articulated_systems &cpy ) : daeElement() { (void)cpy; }
	/**
	 * Overloaded assignment operator
	 */
	virtual domLibrary_articulated_systems &operator=( const domLibrary_articulated_systems &cpy ) { (void)cpy; return *this; }

public: // STATIC METHODS
	/**
	 * Creates an instance of this class and returns a daeElementRef referencing it.
	 * @param bytes The size allocated for this instance.
	 * @return a daeElementRef referencing an instance of this object.
	 */
	static DLLSPEC daeElementRef create(daeInt bytes);
	/**
	 * Creates a daeMetaElement object that describes this element in the meta object reflection framework.
	 * If a daeMetaElement already exists it will return that instead of creating a new one. 
	 * @return A daeMetaElement describing this COLLADA element.
	 */
	static DLLSPEC daeMetaElement* registerElement();

public: // STATIC MEMBERS
	/**
	 * The daeMetaElement that describes this element in the meta object reflection framework.
	 */
	static DLLSPEC daeMetaElement* _Meta;
};


#endif