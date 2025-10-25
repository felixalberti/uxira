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
function add_clinica($codclini, $descripcion)

{
	$sql = "INSERT INTO ".TB_PREF."cm_clinica (codclini, descripcion)
		VALUES (".db_escape($codclini).",".db_escape($descripcion).")";
	db_query($sql,"El item no pudo ser añadido");
}
function update_clinica($codclini, $descripcion)

{
	$sql = "UPDATE ".TB_PREF."cm_clinica SET descripcion=".db_escape($descripcion)." WHERE codclini = ".db_escape($codclini);

	db_query($sql,"El item no pudo ser actualizado");
}
function delete_clinica($codclini)
{
	$sql="DELETE FROM ".TB_PREF."cm_clinica WHERE codclini = ".db_escape($codclini);

	db_query($sql,"El item no pudo ser borrado");
}
//
function get_clinica($codclini)
{
	$sql="SELECT * FROM ".TB_PREF."cm_clinica WHERE codclini='$codclini'";

	$result = db_query($sql,"El clinica no pudo ser retornada");

	return db_fetch($result);
}
//End Felix Alberti 24/07/2010
?>