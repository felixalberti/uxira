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
$page_security = "SA_MEDICAL_APPOINMENT_DAYSWORKMEDICO";
$path_to_root="../..";
include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

//include_once($path_to_root . "/sales/includes/sales_ui.inc");
//include_once($path_to_root . "/sales/includes/sales_db.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas//includes/db/diatrabxmedico_db.inc");
include_once($path_to_root . "/citas//includes/db/medico_db.inc");


$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
page(_("D�as de trabajo por m�dico"), false, false, "", $js);

simple_page_mode(true);

//------------------------------------------------------------------------------------------------
if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	//initialise no input errors assumed initially before we test
	$input_error = 0;
  if (strlen($_POST['codmed']) == 0) 
	{
		$input_error = 1;
		display_error(_("Seleccione un m�dico de la lista."));
		set_focus('codmed');
	}
	elseif (strlen($_POST['coddia']) == 0) 
	{
		$input_error = 1;
		display_error(_("El d�a no puede estar vacio."));
		set_focus('tipocita');
	}
	
	if ($input_error !=1)
	{
    	if ($selected_id != -1) 
    	{
		  update_item_diatrabxmed($selected_id, $_POST['coddia'], $_POST['enuso']); 		
			display_notification(_('El item ha sido actualizado: '));
    	} 
    	else 
    	{
		    add_item_diatrabxmed($_POST['codmed'], $_POST['coddia'], $_POST['enuso']);                         
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
	  $param = explode(";", $selected_id);
		delete_item_diatrabxmed($param['0'], $param['1']);
		display_notification(_('El item seleccionado ha sido borrado'));
		  
  //}
	$Mode = 'RESET';
}
//----------------------------------------------------------------------------------
if ($Mode == 'RESET')
{
	$selected_id = -1;
	//unset($_POST['codmed']);	
	//unset($_POST['tipocita']);
}
//----------------------------------------------------------------------------------
start_form(false, true);

start_table("class='tablestyle_noborder'");
start_row();
   
    $param = explode(";", $selected_id);
    if ($selected_id != -1) $codmed = $param['0'];
    elseif (isset($_POST['codmed'])){
    	$codmed = $_POST['codmed'];
    }
    else $codmed = null;

	  //type_patron_list_row(_("Seleccione un m�dico:"), 'selec_tabla', null, null, true);
	  medico_list_row(_("Seleccione un medico: "), 'medico_sel', $codmed,
	  false, true);
	  //tipo_cita_list_row(_("Seleccionar un tipo de cita:"), 'tipo_cita_sel', null, null, true);
    if (isset($_POST['medico_sel']) && (($_POST['medico_sel'] != '')) ) $selec_tabla = $_POST['medico_sel'];    
    else $selec_tabla = '%';  
   //if (isset($_POST['tipo_cita_sel']) && (($_POST['tipo_cita_sel'] != '')) ) $tipo_cita_sel = $_POST['tipo_cita_sel'];    
   //else $tipo_cita_sel = '%';     
  

end_row();
end_table();
end_form();
//------------------------------------------------------------------------------------------------

function edit_tabla($row){
	  $link = edit_button_cell2("Edit".$row["codmed"].';'.$row["coddia"], _("Edit"));
		return $link;
}	
function borrar_tabla($row){
	  $link = delete_button_cell2("Delete".$row["codmed"].';'.$row["coddia"], _("Delete"));
		return $link;
}	
function dia_semana($row){
	  $dia = $row["coddia"];
		$semanaArray = array(
		1 => "Domingo", 
		2 => "Lunes", 
		3 => "Martes", 
		4 => "Miércoles", 
		5 => "Jueves", 
		6 => "Viernes", 
		7 => "Sábado"	);
    $link = $semanaArray[$dia];					
	  return $link;
}
function dia_sem_cadena($dia){
		$semanaArray = array(
		1 => "Domingo", 
		2 => "Lunes", 
		3 => "Martes", 
		4 => "Miércoles", 
		5 => "Jueves", 
		6 => "Viernes", 
		7 => "Sábado"	);
    $link = $semanaArray[$dia];					
	  return $link;
}
function obt_medico($row){
	  return get_medico($row['codmed']);
}	

/*function col_hidden($row){
		$_POST['id1'] = $row["id"];
		$link = hidden('id1', $_POST['id1']);
		//$link = text_row(_("Id:"), 'id1', null, 6, 6);
		return $link;
}	*/
//------------------------------------------------------------------------------------------------
//if ($selec_tabla == "%") {
   $sql = "SELECT dtm.codmed, dtm.coddia, dtm.enuso FROM ".TB_PREF.
   "cm_diatrabxmedico dtm";
   if ($selec_tabla != "%") $sql = $sql . " where dtm.codmed like '$selec_tabla'";       
//}  	
/*else {   	
   $sql = "SELECT codmed, coddia, patron FROM ".TB_PREF.
   "cm_rel_medpatronxdia  where codmed like '$selec_tabla'";   	
   $sql = $sql . " and tipocita like '$tipo_cita_sel'"; 
} */  
$sql = $sql . " order by dtm.codmed, dtm.coddia";
display_notification($sql);
//------------------------------------------------------------------------------------------------

$cols = array(
	_("M�dico") => Array('align'=>'center','fun'=>'obt_medico'),
	_("D�a") => Array('align'=>'center', 'fun'=>'dia_semana'),
  _("En uso") => array('align'=>'center'),	
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));


