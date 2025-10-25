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
//28/06/2010
$page_security = 1;
$path_to_root="../..";
include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

include_once($path_to_root . "/includes/db/connect_db_sqlserver.inc");
$conn_sqlsvr = conectar_angiosgt();
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas//includes/db/tipocita_x_medico_db.inc");
include_once($path_to_root . "/citas/includes/db/especialidades_db.inc");
include_once($path_to_root . "/citas//includes/db/medico_db.inc");
include_once($path_to_root . "/citas//includes/db/tipocita_db.inc");
include_once($path_to_root . "/citas/includes/db/servicio_db.inc");


$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
page(_("Tipo de cita por médico gestión total"), false, false, "", $js);

simple_page_mode(true);


//------------------------------------------------------------------------------------------------
if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	//initialise no input errors assumed initially before we test
	$input_error = 0;
  if (strlen($_POST['codmed']) == 0) 
	{
		$input_error = 1;
		display_error(_("Seleccione un médico de la lista."));
		set_focus('codmed');
	}
	elseif (strlen($_POST['servicio']) == 0) 
	{
		$input_error = 1;
		display_error(_("El tipo de cita no puede estar vacio."));
		set_focus('tipocita');
	}
	
	if ($input_error !=1)
	{
    	if ($selected_id != -1) 
    	{
		  update_item_tpcitaxmedico($selected_id, $_POST['servicio'], $_POST['precio']); 		
			display_notification(_('El item ha sido actualizado: '));
    	} 
    	else 
    	{
		    add_item_tpcitaxmedico($_POST['codmed'], $_POST['servicio'], $_POST['precio']);                         
			display_notification(_('El nuevo item ha sido añadido'));
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
		delete_item_tpcitaxmedico($param['0'], $param['1']);
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
    if ($selected_id != -1) 
    {$codmed = $param['0'];
     $servicio = $param['1'];
    	}
    elseif (isset($_POST['codmed'])){
    	$codmed = $_POST['codmed'];
    	//$tipocita = $_POST['tipocita'];
    }	
    elseif (isset($_POST['medico_sel'])){
    	$codmed = $_POST['medico_sel'];
    	//$tipocita = $_POST['tipocita'];

    }	
    else 
    {    	
    	$codmed = null;
    	//$tipocita = null;
    }
    	
    if (isset($_POST['especialidad_sel']))
    $especialidad = $_POST['especialidad_sel'];
    else
    if (isset($_POST['especialidad']))
    $especialidad = $_POST['especialidad'];
    else $especialidad = null;	

    if (isset($_POST['serv_sel']))
    $servicio = $_POST['serv_sel'];
    else
    if (isset($_POST['servicio']))
    $servicio = $_POST['servicio'];
    else $servicio = null;

	  //type_patron_list_row(_("Seleccione un médico:"), 'selec_tabla', null, null, true);
	  //medico_list_row(_("Seleccione un medico: "), 'medico_sel', $codmed,
	  //false, true);
	  //tipo_cita_list_row(_("Seleccionar un tipo de cita:"), 'tipo_cita_sel', $tipocita, null, true);
	  
	  especialidad_list_row_gt($conn_sqlsvr,_("Especialidad:"), "especialidad", $especialidad, true, true); 
   // medico_list_row(_("Seleccione un medico: "), 'medico_sel', $codmed, false, true);
    medico_list_row_gt($conn_sqlsvr, _("Seleccione un medico GT: "), 'medico_sel', $codmed, true, true, $especialidad);
    servicio_list_row_gt($conn_sqlsvr, _("Seleccione un servicio GT: "), 'serv_sel', $servicio, false, true, $especialidad); 
	  
    if (isset($_POST['medico_sel']) && (($_POST['medico_sel'] != '')) ) $selec_tabla = $_POST['medico_sel'];    
    else $selec_tabla = '%';  
   if (isset($_POST['tipo_cita_sel']) && (($_POST['tipo_cita_sel'] != '')) ) $tipo_cita_sel = $_POST['tipo_cita_sel'];    
    else $tipo_cita_sel = '%';     
  

end_row();
end_table();
end_form();
//------------------------------------------------------------------------------------------------

function edit_tabla($row){
	  $link = edit_button_cell2("Edit".$row["codmed"].';'.$row["tipocita"], _("Edit"));
		return $link;
}	
function borrar_tabla($row){
	  $link = delete_button_cell2("Delete".$row["codmed"].';'.$row["tipocita"], _("Delete"));
		return $link;
}	
function obt_medico($row){
	  return get_medico_gt($row['codmed']);
}	

function obt_servicio_gt($row){
	  return get_servicio_gt($row['tipocita']);
}	
/*function col_hidden($row){
		$_POST['id1'] = $row["id"];
		$link = hidden('id1', $_POST['id1']);
		//$link = text_row(_("Id:"), 'id1', null, 6, 6);
		return $link;
}	*/
//------------------------------------------------------------------------------------------------
if ($selec_tabla == "%") {
   $sql = "SELECT codmed, tipocita, st.descripcion as desc_cita, precio FROM ".TB_PREF.
   "cm_tpcita_x_medico tcm".
   " left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = tcm.tipocita";
   if ($tipo_cita_sel != "%") $sql = $sql . " where tcm.tipocita like '$tipo_cita_sel'";       
}  	
else {   	
   $sql = "SELECT codmed, tipocita, st.descripcion as desc_cita, precio FROM ".TB_PREF.
   "cm_tpcita_x_medico tcm".
   " left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = tcm.tipocita".   
   " where tcm.codmed like '$selec_tabla'";   	
   $sql = $sql . " and tcm.tipocita like '$tipo_cita_sel'"; 
}   
$sql = $sql . " order by codmed, tipocita";
//------------------------------------------------------------------------------------------------

$cols = array(
	_("Médico") => array('align'=>'center','fun'=>'obt_medico'),
	_("Servicio") => array('align'=>'center'),
	_("Desc. servicio") => array('align'=>'left','fun'=>'obt_servicio_gt'),	
	_("Precio") => 'amount',	
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));


$table =& new_db_pager('doc_tbl', $sql, $cols);
//$table->set_marker('check_overdue', _("Marked items are overdue."));

if (get_post('medico_sel') || get_post('servicio_sel')) {
	$table->set_sql($sql);
	$table->set_columns($cols);
}
$table->width = "50%";
start_form();

display_db_pager($table);
//
//echo "Mode: $Mode - id = $selected_id";
echo "<br>";
start_table($table_style2);
if ($selected_id != -1) { 	
	if ($Mode == 'Edit') {
    $param = explode(";", $selected_id);
    $selected_id = $param['0'];
		$myrow = get_item_tpcitaxmedico($param['0'],$param['1']);
		$_POST['codmed'] = $myrow["codmed"];		
		$_POST['servicio'] = $myrow["tipocita"];
	 	$_POST['precio']  = price_format($myrow["precio"]);
	 
	}
	hidden('selected_id', $selected_id);
	hidden('medico', $selected_id);
	//hidden('id');	
}	

if ($selected_id != -1) {
	//|| item_grupotabla_used($selected_id) || item_plan_asoc_stock_used($selected_id)){
    //label_row(_("Patron:"), $_POST['selec_tabla']);
    //hidden('patron', $_POST['selec_tabla']);	
    //label_row(_("Tipo Cita:"), $_POST['tipo_cita_sel']);
    //hidden('tipocita', $_POST['tipo_cita_sel']);
    //text_row(_("Patron:"), 'patron', null, 6, 6);
     label_row(_("Especialidad:"), get_especialidad_gt($_POST['especialidad'])); 
     hidden('especialidad', $_POST['especialidad']);
     label_row(_("Medico:"), get_medico_gt($_POST['codmed'])); 
     hidden('codmed', $_POST['codmed']);
    //text_row(_("Tipo Cita:"), 'tipo_cita', null, 3, 3);	 
    label_row(_("Servicio:"), get_servicio_gt($_POST['servicio']));
    hidden('servicio', $_POST['servicio']);
    label_row(_("Precio:"), get_precio_x_serv_gt($_POST['servicio']));
    hidden('precio', $_POST['precio']);   
    //label_row(_("Numero:"), $_POST['numero']);
    //hidden('numero', $_POST['numero']); 
}
else {
    //tablas_master_list_row(_("Patron:"), 'selec_tabla', null, false);
    //type_patron_list_row(_("Patrón:"), 'patron', null, false, true);
    //medico_list_row(_("Médico: "), 'codmed', null, false, false);    
    //text_row(_("Tipo Cita:"), 'tipocita', null, 3, 3);
    //tipo_cita_list_row(_("Tipo Cita:"), 'tipocita', null, false, false);
    //text_row(_("Número:"), 'numero', null, 4, 4);    
    especialidad_list_row_gt($conn_sqlsvr,_("Especialidad:"), "especialidad", $especialidad, true, true);    
    medico_list_row_gt($conn_sqlsvr, _("Médico GT: "), 'codmed', $codmed, false, true, $especialidad);
    servicio_list_row_gt($conn_sqlsvr, _("Servicio GT: "), 'servicio', $servicio, false, true, $especialidad);
    text_row(_("Precio:"), 'precio', null, 20, 20);
}

//date_row(_("Hora cita:"), 'hora_cita', null, null, 0, 0, 0, null, false);
//text_row(_("Clínica:"), 'clinica', null, 3, 3);
//text_row(_("Unidad funcional:"), 'codunidfunc', null, 3, 3);

end_table(1);

submit_add_or_update_center($selected_id == -1, '', true);

end_form();
end_page();

if ($conn_sqlsvr){
   sqlsrv_close($conn_sqlsvr);	
}

?>
