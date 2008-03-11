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

#include <dae.h>
#include <dae/daeURI.h>
#include <ctype.h>
#include <dae/daeDocument.h>
#include <dae/daeErrorHandler.h>
#include <dae/daeUtils.h>
#include <pcrecpp.h>

#ifdef _WIN32
#include <direct.h>  // for getcwd (windows)
#else
#include <unistd.h>  // for getcwd (linux)
#endif

using namespace std;

daeString safeCreate(daeString src);
void safeDelete(daeString src);
daeString findCharacterReverse(daeString str, daeChar stopChar);

void
daeURI::initialize()
{
	// Initialize a URI to it's empty state, same as daeURI::reset but also clears out "container"

	uriString			= NULL;
	originalURIString	= NULL;
	protocol			= NULL;
	authority			= NULL;
	filepath			= NULL;
	file				= NULL;
	id					= NULL;
	extension			= NULL;
	state				= uri_empty;
	element				= NULL;
	container			= NULL;
	external			= false;
}

daeURI::~daeURI()
{
	reset();
}

daeURI::daeURI(DAE& dae, bool cwdUri) : dae(&dae)
{
	if (!cwdUri) {
		initialize();
	}
	else {
		// Build a uri pointing to the current working directory. This works for Windows, Linux,
		// and Mac, but not for PS3 since it doesn't have a getcwd call.
		// !!!GAC the buffers should probably be bigger
		char buffer[1024], *b1;
		strcpy(buffer, "file:///");
#ifdef __CELLOS_LV2__
		// The PS3 has no getcwd call, so leave the value as file:///
#else
#ifdef _WIN32
		// Windows getcwd always returns a path beginning with a drive letter, so we add file:/// to the beginning 
		_getcwd(&buffer[8],1024-8);
#else
		// Linux getcwd always returns a path beginning with a slash, so we add file:// to the beginning
		getcwd(&buffer[7],1024-7);
#endif
#endif
		// If the path contains windows backslashes, flip them to forward slashes
		for(b1 = buffer;*b1 != 0;b1++)
			if(*b1 == '\\')
					*b1 = '/';
		// The path must end in a slash or the last part of it will be taken as a filename
		if(*(b1-1) != '/')
			*(b1++) = '/';
		*b1 = '\0';
		initialize();
		setURI(buffer);
		validate();
	}
}

daeURI::daeURI(DAE& dae, daeString uriString, daeBool nofrag) : dae(&dae)
{
	initialize();
	// !!!GAC this is inefficient as hell, but was the best way to isolate this functionality till the
	// !!!GAC daeURI class can be written to support modifying URIs better (should be possible to make a URI,
	// !!!GAC change any member and have getURI return the proper const string URI)
	if(nofrag)
	{
		// Strip off the fragment part before constructing the URI
		daeString temp		= safeCreate(uriString);
		daeChar* fragment	= (daeChar*)findCharacterReverse(temp, '#');
		if(fragment)
		{
			*fragment = 0;
		}
		setURI(temp);
		safeDelete(temp);
	}
	else
	{
		// Generate the URI without changing the string
		setURI(uriString);
	}
	validate();
}

daeURI::daeURI(daeURI& baseURI, daeString uriString) : dae(baseURI.getDAE())
{
	initialize();
	setURI(uriString);
	validate(&baseURI);
}

daeURI::daeURI(const daeURI& copyFrom) : dae(copyFrom.getDAE())
{
	initialize();
	setURI(copyFrom.getOriginalURI());
	element = copyFrom.element;   // !!!GAC SetURI immediately clears element so we must do this after
	state = copyFrom.state;
}

daeURI::daeURI(daeElement& container_, daeString uriString)
	: dae(container_.getDAE())
{
	initialize();
	container = &container_;
	setURI(uriString);
	validate();
}

daeURI::daeURI(DAE& dae, daeElement& container_, daeString uriString)
	: dae(&dae)
{
	initialize();
	container = &container_;
	setURI(uriString);
	validate();
}

void
daeURI::copyFrom(daeURI& copyFrom)
{
	setURI(copyFrom.getOriginalURI());
	element = copyFrom.element;		// !!!GAC SetURI immediately clears element so we must do this after
	state = copyFrom.state;
	// !!!GAC Should there be a call to validate in here?
}

