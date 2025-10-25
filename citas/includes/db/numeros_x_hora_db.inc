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
function add_item_numeros($patron, $tipo_cita, $numero, $hora_cita, $clinica, $codunidfunc)

{
	$sql = "INSERT INTO ".TB_PREF."cm_numeroscita (patron, tipo_cita, 
	numero, hora_cita, clinica, codunidfunc )
		VALUES (".db_escape($patron).",".db_escape($tipo_cita).",".db_escape($numero).",".db_escape($hora_cita).",".
		db_escape($clinica).",".db_escape($codunidfunc).")";
  //display_notification($sql);
	db_query($sql,"El item no pudo ser añadido");
}
function update_item_numeros($select_id, $patron, $tipo_cita, $numero, $hora_cita, $clinica, $codunidfunc)

{
	$sql = "UPDATE ".TB_PREF."cm_numeroscita SET patron = ".db_escape($patron).", tipo_cita = ".db_escape($tipo_cita).
	", numero=".db_escape($numero).", hora_cita=".db_escape($hora_cita).",
	clinica=".db_escape($clinica).", codunidfunc=".db_escape($codunidfunc)." WHERE id = $select_id";
  //display_notification(_($sql));
	db_query($sql,"El item no pudo ser actualizado");
}
function delete_item_numeros($select_id)
{
	$sql="DELETE FROM ".TB_PREF."cm_numeroscita WHERE id = $select_id";
	//display_notification(_($sql));

	db_query($sql,"El item no pudo ser borrado");
}
//End Felix Alberti 28/06/2010
//Begin Felix Alberti 28/06/2010
function get_item_numeros_hora($patron,$tipo_cita,$numero)
{
	$sql="SELECT * FROM ".TB_PREF."cm_numeroscita WHERE patron='$patron' and tipo_cita='$tipo_cita' and numero='$numero'";

	$result = db_query($sql,"Los números x hora no puedieron ser retornados");

	return db_fetch($result);
}
//End Felix Alberti 28/06/2010
//Begin Felix Alberti 28/06/2010
function get_id_numeros_hora($selectid)
{
	$sql="SELECT * FROM ".TB_PREF."cm_numeroscita WHERE id=$selectid";

	$result = db_query($sql,"Los números x hora no puedieron ser retornados");
	return db_fetch($result);
}
//End Felix Alberti 28/06/2010
?>