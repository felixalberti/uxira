<?php
$page_security = "SA_MEDICAL_APPOINMENT_PROCESSCALENDARY";
$path_to_root="../..";
include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/includes/ui.inc");
//require_once('connections/conexion2.php');

simple_page_mode(true);

page(_("Generar Calendarios"));

$error = "";
//mysql_select_db($database_conexion2, $conexion2);
$query_rs_medico = "SELECT * FROM ".TB_PREF."medico_master";
//$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
$rs_medico = db_query($query_rs_medico, "Can't retrieve record 0_medico_master");
//$row_rs_medico = mysql_fetch_assoc($rs_medico);
$row_rs_medico = db_fetch_assoc($rs_medico);
//$totalRows_rs_medico = mysql_num_rows($rs_medico);
$totalRows_rs_medico = db_num_rows($rs_medico);

//mysql_select_db($database_conexion2, $conexion2);
$query_rs_tipo_cita = "SELECT * FROM ".TB_PREF."cm_system_tables tb WHERE tb.codigotabla = 'TPCITA'";
//$rs_tipo_cita = mysql_query($query_rs_tipo_cita, $conexion2) or die(mysql_error());
$rs_tipo_cita = db_query($query_rs_tipo_cita, "Can't retrieve records 0_cm_system_tables");
//$row_rs_tipo_cita = mysql_fetch_assoc($rs_tipo_cita);
$row_rs_tipo_cita = db_fetch_assoc($rs_tipo_cita);
//$totalRows_rs_tipo_cita = mysql_num_rows($rs_tipo_cita);
$totalRows_rs_tipo_cita = db_num_rows($rs_tipo_cita);


function numerodia($dia){
	$semanaArray = array(
		"Domingo" => "1", 
		"Lunes" => "2", 
		"Martes" => "3", 
		"Miércoles" => "4", 
		"Jueves" => "5", 
		"Viernes" => "6", 
		"Sábado" => "7" );
$numReturn = $semanaArray[$dia];
return $numReturn;
}

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


function generar_cod($dia) {
echo ($dia == 1)? 'checked="checked"' : ''; 
}


function dias_x_semana($colname_dia,$medico,$hostname,$db,$user,$pass) {
	//$conexion3 = mysql_pconnect($hostname, $user, $pass) or trigger_error(mysql_error(),E_USER_ERROR); 
	//mysql_select_db($db, $conexion3);
	
	$query_rs_semana = sprintf("SELECT * FROM ".TB_PREF."cm_diatrabxmedico ds, ".TB_PREF."cm_tabla_dias td where ds.enuso = 1 and td.nombre = %s and ds.codmed = %s and td.codigo = ds.coddia", "'".$colname_dia."'","'".$medico."'");
	//$rs_semana = mysql_query($query_rs_semana, $conexion3) or die(mysql_error());
        $rs_semana = db_query($query_rs_semana, "Can't retrieve records 0_cm_diatrabxmedico");
	//$row_rs_semana = mysql_fetch_assoc($rs_semana);
        $row_rs_semana = db_fetch_assoc($rs_semana);
	//$totalRows_rs_semana = mysql_num_rows($rs_semana);
        $totalRows_rs_semana = db_num_rows($rs_semana);
	echo '<br> query: '.$query_rs_semana;
	echo '<br>dias_x_semana --> cant: '.$totalRows_rs_semana;
	if ($totalRows_rs_semana > 0) 
	   {return true; } 
	else 
	   {return false; }
}

function dias_x_medico($medico,$hostname,$db,$user,$pass) {
	//$conexion3 = mysql_pconnect($hostname, $user, $pass) or trigger_error(mysql_error(),E_USER_ERROR); 
	//mysql_select_db($db, $conexion3);
	
	$query_rs_semana = sprintf("SELECT count(*) as cant FROM ".TB_PREF."cm_diatrabxmedico ds where ds.enuso = 1 and ds.codmed like %s","'".$medico."'");
	echo '<br> query dias x semana: '.$query_rs_semana;
	//$rs_semana = mysql_query($query_rs_semana, $conexion3) or die(mysql_error());
        $rs_semana = db_query($query_rs_semana, "Can't retrieve records 0_cm_diatrabxmedico");
	//$row_rs_semana = mysql_fetch_assoc($rs_semana);
        $row_rs_semana = db_fetch_assoc($rs_semana);
	echo ' Cant: '.$row_rs_semana['cant'].' ';
	if ($row_rs_semana['cant'] > 0) 
	   {return true; } 
	else 
	   {return false; }
}