daeURI& daeURI::operator=(daeString uri) {
	setURI(uri);
	return *this;
}

void
daeURI::reset()
{
	// Free up any allocated memory

	if (uriString != NULL)
		safeDelete(uriString);

	if (originalURIString != NULL)
		safeDelete(originalURIString);

	if (protocol != NULL)
		safeDelete(protocol);

	if (authority != NULL)
		safeDelete(authority);

	if (filepath != NULL)
		safeDelete(filepath);

	if (file != NULL)
		safeDelete(file);

	if (id != NULL)
		safeDelete(id);
	
	if (extension != NULL)
		safeDelete(extension);

	// Set everything to the empty string

	uriString			= NULL;
	originalURIString	= NULL;
	protocol			= NULL;
	authority				= NULL;
	filepath			= NULL;
	file				= NULL;
	id					= NULL;
	extension			= NULL;

	state				= uri_empty;
	element				= NULL;
//	container = NULL;   // !!!GAC don't want to clear this, our container doesn't change once it's set
}

DAE* daeURI::getDAE() const {
	return dae;
}

daeString
findCharacterReverse(daeString str, daeChar stopChar)
{
	if (str == NULL)
		return NULL;
	daeString cur = str + strlen(str)-1;
	while((cur >= str) && (*cur != stopChar))
		cur--;
	
	if ((cur >= str) && (*cur == stopChar))
		return cur;
	
	return NULL;
}

daeString
findCharacter(daeString str, daeChar stopChar)
{
	if (str == NULL)
		return NULL;
	daeString end = str + strlen(str);
	daeString cur = str;
	while((*cur != stopChar) && (cur < end))
		cur++;
	
	if (*cur == stopChar)
		return cur;
	
	return NULL;
}

daeString safeCreate(daeString src)
{
	if (src == NULL)
		return NULL;
	daeChar* ret = (daeChar*)daeMemorySystem::malloc("uri",strlen(src)+1);
	if (ret == NULL)
		return NULL;
	strcpy(ret,src);
	
	return ret;
}

void safeDelete(daeString src)
{
	if(src != NULL)
	{
		daeMemorySystem::free("uri",(void*)src);
		src = NULL; 
	}
	
}

void
daeURI::setURI(daeString _URIString)
{
//Contributed by Nus - Wed, 08 Nov 2006
  // Nus: Checking for existing string.
  if(originalURIString && _URIString) {
    if(strcmp(originalURIString, _URIString) == 0)
      return;
  }
  // Nus: If the string exist, delete it first.
  if(originalURIString) {
    safeDelete(originalURIString);
    originalURIString = NULL;
  }
//---------------------------
	originalURIString = safeCreate(_URIString);
	internalSetURI(_URIString);
}

daeChar *validScheme(daeString uri_string)
{
	// attempt to find a valid scheme in a string
	// Failure to find a scheme returns a NULL, success returns the position of the terminating :
	
	// First character must be alpha, fail if it's not
	if(!isalpha(*uri_string))
		return(NULL);

	// Advance to the next character
	uri_string++;

	// Scheme must be at least two (valid) characters long, so go through this loop at least once
	do
	{
		// If the character is NOT alpha, digit, +, - or . then this isn't a valid scheme
		// Note this also fails if we encounter a null terminator before hitting the first :
		if(!(isalpha(*uri_string) || isdigit(*uri_string) || *uri_string == '.' || *uri_string == '+' || *uri_string == '-'))
			return(NULL);
		uri_string++;
	} while(*uri_string != ':' );
	
	return((daeChar *)uri_string);
}

