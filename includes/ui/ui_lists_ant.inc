<?php
/**********************************************************************
    Copyright (C) Uxira, C.A.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
include_once($path_to_root . "/includes/banking.inc");
include_once($path_to_root . "/includes/types.inc");
include_once($path_to_root . "/includes/current_user.inc");

$_search_button = "<input %s type='submit' class='combo_submit' style='border:0;background:url($path_to_root/themes/"
	."%s/images/locate.png) no-repeat;%s' aspect='fallback' name='%s' value=' ' title='"._("Set filter")."'> ";

$_select_button = "<input %s type='submit' class='combo_select' style='border:0;background:url($path_to_root/themes/"
	."%s/images/button_ok.png) no-repeat;%s' aspect='fallback' name='%s' value=' ' title='"._("Select")."'> ";

$all_items = ALL_TEXT;

//----------------------------------------------------------------------------
//	Universal sql combo generator
//	$sql must return selector values and selector texts in columns 0 & 1
//	Options are merged with defaults.

function combo_input($name, $selected_id, $sql, $valfield, $namefield,
	$options=null)
{ 
global $Ajax;

$opts = array(		// default options
	'where'=> array(),		// additional constraints
	'order' => $namefield,	// list sort order
		// special option parameters
	'spec_option'=>false, 	// option text or false
	'spec_id' => 0,		// option id
		// submit on select parameters
	'default' => '', // default value when $_POST is not set
	'multi' => false,	// multiple select
	'select_submit' => false, //submit on select: true/false
	'async' => true,	// select update via ajax (true) vs _page_body reload
		// search box parameters
	'sel_hint' => null,
	'search_box' => false, 	// name or true/false
	'type' => 0,	// type of extended selector:
		// 0 - with (optional) visible search box, search by fragment inside id
		// 1 - with hidden search box, search by option text
		// 2 - with (optional) visible search box, search by fragment at the start of id
		// 3 - TODO reverse: box with hidden selector available via enter; this
		// would be convenient for optional ad hoc adding of new item
	'search_submit' => true, //search submit button: true/false
	'size' => 8,	// size and max of box tag
	'max' => 50,
	'height' => false,	// number of lines in select box
	'cells' => false,	// combo displayed as 2 <td></td> cells
	'search' => array(), // sql field names to search
	'format' => null, 	 // format functions for regular options
	'disabled' => false,
	'box_hint' => null, // box/selectors hints; null = std see below
	'category' => false, // category column name or false
	'show_inactive' => false, // show inactive records. 
	'editable' => false, // false, or length of editable entry field
        'datamulti' => array() // Data multiple seleccioned
);


// ------ merge options with defaults ----------
	if($options != null)
		$opts = array_merge($opts, $options);
        /*if ($name=='medico_evaluation'){
        $converted_res = ($opts['search_submit']) ? 'true' : 'false';*/
        //display_notification("search_submit ".$converted_res);        
        //}
	if (!is_array($opts['where']))  $opts['where'] = array($opts['where']);
        
	$search_box = $opts['search_box']===true ? '_'.$name.'_edit' : $opts['search_box'];
	// select content filtered by search field:
	$search_submit = $opts['search_submit']===true ? '_'.$name.'_button' : $opts['search_submit'];
	// select set by select content field
	$search_button = $opts['editable'] ? '_'.$name.'_button' : ($search_box ? $search_submit : false);

	$select_submit =  $opts['select_submit'];
	$spec_id = $opts['spec_id'];
	$spec_option = $opts['spec_option'];
	if ($opts['type'] == 0) {
		$by_id = true;
		$class = 'combo';
	} elseif($opts['type'] == 1) {
		$by_id = false;
		$class = 'combo2';
	} else {
		$by_id = true;
		$class = 'combo3';
	}

	$disabled = $opts['disabled'] ? "disabled" : '';
	$multi = $opts['multi'];
        
	
	if(!count($opts['search'])) {
		$opts['search'] = array($by_id ? $valfield : $namefield);
	}
	if ($opts['sel_hint'] === null) 
		$opts['sel_hint'] = $by_id || $search_box==false ?
			'' : _('Press Space tab for search pattern entry');

	if ($opts['box_hint'] === null)
		$opts['box_hint'] = $search_box && $search_submit != false ?
			($by_id ? _('Enter code fragment to search or * for all')
			: _('Enter description fragment to search or * for all')) :'';

	if ($selected_id == null) {
		$selected_id = get_post($name, (string)$opts['default']);
	}
	if(!is_array($selected_id))
		$selected_id = array((string)$selected_id); // code is generalized for multiple selection support

	$txt = get_post($search_box);
	$rel = '';
	$limit = '';
        
        
	if (isset($_POST['_'.$name.'_update'])) { // select list or search box change
		if ($by_id) $txt = $_POST[$name];

		if (!$opts['async'])
			$Ajax->activate('_page_body');
		else
			$Ajax->activate($name);
	}
	if (isset($_POST[$search_button])) {
		if (!$opts['async'])
			$Ajax->activate('_page_body');
		else
			$Ajax->activate($name);
	}
	if ($search_box) {
		// search related sql modifications

		$rel = "rel='$search_box'"; // set relation to list
		if ($opts['search_submit']) {
			if (isset($_POST[$search_button])) {
				$selected_id = array(); // ignore selected_id while search
				if (!$opts['async'])
					$Ajax->activate('_page_body');
				else
					$Ajax->activate($name);
			}
			if ($txt == '') {
				if ($spec_option === false && $selected_id == array())
					$limit = ' LIMIT 1';
				else
					$opts['where'][] = $valfield . "=". db_escape(get_post($name, $spec_id));
			}
			else
				if ($txt != '*') {

					foreach($opts['search'] as $i=> $s)
						$opts['search'][$i] = $s . " LIKE "
							.db_escape(($class=='combo3' ? '' : '%').$txt.'%');
					$opts['where'][] = '('. implode($opts['search'], ' OR ') . ')';
				}
		}
	}

	// sql completion
	if (count($opts['where'])) {
		$where = strpos($sql, 'WHERE')==false ? ' WHERE ':' AND ';
		$where .= '('. implode($opts['where'], ' AND ') . ')';
		$group_pos = strpos($sql, 'GROUP BY');
		if ($group_pos) {
			$group = substr($sql, $group_pos);
			$sql = substr($sql, 0, $group_pos) . $where.' '.$group;
		} else {
			$sql .= $where;
		}
	}
	if ($opts['order'] != false) {
		if (!is_array($opts['order']))
			$opts['order'] = array($opts['order']);
		$sql .= ' ORDER BY '.implode(',',$opts['order']);
	}
      
	$sql .= $limit;
        
        //display_warning($sql);
        
	// ------ make selector ----------
	$selector = $first_opt = '';
	$first_id = false;
	$found = false;
	$lastcat = null;
	$edit = false;
//if($name=='stock_id')	display_notification('<pre>'.print_r($_POST, true).'</pre>');
//if($name=='curr_default') display_notification($opts['search_submit']);
	if($result = db_query($sql)) {
                $fila = 0;/*Felix Alberti 27/11/2015*/
		while ($contact_row = db_fetch($result)) {
			$value = $contact_row[0];                        
			$descr = $opts['format']==null ?  $contact_row[1] :
				call_user_func($opts['format'], $contact_row);
			$sel = '';
 		 	if (get_post($search_button) && ($txt == $value)) {
 		 		$selected_id[] = $value;
 		 	}
                          
                        /*Felix Alberti 27/11/2015*/
                         /*Si es seleccion multiple*/
                        if(count($opts['datamulti']) > 0 && $fila < count($opts['datamulti'])) { 
                            //display_notification($opts['datamulti'][$fila]);
                            $selected_id[] = $opts['datamulti'][$fila];
                            $fila++;
                        }
                        /*Felix Alberti 27/11/2015*/

 		 	if (in_array((string)$value, $selected_id, true)) {
				$sel = 'selected';
				$found = $value;
				$edit = $opts['editable'] && $contact_row['editable'] 
					&& (@$_POST[$search_box] == $value)
					? $contact_row[1] : false; // get non-formatted description
				if ($edit)
					break;	// selected field is editable - abandon list construction
			}                       
                        
			// show selected option even if inactive 
			if (!$opts['show_inactive'] && @$contact_row['inactive'] && $sel==='') {
				continue;
			} else 
				$optclass = @$contact_row['inactive'] ? "class='inactive'" : '';

			if ($first_id === false) {
				$first_id = $value;
				$first_opt = $descr;
			}
			$cat = $contact_row[$opts['category']];
			if ($opts['category'] !== false && $cat != $lastcat){
				$selector .= "<optgroup label='".$cat."'>\n";
				$lastcat = $cat;
			}
                       
			$selector .= "<option $sel $optclass value='$value'>$descr</option>\n";
		}
		db_free_result($result);
	}

	// Prepend special option.
	if ($spec_option !== false) { // if special option used - add it
		$first_id = $spec_id;
		$first_opt = $spec_option;
//	}
//	if($first_id !== false) {
		$sel = $found===false ? 'selected' : '';
		$optclass = @$contact_row['inactive'] ? "class='inactive'" : '';
		$selector = "<option $sel value='$first_id'>$first_opt</option>\n"
			. $selector;
	}

	if ($found===false) {
		$selected_id = array($first_id);
	}
	
	$_POST[$name] = $multi ? $selected_id : $selected_id[0];

	$selector = "<select autocomplete='off' ".($multi ? "multiple" : '')
		. ($opts['height']!==false ? ' size="'.$opts['height'].'"' : '')
		. "$disabled name='$name".($multi ? '[]':'')."' class='$class' title='"
		. $opts['sel_hint']."' $rel>".$selector."</select>\n";

	if ($by_id && ($search_box != false || $opts['editable']) ) {
		// on first display show selector list
		if (isset($_POST[$search_box]) && $opts['editable'] && $edit) {
			$selector = "<input type='hidden' name='$name' value='".$_POST[$name]."'>"
			."<input type='text' $disabled name='{$name}_text' id='{$name}_text' size='".
				$opts['editable']."' maxlength='".$opts['max']."' $rel value='$edit'>\n";
				set_focus($name.'_text'); // prevent lost focus
		} else if (get_post($search_submit ? $search_submit : "_{$name}_button"))
			set_focus($name); // prevent lost focus
		if (!$opts['editable'])
	 		$txt = $found;
		$Ajax->addUpdate($name, $search_box, $txt ? $txt : '');
	}

	$Ajax->addUpdate($name, "_{$name}_sel", $selector);

	// span for select list/input field update
	$selector = "<span id='_{$name}_sel'>".$selector."</span>\n";

	 // if selectable or editable list is used - add select button
	if ($select_submit != false || $search_button) {
		global $_select_button;
	// button class selects form reload/ajax selector update
		$selector .= sprintf($_select_button, $disabled, user_theme(),
			(fallback_mode() ? '' : 'display:none;'),
			 '_'.$name.'_update')."\n";
	}
// ------ make combo ----------
	$edit_entry = '';
	if ($search_box != false) {
		$edit_entry = "<input $disabled type='text' name='$search_box' id='$search_box' size='".
			$opts['size']."' maxlength='".$opts['max'].
			"' value='$txt' class='$class' rel='$name' autocomplete='off' title='"
			.$opts['box_hint']."'"
			.(!fallback_mode() && !$by_id ? " style=display:none;":'')
			.">\n";
		if ($search_submit != false || $opts['editable']) {
			global $_search_button;
			$edit_entry .= sprintf($_search_button, $disabled, user_theme(),
				(fallback_mode() ? '' : 'display:none;'),
				$search_submit ? $search_submit : "_{$name}_button")."\n";
		}
	}
	default_focus(($search_box && $by_id) ? $search_box : $name);
        
	if ($search_box && $opts['cells'])
		$str = ($edit_entry!='' ? "<td>$edit_entry</td>" : '')."<td>$selector</td>";
	else
		$str = $edit_entry.$selector;
        
                    
	return $str;
}

/*
	Helper function.
	Returns true if selector $name is subject to update.
*/
function list_updated($name)
{
	return isset($_POST['_'.$name.'_update']) || isset($_POST['_'.$name.'_button']);
}
//----------------------------------------------------------------------------------------------
//	Universal array combo generator
//	$items is array of options 'value' => 'description'
//	Options is reduced set of combo_selector options and is merged with defaults.

function array_selector($name, $selected_id, $items, $options=null)
{
	global $Ajax;

$opts = array(		// default options
	'spec_option'=>false, 	// option text or false
	'spec_id' => 0,		// option id
	'select_submit' => false, //submit on select: true/false
	'async' => true,	// select update via ajax (true) vs _page_body reload
	'default' => '', // default value when $_POST is not set
	'multi'=>false,	// multiple select
		// search box parameters
	'height' => false,	// number of lines in select box
	'sel_hint' => null,
	'disabled' => false
);
// ------ merge options with defaults ----------
	if($options != null)
		$opts = array_merge($opts, $options);
	$select_submit =  $opts['select_submit'];
	$spec_id = $opts['spec_id'];
	$spec_option = $opts['spec_option'];
	$disabled = $opts['disabled'] ? "disabled" : '';
	$multi = $opts['multi'];

	if ($selected_id == null) {
		$selected_id = get_post($name, $opts['default']);
	}
	if(!is_array($selected_id))
		$selected_id = array((string)$selected_id); // code is generalized for multiple selection support

	if (isset($_POST[ '_'.$name.'_update'])) {
		if (!$opts['async'])
			$Ajax->activate('_page_body');
		else
			$Ajax->activate($name);
	}

	// ------ make selector ----------
	$selector = $first_opt = '';
	$first_id = false;
	$found = false;
//if($name=='SelectStockFromList') display_error($sql);
		foreach($items as $value=>$descr) {
			$sel = '';
			if (in_array((string)$value, $selected_id, true)) {
				$sel = 'selected';
				$found = $value;
			}
			if ($first_id === false) {
				$first_id = $value;
				$first_opt = $descr;
			}
			$selector .= "<option $sel value='$value'>$descr</option>\n";
		}

	if ($first_id!==false) {
		$sel = ($found===$first_id) || ($found===false && ($spec_option===false)) ? "selected='selected'" : '';
		$selector = sprintf($first_opt, $sel).$selector;
	}
	// Prepend special option.
	if ($spec_option !== false) { // if special option used - add it
		$first_id = $spec_id;
		$first_opt = $spec_option;
		$sel = $found===false ? 'selected' : '';
		$selector = "<option $sel value='$spec_id'>$spec_option</option>\n"
			. $selector;
	}

	if ($found===false) {
		$selected_id = array($first_id);
	}
	$_POST[$name] = $multi ? $selected_id : $selected_id[0];

	$selector = "<select autocomplete='off' ".($multi  ? "multiple" : '')
		. ($opts['height']!==false ? ' size="'.$opts['height'].'"' : '')
		. "$disabled name='$name".($multi ? '[]' : '')."' class='combo' title='"
		. $opts['sel_hint']."'>".$selector."</select>\n";

	$Ajax->addUpdate($name, "_{$name}_sel", $selector);

	$selector = "<span id='_{$name}_sel'>".$selector."</span>\n";

	if ($select_submit != false) { // if submit on change is used - add select button
		global $_select_button;
		$selector .= sprintf($_select_button, $disabled, user_theme(),
			(fallback_mode() ? '' : 'display:none;'),
			 '_'.$name.'_update')."\n";
	}
	default_focus($name);

	return $selector;
}
//----------------------------------------------------------------------------------------------

function _format_add_curr($row)
{
	static $company_currency;

	if ($company_currency == null)
	{
		$company_currency = get_company_currency();
	}
	return $row[1] . ($row[2] == $company_currency ?
		'' : ("&nbsp;-&nbsp;" . $row[2]));
}

function add_edit_combo($type)
{
	global $path_to_root, $popup_editors, $use_icon_for_editkey;

	if (!isset($use_icon_for_editkey) || $use_icon_for_editkey==0)
		return "";
	// Derive theme path
	$theme_path = $path_to_root . '/themes/' . user_theme();

	$key = $popup_editors[$type][1];
	$onclick = "onclick=\"javascript:callEditor($key); return false;\"";
	$img = "<img width='12' height='12' border='0' alt='Add/Edit' title='Add/Edit' src='$theme_path/images/".ICON_EDIT."'>";
	return "<a target = '_blank' href='#' $onclick tabindex='-1'>$img</a>"; 
}

