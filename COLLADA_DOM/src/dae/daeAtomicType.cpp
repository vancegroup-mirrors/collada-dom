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

#include <sstream>
#include <dae/daeAtomicType.h>
#include <dae/daeElement.h>
#include <dae/daeURI.h>
#include <dae/daeIDRef.h>
#include <dae/daeMetaElement.h>
#include <dae/daeDatabase.h>
#include <dae/daeErrorHandler.h>

daeAtomicTypeArray* daeAtomicType::_Types = NULL;
daeBool daeAtomicType::_TypesInitialized = false;

namespace {
	// Skip leading whitespace
	daeChar* skipWhitespace(daeChar* s) {
		if (s) {
			// !!!GAC NEEDS TO BE CHANGED to use XML standard whitespace parsing
			while(*s == ' ' || *s == '\r' || *s == '\n' || *s == '\t') s++;
		}

		return s;
	}

	// Move forward past this token
	daeChar* skipToken(daeChar* s) {
		while(*s != ' ' && *s != '\r' && *s != '\n' && *s != '\t' && *s != 0) s++;
		return s;
	}

	// Given a string of whitespace-separated tokens, this function returns a null-terminated string
	// containing the next token. If the next token is already null-terminated, no memory is allocated
	// and the function returns the pointer that was passed in. Note that this function assumes that
	// the string passed in starts with the next token and not a whitespace.
	// If returnValue != s, the client should free the returnValue with delete[].
	daeChar* extractToken(daeChar* s) {
		if (!s)
			return 0;

		daeChar* tmp = skipToken(s);
		if (*tmp != 0) {
			daeChar* scopy = new daeChar[tmp-s+1];
			strncpy(scopy, s, tmp-s);
			scopy[tmp-s] = 0;
			return scopy;
		}

		return s;
	}
}

void
daeAtomicType::initializeKnownTypes()
{
	_Types = new daeAtomicTypeArray;
	initializeKnownBaseTypes();
	//mandatory to set here, because the array types are querying the atomic types
	_TypesInitialized = true;
}

void 
daeAtomicType::uninitializeKnownTypes()
{
	if ( _TypesInitialized )
		{
		_TypesInitialized = false;
		unsigned int i;
		for (i=0;i<_Types->getCount();i++)
		{
			daeAtomicType* type = _Types->get(i);
			delete type;
		}
		delete _Types;
	}
}

void
daeAtomicType::initializeKnownBaseTypes()
{
	_Types->append(new daeUIntType);
	_Types->append(new daeIntType);
	_Types->append(new daeLongType);
	_Types->append(new daeShortType);
	_Types->append(new daeULongType);
	_Types->append(new daeFloatType);
	_Types->append(new daeDoubleType);
	_Types->append(new daeStringRefType);
	_Types->append(new daeElementRefType);
	_Types->append(new daeEnumType);
	_Types->append(new daeRawRefType);
	_Types->append(new daeResolverType);
	_Types->append(new daeIDResolverType);
	_Types->append(new daeBoolType);
	_Types->append(new daeTokenType);
}

daeAtomicType*
daeAtomicType::get(daeStringRef typeString)
{
	if (!_TypesInitialized)
		daeAtomicType::initializeKnownTypes();

	int tCount = (int)_Types->getCount();
	int i;
	for(i=0; i<tCount; i++) {
		daeAtomicType* type = _Types->get(i);
		daeStringRefArray& nameBindings = type->getNameBindings();
		int count = (int)nameBindings.getCount();
		int j;
		for(j=0;j<count;j++)
			if (!strcmp(nameBindings[j],typeString))
				break;
		if (j!=count)
			return type;
	}
	return NULL;
}
daeAtomicType::daeAtomicType()
{
	_size = -1;
	_alignment = -1;
	_typeEnum = -1;
	_typeString = "notype";
	_printFormat = "badtype";
	_scanFormat = "";
	_maxStringLength = -1;
}

