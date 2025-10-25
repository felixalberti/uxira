<?php 
require_once('connections/conexion2.php');
if (isset($_POST['medico'])) {
  $medico = $_POST['medico'];  
}
else {
   $medico = 1;  
}   
mysql_select_db($database_conexion2, $conexion2);
$query_rs_medico = "SELECT * FROM 0_medico_master WHERE medico_no = $medico ORDER BY name";
$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
$row_rs_medico = mysql_fetch_assoc($rs_medico);
$totalRows_rs_medico = mysql_num_rows($rs_medico);
echo '<textarea name="textarea" cols="45" rows="5" disabled="disabled">';
echo $row_rs_medico['detalles']; 
echo '</textarea>';
//
/*
echo '<br><br>';
$query_rs_tipo_cita = "SELECT tc.tipocita,  tb.descripcion, tb.comentario, tb.vermas, tb.titulo_vermas, tc.precio FROM 0_cm_tpcita_x_medico tc, 0_cm_system_tables tb WHERE tc.tipocita = tb.codigotipo and tb.codigotabla = 'TPCITA' and tc.codmed = '$medico'";
$rs_tipo_cita = mysql_query($query_rs_tipo_cita, $conexion2) or die(mysql_error());
$row_rs_tipo_cita = mysql_fetch_assoc($rs_tipo_cita);
$totalRows_rs_tipo_cita = mysql_num_rows($rs_tipo_cita);
if ($totalRows_rs_tipo_cita > 0){
echo '<table width="500" border="1" align="center" cellpadding="1" cellspacing="0">';
echo '<tr>';
echo '<td width="139">Tipo de cita </td>';
echo '<td width="34">Acci&oacute;n</td>';
echo '<td width="47">Descripci&oacute;n</td>';
echo '<td width="47" align="right">Valor</td>';
//echo '<td>Saber M&aacute;s </td>';
echo '</tr>';
$Reg = 0;
//
$change = "alert(document.getElementById('tipocita').value);FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value,'POST');";
//$change = "";
$onclick = "alert(document.getElementById('tipocita').value);FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value,'POST');";
do { 
	 $Reg++;
   echo '<tr>';
   echo '<td>'.$row_rs_tipo_cita['descripcion'].'</td>';
   echo '<td><input name="tipocita" type="radio" value="'.$row_rs_tipo_cita['tipocita'].'"';
   if ($Reg == 1) {echo "checked";} 
   echo " onchange=\"".$change."\"></td>";
   echo '<td>';
   echo $row_rs_tipo_cita['tipocita'].' '.$row_rs_tipo_cita['comentario']."".$row_rs_tipo_cita['vermas'].'</td>';
   echo '<td align="right">';
   echo $row_rs_tipo_cita['precio'].'</td>';
   //echo '<td><a href="'.$row_rs_tipo_cita['vermas'].'"';
   //echo ' target="_blank">';
   //echo $row_rs_tipo_cita['titulo_vermas'];
   //echo '</a></td>';
   echo '</tr>';
} while ($row_rs_tipo_cita = mysql_fetch_assoc($rs_tipo_cita));
echo '</table>';
}
*/
//$onclick = "FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value,'POST');";
//echo '<br><input name="buscar" type="button" id="buscar" value="Buscar" onclick="'.$onclick.'"/>';

?>