<?php session_start(); ?>
<?php require_once('connections/conexion2.php');
$_SESSION = array();
session_destroy(); //Para inhabilitar todas las variables de session   ?>
<?php
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

//$editFormAction = $_SERVER['PHP_SELF'];
$editFormAction = "valid_registro.php";
//if (isset($_SERVER['QUERY_STRING'])) {
//  $editFormAction .= "?" . htmlentities($_SERVER['QUERY_STRING']);
//}

 

if ((isset($_POST["MM_insert"])) && ($_POST["MM_insert"] == "form1")) {
   $loginUsername=$_POST['user_id'];
   $LoginRS__query=sprintf("SELECT user_id FROM 0_users WHERE user_id='%s' or cedula='%s'",  get_magic_quotes_gpc() ? $loginUsername : addslashes($loginUsername), $_POST['cedula']); 
   mysql_select_db($database_conexion2, $conexion2);   
   $LoginRS = mysql_query($LoginRS__query, $conexion2) or die(mysql_error());
   $rows = mysql_num_rows($LoginRS);
   if ($rows > 0) {
      $mensaje = 'El usuario ya existe';
   }
   else {
      echo "Variables:".$_POST['sexo'].$_POST['telefono_ofc'].$_POST['nomreferido'];
      $mensaje = '';  
	  //$clave = "siparano34247";
      //$password=crypt($_POST['password'],$clave);
	  $password = md5($_POST['password']);
	  $insertSQL = sprintf("INSERT INTO 0_users (user_id, password, ci_usuario, real_name, last_name, sexo, phone, celular, telefono_ofc, acercanosotros, nomreferido, aceptoterminos, email) 
	   VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)",
						   GetSQLValueString($_POST['user_id'], "text"),
						   GetSQLValueString($password, "text"),
						   GetSQLValueString($_POST['cedula'], "text"),	
						   GetSQLValueString($_POST['real_name'], "text"),	
						   GetSQLValueString($_POST['last_name'], "text"),	
						   GetSQLValueString($_POST['sexo'], "text"),	
						   GetSQLValueString($_POST['telefono_hab'], "text"),	
						   GetSQLValueString($_POST['celular'], "text"),
						   GetSQLValueString($_POST['telefono_ofc'], "text"),						   
						   GetSQLValueString($_POST['referencia'], "text"),
						   GetSQLValueString($_POST['nomreferido'], "text"),
						   GetSQLValueString((isset($_POST['aceptoterminos']) ? $_POST['aceptoterminos'] : "N"), "text"),
						   GetSQLValueString($_POST['email'], "text"));
	  mysql_select_db($database_conexion2, $conexion2);
	  $Result1 = mysql_query($insertSQL, $conexion2) or die(mysql_error());
	  if ($Result1 = 1) {
	        //$redireccionar = "ingreso_usuario.php?cedula=".$_POST['cedula'];
			$_SESSION['nombreuser'] = $_POST['nombres'].", ".$_POST['apellidos'];
			$redireccionar = "index.php";
	        header("Location: " .$redireccionar );
			exit();
	  }
  }
}

mysql_select_db($database_conexion2, $conexion2);
$query_usuario = "SELECT * FROM 0_users";
$usuario = mysql_query($query_usuario, $conexion2) or die(mysql_error());
$row_usuario = mysql_fetch_assoc($usuario);
$totalRows_usuario = mysql_num_rows($usuario);


