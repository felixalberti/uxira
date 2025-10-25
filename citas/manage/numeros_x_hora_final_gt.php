<?php
/**********************************************************************
    Copyright (C) Félix Alberti Julio 2011.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
$page_security = 1;
$path_to_root="../..";
include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/includes/db/connect_db_sqlserver.inc");
$conn_sqlsvr = conectar_angiosgt();
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas//includes/db/numeros_x_hora_db.inc");
include_once($path_to_root . "/citas/includes/db/especialidades_db.inc");
include_once($path_to_root . "/citas/includes/db/servicio_db.inc");


$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
page(_("Números y Hora gestión total"), false, false, "", $js);

simple_page_mode(true);


//------------------------------------------------------------------------------------------------
if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	//initialise no input errors assumed initially before we test
	$input_error = 0;
  if (strlen($_POST['patron']) == 0) 
	{
		$input_error = 1;
		display_error(_("Seleccione un patrón de la lista."));
		set_focus('patron');
	}
	elseif (strlen($_POST['servicio']) == 0) 
	{
		$input_error = 1;
		display_error(_("El servicio o tipo de cita no puede estar vacio."));
		set_focus('tipo_cita');
	}
	elseif (strlen($_POST['numero']) == 0) 
	{
		$input_error = 1;
		display_error(_("El número no puede estar vacio."));
		set_focus('numero');
	}
	
	if ($input_error !=1)
	{
    	if ($selected_id != -1) 
    	{
		  update_item_numeros($selected_id, $_POST['patron'], $_POST['servicio'], $_POST['numero'], $_POST['hora_cita'], $_POST['clinica'],
		  $_POST['codunidfunc']); 		
			display_notification(_('El item ha sido actualizado: '));
    	} 
    	else 
    	{
		    add_item_numeros($_POST['patron'], $_POST['servicio'], $_POST['numero'], $_POST['hora_cita'], $_POST['clinica'],
		  $_POST['codunidfunc']);                         
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
		delete_item_numeros($selected_id);
		display_notification(_('El item seleccionado ha sido borrado'));
		  
  //}
	$Mode = 'RESET';
}
//----------------------------------------------------------------------------------
if ($Mode == 'RESET')
{
	$selected_id = -1;
	refresh_pager('doc_tbl');
}
//----------------------------------------------------------------------------------
start_form(false, true);

start_table("class='tablestyle_noborder'");
start_row();
   
    //$param = explode(";", $selected_id);
    if ($selected_id != -1) 
    {  
    	    	 
    	 $myrow1 = get_id_numeros_hora($selected_id);
		   $codpatron = $myrow1["patron"];		
		   $servicio = $myrow1["tipo_cita"];
    	 
    }
    elseif (isset($_POST['patron'])){
    	$codpatron = $_POST['patron'];
    }
    else {
     $codpatron = null;
    }
    
    if (isset($_POST['servicio_sel'])){
      $servicio = $_POST['servicio_sel'];
      $tipo_cita_sel = $servicio;
    }
    elseif (isset($_POST['servicio'])){
      $servicio = $_POST['servicio'];
      $tipo_cita_sel = $servicio;
    }
    else 
    {$servicio = null;
     $tipo_cita_sel = '%';
    }	
    //
    if (isset($_POST['especialidad_sel']))
    $especialidad = $_POST['especialidad_sel'];
    elseif (isset($_POST['especialidad']))
    $especialidad = $_POST['especialidad'];
    else $especialidad = null;
    

	  type_patron_list_row(_("Seleccionar un patrón:"), 'selec_tabla', $codpatron, null, true);
	  //tipo_cita_list_row(_("Seleccionar un tipo de cita:"), 'tipo_cita_sel', $tipocita, null, true);
	  especialidad_list_row_gt($conn_sqlsvr,_("Especialidad:"), "especialidad_sel", $especialidad, true, true); 
    servicio_list_row_gt($conn_sqlsvr, _("Seleccione un servicio GT: "), 'servicio_sel', $servicio, false, true, $especialidad); 
    if (isset($_POST['selec_tabla']) && (($_POST['selec_tabla'] != '')) ) $selec_tabla = $_POST['selec_tabla'];    
    else $selec_tabla = '%';  
    //if (isset($_POST['servicio_sel']) && (($_POST['servicio_sel'] != '')) ) $tipo_cita_sel = $servicio;    
    //else $tipo_cita_sel = '%';     
  

end_row();
end_table();
end_form();
//------------------------------------------------------------------------------------------------


function edit_tabla($row){
	  $link = edit_button_cell2("Edit".$row["id"], _("Edit"));
		return $link;
}	
/*function borrar_tabla($row){
	  $link = delete_button_cell2("Delete".$row["patron"].';'.$row["tipo_cita"].';'.$row["numero"], _("Delete"));
		return $link;
}*/
function borrar_tabla($row){
	  $link = delete_button_cell2("Delete".$row["id"], _("Delete"));
		return $link;
}		
function obt_servicio_gt($row){
	  return get_servicio_gt($row['tipo_cita']);
}	
//------------------------------------------------------------------------------------------------
if ($selec_tabla == "%") {
   $sql = "SELECT nc.patron, p.descripcion as descri_patron, nc.tipo_cita, st.descripcion, nc.numero, nc.hora_cita, nc.codunidfunc, nc.id FROM ".TB_PREF.
   "cm_numeroscita nc left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = nc.tipo_cita".
   ", ".TB_PREF."cm_patron p where nc.patron = p.patron ";
   if ($tipo_cita_sel != "%") $sql = $sql . " and nc.tipo_cita like '$tipo_cita_sel'";       
}  	
else {   	
   /*$sql = "SELECT nc.patron, p.descripcion as descri_patron, nc.tipo_cita, st.descripcion, nc.numero, nc.hora_cita, nc.codunidfunc, nc.id FROM ".TB_PREF.
   "cm_numeroscita nc left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = nc.tipo_cita".
   ", ".TB_PREF."cm_patron p where nc.patron = p.patron and nc.patron like '$selec_tabla'";  */
   
   $sql = "SELECT nc.patron, p.descripcion as descri_patron, nc.tipo_cita, nc.tipo_cita, nc.numero, nc.hora_cita,
    nc.codunidfunc, nc.id FROM ".TB_PREF."cm_numeroscita nc,".TB_PREF."cm_patron p 
    where nc.patron = p.patron and nc.patron like '$selec_tabla'";
   $sql = $sql . " and nc.tipo_cita like '$tipo_cita_sel'"; 
}   
$sql = $sql . " order by p.patron, nc.tipo_cita, nc.id desc";
//display_notification($sql);
//------------------------------------------------------------------------------------------------

