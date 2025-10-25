<?php
/**********************************************************************
    Copyright April 2016 (C) Felix Alberti.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
$path_to_root = "../..";
include($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/includes/types.inc"); // For tag constants
include_once($path_to_root . "/admin/db/set_group_db.inc");
include_once($path_to_root . "/admin/db/set_db.inc");
include($path_to_root . "/includes/ui.inc");

// Set up page security based on what type of tags we're working with
//if (@$_GET['type'] == "account" || get_post('type') == TAG_ACCOUNT) {
	$page_security = 'SA_GLACCOUNTTAGS';
//} else if(@$_GET['type'] == "dimension" || get_post('type') == TAG_DIMENSION) {
//	$page_security = 'SA_DIMTAGS';
//}

// We use $_POST['type'] throughout this script, so convert $_GET vars
// if $_POST['type'] is not set.
//if (!isset($_POST['type'])) {
//	if ($_GET['type'] == "account")
//		$_POST['type'] = TAG_ACCOUNT;
//	elseif ($_GET['type'] == "dimension")
		$_POST['type'] = TAG_DIMENSION;
//	else
//		die(_("Unspecified tag type"));
//}

// Set up page based on what type of tags we're working with
//switch ($_POST['type']) {
//	case TAG_ACCOUNT:
//		// Account tags
//		$_SESSION['page_title'] = _($help_context = "Account Tags");
//		break;
//	case TAG_DIMENSION:
//		// Dimension tags
//		$_SESSION['page_title'] = _($help_context = "Dimension Tags");
//}

$_SESSION['page_title'] = _($help_context = "Settings Dimension");

page($_SESSION['page_title']);

simple_page_mode(true);
simple_page_mode2(true);

function simple_page_mode2($numeric_id = true)
{
	global $Ajax, $Mode2, $selected_id2;

	$default = $numeric_id ? -1 : '';
	$selected_id2 = get_post('selected_id2', $default);
	foreach (array('ADD_ITEM2', 'UPDATE_ITEM2', 'RESET2') as $m) {
		if (isset($_POST[$m])) {
			$Ajax->activate('_page_body');
			if ($m == 'RESET2') 
				$selected_id2 = $default;
			$Mode2 = $m; return;
		}
	}
	foreach (array('BEd', 'BDel') as $m) {                
		foreach ($_POST as $p => $pvar) {
			if (strpos($p, $m) === 0) {
//				$selected_id2 = strtr(substr($p, strlen($m)), array('%2E'=>'.'));
				unset($_POST['_focus']); // focus on first form entry
				$selected_id2 = quoted_printable_decode(substr($p, strlen($m)));
                                //display_notification($selected_id2);
				$Ajax->activate('_page_body');
				$Mode2 = $m;
				return;
			}
		}
	}
	$Mode2 = '';
}

function submit_add_or_update_center2($add=true, $title=false, $async=false)
{
	echo "<center>";
	if ($add)
		submit('ADD_ITEM2', _("Add new"), true, $title, $async);
	else {
		submit('UPDATE_ITEM2', _("Update"), true, $title, $async);
		submit('RESET2', _("Cancel"), true, $title, $async);
	}
	echo "</center>";
}

//-----------------------------------------------------------------------------------

function can_process() 
{
	if (strlen($_POST['description']) == 0) 
	{
		display_error( _("The description cannot be empty."));
		set_focus('description');
		return false;
	}
        if (strlen($_POST['set_id']) == 0) 
	{
		display_error( _("The set id cannot be empty."));
		set_focus('description');
		return false;
	}
        if (strlen($_POST['order_']) == 0) 
	{
		display_error( _("The order cannot be empty."));
		set_focus('description');
		return false;
	}
	return true;
}

function can_process2() 
{
	
        if ($_POST['cod_dim'] == 0) 
	{
		display_error( _("The dimension code cannot be empty."));
		set_focus('cod_dim');
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------------

if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{
	if (can_process()) 
	{
    	if ($selected_id != -1) 
    	{
    		if( $ret = update_set_group($selected_id, $_POST['description'], $_POST['order_'], input_num('porcent_serv') / 100,
                        $_POST['mb_flag'],@$_POST['valid_on_account'],@$_POST['level_break_on_invoice_printing'],@$_POST['level_break_on_quotation_printing']))
				display_notification(_('Selected tag settings have been updated'));
    	} 
    	else 
    	{
    		if( $ret = add_set_group($_POST['set_id'], $_POST['description'], $_POST['order_'], input_num('porcent_serv') / 100, $_POST['mb_flag'],
                        @$_POST['valid_on_account'],@$_POST['level_break_on_invoice_printing'],@$_POST['level_break_on_quotation_printing']))
				display_notification(_('New tag has been added'));
    	}
		if ($ret) $Mode = 'RESET';
	}
}

if ($Mode2=='ADD_ITEM2' || $Mode2=='UPDATE_ITEM2') 
{       //display_notification(_('programar boton2'));
	if (can_process2()) 
	{
    	if ($selected_id2 != -1) 
    	{
    		if( $ret = update_set($selected_id2, $_POST['set_id'], $_POST['cod_dim']))
				display_notification(_('Selected set have been updated'));
    	} 
    	else 
    	{
    		if( $ret = add_set($_POST['set_id'], $_POST['cod_dim']))
				display_notification(_('New set has been added'));
    	}
		if ($ret) $Mode2 = 'RESET2';
	}
}

//-----------------------------------------------------------------------------------
function get_records_associated_with_setting($id)
{
        $row = get_data_set($id);
        $dim = $row['cod_dim'];
		
	$sql = "SELECT count(*) as cant FROM ".TB_PREF."professional p,"
               .TB_PREF."professional_items pi,"
               .TB_PREF."stock_master sm "
               . "WHERE pi.id_professional = p.professional_id and"
                . " sm.stock_id = pi.stock_id and"
                . " (sm.dimension_id = ".db_escape($dim)." or sm.dimension2_id = ".db_escape($dim).")";
        //display_notification($sql);
	$result = db_query($sql, "could not delete config_x_dimension");
        return db_fetch($result);
}

function get_childrens($selected_id)
{
        $row = get_data_set_group($selected_id);
        $set_id = $row['set_id'];
	$sql = "SELECT count(*) as cant FROM ".TB_PREF."set "
               . "WHERE set_id = ".db_escape($set_id);
        
	$result = db_query($sql, "could not delete config_x_dimension because have childrens records");
        return db_fetch($result);
}

function can_delete($selected_id)
{
	if ($selected_id == -1)
		return false;
        
        $row = get_childrens($selected_id);
        if ($row['cant'] > 0)	
	{
		display_error(_("Cannot delete this records group because have childrens records."));
		return false;
	}        
        
	$row = get_records_associated_with_setting($selected_id);
	
	if ($row['cant'] > 0)	
	{
		display_error(_("Cannot delete this tag because records have been created referring to it."));
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------

if ($Mode == 'Delete')
{
	if (can_delete($selected_id))
	{
		//delete_set_group($selected_id);
		display_notification(_('Selected item has been deleted'));
	}
	$Mode = 'RESET';
}

/**************************************************************************/

