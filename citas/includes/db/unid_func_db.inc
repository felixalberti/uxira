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
function add_unifun($codunifunc, $descripcion)

{
	$sql = "INSERT INTO ".TB_PREF."cm_unidad_func (codunifun, descripcion)
		VALUES (".db_escape($codunifunc).",".db_escape($descripcion).")";
	db_query($sql,"El item no pudo ser añadido");
}
function update_unifun($codunifunc, $descripcion)

{
	$sql = "UPDATE ".TB_PREF."cm_unidad_func SET descripcion=".db_escape($descripcion)." WHERE codunifun = ".db_escape($codunifunc);

	db_query($sql,"El item no pudo ser actualizado");
}
function delete_unifun($codunifunc)
{
	$sql="DELETE FROM ".TB_PREF."cm_unidad_func WHERE codunifun = ".db_escape($codunifunc);

	db_query($sql,"El item no pudo ser borrado");
}
//
function get_unifun($codunifunc)
{
	$sql="SELECT * FROM ".TB_PREF."cm_unidad_func WHERE codunifun='$codunifunc'";

	$result = db_query($sql,"La unidad funcional no pudo ser retornada");

	return db_fetch($result);
}
//End Felix Alberti 24/07/2010
?>