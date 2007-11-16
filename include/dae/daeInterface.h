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

#ifndef __DAE_INTERFACE__
#define __DAE_INTERFACE__

#include <dae/daeTypes.h>
class daeElement;
class daeDatabase;
class daeIOPlugin;
class domCOLLADA;

/**
* Integration library meta register function.
*
* Such a function (@c intRegisterElements() )is autogenerated by the COLLADA code generator 
* and can be modified if necessary to register the meta of the 
* integration library objects.
*/
typedef void (*daeIntegrationLibraryFunc)();

/**
* The @c daeInterface class provides an interface with the COLLADA runtime database.
*/
class daeInterface
{
public:	
	/**
	*  Destructor.
	 */
	virtual DLLSPEC ~daeInterface() {}
	
	/** @name Database setup 
	 * management of the database that stores the COLLADA elements.
	 */ 
	//@{
	/** 
	* Gets the COLLADA runtime database currently being used.
	* @return Returns the database currently being used.
	*/
	virtual DLLSPEC daeDatabase* getDatabase() = 0;
	/** 
	* Sets the COLLADA runtime database to use.
	* @param database Database that stores the COLLADA data,
	* if set to NULL a default database is set.
	* @return Returns @c DAE_OK if success, otherwise, returns a negative error value as defined in daeError.h.
	*/
	virtual DLLSPEC daeInt setDatabase(daeDatabase* database) = 0;
	//@}
	
	/** @name IOPlugin setup
	* handle the backend, which can import or export the COLLADA
	* database to a file system, to a runtime or to any other storage system.
	*/
	//@{
	/**
	* Gets the @c daeIOPlugin currently set.
	* @return Returns the @c daeIOPlugin currently set on the interface.
	*/
	virtual DLLSPEC daeIOPlugin* getIOPlugin() = 0;
	/** 
	* Sets the plugin which will be the interface between the COLLADA runtime database
	* and the rest of the system. 
	* It can be used to read or write from a native XML file, to convert, or to store 
	* in a more complex structure like a storage database.
	* @param plugin Plugin to use, if set to NULL a default plugin is set.
	* @return Returns @c DAE_OK if success, otherwise, returns a negative error value as defined in daeError.h.
	*/
	virtual DLLSPEC daeInt setIOPlugin(daeIOPlugin* plugin) = 0;
	//@}

	/** @name Integration Library Setup
	* definition of an integration library to use when processing COLLADA file.
	* It defines a framework for a user to insert a source code that will be called 
	* in the context of an import or export. Hence, it's the preferred way of 
	* accessing COLLADA runtime structure and convert between COLLADA and the user's
	* runtime.
	*/
	//@{
	/**
	* Gets the integration library register function currently being used.
	* @return Returns the integration library register function currently being used.
	*/
    virtual DLLSPEC daeIntegrationLibraryFunc getIntegrationLibrary() = 0;
	/**
	* Sets the integration library register function.
	* @param regFunc Integration library register function to use.
	* @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
	*/
	virtual DLLSPEC daeInt setIntegrationLibrary(daeIntegrationLibraryFunc regFunc)=0;
	//@}

	/** @name Batch import/export operations
	* import or export a COLLADA database by using the daeIOPlugin currently set.
	*/
	//@{
	/**
	* Loads a COLLADA document into the runtime database
	* @param uri the document to load.  The format for this is defined by the IO plugin
	* being used, in most cases this will be an rfc 2396 compliant relative or absolute URI.  Please check
	* the class documentation for the IO plugin you are using for specific restrictions.  Not all IO plugins
	* support all types of URIs.
	* @param docBuffer A string containing the text of the document to load. This is an optional attribute
	* and should only be used if the document has already been loaded into memory.
	* @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
	*/
	virtual DLLSPEC daeInt load(daeString uri, daeString docBuffer = NULL) = 0;
	/**
	* Saves a single document back to the location it was loaded from.
	* @param uri the URI of the loaded document to be saved, in most cases this will be an rfc 2396 compliant
	* URI but some IO plugins may work differently.  Please check the class documentation for the IO plugin you are using for specific restrictions.
	* @param replace If set to false, save won't save over top of an existing document and will return a DAE_ERR_BACKEND_FILE_EXISTS
	* error.  
	* @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
	*/
	virtual DLLSPEC daeInt save(daeString uri, daeBool replace=true) = 0;
	/**
	* Saves a single document back to the location it was loaded from.
	* @param documentIndex the index of a loaded document to be saved.
	* @param replace If set to false, save won't save over top of an existing document and will return a DAE_ERR_BACKEND_FILE_EXISTS
	* error.  
	* @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
	*/
	virtual DLLSPEC daeInt save(daeUInt documentIndex=0, daeBool replace=true) = 0;
	/**
	* Saves a single document from the runtime database by name.
	* @param uriToSaveTo the URI to save the document to.  The format for this is defined by the IO plugin
	* being used, in most cases this will be an rfc 2396 compliant relative or absolute URI.  Please check
	* the class documentation for the IO plugin you are using for specific restrictions.  Not all IO plugins
	* support all types of URIs.
	* @param docUri the URI of the document to save.  This is also defined by the IO plugin, in
	* most cases this will be the URI of where the document was loaded from.
	* @param replace If set to false, saveAs won't save over top of an existing document and will return a DAE_ERR_BACKEND_FILE_EXISTS
	* error.  
	* @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
    */
	virtual DLLSPEC daeInt saveAs(daeString uriToSaveTo, daeString docUri, daeBool replace=true) = 0;
	/**
	* Saves a single document from the runtime database by index.
	* @param uriToSaveTo the URI to save the document to.  The format for this is defined by the IO plugin
	* being used, in most cases this will be an rfc 2396 compliant relative or absolute URI.  Please check
	* the class documentation for the IO plugin you are using for specific restrictions.  Not all IO plugins
	* support all types of URIs.
	* @param documentIndex the index of the document to save, 0 is the first document loaded...etc.
	* Defaults to saving the first document loaded
	* @param replace If set to false, saveAs won't save over top of an existing document and will return a DAE_ERR_BACKEND_FILE_EXISTS
	* error.  
	* @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
    */
	virtual DLLSPEC daeInt saveAs(daeString uriToSaveTo, daeUInt documentIndex=0, daeBool replace=true) = 0;
	/**
	* Unloads a specific document from the runtime database.
	* @param uri URI of the document to remove.
	* @return Returns DAE_OK if unloaded successfully, otherwise returns a negative value as defined in daeError.h.
	* @note This function is not currently implemented.
	*/
	virtual DLLSPEC daeInt unload(daeString uri) = 0;

