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
function add_especialidades($conn_sqlsvr, $codigo)

{
  $nombre = get_nombre_especialidad($conn_sqlsvr, $codigo);
	$sql = "INSERT INTO ".TB_PREF."cm_especialidad (codigo, nombre)
		VALUES (".db_escape($codigo).",".db_escape($nombre).")";

	db_query($sql,"La especialidad no pudo ser añadida");
}
function update_especialidades($conn_sqlsvr, $item, $codigo)

{
	$nombre = get_nombre_especialidad($conn_sqlsvr, $codigo);
	$sql = "UPDATE ".TB_PREF."cm_especialidad SET codigo=".db_escape($codigo).",
	nombre=".db_escape($nombre)." WHERE id = $item";

	db_query($sql,"La especialidad no pudo ser actualizada");
}
function delete_especialidad($item)
{
	$sql="DELETE FROM ".TB_PREF."cm_especialidad WHERE id='$item'";

	db_query($sql,"La especialidad no pudo ser borrada");
}
function get_especialidad($id)
{
	$sql="SELECT * FROM ".TB_PREF."cm_especialidad WHERE id='$id'";

	$result = db_query($sql,"La especialidad no puede ser retornada");

	return db_fetch($result);
}
function get_nombre_especialidad($conn_sqlsvr, $codigo){
  $sql = "SELECT Z13_NOMBRE FROM Z13_ESPECIALIDAD WHERE Z13_ESPECIALIDAD = $codigo";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$nombre = "";
	if ($result){
		$rows = sqlsrv_has_rows( $result );
    if ( $rows == true ) {
	     $row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
		   $nombre = $row['Z13_NOMBRE'];
    }
    sqlsrv_free_stmt( $result);
  }	
  return $nombre;
  
}
function get_especialidad_gt($id){
	global $conn_sqlsvr;
  $sql = "SELECT Z13_NOMBRE FROM Z13_ESPECIALIDAD WHERE Z13_ESPECIALIDAD = $id";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
	$nombre = $row['Z13_NOMBRE'];
	sqlsrv_free_stmt( $result);
	return $nombre;  
}		
//End Felix Alberti 11/07/2011
?>