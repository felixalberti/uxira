<?php
/**********************************************************************
    Copyright (C) September 2019 Uxira, C.A.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
function getCobertura($account=null){
    $sql = "SELECT sum(rp.amount_cobertura) as amount_cobertura FROM ".TB_PREF."respons_payment rp ".
                   " where rp.id_account = ".db_escape($account)." and inactive = 0";
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
 //display_notification($sql);
    $myrow = db_fetch($result);
    
    return $myrow['amount_cobertura'];
}

function getAnticipo($invoice,$account,$invoicedate){
    if ($account == 0 || $account == NULL) 
    $sql = "SELECT sum(ca.amt) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt".
                   " where ( ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice).") AND
    ca.trans_type_from in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.tran_date <=  ".db_escape($invoicedate)." and dt.trans_no = ca.trans_no_from and dt.type = ca.trans_type_from and dt.ov_amount > 0";         
    else        
    $sql = "SELECT sum(dt.ov_amount) as amount FROM ".TB_PREF."debtor_trans dt".
    " where ".
    " dt.type in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.tran_date <= ".db_escape($invoicedate)." and dt.ov_amount > 0 and dt.account_associated = ". db_escape($account); 
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
  
    //if ($account=='2019060174') display_notification ($sql);
    
    $myrow = db_fetch($result);
    //if ($account=='2019060174') display_notification ('Monto: '.$myrow['amount']);
    return $myrow['amount'];
}

function getCobranza($invoice,$account,$invoicedate,$to){    
    $todate = date2sql($to);
    if ($account == 0 || $account == NULL) 
    $sql = "SELECT IFNULL(sum(ca.amt),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt".
                   " where ( ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice).") AND
    ca.trans_type_from in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.tran_date > ".db_escape($invoicedate)." and dt.tran_date <= ".$todate." and dt.trans_no = ca.trans_no_from and dt.type = ca.trans_type_from and dt.ov_amount > 0";        
    else    
    $sql = "SELECT IFNULL(sum(dt.ov_amount),0) as amount FROM ".TB_PREF."debtor_trans dt".
    " where ".
    " dt.type in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
     " and dt.tran_date > ".db_escape($invoicedate)." and dt.tran_date <= ".db_escape($todate)." and dt.ov_amount > 0 and dt.account_associated = ". db_escape($account); 
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
  
    //if ($account=='2019060174') display_notification ($sql);
    
    $myrow = db_fetch($result);
    //if ($account=='2019060174') display_notification ('Monto: '.$myrow['amount']);
    return $myrow['amount'];
}

function getCobranza_repfactura($invoice,$account,$to){    
    $todate = date2sql($to);
    if ($account == 0 || $account == NULL) 
    $sql = "SELECT IFNULL(sum(IF(dt2.curr_code='US',ca.amt/dt2.rate_bs,ca.amt)),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt, ".
            TB_PREF."debtor_trans dt2".
                   " where ( ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice).") AND
    ca.trans_type_from in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.tran_date < ".$todate." and dt.trans_no = ca.trans_no_from and dt.type = ca.trans_type_from and dt.ov_amount > 0".
    " and dt2.`type` = ".ST_SALESINVOICE." and dt2.trans_no = ".db_escape($invoice)." and dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";        
    else    
    $sql = "SELECT IFNULL(sum(IF(dt.curr_code='US',dt.ov_amount/dt2.rate_bs,dt.ov_amount)),0) as amount FROM ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans dt2".
    " where ".
    " dt.type in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.tran_date < ".db_escape($todate)." and dt.ov_amount > 0 and dt.account_associated = ". db_escape($account).
    " and dt2.`type` = ".ST_SALESINVOICE." and
    dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
  
    //if ($account=='2019060174') display_notification ($sql);
    
    $myrow = db_fetch($result);
    //if ($account=='2019060174') display_notification ('Monto: '.$myrow['amount']);
    return $myrow['amount'];
}

function getCobranza_repcobranza($invoice,$account,$from,$to){
    $fromdate = date2sql($from);        
    $todate = date2sql($to);
    if ($account == 0 || $account == NULL) 
    $sql = "SELECT IFNULL(sum(IF(dt2.curr_code='US',ca.amt/dt2.rate_bs,ca.amt)),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans dt2".
                   " where ( ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice).") AND
    ca.trans_type_from in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.tran_date >= ".db_escape($fromdate)." and dt.tran_date <= ".$todate." and dt.trans_no = ca.trans_no_from and dt.type = ca.trans_type_from and dt.ov_amount > 0".
    " and dt2.`type` = ".ST_SALESINVOICE." and dt2.trans_no = ".db_escape($invoice)." and dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";
    else    
    $sql = "SELECT IFNULL(sum(IF(dt.curr_code='US',dt.ov_amount/dt2.rate_bs,dt.ov_amount)),0) as amount FROM ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans dt2".
    " where ".
    " dt.type in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.tran_date >= ".db_escape($fromdate)." and dt.tran_date <= ".db_escape($todate)." and dt.ov_amount > 0 and dt.account_associated = ". db_escape($account).
    " and dt2.`type` = ".ST_SALESINVOICE." and
    dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
  
    //if ($account=='2020100072') display_notification ($sql);
    
    //if ($account=='2019060110' || $account=='2019050168') display_notification ($sql);
    
    $myrow = db_fetch($result);
    //if ($account=='2019060110' || $account=='2019050168') display_notification ('Monto: '.$myrow['amount']);
    return $myrow['amount'];
}

function getCobranza_repcobranza_sin_fecha($invoice,$account){
    
    if ($account == 0 || $account == NULL) 
    $sql = "SELECT IFNULL(sum(IF(dt2.curr_code='US',ca.amt/dt2.rate_bs,ca.amt)),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans dt2".
                   " where ( ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice).") AND
    ca.trans_type_from in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.trans_no = ca.trans_no_from and dt.type = ca.trans_type_from and dt.ov_amount > 0".
    " and dt2.`type` = ".ST_SALESINVOICE." and dt2.trans_no = ".db_escape($invoice)." and dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";
    else    
    $sql = "SELECT IFNULL(sum(IF(dt.curr_code='US',dt.ov_amount/dt2.rate_bs,dt.ov_amount)),0) as amount FROM ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans dt2".
    " where ".
    " dt.type in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.ov_amount > 0 and dt.account_associated = ". db_escape($account).
    " and dt2.`type` = ".ST_SALESINVOICE." and
    dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
  
    //if ($account=='2020100072') display_notification ($sql);
    
    //if ($account=='2019060110' || $account=='2019050168') display_notification ($sql);
    
    $myrow = db_fetch($result);
    //if ($account=='2019060110' || $account=='2019050168') display_notification ('Monto: '.$myrow['amount']);
    return $myrow['amount'];
}

function getCobranza_cobranza_todate($invoice,$account,$to){
    $todate = date2sql($to);
    if ($account == 0 || $account == NULL) 
    $sql = "SELECT IFNULL(sum(IF(dt2.curr_code='US',ca.amt/dt2.rate_bs,ca.amt)),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans dt2".
                   " where ( ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice).") AND
    ca.trans_type_from in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.tran_date <= ".$todate." and dt.trans_no = ca.trans_no_from and dt.type = ca.trans_type_from and dt.ov_amount > 0".
    " and dt2.`type` = ".ST_SALESINVOICE." and dt2.trans_no = ".db_escape($invoice)." and dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";
    else    
    $sql = "SELECT IFNULL(sum(IF(dt.curr_code='US',dt.ov_amount/dt2.rate_bs,dt.ov_amount)),0) as amount FROM ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans dt2".
    " where ".
    " dt.type in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and dt.tran_date <= ".db_escape($todate)." and dt.ov_amount > 0 and dt.account_associated = ". db_escape($account).
    " and dt2.`type` = ".ST_SALESINVOICE." and
    dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
  
    //if ($account=='2020100072') display_notification ($sql);
    
    //if ($account=='2019060110' || $account=='2019050168') display_notification ($sql);
    
    $myrow = db_fetch($result);
    //if ($account=='2019060110' || $account=='2019050168') display_notification ('Monto: '.$myrow['amount']);
    return $myrow['amount'];
}

function getCobranzaAfter($invoice,$account,$to){
    $todate = date2sql($to);
    if ($account == 0)
    $sql = "SELECT IFNULL(sum(IF(dt2.curr_code='US',ca.amt/dt2.rate_bs,ca.amt)),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans dt2".
                   " where ( ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice).") AND
    ca.trans_type_from in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and ca.date_alloc > ".db_escape($todate)." and dt.trans_no = ca.trans_no_from and dt.type = ca.trans_type_from and dt.ov_amount > 0".
    " and dt2.`type` = ".ST_SALESINVOICE." and dt2.trans_no = ".db_escape($invoice)." and dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";
    else    
    $sql = "SELECT IFNULL(sum(IF(dt.curr_code='US',dt.ov_amount/dt2.rate_bs,dt.ov_amount)),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans dt2".
                   " where ( ca.trans_type_to = ".ST_PATIENT_ACCOUNT." and ca.trans_no_to = ".db_escape($account).") AND
    ca.trans_type_from in ( ".ST_BANKDEPOSIT.",".ST_CUSTPAYMENT.")".
    " and ca.date_alloc > ".db_escape($todate)." and dt.trans_no = ca.trans_no_from and dt.type = ca.trans_type_from and dt.ov_amount > 0".
    " and dt2.`type` = ".ST_SALESINVOICE." and dt2.account_associated = dt.account_associated and dt2.ov_amount > 0";
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
  
    $myrow = db_fetch($result);
    //if ($account=='2019060174') display_notification ($sql);
    //if ($account == '2017110036') display_notification ('getCobranzaAfter - '.$myrow['amount']);    
    
    return $myrow['amount'];
}

function getCobranzaSeguro($invoice,$account,$from,$to,$person_id){
    //$fromdate = date2sql($from);
    if ($account==0) return 0;
    $todate = date2sql($to);
    /*$sql = "SELECT IFNULL(sum(dt.ov_amount + std.dscto_amount + std.tax_amount),0) as amount
        FROM ".TB_PREF."debtor_trans_respons_paym dt, ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt2, "
            .TB_PREF."settlement_trans_detail std
         WHERE dt.cutoff_date >= ".db_escape($from)." and dt.cutoff_date <= ".db_escape($todate)." and 
         dt.type in ( ".ST_RESPONSPAYMENT. " ) and dt.ov_amount > 0 and
         ca.trans_no_from = dt.trans_no and ca.trans_type_from = dt.type and
          ca.trans_no_to = ".db_escape($invoice)." and ca.trans_type_to = ".ST_SALESINVOICE." and
           dt2.trans_no = ca.trans_no_to and dt2.type = ca.trans_type_to and   
	 dt.account_associated = ". db_escape($account)." and dt2.account_associated = ".db_escape($account)." 
         and std.payment_rel = dt.trans_no and std.type = ".ST_SETTLEMENT;*/
    
    $sql = "SELECT IFNULL(sum(dt.ov_amount + std.dscto_amount + std.tax_amount),0) as amount FROM
        ".TB_PREF."debtor_trans_respons_paym dt, ".TB_PREF."settlement_trans_detail std 
        WHERE 
        ((dt.cutoff_date is not null and dt.cutoff_date >= ".db_escape($from)." and dt.cutoff_date <= ".db_escape($todate).") or "
          ." (dt.cutoff_date is null and dt.date_register >= ".db_escape($from)." and dt.date_register <= ".db_escape($todate).")) and " 
         ." dt.type = ".ST_RESPONSPAYMENT." and
          dt.ov_amount > 0 and 
	 dt.account_associated = ". db_escape($account)." and
	  std.payment_rel = dt.trans_no and
	   std.type = ".ST_SETTLEMENT;
    
    
    $result = db_query($sql, "The data of debtor_trans_respons_paym record could not be retrieved");   
    //if ($account=='2019010002') 
    //display_notification ($person_id.' - '.$sql);
    //if ($account=='2019070088') display_notification ($sql);
    $myrow = db_fetch($result);
    //if ($account=='2019070088' ) display_notification ('amount = '.$myrow['amount']);
    return $myrow['amount'];
}

