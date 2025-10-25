<?PHP session_start(); ?>
<?php require_once('connections/conexion2.php'); ?>
<?php
$currentPage = $_SERVER["PHP_SELF"];

$colname_rs_medico = "-1";
if (isset($_SESSION['medico'])) {
  $colname_rs_medico = (get_magic_quotes_gpc()) ? $_SESSION['medico'] : addslashes($_SESSION['medico']);
}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_medico = sprintf("SELECT medico_no, name FROM 0_medico_master WHERE medico_no = '%s'", $colname_rs_medico);
$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
$row_rs_medico = mysql_fetch_assoc($rs_medico);
$totalRows_rs_medico = mysql_num_rows($rs_medico);
if (isset($_POST['tipocita'])) {
    $_SESSION['tipocita'] = $_POST['tipocita'];
}
$colname_rs_tipo_cita = "-1";
if (isset($_SESSION['tipocita'])) {
  $colname_rs_tipo_cita = (get_magic_quotes_gpc()) ? $_SESSION['tipocita'] : addslashes($_SESSION['tipocita']);
}
mysql_select_db($database_conexion2, $conexion2);
$query_rs_tipo_cita = sprintf("SELECT codigotipo, descripcion FROM 0_cm_system_tables WHERE codigotipo = '%s' and codigotabla='TPCITA'", $colname_rs_tipo_cita);
$rs_tipo_cita = mysql_query($query_rs_tipo_cita, $conexion2) or die(mysql_error());
$row_rs_tipo_cita = mysql_fetch_assoc($rs_tipo_cita);
$totalRows_rs_tipo_cita = mysql_num_rows($rs_tipo_cita);

function diasemana($dia){
	$semanaArray = array(
		"1" => "Domingo", 
		"2" => "Lunes", 
		"3" => "Martes", 
		"4" => "Miércoles", 
		"5" => "Jueves", 
		"6" => "Viernes", 
		"7" => "Sábado"	);
	
	$semanaReturn = $semanaArray[$dia];
	
return $semanaReturn;
}
function dia_semana ($dia, $mes, $ano) {
    $dias = array('Domingo', 'Lunes', 'Martes', 'Miércoles', 'Jueves', 'Viernes', 'Sábado');
    return $dias[date("w", mktime(0, 0, 0, $mes, $dia, $ano))];
}
function color_fontnum($val, $val2, $fecha, $hora, $mostrar) {

if ($val == 1) {
echo '<a style="text-decoration:none" ><font color="#FF0000" >'.$mostrar."</font></a>";
}
else {
//if (isset($_SESSION['MM_UserGroup']) && ($_SESSION['MM_UserGroup'] == 'A')) {
if (isset($_SESSION['ci_usuario']) && isset($_SESSION['medico']) && isset($_SESSION['tipocita'])) {
   echo '<a style="text-decoration:none" href="cm_confirm_cita.php?numero='.$val2.'&hora='.$hora.'&fecha='.$fecha.         '"><font         color="#000000" >'.   $mostrar."</font></a>";}
else {   
   echo '<a style="text-decoration:none" href="cm_entrada.php?numero='.$val2.'&hora='.$hora.'&fecha='.$fecha.'"><font color="#000000" >'.   $mostrar."</font></a>";}
//}
//else 
// {
//   echo '<a style="text-decoration:none" href="valida_login.php?numero='.$val2.'&fecha='.$fecha.'"><font color="#000000" >//'.   $val2."</font></a>";
//}
}
}
function calcularFecha($dias){
 
$calculo = strtotime("$dias days");
return date("Y-m-d", $calculo);
} 

$info = parse_ini_file("config.ini",true);
$diasarranque = $info['parametros']['dias_arranque'];
$diasfinal = $info['parametros']['dias_a_mostrar'];
$ano = substr($diasfinal,0,4); 
$mes = substr($diasfinal,5,2);
$mes = str_pad($mes, 2, "0", STR_PAD_LEFT);  
$dia = substr($diasfinal,8,2); 
$dia = str_pad($dia, 2, "0", STR_PAD_LEFT); 
if ((dia_semana($dia, $mes, $ano) == 'Viernes')) {
    $diasfinal=$diasfinal+2;
}
$fecha_inicial = calcularFecha($diasarranque); //Obtener la cantidad de días a mostrar de la BD o un archivo.ini
$fecha_limite = calcularFecha($diasfinal); //Obtener la cantidad de días a mostrar de la BD o un archivo.ini
$maxRows_citasxnumero = 10;
$pageNum_citasxnumero = 0;
if (isset($_GET['pageNum_citasxnumero'])) {
  $pageNum_citasxnumero = $_GET['pageNum_citasxnumero'];
}
$startRow_citasxnumero = $pageNum_citasxnumero * $maxRows_citasxnumero;
if (isset($_POST['tipocita'])) {
$_SESSION['tipocita'] = $_POST['tipocita'];}