mysql_select_db($database_conexion2, $conexion2);
$query_tablasistema = "SELECT * FROM 0_cm_system_tables WHERE codigotabla = 'ACERCA'";
$tablasistema = mysql_query($query_tablasistema, $conexion2) or die(mysql_error());
$row_tablasistema = mysql_fetch_assoc($tablasistema);
$totalRows_tablasistema = mysql_num_rows($tablasistema);
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<?php //require_once('javascript/validacion1.js'); ?>
<head>
<title>Angios: Centro Vascular y Cuidado Integral de Heridas</title>
<SCRIPT type="text/javascript" src="javascript/validacion1.js"></script>
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
.style1 {color: #CC0000;
	font-weight: bold;
}
.style2 {color: #FF0000}
.style5 {color: #000000; font-size: 10px; }
.style7 {color: #000000; font-size: 9px; }
.style8 {color: #000000}
.style9 {color: #666666}
-->
</style>
</head>
<body>
<table width="760" border="0" cellspacing="0" cellpadding="0">
    <tr> 
    <td><table width="760" border="0" align="right"  cellpadding="0" cellspacing="0">
        <tr> 
          <td width="19%" rowspan="2" valign="top" bgcolor="F2F2F2">&nbsp;</td>
          <td width="3%" height="365" rowspan="2" valign="top"> <br> </td>
          <td width="78%" valign="top"><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr> 
                <td><p><font size="4" face="Verdana, Arial, Helvetica, sans-serif"><span class="style1">Citas</span></font></p>
                  <table width="100%" border="0" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
                    <tr> 
                      <td width="9" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
                      <td width="353" height="14" colspan="2" bgcolor="#999999"><strong><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Registrarse<span style="padding-right: 50; padding-bottom: 10"><font face="Arial, Helvetica, sans-serif" size="-1"><font color="#FF0000">
                        <?PHP if (isset($_GET['mensaje']))  {echo "- ".$mensaje;} ?>
                      </font></font></span></font></strong></td>
                    </tr>
                    <tr> 
                      <td colspan="3">
                        <form id="form1" name="form1" method="POST" action="<?php echo $editFormAction; ?>" onSubmit="return ComprobarDatos()">
                          <table width="100%" border="1" cellpadding="0" cellspacing="0" bordercolor="#ECE9D8">
                            <tr>
                              <td align="right" valign="top">&nbsp;</td>
                              <td colspan="3"  valign="top" class="style5" style="padding-right: 50; padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif" class="style2"><font face="Arial, Helvetica, sans-serif" size="-1">(*) </font></font><font size="-1" face="Arial, Helvetica, sans-serif">Campos Obligatorios </font></td>
                            </tr>
                            <tr>
                              <td width="15%" align="right" valign="top"><font face="Arial, Helvetica, sans-serif" size="-1">Usuario<font face="Arial, Helvetica, sans-serif" size="-1">:</font></font></td>
                              <td colspan="3"  valign="top" style="padding-right: 50; padding-bottom: 10"><font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1">
                                <input name="user_id" type="text" id="user_id" tabindex="1"/>
                                <span class="style2">(*)</span> </font><span class="style8">No dejar espacios en blanco </span></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font face="Arial, Helvetica, sans-serif" size="-1">Clave:</font></td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1">
                                <input name="password" type="password" id="password" maxlength="10" />
                                <span class="style2"><font face="Arial, Helvetica, sans-serif" size="-1"> (*) </font></span></font></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font face="Arial, Helvetica, sans-serif" size="-1">Reescriba clave:</font></td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1">
                                <input name="rpassword" type="password" id="rpassword" maxlength="10" />
                                <font size="-1" face="Arial, Helvetica, sans-serif" class="style2"> (*) </font></font></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1">E-mail:</font></font></font></font></td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1">
                                <input name="email" type="text" onBlur="esEmail(this.value)" size="30" />
                                <font size="-1" face="Arial, Helvetica, sans-serif" class="style2"> (*) </font></font></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font face="Arial, Helvetica, sans-serif" size="-1">Reescriba email:</font></td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1">
                                <input name="remail" type="text" size="30" />
                                <font size="-1" face="Arial, Helvetica, sans-serif" class="style2"> (*) </font></font></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font size="-1" face="Arial, Helvetica, sans-serif">C&eacute;dula:</font></td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif">
                                <input name="cedula" type="text" id="cedula" size="10" maxlength="10" />
                                <font size="-1" face="Arial, Helvetica, sans-serif" class="style2"> (*) </font></font></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font size="-1" face="Arial, Helvetica, sans-serif">Nombres:</font></td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif">
                                <input name="real_name" type="text" id="real_name" size="20" maxlength="20" />
                                <font size="-1" face="Arial, Helvetica, sans-serif" class="style2"> (*) </font></font></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font size="-1" face="Arial, Helvetica, sans-serif">Apellidos:</font></td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif">
                                <input name="last_name" type="text" id="last_name" size="20" maxlength="20" />
                                <font size="-1" face="Arial, Helvetica, sans-serif" class="style2"> (*) </font></font></font></td>
                            </tr>
                            <tr>
                              <td height="32" align="right" valign="top" ><font size="-1" face="Arial, Helvetica, sans-serif">Sexo:</font></td>
                              <td height="32" colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif">
                                <select name="sexo" size="1" id="sexo">
                                  <option value="F">Femenino</option>
                                  <option value="M">Masculino</option>
                                </select>
                              <font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif" class="style2">(*) </font></font></font></font></td>
                            </tr>
                            <tr>
                              <td height="32" align="right" valign="top" ><font size="-1" face="Arial, Helvetica, sans-serif">Tel&eacute;fono Hab.:</font></td>
                              <td height="32" colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif">
                                <input name="telefono_hab" type="text" id="telefono_hab" value="(02XX)-" size="20" maxlength="14" />
                              </font></font><font face="Arial, Helvetica, sans-serif"><font face="Arial, Helvetica, sans-serif"><font face="Arial, Helvetica, sans-serif" class="style2"><span class="style5">Ej: (0212)-3456677 (Cambie las X con el c&oacute;digo de area) </span></font></font></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font size="-1" face="Arial, Helvetica, sans-serif">Celular:</font></td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif">
                                <input name="celular" type="text" id="celular" value="(04XX)-" size="20" maxlength="14" />
                              </font></font><font face="Arial, Helvetica, sans-serif"><font face="Arial, Helvetica, sans-serif"><font face="Arial, Helvetica, sans-serif" class="style2"><span class="style7">Ej: (0416)-4567890 (Cabie las X con el c&oacute;digo de la operadora)</span></font></font></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font size="-1" face="Arial, Helvetica, sans-serif">Tel&eacute;fono Ofc.: </font> </td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif">
                                <input name="telefono_ofc" type="text" id="telefono_ofc" value="(02XX)-" size="20" maxlength="14" />
                                <font face="Arial, Helvetica, sans-serif"><font face="Arial, Helvetica, sans-serif"><font face="Arial, Helvetica, sans-serif" class="style2"><span class="style5">Ej: (0212)-3456677 (Cambie las X con el c&oacute;digo de area) </span></font></font></font></font></font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" ><font face="Arial, Helvetica, sans-serif" size="-1">Referido por:</font></td>
                              <td width="39%" valign="top" style="padding-right: 50; padding-bottom: 10"><font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1">
                                <select name="referencia" size="1">
                                  <?php
do {  
?>
                                  <option value="<?php echo $row_tablasistema['codigotipo']?>"><?php echo $row_tablasistema['descripcion']?></option>
                                  <?php
} while ($row_tablasistema = mysql_fetch_assoc($tablasistema));
  $rows = mysql_num_rows($tablasistema);
  if($rows > 0) {
      mysql_data_seek($tablasistema, 0);
	  $row_tablasistema = mysql_fetch_assoc($tablasistema);
  }
?>
                                </select>
                                <font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif" class="style2">(*)</font></font></font></font></td>
                              <td width="12%" align="right" valign="top" ><font face="Arial, Helvetica, sans-serif" size="-1">Nombre:</font></td>
                              <td width="34%" valign="top" style="padding-right: 50; padding-bottom: 10"><input name="nomreferido" type="text" id="nomreferido" size="25" maxlength="25"></td>
                            </tr>
                            <tr>
                              <td colspan="4" align="center" valign="top" style="padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif">Formule una pregunta con su respuesta para ayudarle a recuperar la clave en caso de olvidarla. </font></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" style="padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif">Pregunta:</font></td>
                              <td valign="top" style="padding-right: 50; padding-bottom: 10"><input name="pregunta1" type="text" id="pregunta1" size="30" maxlength="40"></td>
                              <td align="right" valign="top" ><font face="Arial, Helvetica, sans-serif" size="-1">Respuesta:</font></td>
                              <td valign="top" bgcolor="#FFFFFF" style="padding-right: 50; padding-bottom: 10"><input name="resp1" type="text" id="resp1" size="25" maxlength="40"></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" style="padding-bottom: 10"><span class="style9"><font size="-1" face="Arial, Helvetica, sans-serif">Ejemplo:</font></span></td>
                              <td colspan="2" valign="top" style="padding-right: 50; padding-bottom: 10"><span class="style9"><font size="-1" face="Arial, Helvetica, sans-serif">&iquest;Como se llama mi perro? </font></span></td>
                              <td valign="top" bgcolor="#FFFFFF" style="padding-right: 50; padding-bottom: 10"><span class="style9"><font size="-1" face="Arial, Helvetica, sans-serif">Flinston</font></span></td>
                            </tr>
                            <tr>
                              <td align="right" valign="top" style="padding-bottom: 10"><span style="padding-bottom: 10"><font size="-1" face="Arial, Helvetica, sans-serif">Verifique el C&oacute;digo de Seguridad :</font></span></td>
                              <td valign="top" style="padding-right: 50; padding-bottom: 10"><input name="codigo" type="text" id="codigo" size="15" maxlength="20">
                              <font face="Arial, Helvetica, sans-serif" size="-1"><font face="Arial, Helvetica, sans-serif" size="-1"><font size="-1" face="Arial, Helvetica, sans-serif"><font size="-1" face="Arial, Helvetica, sans-serif" class="style2">(*)</font></font></font></font></td>
                              <td align="right" valign="top" ><font face="Arial, Helvetica, sans-serif" size="-1">C&oacute;digo de Seguridad :</font></td>
                              <td valign="top" style="padding-right: 50; padding-bottom: 10"><img src="valida_form/turing.php" alt="Imagen de seguridad" width="128" height="25" border="0"></td>
                            </tr>
                            <tr>
                              <td colspan="4" align="center" valign="top" style="padding-bottom: 10"><font face="Arial, Helvetica, sans-serif" size="-1">
                                <input name="aceptoterminos" type="checkbox" id="aceptoterminos" value="S" onClick="document.form1.registrame.disabled=!document.form1.registrame.disabled" />
Estoy de acuerdo con los T&eacute;rminos que le&iacute; <b> <font color="#FF8000"><u>aqu&iacute;</u></font></b>.</font></td>
                            </tr>
                            <tr>
                              <td valign="top" style="padding-right: 50; padding-bottom: 10">&nbsp;</td>
                              <td colspan="3" align="center" valign="top" style="padding-right: 50; padding-bottom: 10"><input name="registrame" type="submit" id="registrame" value="Reg&iacute;strame" disabled="disabled" /></td>
                            </tr>
                            <tr>
                              <td valign="top" style="padding-right: 50; padding-bottom: 10"><input type="hidden" name="MM_insert" value="form1"></td>
                              <td colspan="3" valign="top" style="padding-right: 50; padding-bottom: 10">&nbsp;</td>
                            </tr>
                          </table>
                        </form>                      </td>
                    </tr>
                  </table></td>
              </tr>
              
              <tr> 
                <td>&nbsp;</td>
              </tr>
              <tr> 
                <td><table width="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
                    <tr> 
                      <td width="9" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
                      <td width="353" height="14" colspan="2" bgcolor="#999999">&nbsp;</td>
                    </tr>
                    <tr> 
                      <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                          <tr> 
                            <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
                                <tr> 
                                  <td width="96%">&nbsp;</td>
                                </tr>
                              </table></td>
                          </tr>
                        </table></td>
                    </tr>
                  </table></td>
              </tr>
              <tr> 
                <td>&nbsp;</td>
              </tr>
              <tr> 
                <td>&nbsp;</td>
              </tr>
            </table> 
          </td>
        </tr>
        <tr>
          <td align="right" valign="bottom">
            <span class="normalGrisHover"><br>
            </span>
            <table width="300" border="0" align="right" cellpadding="0" cellspacing="0" bgcolor="#999999">
              <tr>
                <td width="11" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
                <td width="222" height="14" bgcolor="999999"><div align="center"><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Mayor 
                  informaci&oacute;n</font></div></td>
              </tr>
              <tr>
                <td height="35" colspan="2"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#999999">
                    <tr>
                      <td ><table width="100%" border="0" align="center" cellpadding="5" cellspacing="0" bgcolor="#FFFFFF">
                          <tr>
                            <td colspan="2" align="center"><p align="left"><font size="-2" face="Verdana, Arial, Helvetica, sans-serif">www.angios.com<br>
                                  <a href="mailto:dependencia@univalle.edu.co">info@angios.com</a><br>
Tel&eacute;fono +58 212 993-5064. 
                                                  993-6651 <br>
Edificio. 147, Av. Principal de Chuao. Angios. Frente a el m&oacute;dulo de la polic&iacute;a Baruta. Chuao. <br>
Caracas, Venezuela <br>
                            </font><font size="-2" face="Verdana, Arial, Helvetica, sans-serif">Todos los Derechos Reservados &copy; Angiologia C.A. 2008</font></p></td>
                          </tr>
                      </table></td>
                    </tr>
                </table></td>
              </tr>
            </table>
          </td>
        </tr>
      </table></td>
  </tr>
</table>

</body>
</html>
