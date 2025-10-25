<?php 
require_once('connections/conexion2.php'); ?>
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

mysql_select_db($database_conexion2, $conexion2);
$query_usuario = "SELECT * FROM 0_users";
$usuario = mysql_query($query_usuario, $conexion2) or die(mysql_error());
$row_usuario = mysql_fetch_assoc($usuario);
$totalRows_usuario = mysql_num_rows($usuario);

?>
<?php
// *** Validate request to login to this site.
//if (!isset($_SESSION)) {
  session_start();
//}


$loginFormAction = $_SERVER['PHP_SELF'];
if (isset($_GET['accesscheck'])) {
  $_SESSION['PrevUrl'] = $_GET['accesscheck'];
}
if (isset($_GET['fecha'])) {
  $_SESSION['fecha'] = $_GET['fecha'];
}
if (isset($_GET['numero'])) {
  $_SESSION['numero'] = $_GET['numero'];
}
$pregunta1 = "";
$resp1 = "";
$paso = "Paso 1: Indique su c&eacute;dula";
if (isset($_POST['ci_usuario'] )) {
  $loginCiusuario=$_POST['ci_usuario'];
   mysql_select_db($database_conexion2, $conexion2);
  $LoginRS__query=sprintf("SELECT pregunta1 FROM 0_users WHERE ci_usuario='%s'",
  get_magic_quotes_gpc() ? $loginCiusuario : addslashes($loginCiusuario)); 
   
  $LoginRS = mysql_query($LoginRS__query, $conexion2) or die(mysql_error());
  $loginFoundUser = mysql_num_rows($LoginRS);
  if ($loginFoundUser) {
  	$pregunta1 = mysql_result($LoginRS,0,'pregunta1');
	$_SESSION['aux_pregunta'] = $pregunta1;
    $paso = "Paso 2: Responda la pregunta";
  }
  else {$pregunta1="su cédula no existe";}
}
$continuar = 0;
if (!isset($_POST['passwordnuevo']) && isset($_POST['pregunta1']) && !($_POST['pregunta1'] == "") && (isset($_POST['resp1']) && !($_POST['resp1'] == ""))) {
  $MM_fldUserAuthorization = "login";
  $MM_redirectLoginSuccess = "cm_entrada.php";
  $MM_redirectLoginFailed = "cm_claveolvidada.php";
  $MM_redirecttoReferrer = false;
  mysql_select_db($database_conexion2, $conexion2);
  	
  $LoginRS__query=sprintf("SELECT user_id, password, ci_usuario FROM 0_users WHERE pregunta1='%s' AND resp1='%s'",
  $_POST['pregunta1'], $_POST['resp1']); 
   
  $LoginRS = mysql_query($LoginRS__query, $conexion2) or die(mysql_error());
  $loginFoundUser = mysql_num_rows($LoginRS);
  if ($loginFoundUser) {
    
    $resp1="respuesta correcta";
	$continuar = 1;
    $paso = "Paso 3: Ingrese una clave nueva";
	$_SESSION['pregunta'] = $_POST['pregunta1'];
	$_SESSION['respuesta'] = $_POST['resp1'];
  }
  else {$resp1="respuesta incorrecta";
  $continuar = 0;
  $_SESSION['pregunta'] = "";
  $_SESSION['respuesta'] = "";
  $paso = "Paso 2: Responda la pregunta";
  }  
} 

