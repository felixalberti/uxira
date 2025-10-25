<?php
/**********************************************************************
    Copyright 07/12/2016 (C) Uxira C.A.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
/*Begin Felix Alberti 07/12/2016*/
function get_next_term_key($type_key){
    $sql = "Select id_type_insurance FROM ".TB_PREF."key_type_insurance_imp_format"
    . " WHERE id_next = ".db_escape($type_key);           
    //display_notification($sql);
    $result = db_query($sql,"The next type_term could not be get it");
    $type_key_array = array();
    while ($row = db_fetch($result)){
      $type_key_array[] = $row['id_type_insurance']; 
    }
    return $type_key_array;
}
/*End Felix Alberti 07/12/2016*/

/*Begin Felix Alberti 09/12/2016*/
function get_data_type_insurance($type_key){
    $sql = "Select rep_name FROM ".TB_PREF."key_type_insurance"
    . " WHERE id = ".db_escape($type_key);           
    //display_notification($sql);
    $result = db_query($sql,"The key_type_insurance could not be get it");
    $type_key_array = array();
    $row = db_fetch($result);
    return $row['rep_name'];
}
/*End Felix Alberti 09/12/2016*/
?>
