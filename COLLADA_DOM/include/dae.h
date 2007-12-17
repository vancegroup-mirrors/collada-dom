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

#ifndef __DAE__
#define __DAE__

#include <dae/daeTypes.h>
#include <dae/daeError.h>
#include <dae/daeDatabase.h>
#include <dae/daeIOPlugin.h>
#include <dae/daeIntegrationObject.h>
#include <dae/daeAtomicType.h>
#include <dae/daeMetaElement.h>
#include <dae/daeIDRef.h>
#include <dae/daeURI.h>

class domCOLLADA;
class daeDatabase;

/**
 * Integration library meta register function.
 *
 * Such a function (@c intRegisterElements() )is autogenerated by the COLLADA code generator 
 * and can be modified if necessary to register the meta of the 
 * integration library objects.
 */
typedef void (*daeIntegrationLibraryFunc)();

// The DAE class is the core interface via which you interact with the DOM.
// It has methods to load/save documents, get the root element of each document, etc.
class DLLSPEC DAE
{
public:
	// Constructor. If no database or IO plugin are provided, a default database and
	// IO plugin will be used.
	DAE(daeDatabase* database = NULL, daeIOPlugin* ioPlugin = NULL)
		: atomicTypes(*this),
		  baseUri(*this, true)
	{
		// See the end of the thread linked below for an explanation of why we have the DAE
		// constructor set up this way. Basically, I'm going to be changing the build output 
		// location, and when this happens people sometimes continue to link against the old
		// libraries by accident (e.g. if they just do an svn update). By introducing a new
		// function that gets called from a function in a header file, I'm ensuring that someone
		// who tries linking against old libraries will get a link error. This may not sound
		// very nice, but it's certainly better than getting bizarre runtime crashes.
		// https://collada.org/public_forum/viewtopic.php?t=771&sid=f13c34f2d17ca720c5021bccbe5128b7
		init(database, ioPlugin);
	}

	virtual ~DAE();

	// Release all memory used by the DOM. You never need to call this explicitly. It's
	// called automatically when all DAE objects go out of scope.
	static void cleanup();
	
public:
	// Database setup	
	virtual daeDatabase* getDatabase();
	virtual daeInt setDatabase(daeDatabase* database);

	// IO Plugin setup
	virtual daeIOPlugin* getIOPlugin();
	virtual daeInt setIOPlugin(daeIOPlugin* plugin);

	// Integration Library Setup
	virtual daeIntegrationLibraryFunc getIntegrationLibrary();
	virtual daeInt setIntegrationLibrary(daeIntegrationLibraryFunc regFunc);

	// Document load/save.
	virtual daeInt load(daeString uri, daeString docBuffer = NULL);
	virtual daeInt save(daeString uri, daeBool replace=true);
	virtual daeInt save(daeUInt documentIndex, daeBool replace=true);
	virtual daeInt saveAs(daeString uriToSaveTo, daeString docUri, daeBool replace=true);
	virtual daeInt saveAs(daeString uriToSaveTo, daeUInt documentIndex=0, daeBool replace=true);
	virtual daeInt unload(daeString uri);

	// These are exactly the same as the above load/save/unload functions, except that they
	// work with file paths instead of URIs.
	virtual daeInt loadFile(daeString file, daeString memBuffer = NULL);
	virtual daeInt saveFile(daeString file, daeBool replace = true);
	virtual daeInt saveFileAs(daeString fileToSaveTo, daeString file, daeBool replace = true);
	virtual daeInt saveFileAs(daeString fileToSaveTo, daeUInt documentIndex = 0, daeBool replace = true);
	virtual daeInt unloadFile(daeString file);

	// Remove all loaded documents.
	virtual daeInt clear();

	// Load/Save Progress. Currently not implemented.
	virtual void getProgress(daeInt* bytesParsed,
	                         daeInt* lineNumber,
	                         daeInt* totalBytes,
	                         daeBool reset = false);

	// Get the root domCOLLADA object corresponding to a particular document (identified
	// via the URI of the document.
	virtual domCOLLADA* getDom(daeString uri);
	virtual daeInt setDom(daeString uri, domCOLLADA* dom);

	// Same as getDom/setDom, except works with file paths instead of URIs
	virtual domCOLLADA* getDomFile(daeString file);
	virtual daeInt      setDomFile(daeString file, domCOLLADA* dom);

	virtual daeString getDomVersion();

	// Returns the (modifiable) list of atomic type objects.
	daeAtomicTypeList& getAtomicTypes();

	// Get/set a daeMetaElement object given the meta object's type ID.
	daeMetaElement* getMeta(daeInt typeID);
	void setMeta(daeInt typeID, daeMetaElement& meta);

	// Get all daeMetaElement objects.
	daeMetaElementRefArray& getAllMetas();

	// Add an element to the list of elements that need a daeElement::resolve call.
	void appendResolveElement(daeElement* elem);

	// Call daeElement::resolve on all elements waiting to be resolved.
	void resolveAll();

	// Returns the list of URI resolvers. You can modify the list to add new resolvers.
	daeURIResolverList& getURIResolvers();

	// The base URI used for resolving relative URI references.
	daeURI& getBaseURI();
	void setBaseURI(daeURI& uri);

	// Returns the list of ID reference resolvers. You can modify the list to add new
	// resolvers.
	daeIDRefResolverList& getIDRefResolvers();

private:
	void init(daeDatabase* database, daeIOPlugin* ioPlugin);

	daeDatabase *database;
	daeIOPlugin *plugin;
	bool defaultDatabase;
	bool defaultPlugin;
	daeIntegrationLibraryFunc registerFunc;
	daeAtomicTypeList atomicTypes;
	daeMetaElementRefArray metas;
	daeElementRefArray resolveArray;
	daeURI baseUri;
	daeURIResolverList uriResolvers;
	daeIDRefResolverList idRefResolvers;
};


template <typename T> 
inline T *daeSafeCast(daeElement *element)
{ 
	if (element  &&  element->typeID() == T::typeIDStatic())
		return (T*)element; 
	return NULL; 
}


#endif // __DAE_INTERFACE__
