<?php
/**********************************************************************
    Copyright (C) FELIX ALBERTI, CA.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
//29/06/2010
$page_security = "SA_MEDICAL_APPOINMENT_UNIDFUNC";
$path_to_root="../..";
include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

//include_once($path_to_root . "/sales/includes/sales_ui.inc");
//include_once($path_to_root . "/sales/includes/sales_db.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas/includes/db/unid_func_db.inc");


$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
page(_("Unidad funcional"), false, false, "", $js);

simple_page_mode(true);


//------------------------------------------------------------------------------------------------
if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	//initialise no input errors assumed initially before we test
	$input_error = 0;
  if (strlen($_POST['codunifunc']) == 0) 
	{
		$input_error = 1;
		display_error(_("Indique un c�digo para el codunifunc."));
		set_focus('codunifunc');
	}
	elseif (strlen($_POST['descripcion']) == 0) 
	{
		$input_error = 1;
		display_error(_("La descripci�n no puede estar vacia."));
		set_focus('descripcion');
	}
	
	if ($input_error !=1)
	{
    	if ($selected_id != -1) 
    	{
		  update_unifun($selected_id, $_POST['descripcion']); 		
			display_notification(_('El item ha sido actualizado: '));
    	} 
    	else 
    	{
		    add_unifun($_POST['codunifunc'], $_POST['descripcion']);                         
			display_notification(_('El nuevo item ha sido a�adido'));
    	}
		$Mode = 'RESET';
	}
}
//---------------------------------------------------------------------------------- 

if ($Mode == 'Delete')
{

	// PREVENT DELETES IF DEPENDENT RECORDS IN 'asoc_item_plan'
	/*$sql1= "SELECT COUNT(*) FROM ".TB_PREF."asoc_item_plan WHERE 
	 item_plan=$selected_id";
	$result1 = db_query($sql1, "could not query stock master");
	$myrow1 = db_fetch_row($result1);	
	if ($myrow1[0] > 0) 
	{
		display_error(_("No se puede borrar este item porque esta siendo usado en la tabla de planes."));
    return false;		
	} 	
	
	else 
	{*/
		delete_unifun($selected_id);
		display_notification(_('El item seleccionado ha sido borrado'));
		  
  //}
	$Mode = 'RESET';
}
//----------------------------------------------------------------------------------
if ($Mode == 'RESET')
{
	$selected_id = -1;
	unset($_POST['codunifunc']);	
	unset($_POST['descripcion']);
}
//----------------------------------------------------------------------------------
start_form(false, true);

start_table("class='tablestyle_noborder'");
start_row();
   
    /*$param = explode(";", $selected_id);
    if ($selected_id != -1) $codmed = $param['0'];
    else $codmed = null;*/

	  //type_patron_list_row(_("Seleccione un m�dico:"), 'selec_tabla', null, null, true);
	  //medico_list_row(_("Seleccione un medico: "), 'medico_sel', $codmed,
	  //false, true);
	  //type_patron_list_row(_("Seleccionar un patr�n:"), 'selec_tabla', $codmed, false, true);	  
	  //tipo_cita_list_row(_("Seleccionar un tipo de cita:"), 'tipo_cita_sel', null, null, true);
    //if (isset($_POST['selec_tabla']) && (($_POST['selec_tabla'] != '')) ) $selec_tabla = $_POST['selec_tabla'];    
    //else $selec_tabla = '%';  
   //if (isset($_POST['tipo_cita_sel']) && (($_POST['tipo_cita_sel'] != '')) ) $tipo_cita_sel = $_POST['tipo_cita_sel'];    
   //else $tipo_cita_sel = '%';     
  

end_row();
end_table();
end_form();
//------------------------------------------------------------------------------------------------

function edit_tabla($row){
	  $link = edit_button_cell2("Edit".$row["codunifun"], _("Edit"));
		return $link;
}	
function borrar_tabla($row){
	  $link = delete_button_cell2("Delete".$row["codunifun"], _("Delete"));
		return $link;
}	
/*function col_hidden($row){
		$_POST['id1'] = $row["id"];
		$link = hidden('id1', $_POST['id1']);
		//$link = text_row(_("Id:"), 'id1', null, 6, 6);
		return $link;
}	*/
//------------------------------------------------------------------------------------------------
   $sql = "SELECT codunifun, descripcion FROM ".TB_PREF.
   "cm_unidad_func";
 
$sql = $sql . " order by codunifun";
//display_notification($sql);
//------------------------------------------------------------------------------------------------

$cols = array(
	_("Codigo") => array('align'=>'center'),
	_("Descripcion") => array('align'=>'center'),
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));


$table =& new_db_pager('doc_tbl', $sql, $cols);
//$table->set_marker('check_overdue', _("Marked items are overdue."));

//if (get_post('selec_tabla')) {
	$table->set_sql($sql);
	$table->set_columns($cols);
//}
$table->width = "30%";
start_form();

display_db_pager($table);
//
//echo "Mode: $Mode - id = $selected_id";
echo "<br>";
start_table(TABLESTYLE2);
if ($selected_id != -1) { 	
	if ($Mode == 'Edit') {
		$myrow = get_unifun($selected_id);
		$_POST['codunifunc'] = $myrow["codunifun"];		
		$_POST['descripcion'] = $myrow["descripcion"];

	}
	hidden('selected_id', $selected_id);
	//hidden('selected_codmed', $param[0]);
	//hidden('id');	
}	

if ($selected_id != -1) {

     label_row(_("Codigo:"), $_POST['codunifunc']); 
     hidden('codunifunc', $_POST['codunifunc']);

}
else {

    text_row(_("Codigo:"), 'codunifunc', null, 6, 6);    
}
    text_row(_("Descripcion:"), 'descripcion', null, 20, 20);    


end_table(1);

submit_add_or_update_center($selected_id == -1, '', true);

end_form();
end_page();


?>