do {
mysql_select_db($database_conexion2, $conexion2);
$query_citasxnumero = "SELECT sc.fecha, fxs.numero, fxs.tomado, fxs.hora, DATE_FORMAT(sc.fecha,'%d/%m/%Y') as fecddmmaa FROM 0_cm_semanacita sc, 0_cm_fechasxsemana fxs WHERE sc.fecha = fxs.fecha and sc.enuso = 1 and (fxs.tipocita = '".$_SESSION['tipocita']."' or fxs.tipocita is null or fxs.tipocita = '' ) and fxs.codmed = '".$_SESSION['medico']."'";
$query_citasxnumero = sprintf("%s and (sc.fecha >= %s and sc.fecha <= %s) and sc.codmed = %s", $query_citasxnumero, "'".$fecha_inicial."'","'".$fecha_limite."'","'".$_SESSION['medico']."'");
$query_citasxnumero = sprintf("%s %s", $query_citasxnumero, " order by sc.fecha, fxs.numero");
$query_limit_citasxnumero = sprintf("%s LIMIT %d, %d", $query_citasxnumero, $startRow_citasxnumero, $maxRows_citasxnumero);
//echo $query_limit_citasxnumero;
$citasxnumero = mysql_query($query_limit_citasxnumero, $conexion2) or die(mysql_error());
$row_citasxnumero = mysql_fetch_assoc($citasxnumero);
$totalRows_citasxnumero = mysql_num_rows($citasxnumero);
if ($totalRows_citasxnumero < 1) {
   $diasarranque++;
   $diasfinal++;
   $fecha_inicial = calcularFecha($diasarranque); //Obtener de nuevo la fecha inicial  
   $fecha_limite = calcularFecha($diasfinal); //Obtener de nuevo la fecha final
}
} while ($totalRows_citasxnumero < 1 and $diasarranque <= 4 );

if (isset($_GET['totalRows_citasxnumero'])) {
  $totalRows_citasxnumero = $_GET['totalRows_citasxnumero'];
} else {
  $all_citasxnumero = mysql_query($query_citasxnumero);
  $totalRows_citasxnumero = mysql_num_rows($all_citasxnumero);
}
$totalPages_citasxnumero = ceil($totalRows_citasxnumero/$maxRows_citasxnumero)-1;

