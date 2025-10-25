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
//include_once($path_to_root . "/includes/reserved.inc");

function set_global_supplier($supplier_id) 
{
	$_SESSION['wa_global_supplier_id'] = $supplier_id; 
}

function get_global_supplier($return_all=true) 
{
	if (!isset($_SESSION['wa_global_supplier_id']) || 
		($return_all == false && $_SESSION['wa_global_supplier_id'] == ALL_TEXT))
		return "";	
	return $_SESSION['wa_global_supplier_id'];
}

function set_global_stock_item($stock_id) 
{
	$_SESSION['wa_global_stock_id'] = $stock_id; 
}

function get_global_stock_item($return_all=true) 
{
	if (!isset($_SESSION['wa_global_stock_id']) || 
		($return_all == false && $_SESSION['wa_global_stock_id'] == ALL_TEXT))
		return "";		
	return $_SESSION['wa_global_stock_id'];
}

function set_global_customer($customer_id) 
{
	$_SESSION['wa_global_customer_id'] = $customer_id; 
}

function get_global_customer($return_all=true) 
{
	if (!isset($_SESSION['wa_global_customer_id']) || 
		($return_all == false && $_SESSION['wa_global_customer_id'] == ALL_TEXT))
		return "";
	return $_SESSION['wa_global_customer_id'];
}

function set_global_curr_code($curr_code) 
{
	$_SESSION['wa_global_curr_code'] = $curr_code;
}

function get_global_curr_code() 
{
	if (!isset($_SESSION['wa_global_curr_code']))
		return "";
	return $_SESSION['wa_global_curr_code'];
}
/**/

/*Begin Felix Alberti 06/01/2015*/
function set_global_account($account) 
{
	$_SESSION['wa_global_account'] = $account; 
}

function get_global_account($return_all=true) 
{
	if (!isset($_SESSION['wa_global_account']) || 
		($return_all == false && $_SESSION['wa_global_account'] == ALL_TEXT))
		return "";
	return $_SESSION['wa_global_account'];
}
/*End Felix Alberti 06/01/2015*/

/*Begin Felix Alberti 19/04/2016*/

/*Begin Felix Alberti 19/04/2016*/
function set_global_payment_selected($payment) 
{
	$_SESSION['wa_global_payment_selected'] = $payment; 
}
function get_global_payment_selected($return_all=true) 
{
	if (!isset($_SESSION['wa_global_payment_selected']))
		return 0;
	return $_SESSION['wa_global_payment_selected'];
}
/*End Felix Alberti 19/04/2016*/

/*Begin Felix Alberti 03/05/2016*/
function set_global_type_transac_selected($transac) 
{
	$_SESSION['wa_global_type_transac_selected'] = $transac; 
}
function get_global_type_transac_selected($return_all=true) 
{
	if (!isset($_SESSION['wa_global_type_transac_selected']))
		return "";
	return $_SESSION['wa_global_type_transac_selected'];
}
/*End Felix Alberti 03/05/2016*/

/*Begin Felix Alberti 03/05/2016*/
function set_global_type_method_selected($payment) 
{
	$_SESSION['wa_global_type_method_selected'] = $payment; 
}
function get_global_type_method_selected($return_all=true) 
{
	if (!isset($_SESSION['wa_global_type_method_selected']))
		return "";
	return $_SESSION['wa_global_type_method_selected'];
}
/*End Felix Alberti 03/05/2016*/

/*Begin Felix Alberti 25/04/2016*/
function set_global_debtor_selected($debtor) 
{
	$_SESSION['wa_current_debtor_selected'] = $debtor; 
}
function get_global_debtor_selected($return_all=true) 
{
	if (!isset($_SESSION['wa_current_debtor_selected']))
		return "";
	return $_SESSION['wa_current_debtor_selected'];
}
/*End Felix Alberti 25/04/2016*/

/*Begin Felix Alberti 14/05/2016*/
function set_global_payment_terms($debtor) 
{
	$_SESSION['wa_current_payment_terms_selected'] = $debtor; 
}
function get_global_payment_terms($return_all=true) 
{
	if (!isset($_SESSION['wa_current_payment_terms_selected']))
		return "";
	return $_SESSION['wa_current_payment_terms_selected'];
}
/*End Felix Alberti 14/05/2016*/
//--------------------------------------------------------------------------------------

/*Begin Felix Alberti 19/03/2016*/
/*function set_global_responsible($responsible) 
{
	$_SESSION['wa_global_responsible'] = $responsible;
}

function get_global_responsible($return_all=true) 
{
	if (!isset($_SESSION['wa_global_responsible']) || 
		($return_all == false && $_SESSION['wa_global_responsible'] == ALL_TEXT))
		return "";
	return $_SESSION['wa_global_responsible'];
}*/
/*End Felix Alberti 19/03/2016*/
//--------------------------------------------------------------------------------------
/*Begin Felix Alberti 21/06/2016*/
function set_global_date_from($from) 
{
	$_SESSION['wa_current_last_date_from'] = $from; 
}
function get_global_date_from($return_all=true) 
{
	if (!isset($_SESSION['wa_current_last_date_from']))
		return "";
	return $_SESSION['wa_current_last_date_from'];
}
function set_global_date_to($to) 
{
	$_SESSION['wa_current_last_date_to'] = $to; 
}
function get_global_date_to($return_all=true) 
{
	if (!isset($_SESSION['wa_current_last_date_to']))
		return "";
	return $_SESSION['wa_current_last_date_to'];
}
/*End Felix Alberti 21/06/2016*/
//-----------------------------------------------------------
/*Begin Felix Alberti 07/10/2016*/
function set_global_description($description) 
{
	$_SESSION['wa_global_description'] = $description; 
}
function get_global_description($return_all=true) 
{
	if (!isset($_SESSION['wa_global_description']))
		return "";
	return $_SESSION['wa_global_description'];
}
/*End Felix Alberti 07/10/2016*/

/*Begin Felix Alberti 24/10/2016*/
function set_global_reference($ref) 
{
	$_SESSION['wa_global_reference'] = $ref; 
}
function get_global_reference($return_all=true) 
{
	if (!isset($_SESSION['wa_global_reference']))
		return "";
	return $_SESSION['wa_global_reference'];
}
/*End Felix Alberti 24/10/2016*/
?>