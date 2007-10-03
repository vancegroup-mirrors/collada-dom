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

// The user can choose whether or not to include libxml support in the DOM. Supporting libxml will
// require linking against it. By default libxml support is included.
#if defined(DOM_INCLUDE_LIBXML)

// This is a rework of the XML plugin that contains a complete interface to libxml2 "readXML"
// This is intended to be a seperate plugin but I'm starting out by rewriting it in daeLIBXMLPlugin
// because I'm not sure if all the plugin handling stuff has been tested.  Once I get a working
// plugin I'll look into renaming it so the old daeLIBXMLPlugin can coexist with it.
//
#include <string>
#include <sstream>
#include <modules/daeLIBXMLPlugin.h>
#include <dae.h>
#include <dom.h>
#include <dae/daeMetaElement.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlmemory.h>
#include <dae/daeErrorHandler.h>
#include <dae/daeMetaElementAttribute.h>

// Some helper functions for working with libxml
namespace {
	daeInt getCurrentLineNumber(xmlTextReaderPtr reader) {
#if LIBXML_VERSION >= 20620
		return xmlTextReaderGetParserLineNumber(reader);
#else
		return -1;
#endif
	}

	// The attributes vector passed in should be empty
	void packageCurrentAttributes(xmlTextReaderPtr reader,
																/* out */ std::vector<std::pair<daeString, daeString> >& attributes) {
		int numAttributes = xmlTextReaderAttributeCount(reader);
		if (numAttributes == -1 || numAttributes == 0)
			return;
		attributes.reserve(numAttributes);
		
		while (xmlTextReaderMoveToNextAttribute(reader) == 1) {
			attributes.push_back(std::pair<daeString, daeString>((daeString)xmlTextReaderConstName(reader),
																													 (daeString)xmlTextReaderConstValue(reader)));
		}
	}
}

daeLIBXMLPlugin::daeLIBXMLPlugin()
{
	 xmlInitParser();
	 rawFile = NULL;
	 rawByteCount = 0;
	 saveRawFile = false;
}

daeLIBXMLPlugin::~daeLIBXMLPlugin()
{
	 xmlCleanupParser();
}

daeTArray<std::string> daeLIBXMLPlugin::getSupportedProtocols() {
	daeTArray<std::string> protocols;
	protocols.append("file");
	protocols.append("http");
	return protocols;
}

daeInt daeLIBXMLPlugin::setOption( daeString option, daeString value )
{
	if ( strcmp( option, "saveRawBinary" ) == 0 )
	{
		if ( strcmp( value, "true" ) == 0 || strcmp( value, "TRUE" ) == 0 )
		{
			saveRawFile = true;
		}
		else
		{
			saveRawFile = false;
		}
		return DAE_OK;
	}
	return DAE_ERR_INVALID_CALL;
}

daeString daeLIBXMLPlugin::getOption( daeString option )
{
	if ( strcmp( option, "saveRawBinary" ) == 0 )
	{
		if ( saveRawFile )
		{
			return "true";
		}
		return "false";
	}
	return NULL;
}

void daeLIBXMLPlugin::getProgress(daeInt* bytesParsed,
						 daeInt* lineNumber,
						 daeInt* totalBytes,
						 daeBool reset)
{
	// Need to interface this to libxml
	if (reset)
	{
		//daeChunkBuffer::resetProgress();
	}
#if LIBXML_VERSION >= 20620
	if (bytesParsed)
		*bytesParsed= 0; //xmlTextReaderByteConsumed(reader); // Not sure if this is the right data
	if (lineNumber)
		*lineNumber = 0; //xmlTextReaderGetParserLineNumber(reader);
#else
	if (bytesParsed)
		*bytesParsed= 0;
	if (lineNumber)
		*lineNumber = 0;
#endif
	if (totalBytes)
		*totalBytes = 0; // Not available
}

// A simple structure to help alloc/free xmlTextReader objects
struct xmlTextReaderHelper {
	xmlTextReaderHelper(const daeURI& uri) {
		reader = xmlReaderForFile(uri.getURI(), NULL, 0);
	}

	xmlTextReaderHelper(daeString buffer, const daeURI& baseUri) {
		reader = xmlReaderForDoc((xmlChar*)buffer, baseUri.getURI(), NULL, 0);
	};

