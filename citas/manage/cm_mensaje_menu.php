<?PHP 
session_start();

if (isset($_GET['fecha'])) {$fecha = $_GET['fecha'];} else {$fecha = "";}
if (isset($_GET['numero'])) {$numero = $_GET['numero'];} else {$numero = "";}
if (isset($_GET['hora'])) {$hora = $_GET['hora'];} else {$hora = "";}
if (isset($_GET['cedula'])) {$ci = $_GET['cedula'];} else {$ci = "";}
if (isset($_GET['medico'])) {$medico = $_GET['medico'];} else {$medico = "";}
if (isset($_GET['tipocita'])) {$tipocita = $_GET['tipocita'];} else {$tipocita = "";}
if (isset($_GET['motivo'])) {$motivo = $_GET['motivo'];} else {$motivo = "";}
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
.style4 {font-size: 12px}
.style9 {font-size: 12px; font-family: Arial, Helvetica, sans-serif; color: #FFFFFF; }
.style28 {color: #FF0000; font-weight: bold; font-size: 12px; }
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
                      <td width="353" height="14" colspan="2" bgcolor="#999999">&nbsp;</td>
                    </tr>
                    <tr> 
                      <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                          <tr> 
                            <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
                                <tr> 
                                  <td width="4%">&nbsp;</td>
                                  <td width="96%" valign="top"> <p align="justify">
                                    <!-- buscador  Google --></p>
                                    <table width="412" border="1" align="center" cellpadding="1" cellspacing="0">
                                      <tr>
                                        <td align="center" bgcolor="#CC6666" scope="col"><span class="style9">Mensaje</span></td>
                                        <td align="center" bgcolor="#CC6666" scope="col"><span class="style9">C&oacute;digo</span></td>
                                      </tr>
                                      <tr>
                                        <td align="center" valign="middle" bgcolor="#CCCCCC"><p>&nbsp;</p>
                                        <p><?PHP echo (isset($_GET['mensaje'])) ? $_GET['mensaje']: "" ;?></p></td>
                                        <td align="center" valign="middle" bgcolor="#CCCCCC"><p>&nbsp;</p>
                                        <?PHP echo (isset($_GET['coderror'])) ? $_GET['coderror']: "" ;?></td>
                                      </tr>
                                      <tr>
                                        <td align="center" valign="middle" bgcolor="#CCCCCC" class="style28"><p>&nbsp;</p>
                                        <a class="a_grand" href="<?PHP echo "cm_imprimircita.php?fecha=".$fecha."&numero=".$numero."&hora=".$hora."&ci=".$ci."&medico=".$medico."&tipocita=".$tipocita."&motivo".$motivo ?>" target="popup" onClick="window.open(this.href,this.target,'width=900;height=400');return false;"><?PHP echo (isset($_GET['mensaje2'])) ? $_GET['mensaje2']: "" ;?></a></td>
                                        <td align="center" valign="middle" bgcolor="#CCCCCC">&nbsp;</td>
                                      </tr>
                                      <tr>
                                        <td align="center" valign="middle" bgcolor="#CCCCCC" class="style28"><a href="cm_selec_medico_final.php" ><?PHP echo (isset($_GET['mensaje3'])) ? $_GET['mensaje3']: "" ;?></a></td>
                                        <td align="center" valign="middle" bgcolor="#CCCCCC"><p>&nbsp;</p>
                                        <p>&nbsp;</p></td>
                                      </tr>
                                    </table>
                                    <!-- fin buscador Google -->
                                    <br> 
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
            </table>          </td>
        </tr>
        <tr>
          <td align="right" valign="bottom">
            <span class="normalGrisHover"><a href="/comentarios/" onMouseOver="MM_swapImage('Image21111','','/imagenes/segundonivel/comentarios-over.gif',1)" onMouseOut="MM_swapImgRestore()"></a><br>
            </span><span class="normalGrisHover"><span class="style4"><font face="Verdana, Arial, Helvetica, sans-serif"></font></span></span> </td>
        </tr>
      </table></td>
  </tr>
</table>

</body>
</html>