function generar_num($afecha, $codmed, $tipo_cita) {
//Hacer ciclo y leer tablas que guarda los n�meros
//$query_rs_numeros = sprintf("SELECT * FROM numeroscita where fecha = %s and tipo_cita = %s", "'".$fec_config."'", "'".$tipo_cita."'");
//echo $afecha."-->";
$ano = substr($afecha,0,4); 
$mes = substr($afecha,5,2);
$mes = str_pad($mes, 2, "0", STR_PAD_LEFT);  
$dia = substr($afecha,8,2); 
$dia = str_pad($dia, 2, "0", STR_PAD_LEFT); 
$diasemana = dia_semana($dia,$mes,$ano);
//echo $diasemana;
echo " (".$dia.$mes.$ano.")";
$numdia = numerodia($diasemana);
//echo "dia-".$numdia."......";
$query_rs_numeros = sprintf("SELECT rm.coddia, nc.tipo_cita, nc.numero, hora_cita FROM ".TB_PREF."cm_rel_medpatronxdia rm, ".TB_PREF."cm_numeroscita nc where rm.codmed like '%s' and nc.patron = rm.patron and rm.coddia = '%s' and nc.tipo_cita like '%s'", $codmed, $numdia, $tipo_cita);
//mysql_select_db($database_conexion2, $conexion2);
//$rs_numeros = mysql_query($query_rs_numeros, $conexion2) or die(mysql_error());
$rs_numeros = db_query($query_rs_numeros, "Can't retrieve records 0_cm_rel_medpatronxdia");
//$row_rs_numeros = mysql_fetch_assoc($rs_numeros);
$row_rs_numeros = db_fetch_assoc($rs_numeros);
//$totalRows_rs_numeros = mysql_num_rows($rs_numeros);
$totalRows_rs_numeros = db_num_rows($rs_numeros);
echo "<br>Registros: ".$totalRows_rs_numeros;
//if ($codmed='200'){echo "200-->Registros: ".$totalRows_rs_numeros;}
	if ($totalRows_rs_numeros > 0 and $enuso = 1) {
	  do { 
		  $numero = $row_rs_numeros['numero'];
		  //echo $codmed.$afecha.$numero.$row_rs_numeros['tipo_cita'].$numdia."...";
		  $insertSQL = sprintf("INSERT INTO ".TB_PREF."cm_fechasxsemana (fecha, numero, tipocita, hora, codmed, tomado, dia) VALUES (%s, %s, %s, %s, %s, %s, %s)",
								   GetSQLValueString($afecha, "date"),
								   GetSQLValueString($numero, "text"),
								   GetSQLValueString($row_rs_numeros['tipo_cita'],"text"),
								   GetSQLValueString($row_rs_numeros['hora_cita'],"text"),
                                                                   GetSQLValueString($codmed, "text"),								   
								   '0',
                                                                   GetSQLValueString($numdia, "text"));
				
		  //mysql_select_db($database_conexion2, $conexion2);
		  //$Result1 = mysql_query($insertSQL, $conexion2) or die("fechaxsemana ".$afecha." ".mysql_error());	 
                  $Result1 = db_query($insertSQL, "Can't insert records into 0_cm_fechasxsemana");
		  //if ($Result1) {echo "Si   ";} else {echo "No   ";};
		  //} while ($row_rs_numeros = mysql_fetch_assoc($rs_numeros));
                  } while ($row_rs_numeros = db_fetch_assoc($rs_numeros));
	}
}

/*  Funci�n dia_semana by PaToRoCo (www.patoroco.net)
Se permite la distribuci�n total y modificaci�n de la funci�n, siempre que se nombre al autor */

