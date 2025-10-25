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
//--------------------------------------------------------------------------------------------------

function get_security_role($id)
{
	$sql = "SELECT * FROM ".TB_PREF."security_roles WHERE id=".(int)$id;
	$ret = db_query($sql, "could not retrieve security roles");
	$row = db_fetch($ret);
	if ($row != false) {
		$row['areas'] = explode(';', $row['areas']);
		$row['sections'] = explode(';', $row['sections']);
	}
	return $row;
}

//--------------------------------------------------------------------------------------------------

function add_security_role($name, $description, $sections, $areas, $category_sales, $curr_code)
{       $category_sales = ($category_sales == '' ? 0 : $category_sales);
        $curr_code = ($curr_code == '0' ? NULL : $curr_code);
	$sql = "INSERT INTO ".TB_PREF."security_roles (role, description, sections, areas, sales_type_category, curr_code)
	VALUES ("
	.db_escape($name).","
	.db_escape($description).","
	.db_escape(implode(';', $sections)).","
	.db_escape(implode(';', $areas)).","
        .db_escape($category_sales).","
        .db_escape($curr_code).")";

	db_query($sql, "could not add new security role");
}

//--------------------------------------------------------------------------------------------------

function update_security_role($id, $name, $description, $sections, $areas, $category_sales, $curr_code)
{
        $category_sales = ($category_sales == '' ? 0 : $category_sales);
        $curr_code = ($curr_code == '0' ? NULL : $curr_code);
    	$sql = "UPDATE ".TB_PREF."security_roles SET role=".db_escape($name)
    	.",description=".db_escape($description)
    	.",sections=".db_escape(implode(';', $sections))
    	.",areas=".db_escape(implode(';', $areas))
        .",sales_type_category=".db_escape($category_sales)
        .",curr_code=".db_escape($curr_code)
    	." WHERE id=".(int)$id;
    	db_query($sql, "could not update role");
}
//--------------------------------------------------------------------------------------------------

function delete_security_role($id)
{
	$sql = "DELETE FROM ".TB_PREF."security_roles WHERE id=".(int)$id;

	db_query($sql, "could not delete role");
}
//--------------------------------------------------------------------------------------------------

function check_role_used($id) {
	$sql = "SELECT count(*) FROM ".TB_PREF."users WHERE role_id=".(int)$id;
	$ret = db_query($sql, 'cannot check role usage');
	$row = db_fetch($ret);
	return $row[0];
}

/*Begin Felix Alberti 19/10/2018*/

function get_security_role_plugins($id)
{
	$sql = "SELECT count(*) as cant FROM ".TB_PREF."security_roles_plugins WHERE id=".(int)$id;
	$ret = db_query($sql, "could not retrieve security roles plugins");
	$row = db_fetch($ret);
	return $row['cant'];
}

function get_security_role_plugins_by_rol_grp($id,$grp)
{
	$sql = "SELECT count(*) as cant FROM ".TB_PREF."security_roles_plugins"
                . " WHERE id_rol=".(int)$id." and group_products=$grp";
	$ret = db_query($sql, "could not retrieve security roles plugins");
	$row = db_fetch($ret);
	return $row['cant'];
}

function get_security_role_plugins_all($id)
{
	$sql = "SELECT srp.*, sg.description FROM ".TB_PREF."security_roles_plugins srp, ".TB_PREF."set_group sg WHERE id_rol=".(int)$id.
                " and sg.set_id = srp.group_products";
	$ret = db_query($sql, "could not retrieve security roles plugins");
	return $ret;
}

function add_security_role_plugins($id, $groups_prod)
{
	$sql = "INSERT INTO ".TB_PREF."security_roles_plugins (id_rol, group_products)
	VALUES ("
	.db_escape($id).","
	.db_escape($groups_prod).")";

	db_query($sql, "could not add new security role plugins");
}

//--------------------------------------------------------------------------------------------------

function update_security_role_plugins($id, $id_role, $groups_prod)
{
    	$sql = "UPDATE ".TB_PREF."security_roles_plugins SET"
    	." id_rol=".db_escape($id_role)
    	.",group_products=".db_escape($groups_prod)
    	." WHERE id=".(int)$id;
    	db_query($sql, "could not update role plugins");
}

function delete_security_role_plugins($id)
{
    	$sql = "DELETE FROM ".TB_PREF."security_roles_plugins"
    	." WHERE id=".(int)$id;
    	db_query($sql, "could not delete role plugins");
}
/*End Felix Alberti 19/10/2018*/

?>