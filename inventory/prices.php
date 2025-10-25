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
$page_security = 'SA_SALESPRICE';
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

if (!@$_GET['popup']) {
        if (isset($_GET['without_menu']))
        page(_($help_context = "Inventory Item Sales prices"),true);
        else
	page(_($help_context = "Inventory Item Sales prices"));
}

//---------------------------------------------------------------------------------------------------

check_db_has_stock_items(_("There are no items defined in the system."));

check_db_has_sales_types(_("There are no sales types in the system. Please set up sales types befor entering pricing."));

simple_page_mode(true);
//---------------------------------------------------------------------------------------------------
$input_error = 0;

if (isset($_GET['stock_id']))
{
	$_POST['stock_id'] = $_GET['stock_id'];
}
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

if (!@$_GET['popup'])
{
	echo "<center>" . _("Item:"). "&nbsp;";
	echo sales_items_list('stock_id', $_POST['stock_id'], false, true, '', array('editable' => false));
	echo "<hr></center>";
}
else
	br(2);
set_global_stock_item($_POST['stock_id']);

function process_update_component_kit($selected,$sales_type,$currency,$byselect=1){
    
    if ($byselect == 1){
       $myrow = get_stock_price($selected);
       $stock_id = $myrow['stock_id'];
    }
    else {
        $stock_id = $selected;
    }
    $res_affected_kits = get_component_kit($stock_id,$sales_type,$currency);
   
    $exec = 0;
    
    while ($row = db_fetch($res_affected_kits)) {                            
               
               $result_subitem = get_item_kit_and_prices($row['item_code'],$sales_type);
              
               $line1 = 0;
               
               $sum_price = 0;
               $sum_price_items = 0; 
               
               while ($mysubitem = db_fetch($result_subitem))
               {
                    $line1++;
                    
                    //display_notification('subitem '.$row['item_code'].' - '.$line1.' - '.$mysubitem["stock_id"]);
                    if (is_kit($mysubitem["stock_id"]) > 0){
                        $result_qty = get_item_kit_qty($mysubitem["stock_id"],$sales_type);
                        while ($data_kit = db_fetch($result_qty))
                         {
                           $sum_price += $data_kit['price']*$mysubitem["quantity"];
                           //$sum_price += $data_kit['price'];
                           //display_notification('subitem '.$data_kit["stock_id"].' - '.$data_kit["price"]);
                           //display_notification('sub subitem '.$data_kit["stock_id"].' - '.' - '.$data_kit["stock_id"].' - '.$sum_price);
                         }
                         //$sum_price_items += $sum_price*$mysubitem["quantity"];
                    }
                    else {
                        //display_notification($row['item_code'].' -$ '.'1****************');
                        $price_items_ind = price_kit($mysubitem["stock_id"],$sales_type); 
                        //$sum_price_items += $price_items_ind['price']*$mysubitem["quantity"];
                        $sum_price += $price_items_ind['price']*$mysubitem["quantity"];
                        //display_notification($row['item_code'].' -$ '.'1****************');
                    }
               
               }
               db_free_result($result_subitem);
               /*$myprices = db_fetch($result);
               $sum_price += $myprices['price']*$myprices["quantity"];*/
               
                //update_item_price_by_id($row['id'], ($price_upt*$factor)+$row['price']);
                update_item_price_by_id($row['id'], $sum_price);
                //display_notification('*** - '.$row['item_code'].' - '.$sum_price);
                
                if (is_kit($stock_id)>0){
                   process_update_component_kit($row['item_code'],$sales_type,$currency,0);
                }
                
                //
                
                $exec++;
    }
    db_free_result($res_affected_kits);    
    if ($exec > 0)
    display_notification(_("This price has been recalculated in each kit."));
    
}