void
daeURI::internalSetURI(daeString _URIString)
{
	// Store the originalURI so you can fix it post Reset
	daeString oURI = originalURIString;
	originalURIString = NULL;

	// Reset everything
	reset();

	// Fix original URI String
	originalURIString = oURI;
	
	uriString = safeCreate(_URIString);

	daeChar* tmp = (daeChar*)safeCreate(uriString);
	if ((uriString == NULL)||(tmp == NULL))
		return;
	
	daeChar* curSrc = tmp;
	
#if 1
	// Check for a scheme, two or more characters followed by a :
	
//	daeChar* colon = (daeChar*)findCharacter(curSrc,':');
	
	daeChar* colon = validScheme(curSrc);

//	if(colon && (colon-tmp >= 2 ))
	if(colon)
	{
		// Found a scheme, remember it (protocol should be named scheme) 
		*colon = '\0';
		protocol = safeCreate(curSrc);
		// Advance the current pointer past the colon
		curSrc = colon+1;
	}

	// Check for a net path containing an authority, this would begin with //

	if(curSrc[0] == '/' && curSrc[1] == '/')
	{
		// Advance past the double slash to where the authority would start, then find the next slash
		curSrc = curSrc + 2;
		daeChar* slash = (daeChar*)findCharacter(curSrc,'/');
		// Temporarily remove that slash (avoids some branches)
		if ( slash != NULL ) {
			*slash = '\0';
		}
		// Save the text between the slashes as the authority 
		authority = safeCreate(curSrc);
		// Put the slash back and advance the current pointer to it, this puts us at the start of the path
		if (slash != NULL ) {
			*slash = '/';
			curSrc = slash;
		}
	}

	// Search backwards from the end of the URI for the # which denotes the fragment (called ID here)
	daeChar* idSymbol = (daeChar*)findCharacterReverse(curSrc,'#');
	if (idSymbol != NULL)
	{
		// There was a fragment, isolate it by changing the # to a null
		*idSymbol = '\0';
		idSymbol++;
	}
	id = safeCreate(idSymbol);

	// Search backwards for the last / in the path, everything after is the filename

	daeChar* fname = (daeChar*)findCharacterReverse(curSrc,'/');
	daeChar* dir;
	if (fname == NULL)
	{
		// No / found, so the whole thing is the file name and there is no path
		if ( protocol == NULL )
		{
			fname = curSrc;
		}
		dir = NULL;
	}
	else 
	{
		// Found a slash, so the filename starts after it and dir starts at curSrc
		fname++;
		dir = curSrc;
	}
	file = safeCreate(fname);

	// Pull the extension (if any) off of the filepath
	
	daeString extStr = findCharacterReverse(fname,'.');
	if (extStr != NULL)
		extension = safeCreate(extStr+1);

	// Now pull off the directory path if it exists by putting a zero at the beginning of fname, this insures filepath will end in a slash
	
	if(fname != NULL) *fname = 0;
	filepath = safeCreate(dir);
	
	state = uri_loaded;
	daeMemorySystem::free("tmp",tmp);

#else
	daeBool isAbsolute;
	daeChar* colon = (daeChar*)findCharacter(curSrc,':');

	// IS ABSOLUTE REFERENCE
	if (colon && (strlen(colon) > 2) &&	(colon[1] == '/') && (colon[2] == '/')) 
	{
		*colon = '\0';
		protocol = safeCreate(curSrc);
		curSrc = colon+3;
		daeString hosttmp = curSrc;
		daeChar* slash = (daeChar*)findCharacter(curSrc,'/');
		if (slash != NULL) 
		{
			*slash = '\0';
			authority = safeCreate(hosttmp);
			curSrc = slash+1;
		}
		isAbsolute = true;
	}
	else {
		protocol = NULL;
		isAbsolute = false;
	}

	// Look for the # which denotes the fragment (called ID here)
	daeChar* idSymbol = (daeChar*)findCharacterReverse(curSrc,'#');
	if (idSymbol != NULL)
	{
		// There was a fragment, isolate it by changing the # to a null
		*idSymbol = '\0';
		idSymbol++;
	}

	daeChar* dir = NULL;
	daeChar* fname = (daeChar*)findCharacterReverse(curSrc,'/');
	if (fname == NULL) 
		fname = curSrc;
	else {
		*fname = '\0';
		fname++;
		dir = curSrc;
	}

	filepath = safeCreate(dir);
	int dirLen = (int)strlen(filepath);

	// append a '/'
	if ((filepath != NULL) && (dirLen > 0) &&
		(filepath[dirLen-1] != '/')) {
		daeMemorySystem::free("uri",(void*)filepath);
		filepath =	(daeString)daeMemorySystem::malloc("uri", dirLen+2);
		strcpy((daeChar*)filepath,dir);
		*((daeChar*)filepath+dirLen) = '/';
		*((daeChar*)filepath+dirLen+1) = '\0';
	}

	file = safeCreate(fname);
	id = safeCreate(idSymbol);

	daeString extStr = findCharacterReverse(fname,'.');
	if (extStr != NULL)
		extension = safeCreate(extStr+1);
	
	state = uri_loaded;
	daeMemorySystem::free("tmp",tmp);
#endif
}