daeAtomicType*
daeAtomicType::get(daeEnum typeEnum)
{
	if (!_TypesInitialized)
		daeAtomicType::initializeKnownTypes();

	int tCount = (int)_Types->getCount();
	int i;
	for(i=0; i<tCount; i++) {
		daeAtomicType* type = _Types->get(i);
		if (type->getTypeEnum() == typeEnum)
			return type;
	}
	return NULL;
}

daeBool
daeAtomicType::stringToMemory(daeChar *src, daeChar* dstMemory)
{
	sscanf(src, _scanFormat, dstMemory);
	return true;
}

void daeAtomicType::arrayToString(daeArray& array, std::ostringstream& buffer) {
	if (array.getCount() > 0)
		memoryToString(array.getRaw(0), buffer);
	for (size_t i = 1; i < array.getCount(); i++) {
		buffer << ' ';
		memoryToString(array.getRaw(i), buffer);
	}
}

daeBool
daeAtomicType::stringToArray(daeChar* src, daeArray& array) {
	array.clear();
	array.setElementSize(_size);

	if (src == 0)
		return false;

	// We're about to insert null terminators into the string so that scanf doesn't take forever
	// doing strlens. Since the memory might not be writable, I need to duplicate the string and
	// write into the duplicate, or else I might get access violations.
	// This sucks... surely we can do better than this.
	daeChar* srcDup = new daeChar[strlen(src)+1];
	strcpy(srcDup, src);
	src = srcDup;

	while (*src != 0)
	{
		src = skipWhitespace(src);
		if(*src != 0)
		{
			daeChar* token = src;
			src = skipToken(src);
			daeChar temp = *src;
			*src = 0;

			array.setCount(array.getCount()+1);
			if (!stringToMemory(token, array.getRaw(array.getCount()-1))) {
				delete[] srcDup;
				return false;
			}

			*src = temp;
		}
	}

	delete[] srcDup;
	return true;
}

daeInt daeAtomicType::compareArray(daeArray& value1, daeArray& value2) {
	if (value1.getCount() != value2.getCount())
		return value1.getCount() > value2.getCount() ? 1 : -1;

	for (size_t i = 0; i < value1.getCount(); i++) {
		daeInt result = compare(value1.getRaw(i), value2.getRaw(i));
		if (result != 0)
			return result;
	}

	return 0;
}

void daeAtomicType::copyArray(daeArray& src, daeArray& dst) {
	dst.setCount(src.getCount());
	for (size_t i = 0; i < src.getCount(); i++)
		copy(src.getRaw(i), dst.getRaw(i));
}

daeInt
daeAtomicType::compare(daeChar* value1, daeChar* value2) {
	return memcmp(value1, value2, _size);
}

daeInt
daeAtomicType::append(daeAtomicType* t) {
	if (!_TypesInitialized)
		daeAtomicType::initializeKnownTypes();
	return (daeInt)_Types->append(t);
}
	
const daeAtomicType*
daeAtomicType::getByIndex(daeInt index) {
	return _Types->get(index);
}
	
daeInt
daeAtomicType::getCount() {
	return (daeInt)_Types->getCount();
}

daeEnumType::daeEnumType()
{
	_size = sizeof(daeEnum);
	_alignment = sizeof(daeEnum);
	_typeEnum = EnumType;
	_nameBindings.append("enum");
	_printFormat = "%s";//"%%.%ds";
	_scanFormat = "%s";
	_strings = NULL;
	_values = NULL;
	_typeString = "enum";
}

daeEnumType::~daeEnumType() {
	if ( _strings ) {
		delete _strings;
		_strings = NULL;
	}
	if ( _values ) {
		delete _values;
		_values = NULL;
	}
}