//----------------------------------------------------------------------------------------------------
if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	if (!check_num('price', 0))
	{
		$input_error = 1;
		display_error( _("The price entered must be numeric."));
		set_focus('price');
	}
   	elseif ($Mode == 'ADD_ITEM' && get_stock_price_type_currency($_POST['stock_id'], $_POST['sales_type_id'], $_POST['curr_abrev']))
   	{
      	$input_error = 1;
      	display_error( _("The sales pricing for this item, sales type and currency has already been added."));
		set_focus('supplier_id');
	}

	if ($input_error != 1)
	{

    	if ($selected_id != -1) 
		{      
			//editing an existing price
                        begin_transaction();
                        $changed = 0;
                        $user = $_SESSION["wa_current_user"]->username;
                        $myrow = get_stock_price($selected_id);
                        $stock_id = $myrow['stock_id'];
                        if (!is_kit($stock_id) > 0) $changed = 1;
			update_item_price($selected_id, $_POST['sales_type_id'],
			$_POST['curr_abrev'], input_num('price'), $changed, $user);
                        
                        $myrow = get_stock_price($selected_id);                                               
                        $item_row = get_item($myrow['stock_id']);
                        //display_notification('Seleccion '.$myrow['stock_id']); 
                        
                        $price_item = input_num('price');//Precio dado por pantalla
                        //display_notification('price upt '.$price_upt);
                        //if ($item_row['recalculate_price']==1){
                            $myrow_company = get_company_prefs();
                            $base_sales = $myrow_company["base_sales"];
                            $curr_default = $myrow_company["curr_default"];
                            //$item_autom_into_kit = $myrow_company["item_autom_into_kit"];
                            
                            $res_sales_type = get_sales_type_by_id($_POST['sales_type_id']);
                            $row_sales_type = db_fetch($res_sales_type);
                            $sales_type = $row_sales_type['id'];
                            
                            $factor = $row_sales_type['factor'];
                            $curr_abrev = $myrow['curr_abrev'];    
                            if ($base_sales==$_POST['sales_type_id']){
                                process_update_component_kit($selected_id,$sales_type,$_POST['curr_abrev'],1);
                                //}
                            }
                            else {
                                //$res_affected_kits = get_sum_component_kit($myrow['stock_id'],$myrow['sales_type_id'],$myrow['curr_abrev']);                                                                    
                               process_update_component_kit($selected_id,$sales_type,$_POST['curr_abrev'],1);
                            }
                            
                        commit_transaction();
			$msg = _("This price has been updated.");
		}
		else
		{
                       
                        begin_transaction();

			add_item_price($_POST['stock_id'], $_POST['sales_type_id'],
			    $_POST['curr_abrev'], input_num('price')); 
                        
                        $myrow = get_stock_price($_POST['stock_id']);                                               
                        $item_row = get_item($myrow['stock_id']);
                        
                        $price_item = input_num('price');//Precio dado por pantalla

                            $myrow_company = get_company_prefs();
                            $base_sales = $myrow_company["base_sales"];
                            $curr_default = $myrow_company["curr_default"];
                            //$item_autom_into_kit = $myrow_company["item_autom_into_kit"];
                            
                            $res_sales_type = get_sales_type_by_id($_POST['sales_type_id']);
                            $row_sales_type = db_fetch($res_sales_type);
                            $sales_type = $row_sales_type['id'];
                            
                            $factor = $row_sales_type['factor'];
                            //display_notification('Factor: '.$factor);
                            $curr_abrev = $myrow['curr_abrev'];    
                            if ($base_sales==$_POST['sales_type_id']){
                                process_update_component_kit($selected_id,$sales_type,$_POST['curr_abrev'],1);
                            }
                            else {
                                //$res_affected_kits = get_sum_component_kit($myrow['stock_id'],$myrow['sales_type_id'],$myrow['curr_abrev']);                                                                    
                               process_update_component_kit($selected_id,$sales_type,$_POST['curr_abrev'],1);
                            }
                            
                        commit_transaction();                        
			$msg = _("The new price has been added.");
		}
		display_notification($msg);
		$Mode = 'RESET';
	}

}

//------------------------------------------------------------------------------------------------------

if ($Mode == 'Delete')
{
	
        //Begin Felix Alberti 10/11/2016
        $myrow = get_stock_price($selected_id);                        
        $item_row = get_item($myrow['stock_id']);
        if ($item_row['recalculate_price']==1){

                $sales_type = $myrow['sales_type_id'];
                $res_affected_kits = get_sum_component_kit($myrow['stock_id'],$sales_type,$_POST['curr_abrev']);
                while ($row = db_fetch($res_affected_kits)) {
                    //Actualiza el precio en cada kit que este presente el item
                    $price_item = get_stock_price_type_currency($myrow['stock_id'],$sales_type,$_POST['curr_abrev']);
                    $suma =  $price_item['price'] + $row['price'];                          
                    update_item_price_by_stock_id($row['item_code'], $sales_type,$_POST['curr_abrev'],
                            ($row['price']));
                }

            display_notification(_("This price has been recalculated in each kit."));
        }
        //End Felix Alberti 10/11/2016
        
        //the link to delete a selected record was clicked
	delete_item_price($selected_id);
	display_notification(_("The selected price has been deleted."));                
        
	$Mode = 'RESET';
}

if ($Mode == 'RESET')
{
	$selected_id = -1;
}

