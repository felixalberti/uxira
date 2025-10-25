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
$page_security = 'SA_SALESKIT';
$path_to_root = "../..";
include_once($path_to_root . "/includes/session.inc");

if (!@$_GET['popup'])
{
	$js = "";
        
        if ($use_popup_windows){       
	    $js .= get_js_open_window(800, 500);        
        }       
//page(_($help_context = "Sales Kits & Alias Codes"));
page(_($help_context = "Sales Kits & Alias Codes"), false, false, "", $js);
}

include_once($path_to_root . "/includes/date_functions.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/data_checks.inc");

include_once($path_to_root . "/includes/manufacturing.inc");

check_db_has_stock_items(_("There are no items defined in the system."));

simple_page_mode(true);

/*
if (isset($_GET['item_code']))
{
	$_POST['item_code'] = $_GET['item_code'];
	$selected_kit =  $_GET['item_code'];
}
*/
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
//
//
//--------------------------------------------------------------------------------------------------
function display_kit_items($selected_kit,$sales_type_id)
{
	//$result = get_item_kit($selected_kit);
        $result = get_item_kit_and_prices($selected_kit,$sales_type_id);
	div_start('bom');
	start_table(TABLESTYLE, "width=60%");
	$th = array(_("Stock Item"), _("Description"), _("Quantity"), _("Units"), _("Price"), _("Price Items"), _("Warnings"),
		'','');
	table_header($th);

	$k = 0;
        $sum_price = 0;
        $sum_price_items = 0;
        $kit = 0;
	while ($myrow = db_fetch($result))
	{
        
        $price_items = 0;
            
		alt_table_row_color($k);

		label_cell($myrow["stock_id"]);
		label_cell($myrow["comp_name"]);
        qty_cell($myrow["quantity"], false, 
			$myrow["units"] == '' ? 0 : get_qty_dec($myrow["comp_name"]));
        label_cell($myrow["units"] == '' ? _('kit') : $myrow["units"]);
        
        //Precio Kit
        label_cells(null,price_format(round($myrow["price"]*$myrow["quantity"],2)),false,"align='right'");
       
        if (is_kit($myrow["stock_id"]) > 0){
            $result_qty = get_item_kit_qty($myrow["stock_id"],$sales_type_id);
            while ($data_kit = db_fetch($result_qty))
	    {
              $price_items += $data_kit['price'];
            }
            label_cells(null,price_format(round($price_items*$myrow["quantity"],2)),false,"align='right'");
            $sum_price_items += $price_items*$myrow["quantity"];
            $kit = 1;
            //display_notification($sum_price_items);
        }
        else {
            $price_items_ind = price_kit($myrow["stock_id"],$sales_type_id);
            label_cells(null,price_format(round($price_items_ind['price']*$myrow["quantity"],2)),false,"align='right'");
            $sum_price_items += $price_items_ind['price']*$myrow["quantity"];
            $kit = 0; 
        }
        
        
        if ($kit == 1 && (price_format(round($myrow["price"],2)*$myrow["quantity"]) != price_format($price_items*$myrow["quantity"]))){
           label_cells(null,_("Review"),false,"align='center'");
           //display_notification(price_format(round(($myrow["price"]*$myrow["quantity"]),2)).' <-> '.price_format($price_items*$myrow["quantity"]));
        }
        else {
            if ($kit == 0 && (price_format(round($myrow["price"]*$myrow["quantity"],2)) != price_format(round($price_items_ind['price']*$myrow["quantity"],2)))){
            label_cells(null,_("Review"),false,"align='center'");
            //display_notification(price_format(round($myrow["price"]*$myrow["quantity"],2)).' <-> '.price_format(round($price_items_ind['price']*$myrow["quantity"],2)));
            }
            else
            label_cells(null,"",false,"align='center'");
        }
        //label_cell($myrow["prof"]);
        //
 		edit_button_cell("Edit".$myrow['id'], _("Edit"));
 		delete_button_cell("Delete".$myrow['id'], _("Delete"));                
        $sum_price += $myrow["price"]*$myrow["quantity"];
        end_row();

	} //END WHILE LIST LOOP
        alt_table_row_color($k);
        label_cell('Total');
        label_cell("");
        label_cell("");
        label_cell("");
        label_cells(null,price_format($sum_price),false,"align='right'");
        label_cells(null,price_format($sum_price_items),false,"align='right'");                
        label_cell("");
        label_cell("");
        label_cell("");
        label_cell("");
        end_row();
        
	end_table();
div_end();
}

