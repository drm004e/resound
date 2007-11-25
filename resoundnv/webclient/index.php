<?php
class ResoundLink
{
	private $serverAddress;
	private $serverIsActive;

	// constructor
	function __construct($addr) {
		$this->serverAddress = $addr;
   	}

	function get_address(){return $this->serverAddress;}
	function is_active(){return $this->serverIsActive;}

	// check that the server is alive
	function ping(){
		$fp = @stream_socket_client($this->serverAddress, $errno, $errstr, 30);
		if ($fp) {
			$this->serverIsActive = true;
		} else {
			$this->serverIsActive = false;
		}
	}
	/// returns a string containing the standard get response from the server
	/// returns null on fail
	function get_request()
	{
		$fp = @stream_socket_client($this->serverAddress, $errno, $errstr, 30);
		if ($fp) {
			$ret='';
			$this->serverIsActive = true;
			fwrite($fp, "GET ");
			while (!feof($fp)) {
				$ret.=fgets($fp, 1024);
			}
			fclose($fp);
			return $ret;
		} else {
			$this->serverIsActive = false;
			return null;
		}
	}
	/// send a block or resound xml	to the resound server
	/// returns OK or the exception thrown by the server
	function set_xml($xml){
		$fp = @stream_socket_client($this->serverAddress, $errno, $errstr, 30);
		if ($fp) {
			$this->serverIsActive = true;
			fwrite($fp, "SETXML $xml");
			$result='';
			while (!feof($fp)) {
				$result=fgets($fp, 1024);
			}
			fclose($fp);
			return $result;
		} else {
			$this->serverIsActive = false;
			return null;
		}
	}
	/// returns xml received from the resound server
	/// returns null if the connection failed
	function get_xml(){
		$fp = @stream_socket_client($this->serverAddress, $errno, $errstr, 30);
		
		if ($fp) {
			$ret='';
			$this->serverIsActive = true;
			fwrite($fp, "GETXML");
			while (!feof($fp)) {
				$ret.= fgets($fp, 1024);
			}
			fclose($fp);
			return $ret;
		} else {
			$this->serverIsActive = false;
			return null;
		}
		
	}
}
?>


<html>
<head>
</head>
<body>
<h1>Resoundnv</h1>
<p>Resound nv web control panel.</p>
<?php
$link = new ResoundLink("tcp://localhost:1100");
$link->ping();
echo "<h2>Get request returned:</h2><p><xmp>".$link->get_request()."</xmp></p>\n";
$xml = stripslashes($_POST['xml']);
echo "<h2>setxml request:</h2><p>".$link->set_xml($xml)."</p>\n";
echo "<p><xmp>$xml</xmp></p>";
echo "<h2>resound xml tree:</h2><p><xmp>".$link->get_xml()."</xmp></p>";
?>
<form action="index.php" method="post">
<textarea name="xml" cols="80" rows="10"><?=$xml?></textarea>
<input type="submit" vlaue="submit"/>
</form>
<p>
Server address: <?=$link->get_address();?><br/>
Server status:  <?=$link->is_active();?><br/>
</p>

</body>
</html>