function update_items_automatic($selected_id,$prices_items,$sales_type,$curr_abrev){
    begin_transaction();
                        $changed = 0;
                        $user = $_SESSION["wa_current_user"]->username;
                        $myrow = get_stock_price($selected_id);
                        $stock_id = $myrow['stock_id'];
                        $changed = 1;
			update_item_price_by_stock_id($selected_id, $sales_type, $curr_abrev, $prices_items, $changed, $user);                      
                        
                        process_update_component_kit($selected_id,$sales_type,$curr_abrev,1);
                                                    
    commit_transaction();
}


/*Begin Felix Alberti 02/11/2018*/
if (isset($_POST['ProcessUpdatePrice'])){
    $prices_list = get_prices($_POST['stock_id']);
    while ($sales_type = db_fetch($prices_list))
    {
        $prices_items = prices_x_items($_POST['stock_id'],$sales_type["sales_type_id"]);
        update_items_automatic($_POST['stock_id'],$prices_items,$sales_type['sales_type_id'],$sales_type['curr_abrev']);
        
        if (is_kit($_POST['stock_id']) > 0)
        upd_child($_POST['stock_id'],$sales_type["sales_type_id"],$sales_type['curr_abrev']);
    }
    
    $msg = _("This price has been updated automatic.");
    display_notification($msg);
}
/*End Felix Alberti 02/11/2018*/

function is_kit($item){
   $sql = "SELECT count(*) as cant  
		FROM ".TB_PREF."item_codes 
		WHERE ".
                TB_PREF."item_codes.item_code = ".db_escape($item).
                " and ".TB_PREF."item_codes.stock_id != ".db_escape($item);
   $res = db_query($sql,"the count could not be retreived");
   $myrow = db_fetch($res);
   return $myrow['cant'];
}

function price_kit_loc($kit,$sales_type){
    $sql = "SELECT sum(price) as price  
		FROM ".TB_PREF."item_codes i LEFT JOIN ".TB_PREF."prices p ON
                    ( i.stock_id = p.stock_id and p.sales_type_id = $sales_type)
		WHERE i.item_code = ".db_escape($kit);
   $res = db_query($sql,"item kit prices could not be retreived");   
   $myrow = db_fetch($res);
   return $myrow;
    
}

function description_kit($kit){
    $sql = "SELECT i.description as descript_kit  
		FROM ".TB_PREF."item_codes i
		WHERE i.item_code = ".db_escape($kit);
   $res = db_query($sql,"item kit description could not be retreived");
   $myrow = db_fetch($res);
   return $myrow;
    
}

function prices_x_items($tock_id,$sales_type_id){
        $sum_price_items = 0;
        
        $result = get_item_kit_and_prices($tock_id,$sales_type_id);
        
        while ($myrow = db_fetch($result))
	{
        $price_items = 0;
        if (is_kit($tock_id) > 0){
            $result_qty = get_item_kit_qty($myrow["stock_id"],$sales_type_id);
            while ($data_kit = db_fetch($result_qty))
	    {
              $price_items += $data_kit['price'];
            }
            $sum_price_items += $price_items*$myrow["quantity"];
        }
        else {
            $price_items_ind = price_kit($myrow["stock_id"],$sales_type_id);
            $sum_price_items += $price_items_ind['price']*$myrow["quantity"];
        }
        }
        return $sum_price_items;
}

$show_price_kits = 0;
if (list_updated('stock_id')) {
	$Ajax->activate('price_table');
	$Ajax->activate('price_details');
}
if (list_updated('stock_id') || isset($_POST['stock_id'])) {
	$Ajax->activate('price_table');
	$Ajax->activate('price_details');
        
        $sql = "SELECT count(*) as cant  
		FROM ".TB_PREF."item_codes 
		WHERE ".
                TB_PREF."item_codes.item_code = ".db_escape($_POST['stock_id']).
                " and ".TB_PREF."item_codes.stock_id != ".db_escape($_POST['stock_id']);	
	
        $res = db_query($sql,"item prices could not be retreived");
        $myrow = db_fetch($res);
        if (is_kit($_POST['stock_id']) > 0) $show_price_kits = 1;                
        
        $Ajax->activate('items_kits');
}
if (list_updated('sales_type_id_sel')) { 
        $show_price_kits = 1;
        $Ajax->activate('items_kits');
}
if (list_updated('stock_id') || isset($_POST['_curr_abrev_update']) || isset($_POST['_sales_type_id_update'])) {
	// after change of stock, currency or salestype selector
	// display default calculated price for new settings. 
	// If we have this price already in db it is overwritten later.
	unset($_POST['price']);
	$Ajax->activate('price_details');
}

//---------------------------------------------------------------------------------------------------

$prices_list = get_prices($_POST['stock_id']);

