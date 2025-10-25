<?PHP session_start(); ?>
<?php require_once('connections/conexion2.php'); ?>
<?php

$colname_rs_tipo_cita = "-1";
if (isset($_POST['medico'])) {
  $colname_rs_tipo_cita = (get_magic_quotes_gpc()) ? $_POST['medico'] : addslashes($_POST['medico']);
 }
else {
if (isset($_SESSION['medico'])) {
$colname_rs_tipo_cita = (get_magic_quotes_gpc()) ? $_SESSION['medico'] : addslashes($_SESSION['medico']);
}
}
if (isset($_POST['medico'])) {
$_SESSION['medico'] = $_POST['medico'];}
$maxRows_rs_tipo_cita = 10;
$pageNum_rs_tipo_cita = 0;
if (isset($_GET['pageNum_rs_tipo_cita'])) {
  $pageNum_rs_tipo_cita = $_GET['pageNum_rs_tipo_cita'];
}
$startRow_rs_tipo_cita = $pageNum_rs_tipo_cita * $maxRows_rs_tipo_cita;

//$colname_rs_tipo_cita = "-1";
//if (isset($_POST['medico'])) {
//  $colname_rs_tipo_cita = (get_magic_quotes_gpc()) ? $_POST['medico'] : addslashes($_POST['medico']);
//}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_tipo_cita = "SELECT tc.tipocita,  tb.descripcion, tb.comentario, tb.vermas, tb.titulo_vermas, tc.precio FROM 0_cm_tpcita_x_medico tc, 0_cm_system_tables tb WHERE tc.tipocita = tb.codigotipo and tb.codigotabla = 'TPCITA' and tc.codmed = '$colname_rs_tipo_cita'";
$rs_tipo_cita = mysql_query($query_rs_tipo_cita, $conexion2) or die(mysql_error());
$row_rs_tipo_cita = mysql_fetch_assoc($rs_tipo_cita);
$totalRows_rs_tipo_cita = mysql_num_rows($rs_tipo_cita);

$colname_rs_medico = "-1";
if (isset($_POST['medico'])) {
  $colname_rs_medico = (get_magic_quotes_gpc()) ? $_POST['medico'] : addslashes($_POST['medico']);
}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_medico = sprintf("SELECT ci_medico, medico_no, name FROM 0_medico_master WHERE medico_no = '%s'", $colname_rs_medico);
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

<link href="ccs/hojaestilo.css" rel="stylesheet" type="text/css"> 
<style type="text/css">
<!--
.style4 {font-size: 12px}
.style11 {	color: #FF0000;
	font-size: 12px;
}
.style16 {font-size: 12}
-->
</style>
</head>
<body>
<table width="680" border="0" cellspacing="0" cellpadding="0">
  <tr> 
    <td><table width="678" border="0" align="right"  cellpadding="0" cellspacing="0">
        <tr> 
          <td width="78%" height="365" valign="top"><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr> 
                <td><p><font size="4" ><span class="titulotop1">Citas</span></font></p>
                  <table width="80%" border="0" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
                    <tr> 
                      <td width="9" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
                      <td width="353" height="14" colspan="2" bgcolor="#999999"><strong><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Seleccione el Tipo de Cita </font></strong></td>
                    </tr>
                    <tr> 
                      <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                          <tr> 
                            <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
                                <tr> 
                                  <td width="4%">&nbsp;</td>
                                  <td width="96%" valign="top"> <p align="justify"><font size="-1" face="Verdana, Arial, Helvetica, sans-serif">Para el m&eacute;dico:</font> <span class="style11"><?php echo $row_rs_medico['medico_no'].", "; ?><span class="style16"><?php echo $row_rs_medico['name']; ?></span></span></p>
                                    <!-- buscador  Google -->
                                    <form method="post" action="cm_selec_diacita.php">
                                      <table bgcolor="#ffffff" cellspacing="0" border="0">
                                        <tbody>
                                          <tr valign="middle"> 
                                            <!--<td> <a href="http://www.google.com/"> <img
 src="http://www.google.com/logos/Logo_40wht.gif" border="0"
 alt="Google">
                </a> <br>
                      </td>-->
                                            <td width="538"> <table border="0" width="414" cellspacing="0" cellpadding="0">
                                            
                                              <tr>
                                                <td valign="top" style="padding-right: 50; padding-bottom: 10"><label></label>
                                                    <label></label>
                                                    <label>
                                                  <br>
                                                    <table width="500" border="1" align="center" cellpadding="1" cellspacing="0">
                                                      <tr>
                                                        <td width="139">Tipo de cita </td>
                                                        <td width="34">Acci&oacute;n</td>
                                                        <td width="47">Descripci&oacute;n</td>
                                                        <td width="47" align="right">Valor</td>
                                                        <td>Saber M&aacute;s </td>
                                                      </tr>
                                                      <?PHP $Reg = 0; ?>
                                                      <?php do { $Reg++?>
                                                      <tr>
                                                        <td><?php echo $row_rs_tipo_cita['descripcion']; ?></td>
                                                        <td><input name="tipocita" type="radio" value="<?PHP echo $row_rs_tipo_cita['tipocita']; ?>" <?PHP if ($Reg == 1) {echo "checked";} ?>></td>
                                                        <td><?php echo $row_rs_tipo_cita['comentario']."".$row_rs_tipo_cita['vermas']; ?></td>
                                                        <td align="right"><?php echo $row_rs_tipo_cita['precio']; ?></td>
                                                        <td><a href="<?php echo $row_rs_tipo_cita['vermas']; ?>" target="_blank"><?php echo $row_rs_tipo_cita['titulo_vermas']; ?></a></td>
                                                      </tr>
                                                      <?php } while ($row_rs_tipo_cita = mysql_fetch_assoc($rs_tipo_cita)); ?>
                                                    </table>
                                                  </label></td>
                                              </tr>
                                              <tr>
                                                <td align="center" valign="top" style="padding-right: 50; padding-bottom: 10"><input name="aceptar" type="submit" id="aceptar" value="Aceptar" /></td>
                                              </tr>
											  <tr>
                                                <td align="left" valign="top" ><BR><a href="cm_selec_medico.php"><img src="imagenes/back_red_s.gif" width="83" height="17" border="0"></a></td>
                                              </tr>
                                            </table>                                            </td>
                                          </tr>
                                        </tbody>
                                      </table>
                                    </form>
                                    <!-- fin buscador Google -->
                                    
                                      <!--Buscador Free find-->
                                    <!--fin buscador Freefind-->                                  </td>
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
mysql_free_result($rs_tipo_cita);
?>