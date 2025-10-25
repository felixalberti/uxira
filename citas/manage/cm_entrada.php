<?php session_start();
require_once('connections/conexion2.php');
mysql_select_db($database_conexion2, $conexion2);
$query_usuario = "SELECT * FROM 0_users";
$usuario = mysql_query($query_usuario, $conexion2) or die(mysql_error());
$row_usuario = mysql_fetch_assoc($usuario);
$totalRows_usuario = mysql_num_rows($usuario);?>
<?php
$loginFormAction = $_SERVER['PHP_SELF'];
//if (isset($_GET['accesscheck'])) {
//  $_SESSION['PrevUrl'] = $_GET['accesscheck'];
//}
if (isset($_GET['medico'])) {
  $_SESSION['medico'] = $_GET['medico'];
}
if (isset($_GET['tipocita'])) {
  $_SESSION['tipocita'] = $_GET['tipocita'];
}
if (isset($_GET['fecha'])) {
  $_SESSION['fecha'] = $_GET['fecha'];
}
if (isset($_GET['numero'])) {
  $_SESSION['numero'] = $_GET['numero'];
}
if (isset($_GET['hora'])) {$_SESSION['hora_cita'] = $_GET['hora'];}
//if (isset($_SESSION['MM_UserGroup']))  {unset($_SESSION['MM_UserGroup']);}
if (isset($_POST['login'])) {
  $loginUsername=$_POST['login'];
  //$password=$_POST['password'];
  $clave = "siparano34247";
  //$password=crypt($_POST['password'],$clave);
  $password=md5($_POST['password']);
  $MM_fldUserAuthorization = "login";
  $MM_redirectLoginSuccess = "cm_selec_medico.php";
  $MM_redirectLoginFailed = "cm_entrada.php";
  $MM_redirecttoReferrer = false;
  mysql_select_db($database_conexion2, $conexion2);
  $LoginRS__query=sprintf("SELECT user_id, grupousuario, password, ci_usuario FROM 0_users WHERE user_id='%s' AND password='%s'",
  get_magic_quotes_gpc() ? $loginUsername : addslashes($loginUsername), get_magic_quotes_gpc() ? $password : addslashes($password)); 
  $LoginRS = mysql_query($LoginRS__query, $conexion2) or die(mysql_error());
  $loginFoundUser = mysql_num_rows($LoginRS);
  if ($loginFoundUser ) {
    //$loginStrGroup  = mysql_result($LoginRS,0,'user_id');
    $loginStrGroup  = mysql_result($LoginRS,0,'grupousuario');	
	$ci_usuario = mysql_result($LoginRS,0,'ci_usuario');
	
	//if ($loginStrGroup == 'A' or $loginStrGroup == 'S') {
 	   if (!isset($_SESSION)) {session_start();}
		//declare two session variables and assign them
		$_SESSION['MM_Username'] = $loginUsername;
		$_SESSION['MM_UserGroup'] = $loginStrGroup;	      
		$_SESSION['ci_usuario'] = $ci_usuario;	
		$_SESSION['userautentico'] = "SI";
		$_SESSION['ultiacceso'] = date("Y-n-j H:i:s");
		$_SESSION['fecha_cita'] = date("Y/m/d");
		
		if ($_SESSION['MM_UserGroup'] == "" & isset($_SESSION['cedulaborrar']) && ($_SESSION['cedulaborrar'] != $_SESSION['ci_usuario'])){
		   header("Location: ". "mensaje_menu.php?mensaje=La cita no se puede cambiar, usuario diferente a la cedula"."&mensaje3=Regresar a Citas"); 
		   exit();
		}		
        //if (isset($_SESSION['medico']) && isset($_SESSION['tipocita']) && isset($_SESSION['numero']) && isset($_SESSION['fecha']) ) {
		if (isset($_SESSION['fecha_cita'])){
		    header("Location: " . "cm_confirm_cita.php" );  
		    exit();
		}
		
		if (isset($_SESSION['anularcita']) && ($_SESSION['anularcita'] == "S")) {
		    header("Location: " . "anularcitas.php?".trim($_SESSION['string_param']) );
		    exit();
		}
				
		//if (($_SESSION['MM_UserGroup']) == "A" ) {
		//   header("Location: " . "backoffice/citasparahoyfinal.php" );
		//}	  
	  //  else {header("Location: " . $MM_redirectLoginSuccess );}
	  header("Location: " . $MM_redirectLoginSuccess );
	//}
	//else {
	//	header("Location: ". $MM_redirectLoginFailed );
	//}    
    
	}
	else {
		/*echo "<script languaje='javascript' type='text/javascript'>alert('El usuario no tiene acceso');</script>";*/		
		header("Location: ". $MM_redirectLoginFailed."?mensaje=Login o Clave Incorrecta, Solicite su Clave por Correo" );
	}
}
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>Centro Vascular y Cuidado Integral de Heridas</title>
<SCRIPT type="text/javascript" src="javascript/valida_entra.js"></script>
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
.style5 {font-size: 10px}
.style6 {font-size: 10}
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
            <td><p><font size="4" ><span class="titulotop1">Citas</span></font></p>
                    <table width="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
                      <tr>
                        <td width="9" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
                        <td width="353" height="14" colspan="2" bgcolor="#999999"><strong><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Entrada al Portal</font></strong></td>
                      </tr>
                      <tr>
                        <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                            <tr>
                              <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
                                  <tr>
                                    <td width="96%" valign="top">
