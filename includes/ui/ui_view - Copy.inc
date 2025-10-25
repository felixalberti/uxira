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
include_once($path_to_root . "/admin/db/voiding_db.inc");
include_once($path_to_root . "/includes/types.inc");

//--------------------------------------------------------------------------------------

function get_supplier_trans_view_str($type, $trans_no, $label="", $icon=false, 
	$class='', $id='')
{
	$viewer = "purchasing/view/";
	if ($type == ST_PURCHORDER)
		$viewer .= "view_po.php";
	elseif ($type == ST_SUPPINVOICE)
		$viewer .= "view_supp_invoice.php";
	elseif ($type == ST_SUPPCREDIT)
		$viewer .= "view_supp_credit.php";
	elseif ($type == ST_SUPPAYMENT)
		$viewer .= "view_supp_payment.php";
	elseif ($type == ST_SUPPRECEIVE)
		$viewer .= "view_grn.php";
	else
		return null;
	$viewer .= "?trans_no=$trans_no";

	if ($label == "")
		$label = $trans_no;

	return viewer_link($label, $viewer, $class, $id,  $icon);
}

//--------------------------------------------------------------------------------------

function get_gl_view_str($type, $trans_no, $label="", $force=false, $class='', $id='')
{
	if (!$force && !user_show_gl_info())
		return "";

	$icon = false;
	if ($label == "")
	{
		$label = _("GL");
		$icon = ICON_GL;
	}	

	return viewer_link($label, 
		"gl/view/gl_trans_view.php?type_id=$type&trans_no=$trans_no", 
		$class, $id, $icon);
}

//--------------------------------------------------------------------------------------

function get_gl_view_str_cell($type, $trans_no, $label="")
{
	$str = get_gl_view_str($type, $trans_no, $label);
	if ($str != "")
		return "<td>$str</td>";
}

//--------------------------------------------------------------------------------------

function get_customer_trans_view_str($type, $trans_no, $label="", $icon=false, 
	$class='', $id='', $without_delivery=0, $ci=0)
{
	$viewer = "sales/view/";

	if ($type == ST_SALESINVOICE){                
	        $viewer .= "view_invoice.php";
        }
        elseif ($type == ST_CUSTDEBIT){                
	        $viewer .= "view_debit_note.php";
        }
	elseif ($type == ST_CUSTCREDIT)
		$viewer .= "view_credit.php";
	elseif ($type == ST_CUSTPAYMENT)
		$viewer .= "view_receipt.php";
	elseif ($type == ST_CUSTDELIVERY)
		$viewer .= "view_dispatch.php";
	elseif ($type == ST_SALESORDER || $type == ST_SALESQUOTE)
		$viewer .= "view_sales_order.php";
        elseif ($type == ST_PAYACCOUNTPATIENT)//Felix Alberti 28/12/2015
		$viewer .= "view_pay_patient_account.php";//Felix Alberti 28/12/2015
        elseif ($type === ST_SALESINVOICE_FROM_ACCOUNT)
                $viewer .= "view_invoice_from_account.php";//Felix Alberti 11/05/2016
        elseif ($type === ST_PATIENT_ACCOUNT){
                $viewer = "sales/inquiry/";
                $viewer .= "patient_accounts.php";//Felix Alberti 27/10/2016
        }        
	else
		return null;

  if(!is_array($trans_no)) $trans_no = array($trans_no);

  $lbl = $label;
  $preview_str = '';

  foreach($trans_no as $trans) {
	if ($label == "")
		$lbl = $trans;
	if($preview_str!='') $preview_str .= ',';
        
        if ($type === ST_PATIENT_ACCOUNT){
            $preview_str .= viewer_link($lbl, $viewer."?view=1&ci=$ci&account=$trans&buscarcuenta=1", 
                    $class, $id, $icon);
        }
        else {
            if ($without_delivery==1)
            $preview_str .= viewer_link($lbl, $viewer."?without_delivery=$without_delivery&trans_no=$trans&trans_type=$type", 
                    $class, $id, $icon);
            else
            $preview_str .= viewer_link($lbl, $viewer."?trans_no=$trans&trans_type=$type", 
                    $class, $id, $icon);
        }

  }
  return $preview_str;
}

function get_print_fiscal_view_str($type, $trans_no, $label="", $icon=false, 
	$class='', $id='', $invoicecancel)
{
	$viewer = "reporting/";

	if ($type == ST_SALESINVOICE){                
	        $viewer .= "imp_fiscal.php";
        } 
        elseif ($type == ST_CUSTCREDIT){                
	        $viewer .= "imp_fiscal.php";
        }   
	else
		return null;

  if(!is_array($trans_no)) $trans_no = array($trans_no);

  $lbl = $label;
  $preview_str = '';

  foreach($trans_no as $trans) {
	if ($label == "")
		$lbl = $trans;
	if($preview_str!='') $preview_str .= ',';
        
            if ($type == ST_CUSTCREDIT)
            $preview_str .= viewer_link($lbl, $viewer."?trans_no=$trans&trans_type=$type&AddedID=$trans&InvoiceCancel=$invoicecancel", 
                    $class, $id, $icon);
            else    
            $preview_str .= viewer_link($lbl, $viewer."?trans_no=$trans&trans_type=$type&AddedDI=$trans", 
                    $class, $id, $icon);
       

  }
  return $preview_str;
}

/*Begin Felix Alberti 08/07/2015*/
function get_medico_trans_view_str($label="", $prof, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/view/";

	$viewer .= "view_medico_invoice.php";	


  //if(!is_array($trans_no)) $trans_no = array($trans_no);

  $lbl = $label;
  $preview_str = '';

//  foreach($trans_no as $trans) {
//	if ($label == "")
//		$lbl = $trans;
//	if($preview_str!='') $preview_str .= ',';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer."?profesional_id=$prof", 
		$class, $id, $icon);

  //}
  return $preview_str;
}
/*End Felix Alberti 08/07/2015*/

/*Begin Felix Alberti 13/07/2015*/
function get_medico_pagos_a_medicos_view_str($label="", $ref, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/";

	$viewer .= "pagos_a_medicos.php";	


  //if(!is_array($trans_no)) $trans_no = array($trans_no);

  $lbl = $label;
  $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer."?ref=$ref", 
		$class, $id, $icon);

  //}
  return $preview_str;
}
/*End Felix Alberti 13/07/2015*/

/*Begin Felix Alberti 22/07/2015*/
function get_professional_trans_view_str($type, $trans_no, $label="", $icon=false, 
	$class='', $id='', $prenomina=0)
{
	$viewer = "sales/view/";

	if ($type == ST_LIQUIDAMED)
		$viewer .= "view_medico_pago.php";
	else
		return null;

  if(!is_array($trans_no)) $trans_no = array($trans_no);

  $lbl = $label;
  $preview_str = '';

  foreach($trans_no as $trans) {
	if ($label == "")
		$lbl = $trans;
	if($preview_str!='') $preview_str .= ',';

	$preview_str .= viewer_link($lbl, $viewer."?trans_no=$trans&trans_type=$type&prenomina=$prenomina", 
		$class, $id, $icon);

  }

  return $preview_str;
}
/*End Felix Alberti 22/07/2015*/

