<?php
$serverAddress = "tcp://localhost:1100";
$serverStatus = "Offline";
$fp = @stream_socket_client($serverAddress, $errno, $errstr, 30);
if (!$fp) {
    //echo "$errstr ($errno)<br />\n";
} else {
	$serverStatus = "Online";
    fwrite($fp, "GET / HTTP/1.0\r\nHost: resound.sourceforge.net\r\nAccept: */*\r\n\r\n");
    while (!feof($fp)) {
        echo fgets($fp, 1024);
    }
    fclose($fp);
}

?>

<html>
<head>
</head>
<body>
<h1>Resoundnv</h1>
<p>Resound nv web control panel.</p>
<p>
Server address: <?=$serverAddress?><br/>
Server status:  <?=$serverStatus?><br/>

</p>

</body>
</html>
