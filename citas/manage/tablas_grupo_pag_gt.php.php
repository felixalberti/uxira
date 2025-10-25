<?php
/**********************************************************************
    Copyright (C) CADAFE, CA.
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

//include_once($path_to_root . "/sales/includes/sales_ui.inc");
//include_once($path_to_root . "/sales/includes/sales_db.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas//includes/db/system_tables_db.inc");
//include_once($path_to_root . "/includes/db/connect_db_sqlserver.inc");
//$conn_sqlsvr = conectar_angiosgt();
//include_once($path_to_root . "/citas/includes/db/servicio_db.inc");


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
		display_error(_("El código tipo no puede estar vacio."));
		set_focus('codigotipo');
	}
	elseif (strlen($_POST['descripcion']) == 0) 
	{
		$input_error = 1;
		display_error(_("La descripción del item no puede estar vacio."));
		set_focus('descripcion');
	}
	
	if ($input_error !=1)
	{
    	if ($selected_id != -1) 
    	{
		    update_item_tablas($selected_id, $_POST['descripcion'], $_POST['comentario'], 
                         $_POST['vermas'], $_POST['titulo_vermas'], $_POST['intervalo_tiempo'], $_POST['abr'], $_POST['serv_gt']);    		
			display_notification(_('El item ha sido actualizado'));
    	} 
    	else 
    	{
		    add_item_tablas($_POST['codigotabla'], $_POST['codigotipo'], $_POST['descripcion'], $_POST['comentario'], 
                         $_POST['vermas'], $_POST['titulo_vermas'], $_POST['intervalo_tiempo'], $_POST['abr'], $_POST['serv_gt']);
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
		delete_item_tablas($selected_id);
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
    
 
    //display_notification($selec_tabla);   
    if (!isset($_POST['selec_tabla']) && isset($_POST['codigotabla']) && $_POST['codigotabla']!='%' ){
    	$tabla = $_POST['codigotabla'];
    	$selec_tabla = $_POST['codigotabla'];
    }	
    else
    {    	
     	$tabla = "%";
     	$selec_tabla = '%';
    }
    
    if (!isset($_POST['codigotabla'])){
	    if (isset($_POST['selec_tabla']) && (($_POST['selec_tabla'] != '%')) ) {
	        $selec_tabla = $_POST['selec_tabla'];
	        $tabla = $_POST['selec_tabla'];    
	    }
	    else {
	    	$selec_tabla = '%';
	    	$tabla = "%";
	    }	
    }
    display_notification('arriba '.$tabla);
    
    if (isset($_POST['especialidad_gt']))
    $especialidad_gt = $_POST['especialidad_gt'];
    else $especialidad_gt = null;
    
     if (isset($_POST['servicio_gt']))
    $servicio_gt= $_POST['servicio_gt'];
    else $servicio_gt = null;    

	  type_tables_list_row(_("Seleccionar un Tipo de Tabla:"), 'selec_tabla', $tabla, null, true);
     
  
    

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
function obt_servicio_gt($row){
	  return get_servicio_gt($row['servicio_gt']);
}	
/*function col_hidden($row){
		$_POST['id1'] = $row["id"];
		$link = hidden('id1', $_POST['id1']);
		//$link = text_row(_("Id:"), 'id1', null, 6, 6);
		return $link;
}	*/
//------------------------------------------------------------------------------------------------
if ($selec_tabla == "%")
   $sql = "SELECT id, codigotabla, codigotipo, descripcion, servicio_gt FROM ".TB_PREF.
   "cm_system_tables 
   order by codigotabla, codigotipo";   	
else   	
   $sql = "SELECT id, codigotabla, codigotipo, descripcion, servicio_gt FROM ".TB_PREF.
   "cm_system_tables where codigotabla like '$selec_tabla' order by codigotabla, codigotipo";   	
//------------------------------------------------------------------------------------------------

$cols = array(
	_("Id") => array('align'=>'right'),
	_("Tabla") => array('align'=>'right'),
	_("Código") => array('align'=>'right'),
	_("Nombre") => array('align'=>'right'),
	_("Servicio GT") => array('align'=>'left'), 
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));
//display_notification($selec_tabla.'-'.$sql);

display_notification($selec_tabla.' - '.$sql);
if ($selec_tabla=='%') kill_session('doc_tbl');
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
start_table($table_style2);
if ($selected_id != -1) { 	
	if ($Mode == 'Edit') {

		$myrow = get_item_tablas($selected_id);		
		$_POST['id'] = $myrow["id"];
	  $_POST['codigotabla'] = $myrow["codigotabla"];
	  $_POST['selec_tabla'] = $myrow["codigotabla"];
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
	hidden('selec_tabla', $_POST['selec_tabla']);		
}	

if ($selected_id != -1) {
	//|| item_grupotabla_used($selected_id) || item_plan_asoc_stock_used($selected_id)){
    label_row(_("Tabla:"), $_POST['codigotabla']);
    hidden('codigotabla', $_POST['codigotabla']);	
    hidden('selec_tabla', $_POST['selec_tabla']);	
    label_row(_("Código:"), $_POST['codigotipo']);
    hidden('codigotipo', $_POST['codigotipo']);	    
}
else {
    tablas_master_list_row(_("Tabla:"), 'codigotabla', null, false);
    text_row(_("Código:"), 'codigotipo', null, 4, 3);
}
text_row(_("Descripción:"), 'descripcion', null, 40, 60);
text_row(_("Comentario:"), 'comentario', null, 60, 200);
text_row(_("Ver Más:"), 'vermas', null, 20, 20);
text_row(_("Titulo Ver Más:"), 'titulo_vermas', null, 30, 30);
text_row(_("Intervalo Tiempo:"), 'intervalo_tiempo', null, 4, 4);
text_row(_("Abr:"), 'abr', null, 10, 10);
//especialidad_list_row_gt($conn_sqlsvr,_("Especialidad:"), "especialidad_gt", $especialidad_gt, false, true);
//servicio_list_row_gt($conn_sqlsvr, _("Seleccione un servicio GT: "), 'serv_gt', $servicio_gt, false, true, $especialidad_gt); 

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

/*if ($conn_sqlsvr){
   sqlsrv_close($conn_sqlsvr);	
}*/

?>
