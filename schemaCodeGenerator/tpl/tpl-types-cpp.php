<?php
/*
 * Copyright 2007 Sony Computer Entertainment Inc.
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

if ( $_globals['copyright'] ) {
print $_globals['copyright_text'];
}
?>
#include <<?= $_globals['prefix'] . '/' . $_globals['prefix'] ?>Types.h>
#include <dae/daeDom.h>
#include <dom/domCOLLADA.h>

<?php

foreach( $bag as $type => $meta )
{
  if ( $meta['isComplex'] ) {
  ?>#include <<?= $_globals['prefix'] . '/' . $_globals['prefix'] . ucfirst( $type ) ?>.h>
<?php
  }
}

?>

void registerDomTypes(daeAtomicTypeList& atomicTypes)
{
	daeAtomicType* type = NULL;

<?php

foreach( $bag as $type => $meta )
{
  if ( count( $meta['enum'] ) > 0 && !$meta['useConstStrings'] )
  {?>
	// ENUM: <?= ucfirst( $type ) ?>

	type = new daeEnumType;
	type->_nameBindings.append("<?= ucfirst( $type ) ?>");
	((daeEnumType*)type)->_strings = new daeStringRefArray;
	((daeEnumType*)type)->_values = new daeEnumArray;
<?php
    foreach( $meta['enum'] as $val )
    {?>
	((daeEnumType*)type)->_strings->append("<?= $val ?>");
<?php $val = str_replace( '.', '_', $val ); ?>
	((daeEnumType*)type)->_values->append(<?= strtoupper($type) . "_" . $val ?>);
<?php
    }
    print "\tatomicTypes.append( type );\n\n";
  }
  elseif ( $meta['isComplex'] ) {
  ?>
	// COMPLEX TYPE: <?= ucfirst( $type ) ?>

	type = new daeElementRefType;
	type->_nameBindings.append("<?= ucfirst( $type ) ?>");
	atomicTypes.append( type );

<?php
  }
  /*else if ( $meta['union_type'] ) { //union type
	?>
	// ENUM: <?= ucfirst( $type ) ?>
	
	type = new daeEnumType;
	type->_nameBindings.append("<?= ucfirst( $type ) ?>");
	((daeEnumType*)type)->_strings = new daeStringRefArray;
	((daeEnumType*)type)->_values = new daeEnumArray;
<?php
	$types = explode( ' ', $meta['union_members'] );
	foreach ( $types as $typeName ) {
		if ( isset( $bag[$typeName] ) && count($bag[$typeName]['enum']) > 0 ) {
			foreach( $bag[$typeName]['enum'] as $val )
    {?>
	((daeEnumType*)type)->_strings->append("<?= $val ?>");
<?php $val = str_replace( '.', '_', $val ); ?>
	((daeEnumType*)type)->_values->append(<?= strtoupper($type) . "_" . $val ?>);
<?php
			}
		}
    }
    print "\tatomicTypes.append( type );\n\n";
  }  */
  else if ( !$meta['useConstStrings'] ) { //standard typedef
	$base = strlen( $meta['base'] ) > 0 ? $meta['base'] : $meta['listType'];
	if ( preg_match( "/xs\:/", $base ) ) {
		$base = 'xs' . ucfirst( substr( $base, 3 ) );
	}
	else {
		$base = ucfirst( $base );
	}
  ?>
	// TYPEDEF: <?= ucfirst( $type ) ?>
	//check if this type has an existing base
<?php 
	//special casing URIFragmentType to be a xsURI for automatic resolution
	if ( $type == 'URIFragmentType' ) {
		print "\ttype = atomicTypes.get(\"xsAnyURI\");\n";
	}
	else {
		print "\ttype = atomicTypes.get(\"". $base ."\");\n";
	}
?>
	if ( type == NULL ) { //register as a raw type
		type = new daeRawRefType;
		type->_nameBindings.append("<?= ucfirst( $type ) ?>");
		atomicTypes.append( type );
	}
	else { //add binding to existing type
		type->_nameBindings.append("<?= ucfirst( $type ) ?>");
	}
	
<?php
  }
}
?>
}

daeMetaElement* registerDomElements(DAE& dae)
{
	daeMetaElement* meta = domCOLLADA::registerElement(dae);
	// Enable tracking of top level object by default
	domCOLLADA::_Meta->setIsTrackableForQueries(true);
	return meta;	
}