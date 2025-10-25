<?php
/**********************************************************************
    Copyright (C) CADAFE, CA.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
//Begin Felix Alberti 28/06/2010
function add_item_tpcitaxmedico($codmed, $tipo_cita, $precio=0)

{
	$sql = "INSERT INTO ".TB_PREF."cm_tpcita_x_medico (codmed, tipocita, precio)
		VALUES (".db_escape($codmed).",".db_escape($tipo_cita).",".$precio.")";

	db_query($sql,"El item no pudo ser añadido");
}
function update_item_tpcitaxmedico($codmed, $tipo_cita, $precio)

{
	$sql = "UPDATE ".TB_PREF."cm_tpcita_x_medico SET precio=".$precio." WHERE codmed = ".db_escape($codmed). " and ".
	" tipocita = ".db_escape($tipo_cita);

	db_query($sql,"El item no pudo ser actualizado");
}
function delete_item_tpcitaxmedico($codmed, $tipo_cita)
{
	$sql="DELETE FROM ".TB_PREF."cm_tpcita_x_medico WHERE codmed = ".db_escape($codmed). " and ".
	" tipocita = ".db_escape($tipo_cita);

	db_query($sql,"El item no pudo ser borrado");
}
//End Felix Alberti 28/06/2010
//Begin Felix Alberti 28/06/2010
function get_item_tpcitaxmedico($medico,$tipo_cita)
{
	$sql="SELECT * FROM ".TB_PREF."cm_tpcita_x_medico WHERE codmed='$medico' and tipocita='$tipo_cita'";

	$result = db_query($sql,"Los tipos de citas por médico no puedieron ser retornados");

	return db_fetch($result);
}
//End Felix Alberti 28/06/2010
?>