<?php
/**********************************************************************
    Copyright 14/12/2015 (C) Uxira C.A.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
function add_responsible($account, $responsible, $number_key, $type_key, $amount, $term_key, $memo, $parent=0, $fe_trans, $fe_recepcion){
    //$amount = str_replace(",", ".", $amount);
    $amount = $amount != '' ? $amount : 0;
    $amount = user_numeric($amount);
    $parent = ($parent=='' || $parent==NULL) ? 0 : $parent;
    $type_key = ($type_key=='' || $type_key==NULL) ? 0 : $type_key;
    $user = $_SESSION["wa_current_user"]->username;
    $fe_recepcion = ($fe_recepcion=='') ? '0000-00-00' : $fe_recepcion;
    $fe_recepcion = ($fe_recepcion=='1969-12-31') ? '0000-00-00' : $fe_recepcion;
    $sql = "INSERT INTO ".TB_PREF."respons_payment "
            . "(id_account,id_parent,id_responsible, fe_trans, number_key, type_key, amount_cobertura, term_key, memo, user, inactive, fe_register, fe_recepcion) VALUES ("
            . db_escape($account).",".db_escape($parent).",".db_escape($responsible).",".db_escape($fe_trans).",".db_escape($number_key).","
            . db_escape($type_key).",".db_escape($amount).",".db_escape($term_key).",".db_escape($memo).",".db_escape($user).", 0, Now(), ".db_escape($fe_recepcion)." )";
    //display_notification($sql);
    db_query($sql,"The respons_payment could not be added");
    return db_insert_id();
}

function update_quotation($account,$quotation){
	$sql = "UPDATE ".TB_PREF."sales_orders SET account_associated = ".
db_escape($account)." WHERE order_no = ".db_escape($quotation)." AND trans_type = ".ST_SALESQUOTE;
db_query($sql,"The sales_orders could not be update");
}


function upd_responsible($account, $responsible){    
    $user = $_SESSION["wa_current_user"]->username;
    //display_notification("upd_responsible ".$user);
    $sql = "UPDATE ".TB_PREF."respons_payment set inactive = 1, user_updt = ".db_escape($user).", fe_updated = Now()"
            . " where id_account = ".db_escape($account)." and id_responsible = ".db_escape($responsible)." and inactive = 0";
    //display_notification($sql);
    $result = db_query($sql,"The respons_payment could not be update");
}

function upd_responsible_x_number_key($account, $number_key, $type_key){    
    $user = $_SESSION["wa_current_user"]->username;
    //display_notification("upd_responsible ".$user);
    $sql = "UPDATE ".TB_PREF."respons_payment set inactive = 1, user_updt = ".db_escape($user)
            . " where id_account = ".db_escape($account)." and number_key = ".db_escape($number_key).
            " and type_key = ".db_escape($type_key)." and inactive = 0";
    //display_notification($sql);
    $result = db_query($sql,"The respons_payment could not be update");
}

function upd_responsible_x_id($id){    
    $user = $_SESSION["wa_current_user"]->username;
    //display_notification("upd_responsible ".$user);
    $sql = "UPDATE ".TB_PREF."respons_payment set inactive = 1, user_updt = ".db_escape($user).", fe_updated = Now()"
            . " where id = ".db_escape($id)." and inactive = 0";
    //display_notification($sql);
    $result = db_query($sql,"The respons_payment could not be update");
}

function borrar_responsible($account, $responsible){    
    $user = $_SESSION["wa_current_user"]->username;
    //display_notification("borrar_responsible ".$user);
    $sql = "UPDATE ".TB_PREF."respons_payment set inactive = 1, user_updt = ".db_escape($user).", fe_updated = Now()"
            . " where id_account = ".db_escape($account)." and id_responsible = ".db_escape($responsible);
    //display_notification($sql);
    $result = db_query($sql,"The respons_payment could not be update");
}

function delete_responsible_x_id($id){    
    $user = $_SESSION["wa_current_user"]->username;
    //display_notification("borrar_responsible ".$user);
    $sql = "UPDATE ".TB_PREF."respons_payment set inactive = 1, user_updt = ".db_escape($user).", fe_updated = Now()"
            . " where id = ".db_escape($id);
    //display_notification($sql);
    $result = db_query($sql,"The respons_payment could not be update");
}

function get_responsible_x_id($id){
    $sql = "Select id, amount_cobertura, number_key, type_key, term_key, memo from ".TB_PREF."respons_payment "
    . " where id = ".db_escape($id);           
    //display_notification($sql);
    $result = db_query($sql,"The id from respons_payment could not be get it");
    return db_fetch_assoc($result);
}

function get_responsible($account,$responsible){
    $sql = "Select id, amount_cobertura, number_key, type_key, term_key, memo from ".TB_PREF."respons_payment "
    . " where inactive = 0 and id_account = ".db_escape($account)." and id_responsible = ".db_escape($responsible);           
    //display_notification($sql);
    $result = db_query($sql,"The id from respons_payment could not be get it");
    return db_fetch_assoc($result);
}

function get_responsible_x_numberkey($account,$responsible,$numberkey){
    $sql = "Select id, amount_cobertura, number_key, type_key, term_key, memo from ".TB_PREF."respons_payment "
    . " where inactive = 0 and id_account = ".db_escape($account)." and id_responsible = ".db_escape($responsible)
    . " and number_key = ".db_escape($numberkey);           
    //display_notification($sql);
    $result = db_query($sql,"The id from respons_payment could not be get it");
    return db_fetch_assoc($result);
}

function get_exist_resp($account,$responsible){
    $sql = "Select count(*) as cant from ".TB_PREF."respons_payment"
    . " where id_account = ".db_escape($account)." and id_responsible = ".db_escape($responsible)." and inactive = 0";
   // display_notification($sql);
    $result = db_query($sql,"The count from respons_payment could not be get it");
    return db_fetch_assoc($result);
    
}

function get_exist_acc_resp_x_key($account,$responsible,$number_key){
    $sql = "Select count(*) as cant from ".TB_PREF."respons_payment"
    . " where id_account = ".db_escape($account)." and number_key = ".db_escape($number_key)." and id_responsible = ".db_escape($responsible); 
   // display_notification($sql);
    $result = db_query($sql,"The count from respons_payment per number key could not be get it");
    return db_fetch_assoc($result);
    
}

function get_exist_resp_x_key($account,$number_key,$typekey){
    $sql = "Select count(*) as cant from ".TB_PREF."respons_payment"
    . " where id_account = ".db_escape($account)." and number_key = ".db_escape($number_key)." and type_key = ".db_escape($typekey); 
   // display_notification($sql);
    $result = db_query($sql,"The count from respons_payment per number key could not be get it");
    return db_fetch_assoc($result);
    
}

function get_exist_resp_x_id($id){
    $sql = "Select count(*) as cant from ".TB_PREF."respons_payment"
    . " where id = ". db_escape($id); 
   // display_notification($sql);
    $result = db_query($sql,"The count from respons_payment per id could not be get it");
    return db_fetch_assoc($result);
    
}

function get_particular($account,$id_responsible){
    $sql = "Select * from ".TB_PREF."payer_trans "
    . " where inactive = 0 and id_account = ".db_escape($account)." and id_responsible = ".db_escape($id_responsible);           
    //display_notification($sql);
    $result = db_query($sql,"The id from respons_payment could not be get it");
    return db_fetch_assoc($result);
}

function get_data_customer($cedula)
{    
    $sql = "SELECT d.name, p.phone, p.phone2, d.debtor_no FROM ".TB_PREF."debtors_master d ".
                            "LEFT JOIN ". TB_PREF."cust_branch b ON (d.debtor_no = b.debtor_no) ".
                            "LEFT JOIN ".TB_PREF."crm_contacts c
                                       ON c.entity_id=b.branch_code AND c.type='cust_branch' AND c.action='general'
                            LEFT JOIN ".TB_PREF."crm_persons p ".
                            "ON c.person_id=p.id ".                 

             "where d.cedula = '$cedula'";
     //display_notification($sql);
     $result = db_query($sql,"The cedula in debtors_master could not be found");
     //$myrow = db_fetch_row($result);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}

/*Begin Felix Alberti 24/04/2016*/
function get_cedula_customer($name)
{    
    $sql = "SELECT d.cedula FROM ".TB_PREF."debtors_master d ".
             "where d.name like '%$name%'";
     //display_notification($sql);
     $result = db_query($sql,"The name in debtors_master could not be found");
     //$myrow = db_fetch_row($result);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}
/*End Felix Alberti 24/04/2016*/

function get_data_responsible($account=0, $id_responsible=null){
    $sql = "SELECT r.id, r.id_responsible, r.fe_trans, r.number_key,"
            . "r.type_key, r.amount_cobertura, r.term_key, r.memo, r.user, r.fe_recepcion FROM ".TB_PREF."respons_payment r ".
             "where r.inactive = 0 and r.id_account = ".db_escape($account);
     if ($id_responsible!=null)
     $sql .= " and r.id_responsible = ".  db_escape($id_responsible);    
     //display_notification($sql);
     $result = db_query($sql,"The account in respons_payment could not be found");
     //$myrow = db_fetch_row($result);
     //$myrow = db_fetch_assoc($result);
     //if ($myrow)
     return $result;
}

function get_patient_account($cedula,$new=false)
{    
     $sql = "SELECT d.name, a.id, a.date_admission, a.date_egress, a.phone, a.phone2, ".       
           "a.tran_date, a.doctor, a.medico_a_cargo, a.order_, a.order_amount, a.status_admision, d.debtor_no as customer_id,"
             . " a.left_orders, a.inactive, a.cama, a.status_exit, a.type_med_serv, a.resp_social FROM ".TB_PREF."debtors_master d ".                                       
                            "LEFT JOIN ".TB_PREF."patient_accounts a ";
     if ($new)
         $sql .=
                            "ON (a.debtor_no=d.cedula) ";
     else
         $sql .=
                            "ON (a.debtor_no=d.cedula) ";
                            
     $sql .=        "where d.cedula = '$cedula'";
     //display_notification('get_patient_account === '.$sql);
     $result = db_query($sql,"The cedula in patient_accounts could not be found");
     //$myrow = db_fetch_row($result);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}

function get_patient_account_x_account($account)
{    
     $sql = "SELECT d.name, a.id, a.date_admission, a.date_egress, a.phone, a.phone2, ".             
           "a.tran_date, a.doctor, a.medico_a_cargo, a.order_, a.order_amount, a.status_admision, d.debtor_no as customer_id,"
             . " a.left_orders, a.inactive, a.cama, a.status_exit, a.type_med_serv, a.debtor_no, d.cedula, a.resp_social FROM ".TB_PREF."debtors_master d ".                                       
                            "LEFT JOIN ".TB_PREF."patient_accounts a ".
                            "ON (a.debtor_no=d.cedula) ".
             "where a.id = '$account'";
    
     $result = db_query($sql,"The account in patient_accounts could not be found");
     //display_notification($sql);
     //$myrow = db_fetch_row($result);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}

function get_account_x_id_responsible($id_responsible)
{        
     $sql = "SELECT pt.id_account, pc.difference FROM ".TB_PREF."payer_trans pt, ".TB_PREF."patient_accounts pc ".
             "where pt.pending = 1 and pt.id_responsible = $id_responsible and pt.id_account = pc.id";
     //display_notification($sql);
     $result = db_query($sql,"The id_responsible in patient_accounts could not be found");
     //$myrow = db_fetch_row($result);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}

function add_patient_account($id,$customer_id,$phone,$phone2,$doctor,$doctor_resp,$order,$order_amount,
        $left_orders, $cama, $type_med_serv){
    //display_notification('function add_patient_account '.$order.' - '.$order_amount);
    $SQLDate = date2sql(Today());  
    $user = $_SESSION["wa_current_user"]->username;
    $sql = "INSERT INTO ".TB_PREF."patient_accounts 
                    (id, debtor_no, tran_date, phone, phone2, doctor, medico_a_cargo, order_, order_amount, user,
                     left_orders, cama, type_med_serv )
                    VALUES (".db_escape($id).", ".db_escape($customer_id). ",'".$SQLDate."',".db_escape($phone).
                    ",".db_escape($phone2).",".db_escape($doctor).",".db_escape($doctor_resp).
                    ",".db_escape($order).",".db_escape($order_amount).",".db_escape($user).
                    ",".db_escape($left_orders).",".db_escape($cama).",".db_escape($type_med_serv)."  )"; 
        db_query($sql, "patient_accounts could not be inserted");
        //display_notification($sql);
    //return db_insert_id();
}

function update_status_patient_account($id_account, $status_admision){
    $user = $_SESSION["wa_current_user"]->username;
    //display_notification("upd_responsible ".$user);
    $sql = "UPDATE ".TB_PREF."patient_accounts set status_admision = ".db_escape($status_admision).", user_admision = ".db_escape($user)
            . " where id = ".db_escape($id_account);
    //display_notification($sql);
    $result = db_query($sql,"The status_admision from patient_accounts could not be update");
    
}

function update_exit_patient_account($id_account, $status_admision){
    $user = $_SESSION["wa_current_user"]->username;
    //display_notification("upd_responsible ".$user);
    $sql = "UPDATE ".TB_PREF."patient_accounts set status_exit = ".db_escape($status_admision).", user_exit = ".db_escape($user)
            . " where id = ".db_escape($id_account);
    //display_notification($sql);
    $result = db_query($sql,"The status_exit from patient_accounts could not be update");
    
}

function update_patient_account_all($account,$date_admission,$phone,$phone2,$doctor_prim,
        $medico_tratante,$left_orders,$order_amount,$cama,$date_egress, $type_med_serv, $leyenda_corte_cta=null, $angios_resp_social=0){
    $user = $_SESSION["wa_current_user"]->username;
    
    if ($date_admission=='00/00/0000' || $date_admission=='00-00-0000')
    $date_in='NULL';
    else
    $date_in = db_escape(date2sql($date_admission));
    //    
    //$date_exit = date2sql($date_egress);
    if ($date_egress=='00/00/0000' || $date_egress=='00-00-0000')
    $date_exit='NULL';
    else
    $date_exit = db_escape(date2sql($date_egress));
    //
    //$order_amount = user_numeric($order_amount);
    $sql_select = "SELECT * FROM ".TB_PREF."patient_accounts pa "
             . " where id = ".db_escape($account);
  
    $result = db_query($sql_select,"patient_accounts could not be found");
    $myrow = db_fetch_assoc($result);
    if ($myrow['inactive']>0)
    $sql = "UPDATE ".TB_PREF."patient_accounts set "
            . "date_admission = ".$date_in.", "
            . "phone = ".db_escape($phone).", phone2 = ".db_escape($phone2).", "
            . "doctor = ".db_escape($medico_tratante).", medico_a_cargo = ".db_escape($doctor_prim).", "            
            . "left_orders = ".db_escape($left_orders).", "
            . "order_amount = ".db_escape($order_amount).", "
            . "cama = ".db_escape($cama).", "
            . "date_egress = ".$date_exit.", "
            . "type_med_serv = ".db_escape($type_med_serv).", "
            . "resp_social = ".db_escape($angios_resp_social)
            . " where id = ".db_escape($account);
    else
        $sql = "UPDATE ".TB_PREF."patient_accounts set "
            . "date_admission = ".$date_in.", "
            . "phone = ".db_escape($phone).", phone2 = ".db_escape($phone2).", "
            . "doctor = ".db_escape($medico_tratante).", medico_a_cargo = ".db_escape($doctor_prim).", "            
            . "left_orders = ".db_escape($left_orders).", "
            . "order_amount = ".db_escape($order_amount).", "
            . "cama = ".db_escape($cama).", "
            . "date_egress = ".$date_exit.", "
            . "type_med_serv = ".db_escape($type_med_serv).", "
            . "resp_social = ".db_escape($angios_resp_social)
            . " where id = ".db_escape($account);
    
    //display_notification($sql);
    $result = db_query($sql,"The patient_accounts table could not be update");
    
    /*Habilitar para cuando se vayan a insertar comentarios*/
    /*$sql = "select count(*) as cant from 0_comments where id=$account and type = ".ST_ACCOUNTPATIENT;
    $result = db_query($sql,"The comments table could not be retrieve");
    $myrow = db_fetch_assoc($result);
    if ($myrow['cant'] == 0)
       $sql = "insert into 0_comments values (".ST_ACCOUNTPATIENT.",$account,now(),'$leyenda_corte_cta')";
    else
        $sql = "update 0_comments set memo_ = '$leyenda_corte_cta' where id=$account and type = ".ST_ACCOUNTPATIENT;
    $result = db_query($sql,"The comments table could not be update/insert");*/
    
}

//--------------------------------------------------------------------------------------------------

function last_order($cedula, $date_after, $date_before, $last_order=null ){
    $selected_customer = $_POST['cedulapaciente'];
    $trans_type = ST_SALESQUOTE;
    
    //display_notification($OrdersAfterDate.' - '.$OrdersToDate);
    
    //$date_after = date2sql($OrdersAfterDate);
    //$date_before = date2sql($OrdersToDate);
    
    if (!isset($max_no_order) || (isset($max_no_order) && $max_no_order == 0)) {
        $sql = "SELECT 			
                                max(order_no) AS MaxOrderno
                        FROM ".TB_PREF."sales_orders as sorder,"
                                .TB_PREF."debtors_master as debtor ".
                                "WHERE  ".
                                " sorder.trans_type = ".db_escape($trans_type)."
                                AND sorder.debtor_no = debtor.debtor_no";	

        if ($last_order==null && $date_after!='' && $date_before!='')//Si no selecciono ultimo presupuesto
        $sql .=  " AND sorder.ord_date >= '$date_after'"." AND sorder.ord_date <= '$date_before'";

        if ($cedula != -1)
        $sql .= " AND debtor.cedula=".db_escape($cedula);
        //display_notification($sql);

        $result = db_query($sql,"The MaxOrderno in sales_orders could not be found (last_order)");
        $myrow =    db_fetch_row($result);
        $MaxOrdernoBD = "";
        if ($myrow[0]){
           // display_notification($myrow[0]);
            $MaxOrdernoBD = $myrow[0];
            return $MaxOrdernoBD;
        }
        else
        return null;    
    }
}

function get_data_order($cedula,$max_no_order){
     //display_notification('max order: '.$max_no_order);
     $trans_type = ST_SALESQUOTE; 
     $sql = "SELECT 			
                                total AS OrderTotal,
                                payer AS Responsable_pago,
                                sorder.order_no AS order_no,
                                sorder.doctor,
                                sorder.medico_tratante,
                                sorder.type_medical_serv,
                                sorder.order_type,
                                sorder.ex_rates
                        FROM ".TB_PREF."sales_orders as sorder,"
                                .TB_PREF."debtors_master as debtor ".
                                "WHERE sorder.order_no = ".db_escape($max_no_order).
                                "AND sorder.trans_type = ".db_escape($trans_type)."
                                AND sorder.debtor_no = debtor.debtor_no";

        if ($cedula != -1)
        $sql .= " AND debtor.cedula=".db_escape($cedula);				
        
        //display_notification($sql);
        $result = db_query($sql,"The cedula in debtors_master could not be found (get_data_order)");
        $myrow = db_fetch_assoc($result);
        if ($myrow)
        return $myrow;
        else
        return null;  
    
}

function get_type_medical_serv($id){
        $sql = "SELECT * FROM ".TB_PREF."type_medical_service WHERE id=$id";
        $result = db_query($sql,"The id in type_medical_serv could not be found");
        $myrow = db_fetch_assoc($result);
        if ($myrow)
        return $myrow;
        else
        return null;  
}

function add_payer_trans($account, $responsible, $amount, $paid, $balance, $id_parent=0){
    //display_notification("add_payer_trans ".$account.' - '.$amount);
    //$amount = user_numeric($amount);
    //$balance = user_numeric($balance);
    $user = $_SESSION["wa_current_user"]->username;
    $sql = "INSERT INTO ".TB_PREF."payer_trans "
            . "(id_account,id_parent, id_responsible, fe_trans, amount, paid, balance, user, pending) VALUES ("
            . db_escape($account).",".db_escape($id_parent).",". db_escape($responsible).",Now(),"
            . $amount.",".  db_escape($paid).",".  db_escape($balance).",".db_escape($user).", 1 )";
    //display_notification('$sql '.$sql);
    db_query($sql,"The payer trans could not be added");
}

function add_payer_trans_detail($account, $responsible, $amount, $balance, $discount=0, $trans_no_rel, $type_rel){
    //display_notification("add_payer_trans ".$account.' - '.$trans_no_rel.' - '.$type_rel);   
    $user = $_SESSION["wa_current_user"]->username;
    $sql = "INSERT INTO ".TB_PREF."payer_trans_detail "
            . "(id_account, id_responsible, fe_trans, amount, balance, discount, trans_no_rel, type_rel, user) VALUES ("
            . db_escape($account).",". db_escape($responsible).",Now(),"
            . $amount.",".$balance.",".$discount.", ". db_escape($trans_no_rel).",". 
            db_escape($type_rel).",". db_escape($user)." )";
    //display_notification("add_payer_trans ".$sql); 
    db_query($sql,"The payer trans detail could not be added");
}

function update_payer_trans($account, $id_responsible, $amount=0, $balance=0, $trans_no=0){
    //display_notification('update_payer_trans '.$id_responsible.' - '.$amount);
    $pending = ($balance == 0) ? 0 : 1;
    $sql = "UPDATE ".TB_PREF."payer_trans set paid = paid + ".$amount. ", balance = ".$balance." , pending = $pending "
            . "WHERE id_account = ".db_escape($account)." and id_responsible = $id_responsible and inactive = 0";
    //display_notification("update_payer_trans ".$sql); 
    db_query($sql,"The amount field in payer trans could not be update");   
    
}


function get_data_payer_transac($account){
     $sql = "SELECT pt.id, pt.id_responsible, pt.id_account, pt.fe_trans, "
            . "pt.amount, pt.user, pt.paid, pt.balance, pt.inactive FROM ".TB_PREF."payer_trans pt ".
             "where pt.inactive = 0 and pt.id_account = ".db_escape($account);
     //display_notification($sql);
     $result = db_query($sql,"The account in the payer_trans table could not be found");

     return $result;
    
}

function get_igtf($account){
$tot_credit_tmp = 0;
$sql = "select sum(mp.amount) as amount from 0_debtor_trans dt, 0_cust_allocations ca, 0_debtor_trans_payment_method mp, 0_metodos_pago m where
 dt.type = 12 and 
dt.ov_amount > 0 and ca.trans_no_from = dt.trans_no and ca.trans_type_from = 12 AND ca.trans_no_to = ".db_escape($account)."
  and  mp.trans_no = dt.trans_no and mp.trans_type = 12 and m.description like '%IGTF%' and mp.type_method_pay = m.id";		
      //display_notification($sql);
	   $result = db_query($sql, "The transactions custcredit from customers could not be retrieved");
        while ($myrow = db_fetch($result)) { 
                $tot_credit_tmp += $myrow['amount'];
		}	
		return $tot_credit_tmp;
}		

function get_data_payer_paid($account){
    $sql = "SELECT sum(round((ov_amount+ov_discount),2)) as paid FROM ".TB_PREF."payer_trans_detail pd, ".TB_PREF."debtor_trans dt"
            . " where pd.id_account = ".db_escape($account)." and pd.inactive = 0 and pd.type_rel in (".ST_CUSTPAYMENT.",".ST_BANKDEPOSIT.")"
            . " and dt.trans_no = pd.trans_no_rel and dt.type = pd.type_rel and dt.ov_amount > 0";
    //display_notification($sql);
    $result = db_query($sql,"The account in the payer_trans_detail table could not be found");

    return db_fetch_assoc($result);
}


function get_payer_transac_compromiso($account='',$id_responsible=null){
     $sql = "SELECT pt.amount, pt.paid, pt.balance FROM ".TB_PREF."payer_trans pt ".
             "where pt.inactive = 0 and pt.pending = 1 ". 
             " and id_responsible = ".db_escape($id_responsible).
             " and (pt.paid < pt.amount)";
     if ($account!='')
     $sql .= " and pt.id_account = ".db_escape($account);
     //display_notification('get_payer_transac_compromiso '.$sql);
     $result = db_query($sql,"The account and id_responsible in the payer_trans table could not be found");

     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null; 
    
}

function get_payer_transac_compromiso_cobranza($account='',$id_responsible=null){
     $sql = "SELECT pt.amount, pt.paid, pt.balance FROM ".TB_PREF."payer_trans pt ".
             "where pt.inactive = 0 ". 
             " and id_responsible = ".db_escape($id_responsible);
     if ($account!='')
     $sql .= " and pt.id_account = ".db_escape($account);
     //display_notification('get_payer_transac_compromiso '.$sql);
     $result = db_query($sql,"The account and id_responsible in the payer_trans table could not be found");

     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null; 
    
}

function get_payer_transac_compromiso_sin_reg($account='',$id_responsible=null){
     $sql = "SELECT pa.order_amount, (pa.particular_amount) as paid, (pa.order_amount - "
             . " (pa.respons_amount + pa.particular_amount))  as balance, pa.id as account FROM ".TB_PREF."patient_accounts pa, ".
             TB_PREF."debtors_master dm ".
             //"where pa.date_egress is null and pa.inactive = 0 ". 
			 "where pa.inactive <> -1 ". 
             " and dm.debtor_no = ".db_escape($id_responsible).
             " and pa.debtor_no = dm.tax_id ".
             " and ((pa.respons_amount + pa.particular_amount < pa.order_amount) or ".
             " (pa.respons_amount + pa.particular_amount < pa.account_amount))";
     if ($account!='')
     $sql .= " and pa.id = ".db_escape($account);
     //display_notification('get_payer_transac_compromiso_sin_reg '.$sql);
     $result = db_query($sql,"The account and id_responsible in the patient_accounts table could not be found");

     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null; 
    
}

function get_account_data_amount($account='',$id_responsible=null){
     $sql = "SELECT pa.order_amount, (pa.particular_amount) as paid, (pa.order_amount - "
             . " (pa.respons_amount + pa.particular_amount)) as balance_order,"
             . "(pa.account_amount - "
             . " (pa.respons_amount + pa.particular_amount)) as balance_account FROM ".TB_PREF."patient_accounts pa, ".
             TB_PREF."debtors_master dm ".
             "where ". 
             " dm.debtor_no = ".db_escape($id_responsible).
             " and pa.debtor_no = dm.tax_id ";
     if ($account!='')
     $sql .= " and pa.id = ".db_escape($account);
     //display_notification('get_payer_transac_compromiso_sin_reg '.$sql);
     $result = db_query($sql,"The account and id_responsible in the patient_accounts table could not be found");

     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null; 
    
}

function update_patient_account($cedula,$account,$account_amount=0,$respons_amount=0,$particular_amount=0)
{     
    //$respons_amount = user_numeric($respons_amount);
    //$particular_amount = user_numeric($particular_amount);
     $sql = "UPDATE ".TB_PREF."patient_accounts p  set p.account_amount = ".$account_amount.
             ", respons_amount = ".$respons_amount.", particular_amount = ".$particular_amount.
             " where p.inactive = 0 and p.debtor_no = ".db_escape($cedula)." and id = ".$account;
     //display_notification($sql);
     $result = db_query($sql,"The cedula in patient_accounts could not be found for update");
}

function update_patient_account_order($cedula,$order_,$account)
{     
      $trans_type = ST_SALESQUOTE;
      $sql = "SELECT total, sorder.medico_tratante, sorder.doctor
                        FROM ".TB_PREF."sales_orders as sorder,"
                                .TB_PREF."debtors_master as debtor ".
                                "WHERE  ".
                                " sorder.trans_type = ".db_escape($trans_type)."
                                AND sorder.debtor_no = debtor.debtor_no";	

     $sql .= " AND debtor.cedula=".db_escape($cedula);
     $sql .= " AND sorder.order_no =".db_escape($order_);
     
     $result = db_query($sql,"The order could not be found");
     $myrow = db_fetch_assoc($result);   
    
     $amount = $myrow['total'];
     $doctor = $myrow['doctor'];
     $medico_tratante = $myrow['medico_tratante'];
     //
     $sql = "SELECT sum(amount_cobertura) as cobertura FROM ".TB_PREF."respons_payment rp, "
                                .TB_PREF."patient_accounts pa ".
                                "WHERE  ".
                                " rp.id_account = pa.id".
                                " AND pa.debtor_no = ".  db_escape($cedula).
                                " AND pa.inactive = 0".
                                " AND pa.id = ".db_escape($account);
     $result = db_query($sql,"The responsible could not be found");
     $myrow = db_fetch_assoc($result);
     $cobertura_respon = $myrow['cobertura'];
     //
     $sql = "SELECT sum(pt.amount) as cobertura FROM ".TB_PREF."payer_trans pt, "
                                .TB_PREF."patient_accounts pa ".
                                "WHERE  ".
                                " pt.id_account = pa.id".
                                " AND pa.debtor_no = ".  db_escape($cedula).
                                " AND pa.inactive = 0".
                                " AND pa.id = ".db_escape($account);
     $result = db_query($sql,"The particular could not be found");
     $myrow = db_fetch_assoc($result);
     $cobertura_part = $myrow['cobertura'];
     //
     //$dif = $amount - ($cobertura_respon + $cobertura_part);
     //$paid = $amount - $dif;
    $sql = "SELECT order_ as last_order FROM ".TB_PREF."patient_accounts p WHERE ".
    "p.debtor_no = ".db_escape($cedula)." and p.id = ".db_escape($account);
    $result = db_query($sql,"The last_order could not be retrieved");
    $myrow_account = db_fetch_assoc($result);
    $last_order = $myrow_account['last_order'];
     
     $sql = "UPDATE ".TB_PREF."patient_accounts p set p.order_ = ".db_escape($order_).", p.order_amount = ".$amount.
             ", p.doctor = ".db_escape($doctor).", p.medico_a_cargo = ".db_escape($medico_tratante).
             " where p.inactive = 0 and p.debtor_no = ".db_escape($cedula)." and p.id = ".db_escape($account);
     //display_notification($sql);
     $result = db_query($sql,"The cedula in patient_accounts could not be found for update the order");
     
     $sql = "UPDATE ".TB_PREF."sales_orders as sorder SET sorder.account_associated = ".db_escape($account)
             . " WHERE sorder.trans_type = ".db_escape($trans_type);
     $sql .= " AND sorder.order_no =".db_escape($order_);
     $result = db_query($sql,"The account_associated in sales_orders could not be found for update");
     //
      $sql = "UPDATE ".TB_PREF."sales_orders as sorder SET sorder.account_associated = 0"
             . " WHERE sorder.trans_type = ".db_escape($trans_type);
     $sql .= " AND sorder.order_no =".db_escape($last_order);
     $result = db_query($sql,"The account_associated in sales_orders (last_order) could not be found for update");
     
}

function update_patient_account_x_pay($id,$amount)
{
    
    
     $sql = "UPDATE ".TB_PREF."patient_accounts p set p.particular_amount = p.particular_amount + ".$amount.
             " where id = ".$id;
     //display_notification($sql);
     $result = db_query($sql,"The id in patient_accounts could not be found for update");
}

function update_particular($id_account, $persona){
    //$amount = str_replace(",", "", $amount);
    //$dif = str_replace(",", "", $dif);
    $user = $_SESSION["wa_current_user"]->username;
    $sql = "UPDATE ".TB_PREF."payer_trans pt set pt.user_update = ".db_escape($user).", pt.inactive = 1".
             " where pt.inactive = 0 and pt.id_account = ".db_escape($id_account)." and pt.id_responsible = ".db_escape($persona);
    //display_notification($sql);
    $result = db_query($sql,"The cedula in payer_trans could not be found for update");
    
}

/*Begin Felix Alberti 25/04/2016*/
function get_debtor_customer($cedula)
{    
    $sql = "SELECT d.debtor_no FROM ".TB_PREF."debtors_master d ".
             "where d.cedula = ".db_escape($cedula);
     //display_notification($sql);
     $result = db_query($sql,"The cedula in debtors_master could not be found");
     //$myrow = db_fetch_row($result);
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}
/*End Felix Alberti 25/04/2016*/

/*Begin Felix Alberti 01/05/2016*/
function get_bank_deposits_from_customer($cust){
    $type = ST_BANKDEPOSIT;
    $sql = "SELECT * FROM ".TB_PREF."debtor_trans "
            . "where type = $type and debtor_no = $cust ORDER BY tran_date DESC";
    
    $result = db_query($sql,"Finding bank deposits, The debtor_no in debtors_master could not be found");
}
/*End Felix Alberti 01/05/2016*/

function add_bitacora_patient_account($account,$order_old,$amount_order_old,$order_new,$amount_order_new){
    $user = $_SESSION["wa_current_user"]->username;
    //$trans_date = fecha_hoy(AMERICA_CARACAS);
    $amount_order_old = user_numeric($amount_order_old);
    $amount_order_new = user_numeric($amount_order_new);
    $sql = "INSERT INTO ".TB_PREF."patient_accounts_history "
            . "(id_account, order_old, amount_order_old, order_new, amount_order_new,  user, trans_date) VALUES ("
            . db_escape($account).",".db_escape($order_old).",".db_escape($amount_order_old).",".
             db_escape($order_new).",".db_escape($amount_order_new).",". db_escape($user).",Now()"." )";
    db_query($sql,"The payer patient_accounts_history could not be added");
}

function get_cta_compromiso_particular($id_customer){
    $sql = "SELECT pt.id_account, d.debtor_no FROM ".
                TB_PREF."payer_trans pt, ".TB_PREF."patient_accounts pc, ".TB_PREF."debtors_master d".
             " where pt.id_responsible = ".db_escape($id_customer)." and pt.inactive = 0".
                " and pt.id_account = pc.id and d.cedula = pc.debtor_no";
    //display_notification($sql);
    $result = db_query($sql,"The cta_compromiso_particular could not be added");
    $myrow = db_fetch_assoc($result);
    if ($myrow)
    return $myrow;
    else
    return null;    
}

function get_authoriz_patient_high($account){
    $sql = "SELECT account_amount"
            . " FROM ".TB_PREF."patient_accounts_request_authoriz"
            . " WHERE account = ".db_escape($account)." and user_authoriz is not null and"
            . " date_user_auth != '0000-00-00 00:00:00' and inactive = 0 and used = 0 order by account_amount desc";
    //display_notification('get_authoriz_patient_high: '.$sql);
    $result = db_query($sql,"The authorization could not be retrieved");
    return db_fetch_assoc($result);
}
function get_authoriz_patient_high_exist($account){
    $sql = "SELECT account_amount"
            . " FROM ".TB_PREF."patient_accounts_request_authoriz"
            . " WHERE account = ".db_escape($account)." and user_authoriz is null and"
            . " date_user_auth is null and inactive = 0 and used = 0";
//    display_notification('get_authoriz_patient_high: '.$sql);
    $result = db_query($sql,"The authorization could not be retrieved");
    return db_fetch_assoc($result);
}
/*Begin Felix Alberti 28/11/2016*/
function get_responsible_x_invoice($account,$cust){
    $sql = "Select payer FROM ".TB_PREF."debtor_trans "
    . " WHERE type = ".ST_SALESINVOICE." and account_associated = ".db_escape($account)." and debtor_no = ".db_escape($cust);           
    //display_notification($sql);
    $result = db_query($sql,"The id from respons_payment could not be get it");
    return db_fetch_assoc($result);
}

function update_responsible_x_invoice($account,$cust,$payers){
    $sql = "UPDATE ".TB_PREF."debtor_trans set payer = ".db_escape($payers)
    . " WHERE type = ".ST_SALESINVOICE." and account_associated = ".db_escape($account)." and debtor_no = ".db_escape($cust);           
    //display_notification($sql);
    db_query($sql,"The debtor_trans could not be updated");
}
/*End Felix Alberti 28/11/2016*/
/*Begin Felix Alberti 15/01/2017*/
function get_trans_from_invoice($account){
     $sql = "Select trans_no FROM ".TB_PREF."debtor_trans "
    . " WHERE type = ".ST_SALESINVOICE." and account_associated = ".db_escape($account);           
    //display_notification($sql);
    $result = db_query($sql,"The trans_no could not be get it");
    return db_fetch_assoc($result);    
}
/*End Felix Alberti 15/01/2017*/

/*Begin Felix Alberti 30/09/2019*/
function get_trans_from_invoice_active($account){
    if ($account==0 || $account==NULL) return false;
     $sql = "Select serial, tran_date, ov_amount, curr_code FROM ".TB_PREF."debtor_trans "
    . " WHERE type = ".ST_SALESINVOICE." and account_associated = ".db_escape($account)." and ov_amount > 0";           
    //display_error($sql);
    $result = db_query($sql,"The trans_no could not be get it");
    return $result;    
}
/*End Felix Alberti 30/09/2019*/

/*Begin Felix Alberti 27/04/2017*/
//Función que verifica si una solicitud de autorización fue aprobada por un usuario autorizado
function get_authoriz_uncheck_orders($account){
    $chk = 1;
    //
    $sql = "SELECT count(*) as cant"
            . " FROM ".TB_PREF."debtor_trans_request_authoriz"
            . " WHERE trans_no = ".db_escape($account)." and user_authoriz is not null and"
            . " date_user_auth is not null and inactive = 0 and used = 0 and uncheck_orders = ".$chk;
//    display_notification('get_authoriz_patient_high: '.$sql);
    $result = db_query($sql,"The authorization could not be retrieved");    
    $row = db_fetch_assoc($result);
    if ($row['cant'] > 0)
    return true;    
    else
    return false;
}
/*End Felix Alberti 27/04/2017*/
/*Begin Felix Alberti 22/04/2019*/
function get_kit_onlyquotation($item){
     $sql = "Select count(*) as cant FROM ".TB_PREF."item_codes "
    . " WHERE item_code = '{$item}' and only_quotation=1";
    $result = db_query($sql,"The only_quotation could not be get it");
    $row = db_fetch_assoc($result);
    if ($row['cant'] > 0)
    return true;    
    else
    return false;    
}
/*End Felix Alberti 22/04/2019*/


function is_it_kit($item){
   $sql = "SELECT count(*) as cant  
		FROM ".TB_PREF."item_codes 
		WHERE ".
                TB_PREF."item_codes.item_code = ".db_escape($item).
                " and ".TB_PREF."item_codes.stock_id != ".db_escape($item);
   $res = db_query($sql,"the count could not be retreived");
   $myrow = db_fetch($res);
   return $myrow['cant'];
}

function get_specialties_x_medico($medico){
     $sql = "SELECT specialties_id  
		FROM ".TB_PREF."professional 
		WHERE ".
                TB_PREF."professional.professional_id = ".db_escape($medico);
   $res = db_query($sql,"the count could not be retreived");
   $myrow = db_fetch($res);
   return $myrow['specialties_id'];
}

function get_data_x_prof($prof){
     $sql = "SELECT *  
		FROM ".TB_PREF."professional 
		WHERE ".
                TB_PREF."professional.professional_id = ".db_escape($prof);
       // display_error($sql);
   $res = db_query($sql,"the count could not be retreived");
   $myrow = db_fetch($res);
   return $myrow;
}

function get_kit_x_specialties($item){
     $sql = "SELECT *  
		FROM ".TB_PREF."item_codes 
		WHERE ".
                TB_PREF."item_codes.item_code = ".db_escape($item)." limit 1";
        //display_warning($sql);
   $res = db_query($sql,"the count could not be retreived");
   $myrow = db_fetch($res);
   return $myrow;
}

function get_sum_respons_paym($bank_tran){
    $sql = "select IFNULL(sum(ov_amount),0) as amount from ".TB_PREF."debtor_trans_respons_paym"
         . " where type = ".ST_RESPONSPAYMENT." and bank_tran = ".db_escape($bank_tran);
    $res = db_query($sql,"the count could not be retrieved");
    $myrow = db_fetch($res);
    return $myrow['amount'];
}

function getCobranzaFactParticular($invoice,$account){
    if ($account == 0 || $account == NULL)
    $sql = "SELECT sum(ca.amt) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt".
                   " where ( ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice).") AND
    ca.trans_type_from in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.trans_no = ca.trans_no_from and dt.type = ca.trans_type_from and dt.ov_amount > 0";        
    else    
    $sql = "SELECT sum(dt.ov_amount) as amount FROM ".TB_PREF."debtor_trans dt".
    " where ".
    " dt.type in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.ov_amount > 0 and dt.account_associated = ". db_escape($account); 
    
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
  
    //if ($account=='2017090021') display_notification ($sql);
    
    $myrow = db_fetch($result);
    //if ($account=='2017110040') display_notification (' amount '.' - '.$myrow['amount']);
    return $myrow['amount'];
}

function getCobranzaFactSeguro($invoice){
    $sql = "SELECT sum(dtr.ov_amount + std.dscto_amount + std.tax_amount) as amount FROM ".TB_PREF."debtor_trans dt, ".
            TB_PREF."debtor_trans_respons_paym dtr, ".TB_PREF."bank_trans bt, ".TB_PREF."settlement_trans_detail std
where dt.type = ".ST_SALESINVOICE. " and dt.trans_no = ".db_escape($invoice)." and dtr.account_associated = dt.account_associated"
     ." and dtr.type = ".ST_RESPONSPAYMENT
     ." and bt.type = ".ST_BANKDEPOSIT." and bt.trans_no = dtr.bank_tran and bt.person_type_id = ".PT_RESP_PAYMENT." and dt.ov_amount > 0"
     ." and std.payment_rel = dtr.trans_no and std.type = ".ST_SETTLEMENT; 
    $result = db_query($sql, "The data of debtor_trans_respons_paym record could not be retrieved");   
    //if ($account=='2017090021') display_notification ($sql);
    $myrow = db_fetch($result);
    //if ($account=='2017010005') display_notification ($person_id.' - '.$myrow['amount']);
    return $myrow['amount'];
}

function IsCreditPayment($selected_id)
{
	$sql = "SELECT count(*) as cant FROM ".TB_PREF."payment_terms";
	$sql .= " WHERE days_before_due!=0 OR day_in_following_month!=0 and terms_indicator=$selected_id";
      
        $result = db_query($sql, "The data of payment_terms record could not be retrieved");   
    
        $myrow = db_fetch($result);
    
        if ($myrow['cant']>0)
        return true;    
        else
        return false;
}

//FAP 16/10/2019
function get_count_abono_seguro($account){
    $sql = "select count(*) as cant from ".TB_PREF."debtor_trans_respons_paym dt, ".TB_PREF."debtor_trans dt2,
                ".TB_PREF."debtors_master d, ".TB_PREF."bank_trans bt
                where dt2.type = ".ST_SALESINVOICE. " and dt.account_associated = ".db_escape($account)." and
		dt.type = ".ST_RESPONSPAYMENT." and d.debtor_no = bt.person_id and
		dt.ov_amount > 0 and
		dt2.account_associated = ".db_escape($account)." and bt.type = ".ST_BANKDEPOSIT." and
                bt.trans_no = dt.bank_tran and bt.person_type_id = 7"; 
    return db_query($sql, "The count transactions payment responsible from customers could not be retrieved");
}
function get_abono_seguro($account){
    $sql = "select distinct d.name AS cust_name, bt.trans_no, bt.person_id, dt.tran_date, dt.ov_amount,
                dt.user, dt.date_register, bt.date_trans_bank from ".TB_PREF."debtor_trans_respons_paym dt, ".TB_PREF."debtor_trans dt2,
                ".TB_PREF."debtors_master d, ".TB_PREF."bank_trans bt
                where dt2.type = ".ST_SALESINVOICE. " and dt.account_associated = ".db_escape($account)." and
		dt.type = ".ST_RESPONSPAYMENT." and d.debtor_no = bt.person_id and
		dt.ov_amount > 0 and
		dt2.account_associated = ".db_escape($account)." and bt.type = ".ST_BANKDEPOSIT." and
                bt.trans_no = dt.bank_tran and bt.person_type_id = 7";             
    //display_notification($sql);
    return db_query($sql, "The transactions payment responsible from customers could not be retrieved");
}
//FAP 16/10/2019

?>