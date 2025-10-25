<?php
/**********************************************************************
    Copyright (C) Uxira, CA.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
$page_security = "SA_MEDICAL_APPOINMENT_SYSTEMTABLES";
$path_to_root="../..";
include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

//include_once($path_to_root . "/sales/includes/sales_ui.inc");
//include_once($path_to_root . "/sales/includes/sales_db.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas//includes/db/system_tables_db.inc");


$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
page(_("Tablas del Sistema"), false, false, "", $js);

simple_page_mode(true);


//------------------------------------------------------------------------------------------------
if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	//initialise no input errors assumed initially before we test
	$input_error = 0;
  if (strlen($_POST['codigotabla']) == 0) 
	{
		$input_error = 1;
		display_error(_("Seleccione una tabla de la lista."));
		set_focus('codigotabla');
	}
	elseif (strlen($_POST['codigotipo']) == 0) 
	{
		$input_error = 1;
		display_error(_("El c�digo tipo no puede estar vacio."));
		set_focus('codigotipo');
	}
	elseif (strlen($_POST['descripcion']) == 0) 
	{
		$input_error = 1;
		display_error(_("La descripci�n del item no puede estar vacio."));
		set_focus('descripcion');
	}
	
	if ($input_error !=1)
	{
    	if ($selected_id != -1) 
    	{ 
    		$param = explode(";", $selected_id);
		    update_item_tablas($param[0], $_POST['descripcion'], $_POST['comentario'], 
                         $_POST['vermas'], $_POST['titulo_vermas'], $_POST['intervalo_tiempo'], $_POST['abr']);    		
			display_notification(_('El item ha sido actualizado'));
    	} 
    	else 
    	{
		    add_item_tablas($_POST['codigotabla'], $_POST['codigotipo'], $_POST['descripcion'], $_POST['comentario'], 
                         $_POST['vermas'], $_POST['titulo_vermas'], $_POST['intervalo_tiempo'], $_POST['abr'],0);
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
		delete_item_tablas($param[0]);
		display_notification(_('El item seleccionado ha sido borrado'));
		  
  //}
	$Mode = 'RESET';
}
//----------------------------------------------------------------------------------
if ($Mode == 'RESET')
{
	$selected_id = -1;
	//unset($_POST['codigotabla']);	
	unset($_POST['codigotipo']);			
  unset($_POST['descripcion']);	
  unset($_POST['comentario']);		
	unset($_POST['vermas']);		
	unset($_POST['titulo_vermas']);
	unset($_POST['intervalo_tiempo']);
	unset($_POST['abr']);
}
//----------------------------------------------------------------------------------
start_form(false, true);

start_table("class='tablestyle_noborder'");
start_row();
    
    $param = explode(";", $selected_id);
     if ($selected_id != -1) {
     	$id = $param['0'];
      $tabla = $param['1'];
    	}
    elseif (isset($_POST['codigotabla'])){
    	$tabla = $_POST['codigotabla'];
    }	
    else
    {    	
     	$tabla = null;
    	}

	  type_tables_list_row(_("Seleccionar un Tipo de Tabla:"), 'selec_tabla', $tabla, null, true);
    if (isset($_POST['selec_tabla']) && (($_POST['selec_tabla'] != '')) ) $selec_tabla = $_POST['selec_tabla'];    
    else $selec_tabla = '%';  
  

end_row();
end_table();
end_form();
//------------------------------------------------------------------------------------------------

function edit_tabla($row){
	  global $selec_tabla;
	  if ($selec_tabla=='%')
	  $codigotabla =  $selec_tabla;
	  else
	  $codigotabla = $row["codigotabla"];
	  $link = edit_button_cell2("Edit".$row["id"].';'.$codigotabla, _("Edit"));
		return $link;
}	
function borrar_tabla($row){
	  global $selec_tabla;
	  if ($selec_tabla=='%')
	  $codigotabla =  $selec_tabla;
	  else
	  $codigotabla = $row["codigotabla"];
	  $link = delete_button_cell2("Delete".$row["id"].';'.$codigotabla, _("Delete"));
		return $link;
}	
/*function col_hidden($row){
		$_POST['id1'] = $row["id"];
		$link = hidden('id1', $_POST['id1']);
		//$link = text_row(_("Id:"), 'id1', null, 6, 6);
		return $link;
}	*/
//------------------------------------------------------------------------------------------------
if ($selec_tabla == "%")
   $sql = "SELECT id, cst.codigotabla, codigotipo, descripcion FROM ".TB_PREF.
   "cm_system_tables cst
   order by cst.codigotabla, codigotipo";   	
