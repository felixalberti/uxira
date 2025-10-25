<?php
/**********************************************************************
    Copyright 2016 (C) Uxira, C.A.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
$page_security = 'SA_BATCHCHANGEPRICE';
if (!@$_GET['popup'])
	$path_to_root = "..";
else	
	$path_to_root = "../..";

include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/sales/includes/sales_db.inc");
include_once($path_to_root . "/sales/includes/db/sales_types_db.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/data_checks.inc");
include_once($path_to_root . "/inventory/includes/inventory_db.inc");


//display_notification('0 item: '.$_POST['stock_id']);

if (!@$_GET['popup']) {
        if (isset($_GET['without_menu']))
        page(_($help_context = "Batch change prices"),true);
        else
	page(_($help_context = "Batch change prices"));
}

//---------------------------------------------------------------------------------------------------

check_db_has_stock_items(_("There are no items defined in the system."));

check_db_has_sales_types(_("There are no sales types in the system. Please set up sales types befor entering pricing."));

simple_page_mode(true);
//---------------------------------------------------------------------------------------------------
$input_error = 0;

//if (isset($_GET['stock_id']))
//{
	//$_POST['stock_id'] = $_GET['stock_id'];
//}

//display_notification('1 item: '.$_POST['stock_id']);

if (isset($_GET['Item']))
{
	$_POST['stock_id'] = $_GET['Item'];
}

if (!isset($_POST['curr_abrev']))
{
	$_POST['curr_abrev'] = get_company_currency();
}

//---------------------------------------------------------------------------------------------------
if (!@$_GET['popup'])
	start_form();

if (!isset($_POST['stock_id']))
	$_POST['stock_id'] = get_global_stock_item();

$new_item = get_post('stock_id')=='';



if (list_updated('group_sel') || list_updated('dimension_id_sel') || list_updated('category_id_sel')){
    $_POST['stock_id'] = '';
}

//display_notification('2 item: '.$_POST['stock_id']);

if (!@$_GET['popup'])
{
        start_table(TABLESTYLE, "width=30%");
        
        //stock_categories_list_row(_("Category:"), 'category_id', null, false, $new_item, 'description');
        
        start_row();
        group_dimensions_list_cells(_("Groups:"), 'group_sel', null, true, _("No Groups Filter"), true);
        //group_dimensions_list_cells($label, $name, $selected_id, $no_option, $showname, $submit_on_change)
        dimensions_group_list_cells(_("Dimension")." 1:", 'dimension_id_sel', null, true, _("No Dimension Filter"), false, 0, true, @$_POST['group_sel']);
        end_row();        
        
        start_row();        
        stock_categories_dimension_list_cells(_("Category:"), 'category_id_sel', null, _("No Category Filter"), true, 'description', false);             
        
        stock_items_category_list_cells(_("Select an item:"), 'stock_id', null,
        /*End Felix Alberti 08/07/2016*/
	  null, true, check_value('show_inactive'), false, @$_POST['category_id_sel'], @$_POST['dimension_id_sel']);
        end_row();        
        
        //display_notification('3 item '.$_POST['stock_id']);
        
        start_row();
        small_amount_cells(_("Multiply by:"), 'multiplyby', null, '', null);
        
        submit_cells('recalcule', _("Recalcule"),'',_('Recalcule price'), 'default');
        
        end_row();
        
        
        
        end_table();
        
        br();
        
	//echo "<center>" . _("Item:"). "&nbsp;";
	//echo sales_items_list('stock_id', $_POST['stock_id'], false, true, '', array('editable' => false));    
	//echo "<hr></center>";
}
else
	br(2);
set_global_stock_item($_POST['stock_id']);

//----------------------------------------------------------------------------------------------------
function change_price_item($selected_id,$sales_type_sel,$curr_abrev_sel,$multiplyby) {
    //if ($selected_id == '0011071')
    $myrow_company = get_company_prefs();
    $base_sales = $myrow_company["base_sales"];
    if ($base_sales==$sales_type_sel){
        $res_sales_type = get_all_sales_types(true);
        while ($row_sales_type = db_fetch($res_sales_type)){ 
            $sales_type = $row_sales_type['id'];            
            $myrow = get_stock_price_type_currency($selected_id,$sales_type,$curr_abrev_sel);
            $price_new = $myrow['price'] * $multiplyby;
            update_item_price_by_stock_id($selected_id, $sales_type, $curr_abrev_sel, $price_new);
        }
    }
}