daeBoolType::daeBoolType()
{
	_size = sizeof(daeBool);
	_alignment = sizeof(daeBool);
	_typeEnum = BoolType;
	_printFormat = "%d";
	_scanFormat = "%d";
	_typeString = "bool";
	_maxStringLength = (daeInt)strlen("false")+1;
	_nameBindings.append("bool");
	//_nameBindings.append("xsBool");
	_nameBindings.append("xsBoolean");
}

daeIntType::daeIntType()
{
	_size = sizeof(daeInt);
	_alignment = sizeof(daeInt);
	_typeEnum = IntType;
	_maxStringLength = 16;
	_nameBindings.append("int");
	_nameBindings.append("xsInteger");
	_nameBindings.append("xsHexBinary");
	_nameBindings.append("xsIntegerArray");
	_nameBindings.append("xsHexBinaryArray");
	_nameBindings.append("xsByte");
	_nameBindings.append("xsInt");
	_printFormat = "%d";
	_scanFormat = "%d";
	_typeString = "int";
}
daeLongType::daeLongType()
{
	_size = sizeof(daeLong);
	_alignment = sizeof(daeLong);
	_typeEnum = LongType;
	_maxStringLength = 32;
	_nameBindings.append("xsLong");
	_nameBindings.append("xsLongArray");
	_printFormat = "%lld";
	_scanFormat = "%lld";
	_typeString = "long";
}
daeShortType::daeShortType()
{
	_maxStringLength = 8;
	_size = sizeof(daeShort);
	_alignment = sizeof(daeShort);
	_typeEnum = ShortType;
	_nameBindings.append("short");
	_nameBindings.append("xsShort");
	_printFormat = "%hd";
	_scanFormat = "%hd";
	_typeString = "short";
}
daeUIntType::daeUIntType()
{
	_maxStringLength = 16;
	_size = sizeof(daeUInt);
	_alignment = sizeof(daeUInt);
	_typeEnum = UIntType;
	_nameBindings.append("uint");
	_nameBindings.append("xsNonNegativeInteger");
	_nameBindings.append("xsUnsignedByte");
	_nameBindings.append("xsUnsignedInt");
	_nameBindings.append("xsPositiveInteger");
	_printFormat = "%u";
	_scanFormat = "%u";
	_typeString = "uint";
}
daeULongType::daeULongType()
{
	_size = sizeof(daeULong);
	_alignment = sizeof(daeULong);
	_typeEnum = ULongType;
	_maxStringLength = 32;
	_nameBindings.append("ulong");
	_nameBindings.append("xsUnsignedLong");
	_printFormat = "%llu";
	_scanFormat = "%llu";
	_typeString = "ulong";
}
daeFloatType::daeFloatType()
{
	_maxStringLength = 64;
	_size = sizeof(daeFloat);
	_alignment = sizeof(daeFloat);
	_typeEnum = FloatType;
	_nameBindings.append("float");
	_nameBindings.append("xsFloat");
	_printFormat = "%g";
	_scanFormat = "%g";
	_typeString = "float";
}
daeDoubleType::daeDoubleType()
{
	_size = sizeof(daeDouble);
	_alignment = sizeof(daeDouble);
	_typeEnum = DoubleType;
	_nameBindings.append("double");
	_nameBindings.append("xsDouble");
	_nameBindings.append("xsDecimal");
	_printFormat = "%lg";
	_scanFormat = "%lg";
	_typeString = "double";
	_maxStringLength = 64;
}

daeStringRefType::daeStringRefType()
{
	_size = sizeof(daeStringRef);
	_alignment = sizeof(daeStringRef);
	_typeEnum = StringRefType;
	_nameBindings.append("string");
	_nameBindings.append("xsString");
	_nameBindings.append("xsDateTime");	
	_printFormat = "%s";
	_scanFormat = "%s";
	_typeString = "string";
}