$queryString_citasxnumero = "";
if (!empty($_SERVER['QUERY_STRING'])) {
  $params = explode("&", $_SERVER['QUERY_STRING']);
  $newParams = array();
  foreach ($params as $param) {
    if (stristr($param, "pageNum_citasxnumero") == false && 
        stristr($param, "totalRows_citasxnumero") == false) {
      array_push($newParams, $param);
    }
  }
  if (count($newParams) != 0) {
    $queryString_citasxnumero = "&" . htmlentities(implode("&", $newParams));
  }
}
$queryString_citasxnumero = sprintf("&totalRows_citasxnumero=%d%s", $totalRows_citasxnumero, $queryString_citasxnumero);

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
.style3 {font-family: Arial, Helvetica, sans-serif; font-weight: bold; font-size: 10px; }
.style7 {font-family: Arial, Helvetica, sans-serif; font-size: 10px; }
.style19 {font-size: 10px; color: #CCCCCC; font-weight: bold; }
.style24 {font-size: 10px; color: #000000; font-weight: bold; }
.style25 {
	color: #FFFFFF;
	font-weight: bold;
}
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
                      <td width="353" height="14" colspan="2" bgcolor="#999999"><strong><font color="#FFFFFF" size="-1" face="Verdana, Arial, Helvetica, sans-serif">Seleccione un turno </font></strong></td>
                    </tr>
                    <tr> 
                      <td colspan="3"><table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
                          <tr> 
                            <td bgcolor="#FFFFFF"><table width="100%" border="0" cellspacing="0" cellpadding="12">
							    <tr>
								 <td>								 </td>
								 <td><table width="362" border="0" align="center" cellpadding="0">
                                     <tr>
                                       <td><table width="332" border="0" align="center" cellpadding="4" cellspacing="1">
                                         <tr>
                                           <td width="88" bgcolor="#CC0000"><span class="style25"><font face="Verdana, Arial, Helvetica, sans-serif">M&eacute;dico:</font></span></td>
                                           <td width="169" bgcolor="#CC0000"><span class="style19"><?php echo " ".$row_rs_medico['medico_no'].", "; ?><?php echo $row_rs_medico['name']; ?></span></td>
                                           <td width="47" bgcolor="#CCCCCC"><strong><a href="cm_selec_medico.php">Cambiar</a></strong></td>
                                         </tr>
                                         <tr>
                                           <td bgcolor="#CCCCCC">&nbsp;</td>
                                           <td colspan="2" bgcolor="#CCCCCC">&nbsp;</td>
                                         </tr>
                                         <tr>
                                           <td bgcolor="#CC0000"><span class="style25"><font face="Verdana, Arial, Helvetica, sans-serif">Tipo cita: </font></span></td>
                                           <td bgcolor="#CC0000"><span class="style19"><?php echo $row_rs_tipo_cita['codigotipo']." ".$row_rs_tipo_cita['descripcion']; ?></span></td>
                                           <td bgcolor="#CCCCCC"><strong><a href="cm_selec_tipocita.php">Cambiar</a></strong></td>
                                         </tr>
                                       </table></td>
                                     </tr>

                                   </table>						          </td>
								</tr>
                                <tr> 
                                  <td width="4%">&nbsp;</td>
                                  <td width="96%" valign="top">
                                    
                                                                        
                                    <table width="362" border="0" align="center" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">

                                      <tr>
                                        <th width="409" scope="col"><table width="332" border="0" align="center" cellpadding="4" cellspacing="1">
                                              <tr bgcolor="#DFEBFF" class="style3">
                                                <td align="center" bgcolor="#CC6666"><?php if ($pageNum_citasxnumero > 0) { // Show if not first page ?>
                                                  <a href="<?php printf("%s?pageNum_citasxnumero=%d%s", $currentPage, 0, $queryString_citasxnumero); ?>"><img src="imagenes/first.png" width="16" height="16" border="0" /></a>
                                                <?php } // Show if not first page ?></td>
                                                <td align="center" bgcolor="#CC6666"><span class="style25">D&iacute;a Cita</span></td>
                                                <td colspan="2" align="center" bgcolor="#CC6666"><span class="style25">Fecha</span></td>
                                                <td align="center" bgcolor="#CC6666"><?php if ($pageNum_citasxnumero < $totalPages_citasxnumero) { // Show if not last page ?>
                                                  <a href="<?php printf("%s?pageNum_citasxnumero=%d%s", $currentPage, $totalPages_citasxnumero, $queryString_citasxnumero); ?>"><img src="imagenes/last.png" width="16" height="16" border="0" /></a>
                                                <?php } // Show if not last page ?></td>
                                              </tr>
                                              <tr bgcolor="#DFEBFF" class="style3">
                                                <td align="center"><?php if ($pageNum_citasxnumero > 0) { // Show if not first page ?>
                                                  <a href="<?php printf("%s?pageNum_citasxnumero=%d%s", $currentPage, max(0, $pageNum_citasxnumero - 1), $queryString_citasxnumero); ?>"><img src="imagenes/left.png" width="16" height="16" border="0" /></a>
                                                  <?php } // Show if not first page ?></td>
                                                <td align="center" bgcolor="#DFEBFF"><span class="style24">
                                                  <?php  if ($totalRows_citasxnumero > 0) 
										 {  $ano = substr($row_citasxnumero['fecha'],0,4); 
	                                        $mes = substr($row_citasxnumero['fecha'],5,2);
			                                $mes = str_pad($mes, 2, "0", STR_PAD_LEFT);  
			                                $dia = substr($row_citasxnumero['fecha'],8,2); 
                                            $dia = str_pad($dia, 2, "0", STR_PAD_LEFT); 					 			 
										    echo dia_semana($dia, $mes, $ano); }?>
                                                </span></td>
                                                <td colspan="2" align="center"><span class="style24"><?php echo $row_citasxnumero['fecddmmaa']; ?></span></td>
                                                <td align="center"><?php if ($pageNum_citasxnumero < $totalPages_citasxnumero) { // Show if not last page ?>
                                                  <a href="<?php printf("%s?pageNum_citasxnumero=%d%s", $currentPage, min($totalPages_citasxnumero, $pageNum_citasxnumero + 1), $queryString_citasxnumero); ?>"><img src="imagenes/right.png" width="16" height="16" border="0" /></a>
                                                <?php } // Show if not last page ?></td>
                                              </tr>
                                              <tr bgcolor="#DFEBFF" class="style3">
                                                <td align="center" bgcolor="#CC6666">&nbsp;</td>
                                                <td align="center" bgcolor="#CC6666">&nbsp;</td>
                                                <td align="center" bgcolor="#CC6666">&nbsp;</td>
                                                <td align="center" bgcolor="#CC6666">&nbsp;</td>
                                                <td align="center" bgcolor="#CC6666">&nbsp;</td>
                                              </tr>
                                              <tr bgcolor="#DFEBFF" class="style3">
                                                <td width="65" align="center">D&iacute;a Cita </td>
                                                <td width="99" align="center" bgcolor="#DFEBFF">Fecha</td>
                                                <td width="49" align="center">N&uacute;mero</td>
                                                <td width="41" align="center">Hora</td>
                                                <td width="41" align="center">Cl&iacute;nica</td>
                                              </tr>
                                              <?php $Reg=0; ?>
                                              <?php do { $Reg++;?>
                                              <?PHP if ( $Reg % 2 == 0 )
    {echo "<tr onMouseOver=".'"'."uno(this,'#006699');".'"'."onMouseOut=".'"'."dos(this,'E8E8E8');".'"'." bgcolor='E8E8E8' >";} 
    else {echo "<tr onMouseOver=".'"'."uno(this,'#006699');".'"'."onMouseOut=".'"'."dos(this,'D8D8D8');".'"'." bgcolor='D8D8D8' >";} ?>
                                              <?PHP 
		    if ($totalRows_citasxnumero > 0) {
		    $ano = substr($row_citasxnumero['fecha'],0,4); 
	        $mes = substr($row_citasxnumero['fecha'],5,2);
			$mes = str_pad($mes, 2, "0", STR_PAD_LEFT);  
			$dia = substr($row_citasxnumero['fecha'],8,2); 
            $dia = str_pad($dia, 2, "0", STR_PAD_LEFT); 
			$fecha_ant = "";			}
	   ?>
                                              
                                                <td height="22" align="center"><span class="style3">
                                                  <?php  if ($totalRows_citasxnumero > 0) {$diasem = dia_semana($dia, $mes, $ano); }?>
                                                  <?php color_fontnum($row_citasxnumero['tomado'],$row_citasxnumero['numero'],$row_citasxnumero['fecha'],$row_citasxnumero['hora'],$diasem); ?>
                                                </span></td>
                                                <td height="22" align="center"><span class="style7">
                                                  <?php color_fontnum($row_citasxnumero['tomado'],$row_citasxnumero['numero'],$row_citasxnumero['fecha'],$row_citasxnumero['hora'],$row_citasxnumero['fecddmmaa']); ?>
                                                </span></td>
                                                <td height="22" align="center"><span class="style3"><strong>
                                                  <?php color_fontnum($row_citasxnumero['tomado'],$row_citasxnumero['numero'],$row_citasxnumero['fecha'],$row_citasxnumero['hora'],$row_citasxnumero['numero']); ?>
                                                </strong></span></td>
                                                <td height="22"><?php color_fontnum($row_citasxnumero['tomado'],$row_citasxnumero['numero'],$row_citasxnumero['fecha'],$row_citasxnumero['hora'],$row_citasxnumero['hora']); ?></td>
                                                <td>&nbsp;</td>
                                                <?php $fecha_ant = $row_citasxnumero['fecha']; ?>
                                              </tr>
                                              <?php } while ($row_citasxnumero = mysql_fetch_assoc($citasxnumero)); ?>
                                          </table></th>
                                      </tr>
                                      <tr>
                                        <th scope="col">&nbsp;</th>
                                      </tr>
                                    </table>
                                    
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
            <span class="normalGrisHover"><a href="/comentarios/" onMouseOver="MM_swapImage('Image21111','','/imagenes/segundonivel/comentarios-over.gif',1)" onMouseOut="MM_swapImgRestore()"></a></span><span class="normalGrisHover"><br>
            </span><span class="normalGrisHover"> </span> </td>
        </tr>
      </table></td>
  </tr>
</table>

</body>
</html>