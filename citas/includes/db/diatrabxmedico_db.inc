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
function add_item_diatrabxmed($codmed, $coddia, $enuso)

{
	$sql = "INSERT INTO ".TB_PREF."cm_diatrabxmedico (codmed, coddia, enuso)
		VALUES (".db_escape($codmed).",".db_escape($coddia).",".$enuso.")";
  //display_notification($sql);
	db_query($sql,"El item no pudo ser añadido");
}
function update_item_diatrabxmed($codmed, $coddia, $enuso)

{
	$sql = "UPDATE ".TB_PREF."cm_diatrabxmedico SET enuso=".db_escape($enuso)." WHERE codmed = ".db_escape($codmed). " and ".
	" coddia = ".db_escape($coddia);

	db_query($sql,"El item no pudo ser actualizado");
}
function delete_item_diatrabxmed($codmed, $coddia)
{
	$sql="DELETE FROM ".TB_PREF."cm_diatrabxmedico WHERE codmed = ".db_escape($codmed). " and ".
	" coddia = ".db_escape($coddia);

	db_query($sql,"El item no pudo ser borrado");
}
//
function get_item_diatrabxmed($medico,$coddia)
{
	$sql="SELECT * FROM ".TB_PREF."cm_diatrabxmedico WHERE codmed='$medico' and coddia='$coddia'";

	$result = db_query($sql,"Los días de trabajo por médico no puedieron ser retornados");

	return db_fetch($result);
}
//End Felix Alberti 02/07/2010
?>