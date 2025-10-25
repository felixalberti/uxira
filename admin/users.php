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
$page_security = 'SA_USERS';
$path_to_root = "..";
include_once($path_to_root . "/includes/session.inc");

page(_($help_context = "Users"));

include_once($path_to_root . "/includes/date_functions.inc");
include_once($path_to_root . "/includes/ui.inc");

include_once($path_to_root . "/admin/db/users_db.inc");

simple_page_mode(true);
//-------------------------------------------------------------------------------------------------

function can_process() 
{

	if (strlen($_POST['user_id']) < 4)
	{
		display_error( _("The user login entered must be at least 4 characters long."));
		set_focus('user_id');
		return false;
	}

	if ($_POST['password'] != "") 
	{
    	if (strlen($_POST['password']) < 4)
    	{
    		display_error( _("The password entered must be at least 4 characters long."));
			set_focus('password');
    		return false;
    	}

    	if (strstr($_POST['password'], $_POST['user_id']) != false)
    	{
    		display_error( _("The password cannot contain the user login."));
			set_focus('password');
    		return false;
    	}
	}

	return true;
}

//-------------------------------------------------------------------------------------------------

if (($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') && check_csrf_token())
{

	if (can_process())
	{
    	if ($selected_id != -1) 
    	{
    		update_user_prefs($selected_id,
    			get_post(array('user_id', 'real_name', 'phone', 'email', 'role_id', 'language',
					'print_profile', 'rep_popup' => 0, 'pos',
                                        'modif_price_invoice', 'modif_porc_invoice', 'change_price_list',
                                        'modif_stat_patient_acc', 'is_supervising_user', 'user_is_cashier',
                                        'void_invoice','is_user_invoice','print_remote', 'change_ex_rate',
                                        'no_view_price_ord', 'dimension_id', 'user_is_type_orders')));

    		if ($_POST['password'] != "")
    			update_user_password($selected_id, $_POST['user_id'], md5($_POST['password']));

    		display_notification_centered(_("The selected user has been updated."));
    	} 
    	else 
    	{
    		add_user($_POST['user_id'], $_POST['real_name'], md5($_POST['password']),
				$_POST['phone'], $_POST['email'], $_POST['role_id'], $_POST['language'],
				$_POST['print_profile'], check_value('rep_popup'), $_POST['pos'],
                                @$_POST['modif_price_invoice'], @$_POST['modif_porc_invoice'],
                                @$_POST['change_price_list'], @$_POST['modif_stat_patient_acc'], @$_POST['is_supervising_user'],
                                @$_POST['user_is_cashier'], @$_POST['void_invoice'],
                                @$_POST['is_user_invoice'], @$_POST['print_remote'], @$_POST['change_ex_rate'],
                                @$_POST['no_view_price_ord'], @$_POST['dimension_id'], @$_POST['user_is_type_orders'] );
			$id = db_insert_id();
			// use current user display preferences as start point for new user
			$prefs = $_SESSION['wa_current_user']->prefs->get_all();
			
			update_user_prefs($id, array_merge($prefs, get_post(array('print_profile',
				'rep_popup' => 0, 'language'))));

			display_notification_centered(_("A new user has been added."));
    	}
		$Mode = 'RESET';
	}
}

//-------------------------------------------------------------------------------------------------

if ($Mode == 'Delete' && check_csrf_token())
{
	delete_user($selected_id);
	display_notification_centered(_("User has been deleted."));
	$Mode = 'RESET';
}

//-------------------------------------------------------------------------------------------------
if ($Mode == 'RESET')
{
 	$selected_id = -1;
	$sav = get_post('show_inactive');
	unset($_POST);	// clean all input fields
	$_POST['show_inactive'] = $sav;
}

$result = get_users(check_value('show_inactive'));
start_form();
start_table(TABLESTYLE);

$th = array(_("User login"), _("Full Name"), _("Phone"),
	_("E-mail"), _("Last Visit"), _("Access Level"), "", "");

inactive_control_column($th);
table_header($th);	

$k = 0; //row colour counter

while ($myrow = db_fetch($result)) 
{

	alt_table_row_color($k);

	$last_visit_date = sql2date($myrow["last_visit_date"]);

	/*The security_headings array is defined in config.php */
	$not_me = strcasecmp($myrow["user_id"], $_SESSION["wa_current_user"]->username);

	label_cell($myrow["user_id"]);
	label_cell($myrow["real_name"]);
	label_cell($myrow["phone"]);
	email_cell($myrow["email"]);
	label_cell($last_visit_date, "nowrap");
	label_cell($myrow["role"]);
	
    if ($not_me)
		inactive_control_cell($myrow["id"], $myrow["inactive"], 'users', 'id');
	elseif (check_value('show_inactive'))
		label_cell('');

	edit_button_cell("Edit".$myrow["id"], _("Edit"));
    if ($not_me)
 		delete_button_cell("Delete".$myrow["id"], _("Delete"));
	else
		label_cell('');
	end_row();

} //END WHILE LIST LOOP

inactive_control_row($th);
end_table(1);
//-------------------------------------------------------------------------------------------------
start_table(TABLESTYLE2);

$_POST['email'] = "";
if ($selected_id != -1) 
{
  	if ($Mode == 'Edit') {
		//editing an existing User
		$myrow = get_user($selected_id);

		$_POST['id'] = $myrow["id"];
		$_POST['user_id'] = $myrow["user_id"];
		$_POST['real_name'] = $myrow["real_name"];
		$_POST['phone'] = $myrow["phone"];
		$_POST['email'] = $myrow["email"];
		$_POST['role_id'] = $myrow["role_id"];
		$_POST['language'] = $myrow["language"];
		$_POST['print_profile'] = $myrow["print_profile"];
		$_POST['rep_popup'] = $myrow["rep_popup"];
		$_POST['pos'] = $myrow["pos"];
                $_POST['modif_price_invoice'] = $myrow["modif_price_invoice"];
                $_POST['modif_porc_invoice'] = $myrow["modif_porc_invoice"];
                $_POST['change_price_list'] = $myrow["change_price_list"];
                $_POST['modif_stat_patient_acc'] = $myrow["modif_stat_patient_acc"];
                $_POST['is_supervising_user'] = $myrow["is_supervising_user"];
                $_POST['user_is_cashier'] = $myrow["user_is_cashier"];
                $_POST['void_invoice'] = $myrow["void_invoice"];
                $_POST['is_user_invoice'] = $myrow["is_user_invoice"];
                $_POST['print_remote'] = $myrow["print_remote"];
                $_POST['change_ex_rate'] = $myrow["change_ex_rate"];
                $_POST['no_view_price_ord'] = $myrow["no_view_price_ord"];
                $_POST['dimension_id'] = $myrow["dimension_id"];
				$_POST['user_is_type_orders'] = $myrow["user_is_type_orders"];
	}
	hidden('selected_id', $selected_id);
	hidden('user_id');

	start_row();
	label_row(_("User login:"), $_POST['user_id']);
} 
else 
{ //end of if $selected_id only do the else when a new record is being entered
	text_row(_("User Login:"), "user_id",  null, 22, 20);
	$_POST['language'] = user_language();
	$_POST['print_profile'] = user_print_profile();
	$_POST['rep_popup'] = user_rep_popup();
	$_POST['pos'] = user_pos();
        $_POST['modif_price_invoice'] = 0;
        $_POST['modif_porc_invoice'] = 0;
        $_POST['change_price_list'] = 0;
        $_POST['modif_stat_patient_acc'] = 0;
        $_POST['is_supervising_user'] = 0;
        $_POST['user_is_cashier'] = 0;
        $_POST['void_invoice'] = 0;
        $_POST['is_user_invoice'] = 0;
        $_POST['print_remote'] = 0;
        $_POST['change_ex_rate'] = 0;
        $_POST['no_view_price_ord'] = 0;
        $_POST['dimension_id'] = 0;
		$_POST['user_is_type_orders'] = 0;
}
$_POST['password'] = "";
password_row(_("Password:"), 'password', $_POST['password']);

if ($selected_id != -1) 
{
	table_section_title(_("Enter a new password to change, leave empty to keep current."));
}

text_row_ex(_("Full Name").":", 'real_name',  50);

text_row_ex(_("Telephone No.:"), 'phone', 30);

email_row_ex(_("Email Address:"), 'email', 50);

security_roles_list_row(_("Access Level:"), 'role_id', null); 

languages_list_row(_("Language:"), 'language', null);

pos_list_row(_("User's POS"). ':', 'pos', null);

print_profiles_list_row(_("Printing profile"). ':', 'print_profile', null,
	_('Browser printing support'));

check_row(_("Use popup window for reports:"), 'rep_popup', $_POST['rep_popup'],
	false, _('Set this option to on if your browser directly supports pdf files'));

check_row(_("Modify price invoice:"), 'modif_price_invoice', $_POST['modif_price_invoice'],
	false, _(''));

check_row(_("Modify porc. invoice:"), 'modif_porc_invoice', $_POST['modif_porc_invoice'],
	false, _(''));

check_row(_("Change price list:"), 'change_price_list', $_POST['change_price_list'],
	false, _(''));

check_row(_("Patient account:"), 'modif_stat_patient_acc', $_POST['modif_stat_patient_acc'],
	false, _(''));

check_row(_("Is Supervising user:"), 'is_supervising_user', $_POST['is_supervising_user'],
	false, _(''));

check_row(_("Is a user cashier:"), 'user_is_cashier', $_POST['user_is_cashier'],
	false, _(''));

check_row(_("Void invoice:"), 'void_invoice', $_POST['void_invoice'],
	false, _(''));

check_row(_("Is user invoice:"), 'is_user_invoice', $_POST['is_user_invoice'],
	false, _(''));

check_row(_("Print remote:"), 'print_remote', $_POST['print_remote'],
	false, _(''));

check_row(_("Change ex rate:"), 'change_ex_rate', $_POST['change_ex_rate'],
	false, _(''));

check_row(_("Dont let view price ord rate:"), 'no_view_price_ord', $_POST['no_view_price_ord'],
	false, _(''));

dimensions_list_row_user(_("Dimension")." 1", 'dimension_id', null, true, " ", false, 1);

check_row(_("Is a user type orders:"), 'user_is_type_orders', $_POST['user_is_type_orders'],
	false, _(''));

end_table(1);

submit_add_or_update_center($selected_id == -1, '', 'both');

end_form();
end_page();
?>