//--------------------------------------------------------------------------------------------------

function update_component($kit_code, $selected_item)
{
	global $Mode, $Ajax, $selected_kit;
	
	if (!check_num('quantity', 0))
	{
		display_error(_("The quantity entered must be numeric and greater than zero."));
		set_focus('quantity');
		return;
	}
   	elseif ($_POST['description'] == '')
   	{
      	display_error( _("Item code description cannot be empty."));
		set_focus('description');
		return;
   	}
	elseif ($selected_item == -1)	// adding new item or new alias/kit
	{
		if (get_post('item_code') == '') { // New kit/alias definition
			$kit = get_item_kit($_POST['kit_code']);
    		if (db_num_rows($kit)) {
			  	$input_error = 1;
    	  		display_error( _("This item code is already assigned to stock item or sale kit."));
				set_focus('kit_code');
				return;
			}
			if (get_post('kit_code') == '') {
	    	  	display_error( _("Kit/alias code cannot be empty."));
				set_focus('kit_code');
				return;
			}
		}
   	}

/*Begin Felix Alberti 15/09/2016*/
        $only_add_item_price = 0;
	if (check_item_in_kit($selected_item, $kit_code, $_POST['component'], true)) {                
                //Begin Felix Alberti 15/09/2016
                if (get_exist_price_type_currency($_POST['component'], $_POST['sales_type_id'])==0){
                    $only_add_item_price = 1;
                    $msg =_("The price component has been added.");
                }
                else {
                //End Felix Alberti 15/09/2016
                    //display_error(_("The selected component contains directly or on any lower level the kit under edition. Recursive kits are not allowed."));
                    //set_focus('component');
                    //return;
                //Begin Felix Alberti 15/09/2016
                }
                //End Felix Alberti 15/09/2016
	}
//
//		/*Now check to see that the component is not already in the kit */
	if (check_item_in_kit($selected_item, $kit_code, $_POST['component'])) {
		display_error(_("The selected component is already in this kit. You can modify it's quantity but it cannot appear more than once in the same kit."));
		set_focus('component');
		return;
	}
/*End Felix Alberti 15/09/2016*/
	if ($selected_item == -1) { // new item alias/kit
                if ($only_add_item_price == 0) {
                    if ($_POST['item_code']=='') {
                            $kit_code = $_POST['kit_code'];
                            $selected_kit = $_POST['item_code'] = $kit_code;
                            $msg = _("New alias code has been created.");
                    } 
                     else
                            $msg =_("New component has been added to selected kit.");
                    if (isset($_POST['component']) && $_POST['component']!='' && $_POST['component']!=NULL){
			add_item_code( $kit_code, get_post('component'), get_post('description'),
			get_post('category'), input_num('quantity'), 0);
		    }
		    else
		    $msg =_("New component has not been added to selected kit (Blank Code).");
                }
                //Begin Felix Alberti 15/09/2016
                if ($only_add_item_price == 0 && isset($_POST['component']) && $_POST['component']!='' && $_POST['component']!=NULL) {
                   //add_item_price(get_post('component'), $_POST['sales_type_id'], $_POST['curr_abrev'], input_num('price'));
                }
				
                //End Felix Alberti 15/09/2016
		display_notification($msg);

	} else {
                //Begin Felix Alberti 01/11/2016
                $myrow = get_item_code_stock($_POST['item_code'], $_POST['component']);
                $last_qty = isset($myrow['quantity']) ? $myrow['quantity'] : 0;
                $user = $_SESSION["wa_current_user"]->username;
                /*add_price_history($_POST['item_code'], $_POST['sales_type_id'], get_post('component'),
                        $last_qty, input_num('quantity'), 'U', $user);*/
                //End Felix Alberti 01/11/2016
            
		$props = get_kit_props($_POST['item_code']);
		update_item_code($selected_item, $kit_code, get_post('component'),
			$props['description'], $props['category_id'], input_num('quantity'), 0);
                //Begin Felix Alberti 15/09/2016
//                update_item_only_price(get_post('component'), $_POST['sales_type_id'],
//			$_POST['curr_abrev'], input_num('price'));               
                
                //End Felix Alberti 15/09/2016
		display_notification(_("Component of selected kit has been updated."));
	}
	$Mode = 'RESET';
	$Ajax->activate('_page_body');
}