	~xmlTextReaderHelper() {
		if (reader)
			xmlFreeTextReader(reader);
	}

	xmlTextReaderPtr reader;
};

daeElementRef daeLIBXMLPlugin::readFromFile(const daeURI& uri) {
	xmlTextReaderHelper readerHelper(uri);
	if (!readerHelper.reader) {
		daeErrorHandler::get()->handleError((std::string("Failed to open ") + uri.getURI() +
																				 " in daeLIBXMLPlugin::readFromFile\n").c_str());
		return NULL;
	}
	return read(readerHelper.reader);
}

daeElementRef daeLIBXMLPlugin::readFromMemory(daeString buffer, const daeURI& baseUri) {
	xmlTextReaderHelper readerHelper(buffer, baseUri);
	if (!readerHelper.reader) {
		daeErrorHandler::get()->handleError("Failed to open XML document from memory buffer in "
																				"daeLIBXMLPlugin::readFromMemory\n");
		return NULL;
	}
	return read(readerHelper.reader);
}

daeElementRef daeLIBXMLPlugin::read(_xmlTextReader* reader) {
	// Drop everything up to the first element. In the future, we should try to store header comments somewhere.
	while(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT)
	{
		if (xmlTextReaderRead(reader) != 1) {
			daeErrorHandler::get()->handleError("Error parsing XML in daeLIBXMLPlugin::read\n");
			return NULL;
		}
	}

	return readElement(reader, NULL);
}

daeElementRef daeLIBXMLPlugin::readElement(_xmlTextReader* reader, daeElement* parentElement) {
	assert(xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT);
	daeString elementName = (daeString)xmlTextReaderConstName(reader);
	bool empty = xmlTextReaderIsEmptyElement(reader) != 0;

	std::vector<attrPair> attributes;
	packageCurrentAttributes(reader, /* out */ attributes);
	
	daeElementRef element = beginReadElement(parentElement, elementName, attributes, getCurrentLineNumber(reader));
	if (!element) {
		// We couldn't create the element. beginReadElement already printed an error message. Just make sure
		// to skip ahead past the bad element.
		xmlTextReaderNext(reader);
		return NULL;
	}

	if (xmlTextReaderRead(reader) != 1  ||  empty)
		return element;

	int nodeType = xmlTextReaderNodeType(reader);
	while (nodeType != -1  &&  nodeType != XML_READER_TYPE_END_ELEMENT) {
		if (nodeType == XML_READER_TYPE_ELEMENT) {
			element->placeElement(readElement(reader, element));
		} else if (nodeType == XML_READER_TYPE_TEXT) {
			readElementText(element, (daeString)xmlTextReaderConstValue(reader), getCurrentLineNumber(reader));
			xmlTextReaderRead(reader);
		}	else {
			xmlTextReaderRead(reader);
		}

		nodeType = xmlTextReaderNodeType(reader);
	}

	if (nodeType == XML_READER_TYPE_END_ELEMENT)
		xmlTextReaderRead(reader);

	return element;
}

