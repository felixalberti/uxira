<?php
/**********************************************************************
    Copyright (C) FELIX ALBERTI, C.A.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
//Begin Felix Alberti 24/07/2010
function get_medico($medico_no)
{
	$sql = "SELECT professional_id, description FROM ".TB_PREF."professional where professional_id = $medico_no";
	$result = db_query($sql,"El nombe del medico no puedo ser retornado");  
	$row = db_fetch($result);
	return $row['description'];	
}
//End Felix Alberti 24/07/2010
//Begin Felix Alberti 13/07/2011
function get_medico_gt($medico_no)
{
	global $conn_sqlsvr;
	$sql = "SELECT Z14_NOMBRE FROM Z14_PERSONALMEDICO WHERE Z14_PERSONALMEDICO = $medico_no";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
	$nombre = $row['Z14_NOMBRE'];
	sqlsrv_free_stmt( $result);
	return $nombre;  	
}
//End Felix Alberti 13/07/2011
//Begin Felix Alberti 22/07/2011
function get_medico_datos_gt($medico_no)
{
	global $conn_sqlsvr;
	$sql = "SELECT * FROM Z14_PERSONALMEDICO WHERE Z14_PERSONALMEDICO = $medico_no";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	$row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC);
	$nombre = $row['Z14_NOMBRE'];
	sqlsrv_free_stmt( $result);
	return $nombre;  	
}
//End Felix Alberti 22/07/2011
?>