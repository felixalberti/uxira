<?php
/**********************************************************************
    Copyright 2017 (C) Uxira C.A
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
/*
	Class for supplier/customer payment/credit allocations edition
	and related helpers.
*/
//-----------------------------------------------------------------------------------

class allocation 
{

	var $trans_no; 
	var $type;
	var $person_id = '';
	var $person_name = '';
	var $person_type;	// PT_SUPPLIER/PT_CUSTOMER
	var $person_curr;
	var $date_;
	var $amount = 0; /*Total amount of the transaction in FX */
	var $currency;
        var $account = 0;

	var $allocs; /*array of transactions allocated to */
        
        var $line_payment_methods;  //array of objects of class metodos_pago
        var $show_invoice;
        
        
	function allocation($type, $trans_no, $person_id = null, $person_type_id=null, $account=false, $show_invoice=0)
	{
                //display_notification('constructor '.$type.' - '.$trans_no); 
                   
		$this->allocs = array();

		$this->trans_no = $trans_no;
		$this->type = $type;
                $this->line_payment_methods = array();
		if ($person_id)
			$this->set_person($person_id, $person_type_id);

                 
                $this->show_invoice = $show_invoice;
		$this->read($type, $trans_no, $person_id, $person_type_id, $account); // read payment or credit
               
                
                
	}

	function set_person($person_id, $person_type)
	{
		$this->person_id = $person_id;
		$this->person_type = $person_type;
		$this->person_curr = $person_type == PT_SUPPLIER ?
			get_supplier_currency($person_id) : get_customer_currency($person_id);
		return $this->person_curr;
	}

	function add_item($type, $type_no, $date_, $due_date, $amount, $amount_allocated, 
		$current_allocated, $ref)
	{       //display_notification('function add_item inicio '.$type_no.' - '.$amount);
		if (floatcmp($amount, 0))
		{ //display_notification('function add_item inicio si');
                        //display_notification('add_item inicio si');
			$this->allocs[count($this->allocs)] = new allocation_item($type, $type_no, 
				$date_, $due_date, $amount, $amount_allocated, $current_allocated, $ref);
			return true;
		} 
		else 
		{ //display_notification('function add_item inicio no');
			return false;
		}
	}
	
	function update_item($index, $type, $type_no, $date_, $due_date, 
		$amount, $amount_allocated, $current_allocated, $ref)
	{
		if (floatcmp($amount, 0))
		{
			$this->allocs[$index] = new allocation_item($type, $type_no, 
				$date_, $due_date, $amount, $amount_allocated, $current_allocated, $ref);
			return true;
		} 
		else 
		{
			return false;
		}
	}
	
	function add_or_update_item($type, $type_no, $date_, $due_date, 
		$amount, $amount_allocated, $current_allocated, $ref)
	{
		for ($i = 0; $i < count($this->allocs); $i++) 
		{
			$item = $this->allocs[$i];
			if (($item->type == $type) && ($item->type_no == $type_no)) 
			{
				return $this->update_item($i, $type, $type_no, $date_, $due_date, 
					$amount, $amount_allocated, $current_allocated, $ref);
			}
		}
        return $this->add_item($type, $type_no, $date_, $due_date, 
        	$amount, $amount_allocated, $current_allocated, $ref);
	}
        
        /*Begin Felix Alberti 25/10/2017*/
        function add_metodo_pago_from_bd($line_no,$type,$type_transac,$amount,$number,$date_deposit,$bank_account){            
            $metodo = new payment_method($line_no,$type,$type_transac,$amount,$number,$date_deposit,$bank_account);
            $this->line_payment_methods[$line_no] = $metodo;
        
        }
        /*End Felix Alberti 25/10/2017*/
        
        /*Begin Felix Alberti 15/04/2016*/
        function add_metodo_pago($type,$type_transac,$amount,$number,$date_deposit,$bank_account,$id_orig=null){
            //display_notification('function add_metodo_pago');
            $exist = 0;
            foreach ($this->line_payment_methods as $pay_method) {
                if ($pay_method->delete == 0){
                    if ($pay_method->number == $number ){
                        $exist = 1;
                        break;
                    }
                }
            }
            if ($exist == 1) return -1;
            //
            $line_no = count($this->line_payment_methods) + 1;
            $metodo = new payment_method($line_no,$type,$type_transac,$amount,$number,$date_deposit,$bank_account,$id_orig);
            $this->line_payment_methods[$line_no] = $metodo;
        
        }
        
