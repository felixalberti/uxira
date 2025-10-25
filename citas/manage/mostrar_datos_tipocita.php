<?php
$codmed = $_POST['medico'];
$tipocita = $_POST['tipocita'];
require_once('connections/conexion2.php');  
mysql_select_db($database_conexion2, $conexion2);
$sql =	"SELECT tm.tipocita, st.comentario, tm.precio FROM 0_cm_tpcita_x_medico tm, 0_cm_system_tables st  where tm.tipocita = st.codigotipo and st.codigotabla = 'TPCITA' and codmed = '".$codmed."' and st.codigotipo='".$tipocita."'";
$rs_tipocita = mysql_query($sql, $conexion2) or die(mysql_error());
$row_rs_tipocita = mysql_fetch_assoc($rs_tipocita);
$totalRows_rs_tipocita = mysql_num_rows($rs_tipocita);
echo '<textarea name="textarea" cols="45" rows="5" disabled="disabled">';
echo $row_rs_tipocita['comentario']; 
echo '</textarea>';
?>