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

#ifndef __DAE_IDREF_H__
#define __DAE_IDREF_H__

#include <string>
#include <dae/daeTypes.h>
#include <dae/daeElement.h>
class DAE;

/**
 * The @c daeIDRef is a simple class designed to aid in the parsing and resolution of
 * ID references inside of COLLADA elements.
 * A @c daeIDRef is created for every IDREF data type in the COLLADA schema.
 * It also has the capability to attempt to resolve this reference
 * into a @c daeElement.  If a @c daeIDRef is stored within a @c daeElement it fills
 * in its container field to point to the containing element.
 *
 * The main API is the @c daeIDRef::resolveElement() will use a @c daeIDRefResolver
 * to search for the @c daeElement inside of a @c daeDatabase.
 *
 */
class DLLSPEC daeIDRef
{
public:
	/**
	 * An enum describing the status of the ID resolution process.
	 */
	enum ResolveState{
		/** No ID specified */
		id_empty,
		/** ID specified but not resolved */
		id_loaded,
		/** ID resolution pending */
		id_pending,
		/** ID resolved correctly */
		id_success,
		/** Resolution failed because ID was not found */
		id_failed_id_not_found,
		/** Resolution failed because ID was invalid */
		id_failed_invalid_id,
		/** Resoltion failed due to invalid reference */
		id_failed_invalid_reference,
		/** Resolution failed due to an external error */
		id_failed_externalization,
		/** Resolution failed because we don't have a document in which to search for the element.
				This means you probably forgot to set a container element. */
		id_failed_no_document
	};
	
private:	
	/** ID used to refer to another element */
	std::string id;

	/** Reference to the actual element the ID refers to */
	mutable daeElementRef element;

	/** Element that owns this ID (if any) */
	daeElement* container;

public:
	/**
	 * Simple Constructor
	 */
	daeIDRef();

	/**
	 * Constructs an id reference via a string, using @c setID(); loads the status.
	 * @param id ID to construct a reference for, passed to @c setID() automatically.
	 */
	daeIDRef(daeString id);
	
	/**
	 * Constructs a new id reference by copying an existing one. 
	 * @param constructFromIDRef @c daeIDRef to copy into this one.
	 */
	daeIDRef(const daeIDRef& constructFromIDRef);

	/**
	 * Gets the ID string
	 * @return Returns the full ID string from <tt><i>id.</i></tt> 
	 */
	daeString getID() const;

	/**
	 * Copies <tt><i>ID</i></tt> into the  <tt><i>id	</i></tt> data member.
	 * After the call to @c setID(), the <tt><i>state</i></tt> is set to @c id_loaded
	 * @param ID String to use to configure this @c daeIDRef.
	 */
	void setID(daeString ID);

	/** 
	 * Gets the element that this URI resolves to in memory.
	 * @return Returns a ref to the element.
	 */
	daeElementRef getElement() const;

	/** 
	 * Sets the element that this URI resolves to in memory.
	 * @param newref A ref to the element.
	 */
	void setElement(daeElementRef newref);

	/**
	 * Gets the resolve state of the URI.
	 * @return Returns the current state.
	 * @note This will be removed when daeURI starts managing its state internally.
	 */
	ResolveState getState() const;

	/**
	 * Gets a pointer to the @c daeElement that contains this URI.
	 * @return Returns the pointer to the containing daeElmement.
	 */
	daeElement* getContainer() const;

	/**
	 * Sets the pointer to the @c daeElement that contains this URI.
	 * @param cont Pointer to the containing @c daeElmement.
	 */
	void setContainer(daeElement* cont);

	/**
	 * Outputs all components of this @c daeIDRef to stderr.
	 */
	void print();

	/**
	 * Resets this @c daeIDRef; frees all string references
	 * and returns <tt><i>state</i></tt> to @c empty.
	 */
	void reset();

	/**
	 * Initializes the @c daeIDREf, setting <tt><i>id, element,</i></tt>  and <tt><i>container</i></tt> to NULL.
	 */
	void initialize();

	/**
	 * Comparison operator.
	 * @return Returns true if URI's are equal.
	 */
	bool operator==(const daeIDRef& other) const;

	/**
	 * Assignment operator.
	 * @return Returns a reference to this object.
	 */
	daeIDRef &operator=( const daeIDRef& other);

	// These methods are only provided for backwards compatibility. Use the listed alternatives.
	daeIDRef &get( daeUInt idx ); // Never should have existed. No alternative.
	size_t getCount() const; // Never should have existed. No alternative.
	daeIDRef& operator[](size_t index); // Never should have existed. No alternative.
	void resolveElement( daeString typeNameHint = NULL ); // Call getElement. No separate "resolve" step needed.
	void resolveID(); // Never should have existed. No alternative.
	void validate(); // Never should have existed. No alternative.
	void copyFrom(const daeIDRef& from); // Use the assignment operator instead.
};

class daeIDRefResolver;
typedef daeTArray<daeIDRefResolver*> daeIDRefResolverPtrArray;

/**
 * The @c daeIDRefResolver class is the plugin point for @c daeIDRef resolution.
 * This class is an abstract base class that defines an interface for
 * resolving @c daeIDRefs.
 */
class DLLSPEC daeIDRefResolver
{
public:
	/**
	 * Constructor
	 */
	daeIDRefResolver(DAE& dae);

	/**
	 * Destructor
	 */
	virtual ~daeIDRefResolver();
	
	/**
	 * Provides an abstract interface to convert a @c daeIDRef into a @c daeElement.
	 * @param id The ID of the element to find.
	 * @param docURI The URI of the document containing the element.
	 * @param result A ResolveState value indicating the result.
	 * @return Returns a daeElement with matching ID, if one is found.
	 */
	virtual daeElement* resolveElement(daeString id,
	                                   daeString docURI,
	                                   daeIDRef::ResolveState* result = NULL) = 0;

	/**
	 * Gets the name of this resolver.
	 * @return Returns the string name.
	 */
	virtual daeString getName() = 0;

protected:
	DAE* dae;
};


/**
 * The @c daeDefaultIDRefResolver resolves a @c daeIDRef by checking with a database.
 * It is a concrete implementation for @c daeIDRefResolver.
 */
class DLLSPEC daeDefaultIDRefResolver : public daeIDRefResolver
{
public:
	/**
	 * Constructor
	 * @param database @c daeDatabase for this implementation.
	 */
	daeDefaultIDRefResolver(DAE& dae);

	/**
	 * Destructor
	 */
	~daeDefaultIDRefResolver();

	/*
	 * Implements base class abstract routine from @c daeIDRefResolver.
	 */
	virtual daeElement* resolveElement(daeString id,
	                                   daeString docURI,
	                                   daeIDRef::ResolveState* result = NULL);
	
	/*
	 * Implements base class abstract routine from @c daeIDRefResolver.
	 */
	virtual daeString getName();
};


class daeIDRefResolverList {
public:
	daeIDRefResolverList();
	~daeIDRefResolverList();

	void addResolver(daeIDRefResolver* resolver);
	void removeResolver(daeIDRefResolver* resolver);

	daeElement* resolveElement(daeString id,
	                           daeString docURI,
	                           daeIDRef::ResolveState* result = NULL);

private:
	// Disabled copy constructor/assignment operator
	daeIDRefResolverList(const daeIDRefResolverList& resolverList) { };
	daeIDRefResolverList& operator=(const daeIDRefResolverList& resolverList) { return *this; };

	daeTArray<daeIDRefResolver*> resolvers;
};


#endif //__DAE_IDREF_H__