function getCobranzaSeguro_repfactura($invoice,$account,$to,$person_id){
    //$fromdate = date2sql($from);
    if ($account==0) return 0;
    $todate = date2sql($to);
    $sql = "SELECT IFNULL(sum(dt.ov_amount + std.dscto_amount + std.tax_amount),0) as amount
        FROM ".TB_PREF."debtor_trans_respons_paym dt, ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt2, "
            .TB_PREF."settlement_trans_detail std
         WHERE dt.cutoff_date < ".db_escape($todate)." and 
         dt.type in ( ".ST_RESPONSPAYMENT. " ) and dt.ov_amount > 0 and
         ca.trans_no_from = dt.trans_no and ca.trans_type_from = dt.type and
          ca.trans_no_to = ".db_escape($invoice)." and ca.trans_type_to = ".ST_SALESINVOICE." and
           dt2.trans_no = ca.trans_no_to and dt2.type = ca.trans_type_to and   
	 dt.account_associated = ". db_escape($account)." and dt2.account_associated = ".db_escape($account)." 
         and std.payment_rel = dt.trans_no and std.type = ".ST_SETTLEMENT;
    
    
    $result = db_query($sql, "The data of debtor_trans_respons_paym record could not be retrieved");   
    //if ($account=='B0066262') 
    //display_error ($person_id.' - '.$sql);
    $myrow = db_fetch($result);
    //if ($account=='2019060110' || $account=='2019050168') display_notification ($sql);
    return $myrow['amount'];
}

