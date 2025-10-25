<?php 
  $codmed = $_GET['codmed'];
  require_once('connections/conexion2.php');  
  $sql =	"SELECT tm.tipocita, st.descripcion, tm.precio FROM 0_cm_tpcita_x_medico tm, 0_cm_system_tables st  where tm.tipocita = st.codigotipo and st.codigotabla = 'TPCITA' and codmed = '".$codmed."'";

  mysql_select_db($database_conexion2, $conexion2); 
	$rs=mysql_query($sql, $conexion2) or die(mysql_error());
	header('Content-Type: text/xml');
	echo "<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>\n";
	echo "<tipocitas>\n";
	while ($reg=mysql_fetch_array($rs)){
		echo "<tipocita>";
		echo "<codigo>".$reg['tipocita']."</codigo>";
		echo "<descri>".$reg['descripcion'].' - precio: '.$reg['precio'].' Bsf.'."</descri>";
		echo "</tipocita>\n";
	}
	echo "</tipocitas>";
?>