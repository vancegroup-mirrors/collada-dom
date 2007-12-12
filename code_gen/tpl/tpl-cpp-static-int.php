<?php
/* Copyright Sony Computer Entertainment Inc., 2007.
   All Rights Reserved.
   CONFIDENTIAL INFORMATION */

  global $meta;
  $_context = $bag['context'];
  for( $i=0; $i<count( $_context ); $i++ )
  {
    $_context[$i] = $_globals['int_prefix'] . ucfirst( $_context[$i] );
  }
?><?= $_globals['meta_prefix'] ?>MetaElement * <?= implode( '::', $_context ) ?>::_Meta = NULL;
<?php
  if ( $_globals['full_code'] )
  {
    $keys = array_keys( $bag['inline_elements'] );
    if ( count( $keys ) > 0 )
    {
      foreach( $keys as $k )
      {
        $inner = $bag['inline_elements'][ $k ];
        if ( !$inner['complex_type'] || $inner['isRestriction'] || $inner['isExtension'] ) {
			print applyTemplate( 'CPP_STATIC_INT', $inner );
		}
      }
    }
  }
?>