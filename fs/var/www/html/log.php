<?php
  $data = file_get_contents('php://input');
  //print_r($data);

  if ( $data == '') {
	echo 'Nothing to write';
	exit;
  }

  $token=$_REQUEST['token'];

  if(file_exists('/mnt/hdd/mynode/config.php'))
    include_once('/mnt/hdd/mynode/config.php');
  
  if (! isset ( $OUTDIR ) )
    $OUTDIR = '/mnt/hdd/BTCPAYSERVER';
  
  $notification = $OUTDIR . '/invoices/';
  mkdir ( $notification, 0777, true );

  $id = trim(`date "+%Y%m%d-%H%M"`) . '-' . trim(`dd if=/dev/random count=16 bs=1|xxd -p -c 80`);

  $notification .= $id;
  $controle = $notification . ".control";
  $notification .= ".json";
  //echo $notification . '\n'; 

  file_put_contents($notification, $data);
  file_put_contents($controle, "FILE=$notification\n" );
  file_put_contents($controle, "TOKEN=$token\n", FILE_APPEND  );

  echo "OK\n";
  //print_r( $data );
	
?>