daeInt daeLIBXMLPlugin::write(daeURI *name, daeDocument *document, daeBool replace)
{
	// Make sure database and document are both set
	if (!database)
		return DAE_ERR_INVALID_CALL;
	if(!document)
		return DAE_ERR_COLLECTION_DOES_NOT_EXIST;

	// Extract just the file path from the URI
	daeFixedName finalname;
	if (!name->getPath(finalname,sizeof(finalname)))
	{
		daeErrorHandler::get()->handleError( "can't get path in write\n" );
		return DAE_ERR_BACKEND_IO;
	}
	// !!!steveT: Replace this with real URI to file path conversion code
	// If replace=false, don't replace existing files
	if(!replace)
	{
		// Using "stat" would be better, but it's not available on all platforms
#if defined(WIN32)
		FILE *tempfd = fopen(finalname+1,"r");
#else
		FILE *tempfd = fopen(finalname, "r");
#endif		
		if(tempfd != NULL)
		{
			// File exists, return error
			fclose(tempfd);
			return DAE_ERR_BACKEND_FILE_EXISTS;
		}
		fclose(tempfd);
	}
	if ( saveRawFile )
	{
		daeFixedName rawFilename;
#if defined(WIN32)
		strcpy( rawFilename, finalname+1);
#else
		strcpy( rawFilename, finalname);
#endif
		strcat( rawFilename, ".raw" );
		if ( !replace )
		{
			rawFile = fopen(rawFilename, "rb" );
			if ( rawFile != NULL )
			{
				fclose(rawFile);
				return DAE_ERR_BACKEND_FILE_EXISTS;
			}
			fclose(rawFile);
		}
		rawFile = fopen(rawFilename, "wb");
		if ( rawFile == NULL )
		{
			return DAE_ERR_BACKEND_IO;
		}
		daeFixedName rawURIStr;
		strcpy( rawURIStr, "/" );
		strcat( rawURIStr, rawFilename );
		rawRelPath.setURI( rawURIStr );
		//rawRelPath.setURI( rawFilename );
		rawRelPath.validate();
		rawRelPath.makeRelativeTo( name );
	}

	// Open the file we will write to
	writer = xmlNewTextWriterFilename(name->getURI(), 0);
	if ( !writer ) {
		char msg[512];
		sprintf(msg,"daeLIBXMLPlugin::write(%s) failed\n",name->getURI());
		daeErrorHandler::get()->handleError( msg );
		return DAE_ERR_BACKEND_IO;
	}
	xmlTextWriterSetIndentString( writer, (const xmlChar*)"\t" ); // Don't change this to spaces
	xmlTextWriterSetIndent( writer, 1 ); // Turns indentation on
	xmlTextWriterStartDocument( writer, NULL, NULL, NULL );
	
	writeElement( document->getDomRoot() );
	
	xmlTextWriterEndDocument( writer );
	xmlTextWriterFlush( writer );
	xmlFreeTextWriter( writer );

	if ( saveRawFile && rawFile != NULL )
	{
		fclose( rawFile );
	}

	return DAE_OK;
}

void daeLIBXMLPlugin::writeElement( daeElement* element )
{
	daeIntegrationObject* _intObject = element->getIntObject();
	daeMetaElement* _meta = element->getMeta();
	if(_intObject)
	{
		// added in response to bug 478
		_intObject->toCOLLADAChecked();
		_intObject->toCOLLADAPostProcessChecked();
	}

	//intercept <source> elements for special handling
	if ( saveRawFile )
	{
		if ( strcmp( element->getTypeName(), "source" ) == 0 )
		{
			daeElementRefArray children;
			element->getChildren( children );
			bool validArray = false, teqCommon = false;
			for ( unsigned int i = 0; i < children.getCount(); i++ )
			{
				if ( strcmp( children[i]->getTypeName(), "float_array" ) == 0 || 
					 strcmp( children[i]->getTypeName(), "int_array" ) == 0 )
				{
					validArray = true;
				}
				else if ( strcmp( children[i]->getTypeName(), "technique_common" ) == 0 )
				{
					teqCommon = true;
				}
			}
			if ( validArray && teqCommon )
			{
				writeRawSource( element );
				return;
			}
		}
	}

	if (!_meta->getIsTransparent() ) {
		xmlTextWriterStartElement(writer, (xmlChar*)element->getElementName());
		daeMetaAttributeRefArray& attrs = _meta->getMetaAttributes();
		
		int acnt = (int)attrs.getCount();
		
		for(int i=0;i<acnt;i++) {
			writeAttribute(attrs[i], element);
		}
	}
	writeValue(element);
	
	daeElementRefArray children;
	element->getChildren( children );
	for ( size_t x = 0; x < children.getCount(); x++ ) {
		writeElement( children.get(x) );
	}

	/*if (_meta->getContents() != NULL) {
		daeElementRefArray* era = (daeElementRefArray*)_meta->getContents()->getWritableMemory(element);
		int elemCnt = (int)era->getCount();
		for(int i = 0; i < elemCnt; i++) {
			daeElementRef elem = (daeElementRef)era->get(i);
			if (elem != NULL) {
				writeElement( elem );
			}
		}
	}
	else
	{
		daeMetaElementAttributeArray& children = _meta->getMetaElements();
		int cnt = (int)children.getCount();
		for(int i=0;i<cnt;i++) {
			daeMetaElement *type = children[i]->getElementType();
			if ( !type->getIsAbstract() ) {
				for (int c = 0; c < children[i]->getCount(element); c++ ) {
					writeElement( *(daeElementRef*)children[i]->get(element,c) );
				}
			}
		}
	}*/
	if (!_meta->getIsTransparent() ) {
		xmlTextWriterEndElement(writer);
	}
}

