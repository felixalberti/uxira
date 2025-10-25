<?php
/**********************************************************************
    Copyright (C) FrontAccounting, LLC.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
$page_security = 'SA_VOIDTRANSACTION';
$path_to_root = "..";
include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

include_once($path_to_root . "/includes/date_functions.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/data_checks.inc");
include_once($path_to_root . "/admin/db/transactions_db.inc");

include_once($path_to_root . "/admin/db/voiding_db.inc");
$js = "";
if ($use_date_picker)
	$js .= get_js_date_picker();
if ($use_popup_windows)
	$js .= get_js_open_window(800, 500);
	
page(_($help_context = "Void a Transaction"), false, false, "", $js);


simple_page_mode(true);
//----------------------------------------------------------------------------------------
function exist_transaction($type, $type_no)
{
	$void_entry = get_voided_entry($type, $type_no);

	if ($void_entry != null)
		return false;

	switch ($type) 
	{
		case ST_JOURNAL : // it's a journal entry
			if (!exists_gl_trans($type, $type_no))
				return false;
			break;

		case ST_BANKPAYMENT : // it's a payment
		case ST_BANKDEPOSIT : // it's a deposit
		case ST_BANKTRANSFER : // it's a transfer
			if (!exists_bank_trans($type, $type_no))
				return false;
			break;

		case ST_SALESINVOICE : // it's a customer invoice
		case ST_CUSTCREDIT : // it's a customer credit note
		case ST_CUSTPAYMENT : // it's a customer payment
		case ST_CUSTDELIVERY : // it's a customer dispatch
			if (!exists_customer_trans($type, $type_no))
				return false;
			break;

		case ST_LOCTRANSFER : // it's a stock transfer
			if (get_stock_transfer_items($type_no) == null)
				return false;
			break;

		case ST_INVADJUST : // it's a stock adjustment
			if (get_stock_adjustment_items($type_no) == null)
				return false;
			break;

		case ST_PURCHORDER : // it's a PO
			return false;

		case ST_SUPPRECEIVE : // it's a GRN
			if (exists_grn_on_invoices($type_no))
				return false;
			break;

		case ST_SUPPINVOICE : // it's a suppler invoice
		case ST_SUPPCREDIT : // it's a supplier credit note
		case ST_SUPPAYMENT : // it's a supplier payment
			if (!exists_supp_trans($type, $type_no))
				return false;
			break;

		case ST_WORKORDER : // it's a work order
			if (!get_work_order($type_no, true))
				return false;
			break;

		case ST_MANUISSUE : // it's a work order issue
			if (!exists_work_order_issue($type_no))
				return false;
			break;

		case ST_MANURECEIVE : // it's a work order production
			if (!exists_work_order_produce($type_no))
				return false;
			break;

		case ST_SALESORDER: // it's a sales order
		case ST_SALESQUOTE: // it's a sales quotation
			return false;
		case ST_COSTUPDATE : // it's a stock cost update
			return false;
			break;
	}

	return true;
}

function view_link($trans)
{
	if (!isset($trans['type']))
		$trans['type'] = $_POST['filterType'];
	return get_trans_view_str($trans["type"], $trans["trans_no"]);
}

function select_link($row)
{
	if (!isset($row['type']))
		$row['type'] = $_POST['filterType'];
	if (!is_date_in_fiscalyear($row['trans_date'], true))
		return _("No");
  	return button('Edit'.$row["trans_no"], _("Select"), _("Select"), ICON_EDIT);
}

function gl_view($row)
{
	if (!isset($row['type']))
		$row['type'] = $_POST['filterType'];
	return get_gl_view_str($row["type"], $row["trans_no"]);
}

function date_view($row)
{
	return $row['trans_date'];
}

function ref_view($row)
{
	return $row['ref'];
}

function amount_view($row)
{
	return $row['ov_amount'];
}

function serial_view($row)
{
	return $row['serial'];
}

function user_view($row)
{
	return $row['user'];
}

function voiding_controls()
{
	global $selected_id;

	$not_implemented =  array(ST_PURCHORDER, ST_SALESORDER, ST_SALESQUOTE, ST_COSTUPDATE);

	start_form();

    start_table(TABLESTYLE_NOBORDER);
	start_row();

	systypes_list_cells(_("Type:"), 'filterType', null, true, $not_implemented, _("Select a transacction type"));
	if (list_updated('filterType'))
		$selected_id = -1;

	if (!isset($_POST['FromTransNo']))
        $_POST['FromTransNo'] = "1";
    if (!isset($_POST['ToTransNo']))
        $_POST['ToTransNo'] = "999999";

    ref_cells(_("from #:"), 'FromTransNo');

    ref_cells(_("to #:"), 'ToTransNo');

    submit_cells('ProcessSearch', _("Search"), '', '', 'default');
		
	end_row();
    end_table(1);
    
	$trans_ref = false;
	$sql = get_sql_for_view_transactions($_POST['filterType'], $_POST['FromTransNo'], $_POST['ToTransNo'], $trans_ref);
        //display_notification($sql);
	if ($sql == "")
		return;
        if($_POST['filterType']==ST_SALESINVOICE)
	$cols = array(
		_("#") => array('insert'=>true, 'fun'=>'view_link'), 
		_("Reference") => array('fun'=>'ref_view'), 
		_("Date") => array('type'=>'date', 'fun'=>'date_view'),
		_("GL") => array('insert'=>true, 'fun'=>'gl_view'),
                _("Amount") => array('type'=>'amount', 'fun'=>'amount_view'),
                _("Serial") => array('align'=>'center', 'fun'=>'serial_view'),
                _("User") => array('align'=>'center', 'fun'=>'user_view'),
		_("Select") => array('insert'=>true, 'fun'=>'select_link') 
	);
        elseif($_POST['filterType']==ST_BANKDEPOSIT)
	$cols = array(
		_("#") => array('insert'=>true, 'fun'=>'view_link'), 
		_("Reference") => array('fun'=>'ref_view'), 
		_("Date") => array('type'=>'date', 'fun'=>'date_view'),
		_("GL") => array('insert'=>true, 'fun'=>'gl_view'),
                _("Amount") => array('type'=>'amount', 'fun'=>'amount_view'),
		_("Select") => array('insert'=>true, 'fun'=>'select_link') 
	);
        elseif($_POST['filterType']==ST_CUSTDELIVERY)
	$cols = array(
		_("#") => array('insert'=>true, 'fun'=>'view_link'), 
		_("Reference") => array('fun'=>'ref_view'), 
		_("Date") => array('type'=>'date', 'fun'=>'date_view'),
		_("GL") => array('insert'=>true, 'fun'=>'gl_view'),
                _("Amount") => array('type'=>'amount', 'fun'=>'amount_view'),
                _("User") => array('align'=>'center', 'fun'=>'user_view'),
		_("Select") => array('insert'=>true, 'fun'=>'select_link') 
	);
        else
            $cols = array(
		_("#") => array('insert'=>true, 'fun'=>'view_link'), 
		_("Reference") => array('fun'=>'ref_view'), 
		_("Date") => array('type'=>'date', 'fun'=>'date_view'),
		_("GL") => array('insert'=>true, 'fun'=>'gl_view'),
		_("Select") => array('insert'=>true, 'fun'=>'select_link') 
	);

	$table =& new_db_pager('transactions', $sql, $cols);
	$table->width = "40%";
	display_db_pager($table);

	start_table(TABLESTYLE2);

	if ($selected_id != -1)
	{
		hidden('trans_no', $selected_id);
		hidden('selected_id', $selected_id);
	}
	else
	{
		hidden('trans_no', '');
		$_POST['memo_'] = '';
	}
        
    br();    
        
    label_row(_("Transaction #:"), ($selected_id==-1?'':$selected_id));

    date_row(_("Voiding Date:"), 'date_');

    textarea_row(_("Memo:"), 'memo_', null, 30, 4);

	end_table(1);

    if (!isset($_POST['ProcessVoiding']))
    	submit_center('ProcessVoiding', _("Void Transaction"), true, '', 'default');
    else 
    {
 		if (!exist_transaction($_POST['filterType'],$_POST['trans_no']))
 		{
			display_error(_("The entered transaction does not exist or cannot be voided."));
			unset($_POST['trans_no']);
			unset($_POST['memo_']);
			unset($_POST['date_']);
    		submit_center('ProcessVoiding', _("Void Transaction"), true, '', 'default');
		}	
 		else
 		{
    		display_warning(_("Are you sure you want to void this transaction ? This action cannot be undone."), 0, 1);
   			br();
    		submit_center_first('ConfirmVoiding', _("Proceed"), '', true);
    		submit_center_last('CancelVoiding', _("Cancel"), '', 'cancel');
    	}	
    }

	end_form();
}

//----------------------------------------------------------------------------------------

function check_valid_entries()
{
	if (is_closed_trans($_POST['filterType'],$_POST['trans_no']))
	{
		display_error(_("The selected transaction was closed for edition and cannot be voided."));
		set_focus('trans_no');
		return false;
	}
	if (!is_date($_POST['date_']))
	{
		display_error(_("The entered date is invalid."));
		set_focus('date_');
		return false;
	}
	if (!is_date_in_fiscalyear($_POST['date_']))
	{
		display_error(_("The entered date is not in fiscal year."));
		set_focus('date_');
		return false;
	}

	if (!is_numeric($_POST['trans_no']) OR $_POST['trans_no'] <= 0)
	{
		display_error(_("The transaction number is expected to be numeric and greater than zero."));
		set_focus('trans_no');
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------

function handle_void_transaction()
{
	if (check_valid_entries()==true) 
	{
		$void_entry = get_voided_entry($_POST['filterType'], $_POST['trans_no']);
		if ($void_entry != null) 
		{
			display_error(_("The selected transaction has already been voided."), true);
			unset($_POST['trans_no']);
			unset($_POST['memo_']);
			unset($_POST['date_']);
			set_focus('trans_no');
			return;
		}
                
                //Begin Felix Alberti 08/02/2017
                if ($_POST['filterType']==ST_CUSTPAYMENT || $_POST['filterType']==ST_BANKDEPOSIT){
                    $type = $_POST['filterType'];                                                          
                    $sql = "SELECT ov_amount+ov_gst+ov_freight+ov_freight_tax+ov_discount AS Total";	
	            $sql .= " FROM ".TB_PREF."debtor_trans trans WHERE type = ".$type." and trans_no = ".$_POST['trans_no'];
                    $result_trans = db_query($sql, "The account_associated in transaction record could not be retrieved");
                    $row_trans = db_fetch($result_trans);
                    $amount = $row_trans['Total'];
                }
                //End Felix Alberti 08/02/2017
                //Aqui se llama a la función que anula los registros originales
		$ret = void_transaction($_POST['filterType'], $_POST['trans_no'],
			$_POST['date_'], $_POST['memo_']);

		if ($ret) 
		{       //Begin Felix Alberti 23/11/20016
                        if ($_POST['filterType']==ST_CUSTDELIVERY || $_POST['filterType']==ST_SALESINVOICE || $_POST['filterType']==ST_CUSTPAYMENT || $_POST['filterType']==ST_BANKDEPOSIT){
                            $sql = "SELECT account_associated, order_ FROM ".TB_PREF."debtor_trans ".
                                   "WHERE type = ".$_POST['filterType']." and trans_no = ".$_POST['trans_no'];
                            $result = db_query($sql, "The account_associated in transaction record could not be retrieved");
                            $row = db_fetch($result);
                        }
                        if ($_POST['filterType']==ST_CUSTDELIVERY){
                            if (isset($row['account_associated'])){
                               $account_assoc = $row['account_associated'];
                               /*$sql = "UPDATE ".TB_PREF."debtor_trans set account_associated = 0 "
                                    . "WHERE type = ".ST_CUSTDELIVERY." and trans_no = ".$_POST['trans_no'];
                               db_query($sql, "The account_associated in delivery transaction record could not be updated");*/
                               //
                               /*$sql = "UPDATE ".TB_PREF."sales_orders set account_associated = 0 "
                                    . "WHERE trans_type = ".ST_SALESORDER." and order_no = ".$row['order_'];
                               db_query($sql, "The account_associated in sales_orders record could not be updated");*/
                               //
                               $user = "; User: ".$_SESSION["wa_current_user"]->username;
                               add_audit_trail(ST_CUSTDELIVERY, $_POST['trans_no'], $_POST['date_'], _("Voided Account Assoc.")."\n".$account_assoc.$user);
                               add_audit_trail(ST_SALESORDER, $row['order_'], $_POST['date_'], _("Voided Account Assoc.")."\n".$account_assoc.$user);
                            }
                        }
                        elseif ($_POST['filterType']==ST_SALESINVOICE){
                            if (isset($row['account_associated'])){
                                
                                /*Begin Felix Alberti 05/04/2017*/
                                /*Para corregir las deliveries que estaban asociadas a la factura o cuenta luego
                                de agruparlas cada linea por item y precio se perdía el enlace de las que quedaba
                                (ya manaje una sola linea por item y precio igual)
                                fuera del grupo y deben regresar a su condición original antes de ser facturadas */
                                //
                                $myrow = get_customer_trans_invoice($_POST['trans_no'],ST_SALESINVOICE);
                                if ($myrow){
                                    begin_transaction();
                                    post_void_invoice_from_account($myrow['account_associated'],$myrow['ref_account']);
                                    commit_transaction();
                                }
                                /*End Felix Alberti 05/04/2017*/ 
                                
                               $account_assoc = $row['account_associated'];
                               /*$sql = "UPDATE ".TB_PREF."debtor_trans set account_associated = 0 "
                                    . "WHERE type = ".ST_SALESINVOICE." and trans_no = ".$_POST['trans_no'];
                               db_query($sql, "The account_associated in invoice transaction record could not be updated");*/
                               //
                               $user = "; User: ".$_SESSION["wa_current_user"]->username;
                               add_audit_trail(ST_SALESINVOICE, $_POST['trans_no'], $_POST['date_'], _("Voided Account Assoc.")."\n".$account_assoc.$user);
                            }
                             
                        }
                        //Begin Felix Alberti 08/02/2017
                        elseif ($_POST['filterType']==ST_CUSTPAYMENT || $_POST['filterType']==ST_BANKDEPOSIT){
                            
                            if (isset($row['account_associated'])){
                                $account_assoc = $row['account_associated'];                               
                                
                                $type = $_POST['filterType'];
                                if (isset($row_trans['Total'])){                                  

                                    if ($account_assoc=='0' || $account_assoc=='' || $account_assoc===NULL){
                                        $sql = "UPDATE ".TB_PREF."payer_trans_detail set inactive = 1, amount = 0, balance = 0, "
                                                . "user_cancel = ".db_escape($_SESSION["wa_current_user"]->username).", date_cancel = Now() "
                                            . "WHERE trans_no_rel = ".$_POST['trans_no']." and type_rel = ".$type; 
                                        $result = db_query($sql, "The account_associated in transaction record could not be updated");
                                        
                                        $sql = "SELECT id_account FROM ".TB_PREF."payer_trans_detail ".
                                        "WHERE type_rel = ".$_POST['filterType']." and trans_no_rel = ".$_POST['trans_no'];
                                        $result_pay_det = db_query($sql, "The payer_trans_detail in transaction record could not be retrieved");
                                        $row_pay_det = db_fetch($result_pay_det);
                                        $account_assoc = $row_pay_det['id_account']; 
                                                                        }
                                    else  {  
                                    $sql = "UPDATE ".TB_PREF."payer_trans_detail set inactive = 1, amount = 0, balance = 0, "
                                            . "user_cancel = ".db_escape($_SESSION["wa_current_user"]->username).", date_cancel = Now() "
                                        . "WHERE id_account = ".db_escape($account_assoc)." and trans_no_rel = ".$_POST['trans_no']." and type_rel = ".$type;
                                    db_query($sql, "The payer_trans_detail transaction record could not be updated");
                                    }
                                    
                                    if ($account_assoc!='0' && $account_assoc!='' && $account_assoc!=NULL){                                    
                                        $sql = "UPDATE ".TB_PREF."payer_trans set paid = paid - $amount, balance = balance + $amount, pending = 1, user_update =  ".db_escape($_SESSION["wa_current_user"]->username)
                                            . " WHERE id_account = ".db_escape($account_assoc);
                                        db_query($sql, "The payer_trans transaction record could not be updated");

                                       $sql = "UPDATE ".TB_PREF."patient_accounts set particular_amount = particular_amount - $amount,"
                                               . " dif_against_account = account_amount - $amount, dif_against_quotation = order_amount - $amount "
                                            . "WHERE id = ".db_escape($account_assoc);
                                       db_query($sql, "The account_associated in invoice transaction record could not be updated");
                                   }
                                   
                                   $sql = "UPDATE ".TB_PREF."cust_allocations SET amt = 0"
                                            . " WHERE trans_type_from in ( ".ST_CUSTPAYMENT.",".ST_BANKDEPOSIT.") and trans_no_from = ".$_POST['trans_no']
                                            . " and trans_type_to = ".ST_PATIENT_ACCOUNT;
                                       db_query($sql, "cust_allocations transaction record could not be updated");
                                   
                                   //
                                   $user = "; User: ".$_SESSION["wa_current_user"]->username;
                                   add_audit_trail($type, $_POST['trans_no'], $_POST['date_'], _("Voided Account Assoc.")."\n".$account_assoc.$user." "._("Amount:")." ".$amount);
                               }
                            }
                            
                        }
                        //End Felix Alberti 08/02/2017
                        //End Felix Alberti 23/11/20016
			display_notification_centered(_("Selected transaction has been voided."));
			unset($_POST['trans_no']);
			unset($_POST['memo_']);
			unset($_POST['date_']);
		}
		else {
			display_error(_("The entered transaction does not exist or cannot be voided."));
			set_focus('trans_no');

		}
	}
}

//----------------------------------------------------------------------------------------

if (!isset($_POST['date_']))
{
	$_POST['date_'] = Today();
	if (!is_date_in_fiscalyear($_POST['date_']))
		$_POST['date_'] = end_fiscalyear();
}		
	
if (isset($_POST['ProcessVoiding']))
{
	if (!check_valid_entries())
		unset($_POST['ProcessVoiding']);
	$Ajax->activate('_page_body');
}

if (isset($_POST['ConfirmVoiding']))
{
	handle_void_transaction();
	$Ajax->activate('_page_body');
}

if (isset($_POST['CancelVoiding']))
{
	$selected_id = -1;
	$Ajax->activate('_page_body');
}

//----------------------------------------------------------------------------------------

voiding_controls();

end_page();

?>