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
#include <dae/daeDom.h>
#include <dom/domLibrary_physics_materials.h>
#include <dae/daeMetaCMPolicy.h>
#include <dae/daeMetaSequence.h>
#include <dae/daeMetaChoice.h>
#include <dae/daeMetaGroup.h>
#include <dae/daeMetaAny.h>
#include <dae/daeMetaElementAttribute.h>

daeElementRef
domLibrary_physics_materials::create()
{
	domLibrary_physics_materialsRef ref = new domLibrary_physics_materials;
	return ref;
}


daeMetaElement *
domLibrary_physics_materials::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "library_physics_materials" );
	meta->registerClass(domLibrary_physics_materials::create, &meta);

	daeMetaCMPolicy *cm = NULL;
	daeMetaElementAttribute *mea = NULL;
	cm = new daeMetaSequence( meta, cm, 0, 1, 1 );

	mea = new daeMetaElementAttribute( meta, cm, 0, 0, 1 );
	mea->setName( "asset" );
	mea->setOffset( daeOffsetOf(domLibrary_physics_materials,elemAsset) );
	mea->setElementType( domAsset::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementArrayAttribute( meta, cm, 1, 1, -1 );
	mea->setName( "physics_material" );
	mea->setOffset( daeOffsetOf(domLibrary_physics_materials,elemPhysics_material_array) );
	mea->setElementType( domPhysics_material::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementArrayAttribute( meta, cm, 2, 0, -1 );
	mea->setName( "extra" );
	mea->setOffset( daeOffsetOf(domLibrary_physics_materials,elemExtra_array) );
	mea->setElementType( domExtra::registerElement(dae) );
	cm->appendChild( mea );

	cm->setMaxOrdinal( 2 );
	meta->setCMRoot( cm );	

	//	Add attribute: id
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "id" );
		ma->setType( daeAtomicType::get("xsID"));
		ma->setOffset( daeOffsetOf( domLibrary_physics_materials , attrId ));
		ma->setContainer( meta );
	
		meta->appendAttribute(ma);
	}

	//	Add attribute: name
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "name" );
		ma->setType( daeAtomicType::get("xsNCName"));
		ma->setOffset( daeOffsetOf( domLibrary_physics_materials , attrName ));
		ma->setContainer( meta );
	
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domLibrary_physics_materials));
	meta->validate();

	return meta;
}
