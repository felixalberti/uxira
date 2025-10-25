<?PHP session_start(); ?>
<?php require_once('connections/conexion2.php'); ?>
<?php
//if (((isset($_SESSION['MM_UserGroup'])) && ($_SESSION['MM_UserGroup'] != "A")) &&
   //((isset($_SESSION['cambiocita'])) && ($_SESSION['cambiocita'] != "S" ))) {
   $_SESSION = array();
   session_destroy(); //Para inhabilitar todas las variables de session 
//} 
$_SESSION['fecha_cita'] = date("Y/m/d");

mysql_select_db($database_conexion2, $conexion2);
$query_rs_medico = "SELECT * FROM 0_medico_master ORDER BY medico_no";
$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
$row_rs_medico = mysql_fetch_assoc($rs_medico);
$totalRows_rs_medico = mysql_num_rows($rs_medico);

?>


<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
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
</script> 
<link href="ccs/hojaestilo.css" rel="stylesheet" type="text/css"> 

<style type="text/css">
<!--
.style4 {font-size: 12px}
-->
</style>
</head>
<body onLoad="cargar('medicos_xml.php','medico');">
<table width="1180" border="0" cellspacing="0" cellpadding="0">
  <tr> 
    <td><table width="1178" border="0" align="right"  cellpadding="0" cellspacing="0">
        <tr> 
          <td width="78%" height="365" valign="top"><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr> 
                <td><p><font size="4" ><span class="titulotop1">Citas</span></font></p>
                  <table width="80%" border="0" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
                    <tr> 
                      <td width="9" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
                      <td width="353" height="14" colspan="2" bgcolor="#999999"><strong><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Seleccione el M&eacute;dico </font></strong></td>
                    </tr>
                    <tr> 
                      <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                          <tr> 
                            <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
                                <tr> 
                                  <td width="4%">&nbsp;</td>
                                  <td width="96%" valign="top"> <p align="justify"><font size="-1" face="Verdana, Arial, Helvetica, sans-serif">M&eacute;dico:</font>
                                    <!-- buscador  Google -->
                                    </p>
                                    <table border="0" width="872" cellspacing="0" cellpadding="0">
                                      <tr>
                                        <td width="852" valign="top" style="padding-right: 50; padding-bottom: 10"><form id="form1" name="form1" method="post" action="">
                                          <p>
                                            <select name="medico" size="1" id="medico" onChange="cargar('tipos_citas_x_medico_xml.php?codmed='+document.getElementById('medico').value,'tipocita');FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+'001','POST');FAjax('mostrar_datos_medicos.php','mostrar','medico='+document.getElementById('medico').value,'POST');">
                                            </select>
                                    <p>
                                    <div id="mostrar">
                                    <textarea name="textarea" cols="45" rows="5" disabled="disabled">Seleccione un médico
                                    </textarea>
                                    </div>                                    
                                    </p>                                            
                                            <p align="justify">
                                            <font size="-1" face="Verdana, Arial, Helvetica, sans-serif">Tipo de cita:</font>
                                    <!-- buscador  Google -->
                                    </p>
                                   
                                            <select name="tipocita" id="tipocita" onChange="FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');FAjax('mostrar_datos_medicos.php','mostrar','medico='+document.getElementById('medico').value,'POST');FAjax('mostrar_datos_tipocita.php','mostrartipocita','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value,'POST');" >
                                            </select>
                                            <div id="mostrartipocita">
                                            </div>                                            
                                          </p>
                                         <p> <?php //$fechamostrar = date("d/m/Y") ;?> 
                                         <input name="fecha_desde" type="text" class="style9" id="fecha_desde" value="<?PHP echo $fechamostrar; ?>" size="10" maxlength="10" readonly="readonly" onChange="FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');"/>
                                         <input name="desde" type="button" onClick="displayCalendar(document.forms[0].fecha_desde,'dd/mm/yyyy',this);" value="Desde fecha:" />
                                         </p>
                                         <p>
                                         <input name="fecha_hasta" type="text" class="style9" id="fecha_hasta" value="<?PHP echo $fechamostrar; ?>" size="10" maxlength="10" readonly="readonly" onChange="FAjax('mostrar_datos_fecha_cita.php','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');"/>
                                         <input name="hasta" type="button" onClick="displayCalendar(document.forms[0].fecha_hasta,'dd/mm/yyyy',this);" value="Hasta fecha:" />                                                                                                                                                                            
                                         </p>                                          
                                         <p>
                                         <?php require_once('mostrar_calendario.php'); ?>                                         	
                                         </p>
                                         <p>
                                         <!--<input name="aceptar" type="submit" id="aceptar" value="Aceptar" /> -->                                         
                                         <div id="mostrarfecha"></div>
                                         </p>                                        
										
                                        </form></td>
                                        										
                                      </tr>
                                    </table>                                   
                                   
                                      <!--Buscador Free find-->
                                    <!--fin buscador Freefind--></td>
                                </tr>
                              </table></td>
                          </tr>
                        </table></td>
                    </tr>
                  </table></td>
              </tr>
            </table>          </td>
        </tr>
      </table></td>
  </tr>
</table>

</body>
</html>
<?php
mysql_free_result($rs_medico);
?>