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
//Begin Felix Alberti 27/07/2010
function add_control_sala($fecha_cita, $numero, $hora_cita, $ci_usuario, $name, $name2, $phone,
        $codmed, $tipocita, $codestatus, $observacion, $horallegada, $tecnico=null, $serial_invoice=null, $amount=0)

{
	//$fecha_reg = date("Y/m/d h:i:s");
	//$horaestatus = date("h:i:s");
        $datehoy = new DateTime('now', new DateTimeZone(AMERICA_CARACAS));
        $horaestatus = $datehoy->format('H:i:s');
	$codestatus = "0";
	$fecha_cita = date2sql($fecha_cita);
        $amount = user_numeric($amount);
        if ($amount===null) $amount = 0;
	$sql = "INSERT INTO ".TB_PREF."cm_citas (fecha_cita, fechora_reg, 
	hora_cita, ci_usuario, name, name2, phone, codmed, tipocita, numero, codestatus, horaestatus,
        observacion, horallegada, tecnico, serial_invoice, amount )
		VALUES (".db_escape($fecha_cita).",Now(),".db_escape($hora_cita).",".
                db_escape($ci_usuario).",".db_escape($name).",".db_escape($name2).",".db_escape($phone).",".
		db_escape($codmed).",".db_escape($tipocita).",".db_escape($numero).
                ",".db_escape($codestatus).",".db_escape($horaestatus).",".db_escape($observacion).
                ",".db_escape($horallegada).",".db_escape($tecnico).",".db_escape($serial_invoice).
                ",".db_escape($amount).")";
        //display_notification($sql);
	db_query($sql,"El item no pudo ser anadido");
}
function update_control_sala($fecha_cita, $numero, $tipo_cita, $hora_cita, $codestatus, $observacion, $real_name,
        $last_name, $phone, $tecnico, $serial_invoice, $amount=0)

{
	$fecha_cita = date2sql($fecha_cita);
	$horaestatus = date("h:i:s");
        $amount = user_numeric($amount);
	$sql = "UPDATE ".TB_PREF."cm_citas SET observacion = ".db_escape($observacion).", codestatus=".db_escape($codestatus).",
	horaestatus=".db_escape($horaestatus).", name = ".db_escape($real_name).", 
        name2 = ".db_escape($last_name).", phone = ".db_escape($phone).
        ", tecnico=".db_escape($tecnico).", serial_invoice=".db_escape($serial_invoice).", amount=".db_escape($amount).
        " WHERE fecha_cita = ".db_escape($fecha_cita). " and ".
	" numero = ".db_escape($numero);
        //display_notification($sql);
	db_query($sql,"El item no pudo ser actualizado");
}
function delete_control_sala($fecha_cita, $numero)
{
	$fecha_cita = date2sql($fecha_cita);
	$sql="DELETE FROM ".TB_PREF."cm_citas WHERE fecha_cita = ".db_escape($fecha_cita). " and ".
	"numero = ".db_escape($numero);
	//display_notification(_($sql));

	db_query($sql,"El item no pudo ser borrado ".TB_PREF."cm_citas");
	
	$sql="UPDATE ".TB_PREF."cm_fechasxsemana set tomado = 0 WHERE fecha = ".db_escape($fecha_cita). " and tomado = 1";	
	db_query($sql,"El item no pudo ser borrado ".TB_PREF."cm_fechasxsemana");
}
//
function get_control_sala($fecha_cita, $numero)
{

	$sql="SELECT c.* FROM ".TB_PREF."cm_citas c WHERE fecha_cita = ".db_escape($fecha_cita)." and numero = ".db_escape($numero);

	$result = db_query($sql,"Los datos de las citas no pueden ser retornados");

	return db_fetch($result);
}
//End Felix Alberti 27/07/2010
//
function delete_control_sala_gt($id)
{
	global $conn_sqlsvr;
	$sql = "DELETE FROM Z11_PLANIFICACION WHERE Z11_PLANIFICACION = $id ";
	$result = sqlsrv_query( $conn_sqlsvr, $sql);
	if ( $result === false ) {
		   die( print_r( sqlsrv_errors(), true ));
		   display_notification("Error borrando registro");
		} 	
	}
?>