function supplier_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false,
	$all=false, $editkey = false)
{
	global $all_items;

	$sql = "SELECT supplier_id, supp_ref, curr_code, inactive FROM ".TB_PREF."suppliers ";

	$mode = get_company_pref('no_supplier_list');

	if ($editkey)
		set_editor('supplier', $name, $editkey);
		
	$ret = combo_input($name, $selected_id, $sql, 'supplier_id', 'supp_name',
	array(
		'format' => '_format_add_curr',
	    'order' => array('supp_ref'),
		'search_box' => $mode!=0,
		'type' => 1,
		'spec_option' => $spec_option === true ? _("All Suppliers") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment') :
		_('Select supplier'),
		'show_inactive'=>$all
		));
	if ($editkey)
		$ret .= add_edit_combo('supplier');		
	return $ret;
}

function supplier_list_cells($label, $name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $all=false, $editkey = false)
{
	if ($label != null)
		echo "<td>$label</td><td>\n";
		echo supplier_list($name, $selected_id, $all_option, $submit_on_change, 
		$all, $editkey);
		echo "</td>\n";
}

function supplier_list_row($label, $name, $selected_id=null, $all_option = false, 
	$submit_on_change=false, $all=false, $editkey = false)
{
	echo "<tr><td class='label'>$label</td><td>";
	echo supplier_list($name, $selected_id, $all_option, $submit_on_change,
		$all, $editkey);
	echo "</td></tr>\n";
}
//----------------------------------------------------------------------------------------------
/*Felix Alberti 24/03/2016 se agregó el parametro $mode*/
function customer_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, 
	$show_inactive=false, $editkey = false, $mode = true)
{    
	global $all_items;

	$sql = "SELECT debtor_no, debtor_ref, curr_code, inactive, tax_id, name FROM ".TB_PREF."debtors_master ";
        
        /*Begin Felix Alberti 24/03/2016*/
        if (!$mode)
        $mode = 0;
        else
        /*End Felix Alberti 24/03/2016*/
	$mode = get_company_pref('no_customer_list');
        //display_notification('mode '.$mode);
	if ($editkey)
		set_editor('customer', $name, $editkey);

	$ret = combo_input($name, $selected_id, $sql, 'debtor_no', 'debtor_ref',
	array(
	    'format' => '_format_add_curr',
	    'order' => array('debtor_ref'),
		//'search_box' => $mode!=0,
                'search_box' => true,
		'search' => array("tax_id", "name",  "debtor_ref"),
                'search_submit' => get_company_pref('no_customer_list')!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("All Customers") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F2 - entry new customer') :
		_('Select customer'),
		'show_inactive' => $show_inactive
	) );
	if ($editkey)
		$ret .= add_edit_combo('customer');
	return $ret;
}
/*fap 22/06/2015*/

/*Felix Alberti 07/12/2016 se agregó el parametro $mode*/
function find_professional_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, 
	$show_inactive=false, $editkey = false, $mode = true)
{    
	global $all_items;

        $sql = "SELECT professional_id, description, tax_id, curr_code, inactive FROM ".TB_PREF."professional";
        
        /*Begin Felix Alberti 24/03/2016*/
        if (!$mode)
        $mode = 0;
        else
        /*End Felix Alberti 24/03/2016*/
	$mode = get_company_pref('no_customer_list');
        //display_notification('mode '.$mode);
//	if ($editkey)
//		set_editor('customer', $name, $editkey);

	$ret = combo_input($name, $selected_id, $sql, 'professional_id', 'description',
	array(
	    'format' => '_format_add_curr',
	    'order' => array('description'),
		//'search_box' => $mode!=0,
                'search_box' => true,
		'search' => array("professional_id", "tax_id",  "description"),
                'search_submit' => get_company_pref('no_customer_list')!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Select Professional") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F6 - entry new profesional') :
		_('Select professional'),
		'show_inactive' => $show_inactive
	) );
//	if ($editkey)
//		$ret .= add_edit_combo('customer');
	return $ret;
}
/*fap 07/12/2016*/


/*fap 06/12/2016*/
function responsible_payment_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, 
	$show_inactive=false, $editkey = false, $mode = true)
{    
	global $all_items;

	$sql = "SELECT debtor_no, debtor_ref, curr_code, inactive, tax_id, name FROM ".TB_PREF."debtors_master ";
        
        /*Begin Felix Alberti 24/03/2016*/
        if (!$mode)
        $mode = 0;
        else
        /*End Felix Alberti 24/03/2016*/
	$mode = get_company_pref('no_customer_list');
        //display_notification('mode '.$mode);
	if ($editkey)
		set_editor('customer', $name, $editkey);

	$ret = combo_input($name, $selected_id, $sql, 'debtor_no', 'debtor_ref',
	array(
	    'format' => '_format_add_curr',
	    'order' => array('debtor_ref'),
		//'search_box' => $mode!=0,
                'search_box' => true,
		'search' => array("tax_id", "name",  "debtor_ref"),
                'search_submit' => get_company_pref('no_customer_list')!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Select Responsible") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F2 - entry new customer') :
		_('Select customer'),
		'show_inactive' => $show_inactive
	) );
	if ($editkey)
		$ret .= add_edit_combo('customer');
	return $ret;
}
/*fap 06/12/2015*/


function responsable_pago_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, $datamulti = null,
	$show_inactive=false, $editkey=false, $multi=true)
{
	global $all_items;
        

	$sql = "SELECT debtor_no, debtor_ref, curr_code, inactive FROM ".TB_PREF."debtors_master ";

	$mode = get_company_pref('no_responsible_list');

	if ($editkey)
		set_editor('responsible', $name, $editkey);
        
	$ret = combo_input($name, $selected_id, $sql, 'debtor_no', 'debtor_ref',
	array(
	    'format' => '_format_add_curr',
	    'order' => array('debtor_ref'),
		//'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		//'spec_option' => $spec_option === true ? _("All Responsible") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		//'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F5 - entry new responsible') :
		_('Select responsible'),
		'show_inactive' => $show_inactive,
                'multi' => $multi,
                'datamulti' => $datamulti
	) );
	if ($editkey)
		$ret .= add_edit_combo('responsible');
	return $ret;
}

/*fap 23/05/2016*/
function responsable_pago_selected_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, $datamulti = null,
	$show_inactive=false, $editkey=false, $multi=true)
{
	global $all_items;
        

	$sql = "SELECT debtor_no, debtor_ref, curr_code, inactive FROM ".TB_PREF."debtors_master ";
        
        if (is_array($selected_id)){
           $selected_id_in = implode(',', $selected_id);
           $sql .= "WHERE debtor_no in (".$selected_id_in.")";
        }
        else
        if ($selected_id!=null) {
            $selected_id_in = $selected_id;
            $sql .= "WHERE debtor_no in (".$selected_id_in.")";
        }
        
        //display_warning($sql);

	$mode = get_company_pref('no_responsible_list');

	if ($editkey)
		set_editor('responsible', $name, $editkey);
        
	$ret = combo_input($name, $selected_id, $sql, 'debtor_no', 'debtor_ref',
	array(
	    'format' => '_format_add_curr',
	    'order' => array('debtor_ref'),
		//'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		//'spec_option' => $spec_option === true ? _("All Responsible") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		//'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F5 - entry new responsible') :
		_('Select responsible'),
		'show_inactive' => $show_inactive,
                'multi' => $multi,
                'datamulti' => $datamulti
	) );
	if ($editkey)
		$ret .= add_edit_combo('responsible');
	return $ret;
}
/*fap 23/05/2016*/

/*Begin 24/03/2016*/
function medico_a_cargo_multi_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, $datamulti = null,
	$show_inactive=false, $editkey=false, $multi=true)
{
	global $all_items;

        $sql = "SELECT professional_id, description FROM ".TB_PREF."professional where type_professional_id = 1";
        
        /*if (is_array($selected_id)){
           $selected_id_in = implode(',', $selected_id);
           $sql .= " and professional_id in (".$selected_id_in.")";
        }
        else
        if ($selected_id!=null) {
            $selected_id_in = $selected_id;
            $sql .= " and professional_id in (".$selected_id_in.")";
        }*/
        //display_notification($sql);

	$mode = get_company_pref('no_medico_a_cargo_list');

	if ($editkey)
		set_editor('responsible', $name, $editkey);
        
	$ret = combo_input($name, $selected_id, $sql, 'professional_id', 'description',
	array(
	    //'format' => '_format_add_curr',
	    'order' => array('description'),
		//'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		//'spec_option' => $spec_option === true ? _("All Responsible") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		//'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F5 - entry new responsible') :
		_('Select medico tratante'),
		//'show_inactive' => $show_inactive,
                'multi' => $multi,
                'datamulti' => $datamulti
	) );
	if ($editkey)
		$ret .= add_edit_combo('responsible');
	return $ret;
}

/*Begin Felix Alberti 24/03/2016*/

/*Begin Felix Alberti 23/03/2016*/
//------------------------------------------------------------------------------
function responsable_pago_find_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, $datamulti = null,
	$show_inactive=false, $editkey=false, $multi=true)
{       //display_notification('responsable_pago_find_list '.$name);
	global $all_items;
        

	$sql = "SELECT debtor_no, debtor_ref, name, curr_code, inactive FROM ".TB_PREF."debtors_master ";
        
       
	$mode = get_company_pref('no_responsible_list');

	if ($editkey)
		set_editor('responsible', $name, $editkey);
        
	$ret = combo_input($name, $selected_id, $sql, 'debtor_no', 'debtor_ref',
	array(
	    'format' => '_format_add_curr',
	    'order' => array('debtor_ref'),
		//'search_box' => $mode!=0,            
                'search_box' => true,
		'search' => array("tax_id", "debtor_ref"),
                'search_submit' => get_company_pref('no_customer_list')!=0,
		'type' => 1,
		'size' => 20,
		//'spec_option' => $spec_option === true ? _("All Responsible") : $spec_option,
                'spec_option' => $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		//'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F5 - entry new responsible') :
		_('Select responsible'),
		'show_inactive' => $show_inactive,
                'multi' => $multi,
                'datamulti' => $datamulti
	) );
	if ($editkey)
		$ret .= add_edit_combo('responsible');
	return $ret;
}
/*End Felix Alberti 23/03/2016*/

/*******************************************************************************/
/*Begin Felix Alberti 01/12/2016*/
//------------------------------------------------------------------------------
function account_patient_find_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, 
	$show_inactive=false, $editkey=false)
{       //display_notification('responsable_pago_find_list '.$name);
	global $all_items;
        

	$sql = "SELECT id, concat(id,' ',p.debtor_no,' ',dm.name) as acc, p.inactive FROM ".TB_PREF."patient_accounts p, "
                . TB_PREF."debtors_master dm  WHERE dm.tax_id = p.debtor_no ";
        
       
	$mode = get_company_pref('no_customer_list');

	if ($editkey)
		set_editor('patient', $name, $editkey);
        
	$ret = combo_input($name, $selected_id, $sql, 'p.id', 'acc',
	array(
	    //'format' => '_format_add_curr',
	    'order' => array('p.id desc'),
		//'search_box' => $mode!=0,            
                'search_box' => true,
		'search' => array("id"),
                'search_submit' => get_company_pref('no_customer_list')!=0,
		'type' => 1,
		'size' => 20,
		//'spec_option' => $spec_option === true ? _("All Responsible") : $spec_option,
                'spec_option' => $spec_option === true ? _("Select Account") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		//'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F5 - entry new responsible') :
		_('Select responsible'),
		'show_inactive' => $show_inactive
	) );
	if ($editkey)
		$ret .= add_edit_combo('patient');
	return $ret;
}
/*End Felix Alberti 01/12/2016*/

/*Begin Felix Alberti 28/06/2017*/
//------------------------------------------------------------------------------
function destination_account_patient_find_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, 
	$show_inactive=false, $editkey=false, $debtor_no=null)
{       
	global $all_items;
        

	$sql = "SELECT id, concat(id,' ',p.debtor_no,' ',dm.name) as acc,  0 as inactive FROM ".TB_PREF."patient_accounts p, "
                . TB_PREF."debtors_master dm  WHERE dm.tax_id = p.debtor_no";
        
        if ($debtor_no != null) $sql .= " and dm.debtor_no = ".  db_escape($debtor_no);
        //display_notification('responsable_pago_find_list '.$sql);
	$mode = get_company_pref('no_customer_list');

	if ($editkey)
		set_editor('patient', $name, $editkey);
        
	$ret = combo_input($name, $selected_id, $sql, 'p.id', 'acc',
	array(
	    //'format' => '_format_add_curr',
	    'order' => array('p.tran_date desc'),
		//'search_box' => $mode!=0,            
                'search_box' => true,
		'search' => array("id"),
                'search_submit' => get_company_pref('no_customer_list')!=0,
		'type' => 1,
		'size' => 20,
		//'spec_option' => $spec_option === true ? _("All Responsible") : $spec_option,
                'spec_option' => $spec_option === true ? _("Select Account") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'async' => false,
		//'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F5 - entry new responsible') :
		_('Select responsible'),
		'show_inactive' => $show_inactive
	) );
	if ($editkey)
		$ret .= add_edit_combo('patient');
	return $ret;
}
/*End Felix Alberti 28/06/2016*/

function customer_list_cells($label, $name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $show_inactive=false, $editkey = false, $mode = true, $classlabel=null)
{
	if ($label != null){
                if ($classlabel != null)
                echo "<td class=$classlabel>$label</td>\n";
                else
		echo "<td>$label</td>\n";
        }
	echo "<td nowrap>";
	echo customer_list($name, $selected_id, $all_option, $submit_on_change,
		$show_inactive, $editkey, $mode);
	echo "</td>\n";
}

function customer_list_row($label, $name, $selected_id=null, $all_option = false, 
	$submit_on_change=false, $show_inactive=false, $editkey = false)
{
	global $path_to_root;

	echo "<tr><td class='label'>$label</td><td nowrap>";
	echo customer_list($name, $selected_id, $all_option, $submit_on_change,
		$show_inactive, $editkey);
	echo "</td>\n</tr>\n";
}

/*Begin 07/12/2016*/
function professional_list_row($label, $name, $selected_id=null, $all_option = false, 
	$submit_on_change=false, $show_inactive=false, $editkey = false)
{
	global $path_to_root;

	echo "<tr><td class='label'>$label</td><td nowrap>";
	echo find_professional_list($name, $selected_id, $all_option, $submit_on_change,
		$show_inactive, $editkey);
	echo "</td>\n</tr>\n";
}
/*End 07/12/2016*/

/*Begin Felix Alberti 06/12/2016*/
function responsible_payment_list_row($label, $name, $selected_id=null, $all_option = false, 
	$submit_on_change=false, $show_inactive=false, $editkey = false)
{
	global $path_to_root;

	echo "<tr><td class='label'>$label</td><td nowrap>";
	echo responsible_payment_list($name, $selected_id, $all_option, $submit_on_change,
		$show_inactive, $editkey);
	echo "</td>\n</tr>\n";
}
/*End Felix Alberti 06/12/2016*/

/*Begin Felix Alberti 27/12/2016*/
function responsible_payment_list_cell($label=null, $name, $selected_id=null, $all_option = false, 
	$submit_on_change=false, $show_inactive=false, $editkey = false)
{
	global $path_to_root;

        if ($label!=null)
	echo "<td class='label'>$label</td>";
        echo "<td nowrap>";
	echo responsible_payment_list($name, $selected_id, $all_option, $submit_on_change,
		$show_inactive, $editkey);
	echo "</td>\n";
}
/*End Felix Alberti 27/12/2016*/

//Begin Felix Alberti 01/12/2016
function patient_account_find_list_row($label, $name, $selected_id=null, $all_option = false, 
	$submit_on_change=false, $show_inactive=false, $editkey = false)
{
	global $path_to_root;

	echo "<tr><td class='label'>$label</td><td nowrap>";
	echo account_patient_find_list($name, $selected_id, $all_option, $submit_on_change,
		$show_inactive, $editkey);
	echo "</td>\n</tr>\n";
}
//End Felix Alberti 01/12/2016

