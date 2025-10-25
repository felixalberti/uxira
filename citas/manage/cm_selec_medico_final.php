<?PHP session_start(); ?>
<?php require_once('connections/conexion2.php'); ?>
<?php
   $_SESSION = array();
   session_destroy(); //Para inhabilitar todas las variables de session 

   $_SESSION['fecha_cita'] = date("Y/m/d");

mysql_select_db($database_conexion2, $conexion2);
$query_rs_medico = "SELECT * FROM 0_medico_master ORDER BY medico_no";
$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
$row_rs_medico = mysql_fetch_assoc($rs_medico);
$totalRows_rs_medico = mysql_num_rows($rs_medico);

//Soap
//include_once("../../../proyectsoap/otraprueba/citasmed_soap.php");       
$query1 = "SELECT F13_PLANTILLA, F13_NOMBRE 
  FROM F13_PLANTILLA INNER JOIN F15_GRUPO 
  ON F13_PLANTILLA.F15_GRUPO = F15_GRUPO.F15_GRUPO 
  WHERE F13_ANULADO = 0 AND F35_TIPOSERVICIO = 2 AND
  F13_PLANTILLA.Z13_ESPECIALIDAD = 23";
$query2 = "SELECT Z14_PERSONALMEDICO, CASE Z42_CONSULTORIO WHEN 0 THEN Z14_NOMBRE ELSE Z42_NOMBRE + ' - ' + Z14_NOMBRE END AS Z14_NOMBRE FROM VZ26_PERSONALMEDICO_ESPECIALIDAD_HORARIO_ACTIVO WHERE Z14_ANULADO = 0 AND Z13_ESPECIALIDAD = 23 AND E10_DIASEMANA IN (-1,2) GROUP BY Z14_PERSONALMEDICO, Z42_CONSULTORIO, Z42_NOMBRE, Z14_NOMBRE";


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Centro Vascular y Cuidado Integral de Heridas</title>
<style>

.bot{
	text-decoration: none;
	color: FFFFFF;
	font-size: 10px; 
	font-family: verdana,arial;
	font-weight: bold;
}
.bot:hover{color: 000000;}
.bot1{
	color: D56767;
	font-size: 15px; 
	font-family: Times New Roman,verdana,arial;
	font-weight: bold;
}
.bot1:hover{color: 3B5598;}
.bot2{
	color: D56767;
	font-size: 13px; 
	font-family: Times New Roman,verdana,arial;
}
.bot2:hover{color: 3B5598;}

TD{
font-size: 10px;
FONT-FAMILY: verdana,arial;
color: 000000;
}
</style>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta name="Title" content="angios.com - Clinicas,  Centros Medicos y Hospitales en Caracas/Miranda en Venezuela (en Salud,  Ciencia y Bienestar)" />
  <meta name="Keywords" content="CLINICAS, CITAS MEDICAS, CENTROS MEDICOS Y HOSPITALES de/en Caracas" >
   <meta name="Description" content="CLINICAS, CITAS MEDICAS, CENTROS MEDICOS Y HOSPITALES de/en Caracas" >
  <meta name="Robots" content="index, follow">
<script language="JavaScript" type="text/JavaScript">
<!--
function MM_reloadPage(init) {  //reloads the window if Nav4 resized
  if (init==true) with (navigator) {if ((appName=="Netscape")&&(parseInt(appVersion)==4)) {
    document.MM_pgW=innerWidth; document.MM_pgH=innerHeight; onresize=MM_reloadPage; }}
  else if (innerWidth!=document.MM_pgW || innerHeight!=document.MM_pgH) location.reload();
}
MM_reloadPage(true);
//-->
</script>
<script language="JavaScript" type="text/javascript" src="javascript/libreriaajax.js"></script>
<script language="JavaScript" type="text/javascript" src="javascript/ajax_listas.js"></script>
<link type="text/css" rel="stylesheet" href="../calendario/dhtmlgoodies_calendar.css?random=20051112" media="screen"></LINK>
<SCRIPT type="text/javascript" src="../calendario/dhtmlgoodies_calendar.js?random=20060118"></script>
<script> 
function uno(src,color_entrada) { 
    src.bgColor=color_entrada;src.style.cursor="hand"; 
	src.bgColor=color_entrada;
} 
function dos(src,color_default) { 
    src.bgColor=color_default;src.style.cursor="default"; 
	src.bgColor=color_default;
}
function rellenar(cadena){
if (cadena.length == 2)	return cadena;
cadcero='';
for(i=0;i<(2-cadena.length);i++){
  cadcero+='0';
}
return cadcero+cadena;
} 
</script> 
<link href="ccs/hojaestilo.css" rel="stylesheet" type="text/css">

