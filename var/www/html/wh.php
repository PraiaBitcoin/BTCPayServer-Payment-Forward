<?php
  $data = file_get_contents('php://input');
  //print_r($data);

  if ( $data == '') {
	echo 'Nothing to write';
	exit;
  }

  $session=$_REQUEST['session'];
  

  //if(file_exists('/mnt/hdd/mynode/config.php'))
  //  include_once('/mnt/hdd/mynode/config.php');

  if (! isset ( $session ) )
    $session = '_';

  $event=$_REQUEST['event'];

  if (! isset ( $event ) )
    $event = '_';

  
  if (! isset ( $OUTDIR ) )
    $OUTDIR = '/home/dados/web/webhook/';
  
  $notification = $OUTDIR . $session . '/' . $event . '/';
  mkdir ( $notification, 0777, true );
  chmod ( $notification, 0777 );

  $id = trim(`date "+%Y%m%d-%H%M%S"`) . '-' . trim(`dd if=/dev/random count=16 bs=1|xxd -p -c 80`);

  $notification .= $id;
  $notification .= ".json";
  //echo $notification . '\n'; 

  file_put_contents($notification, $data);
  chmod ( $notification, 0666 );

  echo "OK\n";
  //print_r( $data );
	
?>