<div align="center">
                                      <!-- fin buscador Google --><font face="Arial, Helvetica, sans-serif" size="-1">Si a&uacute;n no est&aacute; registrado haga click <a href="registrar.php">aqui </a></font></div>									
<form id="form1" name="form1" method="POST" onSubmit="return ComprobarDatos();" action="<?php echo $loginFormAction; ?>" >									
									<table width="363" border="1" align="center" cellpadding="0" cellspacing="0">
									 <?php if (isset($_GET['mensaje'])) {
                                        echo '<tr bordercolor="#CCCCCC">';
                                          echo '<td height="28" colspan="2" align="center" valign="middle" bgcolor="#FFFFFF"><span class="style7">';
                                            echo $_GET['mensaje'];} 
                                          echo '</span></td>';
                                        echo '</tr>'; ?>										
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" colspan="2" align="center" valign="middle" bgcolor="#CC6666"><font color="#FFFFFF">Inicio de Sessi&oacute;n </font></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" colspan="2" align="right" valign="middle" bgcolor="#CCCCCC">&nbsp;</td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" valign="middle" align="right" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">Usuario:
                                              
                                          </font> </td>
                                          <td valign="middle" align="left" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">
                                            <input name="login" type="text" id="login" />
                                          </font></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" align="right" valign="middle" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">Clave:
                                            
                                          </font> </td>
                                          <td height="28" align="left" valign="middle" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">
                                            <input name="password" type="password" id="password" />
                                          </font></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="24" colspan="2" align="right" valign="top" bgcolor="#CCCCCC"><p>
                                              <input name="MM_cerrar" type="hidden" id="MM_cerrar" value="cerrar" />
                                              <input name="fecha" type="hidden" id="fecha" value="<?PHP if (isset($_SESSION['fecha'])) {echo $_SESSION['fecha'];} ?>" />
                                              <input name="numero" type="hidden" id="numero" value="<?PHP if (isset($_SESSION['numero'])) {echo $_SESSION['numero'];} ?>" />
                                              <input name="logon" type="submit" id="logon" value="Entrar" />
                                          </p></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="16" colspan="2" align="right" valign="top" bgcolor="#CCCCCC"><p align="center"><font color="#003366"><a href="cm_claveolvidada.php">Olvid&eacute; mi clave</a></font></p>                                            </td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="16" colspan="2" align="center" valign="top" bgcolor="#CCCCCC"><font color="#003366"><a href="cm_cambio_clave.php">Cambio Clave</a></font></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="16" colspan="2" align="center" valign="top" bgcolor="#CCCCCC"><font color="#003366"><a href="cm_resetear_clave.php">Enviar Clave a Email</a></font></td>
                                        </tr>
                                      </table>
									  </form>
                                        <p align="justify">&nbsp;</p>
                                        <p>
                                          <!--Buscador Free find-->
                                          <!--fin buscador Freefind-->
                                    </td>
                                  </tr>
                              </table></td>
                            </tr>
                        </table></td>
                      </tr>
                  </table></td>
          </tr>
        </table></td>
      </tr>
    </table></td>
  </tr>
</table>
</body>
</html>