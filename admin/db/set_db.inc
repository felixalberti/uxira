<?php
/**********************************************************************
    Copyright April 2016 (C) Felix Alberti.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/

function add_set($set_id, $cod_dim)
{
	$sql = "INSERT INTO ".TB_PREF."set (set_id, cod_dim)
		VALUES (".db_escape($set_id).", ".db_escape($cod_dim).")";

	return db_query($sql);
}

//--------------------------------------------------------------------------------------

function update_set($id, $set_id, $cod_dim)
{
	$sql = "UPDATE ".TB_PREF."set SET set_id=".db_escape($set_id).", 
                                       cod_dim=".db_escape($cod_dim);
	
	$sql .= " WHERE id = ".db_escape($id);

	return db_query($sql);
}


//--------------------------------------------------------------------------------------

function delete_set($id)
{
	$sql = "DELETE FROM ".TB_PREF."set WHERE id = ".db_escape($id);

	db_query($sql, "could not delete set");
}

//--------------------------------------------------------------------------------------

?>