<?php
/**********************************************************************
    Copyright 2011 (C) Angios, CA.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
//Begin Felix Alberti 11/07/2011
function get_paciente($conn_sqlsvr, $cedula){
  $sql = "SELECT Z19_NOMBRE FROM Z19_PACIENTE WHERE Z19_NROCEDULA = $cedula";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$nombre = "";
	if ($result){
		$rows = sqlsrv_has_rows( $result );
    if ( $rows == true ) {
	     $row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
		   $nombre = $row['Z19_NOMBRE'];
    }
    sqlsrv_free_stmt( $result);
  }	
  return $nombre;
  
}
function get_nomb_paciente_gt($id){
	global $conn_sqlsvr;
  $sql = "SELECT Z19_NOMBRE FROM Z19_PACIENTE WHERE Z19_PACIENTE = $id";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
	$nombre = $row['Z19_NOMBRE'];
	sqlsrv_free_stmt( $result);
	return $nombre;  
}
function get_ci_paciente_gt($id){
	global $conn_sqlsvr;
  $sql = "SELECT Z19_NROCEDULA FROM Z19_PACIENTE WHERE Z19_PACIENTE = $id";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
	$nombre = $row['Z19_NROCEDULA'];
	sqlsrv_free_stmt( $result);
	return $nombre;  
}				
//End Felix Alberti 11/07/2011
?>