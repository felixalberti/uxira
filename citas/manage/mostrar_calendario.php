<?
$anoInicial = date("Y")-1;
$anoFinal = date("Y")+1;
//$funcionTratarFecha = 'document.location = "?dia="+dia+"&mes="+mes+"&ano="+ano;';
//$funcionTratarFecha = "alert(dia+' '+mes+' '+ano);";
$funcionTratarFecha = "FAjax('mostrar_datos_fecha_cita.php','mostrarfecha',
'medico='+document.getElementById('medico').value+'&tipocita='+document.getElementById('tipocita').value+
'&fecha_desde='+dia+'/'+mes+'/'+ano+'&fecha_hasta='+dia+'/'+mes+'/'+ano,'POST');";
$rell_dias = "dia=rellenar(String(dia));";
$rell_mes = "mes=rellenar(String(mes));";
?><script>
function tratarFecha(dia,mes,ano){
  <?=$rell_dias?>
  <?=$rell_mes?>  	
  <?=$funcionTratarFecha?>
}
</script>
<style>
.m1 {
   font-family:MS Sans Serif;
   font-size:24pt
}
a {
   text-decoration:none;
   color:#000000;
}
</style>
</head>
<body>
<form><table border="0" cellpadding="5" cellspacing="0" bgcolor="#990000">
  <tr>
    <td width="100%">
<?
$fecha = getdate(time());
if(isset($_GET["dia"]))$dia = $_GET["dia"];
else $dia = $fecha['mday'];
if(isset($_GET["mes"]))$mes = $_GET["mes"];
else $mes = $fecha['mon'];
if(isset($_GET["ano"]))$ano = $_GET["ano"];
else $ano = $fecha['year'];
$fecha = mktime(0,0,0,$mes,$dia,$ano);
$fechaInicioMes = mktime(0,0,0,$mes,1,$ano);
$fechaInicioMes = date("w",$fechaInicioMes);
?>
    <select size="1" name="mes" class="m1" onchange="document.location = '?dia=<?=$dia?>&mes=' + document.forms[0].mes.value + '&ano=<?=$ano?>';">
<?
$meses = Array ('enero','febrero','marzo','abril','mayo','junio','julio','agosto','septiembre','octubre','noviembre','diciembre');
for($i = 1; $i <= 12; $i++){
  echo '      <option ';
  if($mes == $i)echo 'selected ';
  echo 'value="'.$i.'">'.$meses[$i-1]."\n";
}
?>
    </select>&nbsp;&nbsp;&nbsp;<select size="1" name="ano" class="m1" onchange="document.location = '?dia=<?=$dia?>&mes=<?=$mes?>&ano=' + document.forms[0].ano.value;">
<?
for ($i = $anoInicial; $i <= $anoFinal; $i++){
  echo '      <option ';
  if($ano == $i)echo 'selected ';
  echo 'value="'.$i.'">'.$i."\n";
}
?>
    </select><br>
    <font size="1">&nbsp;</font><table border="0" cellpadding="2" cellspacing="0" width="100%" class="m1" bgcolor="#FFFFFF" height="100%">
<?
$diasSem = Array ('L','M','M','J','V','S','D');
$ultimoDia = date('t',$fecha);
$numMes = 0;
for ($fila = 0; $fila < 7; $fila++){
  echo "      <tr>\n";
  for ($coln = 0; $coln < 7; $coln++){
    $posicion = Array (1,2,3,4,5,6,0);
    echo '        <td width="14%" height="19"';
    if($fila == 0)echo ' bgcolor="#FF0000"';
    if($dia-1 == $numMes)echo ' bgcolor="#0A246A"';
    echo " align=\"center\">\n";
    echo '        ';
    if($fila == 0)echo '<font color="#D4D0C8">'.$diasSem[$coln];
    elseif(($numMes && $numMes < $ultimoDia) || (!$numMes && $posicion[$coln] == $fechaInicioMes)){
      echo '<a href="#" onclick="tratarFecha('.(++$numMes).','.$mes.','.$ano.')">';
      if($dia == $numMes)echo '<font color="#FFFFFF">';
      echo ($numMes).'</a>';
    }
    echo "</td>\n";
  }
  echo "      </tr>\n";
}
?>
    </table>
    </td>
  </tr>
</table></form>