//Begin Felix Alberti 07/02/2017
function patient_account_find_list_cell($label, $name, $selected_id=null, $all_option = false, 
	$submit_on_change=false, $show_inactive=false, $editkey = false, $classlabel=null)
{
	
        if ($label != null){
                if ($classlabel != null)
                echo "<td class=$classlabel>$label</td>\n";
                else
		echo "<td>$label</td>\n";
        }
	echo "<td nowrap>";
	echo account_patient_find_list($name, $selected_id, $all_option, $submit_on_change,
		$show_inactive, $editkey);
	echo "</td>\n";
}
//End Felix Alberti 07/02/2017

//Begin Felix Alberti 28/06/2017
function patient_destination_account_find_list_cell($label, $name, $selected_id=null, $all_option = false, 
	$submit_on_change=false, $show_inactive=false, $editkey = false, $classlabel=null, $debtor_no=null)
{
	
        if ($label != null){
                if ($classlabel != null)
                echo "<td class=$classlabel>$label</td>\n";
                else
		echo "<td>$label</td>\n";
        }
	echo "<td nowrap>";
	echo destination_account_patient_find_list($name, $selected_id, $all_option, $submit_on_change,
		$show_inactive, $editkey, $debtor_no);
	echo "</td>\n";
}
//End Felix Alberti 28/06/2017

function responsable_pago_list_cell($label, $name, $selected_id=null, $all_option = false, $datamulti = null,
	$submit_on_change=false, $show_inactive=false, $editkey = false )
{
	global $path_to_root;
        
        if ($label != null)
	echo "<td class='label'>$label</td>";
        echo "<td nowrap>";
	echo responsable_pago_list($name, $selected_id, $all_option, $submit_on_change, $datamulti,
		$show_inactive, $editkey, false);               
	echo "</td>";
}

function responsable_pago_find_list_cell($label, $name, $selected_id=null, $all_option = false, $datamulti = null,
	$submit_on_change=false, $show_inactive=false, $editkey = false )
{
	global $path_to_root;
               
	echo responsable_pago_find_list($name, $selected_id, $all_option, $submit_on_change, $datamulti,
		$show_inactive, $editkey, false);             
	
}

function responsable_pago_list_row($label, $name, $selected_id=null, $all_option = false, $datamulti = null,
	$submit_on_change=false, $show_inactive=false, $editkey = false )
{
	global $path_to_root;
                
	echo "<tr><td class='label'>$label</td><td nowrap>";
	echo responsable_pago_list($name, $selected_id, $all_option, $submit_on_change, $datamulti,
		$show_inactive, $editkey);
	echo "</td>\n</tr>\n";
}

function responsable_pago_selected_list_row($label, $name, $selected_id=null, $all_option = false, $datamulti = null,
	$submit_on_change=false, $show_inactive=false, $editkey = false )
{
	global $path_to_root;
                
	echo "<tr><td class='label'>$label</td><td nowrap>";
	echo responsable_pago_selected_list($name, $selected_id, $all_option, $submit_on_change, $datamulti,
		$show_inactive, $editkey);
	echo "</td>\n</tr>\n";
}

function medico_a_cargo_multi_list_row($label, $name, $selected_id=null, $all_option = false, $datamulti = null,
	$submit_on_change=false, $show_inactive=false, $editkey = false )
{
	global $path_to_root;
              
	echo "<tr><td class='label'>$label</td><td nowrap>";
	echo medico_a_cargo_multi_list($name, $selected_id, $all_option, $submit_on_change, $datamulti,
		$show_inactive, $editkey);
	echo "</td>\n</tr>\n";
}

//------------------------------------------------------------------------------------------------

function customer_branches_list($customer_id, $name, $selected_id=null,
	$spec_option = true, $enabled=true, $submit_on_change=false, $editkey = false)
{
	global $all_items;

	$sql = "SELECT branch_code, branch_ref FROM ".TB_PREF."cust_branch
		WHERE debtor_no=" . db_escape($customer_id)." ";

	if ($editkey)
		set_editor('branch', $name, $editkey);

	$where = $enabled ? array("disable_trans = 0") : array();
	$ret = combo_input($name, $selected_id, $sql, 'branch_code', 'branch_ref',
	array(
		'where' => $where,
		'order' => array('branch_ref'),
		'spec_option' => $spec_option === true ? _('All branches') : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'sel_hint' => _('Select customer branch')
	) );
	if ($editkey)
	{
		$ret .= add_edit_combo('branch');
	}	
	return $ret;
}
//------------------------------------------------------------------------------------------------

function customer_branches_list_cells($label,$customer_id, $name, $selected_id=null, 
	$all_option = true, $enabled=true, $submit_on_change=false, $editkey = false, $classlabel=null)
{
	if ($label != null){
            if ($classlabel != null)
            echo "<td class=$classlabel>$label</td>\n";
            else
	    echo "<td>$label</td>\n";
        }
	echo "<td>";
	echo customer_branches_list($customer_id, $name, $selected_id, $all_option, $enabled, 
		$submit_on_change, $editkey);
	echo "</td>\n";
}

function customer_branches_list_row($label, $customer_id, $name, $selected_id=null, 
	$all_option = true, $enabled=true, $submit_on_change=false, $editkey = false)
{
	echo "<tr><td class='label'>$label</td>";
	customer_branches_list_cells(null, $customer_id, $name, $selected_id, 
		$all_option, $enabled, $submit_on_change, $editkey);
	echo "</tr>";
}

//------------------------------------------------------------------------------------------------

function locations_list($name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT loc_code, location_name, inactive FROM ".TB_PREF."locations";

return combo_input($name, $selected_id, $sql, 'loc_code', 'location_name',
	array(
		'spec_option' => $all_option === true ? _("All Locations") : $all_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change
	) );
}

function locations_list_cells($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo locations_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n";
}

function locations_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	locations_list_cells(null, $name, $selected_id, $all_option, $submit_on_change);
	echo "</tr>\n";
}

//-----------------------------------------------------------------------------------------------
/*Begin Felix Alberti 14/04/2016*/
function metodos_pago_list($name, $selected_id=null, $all_option=false, $submit_on_change=false, $type_trans=false)
{
	global $all_items;
        
        $role = $_SESSION['wa_current_user']->access;
        
        if ($all_option)
        $sql = "SELECT m.id, description, m.inactive FROM "
                .TB_PREF."metodos_pago m";
        else
	$sql = "SELECT m.id, description, m.inactive FROM "
                .TB_PREF."metodos_pago m, ".
                TB_PREF."method_payment_group mpg, ".
                TB_PREF."method_payment_assoc_document mpad"
                . " WHERE mpg.role = $role and mpg.id_method = m.id and mpad.type_trans = ".$type_trans.
                " and mpg.inactive = 0 and mpad.id_method = mpg.id_method and mpad.inactive = 0";

return combo_input($name, $selected_id, $sql, 'mpg.id', 'description',
	array(
                'order' => array('id'),
		//'spec_option' => $all_option === true ? _("All Metodos Pago") : $all_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change
	) );
}

function metodos_pago_list_cells($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $type_trans=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo metodos_pago_list($name, $selected_id, $all_option, $submit_on_change, $type_trans);
	echo "</td>\n";
}

function metodos_pago_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $type_trans=false)
{
	echo "<tr><td class='label'>$label</td>";
	metodos_pago_list_cells(null, $name, $selected_id, $all_option, $submit_on_change, $type_trans);
	echo "</tr>\n";
}
/*End Felix Alberti 14/04/2016*/

//-----------------------------------------------------------------------------------------------

function currencies_list($name, $selected_id=null, $submit_on_change=false)
{
	$sql = "SELECT curr_abrev, currency, inactive FROM ".TB_PREF."currencies";

// default to the company currency
return combo_input($name, $selected_id, $sql, 'curr_abrev', 'currency',
	array(
		'select_submit'=> $submit_on_change,
		'default' => get_company_currency(),
		'async' => false		
	) );
}

function currencies_list_cells($label, $name, $selected_id=null, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo currencies_list($name, $selected_id, $submit_on_change);
	echo "</td>\n";
}

function currencies_list_row($label, $name, $selected_id=null, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	currencies_list_cells(null, $name, $selected_id, $submit_on_change);
	echo "</tr>\n";
}

//---------------------------------------------------------------------------------------------------

function fiscalyears_list($name, $selected_id=null, $submit_on_change=false)
{

	$sql = "SELECT * FROM ".TB_PREF."fiscal_year";

// default to the company current fiscal year

return combo_input($name, $selected_id, $sql, 'id', '',
	array(
		'order' => 'begin',
		'default' => get_company_pref('f_year'),
		'format' => '_format_fiscalyears',
		'select_submit'=> $submit_on_change,
		'async' => false
	) );
}

function _format_fiscalyears($row)
{
	return sql2date($row[1]) . "&nbsp;-&nbsp;" . sql2date($row[2])
	. "&nbsp;&nbsp;" . ($row[3] ? _('Closed') : _('Active')) . "</option>\n";
}

function fiscalyears_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo fiscalyears_list($name, $selected_id);
	echo "</td>\n";
}

function fiscalyears_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	fiscalyears_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}
//------------------------------------------------------------------------------------

function dimensions_list($name, $selected_id=null, $no_option=false, $showname=' ',
	$submit_on_change=false, $showclosed=false, $showtype=1)
{
$sql = "SELECT id, CONCAT(reference,'  ',name) as ref FROM ".TB_PREF."dimensions";

$options = array(
	'order' => 'reference',
	'spec_option'=>$no_option ? $showname : false,
	'spec_id' => 0,
	'select_submit'=> $submit_on_change,
	'async' => false,
	);

	if (!$showclosed)
		$options['where'][] = "closed=0";
	if($showtype)
		$options['where'][] = "type_=".db_escape($showtype);

	return combo_input($name, $selected_id, $sql, 'id', 'ref', $options);
}

function dimensions_list_cells($label, $name, $selected_id=null, $no_option=false, $showname=null,
	$showclosed=false, $showtype=0, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo dimensions_list($name, $selected_id, $no_option, $showname, $submit_on_change, $showclosed, $showtype);
	echo "</td>\n";
}

function dimensions_list_row($label, $name, $selected_id=null, $no_option=false, $showname=null,
	$showclosed=false, $showtype=0, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	dimensions_list_cells(null, $name, $selected_id, $no_option, $showname,
		$showclosed, $showtype, $submit_on_change);
	echo "</tr>\n";
}

/*Begin Felix Alberti 29/07/2016*/
function dimensions_group_list($name, $selected_id=null, $no_option=false, $showname=' ',
	$submit_on_change=false, $showclosed=false, $showtype=1, $filtergroup=false)
{
$sql = "SELECT d.id, CONCAT(reference,'  ',name) as ref FROM ".TB_PREF."dimensions d,"
        . TB_PREF."set s WHERE s.cod_dim = d.id";

$options = array(
	'order' => 'reference',
	'spec_option'=>$no_option ? $showname : false,
	'spec_id' => 0,
	'select_submit'=> $submit_on_change,
	'async' => false,
	);

	if (!$showclosed)
		$options['where'][] = "closed=0";
	if($showtype)
		$options['where'][] = "type_=".db_escape($showtype);
        if($filtergroup)
		$options['where'][] = "s.set_id=".db_escape($filtergroup);
        
        //display_notification($sql);

	return combo_input($name, $selected_id, $sql, 'd.id', 'ref', $options);
}

function dimensions_group_list_cells($label, $name, $selected_id=null, $no_option=false, $showname=null,
	$showclosed=false, $showtype=0, $submit_on_change=false, $filtergroup=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo dimensions_group_list($name, $selected_id, $no_option, $showname, $submit_on_change, $showclosed, $showtype, $filtergroup);
	echo "</td>\n";
}

function dimensions_group_list_row($label, $name, $selected_id=null, $no_option=false, $showname=null,
	$showclosed=false, $showtype=0, $submit_on_change=false, $filtergroup=false)
{
	echo "<tr><td class='label'>$label</td>";
	dimensions_group_list_cells(null, $name, $selected_id, $no_option, $showname,
		$showclosed, $showtype, $submit_on_change, $filtergroup);
	echo "</tr>\n";
}
/*End Felix Alberti 29/07/2016*/

//---------------------------------------------------------------------------------------------------

function stock_items_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $opts=array(), $editkey = false)
{
        
	global $all_items;

	$sql = "SELECT stock_id, s.description, c.description, s.inactive, s.editable
			FROM ".TB_PREF."stock_master s,".TB_PREF."stock_category c WHERE s.category_id=c.category_id";

	if ($editkey)
		set_editor('item', $name, $editkey);

	$ret = combo_input($name, $selected_id, $sql, 'stock_id', 's.description',
	array_merge(
	  array(
		'format' => '_format_stock_items',
		'spec_option' => $all_option===true ?  _("All Items") : $all_option,
		'spec_id' => $all_items,
		'search_box' => true,
		'search' => array("stock_id", "c.description","s.description"),
		'search_submit' => get_company_pref('no_item_list')!=0,
		'size'=>10,
		'select_submit'=> $submit_on_change,
		'category' => 2,
		'order' => array('c.description','stock_id')
	  ), $opts) );
	if ($editkey)
		$ret .= add_edit_combo('item');		
	return $ret;
	  
}


/*Begin Felix Alberti 08/07/2016*/
function stock_items_category_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $opts=array(), $editkey = false, $category=false, $dimension=false)
{
        
	global $all_items;

	$sql = "SELECT stock_id, s.description, c.description, s.inactive, s.editable, s.our_code, s.source_code, s.descripcion3, s.marca, s.long_description
			FROM ".TB_PREF."stock_master s,".TB_PREF."stock_category c WHERE s.category_id=c.category_id";
        
        if ($category!==false && $category!==-1){
            $sql .= " and s.category_id = ".$category;
        }
        if ($dimension!==false && $dimension!==0){
            $sql .= " and s.dimension_id = ".$dimension;
        }
        //display_notification($sql);
	if ($editkey)
		set_editor('item', $name, $editkey);

	$ret = combo_input($name, $selected_id, $sql, 'stock_id', 's.description',
	array_merge(
	  array(
		'format' => '_format_stock_items',
		'spec_option' => $all_option===true ?  _("All Items") : $all_option,
		'spec_id' => $all_items,
		'search_box' => true,
		'search' => array("s.our_code","s.source_code","stock_id","c.description","s.description","s.long_description","s.descripcion3","s.marca"),
		'search_submit' => get_company_pref('no_item_list')!=0,
		'size'=>10,
		'select_submit'=> $submit_on_change,
		'category' => 2,
		'order' => array('c.description','s.description','s.our_code','stock_id')
	  ), $opts) );
	if ($editkey)
		$ret .= add_edit_combo('item');		
	return $ret;
	  
}
/*End Felix Alberti 08/07/2016*/
 //-----------------------------------------------------------------------------
 //Begin FAP 16/03/2016
 function stock_items_services_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $opts=array(), $editkey = false)
{
        
	global $all_items;

	$sql = "SELECT stock_id, s.description, c.description, s.inactive, s.editable
			FROM ".TB_PREF."stock_master s,".TB_PREF."stock_category c "
                . "WHERE s.category_id=c.category_id and s.category_id = ".$opts['services'];

	if ($editkey)
		set_editor('item', $name, $editkey);

	$ret = combo_input($name, $selected_id, $sql, 'stock_id', 's.description',
	array_merge(
	  array(
		'format' => '_format_stock_items',
		'spec_option' => $all_option===true ?  _("All Items") : $all_option,
		'spec_id' => $all_items,
		'search_box' => true,
		'search' => array("stock_id", "c.description","s.description"),
		'search_submit' => get_company_pref('no_item_list')!=0,
		'size'=>10,
		'select_submit'=> $submit_on_change,
		'category' => 2,
		'order' => array('c.description','stock_id')
	  ), $opts) );
	if ($editkey)
		$ret .= add_edit_combo('item');		
	return $ret;
	  
}
 //End FAP 16/03/2016