$cols = array(
	_("Cód. patron") => array('align'=>'center'),
	_("Desc. patron") => array('align'=>'left'),	
	_("Tipo cita") => array('align'=>'center'),
	_("Desc. tipo cita") => array('align'=>'left','fun'=>'obt_servicio_gt'),		
	_("Número") => array('align'=>'right'),
	_("Hora cita") => array('align'=>'center'), 
  _("Unid. Funcional") => array('align'=>'center'),	
  _("Id") => array('align'=>'center'),
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));

if (get_post('ADD_ITEM'))
  refresh_pager('doc_tbl');

$table =& new_db_pager('doc_tbl', $sql, $cols);
//$table->set_marker('check_overdue', _("Marked items are overdue."));

//if (get_post('selec_tabla') || get_post('tipo_cita_sel')) {
	$table->set_sql($sql);
	$table->set_columns($cols);
//}
$table->width = "60%";
start_form();

display_db_pager($table);
//
//echo "Mode: $Mode - id = $selected_id";
echo "<br>";
start_table($table_style2);
if ($selected_id != -1) { 	
	if ($Mode == 'Edit') {
    //$param = explode(";", $selected_id);
    //$selected_id = $param['0'];
		//$myrow = get_item_numeros_hora($param[0],$param[1],$param[2]);
		$myrow = get_id_numeros_hora($selected_id);
		$_POST['patron'] = $myrow["patron"];		
		$_POST['tipo_cita'] = $myrow["tipo_cita"];
	  $_POST['numero'] = $myrow["numero"];
	  $_POST['hora_cita'] = $myrow["hora_cita"];	  
		$_POST['clinica'] = $myrow["clinica"];
	  $_POST['codunidfunc']  = $myrow["codunidfunc"];		
		//$_POST['valor_tarifa']  = price_format($myrow["valor"]);
		//hidden('selected_tipo_cita', $param[1]);
	  //hidden('selected_numero', $param[2]);	  
	}
	hidden('selected_id', $selected_id);
	//hidden('selected_tipo_cita', $param[1]);
	//hidden('selected_numero', $param[2]);
	//hidden('id');	
}	

if ($selected_id != -1) {
	
    label_row(_("Patrón:"), $_POST['patron']); 
    hidden('patron', $_POST['patron']);
    
    //tipo_cita_list_row(_("Tipo Cita:"), 'tipo_cita', null, false, false);
    label_row(_("Especialidad:"), get_especialidad_gt($_POST['especialidad']));
    hidden('especialidad', $_POST['especialidad']);   

    label_row(_("Servicio:"), get_servicio_gt($_POST['servicio']));
    hidden('servicio', $_POST['servicio']);   

    label_row(_("Numero:"), $_POST['numero']);
    hidden('numero', $_POST['numero']); 
}
else {
    type_patron_list_row(_("Patrón:"), 'patron', null, true, true);

    especialidad_list_row_gt($conn_sqlsvr,_("Especialidad:"), "especialidad", $especialidad, true, true);
    servicio_list_row_gt($conn_sqlsvr, _("Seleccione un servicio GT: "), 'servicio', $servicio, false, true, $especialidad);
    text_row(_("Número:"), 'numero', null, 4, 4);    
}
text_row(_("Hora cita:"), 'hora_cita', null, 20, 20);
//date_row(_("Hora cita:"), 'hora_cita', null, null, 0, 0, 0, null, false);
text_row(_("Clínica:"), 'clinica', null, 3, 3);

unid_func_list_row(_("Unidad funcional:"), 'codunidfunc', null, false, false);

end_table(1);

submit_add_or_update_center($selected_id == -1, '', true);

end_form();
end_page();

if ($conn_sqlsvr){
   sqlsrv_close($conn_sqlsvr);	
}

?>