if (isset($_POST['passwordnuevo']) && (isset($_SESSION['respuesta']) && !($_SESSION['respuesta'] == ""))) {
  $clave = "siparano34247";
  //$passwordnew=crypt($_POST['passwordnuevo'],$clave); 
  $passwordnew=md5($_POST['passwordnuevo']); 
  $MM_fldUserAuthorization = "login";
  $MM_redirectLoginSuccess = "cm_entrada.php";
  $MM_redirectLoginFailed = "cm_claveolvidada.php";
  $MM_redirecttoReferrer = false;
  mysql_select_db($database_conexion2, $conexion2);
  	
  $LoginRS__query=sprintf("SELECT user_id, password, ci_usuario FROM 0_users WHERE pregunta1='%s' AND resp1='%s'",
  $_SESSION['pregunta'], $_SESSION['respuesta']); 
   
  $LoginRS = mysql_query($LoginRS__query, $conexion2) or die(mysql_error());
  $loginFoundUser = mysql_num_rows($LoginRS);
  if ($loginFoundUser) {
    
    $loginStrGroup  = mysql_result($LoginRS,0,'user_id');
	$ci_usuario = mysql_result($LoginRS,0,'ci_usuario');
    
    //declare two session variables and assign them
    $_SESSION['MM_Username'] = $loginStrGroup;
    $_SESSION['MM_UserGroup'] = $loginStrGroup;	      
	//$_SESSION['ci_usuario'] = $ci_usuario;	
	
    /*$updateSQL = "UPDATE 0_users SET password='".$passwordnew."' WHERE pregunta1='".$_SESSION['pregunta']."' and resp1='".$_SESSION['respuesta']."'";*/
	 $updateSQL = "UPDATE 0_users SET password='".$passwordnew."' WHERE user_id='".$loginStrGroup."'";
                   					   
    mysql_select_db($database_conexion2, $conexion2);
    $Result1 = mysql_query($updateSQL, $conexion2) or die(mysql_error());					   
    if ($Result1==false) {$MM_redirectLoginSuccess='index.php';}
    if (isset($_SESSION['PrevUrl']) && false) {
      $MM_redirectLoginSuccess = $_SESSION['PrevUrl'];	
    }
    
    header("Location: " . $MM_redirectLoginSuccess );
  }
 else {
    session_destroy();	   
   
	header("Location: ". $MM_redirectLoginFailed."?mensaje=No se pudo actualizar su clave su respuesta no concuerda" );
  }

}
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>Centro Vascular y Cuidado Integral de Heridas</title>
<?PHP if (isset($_POST['resp1'])) {
echo '<SCRIPT type="text/javascript" src="javascript/valida_olvidocla2.js"></script>'
;} ?>

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

<link href="css/hojaestilo.css" rel="stylesheet" type="text/css">
<style type="text/css">
<!--
.style1 {	color: #FF0000;
	font-weight: bold;
}
.style4 {font-size: 12px}
.style5 {font-size: 10px}
.style6 {font-size: 10}
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
                        <td width="353" height="14" colspan="2" bgcolor="#999999"><strong><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Clave Olvidada </font></strong></td>
                      </tr>
                      <tr>
                        <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                            <tr>
                              <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
                                  <tr>
                                    <td width="4%">&nbsp;</td>
                                    <td width="96%" valign="top">
<div align="center">
                                          <!-- fin buscador Google -->
                                        Si&nbsp;no&nbsp;tiene&nbsp;cuenta&nbsp;para&nbsp;iniciar&nbsp;sesi&oacute;n&nbsp;puede&nbsp;crearla&nbsp;haciendo&nbsp;clic <a href="registrar.php">AQUI</a> <br>
                                        Contacte al administrador si persiste el problema
                                        <br>
                                      </div>									