void
daeURI::print()
{
	fprintf(stderr,"URI(%s)\n",uriString);
	fprintf(stderr,"protocol = %s\n",protocol);
	fprintf(stderr,"authority = %s\n",authority);
	fprintf(stderr,"path = %s\n",filepath);
	fprintf(stderr,"file = %s\n",file);
	fprintf(stderr,"id = %s\n",id);
	fprintf(stderr,"URI without base = %s\n",originalURIString);
	fflush(stderr);
}

const char* protoString = "://";
const char* hostString = "/";
const char* queryString = "#";
const char* filepathString = "/";

daeString
daeURI::getURI() const
{
	return uriString;
}

daeString
daeURI::getOriginalURI() const
{
	return originalURIString;
}

void
daeURI::validate(daeURI* baseURI)
{
	// If no base URI was supplied, get the application base and use it
	if (baseURI == NULL)
	{
		if ( container == NULL || (baseURI = container->getDocumentURI()) == NULL ) {
			baseURI = &dae->getBaseURI();
		}
		if (this == baseURI ) {
			return;
		}
	}

	// This is rewritten according to the updated rfc 3986
	if((protocol != NULL) && (strlen(protocol)>0))  // if defined(R.scheme) then
	{
		// Everything stays the same except path which we normalize
        // T.scheme    = R.scheme;
        // T.authority = R.authority;
        // T.path      = remove_dot_segments(R.path);
        // T.query     = R.query;
		normalizeURIPath((char *)filepath);
		if ( (file == NULL) || (strlen(file)==0) ) {
			//safeDelete(file);
			//safeDelete(extension);
			//file		= safeCreate(baseURI->file);
			//extension	= safeCreate(baseURI->extension);
		}
	}
	else
	{
		if((authority != NULL) && (strlen(authority)>0)) // if defined(R.authority) then
		{
			// Authority and query stay the same, path is normalized
            // T.authority = R.authority;
            // T.path      = remove_dot_segments(R.path);
            // T.query     = R.query;
			normalizeURIPath((char *)filepath);
			if ( (file == NULL) || (strlen(file)==0) ) {
				//safeDelete(file);
				//safeDelete(extension);
				//file		= safeCreate(baseURI->file);
				//extension	= safeCreate(baseURI->extension);
			}
		}
		else
		{
			if(((filepath == NULL) || (strlen(filepath)==0)) && ((file == NULL) || (strlen(file)==0)))  // if (R.path == "") then
			{
                // T.path = Base.path;
				safeDelete(filepath);
				safeDelete(file);
				safeDelete(extension);
				filepath	= safeCreate(baseURI->filepath);
				file		= safeCreate(baseURI->file);
				extension	= safeCreate(baseURI->extension);
				// We don't handle querys, but if we did
				//if defined(R.query) then
				//   T.query = R.query;
				//else
				//   T.query = Base.query;
				//endif;
			}
			else
			{
				if((filepath != NULL) && (*filepath == '/'))  //if (R.path starts-with "/") then
				{
					// T.path = remove_dot_segments(R.path);
					normalizeURIPath((char *)filepath);
				}
				else
				{
					//T.path = merge(Base.path, R.path);
					daeChar* newPath;
					if((strlen(baseURI->authority) != 0) && (strlen(baseURI->filepath)==0) && (strlen(baseURI->file) == 0)) //authority defined, path empty
					{
						newPath = (daeChar*)daeMemorySystem::malloc("uri", strlen(filepath) + 2);
						*newPath = '/';
						*(newPath+1) = 0;
						strcat(newPath,filepath);
					}
					else
					{
						size_t l = 0;
						if ( filepath != NULL ) {
							l = strlen(filepath);
						}
						newPath = (daeChar*)daeMemorySystem::malloc("uri", strlen(baseURI->filepath) + l + 1);
						*newPath = 0;
						strcat(newPath,baseURI->filepath);
						if ( filepath != NULL ) {
							strcat(newPath,filepath);
						}
						else {
							strcat(newPath,"");
						}
					}
					//T.path = remove_dot_segments(T.path);
					normalizeURIPath(newPath);
					safeDelete(filepath);
					filepath = newPath;
				}
               // T.query = R.query;
			}
			// T.authority = Base.authority;
			safeDelete(authority);
			authority = safeCreate(baseURI->authority);
		}
		// T.scheme = Base.scheme;
		safeDelete(protocol);
		protocol = safeCreate(baseURI->protocol);
	}
	// T.fragment = R.fragment;

	// Now for the purpose of maintaining the class members, we reassemble all this into a string version of the URI
	size_t len = 0;
	if ( protocol != NULL ) {
		len += strlen(protocol);
	}
	if ( authority != NULL ) {
		len += strlen(authority);
	}
	if ( filepath != NULL ) {
		len += strlen(filepath);
	}
	if ( file != NULL ) {
		len += strlen(file);
	}
	if ( queryString != NULL ) {
		len += strlen(queryString);
	}
	if ( id != NULL ) {
		len += strlen(id);
	}
	daeChar* newURI = (daeChar*)
		daeMemorySystem::malloc("uri", len + 5 );
	*newURI = 0;

	if(protocol != NULL && *protocol != 0)
	{
		strcat(newURI, protocol);
		strcat(newURI, ":");
	}
	strcat(newURI, "//");
	if(authority != NULL && *authority != 0)
	{
		strcat(newURI, authority);
	}	
	if(filepath != NULL)
		strcat(newURI, filepath);
	if(file != NULL)
		strcat(newURI, file);

	if(id != NULL && *id != 0)
	{
		strcat(newURI,"#");
		strcat(newURI,id);
	}
	// This becomes the new uriString, no need to call internalSetUri because all the class members are up to date
	safeDelete(uriString);
	uriString = newURI;
	state = uri_pending;

	if ( container != NULL && container->getDocumentURI() != NULL && filepath != NULL && file != NULL ) {
	    daeString fp = container->getDocumentURI()->getFilepath();
	    daeString f = container->getDocumentURI()->getFile();
		if ( strcmp( fp, filepath ) != 0 || strcmp( f, file ) != 0 ) {
			//external reference
			container->getDocument()->addExternalReference( *this );
			external = true;
		}
		else if ( external ) {
			//was external but now isn't
			container->getDocument()->removeExternalReference( *this );
			external = false;
		}
	}
}

