<?php
/* Copyright Sony Computer Entertainment Inc., 2007.
   All Rights Reserved.
   CONFIDENTIAL INFORMATION */

// COMMAND LINE: bin/php.exe gen.php collada.xsd [minimal]
// Note: must be run from home directory of code generator (i.e. where gen.php lives)


if ( file_exists( $argv[1] ) ) { $collada_file = $argv[1]; }
else
{
  die( "Can't find COLLADA file '" . $argv[1] . "'\n" );
}


require_once( 'src/SchemaParser.php' );
require_once( 'om/object-model.php' );
require_once( 'tpl/template-engine.php' );


if ( preg_match( "/min/i", $argv[2] ) || preg_match( "/min/i", $argv[3] ) ) { 
	$_globals['full_code'] = false; 
}
if ( preg_match( "/cprt/i", $argv[2] ) || preg_match( "/cprt/i", $argv[3] ) ) { 
	$_globals['copyright'] = true; 
}

$_globals['accessorsAndMutators'] = true;

$p = new SchemaParser();
$p->parse( $collada_file );


initGen( $collada_file );


$pop = $p->root_elements[0];

//Grab the collada version number
$_globals['constStrings']['COLLADA_VERSION'] = "\"". $pop->getAttribute('version') . "\";\n";
//Grab the collada namespace
$_globals['constStrings']['COLLADA_NAMESPACE'] = "\"". $pop->getAttribute('xmlns') . "\";\n\n";

// Grab simple types and collect meta-data for code-gen
$t_list = $pop->getElementsByType( 'xsSimpleType' );


$typemeta = array();

for( $i=0; $i<count( $t_list ); $i++ )
{
  $local_meta = & $t_list[$i]->generate();
  $typemeta[ $local_meta['type'] ] = & $local_meta;
  //print "Type: ". $local_meta['type'] ." created\n";
}

function propogateArrayTypes( &$lmeta ) {
	global $typemeta;
	if ( $lmeta['isArray'] ) {
		return;
	}
	if( isset( $typemeta[$lmeta['base']] ) ) {
		propogateArrayTypes( $typemeta[$lmeta['base']] );
		$lmeta['isArray'] = $typemeta[$lmeta['base']]['isArray'];
	}
	//print $lmeta['type'] ." isArray = ". $lmeta['isArray'] ."\n";
}
foreach( $typemeta as $k => &$local_meta ) {
	propogateArrayTypes( $local_meta );
}

//Grab global complex types and make them available for all who need them

$_globals['complex_types'] = $pop->getElementsByType( 'xsComplexType' );

//generate type meta data
//print applyTemplate( 'TYPES_HEADER_FILE', $typemeta );
//print applyTemplate( 'TYPES_CPP_FILE', $typemeta );

$element_context = array();
$meta = array();

print "COMPLEX TYPES\n";
for( $i=0; $i<count( $_globals['complex_types'] ); $i++ )
{
  $local_meta = & $_globals['complex_types'][$i]->generate( $element_context, $_globals['global_elements'] );
  $meta[ $local_meta['element_name'] ] = & $local_meta;
}

//collect element meta-data for code-gen

//Grab global groups and make them available for all who need them
$_globals['groups'] = $pop->getElementsByType( 'xsGroup' );
//collect meta-data for code-gen
print "GROUPS\n";
for( $i=0; $i<count( $_globals['groups'] ); $i++ )
{
  $local_meta = & $_globals['groups'][$i]->generate( $element_context, $_globals['global_elements'] );
  $meta[ $local_meta['element_name'] ] = & $local_meta;
}

// Grab global elements and collect meta-data for code-gen
$e_list = $pop->getElementsByType( 'xsElement' );

print "ELEMENTS\n";

for( $i=0; $i<count( $e_list ); $i++ )
{
  $local_meta = & $e_list[$i]->generate( $element_context, $_globals['global_elements'] );
  $meta[ $local_meta['element_name'] ] = & $local_meta;
}

//propogate the substitutableWith lists and attributes inherited by type
foreach( $meta as $k => &$local_meta ) {
	if ( $local_meta['substitution_group'] != '' ) {
		$meta[$local_meta['substitution_group']]['substitutableWith'][] = $k;
		//$meta[$local_meta['substitution_group']]['ref_elements'][] = $k;
		//print $local_meta['substitution_group'] ." sub with ". $k ."\n";
	}
}

$indentNum = 0;
// Generate header files
$includeList = array();
foreach( $meta as $k => &$local_meta )
{
  // Generate the dom
  print applyTemplate( 'HEADER_FILE', $local_meta );
  print applyTemplate( 'CPP_FILE', $local_meta );
  
  // Generate the integration objects
  print applyTemplate( 'HEADER_FILE_INT', $local_meta );
  print applyTemplate( 'CPP_FILE_INT', $local_meta );
}

print applyTemplate( 'TYPES_HEADER_FILE', $typemeta );
print applyTemplate( 'TYPES_CPP_FILE', $typemeta );

// Generate a single includes file and a register() function .cpp
print applyTemplate( 'REGISTER_LIST_FILE', $meta );
print applyTemplate( 'INCLUDE_LIST_FILE', $meta );

print applyTemplate( 'ELEMENTS_FILE', $meta );
print applyTemplate( 'CONSTANTS_FILE', $_globals['constStrings'] );
print applyTemplate( 'CONSTANTS_CPP_FILE', $_globals['constStrings'] );
cleanupGen();

?>