daeTokenType::daeTokenType()
{
	_size = sizeof(daeStringRef);
	_alignment = sizeof(daeStringRef);
	_typeEnum = TokenType;
	_nameBindings.append("token");
	_nameBindings.append("xsID");
	_nameBindings.append("xsNCName");
	_nameBindings.append("xsNMTOKEN");
	_nameBindings.append("xsName");
	_nameBindings.append("xsToken");
	_nameBindings.append("xsNameArray");
	_nameBindings.append("xsTokenArray");
	_nameBindings.append("xsNCNameArray");
	_printFormat = "%s";
	_scanFormat = "%s";
	_typeString = "token";
}

daeElementRefType::daeElementRefType()
{
	_size = sizeof(daeElementRef);
	_alignment = sizeof(daeElementRef);
	_typeEnum = ElementRefType;
	_nameBindings.append("element");
	_nameBindings.append("Element");
	_nameBindings.append("TrackedElement");
	_printFormat = "%p";
	_scanFormat = "%p";
	_typeString = "element";
	_maxStringLength = 64;
}

daeRawRefType::daeRawRefType()
{
	_size = sizeof(daeRawRef);
	_alignment = sizeof(daeRawRef);
	_typeEnum = RawRefType;
	_nameBindings.append("raw");
	_printFormat = "%p";
	_scanFormat = "%p";
	_typeString = "raw";
	_maxStringLength = 64;
}

daeResolverType::daeResolverType()
{
	_size = sizeof(daeURI);
	_alignment = sizeof(daeURI);
	_typeEnum = ResolverType;
	_nameBindings.append("resolver");
	_nameBindings.append("xsAnyURI");
	_printFormat = "%s";
	_scanFormat = "%s";
	_typeString = "resolver";
}
daeIDResolverType::daeIDResolverType()
{
	_size = sizeof(daeIDRef);
	_alignment = sizeof(daeIDRef);
	_typeEnum = IDResolverType;
	_nameBindings.append("xsIDREF");
	_nameBindings.append("xsIDREFS");
	_printFormat = "%s";
	_scanFormat = "%s";
	_typeString = "idref_resolver";
}

daeBool daeIntType::memoryToString(daeChar* src, std::ostringstream& dst) {
	dst << *(daeInt*)src;
	return true;
}

daeBool daeLongType::memoryToString(daeChar* src, std::ostringstream& dst) {
	dst << *(daeLong*)src;
	return true;
}

daeBool daeShortType::memoryToString(daeChar* src, std::ostringstream& dst) {
	dst << *(daeShort*)src;
	return true;
}

daeBool daeUIntType::memoryToString(daeChar* src, std::ostringstream& dst) {
	dst << *(daeUInt*)src;
	return true;
}

daeBool daeULongType::memoryToString(daeChar* src, std::ostringstream& dst) {
#ifdef WIN32
	// Microsoft's stringstream implementation has weird performance issues
	static char buffer[64];
	_snprintf(buffer, 64, _printFormat, *((daeULong*)src));
	dst << buffer;
#else
	dst << *(daeULong*)src;
#endif
	return true;
}

daeBool daeFloatType::memoryToString(daeChar* src, std::ostringstream& dst) {
	if ( *(daeFloat*)src != *(daeFloat*)src ) // NAN
		dst << "NaN";
	else if ( *(daeUInt*)src == 0x7f800000 ) // +INF
		dst << "INF";
	else if ( *(daeUInt*)src == 0xff800000 ) // -INF
		dst << "-INF";
	else
		dst << *(daeFloat*)src;
	return true;
}

daeBool
daeFloatType::stringToMemory(daeChar *src, daeChar* dstMemory)
{
	src = skipWhitespace(src);

	if ( strncmp(src, "NaN", 3) == 0 ) {
		daeErrorHandler::get()->handleWarning("NaN encountered while setting an attribute or value\n");
		*(daeInt*)(dstMemory) = 0x7f800002;
	}
	else if ( strncmp(src, "INF", 3) == 0 ) {
		daeErrorHandler::get()->handleWarning( "INF encountered while setting an attribute or value\n" );
		*(daeInt*)(dstMemory) = 0x7f800000;
	}
	else if ( strncmp(src, "-INF", 4) == 0 ) {
		daeErrorHandler::get()->handleWarning( "-INF encountered while setting an attribute or value\n" );
		*(daeInt*)(dstMemory) = 0xff800000;
	}
	else
	{
		sscanf(src, _scanFormat, dstMemory);
	}
	return true;
}