//------------------------------------------------------------------------------

function _format_stock_items($row)
{
	return (user_show_codes() ?  ($row[0] . "&nbsp;-&nbsp;") : "") . $row[1];
}

function stock_items_list_cells($label, $name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $all=false, $editkey = false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo stock_items_list($name, $selected_id, $all_option, $submit_on_change,
		array('cells'=>true, 'show_inactive'=>$all), $editkey);
}

/*Begin Felix Alberti 08/07/2016*/
function stock_items_category_list_cells($label, $name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $all=false, $editkey = false, $category=false, $dimension=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo stock_items_category_list($name, $selected_id, $all_option, $submit_on_change,
		array('cells'=>true, 'show_inactive'=>$all), $editkey, $category, $dimension);
}
/*End Felix Alberti 08/07/2016*/

function stock_items_services_list_cells($label, $name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $all=false, $editkey = false)
{
	/*if ($label != null)
		echo "<td>$label</td>\n";*/
	echo stock_items_services_list($name, $selected_id, $all_option, $submit_on_change,
		array('cells'=>true, 'show_inactive'=>$all, 'search_box'=>true, 'services'=>4), $editkey);
}

/*function stock_items_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	echo "<tr>\n";
	stock_items_list_cells($label, $name, $selected_id, $all_option, $submit_on_change);
	echo "</tr>\n";
}*/

/*Begin Felix Alberti 15/03/2016*/
function stock_items_services_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	echo "<tr>\n";
	stock_items_services_list_cells($label, $name, $selected_id, $all_option, $submit_on_change);
	echo "</tr>\n";
}
/*End Felix Alberti 15/03/2016*/

//---------------------------------------------------------------------------------------------------
//
// Select item via foreign code.
//
function sales_items_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $type='', $opts=array())
{
	global $all_items;
	// all sales codes
	$sql = "SELECT i.item_code, i.description, c.description, count(*)>1 as kit,
			 i.inactive, if(count(*)>1, '0', s.editable) as editable
			FROM
			".TB_PREF."stock_master s,
			".TB_PREF."item_codes i
			LEFT JOIN
			".TB_PREF."stock_category c
			ON i.category_id=c.category_id
			WHERE i.stock_id=s.stock_id";

	
	if ($type == 'local')	{ // exclude foreign codes
		$sql .=	" AND !i.is_foreign"; 
	} elseif ($type == 'kits') { // sales kits
		$sql .=	" AND !i.is_foreign AND i.item_code!=i.stock_id";
	}
	$sql .= " AND !i.inactive AND !s.inactive AND !s.no_sale";
	$sql .= " GROUP BY i.item_code";

	return combo_input($name, $selected_id, $sql, 'i.item_code', 'c.description',
	array_merge(
	  array(
		'format' => '_format_stock_items',
		'spec_option' => $all_option===true ?  _("All Items") : $all_option,
		'spec_id' => $all_items,
		'search_box' => true,
		'search' => array("i.item_code", "c.description", "i.description"),
		'search_submit' => get_company_pref('no_item_list')!=0,
		'size'=>15,
		'select_submit'=> $submit_on_change,
		'category' => 2,
		'order' => array('c.description','i.item_code'),
		'editable' => 30,
		'max' => 255
	  ), $opts) );
}

function sales_items_list_cells($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $editkey=false)
{
	if ($editkey)
		set_editor('item', $name, $editkey);

	if ($label != null)
		echo "<td>$label</td>\n";
	echo sales_items_list($name, $selected_id, $all_option, $submit_on_change,
		'', array('cells'=>true));
}

function sales_items_list_cells_link($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $editkey=false)
{
	if ($editkey)
		set_editor('item', $name, $editkey);

	if ($label != null)
		echo "<td>".find_item_link($label)."</td>\n";
	echo sales_items_list($name, $selected_id, $all_option, $submit_on_change,
		'', array('cells'=>true));
}
/*fap 22/06/2015*/
function profesional_items_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $type='', $opts=array())        
{   
/*$converted_res = ($submit_on_change) ? 'true' : 'false';

    display_notification($converted_res);*/
	global $all_items;
	// all sales codes
	$sql = "SELECT p.professional_id, p.description, t.description, count(*)>1 as kit,
			 p.inactive, if(count(*)>1, '0', '1') as editable
			FROM
			".TB_PREF."professional p LEFT JOIN ".TB_PREF."type_professional t".
			" ON (p.type_professional_id = t.id)";			
	
	$sql .= " WHERE !p.inactive";
        //if ($selected_id != null) $sql .= " and p.professional_id = $selected_id";
        if ($type != '')
        $sql .= " and p.type_professional_id = " .$type; 
        
	$sql .= " GROUP BY p.type_professional_id, p.professional_id";        
	//display_notification("query profesionales:".$sql);  

	return combo_input($name, $selected_id, $sql, 'p.professional_id', 't.description',
	array_merge(
	  array(
		'format' => '_format_stock_items',
		'spec_option' => $all_option===true ?  _("Todos") : $all_option,
		'spec_id' => $all_items,
		//'search_box' => true,
		'search' => array("p.professional_id", "t.description", "p.description"),
		'search_submit' => get_company_pref('no_item_list')!=0,
		'size'=>15,
		'select_submit'=> $submit_on_change,
		'category' => 2,
		'order' => array('p.type_professional_id','p.professional_id'),
		//'editable' => 30,
		'max' => 255
	  ), $opts) );
}
/*fap 22/06/2015*/
function profesional_items_list_cells($name, $selected_id=null, $all_option=false, $submit_on_change=false, $editkey=false, $type='')
{
	if ($editkey)
		set_editor('item', $name, $editkey);
		
            //display_notification("profesional_items_list_cells");
	echo "<td>";
	echo profesional_items_list($name, $selected_id, $all_option, $submit_on_change, $type, array('cells'=>true));
	echo "</td>\n";
}

/*Begin 02/04/2016*/
function profesional_items_services_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $type='', $stock_id, $opts=array())        
{   
/*$converted_res = ($submit_on_change) ? 'true' : 'false';*/

        //display_notification($type.'-'.$stock_id);
	global $all_items;
	// all sales codes
        /*if ($selected_id != null && $type ==''){
        $sql = "SELECT p.professional_id, p.description, p.description, count(*)>1 as kit,
			 p.inactive, if(count(*)>1, '0', '1') as editable
			FROM ".TB_PREF."professional p";        
        $sql .= " WHERE professional_id = " .db_escape($selected_id); 
        $search = array("p.professional_id", "p.description", "p.description");
        $all_option = false;
        }
        else {*/
	$sql = "SELECT p.professional_id, p.description, t.description, count(*)>1 as kit,
			 p.inactive, if(count(*)>1, '0', '1') as editable
			FROM
			".TB_PREF."professional p, ".TB_PREF."type_professional t, ".
			TB_PREF."professional_items pi";			
	
	$sql .= " WHERE !p.inactive";
        $sql .= " and p.type_professional_id = t.id";
        $sql .= " and pi.id_professional = p.professional_id";
        //if ($selected_id != null) $sql .= " and p.professional_id = $selected_id";
        //if ($type != '')
        //$sql .= " and p.type_professional_id = " .db_escape($type);
        if ($stock_id != '')
        $sql .= " and pi.stock_id = " .db_escape($stock_id); 
        
	$sql .= " GROUP BY p.type_professional_id, p.professional_id";
        $search = array("p.professional_id", "t.description", "p.description");
        //}
	//display_warning("query profesionales:".$sql);  

	return combo_input($name, $selected_id, $sql, 'p.professional_id', 't.description',
	array_merge(
	  array(
		'format' => '_format_stock_items',
		'spec_option' => $all_option===true ?  _("Selection Professional") : $all_option,
		'spec_id' => $all_items,
		//'search_box' => true,
		'search' => array("p.professional_id", "t.description", "p.description"),
		'search_submit' => get_company_pref('no_item_list')!=0,
		'size'=>15,
		'select_submit'=> $submit_on_change,
		'category' => 2,
		'order' => array('p.type_professional_id','p.professional_id'),
		//'editable' => 30,
		'max' => 255
	  ), $opts) );
}
/*End 01/04/2016*/

/*Begin 01/04/2016*/
function profesional_items_services_list_cells($name, $selected_id=null, $all_option=false, $submit_on_change=false, $editkey=false, $type='', $tock_id='')
{
	if ($editkey)
		set_editor('item', $name, $editkey);		
           
	echo "<td>";
	echo profesional_items_services_list($name, $selected_id, $all_option, $submit_on_change, $type, $tock_id, array('cells'=>true));
	echo "</td>\n";
}
/*End 01/04/2016*/

/*fap 03/12/2015*/
function profesional_items_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $type)
{
	echo "<tr><td class='label'>$label</td>";
	profesional_items_list_cells($name, $selected_id, $all_option, $submit_on_change, false, $type);
	echo "</tr>\n";
}
/*fap 03/12/2015*/

/*fap 11/12/2015*/
function type_med_serv_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $opts=array())        
{   

	global $all_items;
	// all sales codes
	$sql = "SELECT t.id, t.description, t.description, t.inactive, 0 as editable
			FROM
			".TB_PREF."type_medical_service t";			
	
	$sql .= " WHERE !t.inactive";

	return combo_input($name, $selected_id, $sql, 't.id', 't.description',
	array_merge(
	  array(
		'format' => '_format_stock_items',
		'spec_option' => $all_option===true ?  _("Todos") : $all_option,
		'spec_id' => $all_items,
		'search' => array("t.id", "t.description", "t.description"),
		'search_submit' => false,
		'size'=>15,
		'select_submit'=> $submit_on_change,
		'order' => array('t.id'),
		//'editable' => 30,
		'max' => 255
	  ), $opts) );
}
/*fap 11/12/2015*/

/*fap 11/12/2015*/
function type_med_serv_list_cells($name, $selected_id=null, $all_option=false, $submit_on_change=false, $editkey=false)
{
	if ($editkey)
		set_editor('item', $name, $editkey);
		
	echo "<td>";
	echo type_med_serv_list($name, $selected_id, $all_option, $submit_on_change, array('cells'=>true));
	echo "</td>\n";
}
/*fap 11/12/2015*/

/*fap 11/12/2015*/
function type_med_serv_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	type_med_serv_list_cells($name, $selected_id, $all_option, $submit_on_change, false);
	echo "</tr>\n";
}
/*fap 11/12/2015*/

/*fap 19/06/2016*/
function number_bed_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $opts=array())        
{   

	global $all_items;
	// all sales codes
	$sql = "SELECT id, description, description, inactive, 0 as editable
			FROM
			".TB_PREF."number_beds";			
	
	$sql .= " WHERE !inactive";

	return combo_input($name, $selected_id, $sql, 'id', 'description',
	array_merge(
	  array(
		'format' => '_format_stock_items',
		'spec_option' => $all_option===true ?  _("Todos") : $all_option,
		'spec_id' => $all_items,
		'search' => array("id", "description", "description"),
		'search_submit' => false,
		'size'=>15,
		'select_submit'=> $submit_on_change,
		'order' => array('id'),
		//'editable' => 30,
		'max' => 255
	  ), $opts) );
}
/*fap 19/06/2016*/

/*fap 19/06/2016*/
function number_bed_list_cells($name, $selected_id=null, $all_option=false, $submit_on_change=false, $editkey=false)
{
	if ($editkey)
		set_editor('item', $name, $editkey);
		
	echo "<td>";
	echo number_bed_list($name, $selected_id, $all_option, $submit_on_change, array('cells'=>true));
	echo "</td>\n";
}
/*fap 19/06/2016*/

/*fap 19/06/2016*/
function number_bed_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	number_bed_list_cells($name, $selected_id, $all_option, $submit_on_change, false);
	echo "</tr>\n";
}
/*fap 19/06/2016*/

/*fap 11/12/2015*/
function key_term_insurance_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $opts=array())        
{   

	global $all_items;
	// all sales codes
	$sql = "SELECT k.id, k.description, k.description, k.inactive, 0 as editable
			FROM
			".TB_PREF."key_term_insurance k";			
	
	$sql .= " WHERE !k.inactive";

	return combo_input($name, $selected_id, $sql, 'id', 'description',
	array(
		'select_submit'=> $submit_on_change,
		'async' => false		
	) );
}
/*fap 11/12/2015*/

/*fap 11/12/2015*/
function key_term_insurance_list_cells($name, $selected_id=null, $all_option=false, $submit_on_change=false, $editkey=false)
{
	if ($editkey)
		set_editor('item', $name, $editkey);
		
	echo "<td>";
	echo key_term_insurance_list($name, $selected_id, $all_option, $submit_on_change, array('cells'=>true));
	echo "</td>\n";
}
/*fap 11/12/2015*/

/*fap 11/12/2015*/
function key_term_insurance_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	key_term_insurance_list_cells($name, $selected_id, $all_option, $submit_on_change, false);
	echo "</tr>\n";
}
/*fap 11/12/2015*/

/*fap 11/12/2015*/
function key_type_insurance_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $opts=array())        
{   
    	global $all_items;
	// all sales codes
	$sql = "SELECT k.id, k.description, k.description, k.inactive, 0 as editable, k.order_
			FROM
			".TB_PREF."key_type_insurance k";			
	
	$sql .= " WHERE !k.inactive";
        if ($selected_id!=null){
            if (is_array($selected_id)){
            $selected_id_sel = implode(',', $selected_id);    
            $sql .= " and k.id in ( ".  $selected_id_sel.")";
            }
            else
            $sql .= " and k.id = ".  db_escape($selected_id);
        }
        //display_notification($sql);
        return combo_input($name, $selected_id, $sql, 'k.id', 'k.description',
	array(
                'order' => array('k.order_'),
		'select_submit'=> $submit_on_change,
		'async' => false		
	) );
}
/*fap 11/12/2015*/

/*fap 11/12/2015*/
function key_type_insurance_list_cells($name, $selected_id=null, $all_option=false, $submit_on_change=false, $editkey=false)
{
	if ($editkey)
		set_editor('item', $name, $editkey);
		
	echo "<td>";
	echo key_type_insurance_list($name, $selected_id, $all_option, $submit_on_change, array('cells'=>true));
	echo "</td>\n";
}
/*fap 11/12/2015*/

/*fap 11/12/2015*/
function key_type_insurance_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	key_type_insurance_list_cells($name, $selected_id, $all_option, $submit_on_change, false);
	echo "</tr>\n";
}
/*fap 11/12/2015*/

function sales_kits_list($name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	return sales_items_list($name, $selected_id, $all_option, $submit_on_change,
		'kits', array('cells'=>false, 'editable' => false));
}

function sales_local_items_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	echo "<tr>";
	if ($label != null)
		echo "<td class='label'>$label</td>\n";
	echo "<td>";
	echo sales_items_list($name, $selected_id, $all_option, $submit_on_change,
		'local', array('cells'=>false, 'editable' => false));
	echo "</td></tr>";
}
//------------------------------------------------------------------------------------

function stock_manufactured_items_list($name, $selected_id=null,
	$all_option=false, $submit_on_change=false)
{
	return stock_items_list($name, $selected_id, $all_option, $submit_on_change,
		array('where'=>array("mb_flag= 'M'")));
}

function stock_manufactured_items_list_cells($label, $name, $selected_id=null,
				$all_option=false, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo stock_manufactured_items_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n";
}

function stock_manufactured_items_list_row($label, $name, $selected_id=null,
		$all_option=false, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	stock_manufactured_items_list_cells(null, $name, $selected_id, $all_option, $submit_on_change);
	echo "</tr>\n";
}
//------------------------------------------------------------------------------------

function stock_component_items_list($name, $parent_stock_id, $selected_id=null,
	$all_option=false, $submit_on_change=false, $editkey = false)
{
	return stock_items_list($name, $selected_id, $all_option, $submit_on_change,
		array('where'=>array("stock_id != ".db_escape($parent_stock_id))), $editkey);
}

