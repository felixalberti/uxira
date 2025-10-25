<?php
/**********************************************************************
    Copyright Abril 2016 (C) Felix Alberti.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
function get_data_customer_x_debtor($debtor)
{    
    $sql = "SELECT * FROM ".TB_PREF."debtors_master ".
             "where debtor_no = $debtor";
     //display_notification($sql);
     $result = db_query($sql,"The debtor_no in debtors_master could not be found");
     //$myrow = db_fetch_row($result);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}
//--------------------------------------------------------------------------------------------------
function get_data_status_respons_account($id)
{    
    $sql = "SELECT * FROM ".TB_PREF."key_type_insurance ".
             "where id = $id";
     //display_notification($sql);
     $result = db_query($sql,"The id in key_type_insurance could not be found");
     //$myrow = db_fetch_row($result);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}
function get_data_respons_paym_account($id)
{    
    $sql = "SELECT dm.name FROM ".TB_PREF."respons_payment rp, ".TB_PREF."debtors_master dm".
             " where dm.debtor_no = rp.id_responsible and rp.id = $id";
     //display_notification($sql);
     $result = db_query($sql,"The id in respons_payment could not be found");
     //$myrow = db_fetch_row($result);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}
/*Get first record of cust_branch*/
function get_data_branch_x_debtor($debtor)
{    
    $sql = "SELECT * FROM ".TB_PREF."cust_branch ".
             "where debtor_no = $debtor";
     $result = db_query($sql,"The debtor_no in cust_branch could not be found");
     //display_notification($sql);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}

?>