else   	
   $sql = "SELECT id, st.codigotabla, codigotipo, descripcion FROM ".TB_PREF.
   "cm_system_tables st where st.codigotabla like '$selec_tabla' order by st.codigotabla, codigotipo";   	
//------------------------------------------------------------------------------------------------

$cols = array(
	_("Id") => array('align'=>'right'),
	_("Tabla") => array('align'=>'right'),
	_("Codigo") => array('align'=>'right'),
	_("Nombre"), 
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));

//if ($selec_tabla=='%') kill_session('doc_tbl');
$table =& new_db_pager('doc_tbl', $sql, $cols);
//$table->set_marker('check_overdue', _("Marked items are overdue."));

if (get_post('selec_tabla')) {
	$table->set_sql($sql);
	$table->set_columns($cols);
}
$table->width = "50%";
start_form();

display_db_pager($table);
//
//echo "Mode: $Mode - id = $selected_id";
echo "<br>";
start_table(TABLESTYLE2);
if ($selected_id != -1) { 	
	if ($Mode == 'Edit') {
    $param = explode(";", $selected_id);
		$myrow = get_item_tablas($param[0]);		
		$_POST['id'] = $myrow["id"];
	  $_POST['codigotabla'] = $myrow["codigotabla"];
	  $_POST['codigotipo'] = $myrow["codigotipo"];	  
		$_POST['descripcion'] = $myrow["descripcion"];
	  $_POST['comentario']  = $myrow["comentario"];		
    $_POST['vermas']  = $myrow["vermas"];	  
    $_POST['titulo_vermas']  = $myrow["titulo_vermas"];	     
    $_POST['intervalo_tiempo']  = $myrow["intervalo_tiempo"];	    
    $_POST['abr']  = $myrow["abr"];	    
		//$_POST['valor_tarifa']  = price_format($myrow["valor"]);
	}
	hidden('selected_id', $selected_id);
	hidden('id');	
}	

if ($selected_id != -1) {
	//|| item_grupotabla_used($selected_id) || item_plan_asoc_stock_used($selected_id)){
    label_row(_("Tabla:"), $_POST['codigotabla']);
    hidden('codigotabla', $_POST['codigotabla']);	
    label_row(_("Codigo:"), $_POST['codigotipo']);
    hidden('codigotipo', $_POST['codigotipo']);	    
}
else {
    tablas_master_list_row(_("Tabla:"), 'codigotabla', null, false);
    text_row(_("Codigo:"), 'codigotipo', null, 4, 3);
}
text_row(_("Descripcion:"), 'descripcion', null, 40, 60);
text_row(_("Comentario:"), 'comentario', null, 60, 200);
text_row(_("Ver Mas:"), 'vermas', null, 20, 20);
text_row(_("Titulo Ver Mas:"), 'titulo_vermas', null, 30, 30);
text_row(_("Intervalo Tiempo:"), 'intervalo_tiempo', null, 4, 4);
text_row(_("Abr:"), 'abr', null, 10, 10);

end_table(1);

submit_add_or_update_center($selected_id == -1, '', true);

end_form();
end_page();

function item_grupotabla_used($selected_id) {
	$sql= "SELECT COUNT(*) FROM ".TB_PREF."stock_master WHERE 
	 modelo=$selected_id or estatus=$selected_id or tipolinea=$selected_id or
	  marca=$selected_id";
	$result = db_query($sql, "could not query stock master");
	$myrow = db_fetch_row($result);	
	return ($myrow[0] > 0);	
}
function item_plan_asoc_stock_used($selected_id) {
	$sql= "SELECT COUNT(*) FROM ".TB_PREF."asoc_item_plan WHERE 
	 item_plan=$selected_id";
	$result = db_query($sql, "could not query stock master");
	$myrow = db_fetch_row($result);	
	return ($myrow[0] > 0);	
}
?>