//--------------------------------------------------------------------------------------------------

if (get_post('update_name')) {
	update_kit_props(get_post('item_code'), get_post('description'), get_post('category'),get_post('recalculate'),get_post('onlyquotation'),get_post('specialties_id'));
	display_notification(_('Kit common properties has been updated'));

	$Ajax->activate('_page_body');
}

if (get_post('update_price')) {
        update_item_price_by_stock_id(get_post('item_code'), get_post('sales_type_id'), $_POST['curr_abrev_kit'], input_num('price_kit'));
	display_notification(_('Kit common price has been updated'));
	$Ajax->activate('_page_body');
}

if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM'){
	update_component($_POST['item_code'], $selected_id);        
}

if ($Mode == 'Delete')
{
	// Before removing last component from selected kit check 
	// if selected kit is not included in any other kit. 
	// 
	$other_kits = get_where_used($_POST['item_code']);
	$num_kits = db_num_rows($other_kits);

	$kit = get_item_kit($_POST['item_code']);
	if ((db_num_rows($kit) == 1) && $num_kits) {

		$msg = _("This item cannot be deleted because it is the last item in the kit used by following kits")
			.':<br>';

		while($num_kits--) {
			$kit = db_fetch($other_kits);
			$msg .= "'".$kit[0]."'";
			if ($num_kits) $msg .= ',';
		}
		display_error($msg);
	} else {
		delete_item_code($selected_id);
		display_notification(_("The component item has been deleted from this bom"));
		$Mode = 'RESET';
	}
}

if ($Mode == 'RESET')
{
	$selected_id = -1;
	unset($_POST['quantity']);
	unset($_POST['component']);
        unset($_POST['price']);
}
//--------------------------------------------------------------------------------------------------
/*Begin Felix Alberti 26/09/2016*/
function prices_inventory_link($label,$stock_id)
{
        $without_menu = 1;
	return get_prices_inventory_view_str($label,$without_menu,$stock_id);
}
/*End Felix Alberti 26/09/2016*/

start_form();

echo "<center>" . _("Select a sale kit:") . "&nbsp;";
echo sales_kits_list('item_code', null, _('New kit'), true);
echo "</center><br>";
$props = get_kit_props($_POST['item_code']);

if (list_updated('item_code')) {
	if (get_post('item_code') == '')
		$_POST['description'] = '';
	$Ajax->activate('_page_body');
}

//Begin Felix Alberti 23/08/2016
if (list_updated('sales_type_id')) {
	$Ajax->activate('_page_body');
}
//End Felix Alberti 23/08/2016

