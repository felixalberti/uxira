<?php function auditoria($numerocita,$ciusuario,$fecha_cita,$cedula,$accion,$database_conexion2,$conexion2){
  $fecactual = date("Y/m/d");
  $hora = date("h:i:s");
  $insertSQL = sprintf("INSERT INTO 0_cm_auditoria (cedula, fecha, accion, hora, ci_paciente, fecha_cita, numerocita) VALUES (%s, %s, %s, %s, %s, %s, %s)",
						   GetSQLValueString($ciusuario, "text"),
						   GetSQLValueString($fecactual, "text"),
						   GetSQLValueString($accion, "text"),
						   GetSQLValueString($hora, "text"),
						   GetSQLValueString($cedula, "text"),
						   GetSQLValueString($fecha_cita, "date"),
						   GetSQLValueString($numerocita, "text"));
	
	  mysql_select_db($database_conexion2, $conexion2);
	  $Result1 = mysql_query($insertSQL, $conexion2) or die(mysql_error());
}
?>