function change_price_kit($selected_id,$sales_type_sel,$curr_abrev_sel,$price_new) {	
    //display_notification('change_price_kit');
//			update_item_price($selected_id, $sales_type_sel, $curr_abrev_sel, $price_new);
                        //if ($selected_id == '0011071') {
                        $myrow = get_stock_price_type_currency($selected_id,$sales_type_sel,$curr_abrev_sel); 
                        //display_notification('** '.$selected_id.' - stock_id: '.$myrow['stock_id']);
                        $item_row = get_item($myrow['stock_id']);
                        $price_item = $price_new;//Precio dado por pantalla
//                        if ($item_row['recalculate_price']==1){
                            $myrow_company = get_company_prefs();
                            $base_sales = $myrow_company["base_sales"];
                            
                            if ($base_sales==$sales_type_sel){
                                $res_sales_type = get_all_sales_types(true);
                                while ($row_sales_type = db_fetch($res_sales_type)){                                    
                                    $sales_type = $row_sales_type['id'];
                                    $factor = $row_sales_type['factor'];
                                    $res_affected_kits = get_sum_component_kit($myrow['stock_id'],$sales_type,$myrow['curr_abrev']);
                                    //display_notification($selected_id.' - stock_id: '.$myrow['stock_id'].' - ('.$sales_type.') - ('.$myrow['curr_abrev'].') '.$base_sales.' <-> '.$sales_type_sel);
                                    while ($row = db_fetch($res_affected_kits)) { 
                                       $display_price =  ($price_item*$factor)+$row['price']; 
                                       //display_notification('1. - Kit: '.$row['item_code'].' - '.$sales_type.' - Price: '.$display_price);
                                       //Actualiza el precio en cada kit que este presente el item
                                       update_item_price_by_stock_id($row['item_code'], $sales_type,
                                       $curr_abrev_sel, (($price_item*$factor)+$row['price']));
                                    }
                                }
                            }
//                            else {
//                                $res_affected_kits = get_sum_component_kit($myrow['stock_id'],$myrow['sales_type_id'],$myrow['curr_abrev']);                                    
//                                while ($row = db_fetch($res_affected_kits)) {                            
//                                       display_notification('2. - '.$row['item_code'].' - '.$row['price']);
//                                       //Actualiza el precio en cada kit que este presente el item
////                                       update_item_price_by_stock_id($row['item_code'], $sales_type_sel,
////                                       $curr_abrev_sel, ($price_item+$row['price']));
//                                }
//                            }
                            //display_notification(_("This price has been recalculated in each kit."));
//                        }
                       
//			$msg = _("This price has been updated.");
//		
                       //}
//	
//		display_notification($msg);

}

//------------------------------------------------------------------------------------------------------

//$show_price_kits = 0;
//if (list_updated('stock_id')) {
//	$Ajax->activate('price_table');
//	$Ajax->activate('price_details');
//}

if (list_updated('stock_id') || isset($_POST['stock_id']) || isset($_POST['process'])) {
	$Ajax->activate('price_table');	
}


//---------------------------------------------------------------------------------------------------

//$prices_list = get_prices($_POST['stock_id']);
$myrow_company = get_company_prefs();
$base_sales = $myrow_company["base_sales"];
//
//
if (isset($_POST['recalcule']) || isset($_POST['process']))
$item_by_filters = get_items_by_filters($_POST['category_id_sel'],$base_sales,$_POST['stock_id']);

div_start('price_table');
start_table(TABLESTYLE, "width=30%");

$th = array(_("Item"), _("Category"), _("Sales Type"), _("Curr Abrev"), _("Price"), _("New Price"), _("Kit"));
table_header($th);
$k = 0; //row colour counter
$calculated = false;
$updated_item = 0;
$updated_kits = 0;
if (isset($item_by_filters) && db_num_rows($item_by_filters) > 0){

    while ($mylistrow = db_fetch($item_by_filters))
    {

            alt_table_row_color($k);

        label_cell($mylistrow["stock_id"]);
        label_cell($mylistrow["category_id"]);
        label_cell($mylistrow["sales_type_id"]);
        label_cell($mylistrow["curr_abrev"]);
        amount_cell($mylistrow["price"]);
        $multiply_by = user_numeric($_POST['multiplyby']);
        $price_new = $mylistrow["price"]*$multiply_by;
        amount_cell($price_new);
        if (isset($_POST['process']) && $multiply_by > 0){
            change_price_item($mylistrow["stock_id"],$mylistrow["sales_type_id"],$mylistrow["curr_abrev"],$multiply_by);
            $updated_item = 1;
        }
        //
        $kits = get_where_used($mylistrow["stock_id"]);
	$num_kits = db_num_rows($kits);
	if ($num_kits) {
            $msg = '';
            while($num_kits--) {
				$kit = db_fetch($kits);
				$msg .= "'".$kit[0]."'";
                                if (isset($_POST['process']) && $multiply_by > 0) {
                                   change_price_kit($mylistrow["stock_id"],$mylistrow["sales_type_id"],$mylistrow["curr_abrev"],$price_new);
                                   $updated_kits = 1;
                                }
				if ($num_kits) $msg .= ',';
			} 
            label_cell($msg);            
        }
        else
        label_cell("N/A");
    // 	edit_button_cell("Edit".$myrow['id'], _("Edit"));
    // 	delete_button_cell("Delete".$myrow['id'], _("Delete"));
        end_row();

    }
}
end_table();