function stock_component_items_list_cells($label, $name, $parent_stock_id, 
	$selected_id=null, $all_option=false, $submit_on_change=false, $editkey = false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo stock_items_list($name, $selected_id, $all_option, $submit_on_change,
		array('where'=>array("stock_id != ".db_escape($parent_stock_id)), 'cells'=>true),
		$editkey);
}
//------------------------------------------------------------------------------------

function stock_costable_items_list($name, $selected_id=null,
	$all_option=false, $submit_on_change=false)
{
	return stock_items_list($name, $selected_id, $all_option, $submit_on_change,
		array('where'=>array("mb_flag!='D'")));
}

/*Begin Felix Alberti 19/07/2016*/
function stock_costable_items_category_list($name, $selected_id=null,
	$all_option=false, $submit_on_change=false, $editkey=false, $category=false)
{
	return stock_items_category_list($name, $selected_id, $all_option, $submit_on_change,
		array('where'=>array("mb_flag!='D'")), $editkey, $category);
}
/*End Felix Alberti 19/07/2016*/

function stock_costable_items_list_cells($label, $name, $selected_id=null, 
	$all_option=false, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo stock_items_list($name, $selected_id, $all_option, $submit_on_change,
		array('where'=>array("mb_flag!='D'"), 'cells'=>true));
}

/*Begin Felix Alberti 19/07/2016*/
function stock_costable_items_category_list_cells($label, $name, $selected_id=null, 
	$all_option=false, $submit_on_change=false, $editkey=false, $category=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo stock_items_category_list($name, $selected_id, $all_option, $submit_on_change,
		array('where'=>array("mb_flag!='D'"), 'cells'=>true), $editkey, $category);
}
/*End Felix Alberti 19/07/2016*/
//------------------------------------------------------------------------------------
function stock_purchasable_items_list($name, $selected_id=null,	
	$all_option=false, $submit_on_change=false, $all=false, $editkey=false)
{
	return stock_items_list($name, $selected_id, $all_option, $submit_on_change,
		array('where'=>array("mb_flag!= 'M'"), 
			'show_inactive'=>$all), $editkey);
}
//
//	This helper is used in PO/GRN/PI entry and supports editable descriptions.
//
function stock_purchasable_items_list_cells($label, $name, $selected_id=null,
			$all_option=false, $submit_on_change=false, $editkey=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo stock_items_list($name, $selected_id, $all_option, $submit_on_change,
		array('where'=>array("mb_flag!= 'M'"), 
			 'editable' => 30,
			 'cells'=>true), $editkey);
}
/*
function stock_purchasable_items_list_row($label, $name, $selected_id=null,
			$all_option=false, $submit_on_change=false, $editkey=false)
{
	echo "<tr><td class='label'>$label</td>";
	stock_purchasable_items_list_cells(null, $name, $selected_id=null,
		$all_option, $submit_on_change, $editkey);
	echo "</tr>\n";
}
*/
//------------------------------------------------------------------------------------

function stock_item_types_list_row($label, $name, $selected_id=null, $enabled=true)
{
	global $stock_types;

	echo "<tr>";
	if ($label != null)
		echo "<td class='label'>$label</td>\n";
	echo "<td>";

	echo array_selector($name, $selected_id, $stock_types, 
		array( 
			'select_submit'=> true, 
			'disabled' => !$enabled) );
	echo "</td></tr>\n";
}

function stock_units_list_row($label, $name, $value=null, $enabled=true)
{
	$result = get_all_item_units();
	echo "<tr>";
	if ($label != null)
		echo "<td class='label'>$label</td>\n";
	echo "<td>";

	while($unit = db_fetch($result))
		$units[$unit['abbr']] = $unit['name'];

	echo array_selector($name, $value, $units, array( 'disabled' => !$enabled) );

	echo "</td></tr>\n";
}

//------------------------------------------------------------------------------------

function tax_types_list($name, $selected_id=null, $none_option=false, $submit_on_change=false)
{
	$sql = "SELECT id, CONCAT(name, ' (',rate,'%)') as name FROM ".TB_PREF."tax_types";

	return combo_input($name, $selected_id, $sql, 'id', 'name',
	array(
		'spec_option' => $none_option,
		'spec_id' => ALL_NUMERIC,
		'select_submit'=> $submit_on_change,
		'async' => false,
	) );
}

function tax_types_list_cells($label, $name, $selected_id=null, $none_option=false,
	$submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo tax_types_list($name, $selected_id, $none_option, $submit_on_change);
	echo "</td>\n";
}

function tax_types_list_row($label, $name, $selected_id=null, $none_option=false,
	$submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	tax_types_list_cells(null, $name, $selected_id, $none_option, $submit_on_change);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------

function tax_groups_list($name, $selected_id=null,
	$none_option=false, $submit_on_change=false)
{
	$sql = "SELECT id, name FROM ".TB_PREF."tax_groups";

	return combo_input($name, $selected_id, $sql, 'id', 'name',
	array(
		'order' => 'id',
		'spec_option' => $none_option,
		'spec_id' => ALL_NUMERIC,
		'select_submit'=> $submit_on_change,
		'async' => false,
	) );
}

function tax_groups_list_cells($label, $name, $selected_id=null, $none_option=false, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo tax_groups_list($name, $selected_id, $none_option, $submit_on_change);
	echo "</td>\n";
}

function tax_groups_list_row($label, $name, $selected_id=null, $none_option=false, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	tax_groups_list_cells(null, $name, $selected_id, $none_option, $submit_on_change);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------

function item_tax_types_list($name, $selected_id=null)
{
	$sql ="SELECT id, name FROM ".TB_PREF."item_tax_types";
	return combo_input($name, $selected_id, $sql, 'id', 'name', array('order' => 'id') );
}

function item_tax_types_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo item_tax_types_list($name, $selected_id);
	echo "</td>\n";
}

function item_tax_types_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	item_tax_types_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------

function shippers_list($name, $selected_id=null)
{
	$sql = "SELECT shipper_id, shipper_name, inactive FROM ".TB_PREF."shippers";
	return combo_input($name, $selected_id, $sql, 'shipper_id', 'shipper_name', 
		array('order'=>array('shipper_name')));
}

function shippers_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo shippers_list($name, $selected_id);
	echo "</td>\n";
}

function shippers_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	shippers_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}

//-------------------------------------------------------------------------------------

function sales_persons_list($name, $selected_id=null, $spec_opt=false)
{
	$sql = "SELECT salesman_code, salesman_name, inactive FROM ".TB_PREF."salesman";
	return combo_input($name, $selected_id, $sql, 'salesman_code', 'salesman_name', 
		array('order'=>array('salesman_name'),
			'spec_option' => $spec_opt,
			'spec_id' => ALL_NUMERIC));
}

function sales_persons_list_cells($label, $name, $selected_id=null, $spec_opt=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>\n";
	echo sales_persons_list($name, $selected_id, $spec_opt);
	echo "</td>\n";
}

function sales_persons_list_row($label, $name, $selected_id=null, $spec_opt=false)
{
	echo "<tr><td class='label'>$label</td>";
	sales_persons_list_cells(null, $name, $selected_id, $spec_opt);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------

function sales_areas_list($name, $selected_id=null)
{
	$sql = "SELECT area_code, description, inactive FROM ".TB_PREF."areas";
	return combo_input($name, $selected_id, $sql, 'area_code', 'description', array());
}

function sales_areas_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo sales_areas_list($name, $selected_id);
	echo "</td>\n";
}

function sales_areas_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	sales_areas_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------

function sales_groups_list($name, $selected_id=null, $special_option=false)
{
	$sql = "SELECT id, description, inactive FROM ".TB_PREF."groups";
	return combo_input($name, $selected_id, $sql, 'id', 'description', array(
		'spec_option' => $special_option===true ? ' ' : $special_option,
		'order' => 'description', 'spec_id' => 0,
	));
}

function sales_groups_list_cells($label, $name, $selected_id=null, $special_option=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo sales_groups_list($name, $selected_id, $special_option);
	echo "</td>\n";
}

function sales_groups_list_row($label, $name, $selected_id=null, $special_option=false)
{
	echo "<tr><td class='label'>$label</td>";
	sales_groups_list_cells(null, $name, $selected_id, $special_option);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------

function _format_template_items($row)
{
	return ($row[0] . "&nbsp;- &nbsp;" . _("Amount") . "&nbsp;".$row[1]);
}

function templates_list($name, $selected_id=null, $special_option=false)
{
	$sql = "SELECT sorder.order_no,	Sum(line.unit_price*line.quantity*(1-line.discount_percent)) AS OrderValue
		FROM ".TB_PREF."sales_orders as sorder, ".TB_PREF."sales_order_details as line
		WHERE sorder.order_no = line.order_no AND sorder.type = 1 GROUP BY line.order_no";
	return combo_input($name, $selected_id, $sql, 'order_no', 'OrderValue', array(
		'format' => '_format_template_items',
		'spec_option' => $special_option===true ? ' ' : $special_option,
		'order' => 'order_no', 'spec_id' => 0,
	));
}

function templates_list_cells($label, $name, $selected_id=null, $special_option=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo templates_list($name, $selected_id, $special_option);
	echo "</td>\n";
}

function templates_list_row($label, $name, $selected_id=null, $special_option=false)
{
	echo "<tr><td class='label'>$label</td>";
	templates_list_cells(null, $name, $selected_id, $special_option);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------

function workorders_list($name, $selected_id=null)
{
	$sql = "SELECT id, wo_ref FROM ".TB_PREF."workorders WHERE closed=0";
	return combo_input($name, $selected_id, $sql, 'id', 'wo_ref', array());
}

function workorders_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo workorders_list($name, $selected_id);
	echo "</td>\n";
}

function workorders_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	workorders_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------

function payment_terms_list($name, $selected_id=null)
{
	$sql = "SELECT terms_indicator, terms, inactive FROM ".TB_PREF."payment_terms";
	return combo_input($name, $selected_id, $sql, 'terms_indicator', 'terms', array());
}

function payment_terms_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo payment_terms_list($name, $selected_id);
	echo "</td>\n";
}

function payment_terms_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	payment_terms_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------

function credit_status_list($name, $selected_id=null)
{
	$sql ="SELECT id, reason_description, inactive FROM ".TB_PREF."credit_status";
	return combo_input($name, $selected_id, $sql, 'id', 'reason_description', array());
}

function credit_status_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo credit_status_list($name, $selected_id);
	echo "</td>\n";
}

function credit_status_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	credit_status_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}

//-----------------------------------------------------------------------------------------------

function sales_types_list($name, $selected_id=null, $submit_on_change=false, $special_option=false)
{
	$sql = "SELECT id, sales_type, inactive FROM ".TB_PREF."sales_types";

	return combo_input($name, $selected_id, $sql, 'id', 'sales_type',
	array(
		'spec_option' => $special_option===true ? _("All Sales Types") : $special_option,
		'spec_id' => 0,
		'select_submit'=> $submit_on_change,
	//	  'async' => false,
	) );
}

function sales_types_list_cells($label, $name, $selected_id=null, $submit_on_change=false, $special_option=false, $params="")
{
	if ($label != null){
            
            if ($params != "")
            {
                    echo "<td $params>$label</td>";
            }
            else    
                    echo "<td>$label</td>\n";
        }
	echo "<td>";
	echo sales_types_list($name, $selected_id, $submit_on_change, $special_option);
	echo "</td>\n";
}

function sales_types_list_row($label, $name, $selected_id=null, $submit_on_change=false, $special_option=false, $params="")
{
	echo "<tr><td class='label'>$label</td>";
	sales_types_list_cells(null, $name, $selected_id, $submit_on_change, $special_option, $params);
	echo "</tr>\n";
}

/*Creada por Felix Alberti 27/11/2015*/
function medical_speciality_list($name, $selected_id=null, $submit_on_change=false, $special_option=false)
{
	$sql = "SELECT id, name, inactive FROM ".TB_PREF."medical_speciality";

	return combo_input($name, $selected_id, $sql, 'id', 'name',
	array(
		'spec_option' => $special_option===true ? _("All Medical Speciality") : $special_option,
		'spec_id' => 0,
		'select_submit'=> $submit_on_change,
	//	  'async' => false,
	) );
}
/*End Creada por Felix Alberti 27/11/2015*/

/*Creada por Felix Alberti 27/11/2015*/
function medical_speciality_list_cells($label, $name, $selected_id=null, $submit_on_change=false, $special_option=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo medical_speciality_list($name, $selected_id, $submit_on_change, $special_option);
	echo "</td>\n";
}
/*End Creada por Felix Alberti 27/11/2015*/

/*Creada por Felix Alberti 27/11/2015*/
function medical_speciality_list_row($label, $name, $selected_id=null, $submit_on_change=false, $special_option=false)
{
	echo "<tr><td class='label'>$label</td>";
	medical_speciality_list_cells(null, $name, $selected_id, $submit_on_change, $special_option);
	echo "</tr>\n";
}
/*End Creada por Felix Alberti 27/11/2015*/

/*Creada por Felix Alberti 27/11/2015*/
function medico_evaluacion_list($name, $selected_id=null, $special_option=true)
{
       global $all_items;
	$sql ="SELECT professional_id, description, inactive FROM ".TB_PREF."professional where type_professional_id = 1";
	return combo_input($name, $selected_id, $sql, 'id', 'description', 
                array('spec_option' => $special_option===true ? _("-") : $special_option,
		      'spec_id' => $all_items)
                );     
        
}
/*End Creada por Felix Alberti 27/11/2015*/

/*Begin Creada por Felix Alberti 27/11/2015*/
function medico_evaluacion_list_cells2($label, $name, $selected_id=null){	        
       
	echo "<td>";
	echo medico_evaluacion_list($name, $selected_id);
	echo "</td>\n";
        
}
/*End Creada por Felix Alberti 27/11/2015*/

/*Begin Creada por Felix Alberti 27/11/2015*/
function medico_evaluacion_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
        medico_evaluacion_list_cells2($label, $name, $selected_id);
	echo "</tr>\n";
}
/*End Creada por Felix Alberti 27/11/2015*/

//-----------------------------------------------------------------------------------------------


/*Creada por Felix Alberti 15/03/2016*/
function medico_tratante_list($name, $selected_id=null)
{
	$sql ="SELECT professional_id, description, inactive FROM ".TB_PREF."professional where type_professional_id = 1";
	return combo_input($name, $selected_id, $sql, 'id', 'description', array());
}
/*End Creada por Felix Alberti 15/03/2016*/

/*Begin Creada por Felix Alberti 15/03/2016*/
function medico_tratante_list_cells2($label, $name, $selected_id=null){	        
       
	echo "<td>";
	echo medico_tratante_list($name, $selected_id);
	echo "</td>\n";
        
}
/*End Creada por Felix Alberti 15/03/2016*/

/*Begin Creada por Felix Alberti 15/03/2016*/
function medico_tratante_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
        medico_tratante_list_cells2($label, $name, $selected_id);
	echo "</tr>\n";
}
/*End Creada por Felix Alberti 15/03/2016*/

/*Creada por Felix Alberti 15/03/2016*/
function medico_a_cargo_list($name, $selected_id=null)
{
	$sql ="SELECT professional_id, description, inactive FROM ".TB_PREF."professional where type_professional_id = 1";
	return combo_input($name, $selected_id, $sql, 'id', 'description', array());
}
/*End Creada por Felix Alberti 15/03/2016*/

/*Begin Creada por Felix Alberti 15/03/2016*/
function medico_a_cargo_list_cells2($label, $name, $selected_id=null){	        
       
	echo "<td>";
	echo medico_a_cargo_list($name, $selected_id);
	echo "</td>\n";
        
}

/*Begin Creada por Felix Alberti 15/03/2016*/
function medico_a_cargo_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
        medico_a_cargo_list_cells2($label, $name, $selected_id);
	echo "</tr>\n";
}
/*End Creada por Felix Alberti 15/03/2016*/





//-----------------------------------------------------------------------------------------------

function movement_types_list($name, $selected_id=null)
{
	$sql = "SELECT id, name FROM ".TB_PREF."movement_types";
	return combo_input($name, $selected_id, $sql, 'id', 'name', array());
}

function movement_types_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo movement_types_list($name, $selected_id);
	echo "</td>\n";
}

function movement_types_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	movement_types_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}

