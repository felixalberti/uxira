<?php
//Módulo HealtCare

function stock_medico_list($name, $selected_id=null, $spec_option, $submit_on_change=false, 
	$show_inactive=false, $editkey = false)
{
    global $all_items;
    
	$sql = "SELECT professional_id, description FROM ".TB_PREF."professional";
	/*if ($editkey)
		set_editor('customer', $name, $editkey);*/

	$ret = combo_input($name, $selected_id, $sql, 'professional_id', 'description',
	array(
	    'order' => array('professional_id'),
		//'spec_option' => $spec_option,
		'spec_id' => $all_items,
		'async' => false
	) );
	/*if ($editkey)
		$ret .= add_edit_combo('customer');*/
	return $ret;
}

function stock_medico_list_cells($label, $name, $selected_id=null, $spec_option)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	echo stock_medico_list($name, $selected_id, $spec_option);
	echo "</td>\n";
}


function stock_medico_list_row($label, $name, $selected_id=null, $spec_option)
{
	echo "<tr>\n";
	stock_medico_list_cells($label, $name, $selected_id, $spec_option);
	echo "</tr>\n";
}


//Begin Felix Alberti 27/05/2009
function stock_responsable_list_row($label, $name, $selected_id=null, $spec_option)
{
	echo "<tr>\n";
	stock_responsable_list_cells($label, $name, $selected_id, $spec_option);
	echo "</tr>\n";
}
function stock_responsable_list_cells($label, $name, $selected_id=null, $spec_option)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	stock_responsable_list($name, $selected_id, $spec_option);
	echo "</td>\n";
}
function stock_responsable_list($name, $selected_id=null, $spec_option)
{
	$sql = "SELECT responsable_no, name FROM ".TB_PREF."respons_pago_master";
	combo_input($name, $selected_id, $sql, 'responsable_no', 'name',
	array('order'=>'responsable_no',	'spec_option' => $spec_option === true ? '' : $spec_option ));
}
//----------------------------------------------------------------------------------------
?>