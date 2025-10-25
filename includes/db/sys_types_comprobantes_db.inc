<?php
/**********************************************************************
    Copyright 12/12/2022 (C) Felix Alberti.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
function save_new_secuence_comprobante_iva($type, $year, $month, $acc)
{
    $sql = "INSERT INTO ".TB_PREF."sys_types_comprobantes_iva(type_id, year_, month_, acc) VALUES (".
            db_escape($type).",".db_escape($year).",".db_escape($month).",".db_escape($acc).")" ;

	db_query($sql, "The transaction acc for $type could not be inserted");
}

//--------------------------------------------------------------------------------------------------
function save_new_secuence_comprobante_islr($type, $year, $month, $acc)
{
    $sql = "INSERT INTO ".TB_PREF."sys_types_comprobantes_islr(type_id, year_, month_, acc) VALUES (".
            db_escape($type).",".db_escape($year).",".db_escape($month).",".db_escape($acc).")" ;

	db_query($sql, "The transaction acc for $type could not be inserted");
}

function save_next_comprobante_iva($type,$year,$month,$reference)
{
    $sql = "UPDATE ".TB_PREF."sys_types_comprobantes_iva SET acc=" . db_escape(trim($reference)) 
		. " WHERE type_id = ".db_escape($type);
    $sql .= " and year_ = ".  db_escape($year)." and month_ = ".  db_escape($month);

	db_query($sql, "The next transaction acc for $type could not be updated");
}

function save_next_comprobante_islr($type,$year,$month,$reference)
{
    $sql = "UPDATE ".TB_PREF."sys_types_comprobantes_islr SET acc=" . db_escape(trim($reference)) 
		. " WHERE type_id = ".db_escape($type);
    $sql .= " and year_ = ".  db_escape($year)." and month_ = ".  db_escape($month);

	db_query($sql, "The next transaction acc for $type could not be updated");
}

//--------------------------------------------------------------------------------------------------

function get_next_comprobante_iva($type,$year,$month)
{
    $sql = "SELECT acc FROM ".TB_PREF."sys_types_comprobantes_iva WHERE type_id = ".db_escape($type);
    $sql .= " and year_ = ".  db_escape($year)." and month_ = ".  db_escape($month);

    $result = db_query($sql,"The last transaction acc for $type could not be retreived");
    //display_notification($sql);
    if (db_num_rows($result) > 0){
    $row = db_fetch_row($result);
    //display_notification('ret: '.$row[0]);
    return $row[0];
    }
    else return false;
}

function get_next_comprobante_islr($type,$year,$month)
{
    $sql = "SELECT acc FROM ".TB_PREF."sys_types_comprobantes_islr WHERE type_id = ".db_escape($type);
    $sql .= " and year_ = ".  db_escape($year)." and month_ = ".  db_escape($month);

    $result = db_query($sql,"The last transaction acc for $type could not be retreived");
    //display_notification($sql);
    if (db_num_rows($result) > 0){
    $row = db_fetch_row($result);
    //display_notification('ret: '.$row[0]);
    return $row[0];
    }
    else return false;
}

?>