function getCobranzaSeguro_todate($invoice,$account,$to){
    //$fromdate = date2sql($from);
    if ($account==0) return 0;
    $todate = date2sql($to);
    
    $sql = "SELECT IFNULL(sum(dtr.ov_amount + std.dscto_amount + std.tax_amount),0) as amount
            FROM ".TB_PREF."debtor_trans_respons_paym dtr,
		 ".TB_PREF."bank_trans bt,
		 ".TB_PREF."settlement_trans_detail std 
            WHERE (bt.type = 2 or bt.type = 12 or bt.`type` = 11) and
                  bt.trans_no = dtr.bank_tran and
                  (( bt.date_trans_bank <= '$todate') or
                  (bt.date_trans_bank is null and dtr.tran_date <= '$todate') )
                  and dtr.`type` = ".ST_RESPONSPAYMENT." 
                  and std.payment_rel = dtr.trans_no
                  and std.type = ".ST_SETTLEMENT."                  
                  and dtr.account_associated = ". db_escape($account)."
                  and dtr.trans_no_invoice = $invoice and bt.person_type_id = 7 
                  group by dtr.trans_no_invoice";
    
    $result = db_query($sql, "The data of debtor_trans_respons_paym record could not be retrieved");   
    //if ($account=='2019020004') 
    //display_error ($sql);
    //if ($account=='2019070088') display_notification ($sql);
    $myrow = db_fetch($result);
    //if ($account=='2019070088' ) display_notification ('amount = '.$myrow['amount']);
    return $myrow['amount'];
}