<link href="javascript/jcarousel/ccs/style.css" rel="stylesheet" type="text/css" />
<!--
  jQuery library
-->
<script type="text/javascript" src="javascript/jcarousel/lib/jquery-1.4.2.min.js"></script>
<!--
  jCarousel library
-->
<script type="text/javascript" src="javascript/jcarousel/lib/jquery.jcarousel.min.js"></script>
<!--
  jCarousel skin stylesheet
-->
<link rel="stylesheet" type="text/css" href="javascript/jcarousel/skins/tango/skin.css" />

<script type="text/javascript">

jQuery(document).ready(function() {
    jQuery('#mycarousel').jcarousel({
        vertical: true,
        scroll: 2
    });
});

</script>
</head>

<body onLoad="cargar('medicos_xml.php','medico');">
</br>
<table width="900" border="1" cellspacing="0" cellpadding="0">
<tr>
<td>
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td width="9" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
    <td height="14" colspan="3" bgcolor="#999999"><strong><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Tomar una cita</font></strong></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td colspan="2">&nbsp;</td>
    <td width="546">&nbsp;</td>
  </tr>  
  <tr>
    <td>&nbsp;</td>
    <td colspan="3">
    	<table border="0" cellpadding="0" cellspacing="0">
		<tr>
    	<td align="left"><div><?php 
		echo '<table  border="0" cellspacing="0" cellpadding="0" >';
		$label = 'Especialidad:';
		echo "<tr><td>$label</td><td nowrap>";
		//dame_datos_angios_gt($query1,'Especialidad:');
		echo "</td>\n</tr>\n";
		$label = 'Médico:';
		echo "<tr><td>$label</td><td nowrap>";
		//dame_datos_angios_gt($query1,'Médico:');
		echo "</td>\n</tr>\n";
		$label = 'Servicio:';
		echo "<tr><td>$label</td><td nowrap>";
		//dame_datos_angios_gt($query1,'Servicio:');
		echo "</td>\n</tr>\n";
		$label = 'Descripción servicio:';
		echo "<tr><td>$label</td><td nowrap>xxx";
		echo "</td>\n</tr>\n";
		echo '</table>';  
		?>		
  	  </div></td>
	    <td align="left"><div id="wrap">
    <ul id="mycarousel" class="jcarousel jcarousel-skin-tango">
    <li><img src="http://localhost/citasmed/citas/manage/imagenes/pic1.jpg" width="75" height="75" alt="" /></li>
    <li><img src="http://localhost/citasmed/citas/manage/imagenes/pic2.jpg" width="75" height="75" alt="" /></li>
    <li><img src="http://localhost/citasmed/citas/manage/imagenes/pic3.jpg" width="75" height="75" alt="" /></li>
    <li><img src="http://static.flickr.com/77/199481108_4359e6b971_s.jpg" width="75" height="75" alt="" /></li>
    <li><img src="http://static.flickr.com/58/199481143_3c148d9dd3_s.jpg" width="75" height="75" alt="" /></li>
    <li><img src="http://static.flickr.com/72/199481203_ad4cdcf109_s.jpg" width="75" height="75" alt="" /></li>
    <li><img src="http://static.flickr.com/58/199481218_264ce20da0_s.jpg" width="75" height="75" alt="" /></li>
    <li><img src="http://static.flickr.com/69/199481255_fdfe885f87_s.jpg" width="75" height="75" alt="" /></li>
    <li><img src="http://static.flickr.com/60/199480111_87d4cb3e38_s.jpg" width="75" height="75" alt="" /></li>
    <li><img src="http://static.flickr.com/70/229228324_08223b70fa_s.jpg" width="75" height="75" alt="" /></li>
  </ul>
    <!-- The content will be dynamically loaded in here --></ul></div></td>
    	</tr>    	
	    
	    </table>
    </td>    	
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td colspan="2">&nbsp;</td>
    <td width="546">&nbsp;</td>
  </tr>  
  <tr>
    <td>&nbsp;</td>
    <td width="108"><font size="-1" face="Verdana, Arial, Helvetica, sans-serif">M&eacute;dico:</font></td>
    <td width="230"><select name="medico" size="1" id="medico" onchange="cargar('tipos_citas_x_medico_xml.php?codmed='+document.getElementById('medico').value,'tipocita');FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+'001','POST');FAjax('mostrar_datos_medicos.php','mostrar','medico='+document.getElementById('medico').value,'POST');">
    </select></td>
    <td width="546" rowspan="3" valign="bottom"><table width="100%" border="0" align="center" cellpadding="1">
      <tr>
        <td colspan="2" align="center"><strong><font size="-1" face="Verdana, Arial, Helvetica, sans-serif">Buscar citas por rangos de fechas</font></strong></td>
        </tr>
      <tr>
        <td width="41%" align="right">&nbsp;</td>
        <td width="59%">&nbsp;</td>
      </tr>
      <tr>
        <td align="right"><input name="fecha_desde" type="text" class="style9" id="fecha_desde" value="<?PHP echo $fechamostrar; ?>" size="10" maxlength="10" readonly="readonly" onchange="FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');"/></td>
        <td><input name="desde" type="button" onclick="displayCalendar(document.getElementById('fecha_desde'),'dd/mm/yyyy',this);" value="Desde fecha:" /></td>
      </tr>
      <tr>
        <td align="right"><input name="fecha_hasta" type="text" class="style9" id="fecha_hasta" value="<?PHP echo $fechamostrar; ?>" size="10" maxlength="10" readonly="readonly" onchange="FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');"/></td>
        <td><input name="hasta" type="button" onclick="displayCalendar(document.getElementById('fecha_hasta'),'dd/mm/yyyy',this);" value="Hasta fecha:" /></td>
      </tr>
    </table></td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    <td colspan="2">&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    <td colspan="2"><div id="mostrar">
      <textarea name="textarea" cols="45" rows="5" disabled="disabled">Seleccione un m&eacute;dico
                                    </textarea>
    </div></td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    <td colspan="2">&nbsp;</td>
    <td width="546" rowspan="23" valign="top"><table width="100%" border="0" align="center" cellpadding="1">
        <tr>
          <td valign="top"><div id="mostrarfecha" align="left"></div></td>
        </tr>
        </table></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td><font size="-1" face="Verdana, Arial, Helvetica, sans-serif">Tipo de cita:</font></td>
    <td><span style="padding-right: 50; padding-bottom: 10">
      <select name="tipocita" id="tipocita" onchange="FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');FAjax('mostrar_datos_medicos.php','mostrar','medico='+document.getElementById('medico').value,'POST');FAjax('mostrar_datos_tipocita.php','mostrartipocita','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value,'POST');" >
      </select>
    </span></td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    <td colspan="2"><div id="mostrartipocita"></div></td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    <td colspan="2" rowspan="19" valign="top"><table width="100%" border="0" align="center" cellpadding="1">
        <tr>
          <td><div align="center">
              <?php require_once('mostrar_calendario.php'); ?>
          </div></td>
        </tr>
        </table></td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
  <tr>
    <td>&nbsp;</td>
    </tr>
</table>
<td>
<tr>
</table>

</body>
</html>
