<?php require_once('connections/conexion2.php'); ?>
<?php
$colname_rs_medico = "-1";
if (isset($_GET['medico'])) {
  $colname_rs_medico = (get_magic_quotes_gpc()) ? $_GET['medico'] : addslashes($_GET['medico']);
}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_medico = sprintf("SELECT * FROM 0_medico_master WHERE medico_no = %s", $colname_rs_medico);
$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
$row_rs_medico = mysql_fetch_assoc($rs_medico);
$totalRows_rs_medico = mysql_num_rows($rs_medico);

$colname_rs_tipocita = "-1";
if (isset($_GET['tipocita'])) {
  $colname_rs_tipocita = (get_magic_quotes_gpc()) ? $_GET['tipocita'] : addslashes($_GET['tipocita']);
}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_tipocita = sprintf("SELECT * FROM 0_cm_system_tables WHERE codigotabla = 'TPCITA' and codigotipo = '%s'", $colname_rs_tipocita);
$rs_tipocita = mysql_query($query_rs_tipocita, $conexion2) or die(mysql_error());
$row_rs_tipocita = mysql_fetch_assoc($rs_tipocita);
$totalRows_rs_tipocita = mysql_num_rows($rs_tipocita);

$colname_rs_paciente = "-1";
if (isset($_GET['ci'])) {
  $colname_rs_paciente = (get_magic_quotes_gpc()) ? $_GET['ci'] : addslashes($_GET['ci']);
}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_paciente = sprintf("SELECT * FROM 0_users WHERE ci_usuario = '%s'", $colname_rs_paciente);
$rs_paciente = mysql_query($query_rs_paciente, $conexion2) or die(mysql_error());
$row_rs_paciente = mysql_fetch_assoc($rs_paciente);
$totalRows_rs_paciente = mysql_num_rows($rs_paciente);
 session_start();


require_once('connections/conexion2.php'); ?>
<?php
$continuar = 0;

if (isset($_GET['fecha'])) {
  $_SESSION['fecha'] = $_GET['fecha'];
}
if (isset($_GET['numero']) ) {
  $_SESSION['numero'] = $_GET['numero'];
}



function GetSQLValueString($theValue, $theType, $theDefinedValue = "", $theNotDefinedValue = "") 
{
  $theValue = (!get_magic_quotes_gpc()) ? addslashes($theValue) : $theValue;

  switch ($theType) {
    case "text":
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;    
    case "long":
    case "int":
      $theValue = ($theValue != "") ? intval($theValue) : "NULL";
      break;
    case "double":
      $theValue = ($theValue != "") ? "'" . doubleval($theValue) . "'" : "NULL";
      break;
    case "date":
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;
    case "defined":
      $theValue = ($theValue != "") ? $theDefinedValue : $theNotDefinedValue;
      break;
  }
  return $theValue;
}

$editFormAction = $_SERVER['PHP_SELF'];
if (isset($_SERVER['QUERY_STRING'])) {
  $editFormAction .= "?" . htmlentities($_SERVER['QUERY_STRING']);
}
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>Centro Vascular y Cuidado Integral de Heridas</title>
<script language="JavaScript">

function ImprimirCentrado(Url,NombreVentana,width,height,extras) {
var largo = width;
var altura = height;
var adicionales= extras;
var top = (screen.height-altura)/2;
var izquierda = (screen.width-largo)/2; nuevaVentana=window.open(''+ Url + '',''+ NombreVentana + '','width=' + largo + ',height=' + altura + ',top=' + top + ',left=' + izquierda + ',features=' + adicionales + ''); 
nuevaVentana.focus();
}
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

function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}
//-->
</script>