function getNotaCreditoManoAlzada($invoice,$item,$cod_med,$account,$to){
    if ($account == 0) return 0;
    $to = date2sql($to);
    $sql = "SELECT count(*) as cant FROM ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans_details detail".
                   " where "
            . " detail.debtor_trans_no = dt.trans_no and detail.debtor_trans_type = dt.type "
            . " and detail.debtor_trans_no = ".db_escape($invoice)." and detail.debtor_trans_type = ".ST_SALESINVOICE
            . " and detail.stock_id != ".db_escape($item)." and detail.medico_no != ".db_escape($cod_med)." and quantity > 0"
            . " and dt.tran_date <= ".db_escape($to)
            . " and detail.level = 2 ";
    $result = db_query($sql, "The count of debtor_trans_details record could not be retrieved"); 
    $myrow = db_fetch($result);
    
    if ($myrow['cant']==0){
    $sql = "SELECT sum(amount) as amount FROM ( ";
    $sql.=  "SELECT DISTINCT * from (SELECT DISTINCT IFNULL(sum((unit_price+unit_tax)*quantity*(1 - discount_percent)),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans_details detail".
                   " where ca.trans_type_from = ".ST_CUSTCREDIT ." and ca.trans_no_from = dt.trans_no "
            . " and  ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice)
            . " and dt.type = ca.trans_type_from and detail.debtor_trans_no = dt.trans_no and detail.debtor_trans_type = dt.type"
            . " and detail.stock_id != ".db_escape($item)." and detail.medico_no != ".db_escape($cod_med)." and quantity > 0"
            . " and dt.tran_date <= ".db_escape($to)
            . " and detail.level = 2"
            . " and NOT EXISTS (SELECT * FROM ".TB_PREF."cust_allocations ca2 "
            . "WHERE ca2.trans_type_from = ".ST_CUSTCREDIT." and ca2.trans_no_from = dt.trans_no and"
            . " ca2.trans_type_to = ".ST_PATIENT_ACCOUNT." and ca2.trans_no_to = ".$account." ) "
            ." ) a";
    $sql .= " UNION ALL ";    
    $sql .= "Select DISTINCT * from (SELECT DISTINCT IFNULL(sum((unit_price+unit_tax)*quantity*(1 - discount_percent)),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans_details detail".
                   " where ca.trans_type_from = ".ST_CUSTCREDIT ." and ca.trans_no_from = dt.trans_no and dt.account_associated = ".db_escape($account)
            . " and  ca.trans_type_to = ".ST_PATIENT_ACCOUNT." and ca.trans_no_to = ".db_escape($account)
            . " and detail.stock_id != ".db_escape($item)." and detail.medico_no != ".db_escape($cod_med)
            . " and dt.type = ca.trans_type_from and detail.debtor_trans_no = dt.trans_no and detail.debtor_trans_type = dt.type and quantity > 0 "
            . " and dt.tran_date <= ".db_escape($to)
            . " and detail.level = 2"
            . " and NOT EXISTS (SELECT * FROM ".TB_PREF."cust_allocations ca2 "
            . "WHERE ca2.trans_type_from = ".ST_CUSTCREDIT." and ca2.trans_no_from = dt.trans_no and"
            . " ca2.trans_type_to = ".ST_SALESINVOICE." and ca2.trans_no_to = ".db_escape($invoice)." ) "
            . ") b"
            . ") t1";    
    
    
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
    //if ($account == '2017110013' ) display_notification($invoice.' - '.$sql);
    $myrow = db_fetch($result);
    
    return $myrow['amount'];
    }
    else {
      return 0;
    }
}