        function remove_metodo_pago_from_cart($line_no)
	{
            $this->line_payment_methods[$line_no]->delete = 1;
	}

        /*End Felix Alberti 15/04/2016*/
        
        /*Begin Felix Alberti 15/04/2016*/
        function get_items_metodos_pago()
	{
		$total = 0;

		foreach ($this->line_payment_methods as $pay_method) {
                        if ($pay_method->delete == 0){
                            $amount = $pay_method->amount;
                            //$total += round($amount, user_price_dec());
                            $data_method = get_data_payment_method($pay_method->type);
                            if ($data_method['polarity']=='+')
                            $total += $amount;
                            //else
                            //$total -= $amount;
                        }
		}
               
		return $total;
	}
        /*End Felix Alberti 15/04/2016*/
        
        /*Begin Felix Alberti 14/10/2016*/
        function get_discount_items_metodos_pago()
	{
		$total = 0;

		foreach ($this->line_payment_methods as $pay_method) {
                        if ($pay_method->delete == 0){
//                            $discount = $pay_method->discount;
                            //$total += round($amount, user_price_dec());
                            $total += $discount;
                        }
		}
               
		return $total;
	}
        /*End Felix Alberti 14/10/2016*/

	//
	//	Read payment or credit current/available allocations to cart.
	//
	// FIXME - read all transactions below twice seems to be suboptimal
	//
	function read($type = null, $trans_no = 0, $person_id=null, $person_type_id=null, $account=false)
	{       
            //display_notification('function read *********');
                //display_notification('(*) function read1 '.$type.' - '.$trans_no.' - person_id: '.$person_id);        
                //display_error('function read 1');
		if ($type !== null) {	// otherwise re-read  
//display_notification('(*) function read2 '.$type.' - '.$trans_no.' - person_id: '.$person_id);   		
			$type = $this->type;
			$trans_no = $this->trans_no;
                        
                        //display_error('function read 3');   
                        
			if (isset($person_type_id))
			{
				$this->person_type = $person_type_id;
				$this->person_id = $person_id;
                //display_error('function read 4');   
                //display_notification('function read2 *********');
			} else { // guess person_type_id
				if (in_array($type, array(ST_BANKPAYMENT, ST_BANKDEPOSIT)))
				{
					$bank_trans = db_fetch(get_bank_trans($type, $trans_no));
					$this->person_type = $bank_trans['person_type_id'];
				} else
					$this->person_type = in_array($type, array(ST_SUPPCREDIT, ST_SUPPAYMENT)) ? PT_SUPPLIER : PT_CUSTOMER;
			
                        //display_error('function read 5');           
                                //display_notification('function read 3*********');
                        }

			if ($trans_no) {
                            //display_notification('1 (*) ***** '.' - '.$trans_no); 
                                //if ($account==false) {
                                    //display_notification("leyendo transacciones del cliente ".$this->person_type.'='.PT_SUPPLIER.' - '.$trans_no);
                                    $trans = $this->person_type == PT_SUPPLIER ? get_supp_trans($trans_no, $type, $person_id)
                                            : get_customer_trans($trans_no, $type, $person_id);
                                    
                                    //display_notification('--function read fino '.$person_id.' - '.$trans['trans_no']);
                                    $this->person_id = $trans[$this->person_type == PT_SUPPLIER ? 'supplier_id':'debtor_no'];
                                    $this->person_name = $trans[$this->person_type == PT_SUPPLIER ? "supplier_name":"DebtorName"];
                                    $this->date_ = sql2date($trans["tran_date"]);
                                    $this->person_curr = $trans['curr_code'];
                                    $this->currency = isset($trans['bank_curr_code']) ? $trans['bank_curr_code'] : $trans['curr_code'];
                                    $this->bank_amount = @$trans["bank_amount"];
                                    $this->amount = $trans["Total"];
                                //}
                               //display_error('function read 6');                
			} else
				$this->date_ = Today();
		}
                //display_error('function read 7');
	/* Now populate the array of possible (and previous actual) allocations 
		for this customer/supplier. First get the transactions that have 
		outstanding balances ie Total-alloc >0 */
                //display_error('2 (*) ***** '.' - '.$trans_no);       
                 
		$this->allocs = array();
		//display_notification('1234-call get_allocatable_to_supp_transactions '.$this->person_id.' - '.$trans_no.' - '.$this->person_type.'<=>'.PT_SUPPLIER); 
		if ($this->person_id && $trans_no == 0)
		{
		    // display_notification('read person_id '.' - '.$this->person_id);
			if ($this->person_type==PT_SUPPLIER){
			    display_notification('123-call get_allocatable_to_supp_transactions'); 
				$trans_items = get_allocatable_to_supp_transactions($this->person_id);
			}
			else {
			display_notification('xyz-call get_allocatable_to_supp_transactions'); 
                            if ($this->show_invoice == 1) {
                                display_notification("read all transactions below twice seems to be suboptimal");
                                //display_error("1 read all transactions below twice seems to be suboptimal ".$trans_no.' - '.$this->person_id);                                 
                                $trans_items = get_allocatable_to_cust_transactions($this->person_id);
                                //display_error("1 end read all transactions below twice seems to be suboptimal ".$this->person_id); 
                            }
                            else {
                                display_notification("read all transactions cta");
                                $sql = "SELECT * FROM ".TB_PREF."payer_trans WHERE id_account = ".  db_escape($account);
                                //display_error('show_allocatable_account_patient '.$sql.' - '.$trans_no);
                                $trans_items = db_query($sql,"respons_payment could not be added");
                                //$myrow_account = db_fetch_assoc($result);
                            }
            }
                        if ($this->show_invoice == 1) {
						display_notification("poraqui0 poraqui0");
                            while ($myrow = db_fetch($trans_items))
                            {       
                                    //display_notification("leyendo transacciones del cliente ".$myrow["trans_no"]);
                                   // display_error("leyendo transacciones del cliente ".$myrow["trans_no"]);
                                    $this->add_item($myrow["type"], $myrow["trans_no"],
                                            sql2date($myrow["tran_date"]),
                                            sql2date($myrow["due_date"]),
                                            $myrow["Total"], // trans total
                                            $myrow["alloc"], // trans total allocated
                                            0,
                                            $myrow["reference"]); // this allocation
                            }
                        }
                        else {
						//display_notification("poraqui1 poraqui1");
                            while ($myrow = db_fetch($trans_items))
                            {       
                                    //display_error("leyendo transacciones del cliente ".$myrow["id"]);
									if (isset($myrow["id_account"])){  
										$this->add_item(ST_PATIENT_ACCOUNT, $myrow["id_account"],
												sql2date($myrow["fe_trans"]),
												sql2date($myrow["fe_trans"]),
												$myrow["amount"], // trans total
												$myrow["paid"], // trans total allocated
												0,
												$myrow["reference"]); // this allocation
									}
                                    //display_notification("add_item final transacciones del cliente ".$myrow["id"]);
                            }
                            
                        }
                        //Begin 28/12/2015 Felix Alberti
                        /*if ($this->person_type==PT_CUSTOMER) {
                            $trans_items_account = get_allocatable_to_payer_transactions($this->person_id);
                            //
                            while ($myrow_acc = db_fetch($trans_items_account))
                            {                                
                                    $this->add_item($myrow_acc["type"], $myrow_acc["trans_no"],
                                            sql2date($myrow_acc["tran_date"]),
                                            sql2date($myrow_acc["tran_date"]),
                                            $myrow_acc["Total"], // trans total
                                            $myrow_acc["alloc"], // trans total allocated
                                            $myrow_acc["amount"], //current allocated
                                            $myrow_acc["reference"]); // this allocation
                            }
                        }*/
                        //End 28/12/2015 Felix Alberti
		}
        //display_notification("return Now get trans that might ---> ".$this->trans_no.' - '.$this->show_invoice);  
		if ($this->trans_no == 0 && $this->show_invoice == 1) return; // this is new payment

	/* Now get trans that might have previously been allocated to by this trans
	NB existing entries where still some of the trans outstanding entered from
	above logic will be overwritten with the prev alloc detail below */
                //display_error("SIGUE ---> ");  
                
		if ($this->person_type==PT_SUPPLIER){
		    //display_notification("xyz-return2 ---> ".$this->trans_no.' - '.$this->show_invoice);  
			$trans_items = get_allocatable_to_supp_transactions($this->person_id, $this->trans_no, $this->type);
			//Si no existe aqui se puede insertar data en la tabla 0_supp_allocations 
		}
		else 
		{
                     //display_error("data ".$this->trans_no.' - '.$account);    
                     if ($this->show_invoice == 1) {
                        //display_error("3 read all transactions allocatable ".$trans_no.' - '.$this->person_id);    
		 	//$trans_items = get_allocatable_to_cust_transactions($this->person_id, $this->trans_no, $this->type);
                        
                        $sql_cont  = "SELECT COUNT(*) as cant FROM ".TB_PREF."cust_allocations alloc WHERE 
                                trans_type_from = " . ST_CUSTPAYMENT . " AND trans_no_from=".db_escape($this->trans_no);
                        $result = db_query($sql_cont, "Cannot retrieve alloc to transactions");
                        //display_error($sql_cont);
                        $myrow = db_fetch($result);
                        //display_error('cant '.$myrow['cant'].' no trans: '.$this->trans_no);
                        if ($myrow['cant'] > 0){
                            $trans_items = get_allocatable_to_cust_transactions($this->person_id,$this->trans_no,$this->type);
                        }
                        else {
                            $cond = "";
                            if ($this->trans_no > 0) $cond = " AND trans.trans_no = ".db_escape($this->trans_no);
                            $sql = get_alloc_trans_sql("0 as amt", "round(ov_amount+ov_gst+ov_freight+ov_freight_tax+ov_discount-alloc,6) > 0
                                    AND trans.type = " . ST_SALESINVOICE . "
                                    AND trans.debtor_no=".db_escape($this->person_id).$cond);                            
                            $trans_items = db_query($sql, "Cannot retrieve alloc to transactions");
                            
                            //display_error($sql);
                        }
                     } 
                     else {
                         //display_notification('--****');
                          /*$sql = "SELECT * FROM ".TB_PREF."cust_allocations WHERE trans_no_from = ".  db_escape($trans_no)." and "
                                  . " trans_type_from = ".ST_CUSTPAYMENT;
                           display_error("1 QUERY ".$sql);    
                          $trans_items = db_query($sql,"cust_allocations could not be added");*/
                          //$trans_items = get_allocatable_to_cust_transactions($this->person_id, $this->trans_no, $this->type);   
                        $sql = get_alloc_trans_sql("amt, alloc.trans_type_to as account, alloc.trans_no_to as trans_account", "trans.trans_no = alloc.trans_no_from
			AND alloc.trans_no_from=".db_escape($trans_no)."
			AND alloc.trans_type_from=".db_escape($this->type)."
			AND trans.debtor_no=".db_escape($this->person_id),
			"".TB_PREF."cust_allocations as alloc");
                        //display_error($sql);
                        $trans_items = db_query($sql." ORDER BY trans_no", "Cannot retrieve alloc to transactions");
                         
                     }
					 //display_notification('--****final->');	
        }    
                //display_notification('--****SIGUE->');				
                //if ($account==false) {
                    while ($myrow = db_fetch($trans_items))
                    {   
						//display_notification('while '.$myrow["type"].' - '.$myrow["trans_no"]);
                        $type = $myrow["type"];
                        $trans_no = $myrow["trans_no"];
						//display_notification($type);
						$amt = 0;
						if (isset($myrow["amt"]) && $myrow["amt"] > 0)
                        $amt = $myrow["amt"];
                        $total = $myrow["Total"];
						//display_notification('while sigue total '.$total);
                        if ($this->person_type==PT_CUSTOMER && $this->show_invoice == 0){
						    if (isset($myrow["account"]))
                            $type = $myrow["account"];
							if (isset($myrow["trans_account"]))
                            $trans_no = $myrow["trans_account"];
                            //display_error("xxxx ".$trans_no);    
                            $total = $amt;
                        }  
                        //display_notification('***-while sigue --> total '.$total);						
                        //display_error($myrow["alloc"].' ---- '.$amt);
                            /*$this->add_or_update_item ($type, $trans_no,
                                    sql2date($myrow["tran_date"]),
                                    sql2date($myrow["due_date"]),
                                    $total,
                                    $myrow["alloc"] - $amt, $amt, $myrow["reference"]);*/
									
							$this->add_item($type, $trans_no, sql2date($myrow["tran_date"]), sql2date($myrow["due_date"]), $total, $myrow["alloc"] - $amt, 
		$amt, $myrow["reference"]);
						//display_notification('while sigue '.$myrow["trans_no"]);
                    }
               //}
                    //display_notification('final while sigue ');
				if ($this->person_type==PT_SUPPLIER){
				}
				else {
                    $sql = "SELECT * FROM ".TB_PREF."debtor_trans_payment_method WHERE trans_type = ".
                            db_escape($this->type)." and trans_no = ".db_escape($this->trans_no);
                    //display_notification('method_paym ****'.$sql);
                    $method_paym_items = db_query($sql);
                    while ($myrow_paym = db_fetch($method_paym_items))
                    {  //display_notification('method_paym ****'.$myrow_paym["id"]);
                       $id_orig = $myrow_paym["id"];
                       $type = $myrow_paym["type_method_pay"];
                       $type_trans = $myrow_paym["type"];
                       $amount = $myrow_paym["amount"];
                       $number = $myrow_paym["number"];
                       $data_method = get_method_payments($type);
                       $bank_account = $data_method['bank_account'];
                       $deposit_date = sql2date($myrow_paym["trans_date"]);
                       $this->add_metodo_pago($type,$type_trans,$amount,$number,$deposit_date,$bank_account,$id_orig);
                    }
				}
	}
	//
	//	Update allocations in database.
	//
	function write($account='')
	{
		begin_transaction();
                
		if ($this->person_type == PT_SUPPLIER)
			clear_supp_alloctions($this->type, $this->trans_no, $this->date_);
		else
		 	clear_cust_alloctions($this->type, $this->trans_no, $this->date_);
                
		// now add the new allocations
		$total_allocated = 0;
		$dec = user_price_dec();
                
		foreach ($this->allocs as $alloc_item)
		{
                        
			if ($alloc_item->current_allocated > 0)
			{                                  
				$amount = round($alloc_item->current_allocated, $dec);
                                
				if ($this->person_type == PT_SUPPLIER) {
					add_supp_allocation($amount,
						$this->type, $this->trans_no,
			    	 	$alloc_item->type, $alloc_item->type_no, $this->date_);

					update_supp_trans_allocation($alloc_item->type, $alloc_item->type_no);
				} else {
//                                        display_error($this->type. ' -    '. $this->trans_no.' <---> '.
//                                                $alloc_item->type. ' - '. $alloc_item->type_no);
                                        //if ($account==''){
	 				add_cust_allocation($amount,
						$this->type, $this->trans_no,
 			     		$alloc_item->type, $alloc_item->type_no, $this->date_);

	 				update_debtor_trans_allocation($alloc_item->type, $alloc_item->type_no);
                                            
//                                        }
//                                        else {
//                                            add_cust_allocation($amount,
//						$this->type, $this->trans_no,
// 			     		$alloc_item->type, $alloc_item->type_no, $this->date_, $this->account, $_POST['ref']);
//
//	 				    //update_debtor_trans_allocation($alloc_item->type, $alloc_item->type_no);
                                        //}
				}
				// Exchange Variations Joe Hunt 2008-09-20 ////////////////////
                                //display_error($this->type. ' -    '. $this->trans_no);
                                if ($account=='' && $alloc_item->type != ST_PATIENT_ACCOUNT)
				exchange_variation($this->type, $this->trans_no,
					$alloc_item->type, $alloc_item->type_no, $this->date_,
					$amount, $this->person_type);

				//////////////////////////////////////////////////////////////
				$total_allocated += $alloc_item->current_allocated;
			}

		}  /*end of the loop through the array of allocations made */
		if ($this->person_type == PT_SUPPLIER)
			update_supp_trans_allocation($this->type, $this->trans_no);
		else {
                       //if ($account==false)
                       update_debtor_trans_allocation($this->type, $this->trans_no);    
                }

                        

		commit_transaction();

	}       
      

} 

//-----------------------------------------------------------------------------------

class allocation_item 
{