//-----------------------------------------------------------------------------------------------
function _format_date($row)
{
	return sql2date($row['reconciled']);
}

function bank_reconciliation_list($account, $name, $selected_id=null, $submit_on_change=false, $special_option=false)
{
	$sql = "SELECT reconciled, reconciled FROM ".TB_PREF."bank_trans
		WHERE bank_act=".db_escape($account)." AND reconciled IS NOT NULL
		GROUP BY reconciled";
	return combo_input($name, $selected_id, $sql, 'id', 'reconciled',
	array(
		'spec_option' => $special_option,
		'format' => '_format_date',
		'spec_id' => '',
		'select_submit'=> $submit_on_change
	) );
}

function bank_reconciliation_list_cells($label,$account, $name, $selected_id=null, $submit_on_change=false, $special_option=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo bank_reconciliation_list($account, $name, $selected_id, $submit_on_change, $special_option);
	echo "</td>\n";
}
/*
function bank_reconciliation_list_row($label, $account, $name, $selected_id=null, $submit_on_change=false, $special_option=false)
{
	echo "<tr>\n";
	bank_reconciliation_list_cells($label, $account, $name, $selected_id, $submit_on_change, $special_option);
	echo "</tr>\n";
}
*/
//-----------------------------------------------------------------------------------------------

function workcenter_list($name, $selected_id=null, $all_option=false)
{
	global $all_items;

	$sql = "SELECT id, name, inactive FROM ".TB_PREF."workcentres";

	return combo_input($name, $selected_id, $sql, 'id', 'name',
	array(
		'spec_option' =>$all_option===true ? _("All Suppliers") : $all_option,
		'spec_id' => $all_items,
	) );
}

function workcenter_list_cells($label, $name, $selected_id=null, $all_option=false)
{
	default_focus($name);
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo workcenter_list($name, $selected_id, $all_option);
	echo "</td>\n";
}

function workcenter_list_row($label, $name, $selected_id=null, $all_option=false)
{
	echo "<tr><td class='label'>$label</td>";
	workcenter_list_cells(null, $name, $selected_id, $all_option);
	echo "</tr>\n";
}

//-----------------------------------------------------------------------------------------------

function bank_accounts_list($name, $selected_id=null, $submit_on_change=false, $spec_option=false)
{
	$sql = "SELECT ".TB_PREF."bank_accounts.id, bank_account_name, bank_curr_code, inactive
		FROM ".TB_PREF."bank_accounts";

	return combo_input($name, $selected_id, $sql, 'id', 'bank_account_name',
	array(
		'format' => '_format_add_curr',
		'select_submit'=> $submit_on_change,
		'spec_option' => $spec_option,
		'spec_id' => '',
		'async' => false
	) );
}

function bank_accounts_list_cells($label, $name, $selected_id=null, $submit_on_change=false, $spec_option=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo bank_accounts_list($name, $selected_id, $submit_on_change, $spec_option);
	echo "</td>\n";
}

function bank_accounts_list_row($label, $name, $selected_id=null, $submit_on_change=false, $spec_option=false)
{
	echo "<tr><td class='label'>$label</td>";
	bank_accounts_list_cells(null, $name, $selected_id, $submit_on_change, $spec_option);
	echo "</tr>\n";
}
//-----------------------------------------------------------------------------------------------
function bank_accounts_label_list_cells($label, $name, $selected_id=null, $submit_on_change=false)
{
	if ($label != null)
		echo "<td class='label'>$label</td>\n";
	echo "<td>";
	echo bank_accounts_list($name, $selected_id, $submit_on_change);
	echo "</td>\n";
}
//-----------------------------------------------------------------------------------------------

function cash_accounts_list_row($label, $name, $selected_id=null, $submit_on_change=false)
{
	$sql = "SELECT ".TB_PREF."bank_accounts.id, bank_account_name, bank_curr_code, inactive
		FROM ".TB_PREF."bank_accounts
		WHERE ".TB_PREF."bank_accounts.account_type=".BT_CASH;

	if ($label != null)
		echo "<tr><td class='label'>$label</td>\n";
	echo "<td>";
	echo combo_input($name, $selected_id, $sql, 'id', 'bank_account_name',
	array(
		'format' => '_format_add_curr',
		'select_submit'=> $submit_on_change,
		'async' => true
	) );
	echo "</td></tr>\n";
}
//-----------------------------------------------------------------------------------------------

function pos_list_row($label, $name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	$sql = "SELECT id, pos_name, inactive FROM ".TB_PREF."sales_pos";

	default_focus($name);
	echo '<tr>';
	if ($label != null)
		echo "<td class='label'>$label</td>\n";
	echo "<td>";

	echo combo_input($name, $selected_id, $sql, 'id', 'pos_name',
	array(
		'select_submit'=> $submit_on_change,
		'async' => true,
		'spec_option' =>$spec_option,
		'spec_id' => -1,
		'order'=> array('pos_name')
	) );
	echo "</td></tr>\n";

}
//-----------------------------------------------------------------------------------------------
// Payment type selector for current user.
//
function sale_payment_list($name, $category, $selected_id=null, $submit_on_change=true)
{
	$sql = "SELECT terms_indicator, terms, inactive FROM ".TB_PREF."payment_terms";
	
	if ($category == PM_CASH) // only cash
			$sql .= " WHERE days_before_due=0 AND day_in_following_month=0";
	if ($category == PM_CREDIT) // only delayed payments
			$sql .= " WHERE days_before_due!=0 OR day_in_following_month!=0";

	return combo_input($name, $selected_id, $sql, 'terms_indicator', 'terms',
	array(
		'select_submit'=> $submit_on_change,
		'async' => true
	) );

}

function sale_payment_list_cells($label, $name, $category, $selected_id=null, $submit_on_change=true)
{
	if ($label != null)
		echo "<td class='label'>$label</td>\n";
	echo "<td>";

	echo sale_payment_list($name, $category, $selected_id, $submit_on_change);

	echo "</td>\n";
}
//-----------------------------------------------------------------------------------------------

function class_list($name, $selected_id=null, $submit_on_change=false)
{
	$sql = "SELECT cid, class_name FROM ".TB_PREF."chart_class";

	return combo_input($name, $selected_id, $sql, 'cid', 'class_name',
	array(
		'select_submit'=> $submit_on_change,
		'async' => false
	) );

}

function class_list_cells($label, $name, $selected_id=null, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo class_list($name, $selected_id, $submit_on_change);
	echo "</td>\n";
}

function class_list_row($label, $name, $selected_id=null, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	class_list_cells(null, $name, $selected_id, $submit_on_change);
	echo "</tr>\n";
}

//-----------------------------------------------------------------------------------------------
function stock_categories_list($name, $selected_id=null, $spec_opt=false, $submit_on_change=false, $order=false)
{
        $order = ($order == false) ? 'category_id' : $order ;
	$sql = "SELECT category_id, description, inactive FROM ".TB_PREF."stock_category";
	return combo_input($name, $selected_id, $sql, 'category_id', 'description',
 	array('order'=>$order,
		'spec_option' => $spec_opt,
		'spec_id' => -1,
 		'select_submit'=> $submit_on_change,
 		'async' => true
 	));
}

/*Begin Felix Alberti 29/07/2016*/
function stock_categories_dimension_list($name, $selected_id=null, $spec_opt=false, $submit_on_change=false, $order=false, $dimension=false)
{
        $order = ($order == false) ? 'category_id' : $order;
	$sql = "SELECT category_id, description, inactive FROM ".TB_PREF."stock_category";
        //if ($dimension!=false) $sql .= " WHERE dimension_id = ".db_escape($dimension);
        //display_notification($sql);
	return combo_input($name, $selected_id, $sql, 'category_id', 'description',
 	array('order'=>$order,
		'spec_option' => $spec_opt,
		'spec_id' => -1,
 		'select_submit'=> $submit_on_change,
 		'async' => true
 	));
}
/*End Felix Alberti 29/07/2016*/

function stock_categories_list_cells($label, $name, $selected_id=null, $spec_opt=false, $submit_on_change=false, $order=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo stock_categories_list($name, $selected_id, $spec_opt, $submit_on_change, $order);
	echo "</td>\n";
}

/*Begin Felix Alberti 29/07/2016*/
function stock_categories_dimension_list_cells($label, $name, $selected_id=null, $spec_opt=false, $submit_on_change=false, $order=false, $dimension=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo stock_categories_dimension_list($name, $selected_id, $spec_opt, $submit_on_change, $order, $dimension);
	echo "</td>\n";
}
/*End Felix Alberti 29/07/2016*/

function stock_categories_list_row($label, $name, $selected_id=null, $spec_opt=false, $submit_on_change=false, $order=false)
{
	echo "<tr><td class='label'>$label</td>";
	stock_categories_list_cells(null, $name, $selected_id, $spec_opt, $submit_on_change, $order);
	echo "</tr>\n";
}

//-----------------------------------------------------------------------------------------------

function gl_account_types_list($name, $selected_id=null, $all_option=false, $all=true)
{
	global $all_items;

	$sql = "SELECT id, name FROM ".TB_PREF."chart_types";

	return combo_input($name, $selected_id, $sql, 'id', 'name',
	array(
		'format' => '_format_account',
		'order' => array('class_id', 'id', 'parent'),
		'spec_option' =>$all_option,
		'spec_id' => $all_items
	) );
}

function gl_account_types_list_cells($label, $name, $selected_id=null, $all_option=false, $all=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo gl_account_types_list($name, $selected_id, $all_option, $all);
	echo "</td>\n";
}

function gl_account_types_list_row($label, $name, $selected_id=null, $all_option=false,	$all=false)
{
	echo "<tr><td class='label'>$label</td>";
	gl_account_types_list_cells(null, $name, $selected_id, $all_option,
		$all);
	echo "</tr>\n";
}

//-----------------------------------------------------------------------------------------------
function gl_all_accounts_list($name, $selected_id=null, $skip_bank_accounts=false,
	$cells=false, $all_option=false, $submit_on_change=false, $all=false)
{
	if ($skip_bank_accounts)
		$sql = "SELECT chart.account_code, chart.account_name, type.name, chart.inactive, type.id
			FROM (".TB_PREF."chart_master chart,".TB_PREF."chart_types type) "
			."LEFT JOIN ".TB_PREF."bank_accounts acc "
			."ON chart.account_code=acc.account_code
				WHERE acc.account_code  IS NULL
			AND chart.account_type=type.id";
	else
		$sql = "SELECT chart.account_code, chart.account_name, type.name, chart.inactive, type.id
			FROM ".TB_PREF."chart_master chart,".TB_PREF."chart_types type
			WHERE chart.account_type=type.id";

	return combo_input($name, $selected_id, $sql, 'chart.account_code', 'chart.account_name',
	array(
		'format' => '_format_account',
		'spec_option' => $all_option===true ?  _("Use Item Sales Accounts") : $all_option,
		'spec_id' => '',
		'type' => 2,
		'order' => array('type.class_id','type.id','account_code'),
		'search_box' => $cells,
			'search_submit' => false,
			'size' => 12,
			'max' => 10,
			'cells' => true,
		'select_submit'=> $submit_on_change,
		'async' => false,
		'category' => 2,
		'show_inactive' => $all
	) );

}

function _format_account($row)
{
		return $row[0] .  "&nbsp;&nbsp;&nbsp;&nbsp;" . $row[1];
}

function gl_all_accounts_list_cells($label, $name, $selected_id=null, 
	$skip_bank_accounts=false, $cells=false, $all_option=false, 
	$submit_on_change=false, $all=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo gl_all_accounts_list($name, $selected_id, 
		$skip_bank_accounts, $cells, $all_option, $submit_on_change, $all);
	echo "</td>\n";
}

function gl_all_accounts_list_row($label, $name, $selected_id=null, 
	$skip_bank_accounts=false, $cells=false, $all_option=false)
{
	echo "<tr><td class='label'>$label</td>";
	gl_all_accounts_list_cells(null, $name, $selected_id, 
		$skip_bank_accounts, $cells, $all_option);
	echo "</tr>\n";
}

function yesno_list($name, $selected_id=null, $name_yes="", $name_no="", $submit_on_change=false)
{
	$items = array();
	$items['0'] = strlen($name_no) ? $name_no : _("No");
	$items['1'] = strlen($name_yes) ? $name_yes : _("Yes");

	return array_selector($name, $selected_id, $items, 
		array( 
			'select_submit'=> $submit_on_change,
			'async' => false ) ); // FIX?
}

function yesno_list_cells($label, $name, $selected_id=null, $name_yes="", $name_no="", $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo yesno_list($name, $selected_id, $name_yes, $name_no, $submit_on_change);
	echo "</td>\n";
}

function yesno_list_row($label, $name, $selected_id=null, $name_yes="", $name_no="", $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	yesno_list_cells(null, $name, $selected_id, $name_yes, $name_no, $submit_on_change);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------------------

function languages_list($name, $selected_id=null, $all_option=false)
{
	global $installed_languages;

	$items = array();
	if ($all_option)
		$items[''] = $all_option;
	foreach ($installed_languages as $lang)
			$items[$lang['code']] = $lang['name'];
	return array_selector($name, $selected_id, $items);
}

function languages_list_cells($label, $name, $selected_id=null, $all_option=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo languages_list($name, $selected_id, $all_option);
	echo "</td>\n";
}

function languages_list_row($label, $name, $selected_id=null, $all_option=false)
{
	echo "<tr><td class='label'>$label</td>";
	languages_list_cells(null, $name, $selected_id, $all_option);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------------------

function bank_account_types_list($name, $selected_id=null)
{
	global $bank_account_types;

	return array_selector($name, $selected_id, $bank_account_types);
}

function bank_account_types_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo bank_account_types_list($name, $selected_id);
	echo "</td>\n";
}

function bank_account_types_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	bank_account_types_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------------------
function payment_person_types_list($name, $selected_id=null, $submit_on_change=false)
{
	global $payment_person_types;

	$items = array();
	foreach ($payment_person_types as $key=>$type)
	{
		if ($key != PT_WORKORDER)
			$items[$key] = $type;
	}		
	return array_selector($name, $selected_id, $items, 
		array( 'select_submit'=> $submit_on_change ) );
}

function payment_person_types_list_cells($label, $name, $selected_id=null, $related=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo payment_person_types_list($name, $selected_id, $related);
	echo "</td>\n";
}

function payment_person_types_list_row($label, $name, $selected_id=null, $related=null)
{
	echo "<tr><td class='label'>$label</td>";
	payment_person_types_list_cells(null, $name, $selected_id, $related);
	echo "</tr>\n";
}

//------------------------------------------------------------------------------------------------

function wo_types_list($name, $selected_id=null)
{
	global $wo_types_array;
	
	return array_selector($name, $selected_id, $wo_types_array, 
		array( 'select_submit'=> true, 'async' => true ) );
}

function wo_types_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td><td>\n";
	echo wo_types_list($name, $selected_id);
	echo "</td></tr>\n";
}

//------------------------------------------------------------------------------------------------

function dateformats_list_row($label, $name, $value=null)
{
	global $dateformats;

	echo "<tr><td class='label'>$label</td>\n<td>";
	echo array_selector( $name, $value, $dateformats );
	echo "</td></tr>\n";
}

function dateseps_list_row($label, $name, $value=null)
{
	global $dateseps;

	echo "<tr><td class='label'>$label</td>\n<td>";
	echo array_selector( $name, $value, $dateseps );
	echo "</td></tr>\n";
}

function thoseps_list_row($label, $name, $value=null)
{
	global $thoseps;

	echo "<tr><td class='label'>$label</td>\n<td>";
	echo array_selector( $name, $value, $thoseps );
	echo "</td></tr>\n";
}

function decseps_list_row($label, $name, $value=null)
{
	global $decseps;

	echo "<tr><td class='label'>$label</td>\n<td>";
	echo array_selector( $name, $value, $decseps );
	echo "</td></tr>\n";
}