<form id="form1" name="form1" method="POST" onSubmit="return ComprobarDatos();" action="<?php echo $loginFormAction;  ?> " >
  <table width="405" border="1" align="center" cellpadding="0" cellspacing="0">
    <tr bordercolor="#CCCCCC">
      <td height="28" colspan="2" align="right" valign="middle" bgcolor="#FFFFFF"><div align="center"><strong><font color="#FF0000">&lt;&lt; Clave Nueva &gt;&gt; </font></strong></div></td>
    </tr>
    <tr bordercolor="#CCCCCC">
      <td height="28" colspan="2" align="center" valign="middle" bgcolor="#CC6666"><font color="#FFFFFF">Clave Olvidada </font></td>
    </tr>
    <tr bordercolor="#CCCCCC">
      <td height="28" colspan="2" align="left" valign="middle" bgcolor="#CCCCCC" class="style1"><?PHP echo $paso; ?> </td>
    </tr>
	<?PHP if(!isset($_POST['ci_usuario']) && !isset($_POST['pregunta1']) ) {
    echo '<tr bordercolor="#CCCCCC">
      <td width="104" height="28" align="right" valign="middle" bgcolor="#CCCCCC"><font size="-1" face="Arial, Helvetica, sans-serif">C&eacute;dula:</font></td>
      <td width="295" height="28" align="left" valign="middle" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">
        <input name="ci_usuario" type="text" id="ci_usuario" value="" size="12" maxlength="10" tabindex="1"/>
		<input name="continuar" type="Submit" id="continuar" value="Continuar" />
      </font></td>
    </tr>';} ?>
    <tr bordercolor="#CCCCCC">
      <td height="28" align="right" valign="middle" bgcolor="#CCCCCC">&nbsp;</td>
      <td height="28" align="left" valign="middle" bgcolor="#CCCCCC">&nbsp;</td>
    </tr> 
	<?php if ((isset($_POST['ci_usuario']) || isset($_POST['pregunta1'])) && $continuar == 0 ) { 
	echo	
    '<tr bordercolor="#CCCCCC">
      <td height="24" align="right" valign="top" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">Pregunta Reto:</font></td>
      <td height="24" align="left" valign="top" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">'.
        '<input name="pregunta1" type="hidden" id="pregunta1" value="'.$_SESSION['aux_pregunta'].'" size="30" maxlength="40" />'.$_SESSION['aux_pregunta'].
      '</font></td>'.
    '</tr>'.
    '<tr bordercolor="#CCCCCC">'.
      '<td height="24" align="right" valign="top" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">Respuesta:</font></td>'.
      '<td height="24" align="left" valign="top" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">'.
        '<input name="resp1" type="text" id="resp1" value="'.$resp1.'" size="30" maxlength="40" tabindex="1"/>'.
        '<input name="continuar2" type="submit" id="continuar2" value="Continuar" />'.
      '</font></td>'.
    '</tr>';} ?>
<?PHP if ($continuar == 1 && isset($_POST['resp1']) && ($_POST['resp1'] != "") ) {echo '<tr bordercolor="#CCCCCC">'.
      '<td height="24" align="right" valign="top" bgcolor="#CCCCCC"><font size="-1" face="Arial, Helvetica, sans-serif">Nueva Clave:</font></td>'.
      '<td height="24" align="left" valign="top" bgcolor="#CCCCCC" ><font face="Arial, Helvetica, sans-serif" size="-1">
        <input name="passwordnuevo" type="password" id="passwordnuevo" maxlength="10" tabindex="1" />'.
      '</font></td>'.
    '</tr>'.
    '<tr bordercolor="#CCCCCC">'.
      '<td height="24" align="right" valign="top" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">Reescribir Clave:</font></td>'.
      '<td height="24" align="left" valign="top" bgcolor="#CCCCCC"><font face="Arial, Helvetica, sans-serif" size="-1">
        <input name="repasswordnue" type="password" id="repasswordnue" maxlength="10" />'.
      '</font>'.
	 ' <input name="continuar3" type="submit" id="continuar3" value="Continuar" /></td>'.	  
    '</tr>';} ?>
    <tr bordercolor="#CCCCCC">
      <td height="24" colspan="2" align="center" valign="top" bgcolor="#CCCCCC"><p>
          <input name="MM_cerrar" type="hidden" id="MM_cerrar" value="cerrar" />
          <input name="fecha" type="hidden" id="fecha" value="<?PHP echo $_SESSION['fecha'];  ?>" />
          <input name="numero" type="hidden" id="numero" value="<?PHP echo $_SESSION['numero'];  ?>" />
      </p></td>
    </tr>
    <tr bordercolor="#CCCCCC">
      <td height="33" colspan="2" align="right" valign="top" bgcolor="#CCCCCC"><p align="center">&nbsp;</p>
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