void daeLIBXMLPlugin::writeAttribute( daeMetaAttribute* attr, daeElement* element)
{
	//don't write if !required and is set && is default
	if ( !attr->getIsRequired() ) {
		//not required
		if ( !element->isAttributeSet( attr->getName() ) ) {
			//early out if !value && !required && !set
			return;
		}
			
		//is set
		//check for default suppression
		if (attr->compareToDefault(element) == 0) {
			// We match the default value, so exit early
			return;
		}
	}

	xmlTextWriterStartAttribute(writer, (xmlChar*)(daeString)attr->getName());
	std::ostringstream buffer;
	attr->memoryToString(element, buffer);
	xmlTextWriterWriteString(writer, (xmlChar*)buffer.str().c_str());
	xmlTextWriterEndAttribute(writer);
}

void daeLIBXMLPlugin::writeValue(daeElement* element) {
	if (daeMetaAttribute* attr = element->getMeta()->getValueAttribute()) {
			std::ostringstream buffer;
			attr->memoryToString(element, buffer);
			std::string s = buffer.str();
			if (!s.empty()) {
				xmlTextWriterWriteString(writer, (xmlChar*)buffer.str().c_str());
			}
	}
}

void daeLIBXMLPlugin::writeRawSource( daeElement *src )
{
	daeElementRef newSrc = src->clone();
	daeElementRef array = NULL;
	daeElement *accessor = NULL;
	daeElementRefArray children;
	newSrc->getChildren( children );
	bool isInt = false;
	for ( int i = 0; i < (int)children.getCount(); i++ )
	{
		if ( strcmp( children[i]->getTypeName(), "float_array" ) == 0 )
		{
			array = children[i];
			newSrc->removeChildElement( array );
		}
		else if ( strcmp( children[i]->getTypeName(), "int_array" ) == 0 )
		{
			array = children[i];
			isInt = true;
			newSrc->removeChildElement( array );
		}
		else if ( strcmp( children[i]->getTypeName(), "technique_common" ) == 0 )
		{
			children[i]->getChildren( children );			
		}
		else if ( strcmp( children[i]->getTypeName(), "accessor" ) == 0 )
		{
			accessor = children[i];
		}
	}

	daeULong *countPtr = (daeULong*)array->getAttributeValue( "count" );
	daeULong count = countPtr != NULL ? *countPtr : 0;

	daeULong *stridePtr = (daeULong*)accessor->getAttributeValue( "stride" );
	daeULong stride = stridePtr != NULL ? *stridePtr : 1;

	children.clear();
	accessor->getChildren( children );
	if ( children.getCount() > stride ) {
		*stridePtr = children.getCount();
	}

	daeFixedName newURI;
	sprintf( newURI, "%s#%d", rawRelPath.getOriginalURI(), rawByteCount );
	accessor->setAttribute( "source", newURI );

	daeArray *valArray = (daeArray*)array->getValuePointer();

	//TODO: pay attention to precision for the array.
	if ( isInt )
	{
		for( size_t i = 0; i < count; i++ )
		{
			daeInt tmp = (daeInt)*(daeLong*)(valArray->getRaw(i));
			rawByteCount += (unsigned long)(fwrite( &tmp, sizeof(daeInt), 1, rawFile ) * sizeof(daeInt));
		}
	}
	else
	{
		for( size_t i = 0; i < count; i++ )
		{
			daeFloat tmp = (daeFloat)*(daeDouble*)(valArray->getRaw(i));
			rawByteCount += (unsigned long)(fwrite( &tmp, sizeof(daeFloat), 1, rawFile ) * sizeof(daeFloat));
		}
	}

	writeElement( newSrc );
}

#endif // DOM_INCLUDE_LIBXML