function themes_list_row($label, $name, $value=null)
{
	global $path_to_root;

	$path = $path_to_root.'/themes/';
	$themes = array();
	$themedir = opendir($path);
	while(false !== ($fname = readdir($themedir)))
	{
		if($fname!='.' && $fname!='..' && $fname!='CVS' && is_dir($path.$fname))
		{
			$themes[$fname] =  $fname;
		}
	}
	ksort($themes);

	echo "<tr><td class='label'>$label</td>\n<td>";
	echo array_selector( $name, $value, $themes );
	echo "</td></tr>\n";
}

function pagesizes_list_row($label, $name, $value=null)
{
	global $pagesizes;

	$items = array();
	foreach ($pagesizes as $pz)
		$items[$pz] = $pz;

	echo "<tr><td class='label'>$label</td>\n<td>";
	echo array_selector( $name, $value, $items );
	echo "</td></tr>\n";
}

function systypes_list($name, $value=null, $spec_opt=false, $submit_on_change=false, $exclude=array())
{
	global $systypes_array;

	// emove non-voidable transactions if needed
	$systypes = array_diff_key($systypes_array, array_flip($exclude));
	return array_selector($name, $value, $systypes, 
		array( 
			'spec_option'=> $spec_opt,
			'spec_id' => ALL_NUMERIC,
			'select_submit'=> $submit_on_change,
			'async' => false,
			)
	);
}

function systypes_list_cells($label, $name, $value=null, $submit_on_change=false, $exclude=array(), $spec_option=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo systypes_list($name, $value, $spec_option, $submit_on_change, $exclude);
	echo "</td>\n";
}

function systypes_list_row($label, $name, $value=null, $submit_on_change=false, $exclude=array())
{
	echo "<tr><td class='label'>$label</td>";
	systypes_list_cells(null, $name, $value, $submit_on_change, $exclude);
	echo "</tr>\n";
}

function journal_types_list_cells($label, $name, $value=null, $submit_on_change=false)
{
	global $systypes_array;

	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";

	$items = $systypes_array;

	// exclude quotes, orders and dimensions
	foreach (array(ST_PURCHORDER, ST_SALESORDER, ST_DIMENSION, ST_SALESQUOTE, ST_LOCTRANSFER) as $excl)
			unset($items[$excl]);
	
	echo array_selector($name, $value, $items, 
		array( 
			'spec_option'=> _("All"),
			'spec_id' => -1,
			'select_submit'=> $submit_on_change,
			'async' => false
			)
	);
	echo "</td>\n";
}

function cust_allocations_list_cells($label, $name, $selected=null)
{
	global $all_items;

	if ($label != null)
		label_cell($label);
	echo "<td>\n";
	$allocs = array( 
		$all_items=>_("All Types"),
		'1'=> _("Sales Invoices"),
		'2'=> _("Overdue Invoices"),
		'3' => _("Payments"),
		'4' => _("Credit Notes"),
		'5' => _("Delivery Notes"),
                '6' => _("Bank Deposit"),
                '7' => _("Debit Notes")
	);
	echo array_selector($name, $selected, $allocs);
	echo "</td>\n";
}

//Begin Felix Alberti 07/07/2015
function medico_allocations_list_cells($label, $name, $selected=null)
{
	global $all_items;

	if ($label != null)
		label_cell($label);
	echo "<td>\n";
	$allocs = array( 
		$all_items=>_("All Types"),
		'1'=> _("Facturas Medicos"),
		'2'=> _("Facturas Vencidas Medicos"),
		'3' => _("Pagos Medicos"),
		'4' => _("NC Medicos"),
		'5' => _("Nota de Entrega Medicos")
	);
	echo array_selector($name, $selected, $allocs);
	echo "</td>\n";
	//return $str;
}
//End Felix Alberti 07/07/2015

//Begin Felix Alberti 02/12/2015
function documentos_cuenta_list_cells($label, $name, $selected=null)
{
	global $all_items;

	if ($label != null)
		label_cell($label);
	echo "<td>\n";
	$allocs = array( 
		$all_items=>_("All Types"),
		'1'=> _("Carta Aval"),
		'2'=> _("Carta de Compromiso"),
		'3' => _("Presupuesto"),
		'4' => _("NC Seguro")
	);
	echo array_selector($name, $selected, $allocs);
	echo "</td>\n";
	//return $str;
}
//End Felix Alberti 02/12/2015

function supp_allocations_list_cell($name, $selected=null)
{
	global $all_items;

	echo "<td>\n";
	$allocs = array( 
		$all_items=>_("All Types"),
		'1'=> _("Invoices"),
		'2'=> _("Overdue Invoices"),
		'3' => _("Payments"),
		'4' => _("Credit Notes"),
		'5' => _("Overdue Credit Notes")
	);
	echo array_selector($name, $selected, $allocs);
	echo "</td>\n";
}

function supp_transactions_list_cell($name, $selected=null)
{
	global $all_items;

	echo "<td>\n";
	$allocs = array( 
		$all_items=>_("All Types"),
		'6'=>_("GRNs"),
		'1'=> _("Invoices"),
		'2'=> _("Overdue Invoices"),
		'3' => _("Payments"),
		'4' => _("Credit Notes"),
		'5' => _("Overdue Credit Notes")
	);

	echo array_selector($name, $selected, $allocs);
	echo "</td>\n";
}

function policy_list_cells($label, $name, $selected=null)
{
	if ($label != null)
		label_cell($label);
	echo "<td>\n";
	echo array_selector($name, $selected, 
				array( '' => _("Automatically put balance on back order"),
			 		'CAN' => _("Cancel any quantites not delivered")) );
	echo "</td>\n";
}

function policy_list_row($label, $name, $selected=null)
{
	echo "<tr><td class='label'>$label</td>";
	policy_list_cells(null, $name, $selected);
	echo "</tr>\n";
}

function credit_type_list_cells($label, $name, $selected=null, $submit_on_change=false)
{
	if ($label != null)
		label_cell($label);
	echo "<td>\n";
	echo array_selector($name, $selected, 
				array( 'Return' => _("Items Returned to Inventory Location"),
			 		'WriteOff' => _("Items Written Off")),
				array( 'select_submit'=> $submit_on_change ) );
	echo "</td>\n";
}

function credit_type_list_row($label, $name, $selected=null, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	credit_type_list_cells(null, $name, $selected, $submit_on_change);
	echo "</tr>\n";
}

function number_list($name, $selected, $from, $to, $no_option=false)
{
	$items = array();
	for ($i = $from; $i <= $to; $i++)
		$items[$i] = "$i";

	return array_selector($name, $selected, $items,
				array(	'spec_option' => $no_option,
						'spec_id' => ALL_NUMERIC) );
}

function number_list_cells($label, $name, $selected, $from, $to, $no_option=false)
{
	if ($label != null)
		label_cell($label);
	echo "<td>\n";
	echo number_list($name, $selected, $from, $to, $no_option);
	echo "</td>\n";
}

function number_list_row($label, $name, $selected, $from, $to, $no_option=false)
{
	echo "<tr><td class='label'>$label</td>";
	echo number_list_cells(null, $name, $selected, $from, $to, $no_option);
	echo "</tr>\n";
}

function print_profiles_list_row($label, $name, $selected_id=null, $spec_opt=false,
	$submit_on_change=true)
{
	$sql = "SELECT profile FROM ".TB_PREF."print_profiles"
		." GROUP BY profile";
	$result = db_query($sql, 'cannot get all profile names');
	$profiles = array();
	while($myrow=db_fetch($result)) {
		$profiles[$myrow['profile']] = $myrow['profile'];
	}

	echo "<tr>";
	if ($label != null)
		echo "<td class='label'>$label</td>\n";
	echo "<td>";

	echo array_selector($name, $selected_id, $profiles, 
		array( 'select_submit'=> $submit_on_change,
			'spec_option'=>$spec_opt,
			'spec_id' => ''
		 ));

	echo "</td></tr>\n";
}

function printers_list($name, $selected_id=null, $spec_opt=false, $submit_on_change=false)
{
	static $printers; // query only once for page display

	if (!$printers) {
		$sql = "SELECT id, name, description FROM ".TB_PREF."printers";	
		$result = db_query($sql, 'cannot get all printers');
		$printers = array();
		while($myrow=db_fetch($result)) {
			$printers[$myrow['id']] = $myrow['name'].'&nbsp;-&nbsp;'.$myrow['description'];
		}
	}
	return array_selector($name, $selected_id, $printers, 
		array( 'select_submit'=> $submit_on_change,
			'spec_option'=>$spec_opt,
			'spec_id' => ''
		 ));
}

//------------------------------------------------------------------------------------------------

function quick_entries_list($name, $selected_id=null, $type=null, $submit_on_change=false)
{
	$where = false;
	$sql = "SELECT id, description FROM ".TB_PREF."quick_entries";
	if ($type != null)
		$sql .= " WHERE type=$type";

	return combo_input($name, $selected_id, $sql, 'id', 'description',
		array(
			'spec_id' => '',
			'order' => 'description',
			'select_submit'=> $submit_on_change,
			'async' => false
		) );

}

function quick_entries_list_cells($label, $name, $selected_id=null, $type, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";	
	echo quick_entries_list($name, $selected_id, $type, $submit_on_change);
	echo "</td>";
}

function quick_entries_list_row($label, $name, $selected_id=null, $type, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	quick_entries_list_cells(null, $name, $selected_id, $type, $submit_on_change);
	echo "</tr>\n";
}


function quick_actions_list_row($label, $name, $selected_id=null, $submit_on_change=false)
{
	global $quick_actions;
	
	echo "<tr><td class='label'>$label</td><td>";
	echo array_selector($name, $selected_id, $quick_actions, 
		array( 
			'select_submit'=> $submit_on_change
		) );
	echo "</td></tr>\n";
}

function quick_entry_types_list_row($label, $name, $selected_id=null, $submit_on_change=false)
{
	global $quick_entry_types;
		
	echo "<tr><td class='label'>$label</td><td>";
	echo array_selector($name, $selected_id, $quick_entry_types, 
		array( 
			'select_submit'=> $submit_on_change
			) );
	echo "</td></tr>\n";
}

function record_status_list_row($label, $name) {
	return yesno_list_row($label, $name, null, 	_('Inactive'), _('Active'));
}

function class_types_list_row($label, $name, $selected_id=null, $submit_on_change=false)
{
	global $class_types;
	
	echo "<tr><td class='label'>$label</td><td>";
	echo array_selector($name, $selected_id, $class_types, 
		array( 
			'select_submit'=> $submit_on_change
		) );
	echo "</td></tr>\n";
}

//------------------------------------------------------------------------------------------------

function security_roles_list($name, $selected_id=null, $new_item=false, $submit_on_change=false,
	$show_inactive = false)
{
	global $all_items;

	$sql = "SELECT id, role, inactive FROM ".TB_PREF."security_roles";

return combo_input($name, $selected_id, $sql, 'id', 'description',
	array(
		'spec_option'=>$new_item ? _("New role") : false,
		'spec_id' => '',
		'select_submit'=> $submit_on_change,
		'show_inactive' => $show_inactive
	) );
}

function security_roles_list_cells($label, $name, $selected_id=null, $new_item=false, $submit_on_change=false,
	$show_inactive = false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo security_roles_list($name, $selected_id, $new_item, $submit_on_change, $show_inactive);
	echo "</td>\n";
}

function security_roles_list_row($label, $name, $selected_id=null, $new_item=false, $submit_on_change=false,
	$show_inactive = false)
{
	echo "<tr><td class='label'>$label</td>";
	security_roles_list_cells(null, $name, $selected_id, $new_item, $submit_on_change, $show_inactive);
	echo "</tr>\n";
}

function tab_list_row($label, $name, $selected_id=null)
{
	global $installed_extensions;
	
	$tabs = array();
	foreach ($_SESSION['App']->applications as $app) {
		$tabs[$app->id] = access_string($app->name, true);
	}
	echo "<tr>\n";
	echo "<td class='label'>$label</td><td>\n";
	echo array_selector($name, $selected_id, $tabs);
	echo "</td></tr>\n";
}

//-----------------------------------------------------------------------------------------------

function tag_list($name, $height, $type, $multi=false, $all=false, $spec_opt = false)
{
	// Get tags
	global $path_to_root;
	include_once($path_to_root . "/admin/db/tags_db.inc");
	$results = get_tags($type, $all);

	while ($tag = db_fetch($results))
		$tags[$tag['id']] = $tag['name'];
	
	if (!isset($tags)) {
		$tags[''] = $all ? _("No tags defined.") : _("No active tags defined.");
		$spec_opt = false;
	}
	return array_selector($name, null, $tags,
		array(
			'multi' => $multi,
			'height' => $height,
			'spec_option'=> $spec_opt,
			'spec_id' => -1,
		) );
}

function tag_list_cells($label, $name, $height, $type, $mult=false, $all=false, $spec_opt = false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>\n";
	echo tag_list($name, $height, $type, $mult, $all, $spec_opt);
	echo "</td>\n";
	
}

function tag_list_row($label, $name, $height, $type, $mult=false, $all=false, $spec_opt = false)
{
	echo "<tr><td class='label'>$label</td>";
	tag_list_cells(null, $name, $height, $type, $mult, $all, $spec_opt);
	echo "</tr>\n";	
}

//---------------------------------------------------------------------------------------------
//	List of sets of active extensions 
//
function extset_list($name, $value=null, $submit_on_change=false)
{
	global $db_connections;

	$items = array();
	foreach ($db_connections as $comp)
		$items[] = sprintf(_("Activated for '%s'"), $comp['name']);
	return array_selector( $name, $value, $items,
		array(
			'spec_option'=> _("Available and/or installed"),
			'spec_id' => -1,
			'select_submit'=> $submit_on_change,
			'async' => true
		));
}

function crm_category_types_list($name, $selected_id=null, $filter=array(), $submit_on_change=true)
{

	$sql = "SELECT id, name, type, inactive FROM ".TB_PREF."crm_categories";

	$multi = false;
	$groups = false;
	$where = array();
	if (@$filter['class']) {
		$where[] = 'type='.db_escape($filter['class']);
	} else
		$groups = 'type';
	if (@$filter['subclass']) $where[] = 'action='.db_escape($filter['subclass']);
	if (@$filter['entity']) $where[] = 'entity_id='.db_escape($filter['entity']);
	if (@$filter['multi']) { // contact category selector for person
		$multi = true;
	}

	return combo_input($name, $selected_id, $sql, 'id', 'name',
	 	array(
	 		'multi' => $multi,
	 		'height' => $multi ? 5:1,
			'category' => $groups,
	 		'select_submit'=> $submit_on_change,
 			'async' => true,
 			'where' => $where
	 	));
}

function crm_category_types_list_row($label, $name, $selected_id=null, $filter=array(), $submit_on_change=true)
{
	echo "<tr><td class='label'>$label</td><td>";
	echo crm_category_types_list($name, $selected_id, $filter, $submit_on_change);
	echo "</td></tr>\n";
}

function payment_type_list_row($label, $name, $selected_id=null, $submit_on_change=false)
{
	global $pterm_types;
	
	echo "<tr><td class='label'>$label</td><td>";
	echo array_selector($name, $selected_id, $pterm_types, 
		array( 
			'select_submit'=> $submit_on_change
		) );
	echo "</td></tr>\n";
}

function coa_list_row($label, $name, $value=null)
{
	global $path_to_root, $installed_extensions;

	$path = $path_to_root.'/sql/';
	$coas = array();
	$sqldir = opendir($path);
	while (false !== ($fname = readdir($sqldir)))
	{
		if (is_file($path.$fname) && substr($fname,-4)=='.sql' && @($fname[2] == '_'))
		{
			$ext = array_search_value($fname, $installed_extensions, 'sql');
			if ($ext!=null) {
				$descr = $ext['name'];
			} elseif ($fname == 'en_US-new.sql') { // two standard COAs
				$descr = _("Standard new company American COA (4 digit)");
			} elseif ($fname == 'en_US-demo.sql') {
				$descr = _("Standard American COA (4 digit) with demo data");
			} else
				$descr = $fname;

			$coas[$fname] =  $descr;
		}
	}
	ksort($coas);

	echo "<tr><td class='label'>$label</td>\n<td>";
	echo array_selector( $name, $value, $coas );
	echo "</td></tr>\n";
}