<link href="ccs/hojaestilo.css" rel="stylesheet" type="text/css">
<style type="text/css">
<!--
.style1 {	color: #CC0000;
	font-weight: bold;
}
.style4 {font-size: 12px}
.style10 {font-size: 10px; font-family: Arial, Helvetica, sans-serif;}
.style11 {font-family: Arial, Helvetica, sans-serif;
	font-size: 10px;
	font-weight: bold;
}
.style2 {color: #FFFFFF}
-->
</style>
</head>
<body onLoad="window.print();" >
<table width="760" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td>
      <table width="760" border="0" cellspacing="0" cellpadding="0">
  <tr> 
    <td width="348"><img src="imagenes/logotrasparente.png" name="LogoAngios" width="170" height="52" border="0" id="LogoAngios"></td>
    <td width="411" colspan="8" align="right"><font size="4" face="Verdana, Arial, Helvetica, sans-serif"><span class="style1">Centro Vascular <BR>Cuidado Integral de Heridas</span></font></td>
  </tr>
  <tr valign="top"> 
    <td height="22" colspan="9">&nbsp;</td>
  </tr>
</table>
</td>
  </tr>
  <tr> 
    <td><table width="760" border="0" align="right"  cellpadding="0" cellspacing="0">
        <tr> 
          <td width="19%" rowspan="2" valign="top" bgcolor="#FFFFFF">&nbsp;</td>
          <td width="3%" height="365" rowspan="2" valign="top"> <br> </td>
          <td width="78%" valign="top"><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr> 
                <td><p>&nbsp;</p>
                  <table width="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
                    <tr> 
                      <td width="9" height="14" align="left" valign="top" bgcolor="#FFFFFF">&nbsp;</td>
                      <td width="353" height="14" colspan="2" bgcolor="#FFFFFF">&nbsp;</td>
                    </tr>
                    <tr> 
                      <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                          <tr> 
                            <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
                                <tr> 
                                  <td width="96%" valign="top"> 
                                    <form id="form1" name="form1" method="POST" action="<?php echo $editFormAction; ?>">
                                      <table width="525" border="1" cellpadding="1" cellspacing="0">
                                        <tr>
                                          <td colspan="3" align="center" bgcolor="#CC6666" scope="col"><span class="style2">Datos de su Cita </span></td>
                                        </tr>
                                        <tr>
                                          <td width="178"   align="left"><span class="style11"><strong>Fecha Cita:</strong></span><span class="style10">
                                            <?PHP  if (isset($_GET['fecha'])) {echo $_GET['fecha'];} ?>
                                          </span></td>
                                          <td width="181" align="left" class="style11">Tipo Cita:<span class="style10"><?php echo $row_rs_tipocita['descripcion']; ?> </span></td>
                                          <td width="152" align="left" class="style11">Hora: <span class="style10">
                                            <?PHP if (isset($_GET['hora'])) {echo $_GET['hora'];} ?>
                                          </span></td>
                                        </tr>
                                        <tr>
                                          <td align="left" class="style11">M&eacute;dico:<span class="style10"><?php echo $row_rs_medico['medico_no']." ".$row_rs_medico['name']; ?></span></td>
                                          <td colspan="2" align="left"><span class="style11">N&uacute;mero de Cita:</span><span class="style10">
                                            <?PHP if (isset($_GET['numero'])) {echo $_GET['numero'];} ?>
                                          </span></td>
                                        </tr>
                                        <tr>
                                          <td align="left"><span class="style11">C&eacute;dula:</span><span class="style10">
                                            <?PHP if (isset($_GET['ci'])) {echo $_GET['ci'];} ?>
                                          </span></td>
                                          <td colspan="2" align="left" class="style11">                                          </td>
                                        </tr>
                                        <tr>
                                          <td align="left"><span class="style11">Nombre: <span class="style10"><?php echo $row_rs_paciente['real_name']; ?> </span></span></td>
                                          <td colspan="2" rowspan="3" align="left" valign="top"><div style="padding-left: 2; padding-top: 1"><?PHP if (isset($_GET['motivo'])) {echo $_GET['motivo'];} ?></div></td>
                                        </tr>
                                                                                
                                      </table>
                                    </form>                                  </td>
                                </tr>
                              </table></td>
                          </tr>
                        </table></td>
                    </tr>
                  </table></td>
              </tr>
              
              
            </table> 
          </td>
        </tr>
        <tr>
          <td align="right" valign="bottom">
            <span class="normalGrisHover"><a href="/comentarios/" onMouseOver="MM_swapImage('Image21111','','/imagenes/segundonivel/comentarios-over.gif',1)" onMouseOut="MM_swapImgRestore()"></a><br>
            </span>
            <table width="300" border="0" align="right" cellpadding="0" cellspacing="0" bgcolor="#999999">
              <tr>
                <td width="11" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
                <td width="222" height="14" bgcolor="999999"><div align="center"><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Nuestra Direcci&oacute;n y Tel&eacute;fonos </font></div></td>
              </tr>
              <tr>
                <td height="35" colspan="2"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#999999">
                    <tr>
                      <td ><table width="100%" border="0" align="center" cellpadding="5" cellspacing="0" bgcolor="#FFFFFF">
                          <tr>
                            <td colspan="2" align="center"><p align="left"><font size="-2" face="Verdana, Arial, Helvetica, sans-serif">www.angios.com<br>
                                      <a href="">info@angios.com</a><br>
                                      Tel&eacute;fono +58 212 993-5064. 
                                                  993-6651 <br>
Edificio. 147, Av. Principal de Chuao. Angios. Frente a el m&oacute;dulo de la polic&iacute;a Baruta. Chuao. <br>
                              Caracas, Venezuela <br>
                            </font></p></td>
                          </tr>
                      </table></td>
                    </tr>
                </table></td>
              </tr>
            </table>
            <span class="normalGrisHover"><span class="style4"><font face="Verdana, Arial, Helvetica, sans-serif"></font></span></span> </td>
        </tr>
      </table></td>
  </tr>
</table>

</body>
</html>
<?php
mysql_free_result($rs_medico);

mysql_free_result($rs_tipocita);

mysql_free_result($rs_paciente);
?>