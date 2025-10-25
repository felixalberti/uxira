<?php
/**********************************************************************
    Copyright Septiembre 2017 (C) Felix Alberti.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
$page_security = 'SA_POSSETUP';
$path_to_root = "../..";
include_once($path_to_root . "/includes/session.inc");

if (!@$_GET['popup'])
{
page(_($help_context = "Method Payments"));
}

include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/sales/includes/db/method_payments_group_role_db.inc");

simple_page_mode(true);
//----------------------------------------------------------------------------------------------------
$role = '-1';

/*if (isset($_POST['role_edit'])){
    display_notification('role_edit');
    $role_edit = $_POST['role_edit'];
}*/

function can_process()
{
	/*if (strlen($_POST['description']) == 0)
	{
		display_error(_("The description cannot be empty."));
		set_focus('description');
		return false;
	}*/
	return true;
}

//----------------------------------------------------------------------------------------------------

if ($Mode=='ADD_ITEM' && can_process())
{
        if (exists_method_payments($_POST['role'], $_POST['method_pay']) == 0 ){
            $inactive = (isset($_POST['inactive']) && $_POST['inactive'] == 1 ? 1 : 0);
            add_method_payments_group($_POST['role'], $_POST['method_pay'], $inactive);
            display_notification(_('New method payment has been added'));
            $Mode = 'RESET';            
            $role = $_POST['role'];
        }
        else {
            $role = $_POST['role'];
            display_warning('El metodo de pago ya existe para el rol');
        }
        
}

//----------------------------------------------------------------------------------------------------

if ($Mode=='UPDATE_ITEM' && can_process())
{
        $window = (isset($_POST['window']) &&$_POST['window'] == 1 ? 1 : 0);
        $selection_amount = (isset($_POST['selection_amount']) && $_POST['selection_amount'] == 1 ? 1 : 0);
        $inactive = (isset($_POST['inactive']) && $_POST['inactive'] == 1 ? 1 : 0);
	update_method_payments($selected_id, $_POST['description'], $window, $_POST['filter_type'],
                $selection_amount, $inactive);
	display_notification(_('Selected method payment has been updated'));
	$Mode = 'RESET';
}

//----------------------------------------------------------------------------------------------------

if ($Mode == 'Delete')
{	
		delete_method_payments_group($selected_id);
		display_notification(_('Selected method payment has been deleted'));
		$Mode = 'RESET';
                $role = $_POST['role'];
	
}

if ($Mode == 'RESET')
{       //display_notification('RESET');
	$selected_id = -1;
	$sav = get_post('show_inactive');
	unset($_POST);
	$_POST['show_inactive'] = $sav;
}
//----------------------------------------------------------------------------------------------------

if (list_updated('role') || isset($_POST['role'])){
   $role = $_POST['role'];
   $Ajax->activate('_page_body');
   //display_notification(1);
}
//elseif (isset($role_edit)){
//   $role = $role_edit;
//   $Ajax->activate('_page_body');
//   display_notification(2);
//}
elseif ($role != ''){
   //display_notification(3);
   $Ajax->activate('_page_body');
}
$result = get_all_method_payments_group_role($role);

start_form();

start_table(TABLESTYLE_NOBORDER);
start_row();
security_roles_list_cells(_("Role:"). "&nbsp;", 'role', $role, true, true, check_value('show_inactive'));
end_row();
end_table();
echo "<hr>";

start_table(TABLESTYLE);

$th = array (_('Id'), _('Description'), 
	 '','');
inactive_control_column($th);
table_header($th);
$k = 0;

while ($myrow = db_fetch($result))
{
    alt_table_row_color($k);
	label_cell($myrow["id"]);
	label_cell($myrow['description']);
	inactive_control_cell($myrow["id"], $myrow["inactive"], "method_payment_group", 'id');
 	edit_button_cell("Edit".$myrow['id'], _("Edit"));
 	delete_button_cell("Delete".$myrow['id'], _("Delete"));
	end_row();
}

inactive_control_row($th);
end_table(1);
//----------------------------------------------------------------------------------------------------

start_table(TABLESTYLE2);
$window = 0;
$inactive = 0;
if ($selected_id != -1)
{
        //$Ajax->activate('_page_body');
 	if ($Mode == 'Edit') {
		$myrow = get_method_payments_group($selected_id);

		//$_POST['description']  = $myrow["description"];
                //$role = $_POST['role_edit'] = $myrow["role"];
                
                $method_pay = $_POST['method_pay'] = $myrow["id_method"];
                $inactive = $_POST['inactive']  = $myrow["inactive"];
                //$Ajax->activate('_page_body');
                /*hidden('role_edit', $_POST['role_edit']);
                hidden('method_pay', $method_pay);               
                label_row(_("Role:"), $_POST['role_edit']);               
                label_row(_("Method Pay:"), $method_pay);*/
	}
	hidden('selected_id', $selected_id);
        
} 
//else {
    //$Ajax->activate('_page_body');
/*start_row();
security_roles_list_cells(_("Role:"). "&nbsp;", 'role_edit', null, false, true, check_value('show_inactive'));
end_row();*/
start_row();
metodos_pago_list_cells("Method Payments", 'method_pay', null, true, false);
end_row();
//}
check_row(_("Inactive"), 'inactive', $inactive);



end_table(1);

submit_add_or_update_center($selected_id == -1, '', 'both');

if (!@$_GET['popup'])
{
end_form();

end_page();
}

?>