	// These are exactly the same as the other load/save/unload functions, except that they
	// work with file paths instead of URIs.
	DLLSPEC virtual daeInt loadFile(daeString file, daeString memBuffer = NULL) = 0;
	DLLSPEC virtual daeInt saveFile(daeString file, daeBool replace = true) = 0;
	DLLSPEC virtual daeInt saveFileAs(daeString fileToSaveTo, daeString file, daeBool replace = true) = 0;
	DLLSPEC virtual daeInt saveFileAs(daeString fileToSaveTo, daeUInt documentIndex = 0, daeBool replace = true) = 0;
	DLLSPEC virtual daeInt unloadFile(daeString file) = 0;
	
	/**
	* Unloads all the documents of the runtime database.
	* This function frees all the @c dom* objects and integration objects created so far,
	* except the object on which the user still has a smart pointer reference.
	* @return Returns DAE_OK if all documents unloaded successfully, otherwise returns a negative value as defined in daeError.h.
	*/
	virtual DLLSPEC daeInt clear() = 0;
	//@}
	
	/** @name Import/export progress
	*/
	//@{
	/**
	* Gets the progress of @c load() operation.
	* This function can be used from another thread to check the progress of a @c load() 
	* operation. The user can update a progress bar <tt> bytesParsed/totalBytes </tt> gives the
	* percentage of progress of the operation.
	* @param bytesParsed Pointer to an integer that receives the number of bytes already 
	* consumed from the file, can be NULL if the user don't want to retrieve this information.
	* @param lineNumber Pointer to an integer that receives the number of lines read so far,
	* can be NULL.
	* @param totalBytes Pointer to an integer that receives the total number of bytes in the 
	* file currently being loaded, can be NULL.
	* @param reset Indicates whether to reset the counters. A value of false is the default behavior
	* that fits most usage. Set it to true to reset 
	* the <tt><i> bytesParsed </i></tt> and <tt><i> lineNumber </i></tt> counters. The system resets the counter at the beginning of 
	* each file.
	*/
	virtual DLLSPEC void getProgress(daeInt* bytesParsed,
							 daeInt* lineNumber,
							 daeInt* totalBytes,
							 daeBool reset = false )=0;
	//@}
	
	/** @name Main DOM Query
	*/
	//@{
	/**
	* Gets the COLLADA tree root of a given document.
	* @param uri Document URI, for the file @c daeIOPlugin, this will be the filename for a file.
	* @return Returns the @c domCOLLADA root object of the document, or NULL if the document is not found.
	*/
	virtual DLLSPEC domCOLLADA* getDom(daeString uri) = 0;
	/**
	* Gets the COLLADA schema version that was used to build the DOM classes
	* @return a text string with the version number in it (ie: 1.3.1)
	*/
	virtual DLLSPEC daeString getDomVersion() = 0;
	/**
	* Sets or inserts a COLLADA tree into the database.
	* The system creates a default database if none is set and then creates a document
	* if the document doesn't already exist. The document keeps a reference on the 
	* @c daeElement, so you can then delete your own reference to the object safely.
	* @param uri the document URI, may be an absolute or relative URI.  The URI will be resolved to an absolute version
	* and then compared with the absolute version of the document's URI.  If the URI contains a fragment, it is stripped out.
	* @param dom Root tree.
	* @return Returns DAE_OK if success, otherwise returns a negative value as defined in daeError.h.
	*/
	virtual DLLSPEC daeInt setDom(daeString uri, domCOLLADA* dom) = 0;

	// Same as getDom/setDom, except works with file paths instead of URIs
	DLLSPEC virtual domCOLLADA* getDomFile(daeString file) = 0;
	DLLSPEC virtual daeInt      setDomFile(daeString file, domCOLLADA* dom) = 0;
	//@}
};

#endif // __DAE_INTERFACE__

