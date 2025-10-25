<?php session_start();
require_once('connections/conexion2.php');
mysql_select_db($database_conexion2, $conexion2);
$query_usuario = "SELECT * FROM 0_users";
$usuario = mysql_query($query_usuario, $conexion2) or die(mysql_error());
$row_usuario = mysql_fetch_assoc($usuario);
$totalRows_usuario = mysql_num_rows($usuario);?>
<?php
$loginFormAction = $_SERVER['PHP_SELF'];
if (isset($_GET['fecha'])) {
  $_SESSION['fecha'] = $_GET['fecha'];
}
if (isset($_GET['numero'])) {
  $_SESSION['numero'] = $_GET['numero'];
}
if (isset($_GET['hora'])) {$_SESSION['hora_cita'] = $_GET['hora'];}
if (isset($_POST['login'])) {
  $loginUsername=$_POST['login'];
  $clave = "siparano34247";
  //$password=crypt($_POST['password'],$clave);
  //$passwordnew=crypt($_POST['passnew'],$clave);
  $password=md5($_POST['password']);
  $passwordnew=md5($_POST['passnew']);
  $MM_fldUserAuthorization = "login";
  $MM_redirectLoginSuccess = "cm_entrada.php";
  $MM_redirectLoginFailed = "cambio_clave.php";
  $MM_redirecttoReferrer = false;
  mysql_select_db($database_conexion2, $conexion2);
  $LoginRS__query=sprintf("SELECT user_id, grupousuario, password, ci_usuario FROM 0_users WHERE user_id='%s' AND password='%s'",
  get_magic_quotes_gpc() ? $loginUsername : addslashes($loginUsername), get_magic_quotes_gpc() ? $password : addslashes($password)); 
  $LoginRS = mysql_query($LoginRS__query, $conexion2) or die(mysql_error());
  $loginFoundUser = mysql_num_rows($LoginRS);
  if ($loginFoundUser ) {
    $loginStrGroup  = mysql_result($LoginRS,0,'grupousuario');	
	$ci_usuario = mysql_result($LoginRS,0,'ci_usuario');
    $updateSQL = "UPDATE 0_users SET password='".$passwordnew."' WHERE user_id='".$loginUsername."' and password='".$password."'";			   
    mysql_select_db($database_conexion2, $conexion2);
    $Result1 = mysql_query($updateSQL, $conexion2) or die(mysql_error());					   
    if ($Result1==true) {
	   header("Location: ". $MM_redirectLoginSuccess ); 
	}
	else  {	header("Location: ". $MM_redirectLoginFailed );}  
   }
   else {
		header("Location: ". $MM_redirectLoginFailed );
   }
}
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>Centro Vascular y Cuidado Integral de Heridas</title>
<SCRIPT type="text/javascript" src="javascript/cambio_clave.js"></script>
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
<link href="ccs/hojaestilo.ccs" rel="stylesheet" type="text/css">
<style type="text/css">
<!--
.style1 {	color: #FF0000;
	font-weight: bold;
}
.style4 {font-size: 12px}
.style5 {font-size: 10px}
.style6 {font-size: 10}
.style7 {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 12px;
}
-->
</style>
</head>
<body>
<table width="760" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td><table width="760" border="0" align="right"  cellpadding="0" cellspacing="0">
      <tr>
        <td width="78%" height="365" valign="top"><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
          <tr>
            <td><p>&nbsp;</p>
                    <table width="100%" border="0" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
                      <tr>
                        <td width="9" height="14" align="left" valign="top" bgcolor="999999"><img src="imagenes/segundonivel/esquinagris.gif" alt="" width="8" height="15"></td>
                        <td width="353" height="14" colspan="2" bgcolor="#999999"><strong><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Cambio Clave </font></strong></td>
                      </tr>
                      <tr>
                        <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                            <tr>
                              <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
                                  <tr>
                                    <td width="4%">&nbsp;</td>
                                    <td width="96%" valign="top">
<div align="center">
                                      <!-- fin buscador Google --><font face="Arial, Helvetica, sans-serif" size="-1">Si a&uacute;n no est&aacute; registrado haga click <a href="registrar.php">aqui </a></font></div>									
<form id="form1" name="form1" method="POST" onSubmit="return ComprobarDatos();" action="<?php echo $loginFormAction; ?>" >									
									<table width="363" border="1" align="center" cellpadding="0" cellspacing="0">
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" colspan="2" align="center" valign="middle" bgcolor="#FFFFFF"><span class="style7">
                                            <?php if (isset($_GET['mensaje'])) {echo $_GET['mensaje'];} ?>
                                          </span></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" colspan="2" align="right" valign="middle" bgcolor="#FFFFFF"><div align="center"><strong><font color="#FF0000">&lt;&lt; Autenticaci&oacute;n Requerida &gt;&gt; </font></strong></div></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" colspan="2" align="center" valign="middle" bgcolor="#CC6666"><font color="#FFFFFF">Inicio de Sessi&oacute;n </font></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" align="right" valign="middle" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">Login:</font></td>
                                          <td height="28" align="left" valign="middle" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">
                                            <input name="login" type="text" id="login" />
                                          </font></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" valign="middle" align="right" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">Clave Actual:
                                              
                                          </font> </td>
                                          <td valign="middle" align="left" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">
                                            <input name="password" type="password" id="password" />
                                          </font></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="28" align="right" valign="middle" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">Clave Nueva:
                                            
                                          </font> </td>
                                          <td height="28" align="left" valign="middle" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">
                                            <input name="passnew" type="password" id="passnew" />
                                          </font></td>
                                        </tr>
                                        <tr bordercolor="#CCCCCC">
                                          <td height="24" align="right" valign="top" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">Repita Clave:</font></td>
                                          <td height="24" align="left" valign="top" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">
                                            <input name="repassnew" type="password" id="repassnew" />
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
                                          <td height="33" colspan="2" align="right" valign="top" bgcolor="#CCCCCC"><p align="center"><font color="#003366"><a href="cm_claveolvidada.php">Olvid&eacute; mi clave</a></font> </p>
                                            <p>&nbsp;</p></td>
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
          <tr>
            <td>&nbsp;</td>
          </tr>
        </table></td>
      </tr>
      <tr>
        <td align="right" valign="bottom"><span class="normalGrisHover"><a href="/comentarios/" onMouseOver="MM_swapImage('Image21111','','/imagenes/segundonivel/comentarios-over.gif',1)" onMouseOut="MM_swapImgRestore()"></a><br>
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
          <span class="normalGrisHover"><span class="style4"><span class="style5"><span class="style6"><font face="Verdana, Arial, Helvetica, sans-serif"></font></span></span></span></span> </td>
      </tr>
    </table></td>
  </tr>
</table>
</body>
</html>
