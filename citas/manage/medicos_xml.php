<?php 

  require_once('connections/conexion2.php');  
  $sql =	"SELECT * FROM 0_medico_master ORDER BY medico_no";

  mysql_select_db($database_conexion2, $conexion2); 
	$rs=mysql_query($sql, $conexion2) or die(mysql_error());
	header('Content-Type: text/xml');
	echo "<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>\n";
	echo "<medicos>\n";
	while ($reg=mysql_fetch_array($rs)){
		echo "<medico>";
		echo "<codigo>".$reg['medico_no']."</codigo>";
		echo "<descri>".$reg['name']."</descri>";
		echo "</medico>\n";
	}
	echo "</medicos>";
?>