daeElementRef daeURI::getElement() {
	if (!element)
		internalResolveElement();
	return element;
}

void daeURI::internalResolveElement() {
	if (state == uri_empty)
		return;
	
	if (state == uri_loaded) {
		if (container != NULL)
			validate(container->getDocumentURI());
		else
			validate();
	}
	dae->getURIResolvers().resolveElement(*this);
}

void daeURI::resolveElement() { }

void
daeURI::resolveURI()
{
	// !!!GAC bug 486, there used to be code here that just returned if state was uri_empty or uri_resolve_local this has been removed.
	if (getElement() != NULL)
	{
		// !!!GAC bug 388 and 421, need to add a fragment (#) before the ID (was setURI(element->getID()))
		if(element->getID() == NULL || element->getID()[0] == 0)
		{
			// We can't resolve to an element that has no ID, so if the ID is blank, fail and return
			state = uri_failed_invalid_reference;
			return;
		}
		daeChar* newID = (daeChar*)daeMemorySystem::malloc("tmp",strlen(element->getID())+2);
		strcpy(newID,"#");
		strcat(newID,element->getID());
		// !!!GAC We have to save element and container because setURI clears them for some reason
		daeElementRef	elementSave = element;
		setURI(newID);
		// !!!GAC Hopefully, calling validate like below is the right thing to do to get the full URI resolution
		element		= elementSave;
		validate(element->getDocumentURI());
		element		= elementSave;
		daeMemorySystem::free("tmp",newID);
		state = uri_success;  // !!!GAC The element pointer and the URI should agree now, so set success
	}
	else
	{
		state = uri_failed_invalid_reference;
	}
}