/*Begin Felix Alberti 28/12/2015*/
function get_prefactura_view_str($label="", $debtor, $account,  $class_=7, $rep=801, $win=1, $icon=false, 
	$class='', $id='')
{
	$viewer = "reporting/";

	$viewer .= "reports_main.php";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer."?Class=".$class_."&REP_ID=".$rep."&Window=".$win."&PARAM_2=".$debtor."&PARAM_3=".$account, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 28/12/2015*/

/*Begin Felix Alberti 26/06/2017*/
function get_quotation_account_view_str($label="", $debtor, $account,  $class_=7, $rep=802, $win=1, $icon=false, 
	$class='', $id='')
{
	$viewer = "reporting/";

	$viewer .= "reports_main.php";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer."?Class=".$class_."&REP_ID=".$rep."&Window=".$win."&PARAM_0=".$debtor."&PARAM_1=".$account, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 26/06/2017*/

/*Begin Felix Alberti 30/06/2017*/
function get_report_status_account_view_str($label="", $account, $id_sec_resp_paym, $id_status,  $class_=7, $rep=803, $win=1, $icon=false, 
	$class='', $id='')
{
	$viewer = "reporting/";

	$viewer .= "reports_main.php";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer."?Class=".$class_."&REP_ID=".$rep."&Window=".$win."&PARAM_0=".$account."&PARAM_1=".$id_sec_resp_paym."&PARAM_2=".$id_status, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 30/06/2017*/

/*Begin Felix Alberti 24/10/2017*/
function get_report_invoice_view_str($label="", $tran_no, $class_=0, $rep='107_uxira', $win=1, $icon=false, 
	$class='', $id='')
{
	$viewer = "reporting/";

	$viewer .= "reports_main_especial1.php";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer."?Class=".$class_."&REP_ID=".$rep."&Window=".$win."&PARAM_0=".$tran_no."&PARAM_1=".$tran_no, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 24/10/2017*/

/*Begin Felix Alberti 09/01/2016*/
function get_sales_orders_view_str($label="", $cust_id, $dimension_id, $account, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/inquiry/";

	$viewer .= "sales_orders_view_cons.php?type=".ST_CUSTDELIVERY."&selected_customer=$cust_id&dimension_id=$dimension_id&call_external=1&account=$account";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 09/01/2016*/

/*Begin Felix Alberti 19/04/2016*/
function get_customer_payment_view_str($label="", $cust_id, $filtertype, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/inquiry/";
        
        if ($filtertype==6)
        $viewer .= "inquiry_authorizations_credits.php?customer_id=$cust_id&filterType=$filtertype";
        else
	$viewer .= "inquiry_customer_payments.php?customer_id=$cust_id&filterType=$filtertype";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter_timer($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 19/04/2016*/

/*Begin Felix Alberti 01/05/2016*/
function get_customer_transactions_view_str($label="", $cust_id, $filtertype, $bank_id, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/inquiry/";
      
	$viewer .= "inquiry_customer_transactions.php?customer_id=$cust_id&filterType=$filtertype&bank_id=$bank_id";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter_timer($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 01/052016*/

/*Begin Felix Alberti 11/01/2016*/
function get_delivery_custom_view_str($label="", $cust_id, $account, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/inquiry/";

	$viewer .= "sales_orders_view_cons.php?type=".ST_CUSTDELIVERY."&selected_customer=$cust_id&call_external=1&account=$account";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 11/01/2016*/

/*Begin Felix Alberti 23/03/2016*/
function get_order_custom_without_delivery_order_view_str($label="", $cust_id, $account, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/inquiry/";
        $withoutorder = 1;
	$viewer .= "sales_orders_view_cons.php?type=".ST_SALESORDER."&selected_customer=$cust_id&withoutorder=$withoutorder&call_external=1&account=$account";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 23/03/2016*/

/*Begin Felix Alberti 18/08/2017*/
function get_order_custom_delivery_vs_order_view_str($label="", $cust_id, $account, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/inquiry/";
        $dif = 1;
	$viewer .= "sales_orders_view_cons.php?type=".ST_SALESORDER."&selected_customer=$cust_id&dif=$dif&call_external=1&account=$account";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 18/08/2017*/

/*Begin Felix Alberti 25/04/2016*/
function get_find_custom_view_str($label="", $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/inquiry/";

	$viewer .= "find_debtor.php?";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter_timer($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 25/04/2016*/

/*Begin Felix Alberti 26/09/2016*/
function get_prices_inventory_view_str($label="", $without_menu, $stock_id, $icon=false, 
	$class='', $id='')
{
	$viewer = "inventory/";

	$viewer .= "prices.php?without_menu=".$without_menu."&stock_id=".$stock_id;

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 26/09/2016*/

/*Begin Felix Alberti 04/05/2016*/
function get_request_authoriz_view_str($label="", $account, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/inquiry/";

	$viewer .= "request_authoriz.php?account=".$account;

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 04/05/2016*/

/*Begin Felix Alberti 27/04/2017*/
function get_request_authoriz_uncheck_order_view_str($label="", $account, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/inquiry/";

	$viewer .= "request_authoriz_uncheck_order.php?account=".$account;

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 27/04/2017*/

/*Begin Felix Alberti 16/06/2016*/
function get_items_stock_view_str($label="", $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/manage/";

	$viewer .= "find_item_stock.php";

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter_timer($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 16/06/2016*/

/*Begin Felix Alberti 25/04/2016*/
function get_custom_view_str($label="", $debtor_no, $icon=false, 
	$class='', $id='')
{
	$viewer = "sales/manage/";

	$viewer .= "customers.php?withoutmenu=1&debtor_no=".$debtor_no;

        $lbl = $label;
        $preview_str = '';

	$preview_str .= viewer_link_ret_parameter($lbl, $viewer, 
		$class, $id, $icon);

  return $preview_str;
}
/*End Felix Alberti 25/04/2016*/

//--------------------------------------------------------------------------------------

function get_banking_trans_view_str($type, $trans_no, $label="", 
	$icon=false, $class='', $id='')
{
	if ($label == "")
		$label = $trans_no;

	if ($type == ST_BANKTRANSFER)
		$viewer = "bank_transfer_view.php";
	elseif ($type == ST_BANKPAYMENT)
		$viewer = "gl_payment_view.php";
	elseif ($type == ST_BANKDEPOSIT)
		$viewer = "gl_deposit_view.php";
	else
		return null;

	return viewer_link($label, "gl/view/$viewer?trans_no=$trans_no", 
		$class, $id,  $icon);
}

//--------------------------------------------------------------------------------------

function get_inventory_trans_view_str($type, $trans_no, $label="", 
	$icon=false, $class='', $id='')
{
	$viewer = "inventory/view/";

	if ($type == ST_INVADJUST)
		$viewer .= "view_adjustment.php";
	elseif ($type == ST_LOCTRANSFER)
		$viewer .= "view_transfer.php";
	else
		return null;
	$viewer .= "?trans_no=$trans_no";

	if ($label == "")
		$label = $trans_no;

	return viewer_link($label, $viewer, $class, $id,  $icon);
}

//--------------------------------------------------------------------------------------

function get_manufacturing_trans_view_str($type, $trans_no, $label="", 
	$icon=false, $class='', $id='')
{
	$viewer = "manufacturing/view/";

	if ($type == ST_MANUISSUE)
		$viewer .= "wo_issue_view.php";
	elseif ($type == ST_MANURECEIVE)
		$viewer .= "wo_production_view.php";
	elseif ($type == ST_WORKORDER)
		$viewer .= "work_order_view.php";
	else
		return null;

	$viewer .= "?trans_no=$trans_no";

	if ($label == "")
		$label = $trans_no;

	return viewer_link($label, $viewer, $class, $id,  $icon);
}

//--------------------------------------------------------------------------------------

function get_dimensions_trans_view_str($type, $trans_no, $label="", $icon=false, 
	$class='', $id='')
{
	if ($type == ST_DIMENSION)
		$viewer = "dimensions/view/view_dimension.php?trans_no=$trans_no";
	else
		return null;

	if ($label == "")
		$label = $trans_no;

	return viewer_link($label, $viewer, $class, $id,  $icon);
}
/*
	Journal entry or cost update postings link
*/
function get_journal_trans_view_str($type, $trans_no, $label="", $icon=false, 
	$class='', $id='')
{
	if ($type == ST_JOURNAL || $type == ST_COSTUPDATE)
		$viewer = "gl/view/gl_trans_view.php?type_id=$type&trans_no=$trans_no";
	else
		return null;

	if ($label == "")
		$label = $trans_no;

	return viewer_link($label, $viewer, $class, $id,  $icon);
}

//--------------------------------------------------------------------------------------

function get_package_view_str($pkg, $label="", $icon=false, $class='', $id='')
{
	if ($label == "")
	{
		$label = _("Info");
//		$icon = ICON_GL;
	}
	return viewer_link($label, "includes/ui/view_package.php?id=$pkg", $class, $id, $icon);
}


//--------------------------------------------------------------------------------------

function get_trans_view_str($type, $trans_no, $label="", $icon=false, 
	$class='', $id='', $prenomina=0, $without_delivery=0, $ci=0)
{
	$view_str = get_customer_trans_view_str($type, $trans_no, $label, $icon, $class, $id, $without_delivery, $ci);
	if ($view_str != null)
		return $view_str;

	$view_str = get_supplier_trans_view_str($type, $trans_no, $label, $icon, $class, $id);
	if ($view_str != null)
		return $view_str;

	$view_str = get_banking_trans_view_str($type, $trans_no, $label, $icon, $class, $id);
	if ($view_str != null)
		return $view_str;

	$view_str = get_inventory_trans_view_str($type, $trans_no, $label, $icon, $class, $id);
	if ($view_str != null)
		return $view_str;

	$view_str = get_manufacturing_trans_view_str($type, $trans_no, $label, $icon, $class, $id);
	if ($view_str != null)
		return $view_str;

	$view_str = get_dimensions_trans_view_str($type, $trans_no, $label, $icon, $class, $id);
	if ($view_str != null)
		return $view_str;

	$view_str = get_journal_trans_view_str($type, $trans_no, $label, $icon, $class, $id);
	if ($view_str != null)
		return $view_str;

        /*Begin Félix Alberti 22/07/2015*/
        $view_str = get_professional_trans_view_str($type, $trans_no, $label, $icon, $class, $id, $prenomina);

	if ($view_str != null)
		return $view_str;
        /*End Félix Alberti 22/07/2015*/

	return null;
}

function get_trans_view_fiscal_str($type, $trans_no, $label="", $icon=false, 
	$class='', $id='', $invoicecancel=null)
{
	$view_str = get_print_fiscal_view_str($type, $trans_no, $label, $icon, $class, $id, $invoicecancel);
	if ($view_str != null)
		return $view_str;


	if ($view_str != null)
		return $view_str;
        /*End Félix Alberti 22/07/2015*/

	return null;
}

/*
	Helper for ui drawing functions.
	Checks whether any of input function parameters has changed or page is called with GET method (first page display).
	$name - context
*/
function check_ui_refresh($name=null)
{
	$bt = debug_backtrace();
	if (!$name)
		$name = $bt[1]['function'];
	$old = @$_SESSION['ui_context'][$name];
	$new = $_SESSION['ui_context'][$name] = $bt[1]['args'];
	return ($new != $old) || ($_SERVER['REQUEST_METHOD'] == 'GET');
}

//--------------------------------------------------------------------------------------
// Displays currency exchange rate for given date.
// When there is no exrate for today, 
// gets it form ECB and stores in local database.
//
function exchange_rate_display($from_currency, $to_currency, $date_, $force_edit=false)
{
    global $Ajax, $xr_provider_authoritative;

	$readonly = false;

	if ($from_currency != $to_currency)
	{
	 	$rate = get_post('_ex_rate');
	 	if (check_ui_refresh() || !$rate) { // readonly or ui context changed
			$comp_currency = get_company_currency();
			if ($from_currency == $comp_currency)
				$currency = $to_currency;
			else
				$currency = $from_currency;

			$rate = get_date_exchange_rate($currency, $date_); // try local

			if ($rate)
				$readonly = true; // if we have already local exrate keep it unchanged 

			if (!$rate) {	// retry from remote service
				$row = get_currency($currency);

 				if ($row['auto_update']) // autoupdate means use remote service & store exrate on first transaction.
 				{
 					$rate = retrieve_exrate($currency, $date_);
					if (!$rate)
						display_warning(sprintf(_("Cannot retrieve exchange rate for currency %s. Please adjust approximate rate if needed."), $currency));
					elseif ($xr_provider_authoritative) {
						// if the remote exrate is considered authoritative we can store the rate here,
						// otherwise exrate will be stored during transaction write
						$readonly = true;
						add_new_exchange_rate($currency, $date_, $rate);
					}
				}
			}
			if (!$rate)	{	// get and edit latest available
				$rate = get_exchange_rate_from_home_currency($currency, $date_);
			}
			if ($from_currency != $comp_currency)
				$rate = 1 / ($rate / get_exchange_rate_from_home_currency($to_currency, $date_));
			$Ajax->activate('_ex_rate_span');
		}

		$rate = number_format2($rate, user_exrate_dec());

		if ($force_edit || !$readonly)
			$ctrl = "<input type=\"text\" name=\"_ex_rate\" size=\"8\" maxlength=\"8\" value=\"$rate\">";
	    else
	    	$ctrl = "<span id=\"_ex_rate\">$rate</span>";

		label_row(_("Exchange Rate:"), $span = "<span style='vertical-align:top;' id='_ex_rate_span'>$ctrl $from_currency = 1 $to_currency</span>" );

	  	$Ajax->addUpdate('_ex_rate_span', '_ex_rate_span', $span);
	}
}

//--------------------------------------------------------------------------------------

function is_voided_display($type, $id, $label)
{
	$void_entry = get_voided_entry($type, $id);

	if ($void_entry == null)
		return false;

	start_table(TABLESTYLE, "width=50%");
	echo "<tr><td align=center><font color=red>$label</font><br>";
	echo "<font color=red>" . _("Date Voided:") . " " . sql2date($void_entry["date_"]) . "</font><br>";
	if (strlen($void_entry["memo_"]) > 0)
		echo "<center><font color=red>" . _("Memo:") . " " . $void_entry["memo_"] . "</font></center><br>";
	echo "</td></tr>";
	end_table(1);

	return true;
}

//--------------------------------------------------------------------------------------

function comments_display_row($type, $id)
{
	$comments = get_comments($type, $id);
	if ($comments and db_num_rows($comments))
	{
		echo "<tr><td colspan=15>";
    	while ($comment = db_fetch($comments))
    	{
    		echo nl2br($comment["memo_"]) . "<br>";
    	}
		echo "</td></tr>";
	}
}

//--------------------------------------------------------------------------------------

function get_comments_string($type, $type_no)
{
	$str_return = "";
	$result = get_comments($type, $type_no);
	while ($comment = db_fetch($result))
	{
		if (strlen($str_return))
			$str_return = $str_return . " \n";
		$str_return = $str_return . $comment["memo_"];
	}
	return $str_return;
}

//--------------------------------------------------------------------------------------

function view_stock_status($stock_id, $description=null, $echo=true)
{
	global $path_to_root;
	if ($description)
		//hyperlink_params_separate($path_to_root . "/inventory/inquiry/stock_status.php", (user_show_codes()?$stock_id . " - ":"") . $description, "stock_id=$stock_id");
		$preview_str = "<a target='_blank' href='$path_to_root/inventory/inquiry/stock_status.php?stock_id=$stock_id' onclick=\"javascript:openWindow(this.href,this.target); return false;\" >". (user_show_codes()?$stock_id . " - ":"") . $description."</a>";
	else
		//hyperlink_params_separate($path_to_root . "/inventory/inquiry/stock_status.php", $stock_id, "stock_id=$stock_id");
		$preview_str = "<a target='_blank' href='$path_to_root/inventory/inquiry/stock_status.php?stock_id=$stock_id' onclick=\"javascript:openWindow(this.href,this.target); return false;\" >$stock_id</a>";
	if($echo)
		echo $preview_str;
	return $preview_str;
}

function view_stock_status_cell($stock_id, $description=null)
{
	echo "<td>";
	view_stock_status($stock_id, $description);
	echo "</td>";
}

//--------------------------------------------------------------------------------------

function display_debit_or_credit_cells($value, $bold=false)
{
	$value = round2($value, user_price_dec());
	if ($value >= 0)
	{
		amount_cell($value, $bold);
		label_cell("");
	}
	elseif ($value < 0)
	{
		label_cell("");
		amount_cell(abs($value), $bold);
	}
}

//--------------------------------------------------------------------------------------

function display_customer_trans_tax_details($tax_items, $columns)
{
	global $alternative_tax_include_on_docs, $suppress_tax_rates;
	$first = true;
    while ($tax_item = db_fetch($tax_items))
    {
     	if (!$tax_item['amount'])
     		continue;

    	$tax = number_format2($tax_item['amount'],user_price_dec());
		if (isset($suppress_tax_rates) && $suppress_tax_rates == 1)
			$tax_type_name = $tax_item['tax_type_name'];
		else
			$tax_type_name = $tax_item['tax_type_name']." (".$tax_item['rate']."%) ";
    	if ($tax_item['included_in_price'])
    	{
			if (isset($alternative_tax_include_on_docs) && $alternative_tax_include_on_docs == 1)
  			{
  				if ($first)
  					label_row(_("Total Tax Excluded"), number_format2($tax_item['net_amount'], user_price_dec()),
  						"colspan=$columns align=right", "align=right");
        		label_row($tax_type_name, $tax, "colspan=$columns align=right", "align=right");
        		$first = false;
        	}
        	else
        		label_row(_("Included") . " " . $tax_type_name
        			. ": $tax", "", "colspan=$columns align=right", "align=right");
        }			
    	else
        	label_row($tax_type_name, $tax, "colspan=$columns align=right", "align=right");
    }
}

//--------------------------------------------------------------------------------------

function display_supp_trans_tax_details($tax_items, $columns)
{
    while ($tax_item = db_fetch($tax_items))
    {
     	if (!$tax_item['amount'])
     		continue;

    	$tax = number_format2(abs($tax_item['amount']),user_price_dec());
    	if ($tax_item['included_in_price'])
        	label_row(_("Included") . " " . $tax_item['tax_type_name'] . " (" . $tax_item['rate'] . "%) "
        		. ": $tax", '', "colspan=$columns align=right", "align=right");
    	else
        	label_row($tax_item['tax_type_name'] . " (" . $tax_item['rate'] . "%)",
        		$tax, "colspan=$columns align=right", "align=right");
    }
}

//--------------------------------------------------------------------------------------

function display_edit_tax_items($taxes, $columns, $tax_included, $rightspan=0, $editable=false)
{
	$total = 0;

    foreach ($taxes as $taxitem)
	{
		$amount = isset($taxitem['Override']) ? $taxitem['Override'] : $taxitem['Value'];
		if ($taxitem['Value'] != 0){
			if ($editable) {
				if (!isset($_POST['mantax['.$taxitem['tax_type_id'].']']))
					$_POST['mantax['.$taxitem['tax_type_id'].']'] = price_format($amount);
				start_row();
				if ($tax_included) {
					$colspan = $columns-1;
					label_cell(_("Included") . " " . $taxitem['tax_type_name'].":",
						"colspan={$colspan} align='right'");
					amount_cells(null, 'mantax['.$taxitem['tax_type_id'].']',
						null, "colspan=$columns align=right", '<td></td>', user_price_dec());
				} else {
					label_cell($taxitem['tax_type_name'], "colspan=$columns align='right'");
					amount_cells(null, 'mantax['.$taxitem['tax_type_id'].']',
						null, "colspan=$columns align=right", null, user_price_dec());
					$total +=  round2($amount, user_price_dec());
				}
				if ($rightspan)
					label_cell('', "colspan ='$rightspan'");
				end_row();
			} else {
				$value = number_format2($taxitem['Value'],user_price_dec());
				if ($tax_included)
				{
					label_row(_("Included") . " " . $taxitem['tax_type_name']
						. " " . $value , "", "colspan=$columns align=right", "align=right", $rightspan);
				}
				else
				{
					label_row($taxitem['tax_type_name'],
						$value, "colspan=$columns align=right", "align=right", $rightspan);
					$total +=  round2($taxitem['Value'], user_price_dec());
				}
			}
		}
	}
	return $total;
}

//--------------------------------------------------------------------------------------

function display_footer_exit($space=2)
{
	if ($space)
		br($space);
	end_page(false, false, true);
	exit;
}

//--------------------------------------------------------------------------------------

function display_allocations($alloc_result, $total, $payments=false)
{
	global $systypes_array;

	if (!$alloc_result || db_num_rows($alloc_result) == 0)
		return;

    display_heading2(($payments ? _("Payments") : _("Allocations")));

    start_table(TABLESTYLE, "width=80%");

    $th = array( _("Type"), _("Number"), _("Date"), _("Total Amount"),
    	_("Left to Allocate"), _("This Allocation"));
	table_header($th);
    $k = $total_allocated = 0;

    while ($alloc_row = db_fetch($alloc_result))
    {

    	alt_table_row_color($k);

    	label_cell($systypes_array[$alloc_row['type']]);
    	label_cell(get_trans_view_str($alloc_row['type'],$alloc_row['trans_no']));
    	label_cell(sql2date($alloc_row['tran_date']));
    	$alloc_row['Total'] = round2($alloc_row['Total'], user_price_dec());
    	$alloc_row['amt'] = round2($alloc_row['amt'], user_price_dec());
    	if ($payments && ($alloc_row['type'] == ST_SUPPAYMENT || $alloc_row['type'] == ST_BANKPAYMENT || $alloc_row['type'] == ST_SUPPCREDIT))
    		$alloc_row['Total'] = -$alloc_row['Total'];
    	amount_cell($alloc_row['Total']);
   		amount_cell($alloc_row['Total'] - $alloc_row['amt']);
    	amount_cell($alloc_row['amt']);
    	end_row();

    	$total_allocated += $alloc_row['amt'];
    }
    start_row();
   	label_cell(_("Total Allocated:"), "align=right colspan=5");
	amount_cell($total_allocated);
	end_row();
	start_row();
    label_cell(_("Left to Allocate:"), "align=right colspan=5");
    $total = round2($total, user_price_dec());
    amount_cell($total - $total_allocated);
    end_row();

    end_table(1);
}

//--------------------------------------------------------------------------------------

function display_allocations_from($person_type, $person_id, $type, $type_no, $total)
{
	switch ($person_type)
	{
		case PT_CUSTOMER :
			$alloc_result = get_allocatable_to_cust_transactions($person_id, $type_no, $type);
			display_allocations($alloc_result, $total);
			return;
		case PT_SUPPLIER :
			$alloc_result = get_allocatable_to_supp_transactions($person_id, $type_no, $type);
			display_allocations($alloc_result, $total);
			return;
	}
}

//--------------------------------------------------------------------------------------

function display_allocations_to($person_type, $person_id, $type, $type_no, $total)
{
	switch ($person_type)
	{
		case PT_CUSTOMER :
			$alloc_result = get_allocatable_from_cust_transactions($person_id, $type_no, $type);
			display_allocations($alloc_result, $total, true);
			return;
		case PT_SUPPLIER :
			$alloc_result = get_allocatable_from_supp_transactions($person_id, $type_no, $type);
			display_allocations($alloc_result, $total, true);
			return;
	}
}

function display_allocations_to_account($person_type, $person_id, $type, $type_no, $total)
{
	switch ($person_type)
	{
		case PT_CUSTOMER :
			$alloc_result = get_allocatable_from_cust_transactions_account($person_id, $type_no, $type);                
			//display_notification($alloc_result);
                        display_allocations($alloc_result, $total, true);
			return;
		case PT_SUPPLIER :
			$alloc_result = get_allocatable_from_supp_transactions($person_id, $type_no, $type);
			display_allocations($alloc_result, $total, true);
			return;
	}
}

//--------------------------------------------------------------------------------------
//
//	Expands selected quick entry $id into GL posings and adds to cart.
//		returns calculated amount posted to bank GL account.
//
function display_quick_entries(&$cart, $id, $base, $type, $descr='')
{
	$bank_amount = 0;
	
	if (!isset($id) || $id == null || $id == "")
	{
		display_error( _("No Quick Entries are defined."));
		set_focus('totamount');
	}
	else
	{
		if ($type == QE_DEPOSIT)
			$base = -$base;
		if ($type != QE_SUPPINV)	// only one quick entry on journal/bank transaction
			$cart->clear_items();
		$qe = get_quick_entry($id);
		if ($qe['bal_type'] == 1)
		{
			if ($qe['base_amount'] == 1.0) // monthly
				$begin = begin_month($cart->tran_date);
			else
			{
				if (is_account_balancesheet($qe['base_desc'])) // total
					$begin = "";
				else
					$begin = begin_fiscalyear(); // from fiscalyear begin
			}		
			$base = get_gl_trans_from_to($begin, $cart->tran_date, $qe['base_desc']);
				
		}
		if ($descr != '') $qe['description'] .= ': '.$descr;
		$result = get_quick_entry_lines($id);
		if (db_num_rows($result) == 0)
		{
			display_error( _("No Quick Entry lines are defined."));
			set_focus('totamount');
			return 0;
		}	
		$totrate = 0;
		while ($row = db_fetch($result))
		{
			$qe_lines[] = $row;

			switch (strtolower($row['action'])) {
				case "t": // post taxes calculated on base amount
				case "t+": // ditto & increase base amount
				case "t-": // ditto & reduce base amount
					if (substr($row['action'],0,1) != 'T') 
						$totrate += get_tax_type_default_rate($row['dest_id']);
			}
		}
		$first = true;
		$taxbase = 0;
		foreach($qe_lines as $qe_line)
		{
			switch (strtolower($qe_line['action'])) {
				case "=": // post current base amount to GL account
					$part = $base;
					break;
				case "a": // post amount to GL account and reduce base
					$part = $qe_line['amount'];
					break;
				case "a+": // post amount to GL account and increase base
					$part = $qe_line['amount']; $base += $part;
					break;
				case "a-": // post amount to GL account and reduce base
					$part = $qe_line['amount']; $base -= $part;
					break;
				case "%":	// store acc*amount% to GL account
					$part = round2($base * $qe_line['amount'] / 100, user_price_dec());
					break;
				case "%+":	// ditto & increase base amount
					$part = round2($base * $qe_line['amount'] / 100, user_price_dec());
					$base += $part;
					break;
				case "%-":	// ditto & reduce base amount
					$part = round2($base * $qe_line['amount'] / 100, user_price_dec());
					$base -= $part;
					break;
				case "t": // post taxes calculated on base amount
				case "t+": // ditto & increase base amount
				case "t-": // ditto & reduce base amount
					if ($first)
					{
						$taxbase = $base/($totrate+100);
						$first = false;
					}

					if (substr($qe_line['action'],0,1) != 'T') 
						$part = $taxbase;
					else
						$part = $base/100;
					$item_tax = get_tax_type($qe_line['dest_id']);
					//if ($type == QE_SUPPINV && substr($qe_line['action'],0,1) != 'T')
					if ($type == QE_SUPPINV)
					{
						$taxgroup = $cart->tax_group_id;
						$rates = 0;
						$res = get_tax_group_rates($cart->tax_group_id);
						while ($row = db_fetch($res))
							$rates += $row['rate'];
						if ($rates == 0)
							continue 2;
					}
					$tax = round2($part * $item_tax['rate'],  user_price_dec());
					if ($tax==0) continue 2;
					$gl_code = ($type == QE_DEPOSIT || ($type == QE_JOURNAL && $base < 0)) 
						? $item_tax['sales_gl_code'] : $item_tax['purchasing_gl_code'];
					if (!is_tax_gl_unique($gl_code)) {
	   					display_error(_("Cannot post to GL account used by more than one tax type."));
						break 2;
					}
					if ($type != QE_SUPPINV)
						$cart->add_gl_item($gl_code, 
							$qe_line['dimension_id'], $qe_line['dimension2_id'], 
							$tax, $qe['description']);
					else 
					{
						$acc_name = get_gl_account_name($gl_code);
						$cart->add_gl_codes_to_trans($gl_code, 
							$acc_name, $qe_line['dimension_id'], 
							$qe_line['dimension2_id'], $tax, $qe['description']);
					}
					if (strpos($qe_line['action'], '+'))
						$base += $tax;
					elseif (strpos($qe_line['action'], '-'))
						$base -= $tax;
					continue 2;
			}
			if ($type != QE_SUPPINV)
				$cart->add_gl_item($qe_line['dest_id'], $qe_line['dimension_id'],
					$qe_line['dimension2_id'], $part, $qe['description']);
			else 
			{
				$acc_name = get_gl_account_name($qe_line['dest_id']);
				$cart->add_gl_codes_to_trans($qe_line['dest_id'], 
					$acc_name, $qe_line['dimension_id'], 
					$qe_line['dimension2_id'], $part, $qe['description']);
			}
		}
	}	
	return $bank_amount;
}

//--------------------------------------------------------------------------------------
//
//	Simple English version of number to words conversion.
//
function _number_to_words($number) 
{ 
    $Bn = floor($number / 1000000000); /* Billions (giga) */ 
    $number -= $Bn * 1000000000; 
    $Gn = floor($number / 1000000);  /* Millions (mega) */ 
    $number -= $Gn * 1000000; 
    $kn = floor($number / 1000);     /* Thousands (kilo) */ 
    $number -= $kn * 1000; 
    $Hn = floor($number / 100);      /* Hundreds (hecto) */ 
    $number -= $Hn * 100; 
    $Dn = floor($number / 10);       /* Tens (deca) */ 
    $n = $number % 10;               /* Ones */

    $res = ""; 

    if ($Bn) 
        $res .= _number_to_words($Bn) . " Billion"; 
    if ($Gn) 
        $res .= (empty($res) ? "" : " ") . _number_to_words($Gn) . " Million"; 
    if ($kn) 
        $res .= (empty($res) ? "" : " ") . _number_to_words($kn) . " Thousand"; 
    if ($Hn) 
        $res .= (empty($res) ? "" : " ") . _number_to_words($Hn) . " Hundred"; 

    $ones = array("", "One", "Two", "Three", "Four", "Five", "Six", 
        "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen", 
        "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", 
        "Nineteen"); 
    $tens = array("", "", "Twenty", "Thirty", "Fourty", "Fifty", "Sixty", 
        "Seventy", "Eighty", "Ninety"); 

    if ($Dn || $n) 
    { 
        if (!empty($res)) 
            $res .= " and "; 
        if ($Dn < 2) 
            $res .= $ones[$Dn * 10 + $n]; 
        else 
        { 
            $res .= $tens[$Dn]; 
            if ($n) 
                $res .= "-" . $ones[$n]; 
        } 
    } 

    if (empty($res)) 
        $res = "zero"; 
    return $res; 
} 

function price_in_words($amount, $document=0)
{
	// use local price_in_words() if the hook is defined
	$price = hook_price_in_words($amount, $document);
	if ($price) 
		return $price;

	// Only usefor Remittance and Receipts as default
	if (!($document == ST_SUPPAYMENT || $document == ST_CUSTPAYMENT || $document == ST_CHEQUE))
		return "";
	if ($amount < 0 || $amount > 999999999999)
		return "";
	$dec = user_price_dec();
	if ($dec > 0)
	{
		$divisor = pow(10, $dec);
        $frac = round2($amount - floor($amount), $dec) * $divisor;
		$frac = sprintf("%0{$dec}d", round2($frac, 0));
		$and = _("and");
    	$frac = " $and $frac/$divisor";
    }
    else
    	$frac = "";
    return _number_to_words(intval($amount)) . $frac;
}    

function get_js_open_window($width, $height)
{
	$js ="function openWindow(url, title)\n"
		. "{\n"
		. " var left = (screen.width - $width) / 2;\n"
		. " var top = (screen.height - $height) / 2;\n"
		. " return window.open(url, title, 'width=$width,height=$height,left='+left+',top='+top+',screenX='+left+',screenY='+top+',status=no,scrollbars=yes');\n"
		. "}\n";
	return $js;
}

function get_js_open_window_timer($width, $height)
{
	$js ="function openWindow(url, title)\n"
		. "{\n"
		. " var left = (screen.width - $width) / 2;\n"
		. " var top = (screen.height - $height) / 2;\n"
		. " var win = window.open(url, title, 'width=$width,height=$height,left='+left+',top='+top+',screenX='+left+',screenY='+top+',status=no,scrollbars=yes');\n"
                . " var timer = setInterval(function() {"   
                . "   if(win.closed) {"  
                . "      clearInterval(timer);"  
                . "      recargar();"
                . "   }"  
                . " }"
                . " , 1000);"
                . " return win"              
                . "}\n";
	return $js;
}

//------------------------------------------------------------------------------
//Begin Felix Alberti 16/03/2016
function get_js_abre_ventana_timer($width, $height)
{
	$js ="function abreVentana(url, title)\n"
		. "{\n"
		. " var left = (screen.width - $width) / 2;\n"
		. " var top = (screen.height - $height) / 2;\n"
		. " var win = window.open(url, title, 'width=$width,height=$height,left='+left+',top='+top+',screenX='+left+',screenY='+top+',status=no,scrollbars=yes');\n"
                . " var timer = setInterval(function() {"   
                . "   if(win.closed) {"                
                . "      clearInterval(timer);"
                . "      recargar();"
                . "   }"  
                . " }"
                . " , 1000);"
                . " return win"              
                . "}\n";
	return $js;
}
////End


//------------------------------------------------------------------------------


/*Begin Creada por Felix Alberti 10/07/2015*/
function get_js_onload()
 {
        $js = "";                        
        //$js ="window.onload = function() {\n"
                //. " alert(\"Reload Programar aqui\"); \n"
        //        . "};\n";
        /*$js ="function getValues(valor){\n
            document.forms[0].elements[\"huboCambio\"].value = valor;\n
        }\n
        ";*/
        $js .="function recargar(){\n"
            //."alert('recargar');"
            ."window.location.reload();
        }\n
        ";
	return $js;
}
/*End Creada por Felix Alberti 10/07/2015*/

/*Begin Creada por Felix Alberti 16/03/2016*/
function get_js_recargar()
 {
       
        $js ="function recargar(){\n".
            "//alert('recargar');\n"
            ."window.location.reload();
        }\n";
	return $js;
}
/*End Creada por Felix Alberti 16/03/2016*/


/*Begin Creada por Felix Alberti 10/07/2015*/
function get_js_onunload(){
        $js ="";
	/*$js .="window.onbeforeunload=navigationError;\n"
	. "var dont_confirm_leave = 0; var leave_message = 'Esta seguro de cerrar la ventana?';\n"
                . " function navigationError(e)\n"
                . "{\n"
                . "  if(dont_confirm_leave!==1)\n"
                . "  {\n"
                        . "   if(!e) e = window.event;\n"
                        . "   //e.cancelBubble is supported by IE - this will kill the bubbling process.\n"
                        . "   e.cancelBubble = true;\n"
                        . "   e.returnValue = leave_message;\n"
                        . "   if (e.stopPropagation)\n"
                        . "     {\n"
                        . "      e.stopPropagation();\n"
                        . "      e.preventDefault();\n"
                        . "   }\n"
                        . "   return leave_message;\n"
                . "  }\n"
		. "}\n";*/
        $js .= 
        "if (window.addEventListener) {\n  
            // all browsers except IE before version 9
            \n
            window.addEventListener (\"beforeunload\", OnBeforeUnLoad(1), false);\n
        }\n
        else {\n
            if (window.attachEvent) {\n   // IE before version 9
                window.attachEvent (\"onbeforeunload\", OnBeforeUnLoad);\n
            }\n
        }\n
        function OnBeforeUnLoad (recargar) {\n
            //Ejecuta reload del padre de la ventana popup
            //window.opener.getValues(1);//hubo cambio
            if (recargar==1){
               window.opener.location.reload(true);\n
            }
        }\n        
        ";
	return $js;
}
/*End Creada por Felix Alberti 10/07/2015*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*Begin Creada por Felix Alberti 02/03/2016*/
function get_js_onload_print_fiscal1()
 {
 	      $js = "function firefox(trans_no) {\n"." //alert(\"firefox\");\n".
				"netscape.security.PrivilegeManager.enablePrivilege(\"UniversalXPConnect\");\n".
				"var exe = window.Components.classes['@mozilla.org/file/local;1'].createInstance(Components.interfaces.nsILocalFile);\n".
				"exe.initWithPath(\"C:\\\\Windows\\\\\\\uxira\\\\\bematech\\\\executablebefisc\\\\execute.bat\");\n".
				"var run = window.Components.classes['@mozilla.org/process/util;1'].createInstance(Components.interfaces.nsIProcess);\n".
				"run.init(exe);\n".
				"var parameters = [10, trans_no];\n".
				"run.run(false, parameters,parameters.length);\n".
                                "alert(\"2 firefox\");\n".
				"}\n";
				
	      $js .= "function RunExecute(tran_no){\n  //alert(\"RunExecute\");\n
	        if((navigator.userAgent.indexOf(\"MSIE\") != -1 ) || (!!document.documentMode == true )){ //IF IE > 10\n
	            if (window.ActiveXObject || \"ActiveXObject\" in window){\n               
	               var oShell = new ActiveXObject(\"Shell.Application\");\n                                                      
	               var commandtoRun = \"C:\\\\Windows\\\\uxira\\\\bematech\\\\executablebefisc\\\\execute.bat 10, tran_no\";\n 
	               oShell.ShellExecute(commandtoRun,\"\",\"\",\"open\",\"1\");\n
	            }\n
	        }\n
					else {\n
				     if (navigator.userAgent.indexOf(\"Firefox\") > 0){\n
                                         //alert('firefox; programado');\n
				         //firefox(tran_no);\n
                                         window.location.assign('appurl://10, '+tran_no);
				     }
				     else\n {\n
                                            //alert('chrone; falta programar');\n
                                            window.location.assign('appurl://10, '+tran_no);
                                      }\n
					}\n
				}\n";				
				
	return $js;
}
function get_js_onload_print_fiscal2(){
$js = 
        "if (window.addEventListener) {\n  
            // all browsers except IE before version 9
            \n
            //alert(\"Paso 1\");
            window.addEventListener (\"onload\", OnRecargar(1), false);\n
        }\n
        else {\n
            //alert(\"Paso 2\");
            if (window.attachEvent) {\n   // IE before version 9
                window.attachEvent (\"onload\", OnRecargar);\n
            }\n
        }\n
        function OnRecargar (recargar) {\n   
            var valorParam = getQueryParam('AddedDI');\n
            if (valorParam != '' && valorParam != undefined){\n
               //alert(valorParam);\n
               //alert(\"Paso 3\");\n
               //RunExecute(valorParam);\n
               quit();
               init('10,'+valorParam);
            }\n  
        }\n 
        function getQueryParam (param) {\n
        //alert(\"getQueryParam\");
				    var found;\n
				    window.location.search.substr(1).split(\"&\").forEach(function(item) {\n				        
				        if (param ==  item.split(\"=\")[0]) {\n
				            found = item.split(\"=\")[1];\n
				        }\n
				    });\n
				    return found;\n
				};  \n     
        ";
	return $js;
}	
/*End Creada por Felix Alberti 02/03/2016*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*Begin Creada por Felix Alberti 25/06/2016*/
function get_js_onload_print_credit_note_fiscal1()
 {
 	      $js = "function firefox(trans_no,invoicecancel) {\n".
				"netscape.security.PrivilegeManager.enablePrivilege(\"UniversalXPConnect\");\n".
				"var exe = window.Components.classes['@mozilla.org/file/local;1'].createInstance(Components.interfaces.nsILocalFile);\n".
				"exe.initWithPath(\"C:\\\\Windows\\\\\\\uxira\\\\\bematech\\\\executablebefisc\\\\execute.bat\");\n".
				"var run = window.Components.classes['@mozilla.org/process/util;1'].createInstance(Components.interfaces.nsIProcess);\n".
				"run.init(exe);\n".
				"var parameters = [11, trans_no, invoicecancel];\n".
				"run.run(false, parameters,parameters.length);\n".
				"}\n";
				
	      $js .= "function RunExecute(tran_no,invoicecancel){\n  //alert(\"RunExecute\");\n
	        if((navigator.userAgent.indexOf(\"MSIE\") != -1 ) || (!!document.documentMode == true )){ //IF IE > 10\n
	            if (window.ActiveXObject || \"ActiveXObject\" in window){\n               
	               var oShell = new ActiveXObject(\"Shell.Application\");\n                                                      
	               var commandtoRun = \"C:\\\\Windows\\\\uxira\\\\bematech\\\\executablebefisc\\\\execute.bat 11, tran_no, invoicecancel\";\n 
	               oShell.ShellExecute(commandtoRun,\"\",\"\",\"open\",\"1\");\n
	            }\n
	        }\n
					else {\n
				     if (navigator.userAgent.indexOf(\"Firefox\") > 0){\n
				         //firefox(tran_no,invoicecancel);\n
                                         window.location.assign('appurl://11, '+tran_no);
				     }
				     else\n
                                     {\n
                                        //alert('chrone; falta programar');\n
                                        window.location.assign('appurl://11, '+tran_no);
                                     }\n
					}\n
				}\n";				
				
	return $js;
}
function get_js_onload_print_credit_note_fiscal2(){
$js = 
        "if (window.addEventListener) {\n  
            // all browsers except IE before version 9
            \n
            //alert(\"Paso 1\");
            window.addEventListener (\"onload\", OnRecargar(1), false);\n
        }\n
        else {\n
            //alert(\"Paso 2\");
            if (window.attachEvent) {\n   // IE before version 9
                window.attachEvent (\"onload\", OnRecargar);\n
            }\n
        }\n
        function OnRecargar (recargar) {\n   
            var valorParam = getQueryParam('AddedID');\n
            var valorParam2 = getQueryParam('InvoiceCancel');\n
            //alert(\"valorParam \"+valorParam);
            //alert(\"InvoiceCancel \"+valorParam2);
            if (valorParam != '' && valorParam != undefined){\n
               //alert(valorParam+' '+valorParam2);\n
               //alert(\"Nota de Credito\");\n
               //RunExecute(valorParam,valorParam2);\n
               quit();
               init('11,'+valorParam+','+valorParam2);
            }\n  
        }\n 
        function getQueryParam (param) {\n
        //alert(\"getQueryParam\");
				    var found;\n
				    window.location.search.substr(1).split(\"&\").forEach(function(item) {\n				        
				        if (param ==  item.split(\"=\")[0]) {\n
				            found = item.split(\"=\")[1];\n
				        }\n
				    });\n
				    return found;\n
				};  \n     
        ";
	return $js;
}	
/*End Creada por Felix Alberti 25/06/2016*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*******************************************************/
//Begin Felix Alberti 29/06/2016
function get_js_onload_print_reporte_x_1()
 {
 	      $js = "function firefox() {\n".
				"netscape.security.PrivilegeManager.enablePrivilege(\"UniversalXPConnect\");\n".
				"var exe = window.Components.classes['@mozilla.org/file/local;1'].createInstance(Components.interfaces.nsILocalFile);\n".
				"exe.initWithPath(\"C:\\\\Windows\\\\uxira\\\\bematech\\\\executablebefisc\\\\execute.bat\");\n".
				"var run = window.Components.classes['@mozilla.org/process/util;1'].createInstance(Components.interfaces.nsIProcess);\n".
				"run.init(exe);\n".
				"var parameters = [98];\n".
				"run.run(false, parameters,parameters.length);\n".
				"}\n";
				
	      $js .= "function RunExecute(){\n  //alert(\"RunExecute\");\n
	        if((navigator.userAgent.indexOf(\"MSIE\") != -1 ) || (!!document.documentMode == true )){ //IF IE > 10\n
	            if (window.ActiveXObject || \"ActiveXObject\" in window){\n               
	               var oShell = new ActiveXObject(\"Shell.Application\");\n                                                      
	               var commandtoRun = \"C:\\\\Windows\\\\uxira\\\\bematech\\\\executablebefisc\\\\execute.bat 98\";\n 
	               oShell.ShellExecute(commandtoRun,\"\",\"\",\"open\",\"1\");\n
	            }\n
	        }\n
					else {\n
				     if (navigator.userAgent.indexOf(\"Firefox\") > 0){\n
				            //firefox();\n
                                            //window.location.assign('appurl://98');
                                            window.location.assign('http://localhost:8080/UxiraClientApp/printInvoiceServlet?opt=98');
				     }
				     else\n
				     //alert('chrone; falta programar');\n
                                     //chrome();
                                     //window.location.assign('appurl://98');
                                     window.location.assign('http://localhost:8080/UxiraClientApp/printInvoiceServlet?opt=98');
					}\n
				}\n";				
				
	return $js;

}
function get_js_onload_print_reporte_x_2(){
$js = 
        "if (window.addEventListener) {\n  
            // all browsers except IE before version 9
            \n
            //alert(\"Paso 1\");
            window.addEventListener (\"onload\", OnRecargar(1), false);\n
        }\n
        else {\n
            //alert(\"Paso 2\");
            if (window.attachEvent) {\n   // IE before version 9
                window.attachEvent (\"onload\", OnRecargar);\n
            }\n
        }\n
        function OnRecargar (recargar) {\n 
            //alert(\"OnRecargar xyz\");
            var valorParam = getQueryParam('Printrep');\n
            if (valorParam != '' && valorParam != undefined){\n
               //alert(valorParam);\n
               //alert(\"Paso 3\");\n
               //RunExecute();
            }\n 
            //RunExecute();
            quit();
            init('100,0');
            
        }\n 
        function getQueryParam (param) {\n
        //alert(\"getQueryParam\");
				    var found;\n
				    window.location.search.substr(1).split(\"&\").forEach(function(item) {\n				        
				        if (param ==  item.split(\"=\")[0]) {\n
				            found = item.split(\"=\")[1];\n
				        }\n
				    });\n
				    return found;\n
				};  \n     
        ";
	return $js;
}	
/*End Creada por Felix Alberti 29/06/2016*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*******************************************************/
//Begin Felix Alberti 29/06/2016
function get_js_onload_print_reporte_z_1()
 {
 	      $js = "function firefox() {\n alert(\"firefox\");\n alert(\"C:\\\\Windows\\\\uxira\\\\bematech\\\\executablebefisc\\\\execute.bat\");\n".
				"netscape.security.PrivilegeManager.enablePrivilege(\"UniversalXPConnect\");\n".
				"var exe = window.Components.classes['@mozilla.org/file/local;1'].createInstance(Components.interfaces.nsILocalFile);\n".
				"exe.initWithPath(\"C:\\\\Windows\\\\uxira\\\\bematech\\\\executablebefisc\\\\execute.bat\");\n".
				"var run = window.Components.classes['@mozilla.org/process/util;1'].createInstance(Components.interfaces.nsIProcess);\n".
				"run.init(exe);\n".
				"var parameters = [99];\n".
				"run.run(false, parameters,parameters.length);\n".
				"}\n";
              
              $js .= "function chrome() {\n"
                      . "alert('estoy en chrome');"
                      . "window.location.assign('appurl://99');"
                      //. "openWindow('chrome-extension://knldjmfmopnpolahpmmgbagdohdnhkik/main.html?factura=10','Impresion');"
                      //. "openWindow('http://localhost/run-my-exe/app/main.html?factura=10','Impresion');"
//                      . "\n chrome.management.launchApp(\"knldjmfmopnpolahpmmgbagdohdnhkik\", function(){
//  if(chrome.runtime.lastError) console.error(chrome.runtime.lastError);
//  else console.log(\"App launched\");
//});\n"
//                      . "\n chrome.runtime.sendMessage(\"knldjmfmopnpolahpmmgbagdohdnhkik\", { message: \"version\" },
//    function (reply) {
//        if (reply) {
//            if (reply.version) {
//                //log the response received from the chrome application
//                console.log(reply.version);
//            }
//        }
//    }
// );"
                      . "}\n";
				
	      $js .= "function RunExecute(){\n  //alert(\"RunExecute\");\n
	        if((navigator.userAgent.indexOf(\"MSIE\") != -1 ) || (!!document.documentMode == true )){ //IF IE > 10\n
	            if (window.ActiveXObject || \"ActiveXObject\" in window){\n               
	               var oShell = new ActiveXObject(\"Shell.Application\");\n                                                      
	               var commandtoRun = \"C:\\\\Windows\\\\uxira\\\\bematech\\\\executablebefisc\\\\execute.bat 99\";\n 
	               oShell.ShellExecute(commandtoRun,\"\",\"\",\"open\",\"1\");\n
	            }\n
	        }\n
					else {\n
				     if (navigator.userAgent.indexOf(\"Firefox\") > 0){\n
				            //firefox();\n
                                            window.location.assign('appurl://99');
				     }
				     else\n
				     //alert('chrone; falta programar');\n
                                     //chrome();
                                     window.location.assign('appurl://99');
					}\n
				}\n";				
				
	return $js;
}
function get_js_onload_print_reporte_z_2(){
$js = 
        "if (window.addEventListener) {\n  
            // all browsers except IE before version 9
            \n
            //alert(\"Paso 1\");
            window.addEventListener (\"onload\", OnRecargar(1), false);\n
            //window.addEventListener(\"load\", function(){
                //alert(\"Paso xyz\");\n
                //chrome.tabs.create({\"url\":\"http://gooele.com\"});\n
                //if (chrome) alert('chrome 1');
                //if (chrome.runtime) alert('chrome runtime');
                //else
                //alert('falta chrome runtime');
                //RunExecute();
            //}, false);
        }\n
        else {\n
            //alert(\"Paso 2\");
            if (window.attachEvent) {\n   // IE before version 9
                window.attachEvent (\"onload\", OnRecargar);\n
            }\n
        }\n
        function OnRecargar (recargar) {\n   
            //alert(\"Paso 2 OnRecargar\");
            var valorParam = getQueryParam('Printrep');\n
            if (valorParam != '' && valorParam != undefined){\n
               //alert(valorParam+' '+valorParam2);\n
               //alert(\"Paso 3\");\n
               //RunExecute();\n               
            }\n             
            quit();
            init('99,0');
        }\n 
        function getQueryParam (param) {\n
        //alert(\"getQueryParam\");
				    var found;\n
				    window.location.search.substr(1).split(\"&\").forEach(function(item) {\n				        
				        if (param ==  item.split(\"=\")[0]) {\n
				            found = item.split(\"=\")[1];\n
				        }\n
				    });\n
				    return found;\n
				};  \n     
        ";
	return $js;
}	
/*End Creada por Felix Alberti 29/06/2016*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

function get_js_function_onblur(){
        $D = 'uxira';
        $file = 'set_global_variable.php';
        /*if ($_SERVER["SERVER_PORT"] != 443)
        $set_var_global = 'http://'.$_SERVER['SERVER_NAME'].'/'.$D.'/'.$file;
        if ($_SERVER["SERVER_PORT"] == 8080)
        $set_var_global = 'http://'.$_SERVER['SERVER_NAME'].':8080/'.$D.'/'.$file;
        elseif ($_SERVER["SERVER_PORT"] == 8888)
        $set_var_global = 'http://'.$_SERVER['SERVER_NAME'].':8888/'.$D.'/'.$file; 
        else*/
        if ($_SERVER["SERVER_PORT"] != '')
        $set_var_global = 'http://'.$_SERVER['SERVER_NAME'].':'.$_SERVER["SERVER_PORT"].'/'.$D.'/'.$file;     
        else
        $set_var_global = 'http://'.$_SERVER['SERVER_NAME'].'/'.$D.'/'.$file;
        
        $js = "function muestraContenidoAjax() {\n
            if(peticion_http.readyState == 4) {\n
              if(peticion_http.status == 200) {\n
                //alert(peticion_http.responseText);\n
              }\n
            }\n
            //console.log('peticion con estatus = '+peticion_http.readyState);\n
        }\n";
        $js .= "function create_Instancia_Ajax(param1) {\n
            // Obtener la instancia del objeto XMLHttpRequest
            if(window.XMLHttpRequest) {\n
              peticion_http = new XMLHttpRequest();\n
            }\n
            else if(window.ActiveXObject) {\n
              peticion_http = new ActiveXObject(\"Microsoft.XMLHTTP\");\n
            }\n
            //console.log(peticion_http);\n            
            peticion_http.onreadystatechange = muestraContenidoAjax;\n
            //
            // Realizar peticion HTTP           
            peticion_http.open('GET', '$set_var_global?param1='+param1, true);
            peticion_http.send(null);
        }\n";
        
        $js .= "function description_onblur (param1) {\n"
                . "create_Instancia_Ajax(param1);"
                . "};  \n   ";
        return $js;
}

/*
  Setting focus on element $name in $form.
  If $form<0 $name is element id.
*/
function set_focus($name, $form_no=0) {
  global $Ajax;
	$Ajax->addFocus(true, $name);
    $_POST['_focus'] = $name;
}
//
//	Set default focus on first field $name if not set yet
//	Returns unique name if $name=null
//	
function default_focus($name=null, $form_no=0) {
	static $next; 
	if ($name==null) 
		$name = uniqid('_el',true);
    if (!isset($_POST['_focus'])) {
	  set_focus($name);
    }
	return $name;
}
/*
	Reset focus to next control element (e.g. link).
*/
function reset_focus()
{
	unset($_POST['_focus']);
}

function get_js_date_picker()
{
    global $go_debug;
    $fpath = company_path().'/js_cache/'.'date_picker.js';

    if (!file_exists($fpath) || $go_debug) {

	global $dateseps, $date_system, $tmonths;

	$how = user_date_format();				// 0 = us/ca, 1 = eu, au, nz, 2 = jp, sw
	$sep = $dateseps[user_date_sep()];		// date separator
	$wstart = (($date_system == 1 || $date_system == 2 || $date_system == 3) ? 6 : ($how == 0 || $how == 3 ? 0 : 1));	// weekstart (sun = 0, mon = 1)
	$months = array(_("January"),_("February"),_("March"),_("April"),_("May"),_("June"),_("July"),_("August"),_("September"),_("October"),_("November"),_("December"));
	$wdays = array(_("Su"),_("Mo"),_("Tu"),_("We"),_("Th"),_("Fr"),_("Sa"));
	$wno = _("W"); // week no
	$back = _("Back");
	if ($date_system == 1)
		list($cyear, $cmon, $cday) = gregorian_to_jalali(date("Y"), date("n"), date("j"));
	elseif ($date_system == 2)
		list($cyear, $cmon, $cday) = gregorian_to_islamic(date("Y"), date("n"), date("j"));


	$js = "
function positionInfo(object) {
  var p_elm = object;
  this.getElementLeft = getElementLeft;
  function getElementLeft() {
    var x = 0;
    var elm;
    if(typeof(p_elm) == 'object'){
      elm = p_elm;
    } else {
      elm = document.getElementById(p_elm);
    }
    while (elm != null) {
      x+= elm.offsetLeft;
      elm = elm.offsetParent;
    }
    return parseInt(x);
  }
  this.getElementWidth = getElementWidth;
  function getElementWidth(){
    var elm;
    if(typeof(p_elm) == 'object'){
      elm = p_elm;
    } else {
      elm = document.getElementById(p_elm);
    }
    return parseInt(elm.offsetWidth);
  }
  this.getElementRight = getElementRight;
  function getElementRight(){
    return getElementLeft(p_elm) + getElementWidth(p_elm);
  }
  this.getElementTop = getElementTop;
  function getElementTop() {
    var y = 0;
    var elm;
    if(typeof(p_elm) == 'object'){
      elm = p_elm;
    } else {
      elm = document.getElementById(p_elm);
    }
    while (elm != null) {
      y+= elm.offsetTop;
      elm = elm.offsetParent;
    }
    return parseInt(y);
  }
  this.getElementHeight = getElementHeight;
  function getElementHeight(){
    var elm;
    if(typeof(p_elm) == 'object'){
      elm = p_elm;
    } else {
      elm = document.getElementById(p_elm);
    }
    return parseInt(elm.offsetHeight);
  }
  this.getElementBottom = getElementBottom;
  function getElementBottom(){
    return getElementTop(p_elm) + getElementHeight(p_elm);
  }
}
function CC() {
  var calendarId = 'CC';
  var currentYear = 0;
  var currentMonth = 0;
  var currentDay = 0;
  var selectedYear = 0;
  var selectedMonth = 0;
  var selectedDay = 0;
  var months = ['$months[0]','$months[1]','$months[2]','$months[3]','$months[4]','$months[5]','$months[6]','$months[7]','$months[8]','$months[9]','$months[10]','$months[11]'];
  var wdays = ['$wdays[0]', '$wdays[1]', '$wdays[2]', '$wdays[3]', '$wdays[4]', '$wdays[5]', '$wdays[6]'];
  var tmonths = ['$tmonths[0]','$tmonths[1]','$tmonths[2]','$tmonths[3]','$tmonths[4]','$tmonths[5]','$tmonths[6]','$tmonths[7]','$tmonths[8]','$tmonths[9]','$tmonths[10]','$tmonths[11]','$tmonths[12]'];
  var dateField = null;
  function getProperty(p_property){
    var p_elm = calendarId;
    var elm = null;
    if(typeof(p_elm) == 'object'){
      elm = p_elm;
    } else {
      elm = document.getElementById(p_elm);
    }
    if (elm != null){
      if(elm.style){
        elm = elm.style;
        if(elm[p_property]){
          return elm[p_property];
        } else {
          return null;
        }
      } else {
        return null;
      }
    }
  }
  function setElementProperty(p_property, p_value, p_elmId){
    var p_elm = p_elmId;
    var elm = null;
    if(typeof(p_elm) == 'object'){
      elm = p_elm;
    } else {
      elm = document.getElementById(p_elm);
    }
    if((elm != null) && (elm.style != null)){
      elm = elm.style;
      elm[ p_property ] = p_value;
    }
  }
  function setProperty(p_property, p_value) {
    setElementProperty(p_property, p_value, calendarId);
  }
  function getDaysInMonth(year, month) {
";
	if ($date_system == 1)
		$js .= "
    return [31,31,31,31,31,31,30,30,30,30,30,(((((((year - ((year > 0) ? 474 : 473)) % 2820) + 474) + 38) * 682) % 2816) < 682 ? 30 : 29)][month-1];
";
	elseif ($date_system == 2)
		$js .= "
    return [30,29,30,29,30,29,30,29,30,29,30,(((((11 * year) + 14) % 30) < 11) ? 30 : 29)][month-1];
";
	else
		$js .= "
    return [31,((!(year % 4 ) && ( (year % 100 ) || !( year % 400 ) ))?29:28),31,30,31,30,31,31,30,31,30,31][month-1];
";
	$js .= "
  }
  function getDayOfWeek(year, month, day) {
";
	if ($date_system == 1 || $date_system == 2)
	{
		$js .= "
	function mod(a, b)
	{
	    return a - (b * Math.floor(a / b));
	}
	function jwday(j)
	{
	    return mod(Math.floor((j + 1.5)), 7);
	}
";
	if ($date_system == 1)
		$js .= "
    var epbase, epyear, t;
    epbase = year - ((year >= 0) ? 474 : 473);
    epyear = 474 + mod(epbase, 2820);
    t = day + ((month <= 7) ? ((month - 1) * 31) : (((month - 1) * 30) + 6)) +
      Math.floor(((epyear * 682) - 110) / 2816) + (epyear - 1) * 365 +
      Math.floor(epbase / 2820) * 1029983 + (1948320.5 - 1);
";
	elseif ($date_system == 2)
		$js .= "
	var t;
	t = Math.floor((11 * year + 3) / 30) + 354 * year + 30 * month -
	  Math.floor((month - 1) / 2) + day + 1948440 - 385;
";
	$js .= "
    return jwday(t);
";
	}
	else
		$js .= "
    var date = new Date(year,month-1,day)
    return date.getDay();
";
	$js .= "
  }
  this.clearDate = clearDate;
  function clearDate() {
    dateField.value = '';
    hide();
  }
  this.getWeek = getWeek;
  function getWeek(year, month, day) {
";
	if ($how == 0 || $how == 3)
		$js .= "  day++;";
	$js .= "
    var date = new Date(year,month-1,day);
    var D = date.getDay();
    if(D == 0) D = 7;
    date.setDate(date.getDate() + (4 - D));
    var YN = date.getFullYear();
    var ZBDoCY = Math.floor((date.getTime() - new Date(YN, 0, 1, -6)) / 86400000);
    var WN = 1 + Math.floor(ZBDoCY / 7);
    return WN;
  }
  this.setDate = setDate;
  function setDate(year, month, day) {
    if (dateField){
";
	if ($how < 3)
		$js .= "
      if (month < 10) {month = '0' + month;}
      if (day < 10) {day = '0' + day;}
";
	else
		$js .= "
      month = tmonths[month];
";
	if ($how == 0 || $how == 3)
		$js .= "
      var dateString = month+'$sep'+day+'$sep'+year;
";
	elseif ($how == 1 || $how == 4)
		$js .= "
      var dateString = day+'$sep'+month+'$sep'+year;
";
	elseif ($how == 2 || $how == 5)
		$js .= "
      var dateString = year+'$sep'+month+'$sep'+day;
";
	$js .= "
      dateField.value = dateString;
	  setFocus(dateField.name);
	if(dateField.getAttribute('aspect')=='cdate')
      setElementProperty('color', (dateField.value==user.date ? 'black':'red'), dateField);
	  if (dateField.className=='searchbox')
	  	dateField.onblur();
      hide();
    }
    return;
  }
  this.changeMonth = changeMonth;
  function changeMonth(change) {
    currentMonth += change;
    currentDay = 0;
    if(currentMonth > 12) {
      currentMonth = 1;
      currentYear++;
    } else if(currentMonth < 1) {
      currentMonth = 12;
      currentYear--;
    }
    calendar = document.getElementById(calendarId);
    calendar.innerHTML = calendarDrawTable();
  }
  this.changeYear = changeYear;
  function changeYear(change) {
    currentYear += change;
    currentDay = 0;
    calendar = document.getElementById(calendarId);
    calendar.innerHTML = calendarDrawTable();
  }
  function getCurrentYear() {
    var year = new Date().getYear();
    if(year < 1900) year += 1900;
    return year;
  }
  function getCurrentMonth() {
    return new Date().getMonth() + 1;
  }
  function getCurrentDay() {
    return new Date().getDate();
  }
  function calendarDrawTable() {
    var dayOfMonth = 1;
    var wstart = $wstart;
    var wno = '&nbsp;$wno&nbsp;';
    var validDay = 0;
    var startDayOfWeek = getDayOfWeek(currentYear, currentMonth, dayOfMonth);
    var daysInMonth = getDaysInMonth(currentYear, currentMonth);
    var css_class = null; //CSS class for each day
    var table = \"<table cellspacing='0' cellpadding='0' border='0'>\";
    table += \"<tr class='header'>\";
    table += \"  <td colspan='2' class='previous'><a href='javascript:changeCCMonth(-1);'>&lt;</a><br><a href='javascript:changeCCYear(-1);'>&laquo;</a></td>\";
    table += \"  <td colspan='4' class='title'>\" + months[currentMonth-1] + \"<br>\" + currentYear + \"</td>\";
    table += \"  <td colspan='2' class='next'><a href='javascript:changeCCMonth(1);'>&gt;</a><br><a href='javascript:changeCCYear(1);'>&raquo;</a></td>\";
    table += \"</tr>\";
    table += \"<tr>\";
    table += \"<th class='weekno'>\"+wno+\"</th>\";
    for (var n=0; n<7; n++)
    	table += \"<th>\" + wdays[(wstart+n)%7]+\"</th>\";
    table += \"</tr>\";
    for(var week=0; week < 6; week++) {
      table += \"<tr>\";
      for(var n=0; n < 7; n++) {
        dayOfWeek = (wstart+n)%7;
        if(week == 0 && startDayOfWeek == dayOfWeek) {
          validDay = 1;
        } else if (validDay == 1 && dayOfMonth > daysInMonth) {
          validDay = 0;
        }
        if (n==0)
        {
        	if (dayOfMonth > daysInMonth)
          		table += \"<td class='empty'>&nbsp;</td>\";
       		else
       			table += \"<td class='weekno'>\"+getWeek(currentYear, currentMonth, dayOfMonth)+\"</td>\";
        }
        if(validDay) {
          if (dayOfMonth == selectedDay && currentYear == selectedYear && currentMonth == selectedMonth) {
            css_class = 'current';
";
	if ($date_system == 1 || $date_system == 2 || $date_system == 3)
		$js .= "
          } else if (dayOfWeek == 5) {
";
	else
		$js .= "
          } else if (dayOfWeek == 0 || dayOfWeek == 6) {
";
		$js .= "
            css_class = 'weekend';
          } else {
            css_class = 'weekday';
          }
          table += \"<td><a class='\"+css_class+\"' href=\\\"javascript:setCCDate(\"+currentYear+\",\"+currentMonth+\",\"+dayOfMonth+\")\\\">\"+dayOfMonth+\"</a></td>\";
          dayOfMonth++;
        } else {
          table += \"<td class='empty'>&nbsp;</td>\";
        }
      }
      table += \"</tr>\";
    }
    table += \"<tr class='header'><th colspan='8' style='padding: 3px;text-align:center;'><a href='javascript:hideCC();'>$back</a></td></tr>\";
    table += \"</table>\";
    return table;
  }
  this.show = show;
  function show(field) {
    can_hide = 0;
    if (dateField == field) {
      return;
    } else {
      dateField = field;
    }
    if(dateField) {
      try {
        var dateString = new String(dateField.value);
        var dateParts = dateString.split('$sep');
";
	if ($how == 0)
		$js .= "
        selectedMonth = parseInt(dateParts[0],10);
        selectedDay = parseInt(dateParts[1],10);
        selectedYear = parseInt(dateParts[2],10);
";
	elseif ($how == 1)
		$js .= "
        selectedDay = parseInt(dateParts[0],10);
        selectedMonth = parseInt(dateParts[1],10);
        selectedYear = parseInt(dateParts[2],10);
";
	elseif ($how == 2)
		$js .= "
        selectedYear = parseInt(dateParts[0],10);
        selectedMonth = parseInt(dateParts[1],10);
        selectedDay = parseInt(dateParts[2],10);
";
	elseif ($how == 3)
		$js .= "
        selectedDay = parseInt(dateParts[1],10);
        selectedMonth = parseInt(tmonths.indexOf(dateParts[0]),10);
        selectedYear = parseInt(dateParts[2],10);
";
	elseif ($how == 4)
		$js .= "
        selectedDay = parseInt(dateParts[0],10);
        selectedMonth = parseInt(tmonths.indexOf(dateParts[1]),10);
        selectedYear = parseInt(dateParts[2],10);
";
	else
		$js .= "
        selectedYear = parseInt(dateParts[0],10);
        selectedMonth = parseInt(tmonths.indexOf(dateParts[1]),10);
        selectedDay = parseInt(dateParts[2],10);
";
	$js .= "
      } catch(e) {}
    }
    if (!(selectedYear && selectedMonth && selectedDay)) {
";
	if ($date_system == 1 || $date_system == 2)
	{
		$js .= "
      selectedMonth = $cmon;
      selectedDay = $cday;
      selectedYear = $cyear;
";
	}
	else
	{
		$js .= "
      selectedMonth = getCurrentMonth();
      selectedDay = getCurrentDay();
      selectedYear = getCurrentYear();
";
	}
	$js .= "
    }
    currentMonth = selectedMonth;
    currentDay = selectedDay;
    currentYear = selectedYear;
    if(document.getElementById){
      calendar = document.getElementById(calendarId);
      calendar.innerHTML = calendarDrawTable(currentYear, currentMonth);
      var fieldPos = new positionInfo(dateField);
      var calendarPos = new positionInfo(calendarId);
      var x = fieldPos.getElementLeft();
      var y = fieldPos.getElementBottom();
      setProperty('left', x + 'px');
      setProperty('top', y + 'px');
      setProperty('display', 'block');
      if (document.all) {
        setElementProperty('left', x + 'px', 'CCIframe');
        setElementProperty('top', y + 'px', 'CCIframe');
        setElementProperty('width', calendarPos.getElementWidth() + 'px', 'CCIframe');
        setElementProperty('height', calendarPos.getElementHeight() + 'px', 'CCIframe');
        setElementProperty('display', 'block', 'CCIframe');
      }
    }
  }
  this.hide = hide;
  function hide() {
    if(dateField) {
      setProperty('display', 'none');
      setElementProperty('display', 'none', 'CCIframe');
      dateField = null;
    }
  }
  this.visible = visible;
  function visible() {
    return dateField
  }
  this.can_hide = can_hide;
  var can_hide = 0;
}
var cC = new CC();
function date_picker(textField) {
  cC.show(textField);
}
function hideCC() {
  if (cC.visible()) {
    cC.hide();
  }
}
function setCCDate(year, month, day) {
  cC.setDate(year, month, day);
}
function changeCCYear(change) {
  cC.changeYear(change);
}
function changeCCMonth(change) {
  cC.changeMonth(change);
}
document.write(\"<iframe id='CCIframe' src='javascript:false;' frameBorder='0' scrolling='no'></iframe>\");
document.write(\"<div id='CC'></div>\");";

     cache_js_file($fpath, $js);
    }
    add_js_ufile($fpath);

 return '';
}

function alert($msg)
{
	echo "\n<script type=\"text/javascript\">\n"
		. "<!--\n"
		. "alert('$msg');\n"
		. "-->\n"
		. "</script>\n";
}

if (!function_exists('_vd'))
{
  	function _vd($mixed, $title = '', $exit = false)
  	{
    	$str = (!empty($title) ? ($title .':') : '') .'<pre>';
    	$str .= print_r($mixed, true); //var_dump($mixed);
    	$str .= "</pre>\n";
		display_notification('<table><tr><td>'.$str.'</td></tr></table>');
    	if ($exit)
    		exit;
  	}
}

function _vl($mixed, $title = '', $exit = false)
{
	error_log((!empty($title) ? ($title .':') : '') . var_export($mixed, true));
	if ($exit)
		exit;
}

function display_backtrace($cond=true, $msg='') {
	if ($cond) {
		if ($msg) 
			$str = "<center><span class='headingtext'>$msg</span></center>\n";
		else
			$str = '';
		$str .= get_backtrace(true);
   		display_error($str);
	}
}

//
// FIXME: $payment_services array will be moved to bank_accounts in 2.4.x
//
if (!isset($payment_services))
{
	$payment_services = array(
		'PayPal' => "https://www.paypal.com/xclick?business=<company_email>&item_name=<comment>&amount=<amount>&currency_code=<currency>",
	);
}
/*
*	Payment link generation. Options provided during invoice generation:
*	company_email, comment, amount, currency
*/
function payment_link($name, $options)
{
	global $payment_services;

	$link = @$payment_services[$name];

	if (!$link) return null;

	$patterns = array();
	foreach ($options as $id => $option)
		$patterns['<'.$id.'>'] = urlencode($options[$id]);

	return strtr($link, $patterns);
}

function get_js_enable_chk()
{
	$js ="function enable_chk()\n"
		. "{\n"
                . "var Form = document.getElementsByTagName(\"input\");\n"
                . "for(I = 0; I < Form.length; I++)  {\n"
                . " if (Form[I].type == 'checkbox') {\n"
                . " var Name = Form[I].getAttribute('name');\n"
                . " var Value = Form[I].value;\n"
                //. "  alert(Name + ' : ' + Value);\n"
                . " Form[I].checked = true;\n"
                . " }\n"
                . " }\n"
		. "}\n";
	return $js;
}

function get_js_disable_chk()
{
	$js ="function disable_chk()\n"
		. "{\n"
                . "var Form = document.getElementsByTagName(\"input\");\n"
                . "for(I = 0; I < Form.length; I++)  {\n"
                . " if (Form[I].type == 'checkbox') {\n"
                . " var Name = Form[I].getAttribute('name');\n"
                . " var Value = Form[I].value;\n"
                //. "  alert(Name + ' : ' + Value);\n"
                . " Form[I].checked = false;\n"
                . " }\n"
                . " }\n"
		. "}\n";
	return $js;
}


function create_client_socket(){   
    
    $js = " var socket;\n";
    $js .= "function init(cmd1)\n"
	. "{\n ";
    $js .= "var host = 'ws://127.0.0.1:9001/echobot'; // SET THIS TO YOUR SERVER";
    $js .= "\n try {\n
	    socket = new WebSocket(host);\n";
    $js .= "console.log('WebSocket - status '+socket.readyState);\n";
    $js .= "socket.onopen    = function(msg) {\n                 
					         console.log('Welcome - status '+this.readyState);\n 
            if (cmd1 == null) {\n                                     
              alert('Comando no enviado');\n
            }\n                                                 
            else
            if (this.readyState != 1) {\n                                     
              alert('No pudo conectarse. '+'Disconnected - status '+this.readyState);\n
            }\n
            else
            if (this.readyState == 1 && cmd1 != null) {\n                                     
              send(cmd1);\n
            }\n
						 };\n
		socket.onmessage = function(msg) {\n 
						 console.log('Received: '+msg.data);\n 
						 };\n
		socket.onclose   = function(msg) {\n 
                                                 alert('No pudo conectarse al socket. '+'Disconnected - status '+this.readyState);\n
						 console.log('Disconnected - status '+this.readyState);\n 
						 };\n";
    $js .= " }\n
	catch(ex){\n 
		console.log(ex);\n 
	}\n
	//$('msg').focus();\n";
    $js .= " }\n";
    $js .= "function send(msg){\n";
    $js .= "try {\n 
		socket.send(msg);\n 
		console.log('Sent: '+msg);\n 
	} catch(ex) {\n 
		console.log(ex);\n 
	}\n";
    $js .= "}\n";
    $js .= " function quit(){\n
	if (socket != null) {\n
		console.log('Goodbye!');\n
		socket.close();\n
		socket=null;\n
	}\n
    }\n";
    $js .= " function reconnect() {\n
	quit();\n
	init();\n
    }\n"; 
    return $js;
}

?>