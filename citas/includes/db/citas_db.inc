<?php
/**********************************************************************
    Copyright (C) Angios, CA.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
//Begin Felix Alberti 12/07/2011
//Crea una cita en el sistema Angios Gestion Total
function crear_cita_gt($conn_sqlsvr, $plantilla, $ci_paciente, $medico, $especialidad, $fechaplanif='', $horaplanif='',
 $turnoplan, $motivoconsulta='', $observacion='' )

{
	display_notification("crear_cita_gt");
	$usuariocreacion = 77;
	$statusplanif = 1;//Citado
	$unidadserv = 14;
	$grupo = 0;
	$subgrupo = 0;
	$precio = 0;
	get_grupoysubgrupo($conn_sqlsvr,$plantilla,$grupo,$subgrupo,$precio);	
	$paciente = get_id_paciente($conn_sqlsvr,$ci_paciente);	
	$seguro = get_seguro($conn_sqlsvr,$paciente);
	$fechaplanif = date2sqlsvr($fechaplanif);
	$fecha_reg = date("Ymd");
	$horaestatus = date("h:i:s");
	$sql = "INSERT INTO 
	         Z11_PLANIFICACION (E03_UNIDADDESERVICIO, Z28_ESTATUSPLANIFICACION, Z19_PACIENTE, Z14_PERSONALMEDICO,
	         Z13_ESPECIALIDAD, F13_PLANTILLA, F15_GRUPO, F16_SUBGRUPO,
	         C08_CLIENTE, Z11_FECHAPLANIFICACION, Z11_HORAPLANIFICACION, Z30_TURNOPLAN,
	         Z11_PRECIO, Z11_MOTIVOCONSULTA, Z11_OBSERVACION,
           S04_USUARIOCREACION, Z11_WSCREACION,Z11_FECHACREACION, Z11_HORACREACION)
           VALUES (".db_escape($unidadserv).", ".db_escape($statusplanif).", ".$paciente.", ".db_escape($medico).
           ", ".db_escape($especialidad).", ".db_escape($plantilla).",".db_escape($grupo).", ".db_escape($subgrupo).
           ", ".db_escape($seguro).",".db_escape($fechaplanif).",".db_escape( $horaplanif).", ".db_escape($turnoplan).
           ", ".db_escape($precio).",".db_escape($motivoconsulta).",".db_escape($observacion).
           ", ".db_escape($usuariocreacion).", '192.168.0.32',".db_escape($fecha_reg).",".db_escape($horaestatus).")";
	$result = sqlsrv_query( $conn_sqlsvr, $sql);
	display_notification("Plantilla: ".$plantilla."-paciente: ".$paciente."-med:".$medico."- esp:".$especialidad);
	display_notification("g:".$grupo."- sub:".$subgrupo."-seguro: ".$seguro);
  display_notification($sql);
	if ( $result === false ) {
	   die( print_r( sqlsrv_errors(), true ));
	   display_notification("Error");
	}           
}

function update_citas($conn_sqlsvr,$id,$newhora,$motivoconsulta, $observacion){
$sql = "UPDATE Z11_PLANIFICACION SET Z11_HORAPLANIFICACION=".db_escape($newhora).
", Z11_MOTIVOCONSULTA=".db_escape($motivoconsulta).", Z11_OBSERVACION=".db_escape($observacion)."  WHERE Z11_PLANIFICACION = ".$id;
$result = sqlsrv_query($conn_sqlsvr, $sql );
if (!$result) display_notification('Los datos no fueron actualizados. Revise el update --> '.$sql);
else
sqlsrv_free_stmt($result);	
}	

function get_grupoysubgrupo($conn_sqlsvr,$plantilla,&$grupo,&$subgrupo,&$precio){
	$sql = "SELECT F15_GRUPO, F16_SUBGRUPO, F13_PRECIO FROM F13_PLANTILLA WHERE F13_PLANTILLA = ".$plantilla;
	//display_notification('get_grupoysubgrupo '.$sql);
	$result = sqlsrv_query($conn_sqlsvr, $sql );
	if ($result){
		$rows = sqlsrv_has_rows( $result);
    if ( $rows == true ) {
	     $row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
		   $grupo = $row['F15_GRUPO'];
		   $subgrupo = $row['F16_SUBGRUPO'];
		   $precio = $row['F13_PRECIO'];
    }
    sqlsrv_free_stmt( $result);    
  }	
}	
function get_seguro($conn_sqlsvr,$paciente){
	$sql =  "SELECT C08_CLIENTE FROM Z19_PACIENTE WHERE Z19_PACIENTE = ".$paciente;
	//display_notification('get_seguro '.$sql);
	$result = sqlsrv_query( $conn_sqlsvr,$sql);
	$seguro = "";
	if ($result){
		$rows = sqlsrv_has_rows( $result );
    if ( $rows == true ) {
	     $row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
		   $seguro = $row['C08_CLIENTE'];
		   if ($seguro == null) $seguro = '0';
    }
    else
    $seguro = '0';
    sqlsrv_free_stmt( $result);
  }	
  else
  $seguro = '0'; 
  return $seguro;
}
function get_id_paciente($conn_sqlsvr,$cedula){
	$sql = "SELECT Z19_PACIENTE FROM Z19_PACIENTE WHERE Z19_NROCEDULA = $cedula";
	//display_notification('get_id_paciente '.$sql);
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
	$id_paciente = $row['Z19_PACIENTE'];
	//display_notification('get_id_paciente '.$id_paciente);
	sqlsrv_free_stmt( $result); 
	return $id_paciente;
}

function get_cedula_paciente($conn_sqlsvr,$id){
	$sql = "SELECT Z19_NROCEDULA FROM Z19_PACIENTE WHERE Z19_PACIENTE = $id";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
	$cedula_paciente = $row['Z19_NROCEDULA'];
	sqlsrv_free_stmt( $result); 
	return $cedula_paciente;
}

function get_datoscita($conn_sqlsvr,$id){
	$sql = "SELECT Z19_PACIENTE, Z13_ESPECIALIDAD, Z14_PERSONALMEDICO,
	 F13_PLANTILLA, CONVERT(nvarchar(30), Z11_HORAPLANIFICACION, 108) AS Z11_HORAPLANIFICACION,
	 CONVERT(nvarchar(10), Z11_FECHAPLANIFICACION, 103) AS Z11_FECHAPLANIFICACION, Z11_MOTIVOCONSULTA, Z11_OBSERVACION 
	 FROM Z11_PLANIFICACION WHERE Z11_PLANIFICACION = $id";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	display_notification('get_id_paciente '.$sql);
	$datos = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
	sqlsrv_free_stmt( $result); 
	return $datos;
}				
//End Felix Alberti 12/07/2011
?>