daeBool daeURI::getPath(daeChar *dest, daeInt size) const
{

	if( file == NULL ) 
	{
		//printf("****** %s : %s\n", uriString, originalURIString);
		return false;
	}
    *dest = 0;
	int lenPath = 0;
	if ( filepath != NULL ) lenPath = (int)strlen(filepath);
	int lenFile = (int)strlen(file);

	int len =  lenPath + lenFile;
	if (len < size)
	{
		if ( filepath != NULL ) {
			strcpy(dest,filepath);
		}
		strcat(dest,file);
		return true;
	}
	else 
		return false;
}

// This code is loosely based on the RFC 2396 normalization code from
// libXML. Specifically it does the RFC steps 6.c->6.g from section 5.2
// The path is modified in place, there is no error return.
void daeURI::normalizeURIPath(char *path) 
{
    char 
		*cur, // location we are currently processing
		*out; // Everything from this back we are done with

	// Return if the path pointer is null

	if (path == NULL) return;

	// Skip any initial / characters to get us to the start of the first segment

	for(cur=path; *cur == '/'; cur++);

	// Return if we hit the end of the string

    if (*cur == 0) return;

    // Keep everything we've seen so far.
    
	out = cur;

    // Analyze each segment in sequence for cases (c) and (d).

	while (*cur != 0) 
	{
		// (c) All occurrences of "./", where "." is a complete path segment, are removed from the buffer string.
		
		if ((*cur == '.') && (*(cur+1) == '/')) 
		{
			cur += 2;
			// If there were multiple slashes, skip them too
			while (*cur == '/') cur++;
			continue;
		}

		// (d) If the buffer string ends with "." as a complete path segment, that "." is removed.

		if ((*cur == '.') && (*(cur+1) == 0))
			break;

		// If we passed the above tests copy the segment to the output side

		while (*cur != '/' && *cur != 0)
		{
			*(out++) = *(cur++);
		}

		if(*cur != 0)
		{
			// Skip any occurrances of // at the end of the segment

			while ((*cur == '/') && (*(cur+1) == '/')) cur++;

			// Bring the last character in the segment (/ or a null terminator) into the output
        
			*(out++) = *(cur++);
		}
    }

	*out = 0;

    // Restart at the beginning of the first segment for the next part

	for(cur=path; *cur == '/'; cur++);
    if (*cur == 0) return;

    // Analyze each segment in sequence for cases (e) and (f).
    //
    // e) All occurrences of "<segment>/../", where <segment> is a
    //    complete path segment not equal to "..", are removed from the
    //    buffer string.  Removal of these path segments is performed
    //    iteratively, removing the leftmost matching pattern on each
    //    iteration, until no matching pattern remains.
    //
    // f) If the buffer string ends with "<segment>/..", where <segment>
    //    is a complete path segment not equal to "..", that
    //    "<segment>/.." is removed.
    //
    // To satisfy the "iterative" clause in (e), we need to collapse the
    // string every time we find something that needs to be removed.  Thus,
    // we don't need to keep two pointers into the string: we only need a
    // "current position" pointer.
    //
	bool trew = true;
    while (trew) 
	{
        char *segp, *tmp;

        // At the beginning of each iteration of this loop, "cur" points to
        // the first character of the segment we want to examine.

        // Find the end of the current segment.  
        
		for(segp = cur;(*segp != '/') && (*segp != 0); ++segp);

        // If this is the last segment, we're done (we need at least two
        // segments to meet the criteria for the (e) and (f) cases).

		if (*segp == 0)
			break;

        // If the first segment is "..", or if the next segment _isn't_ "..",
        // keep this segment and try the next one.

        ++segp;
        if (((*cur == '.') && (cur[1] == '.') && (segp == cur+3))
            || ((*segp != '.') || (segp[1] != '.')
            || ((segp[2] != '/') && (segp[2] != 0)))) 
		{
			cur = segp;
			continue;
        }

        // If we get here, remove this segment and the next one and back up
        // to the previous segment (if there is one), to implement the
        // "iteratively" clause.  It's pretty much impossible to back up
        // while maintaining two pointers into the buffer, so just compact
        // the whole buffer now.

        // If this is the end of the buffer, we're done.

        if (segp[2] == 0) 
		{
			*cur = 0;
			break;
        }

        // Strings overlap during this copy, but not in a bad way, just avoid using strcpy
		
		tmp = cur;
		segp += 3;
		while ((*(tmp++) = *(segp++)) != 0);

        // If there are no previous segments, then keep going from here.
        
		segp = cur;
        while ((segp > path) && (*(--segp) == '/'));
        
		if (segp == path)
          continue;

        // "segp" is pointing to the end of a previous segment; find it's
        // start.  We need to back up to the previous segment and start
        // over with that to handle things like "foo/bar/../..".  If we
        // don't do this, then on the first pass we'll remove the "bar/..",
        // but be pointing at the second ".." so we won't realize we can also
        // remove the "foo/..".

		for(cur = segp;(cur > path) && (*(cur-1) != '/'); cur--);
    }

	*out = 0;

	// g) If the resulting buffer string still begins with one or more
	//    complete path segments of "..", then the reference is
	//    considered to be in error. Implementations may handle this
	//    error by retaining these components in the resolved path (i.e.,
	//    treating them as part of the final URI), by removing them from
	//    the resolved path (i.e., discarding relative levels above the
	//    root), or by avoiding traversal of the reference.
	//
	// We discard them from the final path.

    if (*path == '/') 
	{
		for(cur=path; (*cur == '/') && (cur[1] == '.') && (cur[2] == '.') && ((cur[3] == '/') || (cur[3] == 0)); cur += 3);

		if (cur != path) 
		{
			for(out=path; *cur != 0; *(out++) = *(cur++));

			*out = 0;
		}
    }
    return;
}
// This function will take a resolved URI and create a version of it that is relative to
// another existing URI.  The new URI is stored in the "originalURI"
int daeURI::makeRelativeTo(const daeURI* relativeToURI)
{
	if( getState() == uri_empty || relativeToURI->getState() == uri_empty ) 
		return(DAE_ERR_INVALID_CALL);
	if( getState() == uri_loaded )
	{
		if (container != NULL)
			validate(container->getDocumentURI());
		else
			validate();
	}
	if( relativeToURI->getState() == uri_loaded )
	{
		if (relativeToURI->getContainer() != NULL)
			const_cast<daeURI*>(relativeToURI)->validate(relativeToURI->getContainer()->getDocumentURI());
		else
			const_cast<daeURI*>(relativeToURI)->validate();
	}


	// Can only do this function if both URIs have the same scheme and authority

	if((strcmp(getProtocol(), relativeToURI->getProtocol()) != 0) || (strcmp(getAuthority(), relativeToURI->getAuthority()) != 0))
		return(DAE_ERR_INVALID_CALL);  // !!!GAC Need to assign a real error code to this
	
	// advance till we find a segment that doesn't match

	const char *this_filepath		= getFilepath();
	const char *relativeTo_filepath = relativeToURI->getFilepath();
	const char *this_slash			= this_filepath;
	const char *relativeTo_slash	= relativeTo_filepath;

	while((*this_filepath == *relativeTo_filepath) && *this_filepath)
	{
		if(*this_filepath == '/')
		{
			this_slash = this_filepath;
			relativeTo_slash = relativeTo_filepath;
		}
		this_filepath++;
		relativeTo_filepath++;
	}

	// Decide how many ../ segments are needed (Filepath should always end in a /)
	int segment_count = 0;
	relativeTo_slash++;
	while(*relativeTo_slash != 0)
	{
		if(*relativeTo_slash == '/')
			segment_count ++;
		relativeTo_slash++;
	}
	this_slash++;
	// Delete old URI string
	safeDelete(originalURIString);
	// Allocate memory for a new "originalURI" and free the old one
	char *newRelativeURI;
	if ( getID() == NULL )
	{
		newRelativeURI = (char*) daeMemorySystem::malloc("uri",strlen(this_slash)+ strlen(file)+(segment_count*3)+1);
	}
	else
	{
		newRelativeURI = (char*) daeMemorySystem::malloc("uri",strlen(this_slash)+ strlen(file)+(segment_count*3)+strlen(id)+2);
	}
	char *temp = newRelativeURI;
	for(int i = 0; i < segment_count; i++)
	{
		strcpy(temp,"../");
		temp += 3;
	}
	strcpy(temp,this_slash);
	strcat(temp,file);
	if(id!=NULL && strlen(getID()) != 0)
	{
		strcat(temp,"#");
		strcat(temp,getID());
	}
	originalURIString = newRelativeURI;
	return(DAE_OK);
}


