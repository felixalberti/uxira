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
$path_to_root = "../..";

include_once($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/sales/includes/sales_ui.inc");
include_once($path_to_root . "/reporting/includes/reporting.inc");
require_once($path_to_root . "/inventory/includes/paginator.php");

$page_security = 'SA_SALESTRANSVIEW';

set_page_security( @$_POST['order_view_mode'],
	array(	'OutstandingOnly' => 'SA_SALESDELIVERY',
			'InvoiceTemplates' => 'SA_SALESINVOICE'),
	array(	'OutstandingOnly' => 'SA_SALESDELIVERY',
			'InvoiceTemplates' => 'SA_SALESINVOICE')
);

if (get_post('type'))
	$trans_type = $_POST['type'];
elseif (isset($_GET['type']) && $_GET['type'] == ST_SALESQUOTE)
	$trans_type = ST_SALESQUOTE;
else
	$trans_type = ST_SALESORDER;

if ($trans_type == ST_SALESORDER)
{
	if (isset($_GET['OutstandingOnly']) && ($_GET['OutstandingOnly'] == true))
	{
		$_POST['order_view_mode'] = 'OutstandingOnly';
		$_SESSION['page_title'] = _($help_context = "Search Outstanding Sales Orders");
	}
	elseif (isset($_GET['InvoiceTemplates']) && ($_GET['InvoiceTemplates'] == true))
	{
		$_POST['order_view_mode'] = 'InvoiceTemplates';
		$_SESSION['page_title'] = _($help_context = "Search Template for Invoicing");
	}
	elseif (isset($_GET['DeliveryTemplates']) && ($_GET['DeliveryTemplates'] == true))
	{
		$_POST['order_view_mode'] = 'DeliveryTemplates';
		$_SESSION['page_title'] = _($help_context = "Select Template for Delivery");
	}
	elseif (!isset($_POST['order_view_mode']))
	{
		$_POST['order_view_mode'] = false;
		$_SESSION['page_title'] = _($help_context = "Search All Sales Orders");
	}
}
else
{
	$_POST['order_view_mode'] = "Quotations";
	$_SESSION['page_title'] = _($help_context = "Search All Sales Quotations");
}

if (!@$_GET['popup'])
{
        
        global $js_userlib, $js_static;
        $js_static = array();
        $js_userlib = array(
            $path_to_root.'/js/jquery-2.0.3.min.js',$path_to_root.'/js/bootstrap-editable.min.js'
            );
        
	$js = "";
	if ($use_popup_windows)
		$js .= get_js_open_window(900, 600);
//	if ($use_date_picker)
//		$js .= get_js_date_picker();
        //add_files_jquery();
        
//        $js .= '<script language="javascript" type="text/javascript" src="'.
////			$path_to_root.'/js/jquery-1.7.2.min.js' . '"></script>';
//        $js .= '<script language="javascript" type="text/javascript" src="'.
//			$path_to_root.'/js/jquery-ui-1.11.0.js' . '"></script>';
//        $js .= '<script language="javascript" type="text/javascript" src="'.
//			$path_to_root.'/js/jquery.jeditable.js' . '"></script>';
        $src_loading = company_path()."/images";
//        $js .= "
//		$(function() {
//			  $('.id_invent').editable('facdetalles/update', {
//				 indicator : \"<img src='".$src_loading."/loading.gif'>\",
//				 id        : 'data[FacDetalle][id]',
//				 name      : 'data[FacDetalle][id_invent]',
//				 type      : 'select',
//				 data      : ".json_encode($descriArticulos).",
//				 cancel    : 'Cancelar',
//				 submit    : 'Guardar',
//				 tooltip   : 'Click para editar el articulo',
//				 method    : 'POST',
//			});
//		});
//	  ";
        $js .= "
		$(function() {
			  $('.descri').editable('".$path_to_root."/inventory/items_update_descri.php', {
				 indicator : \"<img src='".$src_loading."/loading.gif'>\",
				 id        : 'descri',
				 name      : 'descri',
				 type      : 'editlink',
				 data      : function(string) {return $.trim(string)},
				 tooltip   : 'Click para editar la cantidad',
				 method    : 'POST',
				 cssclass  : 'cantidades',
				 width     : 120
			});
		});
	  ";
        
//        $js = "jQuery.exists = function(selector) {return ($(selector).length > 0);}
//if ($.exists('#pag')) { alert('finito') }";
        //add_js_ufile($fpath);
        //$js .= get_js_date_picker();
	page($_SESSION['page_title'], false, false, "", $js);
}

if (isset($_GET['selected_customer']))
{
	$selected_customer = $_GET['selected_customer'];
}
elseif (isset($_POST['selected_customer']))
{
	$selected_customer = $_POST['selected_customer'];
}
else
	$selected_customer = -1;

//---------------------------------------------------------------------------------------------

if (isset($_POST['SelectStockFromList']) && ($_POST['SelectStockFromList'] != "") &&
	($_POST['SelectStockFromList'] != ALL_TEXT))
{
 	$selected_stock_item = $_POST['SelectStockFromList'];
}
else
{
	unset($selected_stock_item);
}
//---------------------------------------------------------------------------------------------
//	Query format functions
//
function check_overdue($row)
{
	global $trans_type;
	if ($trans_type == ST_SALESQUOTE)
		return (date1_greater_date2(Today(), sql2date($row['delivery_date'])));
	else
		return ($row['type'] == 0
			&& date1_greater_date2(Today(), sql2date($row['delivery_date']))
			&& ($row['TotDelivered'] < $row['TotQuantity']));
}

function view_link($dummy, $order_no)
{
	global $trans_type;
	return  get_customer_trans_view_str($trans_type, $order_no);
}

function prt_link($row)
{    
	global $trans_type;
	return print_document_link($row['stock_id'], _("Print"), true, $trans_type, ICON_PRINT);
}

function edit_link($row) 
{
	if (@$_GET['popup'])
		return '';
	//global $trans_type;
	$modify = "ModifyStock_id";
  return pager_link( _("Edit"),
    "/inventory/items_update.php?$modify=" . $row['stock_id'], ICON_EDIT);
}

function dispatch_link($row)
{
	global $trans_type;
	if ($trans_type == ST_SALESORDER)
  		return pager_link( _("Dispatch"),
			"/sales/customer_delivery.php?OrderNumber=" .$row['order_no'], ICON_DOC);
	else		
  		return pager_link( _("Sales Order"),
			"/sales/sales_order_entry.php?OrderNumber=" .$row['order_no'], ICON_DOC);
}

function invoice_link($row)
{
	global $trans_type;
	if ($trans_type == ST_SALESORDER)
  		return pager_link( _("Invoice"),
			"/sales/sales_order_entry.php?NewInvoice=" .$row["order_no"], ICON_DOC);
	else
		return '';
}

function delivery_link($row)
{
  return pager_link( _("Delivery"),
	"/sales/sales_order_entry.php?NewDelivery=" .$row['order_no'], ICON_DOC);
}

function order_link($row)
{
  /*Begin 30/12/2015 Felix Alberti template QuoteToNewQuote*/
  if (isset($_GET['QuoteToNewQuote']) || isset($_POST['QuoteToNewQuote']))
  return pager_link( _("New Quotation"),
	"/sales/sales_order_entry.php?QuoteToNewQuote=" .$row['order_no'], ICON_DOC);    
  else
  /*End 30/12/2015 Felix Alberti template QuoteToNewQuote*/
  return pager_link( _("Sales Order"),
	"/sales/sales_order_entry.php?NewQuoteToSalesOrder=" .$row['order_no'], ICON_DOC);
}

function tmpl_checkbox($row)
{
	//global $trans_type;
//	if ($trans_type == ST_SALESQUOTE)
//		return '';
//	if (@$_GET['popup'])
//		return '';
	$name = "chgtpl" .$row['stock_id'];
	//$value = $row['type'] ? 1:0;
        $value = $row['stock_id'];

// save also in hidden field for testing during 'Update'

 return checkbox(null, $name, $value, true,
 	_('Set this order as a template for direct deliveries/invoices'))
	. hidden('last['.$row['stock_id'].']', $value, false);
}
//---------------------------------------------------------------------------------------------
// Update db record if respective checkbox value has changed.
//
function change_tpl_flag($id)
{
	global	$Ajax;
	
  	$sql = "UPDATE ".TB_PREF."sales_orders SET type = !type WHERE order_no=$id";

  	db_query($sql, "Can't change sales order type");
	$Ajax->activate('orders_tbl');
}

$id = find_submit('_chgtpl');
if ($id != -1)
	change_tpl_flag($id);

if (isset($_POST['Update']) && isset($_POST['last'])) {
	foreach($_POST['last'] as $id => $value)
		if ($value != check_value('chgtpl'.$id))
			change_tpl_flag($id);
}

$show_dates = !in_array($_POST['order_view_mode'], array('OutstandingOnly', 'InvoiceTemplates', 'DeliveryTemplates'));
//---------------------------------------------------------------------------------------------
//	Order range form
//
if (get_post('_OrderNumber_changed') || get_post('_OrderReference_changed')) // enable/disable selection controls
{
	$disable = get_post('OrderNumber') !== '' || get_post('OrderReference') !== '';

  	if ($show_dates) {
			$Ajax->addDisable(true, 'OrdersAfterDate', $disable);
			$Ajax->addDisable(true, 'OrdersToDate', $disable);
	}

	$Ajax->activate('orders_tbl');
}

if (!@$_GET['popup'])
	start_form();

start_table(TABLESTYLE_NOBORDER);
start_row();
//ref_cells(_("#:"), 'OrderNumber', '',null, '', true);
//ref_cells(_("Ref"), 'OrderReference', '',null, '', true);
//if ($show_dates)
//{
//  	date_cells(_("from:"), 'OrdersAfterDate', '', null, -30);
//  	date_cells(_("to:"), 'OrdersToDate', '', null, 1);
//}
//locations_list_cells(_("Location:"), 'StockLocation', null, true, true);

//if($show_dates) {
//	end_row();
//	end_table();
//
//	start_table(TABLESTYLE_NOBORDER);
//	start_row();
//}
stock_items_list_cells(_("Item:"), 'SelectStockFromList', null, true, true);
//if (!@$_GET['popup'])
//	customer_list_cells(_("Select a customer: "), 'customer_id', null, true, true);
//if ($trans_type == ST_SALESQUOTE)
//	check_cells(_("Show All:"), 'show_all');

submit_cells('SearchOrders', _("Search"),'',_('Select documents'), 'default');
hidden('order_view_mode', $_POST['order_view_mode']);
hidden('type', $trans_type);

end_row();

end_table(1);
//---------------------------------------------------------------------------------------------
//	Orders inquiry table
//
//$sql = get_sql_for_sales_orders_view($selected_customer, $trans_type, $_POST['OrderNumber'], $_POST['order_view_mode'],
//	@$selected_stock_item, @$_POST['OrdersAfterDate'], @$_POST['OrdersToDate'], @$_POST['OrderReference'], $_POST['StockLocation'], $_POST['customer_id']);

$sql = "SELECT stock_id, category_id, description, long_description, units, mb_flag,"
        . " dimension_id, inactive FROM ".TB_PREF."stock_master";

function get_items_local($selected_stock_item=false)
{
	$sql = "SELECT stock_id, category_id, description, long_description, units, mb_flag,"
        . " dimension_id, inactive FROM ".TB_PREF."stock_master";
	//if (!$all) $sql .= " AND !u.inactive";
        if ($selected_stock_item)
        $sql .= " WHERE stock_id = ".  db_escape($selected_stock_item);
        //display_notification($sql);
	
	return db_query($sql, "could not get items");
}

if (isset($selected_stock_item)){
$result = get_items_local($selected_stock_item);
$Ajax->activate('_page_body');
}
else
$result = get_items_local(false);

//if (isset($selected_stock_item)) $sql .= " WHERE stock_id = ".  db_escape($selected_stock_item);


//    $cols = array(
//		_("Stock_id #"),
//		_("Category"),
//		_("Description"),
//		_("Long Description"),
//		_("Units"),
//		_("Flag"), 
//		_("Dimension 1"),
//                _("Inactive")
//	);
//
//if ($_POST['order_view_mode'] == 'OutstandingOnly') {
//	//array_substitute($cols, 4, 1, _("Cust Order Ref"));
//	array_append($cols, array(
//		array('insert'=>true, 'fun'=>'dispatch_link'),
//		array('insert'=>true, 'fun'=>'edit_link')));
//
//} elseif ($_POST['order_view_mode'] == 'InvoiceTemplates') {
//	array_substitute($cols, 4, 1, _("Description"));
//	array_append($cols, array( array('insert'=>true, 'fun'=>'invoice_link')));
//
//} else if ($_POST['order_view_mode'] == 'DeliveryTemplates') {
//	array_substitute($cols, 4, 1, _("Description"));
//	array_append($cols, array(
//			array('insert'=>true, 'fun'=>'delivery_link'))
//	);
//} elseif ($trans_type == ST_SALESQUOTE) {
//	 array_append($cols,array(
//					array('insert'=>true, 'fun'=>'edit_link'),
//					array('insert'=>true, 'fun'=>'order_link'),
//					array('insert'=>true, 'fun'=>'prt_link')));
//} elseif ($trans_type == ST_SALESORDER) {
//	 array_append($cols,array(
//			_("Tmpl") => array('insert'=>true, 'fun'=>'tmpl_checkbox'),
//					array('insert'=>true, 'fun'=>'edit_link'),
//					array('insert'=>true, 'fun'=>'prt_link')));
//};
//
//
//$table =& new_db_pager('orders_tbl', $sql, $cols);
////$table->set_marker('check_overdue', _("Marked items are overdue."));
//
//$table->width = "80%";
//
//display_db_pager($table);
//submit_center('Update', _("Update"), true, '', null);
//
///*Begin 30/12/2015 Felix Alberti template QuoteToNewQuote*/
//if (isset($_GET['QuoteToNewQuote']))
//    hidden("QuoteToNewQuote", $_GET['QuoteToNewQuote']);
//elseif (isset($_POST['QuoteToNewQuote']))
//    hidden("QuoteToNewQuote", $_POST['QuoteToNewQuote']);
///*End 30/12/2015 Felix Alberti template QuoteToNewQuote*/


//start_form();
start_table(TABLESTYLE);

$th = array(_("Stock id"), _("Category"), _("Description"),
	_("Long Description"), _("Units"), _("Flag"), _("Dimension id"), "", "");

inactive_control_column($th);
table_header($th);	

$k = 0; //row colour counter

while ($myrow = db_fetch($result)) 
{

	alt_table_row_color($k);
        label_cell($myrow["stock_id"]);
	label_cell($myrow["category_id"]);
        echo "<div class='descri'>";
	label_cell($myrow["description"]);
        echo "</div>";
        label_cell($myrow["long_description"]);
        label_cell($myrow["units"]);
        label_cell($myrow["mb_flag"]);
        label_cell($myrow["dimension_id"]);
        edit_button_cell("Edit".$myrow["stock_id"], _("Edit"));
        delete_button_cell("Delete".$myrow["stock_id"], _("Delete"));
 end_row();

} //END WHILE LIST LOOP

end_table(1);

br();

    $limit      = ( isset( $_GET['limit'] ) ) ? $_GET['limit'] : 10;
    $page       = ( isset( $_GET['page'] ) ) ? $_GET['page'] : 1;
    $links      = ( isset( $_GET['links'] ) ) ? $_GET['links'] : 1;
    $query      = "SELECT * FROM 0_stock_master";
 
    $Paginator  = new Paginator( $db, $query );
 
    $results    = $Paginator->getData( $page, $limit );
    
start_table(TABLESTYLE);

$th = array(_("Stock id"), _("Category"), _("Description"),
	_("Long Description"), _("Units"), _("Flag"), _("Dimension id"), "", "");

inactive_control_column($th);
table_header($th);	
  
$k = 0; //row colour counter
    for( $i = 0; $i < count( $results->data ); $i++ ) :
        alt_table_row_color($k);
        label_cell($results->data[$i]["stock_id"]);
	label_cell($results->data[$i]["category_id"]);
	label_cell($results->data[$i]["description"]);
        label_cell($results->data[$i]["long_description"]);
        label_cell($results->data[$i]["units"]);
        label_cell($results->data[$i]["mb_flag"]);
        label_cell($results->data[$i]["dimension_id"]);
        edit_button_cell("Edit".$results->data[$i]["stock_id"], _("Edit"));
        delete_button_cell("Delete".$results->data[$i]["stock_id"], _("Delete"));
        end_row();
    endfor;
    start_row();
echo "<td colspan='9'>";
div_start("pag");
echo $Paginator->createLinks( $links, 'pagination pagination-sm' );
div_end();
echo "</td>";
end_row();

end_table(1);
       
        
if (!@$_GET['popup'])
{
	end_form();
	end_page();
}
?>
