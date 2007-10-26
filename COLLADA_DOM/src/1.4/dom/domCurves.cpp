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

#include <dae/daeDom.h>
#include <dom/domCurves.h>
#include <dae/daeMetaCMPolicy.h>
#include <dae/daeMetaSequence.h>
#include <dae/daeMetaChoice.h>
#include <dae/daeMetaGroup.h>
#include <dae/daeMetaAny.h>
#include <dae/daeMetaElementAttribute.h>

daeElementRef
domCurves::create(daeInt)
{
	domCurvesRef ref = new domCurves;
	return ref;
}


daeMetaElement *
domCurves::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "curves" );
	_Meta->registerClass(domCurves::create, &_Meta);

	daeMetaCMPolicy *cm = NULL;
	daeMetaElementAttribute *mea = NULL;
	cm = new daeMetaSequence( _Meta, cm, 0, 1, -1 );

	mea = new daeMetaElementArrayAttribute( _Meta, cm, 0, 1, -1 );
	mea->setName( "curve" );
	mea->setOffset( daeOffsetOf(domCurves,elemCurve_array) );
	mea->setElementType( domCurveType::registerElement() );
	cm->appendChild( mea );
	
	cm->setMaxOrdinal( 2999 );
	_Meta->setCMRoot( cm );	
	
	
	_Meta->setElementSize(sizeof(domCurves));
	_Meta->validate();

	return _Meta;
}


daeMetaElement * domCurves::_Meta = NULL;

