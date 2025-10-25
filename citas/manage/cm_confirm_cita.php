<?php session_start();
//if (!isset($_SESSION['tipocita']) || !isset($_SESSION['medico']) || !isset($_SESSION['ci_usuario']) ) {
//   echo "Acceso restringido, esta tratando de entrar de forma incorrecta.";
//   exit();
//}
if (!isset($_SESSION['fecha_cita']) || !isset($_SESSION['ci_usuario']) ) {
   echo "Acceso restringido, esta tratando de entrar de forma incorrecta.";
   exit();
}

require_once('connections/conexion2.php'); ?>
<?php
$continuar = 0;

if (isset($_GET['fecha'])) {
  $_SESSION['fecha'] = $_GET['fecha'];
}
if (isset($_GET['numero']) ) {
  $_SESSION['numero'] = $_GET['numero'];
}
if (isset($_GET['hora']) && !isset($_SESSION['hora_cita'])) {
  $_SESSION['hora_cita'] = $_GET['hora'];
}


if ((isset($_POST["MM_insert"])) && ($_POST["MM_insert"] == "form1")) {
	if (!isset($_SESSION['ci_usuario']) || !isset($_SESSION['fecha']) || !isset($_SESSION['numero']) ) {
	    echo "Acceso restringido, debe ingresar usuando su usuario y clave en la opcion LOGIN del menu.";
        exit();
	}
	$colname_rs_cita = "-1";
	if (isset($_SESSION['fecha'])) {
	  $colname_rs_cita = (get_magic_quotes_gpc()) ? $_SESSION['fecha'] : addslashes($_SESSION['fecha']);
	}
	$colname2_rs_cita = "-1";
	if (isset($_SESSION['ci_usuario'])) {
	  $colname2_rs_cita = (get_magic_quotes_gpc()) ? $_SESSION['ci_usuario'] : addslashes($_SESSION['ci_usuario']);
	}
	mysql_select_db($database_conexion2, $conexion2);
	$query_rs_cita = sprintf("SELECT * FROM 0_cm_citas WHERE fecha_cita = '%s' and ci_usuario = '%s'", $colname_rs_cita,$colname2_rs_cita);
	$rs_cita = mysql_query($query_rs_cita, $conexion2) or die(mysql_error());
	$row_rs_cita = mysql_fetch_assoc($rs_cita);
	$totalRows_rs_cita = mysql_num_rows($rs_cita);
	if ($totalRows_rs_cita) {
 	    header("Location: ". "cm_mensaje_menu.php?mensaje=El paciente tiene ya tiene una cita para la fecha ".$_SESSION['fecha']."&coderror=A01"."&mensaje3=Regresar a Citas" );
        $continuar = 0;
		}
	else {$continuar = 1;}
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

if ($continuar == 1) {
	if ((isset($_POST["MM_insert"])) && ($_POST["MM_insert"] == "form1")) {
	  $insertSQL = sprintf("INSERT INTO 0_cm_citas (fecha_cita, fechora_reg, ci_usuario, observacion, tipocita, numero, hora_cita, codmed, codestatus) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)",
						   GetSQLValueString($_POST['fecha_cita'], "date"),
						   GetSQLValueString(date("Y/m/d h:i"), "date"),
						   GetSQLValueString($_SESSION['ci_usuario'], "text"),
						   GetSQLValueString($_POST['observacion'], "text"),
						   GetSQLValueString($_SESSION['tipocita'], "text"),
						   GetSQLValueString($_POST['numerocita'], "text"),
						   GetSQLValueString($_SESSION['hora_cita'], "text"),
						   GetSQLValueString($_SESSION['medico'], "text"),
						   '0');
	
	  mysql_select_db($database_conexion2, $conexion2);
	  $Result1 = mysql_query($insertSQL, $conexion2) or die(mysql_error());
	}
	
	if ((isset($_POST["MM_update"])) && ($_POST["MM_update"] == "form1")) {
	  $updateSQL = sprintf("UPDATE 0_cm_fechasxsemana SET tomado='1' WHERE numero=%s and codmed=%s and fecha=%s",
						   GetSQLValueString($_POST['numerocita'], "text"),
						   GetSQLValueString($_SESSION['medico'], "text"),
						   GetSQLValueString($_POST['fecha_cita'], "text"));
	
	  mysql_select_db($database_conexion2, $conexion2);
	  $Result1 = mysql_query($updateSQL, $conexion2) or die(mysql_error());
	  //para el caso que se este cambiando la cita se debe borrar la cita anterior
	  if (isset($_SESSION['cambiocita']) && $_SESSION['cambiocita'] == 'S') {
	     if (isset($_SESSION['numerocita_ant'])) $num_cita_ant = $_SESSION['numerocita_ant'];
		 if (isset($_SESSION['medico_ant'])) $medico_ant = $_SESSION['medico_ant'];
		 if (isset($_SESSION['fecha_cita_ant'])) $fecha_ant = $_SESSION['fecha_cita_ant'];
		 $updateSQL = sprintf("UPDATE 0_cm_fechasxsemana SET tomado='0' WHERE numero=%s and codmed=%s and fecha=%s",
						   GetSQLValueString($num_cita_ant, "text"),
						   GetSQLValueString($medico_ant, "text"),
						   GetSQLValueString($fecha_ant, "text"));
	
		  mysql_select_db($database_conexion2, $conexion2);
		  $Result1 = mysql_query($updateSQL, $conexion2) or die(mysql_error());
		  if ($Result1) {
			  //Borrar citas
			  if (isset($_SESSION['cedulaborrar'])) $cedula_borrar = $_SESSION['cedulaborrar'];
			  $deleteSQL = sprintf("DELETE FROM 0_cm_citas WHERE fecha_cita=%s and ci_usuario=%s",
							   GetSQLValueString($fecha_ant, "text"),
							   GetSQLValueString($cedula_borrar, "text"));
			  mysql_select_db($database_conexion2, $conexion2);
			  $Result1 = mysql_query($deleteSQL, $conexion2) or die(mysql_error());	
			  if ($Result1) {
				 $ciusuario = $_SESSION['ci_usuario'] ;
				 $cedula = $cedula_borrar ;
				 $fecha_cita = $fecha_ant;
				 $numerocita = $num_cita_ant;
				 require_once('cm_auditoria.php'); 
				 auditoria($numerocita,$ciusuario,$fecha_cita,$cedula,'A',$database_conexion2,$conexion2);
				 auditoria($_POST['numerocita'],$ciusuario,$_POST['fecha_cita'],$cedula,'C',$database_conexion2,$conexion2);
			   }
               //$_SESSION = array();
               //session_destroy(); //Para inhabilitar todas las variables de session   			   
		   } 
		}   
	  
	}
	$fechaimp = $_SESSION['fecha'] ;
	$numeroimp = $_SESSION['numero'] ;
	$horaimp = $_SESSION['hora_cita'] ;
	$cedulaimp = $_SESSION['ci_usuario'] ;
	$medico = $_SESSION['medico'];
	$tipocita = $_SESSION['tipocita'];
	$motivo = $_POST['observacion'];
	if ($_SESSION['MM_UserGroup'] != "A" ) {
	    $_SESSION = array();
        session_destroy(); //Para inhabilitar todas las variables de session
	}
	
	   
   header("Location: ". "cm_mensaje_menu.php?mensaje=La solicitud ha sido creada satisfactoriamente&coderror=000&mensaje2=Imprimir cita&fecha=".$fechaimp."&numero=".$numeroimp."&hora=".$horaimp."&cedula=".$cedulaimp."&medico=".$medico."&tipocita=".$tipocita."&motivo".$motivo."&mensaje3=Regresar a Citas"  );
}

$colname_rs_paciente = "-1";
if (isset($_SESSION['ci_usuario'])) {
  $colname_rs_paciente = (get_magic_quotes_gpc()) ? $_SESSION['ci_usuario'] : addslashes($_SESSION['ci_usuario']);
}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_paciente = sprintf("SELECT * FROM 0_users WHERE ci_usuario = '%s'", $colname_rs_paciente);
$rs_paciente = mysql_query($query_rs_paciente, $conexion2) or die(mysql_error());
$row_rs_paciente = mysql_fetch_assoc($rs_paciente);
$totalRows_rs_paciente = mysql_num_rows($rs_paciente);

$colname_rs_tipo_cita = "-1";
if (isset($_SESSION['tipocita'])) {
  $colname_rs_tipo_cita = (get_magic_quotes_gpc()) ? $_SESSION['tipocita'] : addslashes($_SESSION['tipocita']);
}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_tipo_cita = sprintf("SELECT * FROM 0_cm_system_tables WHERE codigotipo = '%s' and codigotabla = 'TPCITA'", $colname_rs_tipo_cita);
$rs_tipo_cita = mysql_query($query_rs_tipo_cita, $conexion2) or die(mysql_error());
$row_rs_tipo_cita = mysql_fetch_assoc($rs_tipo_cita);
$totalRows_rs_tipo_cita = mysql_num_rows($rs_tipo_cita);

$colname_rs_medico = "-1";
if (isset($_SESSION['medico'])) {
  $colname_rs_medico = (get_magic_quotes_gpc()) ? $_SESSION['medico'] : addslashes($_SESSION['medico']);
}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_medico = sprintf("SELECT * FROM 0_medico_master WHERE medico_no = %s", $colname_rs_medico);
$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
$row_rs_medico = mysql_fetch_assoc($rs_medico);
$totalRows_rs_medico = mysql_num_rows($rs_medico);
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>Centro Vascular y Cuidado Integral de Heridas</title>
<script language="JavaScript">
<!--



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

//-->//-->
</script>

<link href="ccs/hojaestilo.css" rel="stylesheet" type="text/css">
<style type="text/css">
<!--
.style10 {font-size: 10px; font-family: Arial, Helvetica, sans-serif;}
.style11 {font-family: Arial, Helvetica, sans-serif;
	font-size: 10px;
	font-weight: bold;
}
.style2 {color: #FFFFFF}
-->
</style>
</head>
<body>
<table width="760" border="0" cellspacing="0" cellpadding="0">
  <tr> 
    <td><table width="760" border="0" align="right"  cellpadding="0" cellspacing="0">
        <tr> 
          <td width="78%" height="365" valign="top"><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr> 
                <td><p><font size="4" ><span class="titulotop1">Citas</span></font><font color="#FFFFFF" size="1" face="Arial, Helvetica, sans-serif"><a name="paginas_web" id="paginas_web"></a></font></p>
                  <table width="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
                    <tr> 
                      <td width="9" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
                      <td width="353" height="14" colspan="2" bgcolor="#999999"><strong><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Verifique los Datos de su Cita y Confirmela </font></strong></td>
                    </tr>
                    <tr> 
                      <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                          <tr> 
                            <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
                                <tr> 
                                  <td width="96%" valign="top"> 
                                    <form id="form1" name="form1" method="POST" action="<?php echo $editFormAction; ?>">
                                      <table width="527" border="1" cellpadding="1" cellspacing="0">
                                        <tr>
                                          <td colspan="3" align="center" bgcolor="#CC6666" scope="col"><span class="style2">Solicitud de Cita M&eacute;dica <?PHP if (isset($_SESSION['cambiocita']) && ($_SESSION['cambiocita'] == "S")) {echo " - Cambiando una Cita";}?> </span></td>
                                        </tr>
                                        <tr>
                                          <td width="176"   align="left"><span class="style11"><strong>Fecha Cita:</strong></span><span class="style10">
                                            <?PHP  if (isset($_SESSION['fecha'])) {echo $_SESSION['fecha'];} ?>
                                          </span></td>
                                          <td width="182" align="left" class="style11">Tipo Cita:<span class="style10"><?php echo $row_rs_tipo_cita['descripcion']; ?></span></td>
                                          <td width="155" align="left" class="style11">&nbsp;</td>
                                        </tr>
                                        <tr>
                                          <td align="left" class="style11">M&eacute;dico:<span class="style10"><?php echo $row_rs_medico['medico_no'].", ".$row_rs_medico['name']; ?></span></td>
                                          <td align="left"><span class="style11">N&uacute;mero de Cita:</span><span class="style10">
                                            <?PHP if (isset($_SESSION['numero'])) {echo $_SESSION['numero'];} ?>
                                          </span></td>
                                          <td align="left"><span class="style11">Hora:<span class="style10">
                                            <?PHP if (isset($_SESSION['hora_cita'])) {echo $_SESSION['hora_cita'];} ?>
                                          </span></span></td>
                                        </tr>
                                        <tr>
                                          <td align="left"><span class="style11">C&eacute;dula:</span><span class="style10">
                                            <?PHP if (isset($_SESSION['ci_usuario'])) {echo $_SESSION['ci_usuario'];} ?>
                                          </span></td>
                                          <td colspan="2" align="left" class="style11">Explique brevemente su motivo de consulta:                                          </td>
                                        </tr>
                                        <tr>
                                          <td align="left"><span class="style11">Nombre: <span class="style10"><?php echo $row_rs_paciente['real_name']; ?></span></span></td>
                                          <td colspan="2" rowspan="3" align="left" valign="top"><div style="padding-left: 2; padding-top: 1"><span class="style11">
                                            <textarea name="observacion" cols="48" rows="2" class="style11" id="observacion"></textarea>
                                          </span></div></td>
                                        </tr>
                                        <tr>
                                          <td align="left"><span class="style11">Apellidos: <span class="style10"></span></span></td>
                                        </tr>
                                        <tr>
                                          <td align="left"><span class="style11">Tel&eacute;fono:<?php echo $row_rs_paciente['phone']; ?></span></td>
                                        </tr>
                                        <tr>
                                          <td align="right" class="style11"><input name="numerocita" type="hidden" class="style11" id="numerocita" value="<?PHP echo $_SESSION['numero']; ?>" size="3" maxlength="3" />
                                              <input name="fecha_cita" type="hidden" class="style11" id="fecha_cita" value="<?PHP if (isset($_SESSION['fecha'])) {echo $_SESSION['fecha'];} ?>" size="10" maxlength="10" />
                                              <input name="ci_usuario" type="hidden" class="style11" id="ci_usuario" value="<?PHP if (isset($_SESSION['ci_usuario'])) {echo $_SESSION['ci_usuario'];} ?>" size="10" maxlength="10" />
                                              <a href="cm_selec_medico.php">Modificar</a></td>
                                          <td colspan="2"><input name="guardar" type="submit" id="guardar" value="Guardar" />
                                          <input type="hidden" name="MM_insert" value="form1">
                                          <input type="hidden" name="MM_update" value="form1"></td>
                                        </tr>
                                      </table>				  
                                    </form>								   </td>
                                </tr>
                              </table>							  </td>
                          </tr>
                        </table>						</td>
                    </tr>
                  </table>				  </td>
              </tr>
            </table>          </td>
        </tr>
      </table></td>
  </tr>
</table>

</body>
</html>
<?php
mysql_free_result($rs_paciente);
?>