$selected_kit = $_POST['item_code'];
//----------------------------------------------------------------------------------
if (get_post('item_code') == '') {
// New sales kit entry
	start_table(TABLESTYLE2);
	text_row(_("Alias/kit code:"), 'kit_code', null, 20, 21);
} else
{
	 // Kit selected so display bom or edit component
	$_POST['description'] = $props['description'];
	$_POST['category'] = $props['category_id'];
        $_POST['recalculate'] = $props['recalculate'];
        $_POST['onlyquotation'] = $props['only_quotation'];
        $_POST['specialties_id'] = $props['specialties_id'];
	start_table(TABLESTYLE2);
	text_row(_("Description:"), 'description', null, 50, 200);
	stock_categories_list_row(_("Category:"), 'category', null, false, false, 'description');
        start_row();
        echo '<td>'._("Price").":";
        echo '</td>';
        echo '<td>'.prices_inventory_link(_("Sales &Pricing").' '.$selected_kit,$selected_kit).'</td>';
        end_row();
        check_row(_("Recalculate"), 'recalculate');
        check_row(_("Only Quotation"), 'onlyquotation');
        specialties_list_row(_("Specialties:"), 'specialties_id', null, _("Select one"), false, 'description');
        //echo '<tr>';
        //profesional_items_services_list_cells('profesional', null, true, false, false, '', '',_("Professional"));
        //echo "</tr>\n";
	submit_row('update_name', _("Update"), false, 'align=center colspan=2', _('Update kit/alias name'), true);
	end_row();
	end_table(1);
        
        //Begin Felix Alberti 23/08/2016
        echo '<br>';
        start_table(TABLESTYLE2);
        sales_types_list_row(_("Sales Type:"), 'sales_type_id', null, true);
        
        
//        $price_kit = get_stock_price_type_currency($_POST['item_code'], $_POST['sales_type_id'], 'BS');
//        $_POST['price_kit'] = $price_kit['price'];
//        $Ajax->activate('price_kit');
//        if (isset($_POST['price_kit']))
//        $_POST['price_kit'] = price_format($_POST['price_kit']);
//        $res = get_item_edit_info(get_post('item_code'));
//	$dec =  $res["decimals"] == '' ? 0 : $res["decimals"];
//	$units = $res["units"] == '' ? _('kits') : $res["units"];
//        hidden ('curr_abrev_kit', $price_kit['curr_abrev']);
//        start_row();
//        $Ajax->activate('curr_abrev_kit');
//        label_cells(_("Currency:"),$price_kit['curr_abrev'], null, null, 'curr_abrev_kit');
//        end_row();
//        amount_row(_("Price:"), 'price_kit', null, '', _('per') .' '.$units);
//        submit_row('update_price', _("Update"), false, 'align=center colspan=2', _('Update price kit'), true);
        end_table(1);
        //End Felix Alberti 23/08/2016
                
	display_kit_items($selected_kit,$_POST['sales_type_id']);
	echo '<br>';
	start_table(TABLESTYLE2);
}

	if ($Mode == 'Edit') {
                
		$myrow = get_item_code($selected_id);
                
                //display_notification($selected_id.'-'.$myrow["stock_id"]);
		$_POST['component'] = $myrow["stock_id"];
		$_POST['quantity'] = number_format2($myrow["quantity"], get_qty_dec($myrow["stock_id"]));
                $_POST['profesional'] = $myrow["professional_id"];
                //$Ajax->activate('edit_component');
                $data_price = get_data_item_price($_POST['component'],$_POST['sales_type_id']);
                $_POST['price'] = $data_price['price'];
                //display_notification('curr_abrev: -'.$data_price['curr_abrev']);
                $Ajax->activate('component');
                $Ajax->activate('price');
                $Ajax->activate('curr_abrev');
                
                
	sales_local_items_list_row(_("Component:"),'component', $_POST['component'], false, true);
	}
        else
	sales_local_items_list_row(_("Component:"),'component', null, false, true);
        
	hidden("selected_id", $selected_id);
        
//        if ($Mode == 'Edit') {
//            sales_local_items_list_row(_("Component:"),'component', $myrow["stock_id"], false, true);
//	}
//	else
        //div_start('edit_component');
	//sales_local_items_list_row(_("Component:"),'component', null, false, true);
        //display_notification($_POST['component']);

//	if (get_post('description') == '')
//		$_POST['description'] = get_kit_name($_POST['component']);
	if (get_post('item_code') == '') { // new kit/alias
		if ($Mode!='ADD_ITEM' && $Mode!='UPDATE_ITEM') {
			$_POST['description'] = $props['description'];
			$_POST['category'] = $props['category_id'];
		}
		text_row(_("Description:"), 'description', null, 50, 200);
		stock_categories_list_row(_("Category:"), 'category', null);
	}
	$res = get_item_edit_info(get_post('component'));
	$dec =  $res["decimals"] == '' ? 0 : $res["decimals"];
	$units = $res["units"] == '' ? _('kits') : $res["units"];
	if (list_updated('component')) 
	{       
		$_POST['quantity'] = number_format2(1, $dec);
		$Ajax->activate('quantity');
		$Ajax->activate('category');                
	}
	
	qty_row(_("Quantity:"), 'quantity', number_format2(1, $dec), '', $units, $dec);
        
        
//        if (isset($_POST['price']))
//        $_POST['price'] = price_format($_POST['price']);
//        amount_row(_("Price:"), 'price', null, '', _('per') .' '.$units);
//        if ($Mode == 'Edit'){
//            if (!$data_price['curr_abrev'])
//            currencies_list_row(_("Currency:"), 'curr_abrev', null, true);
//            else
            if (isset($data_price['curr_abrev']))
            hidden ('curr_abrev', $data_price['curr_abrev']);
//            start_row();
//            label_cells(_("Currency:"),$data_price['curr_abrev'], null, null, 'curr_abrev');
//            end_row();
//        }
//        else
//        currencies_list_row(_("Currency:"), 'curr_abrev', null, true);
        
        //div_end();
        
       
	end_table(1);
	submit_add_or_update_center($selected_id == -1, '', 'both');
	end_form();
//----------------------------------------------------------------------------------

end_page();

?>
