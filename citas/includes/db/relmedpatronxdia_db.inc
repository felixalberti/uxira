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
//Begin Felix Alberti 29/06/2010
function add_item_medpatronxdia($codmed, $coddia, $patron)

{
	$sql = "INSERT INTO ".TB_PREF."cm_rel_medpatronxdia (codmed, coddia, patron)
		VALUES (".db_escape($codmed).",".db_escape($coddia).",".db_escape($patron).")";

	db_query($sql,"El item no pudo ser añadido");
}
function update_item_medpatronxdia($codmed, $coddia, $patron)

{
	$sql = "UPDATE ".TB_PREF."cm_rel_medpatronxdia SET patron=".db_escape($patron)." WHERE codmed = ".db_escape($codmed). " and ".
	" coddia = ".db_escape($coddia);

	db_query($sql,"El item no pudo ser actualizado");
}
function delete_item_medpatronxdia($codmed, $coddia)
{
	$sql="DELETE FROM ".TB_PREF."cm_rel_medpatronxdia WHERE codmed = ".db_escape($codmed). " and ".
	" coddia = ".db_escape($coddia);

	db_query($sql,"El item no pudo ser borrado");
}
//End Felix Alberti 28/06/2010
//Begin Felix Alberti 28/06/2010
function get_item_medicopatronxdia($medico,$coddia)
{
	$sql="SELECT * FROM ".TB_PREF."cm_rel_medpatronxdia WHERE codmed='$medico' and coddia='$coddia'";

	$result = db_query($sql,"Los patrones por día y por médico no puedieron ser retornados");

	return db_fetch($result);
}
//End Felix Alberti 29/06/2010
?>