daeBool daeDoubleType::memoryToString(daeChar* src, std::ostringstream& dst) {
	if ( *(daeDouble*)src != *(daeDouble*)src ) // NAN
		dst << "NaN";
	else if ( *(daeULong*)src == 0x7ff0000000000000LL ) // +INF
		dst << "INF";
	else if ( *(daeULong*)src == 0xfff0000000000000LL ) // -INF
		dst << "-INF";
	else {
#ifdef WIN32
		// Microsoft's stringstream implementation has weird performance issues
		static char buffer[64];
		_snprintf(buffer, 64, _printFormat, *((daeDouble*)src));
		dst << buffer;
#else
		dst << *(daeDouble*)src;
#endif
	}
	return true;
}

daeBool
daeDoubleType::stringToMemory(daeChar *src, daeChar* dstMemory)
{
	src = skipWhitespace(src);

	if ( strncmp(src, "NaN", 3) == 0 ) {
		daeErrorHandler::get()->handleWarning( "NaN encountered while setting an attribute or value\n" );
		*(daeLong*)(dstMemory) = 0x7ff0000000000002LL;
	}
	else if ( strncmp(src, "INF", 3) == 0 ) {
		daeErrorHandler::get()->handleWarning( "INF encountered while setting an attribute or value\n" );
		*(daeLong*)(dstMemory) = 0x7ff0000000000000LL;
	}
	else if ( strncmp(src, "-INF", 4) == 0 ) {
		daeErrorHandler::get()->handleWarning( "-INF encountered while setting an attribute or value\n" );
		*(daeLong*)(dstMemory) = 0xfff0000000000000LL;
	}
	else
	{
		sscanf(src, _scanFormat, dstMemory);
	}
	return true;
}

daeBool daeRawRefType::memoryToString(daeChar* src, std::ostringstream& dst) {
	dst << (void *)(*((daeRawRef*)src));
	return true;
}

daeBool daeStringRefType::memoryToString(daeChar* src, std::ostringstream& dst) {
	daeString s = *((daeStringRef *)src);
	if (s)
		dst << s;
	return true;
}

namespace {
	// String replace function. Usage: replace("abcdef", "cd", "12") --> "ab12ef"
	std::string replace(const std::string& s, const std::string& replace, const std::string& replaceWith) {
		if (replace.empty())
			return s;
	
		std::string result;
		size_t pos1 = 0, pos2 = s.find(replace);
		while (pos2 != std::string::npos) {
			result += s.substr(pos1, pos2-pos1);
			result += replaceWith;
			pos1 = pos2 + replace.length();
			pos2 = s.find(replace, pos1);
		}

		result += s.substr(pos1, s.length()-pos1);
		return result;
	}
}

daeBool daeResolverType::memoryToString(daeChar* src, std::ostringstream& dst) {
	// Get the URI we are trying to write
	daeURI *thisURI = ((daeURI *)src);
	std::string s;

	// !!!GAC We may want to re-resolve the URI before writing, if so call thisURI->resolveURI() here
	// !!!GAC if you're willing to trust that everything is properly resolved, this isn't needed
	
	// Was this URI successfully resolved ?  (if element or collection is null, we can't write the URI correctly)
	if(thisURI->getState() != daeURI::uri_success || !(thisURI->getElement()) || !(thisURI->getContainer()))
	{
		// This URI was never successfully resolved, so write out it's original value
		s = thisURI->getOriginalURI() ? thisURI->getOriginalURI() : "";
	}
	else
	{
		// This URI was successfully resolved, we need to determine if it is referencing this document (the one being written)
		// or some other document so we know what URI to write out.
		// !!!GAC this approach should be safe, if the collection pointer of our document matches the collection pointer 
		// !!!GAC of the element our URI is pointing at, we are pointing at our own doc.
		if(thisURI->getElement()->getDocument() == thisURI->getContainer()->getDocument())
		{
			// we will send back the original URI if we're pointing at ourselves
			s = thisURI->getOriginalURI() ? thisURI->getOriginalURI() : "";
		}
		else
		{
			// we will send back the full resolved URI
			s = thisURI->getURI() ? thisURI->getURI() : "";
		}
	}

	// Encode spaces with %20
	dst << replace(s, " ", "%20");
	return true;
}

