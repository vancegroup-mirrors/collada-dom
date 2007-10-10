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
#include <dom/domTorus.h>
#include <dae/daeMetaCMPolicy.h>
#include <dae/daeMetaSequence.h>
#include <dae/daeMetaChoice.h>
#include <dae/daeMetaGroup.h>
#include <dae/daeMetaAny.h>
#include <dae/daeMetaElementAttribute.h>

daeElementRef
domTorus::create(daeInt)
{
	domTorusRef ref = new domTorus;
	return ref;
}


daeMetaElement *
domTorus::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "torus" );
	_Meta->registerClass(domTorus::create, &_Meta);

	daeMetaCMPolicy *cm = NULL;
	daeMetaElementAttribute *mea = NULL;
	cm = new daeMetaSequence( _Meta, cm, 0, 1, 1 );

	mea = new daeMetaElementAttribute( _Meta, cm, 0, 1, 1 );
	mea->setName( "radius" );
	mea->setOffset( daeOffsetOf(domTorus,elemRadius) );
	mea->setElementType( domTorus::domRadius::registerElement() );
	cm->appendChild( mea );
	
	cm->setMaxOrdinal( 0 );
	_Meta->setCMRoot( cm );	
	
	
	_Meta->setElementSize(sizeof(domTorus));
	_Meta->validate();

	return _Meta;
}

daeElementRef
domTorus::domRadius::create(daeInt)
{
	domTorus::domRadiusRef ref = new domTorus::domRadius;
	return ref;
}


daeMetaElement *
domTorus::domRadius::registerElement()
{
    if ( _Meta != NULL ) return _Meta;
    
    _Meta = new daeMetaElement;
    _Meta->setName( "radius" );
	_Meta->registerClass(domTorus::domRadius::create, &_Meta);

	_Meta->setIsInnerClass( true );
	//	Add attribute: _value
 	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Float2"));
		ma->setOffset( daeOffsetOf( domTorus::domRadius , _value ));
		ma->setContainer( _Meta );
		_Meta->appendAttribute(ma);
	}
	
	
	_Meta->setElementSize(sizeof(domTorus::domRadius));
	_Meta->validate();

	return _Meta;
}


daeMetaElement * domTorus::_Meta = NULL;
daeMetaElement * domTorus::domRadius::_Meta = NULL;


