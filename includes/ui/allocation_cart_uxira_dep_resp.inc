<?php
/**********************************************************************
    Copyright 2017 Agosto (C) Uxira, C.A.
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
        var $tax_amount;
        var $deductible_amount;
        var $minimum_amount;
        var $sales_gl_account;
        var $cutoff_date;//fap 11/04/2019
        var $limit_allocate;//fap 08/07/2019

	function allocation($type, $trans_no, $person_id = null, $person_type_id=null, $serial=null)
	{
                        
		$this->allocs = array();

		$this->trans_no = $trans_no;
		$this->type = $type;
                $this->line_payment_methods = array();
		if ($person_id)
			$this->set_person($person_id, $person_type_id);

		$this->read($type, $trans_no, $person_id, $person_type_id, $serial); // read payment or credit
	}

	function set_person($person_id, $person_type)
	{
		$this->person_id = $person_id;
		$this->person_type = $person_type;
		$this->person_curr = $person_type == PT_SUPPLIER ?
			get_supplier_currency($person_id) : get_customer_currency($person_id);
		return $this->person_curr;
	}
        
        function set_cutoff_date($cutoff_date)
        {
            $this->cutoff_date = $cutoff_date;
        }

	function add_item($type, $type_no, $date_, $due_date, $amount, $amount_allocated, 
		$current_allocated, $ref, $debtor_no, $branch_code, $type_paym=0, $trans_no_pay=0, $account_assoc, $tax=null, $minimum_amount=null, $amoun_cobertura, $serial, $tax_id, $dcto=null, $number_key)
	{ 
		if (floatcmp($amount, 0))
		{       //display_error("add item ".$amount);
			$this->allocs[count($this->allocs)] = new allocation_item($type, $type_no, 
				$date_, $due_date, $amount, $amount_allocated, $current_allocated, $ref, $debtor_no, $branch_code, $type_paym, $trans_no_pay, $account_assoc, $tax, $minimum_amount, $amoun_cobertura, $serial, $tax_id, $dcto, $number_key);
			return true;
		} 
		else 
		{
			return false;
		}
	}
	
	function update_item($index, $type, $type_no, $date_, $due_date, 
		$amount, $amount_allocated, $current_allocated, $ref, $debtor_no, $branch_code, $type_paym, $trans_paym, $account_associated, $tax=null, $minimum_amount=null, $amount_cobertura, $serial, $tax_id, $dscto, $number_key)
	{
                //display_notification("2 return update_item update ".$trans_paym);
                //display_notification("2 return update_item update ---*  ".$amount.', '.$tax.', '.$dscto);
		if (floatcmp($amount_cobertura, 0))
		{
                    //display_notification('('.$index.') - update_item --> '.$type_paym . " - ".$trans_paym.' -> '.$tax.' - '.$dscto);
			$this->allocs[$index] = new allocation_item($type, $type_no, 
				$date_, $due_date, $amount, $amount_allocated, $current_allocated, $ref, $debtor_no,
                                $branch_code, $type_paym, $trans_paym, $account_associated, $tax,
                                $minimum_amount, $amount_cobertura, $serial, $tax_id, $dscto, $number_key);
			return true;
		} 
		else 
		{
			return false;
		}
	}
        
        function update_item_data_extra($type, $type_no, $date_, $due_date, 
					$amount, $amount_allocated, $current_allocated, $ref, $debtor_no, $branch_code, $type_paym, $trans_paym, $account_associated, $tax,
                                        $minimum_amount, $amount_cobertura, $serial, $tax_id, $dscto, $number_key){
                        // display_notification("datos ( type: ".$type_paym . " - trans_no: ". $trans_paym." )");
			
                        
                        for ($i = 0; $i < count($this->allocs); $i++) 
		        {   $item = $this->allocs[$i];                           
                            if (($item->type == $type) && ($item->type_no == $type_no) &&
                                    ($item->number_key == $number_key)) 
                            {    
                                 //display_notification("update_item_tax ".$item->type." - ".$type." <-> ".$item->type_no . " - " . $type_no);
                                 //display_notification("1 return update_item update ".$trans_paym);

                                    return $this->update_item($i, $type, $type_no, $date_, $due_date, 
                                            $amount, $amount_allocated, $current_allocated, $ref, $debtor_no, $branch_code, $type_paym,
                                            $trans_paym, $account_associated, $tax, $minimum_amount, $amount_cobertura, $serial, $tax_id, $dscto, $number_key);
                            }
                        }
		
        }
	
	function add_or_update_item($type, $type_no, $date_, $due_date, 
		$amount, $amount_allocated, $current_allocated, $ref)
	{       display_error('add_or_update_item');
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
        
       
        
        function update_item_tax($type=null, $type_no=null, $date_=null, $due_date=null, 
		$amount=0, $amount_allocated=0, $current_allocated=0, $ref=null, $debtor_no=null, $branch_code=null, $type_paym=null,
                $trans_paym=null, $account_associated=null, $tax=null, $minimum_amount=null,
                $amount_cobertura=null, $serial=null, $tax_id=null, $dscto=null, $number_key=null)
	{
                //display_notification('update_item_tax '.$type_no.' - dscto '.$tax_id.' - '.$dscto);
		for ($i = 0; $i < count($this->allocs); $i++) 
		{
			$item = $this->allocs[$i];
			if (($item->type == $type) && ($item->type_no == $type_no) && ($item->number_key == $number_key)) 
			{
                           
				return $this->update_item($i, $type, $type_no, $date_, $due_date, 
					$amount, $amount_allocated, $current_allocated, $ref,
                                        $debtor_no, $branch_code, $type_paym, $trans_paym, $account_associated, $tax, $minimum_amount, $amount_cobertura, $serial, $tax_id, $dscto, $number_key);
			}
		}        
	}
        
        /*Begin Felix Alberti 15/04/2016*/
        function add_metodo_pago($type,$type_transac,$amount,$number){
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
            $metodo = new payment_method($line_no,$type,$type_transac,$amount,$number);
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
                            $total += $amount;
                        }
		}
               
		return $total;
	}
        /*End Felix Alberti 15/04/2016*/

	//
	//	Read payment or credit current/available allocations to cart.
	//
	// FIXME - read all transactions below twice seems to be suboptimal
	//
	function read($type = null, $trans_no = 0, $person_id=null, $person_type_id=null, $serial = null)
	{     
                global $go_debug; 
		if ($type !== null) {	// otherwise re-read                    
			$type = $this->type;
			$trans_no = $this->trans_no;
                        
			if (isset($person_type_id))
			{   
				$this->person_type = $person_type_id;
				$this->person_id = $person_id;
			} else { // guess person_type_id
				if (in_array($type, array(ST_BANKPAYMENT, ST_BANKDEPOSIT)))
				{
                                 
					$bank_trans = db_fetch(get_bank_trans($type, $trans_no));
					$this->person_type = $bank_trans['person_type_id'];
				} else
					$this->person_type = in_array($type, array(ST_SUPPCREDIT, ST_SUPPAYMENT)) ? PT_SUPPLIER : PT_CUSTOMER;
			}

			if ($trans_no) {
                            
                                if ($this->person_type == PT_RESP_PAYMENT){
                                $trans = get_customer_bank_trans($trans_no, $type, $person_id);
                                }                            
                                else { 
				$trans = $this->person_type == PT_SUPPLIER ? get_supp_trans($trans_no, $type, $person_id)
					: get_customer_trans($trans_no, $type, $person_id);
                                }

                                if ($this->person_type == PT_RESP_PAYMENT){
                                $this->person_id = $trans['debtor_no'];
                                $this->person_name = $trans["DebtorName"];
                                }
                                else {
				$this->person_id = $trans[$this->person_type == PT_SUPPLIER ? 'supplier_id':'debtor_no'];
				$this->person_name = $trans[$this->person_type == PT_SUPPLIER ? "supplier_name":"DebtorName"];
                                }
				$this->date_ = sql2date($trans["tran_date"]);
				$this->person_curr = $trans['curr_code'];
				$this->currency = isset($trans['bank_curr_code']) ? $trans['bank_curr_code'] : $trans['curr_code'];
				$this->bank_amount = @$trans["bank_amount"];
				$this->amount = $trans["Total"];
                                $this->limit_allocate = @$trans["amount"];                                
                                //display_error('this->limit_allocate->'.$this->limit_allocate);
			} else
				$this->date_ = Today();
		}
                
                $sum_base_imp = 0;
                $sum_deduc = 0;
                $sum_min_amount = 0; 
                $sales_gl_code = '';
                
                $cust_branch = get_data_branch_x_debtor($this->person_id);
                $ret_tax_array = get_tax_group_items_as_array_settlement($cust_branch['tax_settlement_group_id']);
                //var_dump($ret_tax_array);
                $line = 0;
               
                foreach ($ret_tax_array as $taxitem) 
                {
                    //display_notification($line.' - '.$taxitem["minimum_amount"]);
                    //display_notification($line.' - '.$taxitem["rate"]);
                    $sum_base_imp = $taxitem["rate"];
                    $sum_deduc = $taxitem["deductible_amount"];
                    $sum_min_amount = $taxitem["minimum_amount"];
                    $sales_gl_code = $taxitem["sales_gl_code"];
                    $line++;
                }
                $this->tax_amount = $sum_base_imp;
                $this->deductible_amount = $sum_deduc;
                $this->minimum_amount = $sum_min_amount;
                $this->sales_gl_account = $sales_gl_code;
                
                
	/* Now populate the array of possible (and previous actual) allocations 
		for this customer/supplier. First get the transactions that have 
		outstanding balances ie Total-alloc >0 */

		$this->allocs = array();
		if ($this->person_id)
		{
			if ($this->person_type==PT_SUPPLIER)
				$trans_items = get_allocatable_to_supp_transactions($this->person_id);
			else {//display_notification("read all transactions below twice seems to be suboptimal (uxira)");
                        /*Begin 25/06/2016 Felix Alberti*/
                        //$trans_items = get_allocatable_to_cust_transactions_uxira($this->person_id);
                        
                        if ($this->person_type == PT_RESP_PAYMENT)
                        $trans_items = get_allocatable_to_cust_transactions_uxira_fact_dep($this->person_id, null, $type, $serial);                            
                        else
                        $trans_items = get_allocatable_to_cust_transactions_uxira($this->person_id, null, $type);
                        /*End 25/06/2016 Felix Alberti*/
                        }
                        if (db_num_rows($trans_items) == 0) {
                           //if($go_debug)
			   //display_backtrace();
                           display_error ('Este responsable de pago no tiene facturas, revise');
                        }
                        //else
                        //display_error ('Este responsable de pago si tieve facturas');
			while ($myrow = db_fetch($trans_items))
			{  // display_error ($myrow["tran_date"].'-'.$myrow["Total"].'-'.$myrow["account_associated"]);
                            
                            $amtpart = getCobranzaFactParticular($myrow["trans_no"],$myrow["account_associated"]);
                            $amt = getCobranzaFactSeguro($myrow["trans_no"]);
                            $amt += $amtpart;
                            //if ($amt < $myrow["Total"]){
				$this->add_item($myrow["type"], $myrow["trans_no"],
					sql2date($myrow["tran_date"]),
					sql2date($myrow["due_date"]),
					$myrow["Total"], // trans total
					$amt, // trans total allocated
					0,
					$myrow["reference"],
                                        $myrow["debtor_no"],
                                        $myrow["branch_code"],
                                        null,
                                        null,
                                        $myrow["account_associated"],
                                        $this->tax_amount,
                                        $this->minimum_amount,
                                        $myrow["amount_cobertura"],
                                        $myrow["serial"],
                                        $myrow["tax_id"],
                                        0,
                                        $myrow["number_key"]); // this allocation
                            //}
			}
                        
		}
		if ($this->trans_no == 0) return; // this is new payment

	/* Now get trans that might have previously been allocated to by this trans
	NB existing entries where still some of the trans outstanding entered from
	above logic will be overwritten with the prev alloc detail below */

		if ($this->person_type==PT_SUPPLIER)
			$trans_items = get_allocatable_to_supp_transactions($this->person_id, 
				$this->trans_no, $this->type);
		else 
		 	$trans_items = get_allocatable_to_cust_transactions($this->person_id,
                $this->trans_no, $this->type);

                //if (db_num_rows($trans_items) == 0) {
                    //display_error ('No hay registros');
                //}
		while ($myrow = db_fetch($trans_items))
		{
			$this->add_or_update_item ($myrow["type"], $myrow["trans_no"],
				sql2date($myrow["tran_date"]),
				sql2date($myrow["due_date"]),
				$myrow["Total"],
				$myrow["alloc"] - $myrow["amt"], $myrow["amt"], $myrow["reference"]);
		}
	}
	//
	//	Update allocations in database.
	//
	function write()
	{       global $Refs;
        
		begin_transaction();
                
		if ($this->person_type == PT_SUPPLIER)
			clear_supp_alloctions($this->type, $this->trans_no, $this->date_);
		else
		 	clear_cust_alloctions($this->type, $this->trans_no, $this->date_);
                
                if ($this->person_type == PT_RESP_PAYMENT){
                   $ref = $Refs->get_next(ST_RESPONSPAYMENT);
                }
                
		// now add the new allocations
		$total_allocated = 0;
		$dec = user_price_dec();
                $rows = 0;
                $index = 0;
                $tot_allocated = 0;
                $tot_tax_amount = 0;
                $tot_dscto_amount = 0;
                foreach ($this->allocs as $id => $alloc_item)
		{
			if ($alloc_item->current_allocated > 0){
                            if (isset($_POST['tax_amount' . $id])){
                                $tax_amount = user_numeric($_POST['tax_amount' . $id]);
                                $dscto_amount = user_numeric($_POST['descuento_amount' . $id]);
                                //display_notification('call update_item_tax - '.$alloc_item->type_no.' - (dscto: '.$dscto_amount.') '.$tax_amount.' - '.$alloc_item->number_key);
                                $this->update_item_tax($alloc_item->type, $alloc_item->type_no,
                                        $alloc_item->date_, $alloc_item->due_date, 
                                        $alloc_item->amount, $alloc_item->amount_allocated, $alloc_item->current_allocated,
                                        $alloc_item->ref, $alloc_item->debtor_no, $alloc_item->branch_code,
                                        $alloc_item->type_paym, $alloc_item->trans_no_paym, $alloc_item->account_associated,
                                        $tax_amount, $alloc_item->minimum_amount, $alloc_item->amount, $alloc_item->serial, $tax_amount, $dscto_amount, $alloc_item->number_key);
                            }
                            $tot_allocated += round($alloc_item->current_allocated, $dec);
                            $tot_tax_amount += round($tax_amount, $dec);
                            $tot_dscto_amount += round($dscto_amount, $dec);
                        }
                
               }
                              
               /*if (price_format($tot_allocated) < price_format($this->amount)){    
                   display_error(_('The process was not executed, Left for allocated').' '. price_format($this->amount - $tot_allocated ).' ---> '.price_format($tot_allocated).' - '.price_format($this->amount));
                   return -1;
               } */
               /*elseif (price_format($tot_allocated) > price_format($this->amount)){
                   display_error(_('The process was not executed, The amount allocated').' '. price_format($this->amount - $tot_allocated )._(" is greater than").' ---> '.price_format($tot_allocated).' - '.price_format($this->amount));
                   return -1;
               }*/ 
               
                $today = date("Y/m/d");
                      
                $next_settlement = get_next_trans_no(ST_SETTLEMENT);
                    
		foreach ($this->allocs as $alloc_item)
		{
			if ($alloc_item->current_allocated > 0)
			{
                            $trans_no = 0;
                            $rows++;
				$amount = round($alloc_item->current_allocated, $dec);
                                $tax_amount = round($alloc_item->tax_amount, $dec);
                                $dsc_amount = round($alloc_item->dscto, $dec);

				if ($this->person_type == PT_SUPPLIER) {
					add_supp_allocation($amount,
						$this->type, $this->trans_no,
			    	 	$alloc_item->type, $alloc_item->type_no, $this->date_);

					update_supp_trans_allocation($alloc_item->type, $alloc_item->type_no);
				} else {
                                    
                                        if ($this->person_type == PT_RESP_PAYMENT){
                                            $bank_tran = $this->trans_no;
                                            //Transaction Payment
                                            $trans_no = write_customer_trans_respons_paym(ST_RESPONSPAYMENT, 0, $alloc_item->debtor_no, $alloc_item->branch_code, $today,
                                            $ref, $amount, 0, 0, 0, 0,
                                            0, 0, 0, "",
                                            $amount, 0, 0, 0, null,
                                            null, '', 0, '', 0,
                                            null, $alloc_item->serial, $alloc_item->account_associated, null,
                                            0, 0, $alloc_item->number_key, 0, $bank_tran, null, $this->cutoff_date, $alloc_item->type_no);
                                            
                                            $branch_data = get_branch_accounts($alloc_item->branch_code);
                                            
                                            add_gl_trans_customer(ST_RESPONSPAYMENT, $trans_no,
                                                    $today, $branch_data["receivables_account"], 0, 0, ($tax_amount * -1),
			                    $alloc_item->debtor_no, "The amount debtor GL posting could not be inserted");                                           
                                            
                                            if ($tax_amount != 0){
                                                add_gl_trans_customer(ST_RESPONSPAYMENT, $trans_no,
                                                    $today, $this->sales_gl_account, 0, 0, ($tax_amount),
			                        $alloc_item->debtor_no, "The tax_amount debtor GL posting could not be inserted");
                                            }
                                            
                                            $default_prompt_payment_act = get_company_pref('default_prompt_payment_act');
                                            
                                            if ($dsc_amount != 0){
                                               add_gl_trans_customer(ST_RESPONSPAYMENT, $trans_no,
                                                    $today, $default_prompt_payment_act, 0, 0, ($dsc_amount),
			                       $alloc_item->debtor_no, "The dsc_amount debtor GL posting could not be inserted");
                                            }
                                            
                                            add_cust_allocation($amount,
						ST_RESPONSPAYMENT, $trans_no,
 			     		    $alloc_item->type, $alloc_item->type_no, $this->date_, $alloc_item->account_associated, null, $alloc_item->number_key, $alloc_item->serial);
                                            
                                            /*$sql = "INSERT INTO ".TB_PREF."settlement_trans_detail
                                            (trans_no, type, amount, dscto_amount, tax_amount, invoice_rel, payment_rel) 
                                            VALUES( ".db_escape($next_settlement).",".ST_SETTLEMENT.",
                                            ".db_escape($amount).",".db_escape($alloc_item->dscto).",".db_escape($alloc_item->tax_amount).",
                                            ".db_escape($alloc_item->type_no).",".db_escape($trans_no).")";

                                            db_query($sql,"settlement_trans_detail could not be added");*/
                                            
                                            //actualiza alloc de cada factura
                                            //update_debtor_trans_allocation_number_key($alloc_item->type, $alloc_item->type_no, $alloc_item->number_key);
                                            update_debtor_trans_allocation_payments_special($alloc_item->type, $alloc_item->type_no, $amount);
                                            //actualiza alloc de cada pago
                                            //update_debtor_trans_respons_allocation($alloc_item->type_paym, $alloc_item->trans_no_paym, $alloc_item->number_key);

                                            /*exchange_variation($alloc_item->type_paym, $alloc_item->trans_no_paym,
                                                                $alloc_item->type, $alloc_item->type_no, $this->date_,
                                                                $amount, $this->person_type);*/                             
                            
                                            
                                             $sql = "INSERT INTO ".TB_PREF."settlement_trans_detail
                                            (trans_no, type, amount, dscto_amount, tax_amount, invoice_rel, payment_rel) 
                                            VALUES( ".db_escape($next_settlement).",".ST_SETTLEMENT.",
                                            ".db_escape($amount).",".db_escape($dsc_amount).",".db_escape($tax_amount).",
                                            ".db_escape($alloc_item->type_no).",".db_escape($trans_no).")";

                                            db_query($sql,"settlement_trans_detail could not be added");
                                        }
                                        else {
	 				add_cust_allocation($amount,
						$this->type, $this->trans_no,
 			     		$alloc_item->type, $alloc_item->type_no, $this->date_);
                                        }
                                        if ($this->person_type != PT_RESP_PAYMENT)
	 				update_debtor_trans_allocation($alloc_item->type, $alloc_item->type_no);
                                        //Si es una nota de credito contra un pago
                                        //busca si es un compromiso particular de una cuenta de un paciente
                                        if ($this->type==ST_CUSTCREDIT &&
                                                have_payment_commitment($alloc_item->type_no, $alloc_item->type) &&
                                                !can_cancel_payment_commitment($alloc_item->type_no, $alloc_item->type, $amount)){
                                            update_cancel_payment_commitment($alloc_item->type, $alloc_item->type_no, $this->trans_no);
                                        }
				}
				// Exchange Variations Joe Hunt 2008-09-20 ////////////////////
                                //display_notification($this->type. ' -    '. $this->trans_no);
                                if ($this->person_type == PT_RESP_PAYMENT && $trans_no != 0){  
                                    //display_notification('trans_no '.$trans_no);
                                    //display_notification('amount '.$alloc_item->amount);
                                    //Se actualiza los datos en memoria ($trans_no) en el objeto para actualizarlo al final
                                    //display_notification('actualiza - '.$trans_no.' - (dscto: '.$alloc_item->dscto.') '.$alloc_item->tax_id.' - '.$alloc_item->number_key);
                                    $this->update_item_data_extra($alloc_item->type, $alloc_item->type_no, 
                                            $alloc_item->date_, $alloc_item->due_date, 
					$alloc_item->amount, $alloc_item->amount_allocated, $alloc_item->current_allocated,
                                            $alloc_item->ref, $alloc_item->debtor_no, $alloc_item->branch_code,
                                            ST_RESPONSPAYMENT, $trans_no, $alloc_item->account_associated, $alloc_item->tax_amount,
                                            $alloc_item->minimum_amount, $alloc_item->amount, $alloc_item->serial,
                                            $alloc_item->tax_id, $alloc_item->dscto, $alloc_item->number_key);
                                    
                                    
                                 
                                }
                                else
				exchange_variation($this->type, $this->trans_no,
					$alloc_item->type, $alloc_item->type_no, $this->date_,
					$amount, $this->person_type);

				//////////////////////////////////////////////////////////////
				$total_allocated += $alloc_item->current_allocated;
                                
                           if ($this->person_type == PT_RESP_PAYMENT) $ref++;
                           
                           $index++;
			}

		}  /*end of the loop through the array of allocations made */
                /*if ($this->person_type == PT_RESP_PAYMENT && $trans_no != 0){
                    update_debtor_trans_allocation(ST_CUSTPAYMENT, $trans_no);
                }*/
                display_notification('total_allocated --> '.$total_allocated);
                
                
		if ($this->person_type == PT_SUPPLIER)
		    update_supp_trans_allocation($this->type, $this->trans_no);
		elseif ($this->person_type == PT_CUSTOMER)
		    update_debtor_trans_allocation($this->type,	$this->trans_no);

                if ($this->person_type == PT_RESP_PAYMENT && $rows > 0){
                    /*$amount = 0;
                    foreach ($this->allocs as $alloc_item)
		    {
                        if (round($alloc_item->current_allocated, $dec) > 0 && $alloc_item->trans_no_paym != 0)
                        $amount = $amount + round($alloc_item->current_allocated, $dec);
                    }*/
                    $bank_tran = $this->trans_no;
                    $amount_acum = get_sum_respons_paym($bank_tran);
                    if ($amount_acum >= $this->limit_allocate ){
                        $Refs->save(ST_RESPONSPAYMENT, 0, $ref);
                        $sql = "UPDATE ".TB_PREF."bank_trans SET used = 1, used_date = Now() WHERE type = ".ST_BANKDEPOSIT." and  trans_no = ".$this->trans_no;
                        db_query($sql, "The debtor transaction record could not be inserted or updated");
                    }
                }
          
                
                if ($this->person_type == PT_RESP_PAYMENT){                    
                    //display_notification(1);
                    
                    foreach ($this->allocs as $alloc_item)
		    {
                        //display_notification(2);
                        $amount = round($alloc_item->current_allocated, $dec);
                        //display_notification('alloc_item->trans_no_paym '.$alloc_item->trans_no_paym);
                        if ($amount > 0 && $alloc_item->trans_no_paym != 0){
                            
                            //actualiza alloc de cada factura
                            //update_debtor_trans_allocation($alloc_item->type, $alloc_item->type_no);
                            //actualiza alloc de cada pago
                            //update_debtor_trans_respons_allocation($alloc_item->type_paym, $alloc_item->trans_no_paym, $alloc_item->number_key);

                            exchange_variation($alloc_item->type_paym, $alloc_item->trans_no_paym,
                                                $alloc_item->type, $alloc_item->type_no, $this->date_,
                                                $amount, $this->person_type); 
                            /*
                            $amount = round($alloc_item->current_allocated, $dec);
                            $tax_amount = round($alloc_item->tax_amount, $dec);
                            $dscto = round($alloc_item->dscto, $dec);  */                         
                           
                            
                            //display_notification('INSERT INTO settlement_trans_detail ( '.$amount.', '.$dscto.', '.$tax_amount).')';    
                            /*$sql = "INSERT INTO ".TB_PREF."settlement_trans_detail
                                            (trans_no, type, amount, dscto_amount, tax_amount, invoice_rel, payment_rel) 
                                            VALUES( ".db_escape($next_settlement).",".ST_SETTLEMENT.",
                                            ".db_escape($amount).",".db_escape($dscto).",".db_escape($tax_amount).",
                                            ".db_escape($alloc_item->type_no).",".db_escape($trans_no).")";

                            db_query($sql,"settlement_trans_detail could not be added");*/
                            
                            
                        }
                    }
                    $bank_tran = $this->trans_no;
                    $user = $_SESSION["wa_current_user"]->username;
                    $branch_code = get_cust_first_branches($this->person_id);
                    $sql = "INSERT INTO ".TB_PREF."settlement_trans
			(trans_no, type, tran_date, debtor_no, branch_code, bank_tran, amount, tax_range, tax_amount, user) 
			VALUES( ".db_escape($next_settlement).",".ST_SETTLEMENT.",
	  		Now(),". db_escape($this->person_id).",".db_escape($branch_code).",".db_escape($bank_tran).","
                        .db_escape($this->amount).",".db_escape($this->tax_amount).","
                        .db_escape($tot_tax_amount).",".db_escape($user).")";

	            db_query($sql,"settlement_trans could not be added");
                    
                    save_next_trans_no(ST_SETTLEMENT, $next_settlement);
                    
                }
                
                commit_transaction();
                
                return $next_settlement;

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
        var $amount_coverage;
	var $ref;
        var $debtor_no;
        var $branch_code;
	
	var $current_allocated;
        
        var $type_paym;
        var $trans_no_paym;
	var $account_associated;
        var $tax_amount;
        var $tot_amount;
        var $minimum_amount;
        var $serial;
        var $tax_id;
        var $dscto;
        var $number_key;
        
	function allocation_item ($type, $type_no, $date_, $due_date, $amount, 
		$amount_allocated, $current_allocated, $ref,
                $debtor_no, $branch_code, $type_paym=0, $trans_no_paym=0, $account_associated=null, $tax_amount, $minimum_amount, $amoun_cobertura, $serial=null, $tax_id, $dscto, $number_key=null)
	{

		$this->type = $type;
		$this->type_no = $type_no;

		$this->ref = $ref;

		$this->date_ = $date_;
		$this->due_date = $due_date;
		
		//$this->amount = $amount;
                $this->amount = $amoun_cobertura;
		$this->amount_allocated = $amount_allocated;
		$this->current_allocated = $current_allocated;
                
                $this->debtor_no = $debtor_no;
                $this->branch_code = $branch_code;
                
                    
                $this->type_paym = $type_paym;
                $this->trans_no_paym = $trans_no_paym;
                
                /*if ($type_paym == 80) {
                    display_notification('trans_no_paym => '.$trans_no_paym);
                }*/
                
                $this->account_associated = $account_associated;
                $this->tax_amount = $tax_amount;
                $this->minimum_amount = $minimum_amount;
                //$this->amount_coverage = $amoun_cobertura;
                $this->amount_coverage = $amount;
                $this->serial = $serial;
                $this->tax_id = $tax_id;
                $this->dscto = $dscto;
                $this->number_key = $number_key;
                
                //display_error('allocation_item '.$this->account_associated.'-'.$this->amount);
               
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
    var $delete = 0;
    //
    function payment_method($line, $type, $type_transac, $amount, $number){
        //display_notification('constructor type: '.$type);
        $this->line = $line;
        $this->type = $type;
        $this->type_trans = $type_transac;
        $this->amount = $amount;
        $this->number = $number;
    }
}
//--------------------------------------------------------------------------------

function show_allocatable($show_totals) {

	global $systypes_array;
	
    $k = $counter = $total_allocated = $tot_final = 0;

	$cart = $_SESSION['alloc'];
	$supp_ref = in_array($cart->type, array(ST_SUPPCREDIT, ST_SUPPAYMENT, ST_BANKPAYMENT));
        //display_error("count item ".count($cart->allocs));
	if (count($cart->allocs)) 
	{
                
                $_POST['minimum_amount'] = price_format($cart->minimum_amount);
            
		if ($cart->currency != $cart->person_curr)
			display_heading(sprintf(_("Allocated amounts in %s:"), $cart->person_curr));
                
                new_amt_extra_cells('tax_base_amount', $cart->tax_amount, 'hidden', 4);
                //_("Transaction Type"), $supp_ref ? _("Supplier Ref"): _("Ref") _("Account Assoc")
		start_table(TABLESTYLE, "width=60%");
   		$th = array( _("Serial"), _("#"), _("Customer"), _("Date"), _("Amount"), _("Amt Coverage"),
   			_("Other Allocations"), _("Left to Allocate"), _("This Allocation"),
                    _("Dsct"), _("Tax"),_("Total"),'','');

	   	table_header($th);

		foreach ($cart->allocs as $id => $alloc_item)
		{   //display_error('alloc_item '.$alloc_item->account_associated.'-'.$alloc_item->amount.'-'.$alloc_item->amount_allocated);
		    if (floatcmp2(abs($alloc_item->amount), $alloc_item->amount_allocated))
		    {
			alt_table_row_color($k);
    			//label_cell($systypes_array[$alloc_item->type]);
                        label_cell($alloc_item->serial);
	   		label_cell(get_trans_view_str($alloc_item->type, $alloc_item->type_no));
                        
                        $account = get_trans_view_str(ST_PATIENT_ACCOUNT, $alloc_item->account_associated,"", false, 
	'', '', 0, 0, $alloc_item->tax_id);
                        
		   	label_cell($account . ' - '.get_customer_name($alloc_item->debtor_no));
                        //label_cell($alloc_item->account_associated);
                        //label_cell($alloc_item->ref);
    			label_cell($alloc_item->date_, "align=right");
    			//label_cell($alloc_item->due_date, "align=right");                        
                        amount_cell(abs($alloc_item->amount_coverage));
	    		amount_cell(abs($alloc_item->amount));
				$amt_allocated = $alloc_item->amount_allocated;
				if ($amt_allocated < 0) $amt_allocated = $amt_allocated * -1;
				if ($amt_allocated > $alloc_item->amount)
				$amt_allocated  = $alloc_item->amount;
			    amount_cell(abs($amt_allocated));

                      
		    	$_POST['amount' . $id] = price_format($alloc_item->current_allocated);
                        //$_POST['tax_amount' . $id] = price_format($alloc_item->tax_amount);
                        if (!isset($_POST['descuento_amount' . $id]))
                        $_POST['descuento_amount' . $id] = price_format(0);
                        if (!isset($_POST['tax_amount' . $id]))
                        $_POST['tax_amount' . $id] = price_format(0);
                        //$tax_amount = $_POST['tax_amount' . $id];                        
                            
                        $_POST['tot_amount' . $id] = price_format($alloc_item->tot_amount);

	    		$un_allocated = round((abs($alloc_item->amount) - $alloc_item->amount_allocated), 6);
				if ($un_allocated < 0) $un_allocated = $un_allocated * -1;
				if ($un_allocated > $alloc_item->amount)
				$un_allocated  = $alloc_item->amount;
	    		amount_cell($un_allocated, false,'', 'maxval'.$id);
    			amount_cells(null, "amount" . $id);//, input_num('amount' . $id));
                        new_amount_cells(null, "descuento_amount" . $id,  null, null, null, null, null, 'descuento_amount');//, input_num('descuento_amount' . $id));                        
                        new_amount_cells(null, "tax_amount" . $id,  null, null, null, null, null, 'tax_amount');//, input_num('tax_amount' . $id));
                        new_amount_cells(null, "tot_amount" . $id, null, null, null, null, null, 'tot_amount');//, input_num('tot_amount' . $id));
			/*label_cell("<a href='#' name=Alloc$id onclick='allocate_all(this.name.substr(5));return true;'>"
					 . _("All") . "</a>");*/
			label_cell("<a href='#' name=DeAll$id onclick='allocate_none(this.name.substr(5));return true;'>"
					 . _("None") . "</a>".hidden("un_allocated" . $id, 
			price_format($un_allocated), false));
                        
                        echo '<td>'.print_report_invoice_link($alloc_item->type_no.'-'.ST_SALESINVOICE, _("Print"), true, $alloc_item->type_no, ICON_PRINT).'</td>';
                       
                        new_amt_extra_cells('minimum_amount'. $id, $alloc_item->minimum_amount, 'hidden', 4);
                                
			end_row();

   	    		$total_allocated += input_num('amount' . $id);
                        $tot_final += input_num('tot_amount' . $id);
		   	}
		}
                //display_notification($tot_final);
                $extra["name"] = 'total_final';
                $extra["value"] = price_format($tot_final);
                $extra["colspan_after"] = 2;
                
		if ($show_totals) {
    	   	label_row(_("Total Allocated"), price_format($total_allocated),
	    		"colspan=8 align=right", "align=right id='total_allocated'", 2, null, $extra);
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
                        $extra["name"] = '';
                        $extra["colspan_after"] = 2;
	        label_row(_("Left to Allocate"), $font1 . $left_to_allocate . $font2, 
				"colspan=8 align=right", "nowrap align=right id='left_to_allocate'",
				 3, null, $extra);
		}
		end_table(1);
	}
        
        
	hidden('TotalNumberOfAllocs', count($cart->allocs));
}
//--------------------------------------------------------------------------------

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