daeBool daeIDResolverType::memoryToString(daeChar* src, std::ostringstream& dst) {
	dst << ((daeIDRef *)src)->getID();
	return true;
}

void
daeAtomicType::resolve(daeElementRef element, daeChar* src)
{
	// just to remove the warnings 
	(void)element;
	(void)src; 
}

void
daeResolverType::resolve(daeElementRef element, daeChar* src)
{
	daeURI* resolver = (daeURI*)src;
	resolver->setContainer(element);
	resolver->resolveElement();
}

daeBool
daeResolverType::stringToMemory(daeChar* src, daeChar* dstMemory)
{
#define MAX_PATH 1024
	daeChar tempstr[MAX_PATH];
	memset(tempstr,0,MAX_PATH);
	daeChar* s;
	daeChar* t;
	for(s=src, t=tempstr; *s!=0; s++,t++)
	{
		if (*s == '%') {
			if ((*(s+1) == '2') && (*(s+2) == '0'))
			{
				(*t)=' ';
				s+=2;
				continue;
			}
		} else if (*s == ' ') {
			char err[512];
			memset( err, 0, 512 );
			sprintf(err,"uri contains white space, dom will convert them to %%20 in output files!\n  uri=%s", src);
			daeErrorHandler::get()->handleWarning( err );
		}
		*t=*s;
	}
	((daeURI*)dstMemory)->setURI(tempstr);
	return true;
}
void
daeIDResolverType::resolve(daeElementRef element, daeChar* src)
{
	daeIDRef* resolver = (daeIDRef*)src;
	resolver->setContainer( element );
}

daeBool
daeIDResolverType::stringToMemory(daeChar* src, daeChar* dstMemory)
{
	src = skipWhitespace(src);
	daeChar* id = extractToken(src);
	((daeIDRef*)dstMemory)->setID(id);
	if (id != src)
		delete[] id;
	return true;
}

daeBool
daeStringRefType::stringToMemory(daeChar* srcChars, daeChar* dstMemory)
{
	*((daeStringRef*)dstMemory) = srcChars;
	return true;
}

daeBool
daeTokenType::stringToMemory(daeChar* src, daeChar* dst)
{
	src = skipWhitespace(src);
	daeChar* srcTmp = extractToken(src);
	*((daeStringRef*)dst) = srcTmp;
	if (srcTmp != src)
		delete[] srcTmp;
	return true;
}

daeBool
daeEnumType::stringToMemory(daeChar* src, daeChar* dst )
{
	src = skipWhitespace(src);
	daeChar* srcTmp = extractToken(src);

	size_t index(0); 
	if ( _strings->find(srcTmp,index) == DAE_ERR_QUERY_NO_MATCH ) return false;
	daeEnum val = _values->get( index );
	*((daeEnum*)dst) = val;

	if (srcTmp != src)
		delete[] srcTmp;

	return true;
}

daeBool daeEnumType::memoryToString(daeChar* src, std::ostringstream& dst) {
	daeStringRef s = "unknown";
	if (_strings != NULL) {
		size_t index;
		if (_values->find(*((daeEnum*)src), index) == DAE_OK)
			s = _strings->get(index);
	}
	dst << (const char*)s;
	return true;
}