	var $type;
	var $type_no;
	
	var $date_;
	var $due_date;
	
	var $amount_allocated;
	var $amount;
	var $ref;
	
	var $current_allocated;
	
	function allocation_item ($type, $type_no, $date_, $due_date, $amount, 
		$amount_allocated, $current_allocated, $ref)
	{

		$this->type = $type;
		$this->type_no = $type_no;

		$this->ref = $ref;

		$this->date_ = $date_;
		$this->due_date = $due_date;
		
		$this->amount = $amount;
		$this->amount_allocated = $amount_allocated;
		$this->current_allocated = $current_allocated;
	}
}
//--------------------------------------------------------------------------------
class payment_method
{ 
    var $line;
    var $type;
    var $type_trans;
    var $amount;
    var $number;
    var $date_deposit;
    var $bank_account;
    var $delete = 0;
    var $id_orig;
    //
    function payment_method($line, $type, $type_transac, $amount, $number, $date_deposit, $bank_account, $id_orig=null){
        //display_notification('constructor type: '.$line.' - '.$type);
        $this->line = $line;
        $this->type = $type;
        $this->type_trans = $type_transac;
        $this->amount = $amount;
        $this->number = $number;
        $this->date_deposit = $date_deposit;
        $this->bank_account = $bank_account;
        $this->id_orig = $id_orig;
        
    }
}
//--------------------------------------------------------------------------------

function show_allocatable($show_totals) {
    //display_notification('show_allocatable');
	global $systypes_array;
	
    $k = $counter = $total_allocated = 0;

	$cart = $_SESSION['alloc'];
	$supp_ref = in_array($cart->type, array(ST_SUPPCREDIT, ST_SUPPAYMENT, ST_BANKPAYMENT));

	if (count($cart->allocs)) 
	{
		if ($cart->currency != $cart->person_curr)
			display_heading(sprintf(_("Allocated amounts in %s:"), $cart->person_curr));
		start_table(TABLESTYLE, "width=60%");
   		$th = array(_("Transaction Type"), _("#"), $supp_ref ? _("Supplier Ref"): _("Ref"), _("Date"), _("Due Date"), _("Amount"),
   			_("Other Allocations"), _("Left to Allocate"), _("This Allocation"),'','');

	   	table_header($th);

		foreach ($cart->allocs as $id => $alloc_item)
		{                    
		    if (floatcmp(abs($alloc_item->amount), $alloc_item->amount_allocated))
		    {
				alt_table_row_color($k);
    			 label_cell($systypes_array[$alloc_item->type]);
	   			label_cell(get_trans_view_str($alloc_item->type, $alloc_item->type_no));
		   		label_cell($alloc_item->ref);
    			label_cell($alloc_item->date_, "align=right");
    			label_cell($alloc_item->due_date, "align=right");
	    		amount_cell(abs($alloc_item->amount));
				amount_cell($alloc_item->amount_allocated);

		    	$_POST['amount' . $id] = price_format($alloc_item->current_allocated);

	    		$un_allocated = round((abs($alloc_item->amount) - $alloc_item->amount_allocated), 6);
	    		amount_cell($un_allocated, false,'', 'maxval'.$id);
    			amount_cells(null, "amount" . $id);//, input_num('amount' . $id));
				label_cell("<a href='#' name=Alloc$id onclick='allocate_all(this.name.substr(5));return true;'>"
					 . _("All") . "</a>");
				label_cell("<a href='#' name=DeAll$id onclick='allocate_none(this.name.substr(5));return true;'>"
					 . _("None") . "</a>".hidden("un_allocated" . $id, 
					 price_format($un_allocated), false));
				end_row();

   	    		$total_allocated += input_num('amount' . $id);
		   	}
		}
		if ($show_totals) {
    	   	label_row(_("Total Allocated"), price_format($total_allocated),
	    		"colspan=8 align=right", "align=right id='total_allocated'", 3);
/*
			$amount = $_SESSION['alloc']->amount;

			if ($_SESSION['alloc']->type == ST_SUPPCREDIT
				|| $_SESSION['alloc']->type == ST_SUPPAYMENT
				||  $_SESSION['alloc']->type == ST_BANKPAYMENT)
				$amount = -$amount;
*/
			$amount = abs($cart->amount);

			if (floatcmp($amount, $total_allocated) < 0)
	        {
        		$font1 = "<font color=red>";
        		$font2 = "</font>";
    	    }
	        else
        		$font1 = $font2 = "";
			$left_to_allocate = price_format($amount - $total_allocated);
	        label_row(_("Left to Allocate"), $font1 . $left_to_allocate . $font2, 
				"colspan=8 align=right", "nowrap align=right id='left_to_allocate'",
				 3);
		}
		end_table(1);
	}
	hidden('TotalNumberOfAllocs', count($cart->allocs));
}
//--------------------------------------------------------------------------------
//Begin Felix Alberti 11/10/2016
function show_allocatable_account_patient($account) {
//    $sql = "SELECT * FROM ".TB_PREF."payer_trans WHERE id_account = ".  db_escape($account);
    //display_notification('function show_allocatable_account_patient ');
//    $result = db_query($sql,"The respons_payment could not be added");
//    $myrow = db_fetch_assoc($result);
    //$id = '0';
    $balance = 0;
    if (count($_SESSION['alloc']->line_payment_methods) > 0 ){
        foreach ($_SESSION['alloc']->line_payment_methods as $pay_method){
            $data_method = get_method_payments($pay_method->type);
            if ($data_method['polarity']=='+')
            $balance += $pay_method->amount;
            else
            $balance -= $pay_method->amount;
        }
    }
//    else        
//    $balance = abs($myrow['amount']) - $myrow['paid'];
    
//    display_notification($myrow['amount'].' - '.$myrow['paid'].' = '.'balance '.$balance);
    $cart = $_SESSION['alloc']; 
    $k = 0;
    if (count($cart->allocs)) 
    {
                start_table(TABLESTYLE, "width=60%");
   		$th = array(_("Transaction Type"), _("#"), _("Date"), _("Amount"),
   			_("Other Allocations"), _("Left to Allocate"), _("This Allocation"),'','');

	   	table_header($th);                
           
                foreach ($cart->allocs as $id => $alloc_item)
		{  
                alt_table_row_color($k);
                     
//                if (isset($myrow['fe_trans'])){
                    if (count($_SESSION['alloc']->line_payment_methods) <= 0 )
                    $balance = abs($alloc_item->amount) - $alloc_item->amount_allocated;
                    
                    label_cell("Account Patient");
                    if ($account==null)
                    label_cell($alloc_item->type_no);
                    else
                    label_cell($account);
                    //label_cell($myrow['fe_trans'], "align=right");
                    label_cell($alloc_item->date_, "align=right");
                    //label_cell(price_format($myrow['amount']), "align=right");
                    label_cell(price_format(abs($alloc_item->amount)), "align=right");
                    //label_cell(price_format($myrow['paid']), "align=right");
                    label_cell(price_format($alloc_item->amount_allocated), "align=right");

                    $_POST['amount' . $id] = price_format($balance);

                    $un_allocated = round(($balance), 6);
                    amount_cell($un_allocated, false,'', 'maxval'.$id);
                    amount_cells(null, "amount" . $id);
                    label_cell("<a href='#' name=Alloc$id onclick='allocate_all(this.name.substr(5));return true;'>"
                                             . _("All") . "</a>");
                    label_cell("<a href='#' name=DeAll$id onclick='allocate_none(this.name.substr(5));return true;'>"
                                             . _("None") . "</a>".hidden("un_allocated" . $id, 
                                             price_format($un_allocated), false));
//                }
                end_row();
                }
                end_table(1);
                
                hidden('TotalNumberOfAllocs', 1);
    }            
}
//End Felix Alberti 11/10/2016

//Begin Felix Alberti 31/10/2016
function show_allocatable_account_patient_pend($account,$trans_no,$debtor_no,$type,$alloc) {   
    
    $sql = "SELECT count(*) as cant FROM ".TB_PREF."payer_trans WHERE id_account = ".  db_escape($account).
            " and id_responsible = ".db_escape($debtor_no);
    //display_notification('show_allocatable_account_patient '.$sql);
    $result = db_query($sql,"The payer_trans could not be retrieved");
    $myrow = db_fetch_assoc($result);
    if ($myrow['cant'] == 0){
        add_payer_trans($account, $debtor_no, $alloc, 0, 0, 0);
    }
    
    $sql = "SELECT * FROM ".TB_PREF."payer_trans WHERE id_account = ".  db_escape($account);
    //display_notification('show_allocatable_account_patient '.$sql);
    $result = db_query($sql,"The respons_payment could not be retrieved");
    $myrow = db_fetch_assoc($result);
    $id = '0';
    
    $balance = abs($myrow['amount']) - $myrow['paid'];
   
        
    //display_notification($myrow['amount'].' - '.$myrow['paid'].' = '.'balance '.$balance.' type= '.$type);   
    
    start_table(TABLESTYLE, "width=60%");
   		$th = array(_("Transaction Type"), _("#"), _("Date"), _("Amount"),
   			_("Other Allocations"), _("Left to Allocate"), _("This Allocation"));

	   	table_header($th);
                
                if (isset($myrow['fe_trans'])){
                    label_cell("Account Patient");
                    label_cell($account);
                    label_cell($myrow['fe_trans'], "align=right");
                    label_cell(price_format($myrow['amount']), "align=right");
                    label_cell(price_format($myrow['paid']), "align=right");
                    //label_cell(price_format($myrow['balance']), "align=right");
                    //$_POST['amount' . $id] = price_format($alloc);
                    //amount_cells(null, "amount_account");
                    $un_allocated = round(($balance), 6);
                    amount_cell($un_allocated, false,'', 'maxval'.$id);
                    hidden('balance'.$id, $balance);
                    //amount_cells(null, "amount" . $id);
                    label_cell(price_format($alloc),"align='right'");
                    hidden('amount' . $id, $alloc);
                }
                end_table(1);
                
                hidden('patient_account', $account);                
                hidden('debtor_no', $debtor_no);
                hidden('payment_no', $trans_no);
		hidden('type', $type);
                
}
//End Felix Alberti 31/10/2016

function check_allocations()
{     
	global $SysPrefs;

	$total_allocated = 0;

	for ($counter = 0; $counter < get_post("TotalNumberOfAllocs"); $counter++)
	{
		if (!isset($_POST['amount'.$counter])) continue;
		if (!check_num('amount' . $counter, 0))
		{
			display_error(_("The entry for one or more amounts is invalid or negative."));
			set_focus('amount'.$counter);
			return false;
		 }

		  /* Now check to see that the AllocAmt is no greater than the
		 amount left to be allocated against the transaction under review;
		 skip check if no allocation is set to avoid deadlock on mistakenly overallocated transactions*/
		 $allocated = input_num('amount' . $counter);
		 if ($allocated && ($allocated > input_num('un_allocated' . $counter)))
		 {
			display_error(_("At least one transaction is overallocated."));
			set_focus('amount'.$counter);
			return false;
		 }

		 $_SESSION['alloc']->allocs[$counter]->current_allocated = input_num('amount' . $counter);

		 $total_allocated += input_num('amount' . $counter);
	}
/*
	$amount = $_SESSION['alloc']->amount;

	if (in_array($_SESSION['alloc']->type, array(ST_BANKPAYMENT, ST_SUPPCREDIT, ST_SUPPAYMENT)))
		$amount = -$amount;
*/
	$amount = abs($_SESSION['alloc']->amount);

	if ($total_allocated - ($amount + input_num('discount'))  > $SysPrefs->allocation_settled_allowance())
	{
		display_error(_("These allocations cannot be processed because the amount allocated is more than the total amount left to allocate."));
		return false;
	}

	return true;
}
