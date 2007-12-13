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
#ifndef __domJoint_h__
#define __domJoint_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
#include <dom/domJointType.h>

class domJoint : public daeElement
{
public:
	COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::JOINT; }
protected:  // Attributes
	xsID attrId;
	xsNCName attrName;
	xsNCName attrSid;

protected:  // Elements
	domJointType_Array elemPrismatic_array;
	domJointType_Array elemRevolute_array;
	domJointType_Array elemScalable_array;
	domExtra_Array elemExtra_array;
	/**
	 * Used to preserve order in elements that do not specify strict sequencing of sub-elements.
	 */
	daeElementRefArray _contents;
	/**
	 * Used to preserve order in elements that have a complex content model.
	 */
	daeUIntArray       _contentsOrder;

	/**
	 * Used to store information needed for some content model objects.
	 */
	daeTArray< daeCharArray * > _CMData;


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
	 * Gets the sid attribute.
	 * @return Returns a xsNCName of the sid attribute.
	 */
	xsNCName getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( xsNCName atSid ) { *(daeStringRef*)&attrSid = atSid; _validAttributeArray[2] = true; }

	/**
	 * Gets the prismatic element array.
	 * @return Returns a reference to the array of prismatic elements.
	 */
	domJointType_Array &getPrismatic_array() { return elemPrismatic_array; }
	/**
	 * Gets the prismatic element array.
	 * @return Returns a constant reference to the array of prismatic elements.
	 */
	const domJointType_Array &getPrismatic_array() const { return elemPrismatic_array; }
	/**
	 * Gets the revolute element array.
	 * @return Returns a reference to the array of revolute elements.
	 */
	domJointType_Array &getRevolute_array() { return elemRevolute_array; }
	/**
	 * Gets the revolute element array.
	 * @return Returns a constant reference to the array of revolute elements.
	 */
	const domJointType_Array &getRevolute_array() const { return elemRevolute_array; }
	/**
	 * Gets the scalable element array.
	 * @return Returns a reference to the array of scalable elements.
	 */
	domJointType_Array &getScalable_array() { return elemScalable_array; }
	/**
	 * Gets the scalable element array.
	 * @return Returns a constant reference to the array of scalable elements.
	 */
	const domJointType_Array &getScalable_array() const { return elemScalable_array; }
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
	/**
	 * Gets the _contents array.
	 * @return Returns a reference to the _contents element array.
	 */
	daeElementRefArray &getContents() { return _contents; }
	/**
	 * Gets the _contents array.
	 * @return Returns a constant reference to the _contents element array.
	 */
	const daeElementRefArray &getContents() const { return _contents; }

protected:
	/**
	 * Constructor
	 */
	domJoint() : attrId(), attrName(), attrSid(), elemPrismatic_array(), elemRevolute_array(), elemScalable_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domJoint() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Copy Constructor
	 */
	domJoint( const domJoint &cpy ) : daeElement() { (void)cpy; }
	/**
	 * Overloaded assignment operator
	 */
	virtual domJoint &operator=( const domJoint &cpy ) { (void)cpy; return *this; }

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