daeBool
daeBoolType::stringToMemory(daeChar* srcChars, daeChar* dstMemory)
{
	if (strncmp(srcChars,"true",4)==0 || strncmp(srcChars,"1",1)==0)
		*((daeBool*)dstMemory) = true;
	else
		*((daeBool*)dstMemory) = false;
	return true;
}

daeBool daeBoolType::memoryToString(daeChar* src, std::ostringstream& dst) {
	if (*((daeBool*)src))
		dst << "true";
	else
		dst << "false";
	return true;
}
//!!!ACL added for 1.4 complex types and groups

// Unimplemented
daeBool daeElementRefType::memoryToString(daeChar* src, std::ostringstream& dst) {
	(void)src;
	(void)dst;
	return false;
}

daeMemoryRef daeBoolType::create() {
	return (daeMemoryRef)new daeBool;
}

daeMemoryRef daeIntType::create() {
	return (daeMemoryRef)new daeInt;
}

daeMemoryRef daeLongType::create() {
	return (daeMemoryRef)new daeLong;
}

daeMemoryRef daeUIntType::create() {
	return (daeMemoryRef)new daeUInt;
}

daeMemoryRef daeULongType::create() {
	return (daeMemoryRef)new daeULong;
}

daeMemoryRef daeShortType::create() {
	return (daeMemoryRef)new daeShort;
}

daeMemoryRef daeFloatType::create() {
	return (daeMemoryRef)new daeFloat;
}

daeMemoryRef daeDoubleType::create() {
	return (daeMemoryRef)new daeDouble;
}

daeMemoryRef daeStringRefType::create() {
	return (daeMemoryRef)new daeStringRef;
}

daeMemoryRef daeTokenType::create() {
	return (daeMemoryRef)new daeStringRef;
}

daeMemoryRef daeElementRefType::create() {
	return (daeMemoryRef)new daeElementRef;
}

daeMemoryRef daeEnumType::create() {
	return (daeMemoryRef)new daeEnum;
}

daeMemoryRef daeRawRefType::create() {
	return (daeMemoryRef)new daeRawRef;
}

daeMemoryRef daeResolverType::create() {
	return (daeMemoryRef)new daeURI;
}

daeMemoryRef daeIDResolverType::create() {
	return (daeMemoryRef)new daeIDRef;
}


void daeBoolType::destroy(daeMemoryRef obj) {
	delete (daeBool*)obj;
}

void daeIntType::destroy(daeMemoryRef obj) {
	delete (daeInt*)obj;
}

void daeLongType::destroy(daeMemoryRef obj) {
	delete (daeLong*)obj;
}

void daeUIntType::destroy(daeMemoryRef obj) {
	delete (daeUInt*)obj;
}

void daeULongType::destroy(daeMemoryRef obj) {
	delete (daeULong*)obj;
}

void daeShortType::destroy(daeMemoryRef obj) {
	delete (daeShort*)obj;
}

void daeFloatType::destroy(daeMemoryRef obj) {
	delete (daeFloat*)obj;
}

void daeDoubleType::destroy(daeMemoryRef obj) {
	delete (daeDouble*)obj;
}

void daeStringRefType::destroy(daeMemoryRef obj) {
	delete (daeStringRef*)obj;
}

void daeTokenType::destroy(daeMemoryRef obj) {
	delete (daeStringRef*)obj;
}

void daeElementRefType::destroy(daeMemoryRef obj) {
	delete (daeElementRef*)obj;
}

void daeEnumType::destroy(daeMemoryRef obj) {
	delete (daeEnum*)obj;
}

void daeRawRefType::destroy(daeMemoryRef obj) {
	delete (daeRawRef*)obj;
}

void daeResolverType::destroy(daeMemoryRef obj) {
	delete (daeURI*)obj;
}

void daeIDResolverType::destroy(daeMemoryRef obj) {
	delete (daeIDRef*)obj;
}