function payment_services($name)
{
	global $payment_services;

	$services = array_combine(array_keys($payment_services), array_keys($payment_services));

	return array_selector($name, null, $services, array(
			'spec_option'=> _("No payment Link"),
			'spec_id' => '',
		));
}

/*Begin Felix Alberti 12/08/2015*/
function insurance_company_list($name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT id, name, inactive FROM ".TB_PREF."insurance_company";

return combo_input($name, $selected_id, $sql, 'id', 'name',
	array(
		'spec_option' => $all_option === true ? _("All Insurance") : $all_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change
	) );
}

function insurance_company_cells($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo insurance_company_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n";
}

function insurance_company_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	insurance_company_cells(null, $name, $selected_id, $all_option, $submit_on_change);
	echo "</tr>\n";
}
/*End Felix Alberti 12/08/2015*/
/*****************************/

/*Begin Felix Alberti 06/01/2016*/
function account_patient_allocate_list($name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer=null)
{
	global $all_items;
        //display_notification('account_patient_list'.' - '.$id_customer);
        $sql = "SELECT pa.id, concat(pa.id,'-',name) as descri, pa.inactive FROM ".
                TB_PREF."patient_accounts pa, ".TB_PREF."debtors_master dm".
             " where ".
                " dm.debtor_no = ".db_escape($id_customer)." and pa.debtor_no = dm.tax_id";
return combo_input($name, $selected_id, $sql, 'id_account', 'descri',
	array(
		'spec_option' => $all_option === true ? _("All Account") : $all_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change
	) );
}


/*Begin Felix Alberti 06/01/2016*/
function account_patient_list($name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer=null)
{
	global $all_items;
        //display_notification('account_patient_list'.' - '.$id_customer);
        $sql = "SELECT pt.id_account, concat(pt.id_account,'-',name,' -> ', pt.balance) as descri, pt.inactive FROM ".
                TB_PREF."payer_trans pt, ".TB_PREF."patient_accounts pc, ".TB_PREF."debtors_master d".
             " where pt.id_responsible = ".db_escape($id_customer).
                " and pt.id_account = pc.id and d.cedula = pc.debtor_no";
     //display_notification($sql);
return combo_input($name, $selected_id, $sql, 'id_account', 'descri',
	array(
		'spec_option' => $all_option === true ? _("All Account") : $all_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change
	) );
}

function account_patient_cells($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo account_patient_list($name, $selected_id, $all_option, $submit_on_change, $id_customer);
	echo "</td>\n";
}

function account_patient_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer)
{
	echo "<tr><td class='label'>$label</td>";
	account_patient_cells(null, $name, $selected_id, $all_option, $submit_on_change, $id_customer);
	echo "</tr>\n";
}
/*End Felix Alberti 06/01/2016*/
/****************************************************************/
/****************************************************************/
/*Begin Felix Alberti 04/02/2017*/
function account_patient_pend_list($name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer=null)
{
	global $all_items;
        //display_notification('account_patient_list'.' - '.$id_customer);
        $sql = "SELECT pc.id, concat(pc.id,'-',name,' -> ', pc.account_amount) as descri, 0 as inactive FROM ".
                TB_PREF."patient_accounts pc, ".TB_PREF."debtors_master d".
             " where d.debtor_no = ".db_escape($id_customer).
                " and pc.left_payment = 1 ".
                " and d.cedula = pc.debtor_no";
        //display_notification($sql);
return combo_input($name, $selected_id, $sql, 'id', 'descri',
	array(
                'order' => 'pc.tran_date desc',
		'spec_option' => $all_option === true ? _("Select a Account") : $all_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change
	) );
}

function account_patient_pend_cells($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo account_patient_pend_list($name, $selected_id, $all_option, $submit_on_change, $id_customer);
	echo "</td>\n";
}

function account_patient_pend_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer)
{
	echo "<tr><td class='label'>$label</td>";
	account_patient_pend_cells(null, $name, $selected_id, $all_option, $submit_on_change, $id_customer);
	echo "</tr>\n";
}
/*End Felix Alberti 04/02/2017*/


//---------------------------------------------------------------------------
/*Begin Felix Alberti 25/05/2016*/
function account_patient_opened_list($name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer=null)
{
	global $all_items;
        //display_notification('account_patient_list'.' - '.$id_customer);
        $sql = "SELECT pc.id, concat(pc.id, '-' ,name) as descri, pc.inactive FROM "
                .TB_PREF."patient_accounts pc, ".TB_PREF."debtors_master d".
             " where d.debtor_no = ".db_escape($id_customer).
                " and d.cedula = pc.debtor_no and left_orders = 1";
     //display_notification($sql);
return combo_input($name, $selected_id, $sql, 'id', 'descri',
	array(
		'spec_option' => $all_option === true ? _("All Account") : $all_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
                'show_inactive' => true,
                'order' => 'pc.id desc'
	) );
}

function account_patient_opened_cells($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer, $class_label='label')
{
	if ($label != null)
		echo "<td class=$class_label>$label</td>\n";
	echo "<td>";
	echo account_patient_opened_list($name, $selected_id, $all_option, $submit_on_change, $id_customer);
	echo "</td>\n";
}

function account_patient_opened_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer, $class_label='label', $type_doc=null)
{
	echo "<tr><td class=$class_label>$label</td>";
	account_patient_opened_cells(null, $name, $selected_id, $all_option, $submit_on_change, $id_customer, $class_label);
	echo "</tr>\n";
}
/*End Felix Alberti 25/05/2016*/

/*Begin Felix Alberti 28/08/2017*/
function account_patient_assoc_order_list($name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer=null, $order=null, $type=null)
{
	global $all_items;
        //display_notification('account_patient_list'.' - '.$id_customer);
       
        $sql = "SELECT pc.id, concat(pc.id, '-' ,name) as descri, pc.inactive FROM "
                .TB_PREF."patient_accounts pc, ".TB_PREF."debtors_master d,".
                TB_PREF."sales_orders sorder ".
             " where d.debtor_no = ".db_escape($id_customer).
                " and d.cedula = pc.debtor_no and sorder.account_associated = pc.id and"
                . " sorder.trans_type = ".$type." and sorder.order_no = ".$order;
        
     //display_notification($sql);
return combo_input($name, $selected_id, $sql, 'id', 'descri',
	array(
		'spec_option' => $all_option === true ? _("All Account") : $all_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
                'show_inactive' => true,
                'order' => 'pc.id desc'
	) );
}
function account_patient_assoc_order_cells($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer, $class_label='label', $order, $type)
{
	if ($label != null)
		echo "<td class=$class_label>$label</td>\n";
	echo "<td>";
	echo account_patient_assoc_order_list($name, $selected_id, $all_option, $submit_on_change, $id_customer, $order, $type);
	echo "</td>\n";
}
function account_patient_assoc_order_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $id_customer, $class_label='label', $order, $type)
{
	echo "<tr><td class=$class_label>$label</td>";
	account_patient_assoc_order_cells(null, $name, $selected_id, $all_option, $submit_on_change, $id_customer, $class_label, $order, $type);
	echo "</tr>\n";
}
/*End Felix Alberti 28/08/2017*/

/*Begin Felix Alberti 21/03/2016*/
function system_tables_list($name, $selected_id=null, $all_option=false, 
	$submit_on_change=false, $type='', $opts=array())        
{   
/*$converted_res = ($submit_on_change) ? 'true' : 'false';

    display_notification($converted_res);*/
	global $all_items;
	// all sales codes
	$sql = "SELECT table_id, description, inactive, order_
			FROM
			".TB_PREF."system_tables";			
	
	$sql .= " WHERE !inactive";    
        if ($type != '')
        $sql .= " and type_table_id = " .$type; 
        
	$sql .= " GROUP BY type_table_id, table_id ";        
	//display_notification("query profesionales:".$sql);  

	return combo_input($name, $selected_id, $sql, 'table_id', 'description',
	array(
                'order' => 'order_',	
		'spec_option' => $all_option === true ? _("All Tables") : $all_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change
	) );
}

function system_tables_list_cells($name, $selected_id=null, $all_option=false, $submit_on_change=false, $editkey=false, $type='')
{
	if ($editkey)
		set_editor('item', $name, $editkey);
		
            //display_notification("profesional_items_list_cells");
	echo "<td>";
	echo system_tables_list($name, $selected_id, $all_option, $submit_on_change, $type, array('cells'=>true));
	echo "</td>\n";
}

function system_tables_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false, $type)
{
	echo "<tr><td class='label'>$label</td>";
	system_tables_list_cells($name, $selected_id, $all_option, $submit_on_change, false, $type);
	echo "</tr>\n";
}
/*End Felix Alberti 21/03/2016*/

/*Begin Felix Alberti 04/05/2016*/
function level_break_account_list($name, $selected=null)
{
		
	$allocs = array( 
		'1'=> _("Group"),
		'2' => _("Dimensions"),
		'3' => _("Items")
	);
	return array_selector($name, $selected, $allocs);
}
/*End Felix Alberti 04/05/2016*/

/*Begin Felix Alberti 07/05/2016*/
function list_type_enter($name, $selected=null)
{
		
	$allocs = array( 
		'1'=> _("Hospitalización"),
		'2' => _("Emergencia")
	);
	return array_selector($name, $selected, $allocs);
}

function type_enter_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{	       
        if ($label != null)
		echo "<td class='label'>$label</td>\n";
	echo "<td>";
	echo list_type_enter($name, $selected_id);
	echo "</td>\n";
}
/*End Felix Alberti 07/05/2016*/
//--------------------------------------------------------------------------
/*Begin Felix Alberti 08/06/2016*/
function sales_pos_cash_list($name, $selected_id=null)
{
	global $sales_pos_cash;

	return array_selector($name, $selected_id, $sales_pos_cash);
}

function sales_pos_cash_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo sales_pos_cash_list($name, $selected_id);
	echo "</td>\n";
}

function sales_pos_cash_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	sales_pos_cash_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}
/*End Felix Alberti 08/06/2016*/

/*Begin Felix Alberti 29/07/2016*/
function group_dimensions_list($name, $selected_id=null, $no_option=false, $showname=' ',
	$submit_on_change=false)
{
$sql = "SELECT set_id, description as descrip, order_ as ord FROM ".TB_PREF."set_group";

$options = array(
	'order' => 'ord',
	'spec_option'=>$no_option ? $showname : false,
	'spec_id' => 0,
	'select_submit'=> $submit_on_change,
	'async' => false,
	);

//	if (!$showclosed)
//		$options['where'][] = "closed=0";
//	if($showtype)
//		$options['where'][] = "type_=".db_escape($showtype);

	return combo_input($name, $selected_id, $sql, 'set_id', 'descrip', $options);
}

function group_dimensions_list_cells($label, $name, $selected_id=null, $no_option=false, $showname=null, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo group_dimensions_list($name, $selected_id, $no_option, $showname, $submit_on_change);
	echo "</td>\n";
}

function group_dimension_list_row($label, $name, $selected_id=null, $no_option=false, $showname=null,
	$submit_on_change=false)
{
	echo "<tr><td class='label'>$label</td>";
	group_dimensions_list_cells(null, $name, $selected_id, $no_option, $showname, $submit_on_change);
	echo "</tr>\n";
}
/*End Felix Alberti 29/07/2016*/

/*Begin Felix Alberti 04/05/2016*/
function method_polarity_list($name, $selected=null)
{
		
	$allocs = array( 
		'+'=> _("+"),
		'-' => _("-")
	);
	return array_selector($name, $selected, $allocs);
}

function method_polarity_list_cells($label, $name, $selected_id=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo method_polarity_list($name, $selected_id);
	echo "</td>\n";
}

function method_polarity_list_row($label, $name, $selected_id=null)
{
	echo "<tr><td class='label'>$label</td>";
	method_polarity_list_cells(null, $name, $selected_id);
	echo "</tr>\n";
}
/*End Felix Alberti 04/05/2016*/


function documents_list_cells($label, $name, $selected=null, $enabled=true)
{

	if ($label != null)
		label_cell($label);
	echo "<td>\n";
	$allocs = array(
                ST_SALESINVOICE=>_("Sales Invoice"),
		ST_CUSTPAYMENT=> _("Customer Payment"),
		ST_CUSTDELIVERY=> _("Delivery Note")
	);
	echo array_selector($name, $selected, $allocs, array( 
			'select_submit'=> true, 
			'disabled' => !$enabled));
	echo "</td>\n";
}

/*Begin Felix Alberti 27/01/2017*/
function status_account_list($name, $selected_id=null, $all=false, $submit_on_change=false,
	$show_inactive = false, $msg)
{
	global $all_items;

	$sql = "SELECT id, description, inactive FROM ".TB_PREF."key_type_insurance";
        
        $msg = $msg == '' ? _("All") : _($msg);
return combo_input($name, $selected_id, $sql, 'id', 'description',
	array(
                'order' => 'order_',
		'spec_option'=>$all ? $msg : false,
		'spec_id' => '',
		'select_submit'=> $submit_on_change,
		'show_inactive' => $show_inactive
	) );
}

function status_account_list_cells($label, $name, $selected_id=null, $new_item=false, $submit_on_change=false,
	$show_inactive = false, $msg='')
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo status_account_list($name, $selected_id, $new_item, $submit_on_change, $show_inactive, $msg);
	echo "</td>\n";
}
/*End Felix Alberti 27/01/2017*/
/*Begin Felix Alberti 03/02/2017*/
function list_type_person($name, $selected=null)
{
		
	$allocs = array( 
		'V'=> _("Venezolano/a"),
		'E' => _("Extranjero/a"),
		'J' => _("Juridico"),
		'G' => _("Gobierno"),
		'O' => _("Otro"),
	);
	return array_selector($name, $selected, $allocs);
}

function type_person_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{	       
    if ($label != null)
	echo "<td class='label'>$label</td>\n";
	echo "<td>";
	echo list_type_person($name, $selected_id);
	echo "</td>\n";
}
/*End Felix Alberti 03/07/2017*/

/*Begin Felix Alberti 10/08/2017*/
function list_type_address($name, $selected=null)
{
		
	$allocs = array( 
		'R'=> _("PN Residente"),
		'NR' => _("No Residente"),
		'D' => _("Domiciliada"),
		'ND' => _("No Domiciliada")
	);
	return array_selector($name, $selected, $allocs);
}

function type_address_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{	       
    if ($label != null)
	echo "<td class='label'>$label</td>\n";
	echo "<td>";
	echo list_type_address($name, $selected_id);
	echo "</td>\n";
}
/*End Felix Alberti 10/08/2017*/

/*Begin Felix Alberti 17/08/2017*/
function list_tax_module($name, $selected=null)
{
		
	$allocs = array( 
		'1'=> _("Finiquito"),
                '2'=> _("Caso B"),
                '3'=> _("Caso C"),
	);
	return array_selector($name, $selected, $allocs);
}

function tax_module_list_row($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{	       
    if ($label != null)
	echo "<td class='label'>$label</td>\n";
	echo "<td>";
	echo list_tax_module($name, $selected_id);
	echo "</td>\n";
}
/*End Felix Alberti 17/08/2017*/

function pacientes_list_cells($label, $name, $selected_id=null, $all_option=false, $submit_on_change=false)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo pacientes_list($name, $selected_id, $all_option, $submit_on_change,
		array('cells'=>true));
}
function pacientes_list($name, $selected_id=null, $all_option=false, $submit_on_change=false, $opts=array())
{
	global $all_items;

	$sql = "SELECT ci_usuario, real_name, real_name
			FROM ".TB_PREF."users";

	return combo_input($name, $selected_id, $sql, 'ci_usuario', 'real_name',
	array_merge(
	  array(
		'format' => '_format_stock_items',
		'spec_option' => $all_option===true ?  _("All Items") : $all_option,
		'spec_id' => $all_items,
		'search_box' => true,
		'search' => array("ci_usuario", "real_name","real_name"),
		'search_submit' => get_company_pref('no_item_list')!=0,
		'size'=>10,
		'select_submit'=> $submit_on_change,
		'category' => 2,
		'order' => array('real_name','ci_usuario')
	  ), $opts) );
}
?>
