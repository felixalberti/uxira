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
//Begin Felix Alberti 09/06/2009
function add_item_tablas($codigotabla, $codigotipo, $descripcion, $comentario, 
                         $vermas, $titulo_vermas, $intervalo_tiempo, $abr, $servicio_gt)

{
	$sql = "INSERT INTO ".TB_PREF."cm_system_tables (codigotabla, codigotipo, 
	descripcion, comentario, vermas,
	titulo_vermas, intervalo_tiempo, abr, servicio_gt )
		VALUES (".db_escape($codigotabla).",".db_escape($codigotipo).",".db_escape($descripcion).",".
		db_escape($comentario).",".db_escape($vermas).",".db_escape($titulo_vermas).",".$intervalo_tiempo.",".db_escape($abr).
		",".db_escape($servicio_gt).")";

	db_query($sql,"El item no pudo ser a�adido");
}
function update_item_tablas($Item, $descripcion='', $comentario='', 
                         $vermas='', $titulo_vermas='', $intervalo_tiempo=0, $abr='')

{
	$sql = "UPDATE ".TB_PREF."cm_system_tables SET descripcion=".db_escape($descripcion).",
	comentario=".db_escape($comentario).", vermas=".db_escape($vermas).", titulo_vermas=".db_escape($titulo_vermas).", intervalo_tiempo=".
	db_escape($intervalo_tiempo).",	abr=".db_escape($abr)." WHERE id = $Item";

	db_query($sql,"El item no pudo ser actualizado");
}
function delete_item_tablas($Item)
{
	$sql="DELETE FROM ".TB_PREF."cm_system_tables WHERE id='$Item'";

	db_query($sql,"El item no pudo ser borrado");
}
function get_item_tablas($id)
{
	$sql="SELECT * FROM ".TB_PREF."cm_system_tables WHERE id='$id'";

	$result = db_query($sql,"La tabla no puede ser retornada");

	return db_fetch($result);
}
//End Felix Alberti 09/06/2009
?>