if ($updated_item==1)
display_notification(_("This price has been update in all items indicated."));

if ($updated_kits==1)
display_notification(_("This price has been recalculated and updated in each kit."));

//if (db_num_rows($prices_list) == 0)
//{
//	if (get_company_pref('add_pct') != -1)
//		$calculated = true;
//	display_note(_("There are no prices set up for this part."), 1);
//}
div_end();
//------------------------------------------------------------------------------------------------

echo "<br>";



hidden('selected_id', $selected_id);
if (@$_GET['popup'])
{
	hidden('_tabs_sel', get_post('_tabs_sel'));
	hidden('popup', @$_GET['popup']);
}
//div_start('price_details');
//start_table(TABLESTYLE2);

//currencies_list_row(_("Currency:"), 'curr_abrev', null, true);
//
//sales_types_list_row(_("Sales Type:"), 'sales_type_id', null, true);
//
//if (!isset($_POST['price'])) {
//	$_POST['price'] = price_format(get_kit_price(get_post('stock_id'), 
//		get_post('curr_abrev'),	get_post('sales_type_id')));
//        //display_notification('Price::: '.$_POST['price']);
//}

//$kit = get_item_code_dflts($_POST['stock_id']);
//small_amount_row(_("Price:"), 'price', null, '', _('per') .' '.$kit["units"]);
//
//end_table(1);
//if ($calculated)
//	display_note(_("The price is calculated."), 0, 1);

//submit_add_or_update_center($selected_id == -1, '', 'both');
submit_center('process', _("Process"), true, '',  'default');
//div_end();

//echo "<br>";
//
//div_start('items_kits');
//if ($show_price_kits == 1)
//{
////Begin Felix Alberti 23/08/2016
//        start_table(TABLESTYLE2);
//        sales_types_list_row(_("Sales Type:"), 'sales_type_id_sel', null, true);
//        end_table(1);
////End Felix Alberti 23/08/2016    
//    
//echo '<br>';
//       
//$sql = "SELECT i.item_code, i.description as descript_kit, i.stock_id, sm.description, sm.source_code, sm.our_code, p.price, s.sales_type, i.quantity 
//    FROM ".TB_PREF."sales_types s, ".TB_PREF."item_codes i left join ".TB_PREF."stock_master sm ON
//        (sm.stock_id = i.stock_id) left join ".TB_PREF."prices p 
//        ON ( p.stock_id = i.stock_id and p.sales_type_id = ".db_escape($_POST['sales_type_id_sel'])." ) where i.item_code = ".db_escape($_POST['stock_id']).
//        " and s.id = ".db_escape($_POST['sales_type_id_sel']).
//		" ORDER BY s.id, i.stock_id";	
//	
//$res = db_query($sql,"item prices could not be retreived");
//
//start_table(TABLESTYLE, "width=30%");
//$th = array(_("Stock_id"), _("Source Code"), _("Our Code"), _("Sales Type"), _("Quantity"), _("Price ")._(" V. U"), _("Price")._(" V.U.Cant."));
//table_header($th);
//$k = 0; //row colour counter
//$calculated = false;
//$sum_prices = 0;
//$sum_prices_kit = 0;
//while ($myrow = db_fetch($res))
//{
//        $price_items = 0;
//        
//	alt_table_row_color($k);
//        
//        if (is_kit($myrow["stock_id"])>0){
//            $kit = 1;
//            $myrow_kit = description_kit($myrow["stock_id"]);
//            $descri = $myrow_kit["descript_kit"];
//        }
//        else {
//            $kit = 0;
//            $descri = $myrow["description"];
//        }
//        label_cell($myrow["stock_id"].'-'.$descri);
//        label_cell($myrow["source_code"]);
//        label_cell($myrow["our_code"]);
//        label_cell($myrow["sales_type"]);
//        label_cell($myrow["quantity"]);
//        if ($kit==1){                        
//            $price_kit_loc = price_kit_loc($myrow["stock_id"],$_POST['sales_type_id_sel']);
//            amount_cell($price_kit_loc['price']);
//            amount_cell($price_kit_loc['price']*$myrow["quantity"]);
//            $sum_prices_kit += $price_kit_loc["price"]*$myrow["quantity"];
//        }
//        else {
//            amount_cell($myrow["price"]);
//            amount_cell($myrow["price"]*$myrow["quantity"]);
//            $sum_prices_kit += $myrow["price"]*$myrow["quantity"];
//        }
//
//    end_row();
//
//}
//alt_table_row_color($k);
//        label_cell("Total");
//        label_cell("");        
// 	label_cell("");
// 	label_cell("");
//        label_cell("");
//        label_cell("");
//        amount_cell($sum_prices_kit);
//end_row();
//
//end_table();
//}
//div_end();
//
//echo "<br>";

if (!@$_GET['popup'])
{
	end_form();
	end_page(@$_GET['popup'], false, false);
}	
?>
