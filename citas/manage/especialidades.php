<?php
/**********************************************************************
    Copyright 2011 (C) Angios, CA.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
$page_security = "SA_MEDICAL_APPOINMENT_SPECIALITY";
$path_to_root="../..";
include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas//includes/db/especialidades_db.inc");
include_once($path_to_root . "/includes/db/connect_db_sqlserver.inc");

$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
page(_("Especialidades"), false, false, "", $js);

simple_page_mode(true);

//$conn_sqlsvr = conectar_angiosgt();
//------------------------------------------------------------------------------------------------
if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	//initialise no input errors assumed initially before we test
	$input_error = 0;
  
	if (strlen($_POST['codigo']) == 0) 
	{
		$input_error = 1;
		display_error(_("El c�digo tipo no puede estar vacio."));
		set_focus('codigo');
	}
	
	
	if ($input_error !=1)
	{
    	if ($selected_id != -1) 
    	{
		    update_especialidades($conn_sqlsvr, $selected_id, $_POST['codigo']);    		
			  display_notification(_('El item ha sido actualizado'));
    	} 
    	else 
    	{
		    add_especialidades($conn_sqlsvr,$_POST['codigo']);
			  display_notification(_('El nuevo item ha sido a�adido'));
    	}
		$Mode = 'RESET';
	}
}
//---------------------------------------------------------------------------------- 

if ($Mode == 'Delete')
{

	// PREVENT DELETES IF DEPENDENT RECORDS IN 'asoc_item_plan'
	
		delete_especialidad($selected_id);
		display_notification(_('El item seleccionado ha sido borrado'));
		  
  
	$Mode = 'RESET';
}
//----------------------------------------------------------------------------------
if ($Mode == 'RESET')
{
	$selected_id = -1;
	//unset($_POST['codigotabla']);	
	unset($_POST['codigo']);			
  //unset($_POST['nombre']);	  
}
//----------------------------------------------------------------------------------
start_form(false, true);

start_table("class='tablestyle_noborder'");
start_row();
    
    
    if (isset($_POST['codigotabla'])){
    	$tabla = $_POST['codigotabla'];
    }	
    else
    {    	
     	$tabla = null;
    	}

	  especialidad_list_row(_("Seleccionar una Especialidad:"), 'selec_tabla', $tabla, null, true);
    if (isset($_POST['selec_tabla']) && (($_POST['selec_tabla'] != '')) ) $selec_tabla = $_POST['selec_tabla'];    
    else $selec_tabla = '%';  
  

end_row();
end_table();
end_form();
//------------------------------------------------------------------------------------------------

function edit_tabla($row){
	  $link = edit_button_cell2("Edit".$row["id"], _("Edit"));
		return $link;
}	
function borrar_tabla($row){
	  $link = delete_button_cell2("Delete".$row["id"], _("Delete"));
		return $link;
}	
//------------------------------------------------------------------------------------------------
if ($selec_tabla == "%")
   $sql = "SELECT id, codigo, nombre FROM ".TB_PREF.
   "cm_especialidad order by nombre";   	
else   	
   $sql = "SELECT id, codigo, nombre FROM ".TB_PREF.
   "cm_especialidad where codigo like '$selec_tabla' order by nombre";   	
//------------------------------------------------------------------------------------------------
//Display_notification($selec_tabla." . ".$sql);

$cols = array(
	_("Id") => array('align'=>'right'),
	_("C�digo") => array('align'=>'right'),
	_("Nombre"), 
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));


$table =& new_db_pager('doc_tbl', $sql, $cols);
//$table->set_marker('check_overdue', _("Marked items are overdue."));

//if (get_post('selec_tabla') || get_post('codigo')) {
	$table->set_sql($sql);
	$table->set_columns($cols);
//}
$table->width = "50%";
start_form();

display_db_pager($table);
//
//echo "Mode: $Mode - id = $selected_id";
echo "<br>";
start_table($table_style2);
if ($selected_id != -1) { 	
	if ($Mode == 'Edit') {

		$myrow = get_especialidad($selected_id);		
		$_POST['id'] = $myrow["id"];
	  //$_POST['codigotabla'] = $myrow["codigotabla"];
	  $_POST['codigo'] = $myrow["codigo"];	  
		//$_POST['nombre'] = $myrow["nombre"];
	}
	hidden('selected_id', $selected_id);
	hidden('id');	
}	


especialidad_list_row_gt($conn_sqlsvr,_("Codigo:"), "codigo", null, false, true);



end_table(1);

submit_add_or_update_center($selected_id == -1, '', true);

end_form();
end_page();

if ($conn_sqlsvr){
   sqlsrv_close($conn_sqlsvr);	
}

?>
