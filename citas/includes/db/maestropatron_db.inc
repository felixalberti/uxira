<?php
/**********************************************************************
    Copyright (C) FELIX ALBERTI, CA.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
//Begin Felix Alberti 02/07/2010
function add_patron($patron, $descripcion)

{
	$sql = "INSERT INTO ".TB_PREF."cm_patron (patron, descripcion)
		VALUES (".db_escape($patron).",".db_escape($descripcion).")";
	db_query($sql,"El item no pudo ser añadido");
}
function update_patron($patron, $descripcion)

{
	$sql = "UPDATE ".TB_PREF."cm_patron SET descripcion=".db_escape($descripcion)." WHERE patron = ".db_escape($patron);

	db_query($sql,"El item no pudo ser actualizado");
}
function delete_patron($patron)
{
	$sql="DELETE FROM ".TB_PREF."cm_patron WHERE patron = ".db_escape($patron);

	db_query($sql,"El item no pudo ser borrado");
}
//
function get_patron($patron)
{
	$sql="SELECT * FROM ".TB_PREF."cm_patron WHERE patron='$patron'";

	$result = db_query($sql,"El patrón no puedieron ser retornado");

	return db_fetch($result);
}
//End Felix Alberti 02/07/2010
?>