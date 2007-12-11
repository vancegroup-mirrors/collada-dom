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
#include <dom/domFx_sampler3D_common.h>
#include <dae/daeMetaCMPolicy.h>
#include <dae/daeMetaSequence.h>
#include <dae/daeMetaChoice.h>
#include <dae/daeMetaGroup.h>
#include <dae/daeMetaAny.h>
#include <dae/daeMetaElementAttribute.h>

daeElementRef
domFx_sampler3D_common::create()
{
	domFx_sampler3D_commonRef ref = new domFx_sampler3D_common;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "fx_sampler3D_common" );
	meta->registerClass(domFx_sampler3D_common::create, &meta);

	daeMetaCMPolicy *cm = NULL;
	daeMetaElementAttribute *mea = NULL;
	cm = new daeMetaSequence( meta, cm, 0, 1, 1 );

	mea = new daeMetaElementAttribute( meta, cm, 0, 1, 1 );
	mea->setName( "source" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemSource) );
	mea->setElementType( domFx_sampler3D_common::domSource::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementAttribute( meta, cm, 1, 0, 1 );
	mea->setName( "wrap_s" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemWrap_s) );
	mea->setElementType( domFx_sampler3D_common::domWrap_s::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementAttribute( meta, cm, 2, 0, 1 );
	mea->setName( "wrap_t" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemWrap_t) );
	mea->setElementType( domFx_sampler3D_common::domWrap_t::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementAttribute( meta, cm, 3, 0, 1 );
	mea->setName( "wrap_p" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemWrap_p) );
	mea->setElementType( domFx_sampler3D_common::domWrap_p::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementAttribute( meta, cm, 4, 0, 1 );
	mea->setName( "minfilter" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemMinfilter) );
	mea->setElementType( domFx_sampler3D_common::domMinfilter::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementAttribute( meta, cm, 5, 0, 1 );
	mea->setName( "magfilter" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemMagfilter) );
	mea->setElementType( domFx_sampler3D_common::domMagfilter::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementAttribute( meta, cm, 6, 0, 1 );
	mea->setName( "mipfilter" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemMipfilter) );
	mea->setElementType( domFx_sampler3D_common::domMipfilter::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementAttribute( meta, cm, 7, 0, 1 );
	mea->setName( "border_color" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemBorder_color) );
	mea->setElementType( domFx_sampler3D_common::domBorder_color::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementAttribute( meta, cm, 8, 0, 1 );
	mea->setName( "mipmap_maxlevel" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemMipmap_maxlevel) );
	mea->setElementType( domFx_sampler3D_common::domMipmap_maxlevel::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementAttribute( meta, cm, 9, 0, 1 );
	mea->setName( "mipmap_bias" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemMipmap_bias) );
	mea->setElementType( domFx_sampler3D_common::domMipmap_bias::registerElement(dae) );
	cm->appendChild( mea );

	mea = new daeMetaElementArrayAttribute( meta, cm, 10, 0, -1 );
	mea->setName( "extra" );
	mea->setOffset( daeOffsetOf(domFx_sampler3D_common,elemExtra_array) );
	mea->setElementType( domExtra::registerElement(dae) );
	cm->appendChild( mea );

	cm->setMaxOrdinal( 10 );
	meta->setCMRoot( cm );	

	meta->setElementSize(sizeof(domFx_sampler3D_common));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domSource::create()
{
	domFx_sampler3D_common::domSourceRef ref = new domFx_sampler3D_common::domSource;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domSource::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "source" );
	meta->registerClass(domFx_sampler3D_common::domSource::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("xsNCName"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domSource , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domSource));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domWrap_s::create()
{
	domFx_sampler3D_common::domWrap_sRef ref = new domFx_sampler3D_common::domWrap_s;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domWrap_s::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "wrap_s" );
	meta->registerClass(domFx_sampler3D_common::domWrap_s::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Fx_sampler_wrap_common"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domWrap_s , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domWrap_s));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domWrap_t::create()
{
	domFx_sampler3D_common::domWrap_tRef ref = new domFx_sampler3D_common::domWrap_t;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domWrap_t::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "wrap_t" );
	meta->registerClass(domFx_sampler3D_common::domWrap_t::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Fx_sampler_wrap_common"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domWrap_t , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domWrap_t));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domWrap_p::create()
{
	domFx_sampler3D_common::domWrap_pRef ref = new domFx_sampler3D_common::domWrap_p;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domWrap_p::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "wrap_p" );
	meta->registerClass(domFx_sampler3D_common::domWrap_p::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Fx_sampler_wrap_common"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domWrap_p , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domWrap_p));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domMinfilter::create()
{
	domFx_sampler3D_common::domMinfilterRef ref = new domFx_sampler3D_common::domMinfilter;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domMinfilter::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "minfilter" );
	meta->registerClass(domFx_sampler3D_common::domMinfilter::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Fx_sampler_filter_common"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domMinfilter , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domMinfilter));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domMagfilter::create()
{
	domFx_sampler3D_common::domMagfilterRef ref = new domFx_sampler3D_common::domMagfilter;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domMagfilter::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "magfilter" );
	meta->registerClass(domFx_sampler3D_common::domMagfilter::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Fx_sampler_filter_common"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domMagfilter , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domMagfilter));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domMipfilter::create()
{
	domFx_sampler3D_common::domMipfilterRef ref = new domFx_sampler3D_common::domMipfilter;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domMipfilter::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "mipfilter" );
	meta->registerClass(domFx_sampler3D_common::domMipfilter::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Fx_sampler_filter_common"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domMipfilter , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domMipfilter));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domBorder_color::create()
{
	domFx_sampler3D_common::domBorder_colorRef ref = new domFx_sampler3D_common::domBorder_color;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domBorder_color::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "border_color" );
	meta->registerClass(domFx_sampler3D_common::domBorder_color::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaArrayAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("Fx_color_common"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domBorder_color , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domBorder_color));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domMipmap_maxlevel::create()
{
	domFx_sampler3D_common::domMipmap_maxlevelRef ref = new domFx_sampler3D_common::domMipmap_maxlevel;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domMipmap_maxlevel::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "mipmap_maxlevel" );
	meta->registerClass(domFx_sampler3D_common::domMipmap_maxlevel::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("xsUnsignedByte"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domMipmap_maxlevel , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domMipmap_maxlevel));
	meta->validate();

	return meta;
}

daeElementRef
domFx_sampler3D_common::domMipmap_bias::create()
{
	domFx_sampler3D_common::domMipmap_biasRef ref = new domFx_sampler3D_common::domMipmap_bias;
	return ref;
}


daeMetaElement *
domFx_sampler3D_common::domMipmap_bias::registerElement(DAE& dae)
{
	daeMetaElement* meta = dae.getMeta(getTypeStatic());
	if ( meta != NULL ) return meta;

	meta = new daeMetaElement;
	dae.setMeta(getTypeStatic(), *meta);
	meta->setName( "mipmap_bias" );
	meta->registerClass(domFx_sampler3D_common::domMipmap_bias::create, &meta);

	meta->setIsInnerClass( true );
	//	Add attribute: _value
	{
		daeMetaAttribute *ma = new daeMetaAttribute;
		ma->setName( "_value" );
		ma->setType( daeAtomicType::get("xsFloat"));
		ma->setOffset( daeOffsetOf( domFx_sampler3D_common::domMipmap_bias , _value ));
		ma->setContainer( meta );
		meta->appendAttribute(ma);
	}

	meta->setElementSize(sizeof(domFx_sampler3D_common::domMipmap_bias));
	meta->validate();

	return meta;
}