div_start('price_table');
start_table(TABLESTYLE, "width=50%");

$th = array(_("Currency"), _("Sales Type"), _("Price"), _("Price Items") , "", "");
table_header($th);
$k = 0; //row colour counter
$calculated = false;
while ($myrow = db_fetch($prices_list))
{

	alt_table_row_color($k);

	label_cell($myrow["curr_abrev"]);
    label_cell($myrow["sales_type"]);
    amount_cell($myrow["price"]);
    label_cell(price_format(prices_x_items($_POST['stock_id'],$myrow["sales_type_id"])),"align='right'");
 	edit_button_cell("Edit".$myrow['id'], _("Edit"));
 	delete_button_cell("Delete".$myrow['id'], _("Delete"));
    end_row();

}
end_table();
if (db_num_rows($prices_list) == 0)
{
	if (get_company_pref('add_pct') != -1)
		$calculated = true;
	display_note(_("There are no prices set up for this part."), 1);
}

echo "<br/>";

$updateitems = _("Update Prices Items automatic");
submit_center_first('ProcessUpdatePrice', $updateitems, _('Update Prices items automatic'), 'default');

div_end();
//------------------------------------------------------------------------------------------------

echo "<br>";

if ($Mode == 'Edit')
{
	$myrow = get_stock_price($selected_id);
	$_POST['curr_abrev'] = $myrow["curr_abrev"];
	$_POST['sales_type_id'] = $myrow["sales_type_id"];
	$_POST['price'] = price_format($myrow["price"]);
}

hidden('selected_id', $selected_id);
if (@$_GET['popup'])
{
	hidden('_tabs_sel', get_post('_tabs_sel'));
	hidden('popup', @$_GET['popup']);
}
div_start('price_details');
start_table(TABLESTYLE2);

currencies_list_row(_("Currency:"), 'curr_abrev', null, true);

sales_types_list_row(_("Sales Type:"), 'sales_type_id', null, true);

if (!isset($_POST['price'])) {
	$_POST['price'] = price_format(get_kit_price(get_post('stock_id'), 
		get_post('curr_abrev'),	get_post('sales_type_id')));
}

$kit = get_item_code_dflts($_POST['stock_id']);
small_amount_row(_("Price:"), 'price', null, '', _('per') .' '.$kit["units"]);

end_table(1);
if ($calculated)
	display_note(_("The price is calculated."), 0, 1);

submit_add_or_update_center($selected_id == -1, '', 'both');
div_end();

/*Only for view table child item*/
br();
br();
$sales_type_id = $_POST['sales_type_id'];
$result = get_item_kit_and_prices($_POST['stock_id'],$sales_type_id);

start_outer_table(TABLESTYLE2);
	table_section(1);
	table_section_title(_("Items child view for update"));
end_outer_table(1);


start_table(TABLESTYLE, "width=50%");

$th = array(_("Stock Item"), _("Description"), _("Qty"), _("Units") , _("Price"), _("Price Items"));
table_header($th);

while ($myrow = db_fetch($result))
{
        
        if (is_kit($myrow["stock_id"]) > 0){
            
            $price_items = 0;

            alt_table_row_color($k);

            label_cell($myrow["stock_id"]);
            label_cell($myrow["comp_name"]);

            qty_cell($myrow["quantity"], false, 
                            $myrow["units"] == '' ? 0 : get_qty_dec($myrow["comp_name"]));
            label_cell($myrow["units"] == '' ? _('kit') : $myrow["units"]);

            //Precio Kit
            label_cells(null,price_format(round($myrow["price"]*$myrow["quantity"],2)),false,"align='right'");

                $result_qty = get_item_kit_qty($myrow["stock_id"],$sales_type_id);
                while ($data_kit = db_fetch($result_qty))
                {
                  $price_items += $data_kit['price'];
                }
                label_cells(null,price_format(round($price_items*$myrow["quantity"],2)),false,"align='right'");
        }
}

end_table(1);

function upd_child($stock_id,$sales_type_id,$curr_abrev){
    $result = get_item_kit_and_prices($stock_id,$sales_type_id);
    while ($myrow = db_fetch($result))
    {        
        if (is_kit($myrow["stock_id"]) > 0){
            //display_notification('child stock_id = '.$myrow["stock_id"].'-'.$sales_type_id);
            $prices_items = prices_x_items($myrow["stock_id"],$sales_type_id);
            update_items_automatic($myrow["stock_id"],$prices_items,$sales_type_id,$curr_abrev);
        }
    }
}


if (!@$_GET['popup'])
{
	end_form();
	end_page(@$_GET['popup'], false, false);
}	
?>