$table =& new_db_pager('doc_tbl', $sql, $cols);
//$table->set_marker('check_overdue', _("Marked items are overdue."));

if (get_post('medico_sel')) {
	$table->set_sql($sql);
	$table->set_columns($cols);
}
$table->width = "40%";
start_form();

display_db_pager($table);
//
//echo "Mode: $Mode - id = $selected_id";
echo "<br>";
start_table(TABLESTYLE2);
if ($selected_id != -1) { 	
	if ($Mode == 'Edit') {
		//display_notification($selected_id);
    $param = explode(";", $selected_id);
    $selected_id = $param['0'];
		$myrow = get_item_diatrabxmed($param['0'],$param['1']);
		$_POST['codmed'] = $myrow["codmed"];		
		$_POST['coddia'] = $myrow["coddia"];
	 	$_POST['enuso']  = $myrow["enuso"];
	 	
	}
	hidden('selected_id', $selected_id);
	hidden('selected_codmed', $param[0]);
	hidden('coddia_aux', $myrow['coddia']);
	//hidden('id');	
}	

if ($selected_id != -1) {
	//|| item_grupotabla_used($selected_id) || item_plan_asoc_stock_used($selected_id)){
    //label_row(_("Patron:"), $_POST['selec_tabla']);
    //hidden('patron', $_POST['selec_tabla']);	
    //label_row(_("Tipo Cita:"), $_POST['tipo_cita_sel']);
    //hidden('tipocita', $_POST['tipo_cita_sel']);
    //text_row(_("Patron:"), 'patron', null, 6, 6);
     label_row(_("Medico:"), get_medico($_POST['codmed'])); 
     hidden('codmed', $_POST['codmed']);
    //text_row(_("Tipo Cita:"), 'tipo_cita', null, 3, 3);
    if (isset($_POST['coddia'])){
       $_POST['desc_dia'] = dia_sem_cadena($_POST['coddia']);
       label_row(_("D�a:"), $_POST['desc_dia']);
    }
    //label_row(_("D�a:"), $_POST['coddia']);
    hidden('coddia', $_POST['coddia']);   
    //label_row(_("Numero:"), $_POST['numero']);
    //hidden('numero', $_POST['numero']); 
}
else {
    //tablas_master_list_row(_("Patron:"), 'selec_tabla', null, false);
    //type_patron_list_row(_("Patr�n:"), 'patron', null, false, true);
    medico_list_row(_("M�dico: "), 'codmed', null, false, false);    
    //text_row(_("Tipo Cita:"), 'tipocita', null, 3, 3);
    //tipo_cita_list_row(_("D�a:"), 'coddia', null, false, false);
    day_week_list_row(_("D�a:"), 'coddia', null,(!isset($_POST['coddia']) || $selected_id));
    //text_row(_("D�a:"), 'coddia', null, 2, 1);    
}
//text_row(_("En uso:"), 'enuso', null, 2, 1);
yesno_list_row(_("En uso:"), 'enuso', null, "", "", false);
//date_row(_("Hora cita:"), 'hora_cita', null, null, 0, 0, 0, null, false);
//text_row(_("Cl�nica:"), 'clinica', null, 3, 3);
//text_row(_("Unidad funcional:"), 'codunidfunc', null, 3, 3);

end_table(1);

submit_add_or_update_center($selected_id == -1, '', true);

end_form();
end_page();

?>