function dia_semana ($dia, $mes, $ano) {
    $dias = array('Domingo', 'Lunes', 'Martes', 'Miércoles', 'Jueves', 'Viernes', 'Sábado');
    return $dias[date("w", mktime(0, 0, 0, $mes, $dia, $ano))];
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
//


//**************************************
//if ((isset($_POST["MM_insert"])) && ($_POST["MM_insert"] == "form1")) {

     //Actualiza fechas según tipo de cita y médico
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	$codmed = $_POST['medico'];
	echo 'Médico: '.$codmed;
	$query_rs_cita = "SELECT * FROM ".TB_PREF."cm_citas WHERE fecha_cita >= '".date("Y/m/d")."' and codmed like '".$codmed."'";
	echo $query_rs_cita.' Generar Fechas: '.$_POST["generarfecha"];
		
	    //mysql_select_db($database_conexion2, $conexion2);		
	    //$rs_cita = mysql_query($query_rs_cita, $conexion2) or die(mysql_error());
            $rs_cita = db_query($query_rs_cita, "Can't retrieved records into 0_cm_citas");
	    //$row_rs_cita = mysql_fetch_assoc($rs_cita);
            $row_rs_cita = db_fetch_assoc($rs_cita);
	    //$totalRows_rs_cita = mysql_num_rows($rs_cita);
            $totalRows_rs_cita = db_num_rows($rs_cita);
	    if ($totalRows_rs_cita) {
	       $error = "Existen citas para la fecha de generación, no se generaron las fechas";
		} 
		else {$error = "";}
	
	$bdiastrabajo = dias_x_medico($codmed,$hostname_conexion2,$database_conexion2,$username_conexion2,$password_conexion2);	
	if(!($bdiastrabajo)){
	  $error = "<br>El(Los) médico(s) no tiene(n) dáas de trabajo activos";
	  echo "El(Los) médico(s) no tiene(n) d�as de trabajo activos";
  }
	else
	echo "<br>El(Los) médico(s) tiene(n) días de trabajo activos ";
	
	if  (isset($_POST["generarfecha"]) && ($_POST['generarfecha'] == "1" && $error == "" && $bdiastrabajo)) {
	

		$codmed = $_POST['medico'];

		$deleteSQL = "DELETE FROM ".TB_PREF."cm_semanacita where codmed like '".$codmed."' and fecha >= '".date("Y/m/d")."'";
		//mysql_select_db($database_conexion2, $conexion2);
		//$Result1 = mysql_query($deleteSQL, $conexion2) or die(mysql_error());
                $Result1 = db_query($deleteSQL, "Can't delete records into 0_cm_semanacita");
		echo '<br>'.$deleteSQL;
		$deleteSQL = "DELETE FROM ".TB_PREF."cm_fechasxsemana where codmed like '".$codmed."' and fecha >='".date("Y/m/d"). "' and numero in (SELECT nc.numero FROM ".TB_PREF."cm_rel_medpatronxdia rm, ".TB_PREF."cm_numeroscita nc where rm.codmed like '".$codmed."' and rm.patron = nc.patron and nc.tipo_cita like '".$_POST['tipocita']. "' )";
		//mysql_select_db($database_conexion2, $conexion2);
		//$Result1 = mysql_query($deleteSQL, $conexion2) or die(mysql_error());
                $Result1 = db_query($deleteSQL, "Can't delete records into 0_cm_fechasxsemana");
		echo '<br>'.$deleteSQL;
		
		//Para meses de 30 dias
		for ($mes = 4; $mes <= 11; $mes++) {
			if ($mes == 4 or $mes == 6 or $mes == 9 or $mes == 11) {
			for ($dia = 1; $dia <= 30; $dia++) {
			  $fecha = $_POST['year']."/".str_pad($mes, 2, "0", STR_PAD_LEFT)."/".$dia;
			  $fechacomp = $_POST['year'].str_pad($mes, 2, "0", STR_PAD_LEFT).str_pad($dia, 2, "0", STR_PAD_LEFT);
			  if ($fechacomp >= date("Ymd")) {			  
				  $diauso = dia_semana($dia,str_pad($mes, 2, "0", STR_PAD_LEFT),$_POST['year']);
				 
				  $semana = strftime("%U",strtotime($fecha)) ;
				  $query_rs_medico = "SELECT * FROM ".TB_PREF."medico_master where medico_no like '".$codmed."'";
				  //$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
                                  $rs_medico = db_query($query_rs_medico, "Can't retrieve records into 0_medico_master");
				  //$row_rs_medico = mysql_fetch_assoc($rs_medico);
                                  $row_rs_medico = db_fetch_assoc($rs_medico);
				  //$cant = mysql_num_rows($rs_medico);
                                  $cant = db_num_rows($rs_medico);
				  //echo "aja";//,settype($cant,'string');
				  echo '<br>dia:'.$dia.' - mes:'.$mes;
				  do { 
						  $codmedaux = $row_rs_medico['medico_no'];
						  if (dias_x_semana($diauso,$codmedaux,$hostname_conexion2,$database_conexion2,$username_conexion2,$password_conexion2))
							   {$enuso = 1; }
						  else
							   {$enuso = 0;
						  continue;}					  
						  echo $codmed."-".$codmedaux;
						  $insertSQL = sprintf("INSERT INTO ".TB_PREF."cm_semanacita (semana, fecha, codmed, enuso) VALUES (%s, %s, %s, %s)",
											   GetSQLValueString($semana, "date"),
											   GetSQLValueString($fecha, "text"),
											   GetSQLValueString($codmedaux, "text"),						   
											   GetSQLValueString($enuso, "text"));
						
						  //mysql_select_db($database_conexion2, $conexion2);
						  //$Result1 = mysql_query($insertSQL, $conexion2) or die("semanacita ".$fecha." ".mysql_error());
                                                  $Result1 = db_query($insertSQL, "Can't insert records into 0_cm_semanacita");
						  //
						  //Hacer ciclo y leer tablas que guarda los n�meros
						  echo "Grabar".$codmed."-".$codmedaux." ".$fecha;	 
						  generar_num($fecha, $codmedaux, $_POST['tipocita']);
				  
				  //} while ($row_rs_medico = mysql_fetch_assoc($rs_medico));
                                  } while ($row_rs_medico = db_fetch_assoc($rs_medico));
				}  
			  
			}  
			}
		}
		//Para meses de 31 dias
		for ($mes = 1; $mes <= 12; $mes++) {
			if ($mes == 1 or $mes == 3 or $mes == 5 or $mes == 7 or $mes == 8 or $mes == 10 or $mes == 12) {
			for ($dia = 1; $dia <= 31; $dia++) {
			  $fecha = $_POST['year']."/".str_pad($mes, 2, "0", STR_PAD_LEFT)."/".$dia;
			  $fechacomp = $_POST['year'].str_pad($mes, 2, "0", STR_PAD_LEFT).str_pad($dia, 2, "0", STR_PAD_LEFT);
			  if ($fechacomp >= date("Ymd")) {	
				   $diauso = dia_semana($dia,str_pad($mes, 2, "0", STR_PAD_LEFT),$_POST['year']);
				   
				  $semana = strftime("%U",strtotime($fecha)) ;
				  $query_rs_medico = "SELECT * FROM ".TB_PREF."medico_master where medico_no like '".$codmed."'";                                  
				  //$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
                                  $rs_medico = db_query($query_rs_medico, "Can't retrieve records into 0_medico_master");
				  //$row_rs_medico = mysql_fetch_assoc($rs_medico);
                                  $row_rs_medico = db_fetch_assoc($rs_medico);
				  echo '<br>dia:'.$dia.' - mes:'.$mes;
				  do { 
						  $codmedaux = $row_rs_medico['medico_no'];
							if (dias_x_semana($diauso,$codmedaux,$hostname_conexion2,$database_conexion2,$username_conexion2,                           $password_conexion2))
							  {$enuso = 1;}
						   else
							  {$enuso = 0;
							   continue;}					  
						  $insertSQL = sprintf("INSERT INTO ".TB_PREF."cm_semanacita (semana, fecha, codmed, enuso) VALUES (%s, %s, %s, %s)",
											   GetSQLValueString($semana, "date"),
											   GetSQLValueString($fecha, "text"),
											   GetSQLValueString($codmedaux, "text"),						   
											   GetSQLValueString($enuso, "text"));
						
						  //mysql_select_db($database_conexion2, $conexion2);
						  //$Result1 = mysql_query($insertSQL, $conexion2) or die(mysql_error());
                                                  $Result1 = db_query($insertSQL, "Can't insert records into 0_cm_semanacita");
						  //Hacer ciclo y leer tablas que guarda los n�meros	  	  
						  generar_num($database_conexion2, $conexion2, $fecha, $codmedaux, $_POST['tipocita']);
				  
				  //} while ($row_rs_medico = mysql_fetch_assoc($rs_medico));
                                  } while ($row_rs_medico = db_fetch_assoc($rs_medico));
			   }  
			}  
			}
		}
		//Para el mes de febrero
		$mes = 2;
		$dialim = 0;
		//$ano = date("Y");
		$ano = $_POST['year'];
		//Cada cuatrocientos años se elimina tres bisiestos
		if (($ano % 4 == 0) && (($ano % 100 != 0) || ($ano % 400 == 0)))
		{$dialim = 29;}
		else 
		{$dialim = 28;}
		
		for ($dia = 1; $dia <= $dialim; $dia++) {
			  //Validar si el año es Bisiesto
			  $fecha = $_POST['year']."/".str_pad($mes, 2, "0", STR_PAD_LEFT)."/".$dia;
  			  $fechacomp = $_POST['year'].str_pad($mes, 2, "0", STR_PAD_LEFT).str_pad($dia, 2, "0", STR_PAD_LEFT);
			  if ($fechacomp >= date("Ymd")) {
				  $diauso = dia_semana($dia,str_pad($mes, 2, "0", STR_PAD_LEFT),$_POST['year']);
				 
				  $semana = strftime("%U",strtotime($fecha)) ;
				  $query_rs_medico = "SELECT * FROM ".TB_PREF."cm_medico_master where medico_no like '".$codmed."'";
				  //$rs_medico = mysql_query($query_rs_medico, $conexion2) or die(mysql_error());
                                  $rs_medico = db_query($query_rs_medico, "Can't retrieve records into 0_cm_medico_master");
				  //$row_rs_medico = mysql_fetch_assoc($rs_medico);
                                  $row_rs_medico = db_fetch_assoc($rs_medico);
				  
				  do {
						  $codmedaux = $row_rs_medico['medico_no'];
						  if (dias_x_semana($diauso,$codmed,$hostname_conexion2,$database_conexion2,$username_conexion2,                          $password_conexion2))
							  {$enuso = 1;}
						  else
							  {$enuso = 0;
							   continue;}					   			   
						  $insertSQL = sprintf("INSERT INTO ".TB_PREF."cm_semanacita (semana, fecha, codmed, enuso) 
											   VALUES (%s, %s, %s, %s)",
											   GetSQLValueString($semana, "date"),
											   GetSQLValueString($fecha, "text"),
											   GetSQLValueString($codmedaux, "text"),						   
											   GetSQLValueString($enuso, "text"));
						
						  //mysql_select_db($database_conexion2, $conexion2);
						  //$Result1 = mysql_query($insertSQL, $conexion2) or die(mysql_error());
                                                  $Result1 = db_query($insertSQL, "Can't inserte records into 0_cm_semanacita");
						  //Hacer ciclo y leer tablas que guarda los n�meros	  	  
						  generar_num($database_conexion2, $conexion2, $fecha, $codmedaux, $_POST['tipocita']);
				  
				  //} while ($row_rs_medico = mysql_fetch_assoc($rs_medico));
                                  } while ($row_rs_medico = db_fetch_assoc($rs_medico));
			   }  
		}  
	
	}  //%%%%%%%%%%%%%%%%%%%%%%
//} //***********************
$p_time = 180;
$t_url = "genera_calen.php?"."&msj=".$error;   
echo "\t<meta http-equiv=\"Refresh\" content=\"$p_time;URL=$t_url\" />\n";

end_page();
?>