daeBool daeURIResolver::_loadExternalDocuments = true;

daeURIResolver::daeURIResolver(DAE& dae) : dae(&dae) { }

daeURIResolver::~daeURIResolver() { }

void daeURIResolver::setAutoLoadExternalDocuments( daeBool load ) 
{ 
	_loadExternalDocuments = load; 
}

daeBool daeURIResolver::getAutoLoadExternalDocuments() 
{ 
	return _loadExternalDocuments; 
}


daeURIResolverList::daeURIResolverList() { }

daeURIResolverList::~daeURIResolverList() {
	for (size_t i = 0; i < resolvers.getCount(); i++)
		delete resolvers[i];
}

void daeURIResolverList::addResolver(daeURIResolver* resolver) {
	resolvers.append(resolver);
}

void daeURIResolverList::removeResolver(daeURIResolver* resolver) {
	resolvers.remove(resolver);
}

void daeURIResolverList::resolveElement(daeURI& uri) {
	for (size_t i = 0; i < resolvers.getCount(); i++) {
		if (resolvers[i]->isProtocolSupported(uri.getProtocol())) {
			if (uri.getFile() == NULL ||
			    uri.getFile()[0] == '\0' ||
			    resolvers[i]->isExtensionSupported(uri.getExtension())) {
				resolvers[i]->resolveElement(uri);
			}
		}
	}
}