function getNotaCredito($invoice,$item,$cod_med,$to){
    $to = date2sql($to);
    $sql = "SELECT DISTINCT IFNULL(sum((unit_price+unit_tax)*quantity*(1 - discount_percent)),0) as amount FROM  ".TB_PREF."cust_allocations ca, ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans_details detail".
                   " where ca.trans_type_from = ".ST_CUSTCREDIT ." and ca.trans_no_from = dt.trans_no "
            . " and  ca.trans_type_to = ".ST_SALESINVOICE." and ca.trans_no_to = ".db_escape($invoice)
            . " and dt.type = ca.trans_type_from and detail.debtor_trans_no = dt.trans_no and detail.debtor_trans_type = dt.type"
            . " and detail.stock_id = ".db_escape($item)." and detail.medico_no = ".db_escape($cod_med)." and quantity > 0"
            . " and dt.tran_date <= ".db_escape($to)
            . " and detail.level = 2";
    
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
    //if ($account == 2017110013 ) display_notification($sql);
    //if ($invoice == 20876 || $invoice == 20877) display_notification($sql);
    //display_notification($sql);
    $myrow = db_fetch($result);
    
    return $myrow['amount'];
}

function getTotFac($invoice,$type){
    $sql = "SELECT sum(quantity * (IF(dt.curr_code='US',(unit_price/dt.rate)+(unit_tax/dt.rate),unit_price+unit_tax))) as amount FROM ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans_details detail".
                   " where dt.trans_no = ".db_escape($invoice)
            . " and dt.type = ".db_escape($type)." and detail.debtor_trans_no = dt.trans_no and"
            . " detail.debtor_trans_type = dt.type and detail.level = 2";
    $result = db_query($sql, "The data of payer_trans_detail record could not be retrieved"); 
    
    $myrow = db_fetch($result);
    
    return $myrow['amount'];
}

function getTotDebito($account,$item,$cod_med,$debitdate){ 
    if ($account == 0) return 0;    
    $debitdate = date2sql($debitdate);
    $sql = "SELECT sum(quantity * (unit_price+unit_tax)) as amount FROM ".TB_PREF."debtor_trans dt, ".TB_PREF."debtor_trans_details detail".
                   " where dt.account_associated = ".db_escape($account)
            . " and dt.type = ".ST_CUSTDEBIT." and detail.debtor_trans_no = dt.trans_no and detail.debtor_trans_type = dt.type"
            . " and detail.stock_id = ".db_escape($item)." and detail.medico_no = ".db_escape($cod_med)    
            . " and dt.tran_date <= ".db_escape($debitdate)." and quantity > 0 and detail.level = 2";    
    $result = db_query($sql, "The data of debito record could not be retrieved"); 
    
    $myrow = db_fetch($result);
    //if ($account == 2017100043 ) display_notification($item.' - '.$myrow['amount']);
    return $myrow['amount'];
}

?>