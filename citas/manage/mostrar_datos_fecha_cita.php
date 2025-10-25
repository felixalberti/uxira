<?php
session_start();
require_once('connections/conexion2.php');
if (isset($_POST['tipocita'])) {
  $tipocita = $_POST['tipocita'];  
}
else {
   $tipocita = 1;  
}
if (isset($_POST['medico'])) {
  $medico = $_POST['medico'];  
}
else {
   $medico = 1;  
}
//
$currentPage = $_SERVER["PHP_SELF"];
$maxRows_citasxnumero = 10;
$pageNum_citasxnumero = 0;
if (isset($_GET['pageNum_citasxnumero'])) {
  $pageNum_citasxnumero = $_GET['pageNum_citasxnumero'];
}
$startRow_citasxnumero = $pageNum_citasxnumero * $maxRows_citasxnumero;
//
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
    global $tipocita, $medico;

	if ($val == 1) {
	echo '<a style="text-decoration:none" ><font color="#FF0000" >'.$mostrar."</font></a>";
	}
	else {
	if (isset($_SESSION['ci_usuario']) && isset($_SESSION['medico']) && isset($_SESSION['tipocita'])) {
	   echo '<a style="text-decoration:none" href="cm_confirm_cita.php?numero='.$val2.'&hora='.$hora.'&fecha='.$fecha.         '&medico='.$medico.'&tipocita='.$tipocita.'"><font         color="#000000" >'.   $mostrar."</font></a>";}
	else {   
	   echo '<a style="text-decoration:none" href="cm_entrada.php?numero='.$val2.'&hora='.$hora.'&fecha='.$fecha.'&medico='.$medico.'&tipocita='.$tipocita.'"><font color="#000000" >'.   $mostrar."</font></a>";}
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

if (isset($_POST['fecha_desde']) && $_POST['fecha_desde']!="" && isset($_POST['fecha_hasta']) && $_POST['fecha_hasta']!=""){
	 $date_ = $_POST['fecha_desde'];
	 $day = substr($date_,0,2);
	 $month = substr($date_,3,2);
	 $year = substr($date_,6,4);
	 $fecha_inicial = $year.$month.$day;
	 $date_ = $_POST['fecha_hasta'];
   $day = substr($date_,0,2);
	 $month = substr($date_,3,2);
	 $year = substr($date_,6,4);	 
	 $fecha_limite = $year.$month.$day;
	 //
   mysql_select_db($database_conexion2, $conexion2);
	 $query_citasxnumero = "SELECT sc.fecha, fxs.numero, fxs.tomado, fxs.hora, DATE_FORMAT(sc.fecha,'%d/%m/%Y') as fecddmmaa FROM 0_cm_semanacita sc, 0_cm_fechasxsemana fxs WHERE sc.fecha = fxs.fecha and sc.enuso = 1 and (fxs.tipocita = '".$tipocita."' or fxs.tipocita is null or fxs.tipocita = '' ) and fxs.codmed = '".$medico."'";
	 $query_citasxnumero = sprintf("%s and (sc.fecha >= %s and sc.fecha <= %s) and sc.codmed = %s", $query_citasxnumero, "'".$fecha_inicial."'","'".$fecha_limite."'","'".$medico."'");
	 $query_citasxnumero = sprintf("%s %s", $query_citasxnumero, " order by sc.fecha, fxs.numero");
	 $query_limit_citasxnumero = sprintf("%s LIMIT %d, %d", $query_citasxnumero, $startRow_citasxnumero, $maxRows_citasxnumero);
   $citasxnumero = mysql_query($query_limit_citasxnumero, $conexion2) or die(mysql_error());
	 $row_citasxnumero = mysql_fetch_assoc($citasxnumero);
	 $totalRows_citasxnumero = mysql_num_rows($citasxnumero);	 
	 //echo '<br>sisisi'.$query_limit_citasxnumero;
}
else {
		$fecha_inicial = calcularFecha($diasarranque); //Obtener la cantidad de días a mostrar de la BD o un archivo.ini
		$fecha_limite = calcularFecha($diasfinal); //Obtener la cantidad de días a mostrar de la BD o un archivo.ini
		do {
		mysql_select_db($database_conexion2, $conexion2);
		$query_citasxnumero = "SELECT sc.fecha, fxs.numero, fxs.tomado, fxs.hora, DATE_FORMAT(sc.fecha,'%d/%m/%Y') as fecddmmaa FROM 0_cm_semanacita sc, 0_cm_fechasxsemana fxs WHERE sc.fecha = fxs.fecha and sc.enuso = 1 and (fxs.tipocita = '".$tipocita."' or fxs.tipocita is null or fxs.tipocita = '' ) and fxs.codmed = '".$medico."'";
		$query_citasxnumero = sprintf("%s and (sc.fecha >= %s and sc.fecha <= %s) and sc.codmed = %s", $query_citasxnumero, "'".$fecha_inicial."'","'".$fecha_limite."'","'".$medico."'");
		$query_citasxnumero = sprintf("%s %s", $query_citasxnumero, " order by sc.fecha, fxs.numero");
		$query_limit_citasxnumero = sprintf("%s LIMIT %d, %d", $query_citasxnumero, $startRow_citasxnumero, $maxRows_citasxnumero);
		//echo '<br>'.$query_limit_citasxnumero;
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
}

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

//Rigth
if ($pageNum_citasxnumero < $totalPages_citasxnumero) {   
   $ejecutar = "FAjax('".sprintf("%s?pageNum_citasxnumero=%d%s", $currentPage, min($totalPages_citasxnumero, $pageNum_citasxnumero + 1), $queryString_citasxnumero)."','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');";          
   $right = "<input name='right' id='right' value='right'".
			   " onKeyDown=\"".$ejecutar."\" onKeyUp=\"".$ejecutar."\" onMouseDown=\"".$ejecutar."\" type='image'".
			   " src='imagenes/right.png' />";
}
//Left
if ($pageNum_citasxnumero > 0) {			   
   $ejecutar = "FAjax('".sprintf("%s?pageNum_citasxnumero=%d%s", $currentPage, max(0, $pageNum_citasxnumero - 1), $queryString_citasxnumero)."','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');";          	
   $left = "<input name='left' id='left' value='left'".
			   " onKeyDown=\"".$ejecutar."\" onKeyUp=\"".$ejecutar."\" onMouseDown=\"".$ejecutar."\" type='image'".
			   " src='imagenes/left.png' />";			   
}			   
//First
if ($pageNum_citasxnumero > 0) {			   
   $ejecutar = "FAjax('".sprintf("%s?pageNum_citasxnumero=%d%s", $currentPage, 0, $queryString_citasxnumero)."','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');";           
   $first = "<input name='first' id='first' value='first'".
			   " onKeyDown=\"".$ejecutar."\" onKeyUp=\"".$ejecutar."\" onMouseDown=\"".$ejecutar."\" type='image'".
			   " src='imagenes/first.png' />";			   
}	
//Last		   
if ($pageNum_citasxnumero < $totalPages_citasxnumero) {	
   $ejecutar = "FAjax('".sprintf("%s?pageNum_citasxnumero=%d%s", $currentPage, $totalPages_citasxnumero, $queryString_citasxnumero)."','mostrarfecha','medico='+document.getElementById('medico').value+'&amp;tipocita='+document.getElementById('tipocita').value+'&amp;fecha_desde='+document.getElementById('fecha_desde').value+'&amp;fecha_hasta='+document.getElementById('fecha_hasta').value,'POST');";          			   
   $last = "<input name='last' id='last' value='last'".
			   " onKeyDown=\"".$ejecutar."\" onKeyUp=\"".$ejecutar."\" onMouseDown=\"".$ejecutar."\" type='image'".
			   " src='imagenes/last.png' />";			   
}			  
			  		   			   			   
?>
<br>
<br>
<table width="362" border="0" align="center" cellpadding="0" cellspacing="0" bordercolor="#CCCCCC">
<tr>
<th width="409" scope="col"><table width="332" border="0" align="center" cellpadding="4" cellspacing="1">
<tr bgcolor="#DFEBFF" class="style3">
<td align="center" bgcolor="#CC6666"><?php if ($pageNum_citasxnumero > 0) { // Show if not first page 
	echo $first; ?>
<?php } // Show if not first page ?></td>
<td align="center" bgcolor="#CC6666"><span class="style25">D&iacute;a Cita</span></td>
<td colspan="2" align="center" bgcolor="#CC6666"><span class="style25">Fecha</span></td>
<td align="center" bgcolor="#CC6666"><?php if ($pageNum_citasxnumero < $totalPages_citasxnumero) { // Show if not last page 
	echo $last; ?>
<?php } // Show if not last page ?></td>
</tr>
 <tr bgcolor="#DFEBFF" class="style3">
    <td align="center"><?php if ($pageNum_citasxnumero > 0) { // Show if not first page 
    	echo $left; ?>
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
   <td align="center"><?php if ($pageNum_citasxnumero < $totalPages_citasxnumero) { // Show if not last page 
   	echo $right; ?>
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