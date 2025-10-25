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
//Begin Felix Alberti 13/07/2011
function get_servicio_gt($serv){
	global $conn_sqlsvr;
  $sql = "SELECT F13_NOMBRE FROM F13_PLANTILLA WHERE F13_PLANTILLA = $serv";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$nombre = "";
	if ($result){
		$row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
		$nombre = $row['F13_NOMBRE'];
		sqlsrv_free_stmt( $result);
  }
	return $nombre;  
}		
//End Felix Alberti 13/07/2011
//Begin Felix Alberti 22/07/2011
function get_precio_x_serv_gt($serv){
	global $conn_sqlsvr;
  $sql = "SELECT F13_PRECIO FROM F13_PLANTILLA WHERE F13_PLANTILLA = $serv";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$nombre = "";
	if ($result){
		$row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
		$nombre = $row['F13_PRECIO'];
		sqlsrv_free_stmt( $result);
  }
	return $nombre;  
}		
//End Felix Alberti 22/07/2011
?>