function can_delete2($selected_id2)
{
	if ($selected_id2 == -1)
		return false;
	//$row = get_records_associated_with_setting($selected_id2);
	
	if ($row['cant'] > 0)	
	{
		display_error(_("Cannot delete this tag because records have been created referring to it."));
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------

if ($Mode2 == 'BDel')
{
	/*if (can_delete2($selected_id2))
	{
        */
		delete_set($selected_id2);
		display_notification(_('Selected item has been deleted'));
	/*}*/
	$Mode = 'RESET2';
}

//-----------------------------------------------------------------------------------

if ($Mode == 'RESET')
{
	$selected_id = -1;
	$_POST['set_id'] = $_POST['description'] = $_POST['order_'] = '';
}

if ($Mode2 == 'RESET2')
{
	$selected_id2 = -1;
        $selected_id = -1;
	$_POST['set_id'] = $_POST['cod_dim'] = '';
}

//-----------------------------------------------------------------------------------

function get_set_group()
{
	$sql = "SELECT id, set_id, description, order_ FROM ".TB_PREF."set_group";

	return db_query($sql, "could not get set_group");
}

function get_set($id)
{
	$sql = "SELECT s.*, d.name as dimension FROM ".TB_PREF."set s,  ".TB_PREF."dimensions d WHERE s.cod_dim = d.id and s.set_id = ".$id;

	return db_query($sql, "could not get set_group");
}

function get_data_set_group($id='')
{
	$sql = "SELECT id, set_id, description, order_, porcent_serv, mb_flag, valid_on_account, level_break_on_invoice_printing,"
                . "level_break_on_quotation_printing FROM ".TB_PREF."set_group";
	
	if ($id!='') $sql .= " WHERE id = ".db_escape ($id);

	$result = db_query($sql, "could not get set_group");
        return db_fetch($result);
}

function get_data_set($id='')
{
	$sql = "SELECT s.id, s.set_id, s.cod_dim, d.name as description FROM ".TB_PREF."set s, ".TB_PREF."dimensions d";
	
	if ($id!='') $sql .= " WHERE s.cod_dim = d.id and s.id = ".db_escape ($id);

	$result = db_query($sql, "could not get set");
        return db_fetch($result);
}

$result = get_set_group();

start_form();
start_table(TABLESTYLE);
$th = array(_("Id"),_("Set id"), _("Description"),  _("Order"), "", "");
//inactive_control_column($th);
table_header($th);

$k = 0;
while ($myrow = db_fetch($result)) 
{
	alt_table_row_color($k);
        label_cell($myrow['id']);
	label_cell($myrow['set_id']);
	label_cell($myrow['description']);
        label_cell($myrow['order_']);
	//inactive_control_cell($myrow["id"], $myrow["inactive"], 'tags', 'id');
	edit_button_cell("Edit".$myrow["id"], _("Edit"));
	delete_button_cell("Delete".$myrow["id"], _("Delete"));
	end_row();
}

//inactive_control_row($th);
end_table(1);

//-----------------------------------------------------------------------------------

start_table(TABLESTYLE2);

if ($selected_id != -1) // We've selected a tag 
{
	if ($Mode == 'Edit') {
		// Editing an existing tag
		$myrow = get_data_set_group($selected_id);
	
                $_POST['id'] = $myrow["id"];
		$_POST['set_id'] = $myrow["set_id"];
		$_POST['description'] = $myrow["description"];
                $_POST['order_'] = $myrow["order_"];
                $_POST['porcent_serv'] = percent_format($myrow["porcent_serv"] * 100);
                $_POST['mb_flag'] = $myrow["mb_flag"];
                $_POST['valid_on_account'] = $myrow["valid_on_account"];
                $_POST['level_break_on_invoice_printing'] = $myrow["level_break_on_invoice_printing"];
                $_POST['level_break_on_quotation_printing'] = $myrow["level_break_on_quotation_printing"];
	}
	// Note the selected tag
	hidden('selected_id', $selected_id);
}
else {
    $_POST['porcent_serv'] = null;
}
text_row_ex(_("Set Id:"), 'set_id', 15, 30);
text_row_ex(_("Description:"), 'description', 40, 60);
text_row(_("Order:"), 'order_', null, 5, 3);
percent_row(_("Serv. Percent:"), 'porcent_serv', $_POST['porcent_serv']);
stock_item_types_list_row(_("Item Type:"), 'mb_flag', null, true);
check_row(_("Valid On The Account:"), 'valid_on_account', @$_POST['valid_on_account'],
	false, _('Valid On The Account'));
start_row();
echo "<tr><td class='label'>"._("Level Break On Invoice Printing:")."</td>";
echo "<td>";
echo level_break_account_list('level_break_on_invoice_printing', null);
echo "</td>";
end_row();
start_row();
echo "<tr><td class='label'>"._("Level Break On Quotation Printing:")."</td>";
echo "<td>";
echo level_break_account_list('level_break_on_quotation_printing', null);
echo "</td>";
end_row();
hidden('type');

end_table(1);

submit_add_or_update_center($selected_id == -1, '', 'both');

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
if ($selected_id != -1) // We've selected a tag 
{
br();

$result = get_set($_POST['set_id']);

start_table(TABLESTYLE);
$th = array(_("Id"),_("Dimension"), "", "");
table_header($th);

$k = 0;
while ($myrow = db_fetch($result)) 
{
	alt_table_row_color($k);
        label_cell($myrow['id']);
	label_cell($myrow['dimension']);
	edit_button_cell("BEd".$myrow["id"], _("Edit"));
	delete_button_cell("BDel".$myrow["id"], _("Delete"));
	end_row();
}

end_table(1);

div_start('edit_line');
	start_table(TABLESTYLE2);

	if ($selected_id2 != -1) 
	{
	 	if ($Mode2 == 'BEd') 
	 	{
			//editing an existing status code
			$myrow = get_data_set($selected_id2);

			$_POST['id']  = $myrow["id"];
			$_POST['set_id']  = $myrow["set_id"];
                        $_POST['cod_dim']  = $myrow["cod_dim"];
			
	 	}
	} 

        hidden('set_id', $_POST['set_id']);
        dimensions_list_row(_("Dimension")." 1", 'cod_dim', null, true, " ", false, 1);
	
	end_table(1);
	
	div_end();
	
	hidden('selected_id', $selected_id);
	hidden('selected_id2', $selected_id2);

	submit_add_or_update_center2($selected_id2 == -1, '', true);
}


end_form();

//------------------------------------------------------------------------------------

end_page();

?>