daeInt
daeStringRefType::compare(daeChar* value1, daeChar* value2) {
	daeString s1 = *((daeStringRef *)value1);
	daeString s2 = *((daeStringRef *)value2);
	return strcmp(s1, s2);
}

daeInt daeIDResolverType::compare(daeChar* value1, daeChar* value2) {
	return (daeIDRef&)*value1 == (daeIDRef&)*value2;
}


daeArray* daeBoolType::createArray() {
	return new daeTArray<daeBool>;
}

daeArray* daeIntType::createArray() {
	return new daeTArray<daeInt>;
}

daeArray* daeLongType::createArray() {
	return new daeTArray<daeLong>;
}

daeArray* daeUIntType::createArray() {
	return new daeTArray<daeUInt>;
}

daeArray* daeULongType::createArray() {
	return new daeTArray<daeULong>;
}

daeArray* daeShortType::createArray() {
	return new daeTArray<daeShort>;
}

daeArray* daeFloatType::createArray() {
	return new daeTArray<daeFloat>;
}

daeArray* daeDoubleType::createArray() {
	return new daeTArray<daeDouble>;
}

daeArray* daeStringRefType::createArray() {
	return new daeTArray<daeStringRef>;
}

daeArray* daeTokenType::createArray() {
	return new daeTArray<daeStringRef>;
}

daeArray* daeElementRefType::createArray() {
	return new daeTArray<daeElementRef>;
}

daeArray* daeEnumType::createArray() {
	return new daeTArray<daeEnum>;
}

daeArray* daeRawRefType::createArray() {
	return new daeTArray<daeRawRef>;
}

daeArray* daeResolverType::createArray() {
	return new daeTArray<daeURI>;
}

daeArray* daeIDResolverType::createArray() {
	return new daeTArray<daeIDRef>;
}


void daeBoolType::copy(daeChar* src, daeChar* dst) {
	(daeBool&)*dst = (daeBool&)*src;
}

void daeIntType::copy(daeChar* src, daeChar* dst) {
	(daeInt&)*dst = (daeInt&)*src;
}

void daeLongType::copy(daeChar* src, daeChar* dst) {
	(daeLong&)*dst = (daeLong&)*src;
}

void daeUIntType::copy(daeChar* src, daeChar* dst) {
	(daeUInt&)*dst = (daeUInt&)*src;
}

void daeULongType::copy(daeChar* src, daeChar* dst) {
	(daeULong&)*dst = (daeULong&)*src;
}

void daeShortType::copy(daeChar* src, daeChar* dst) {
	(daeShort&)*dst = (daeShort&)*src;
}

void daeFloatType::copy(daeChar* src, daeChar* dst) {
	(daeFloat&)*dst = (daeFloat&)*src;
}

void daeDoubleType::copy(daeChar* src, daeChar* dst) {
	(daeDouble&)*dst = (daeDouble&)*src;
}

void daeStringRefType::copy(daeChar* src, daeChar* dst) {
	(daeStringRef&)*dst = (daeStringRef&)*src;
}

void daeTokenType::copy(daeChar* src, daeChar* dst) {
	(daeStringRef&)*dst = (daeStringRef&)*src;
}

void daeElementRefType::copy(daeChar* src, daeChar* dst) {
	(daeElementRef&)*dst = (daeElementRef&)*src;
}

void daeEnumType::copy(daeChar* src, daeChar* dst) {
	(daeEnum&)*dst = (daeEnum&)*src;
}

void daeRawRefType::copy(daeChar* src, daeChar* dst) {
	(daeRawRef&)*dst = (daeRawRef&)*src;
}

void daeResolverType::copy(daeChar* src, daeChar* dst) {
	(daeURI&)*dst = (daeURI&)*src;
}

void daeIDResolverType::copy(daeChar* src, daeChar* dst) {
	(daeIDRef&)*dst = (daeIDRef&)*src;
}