void daeURIResolverList::resolveURI(daeURI& uri) {
	bool foundProtocol = false;
	for (size_t i = 0; i < resolvers.getCount(); i++) {
		if (resolvers[i]->isProtocolSupported(uri.getProtocol())) {
			if (resolvers[i]->resolveURI(uri))
				return;
			foundProtocol = true;
		}
	}

	if (!foundProtocol)
		uri.setState(daeURI::uri_failed_unsupported_protocol);
}



string cdom::filePathToUri(const string& filePath) {
	string uri = filePath;

	// Windows - convert "c:\" to "/c:\"
	if (uri.length() >= 2  &&  isalpha(uri[0])  &&  uri[1] == ':')
		uri.insert(0, "/");
	else if (!uri.empty()  &&  uri[0] == '\\') {
		// Windows - If it's an absolute path with no drive letter, or a UNC path,
		// prepend "file:///"
		uri.insert(0, "file:///");
	}
	
	// Windows - convert backslashes to forward slashes
	uri = replace(uri, "\\", "/");

	// Convert spaces to %20
	uri = replace(uri, " ", "%20");

	return uri;
}


namespace {
	// Returns true if parsing succeeded, false otherwise. Parsing can fail if the uri
	// reference isn't properly formed.
	bool parseUriRef(const string& uriRef,
	                 string& scheme,
	                 string& authority,
	                 string& path,
	                 string& query,
	                 string& fragment) {
		// This regular expression for parsing URI references comes from the URI spec:
		//   http://tools.ietf.org/html/rfc3986#appendix-B
		static pcrecpp::RE re("^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?");
		string s1, s3, s6, s8;
		if (re.FullMatch(uriRef, &s1, &scheme, &s3, &authority, &path, &s6, &query, &s8, &fragment))
			return true;

		return false;
	}
}


string cdom::uriToFilePath(const string& uriRef) {
	string scheme, authority, path, query, fragment;
	parseUriRef(uriRef, scheme, authority, path, query, fragment);

	// Make sure we have a file scheme URI, or that it doesn't have a scheme
	if (!scheme.empty()  &&  scheme != "file")
		return "";

	string filePath = path;

#ifdef WIN32
	// Windows - replace two leading slashes with one leading slash, so that
	// ///otherComputer/file.dae becomes //otherComputer/file.dae
	if (filePath.length() >= 2  &&  filePath[0] == '/'  &&  filePath[1] == '/')
		filePath.erase(0, 1);

	// Windows - convert "/C:/" to "C:/"
	if (filePath.length() >= 3  &&  filePath[0] == '/'  &&  filePath[2] == ':')
		filePath.erase(0, 1);

	// Windows - convert forward slashes to back slashes
	filePath = replace(filePath, "/", "\\");
#endif

	// Replace %20 with space
	filePath = replace(filePath, "%20", " ");
	
	return filePath;
}
