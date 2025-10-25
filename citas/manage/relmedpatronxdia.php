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
$page_security = "SA_MEDICAL_APPOINMENT_RELMEDPATRONBYDAY";
$path_to_root="../..";
include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

//include_once($path_to_root . "/sales/includes/sales_ui.inc");
//include_once($path_to_root . "/sales/includes/sales_db.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas//includes/db/relmedpatronxdia_db.inc");
include_once($path_to_root . "/citas//includes/db/medico_db.inc");


$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
page(_("Relacion medico y patron por dia"), false, false, "", $js);

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
		  update_item_medpatronxdia($selected_id, $_POST['coddia'], $_POST['patron']); 		
			display_notification(_('El item ha sido actualizado: '));
    	} 
    	else 
    	{
		    add_item_medpatronxdia($_POST['codmed'], $_POST['coddia'], $_POST['patron']);                         
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
		delete_item_medpatronxdia($param['0'], $param['1']);
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
echo '<table width="100%" border="0" cellpadding="1">';
echo '<tr>';
echo '<td valign="top" width="54%">';

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
		4 => "Miercoles", 
		5 => "Jueves", 
		6 => "Viernes", 
		7 => "Sabado"	);
    $link = $semanaArray[$dia];					
	  return $link;
}
function dia_sem_cadena($dia){
		$semanaArray = array(
		1 => "Domingo", 
		2 => "Lunes", 
		3 => "Martes", 
		4 => "Miercoles", 
		5 => "Jueves", 
		6 => "Viernes", 
		7 => "Sabado"	);
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
   $sql = "SELECT rmp.codmed, rmp.coddia, rmp.patron, p.descripcion as desc_patron FROM ".TB_PREF.
   "cm_rel_medpatronxdia rmp, ".TB_PREF."cm_patron p";
   if ($selec_tabla != "%") $sql = $sql . " where p.patron = rmp.patron and rmp.codmed like '$selec_tabla'";       
//}  	
/*else {   	
   $sql = "SELECT codmed, coddia, patron FROM ".TB_PREF.
   "cm_rel_medpatronxdia  where codmed like '$selec_tabla'";   	
   $sql = $sql . " and tipocita like '$tipo_cita_sel'"; 
} */  
$sql = $sql . " order by rmp.codmed, rmp.coddia";
display_notification($sql);
//------------------------------------------------------------------------------------------------

$cols = array(
	_("Medico") => Array('align'=>'center','fun'=>'obt_medico'),
	_("Dia") => Array('align'=>'center','fun'=>'dia_semana'),
	_("Cod. patron") => array('align'=>'center'),	
  _("Desc. patron") => array('align'=>'left'),	
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));


$table =& new_db_pager('doc_tbl', $sql, $cols);
//$table->set_marker('check_overdue', _("Marked items are overdue."));

if (get_post('medico_sel')) {
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
		//display_notification($selected_id);
    $param = explode(";", $selected_id);
    $selected_id = $param['0'];
		$myrow = get_item_medicopatronxdia($param['0'],$param['1']);
		$_POST['codmed'] = $myrow["codmed"];		
		$_POST['coddia'] = $myrow["coddia"];
	 	$_POST['patron']  = $myrow["patron"];
	}
	hidden('selected_id', $selected_id);
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
       label_row(_("Día:"), $_POST['desc_dia']);
    }    	 
    //label_row(_("D�a:"), $_POST['coddia']);
    hidden('coddia', $_POST['coddia']);   
    //label_row(_("Numero:"), $_POST['numero']);
    //hidden('numero', $_POST['numero']); 
}
else {
    //tablas_master_list_row(_("Patron:"), 'selec_tabla', null, false);
    //type_patron_list_row(_("Patr�n:"), 'patron', null, false, true);
    medico_list_row(_("Medico: "), 'codmed', null, false, false);    
    //text_row(_("Tipo Cita:"), 'tipocita', null, 3, 3);
    //tipo_cita_list_row(_("D�a:"), 'coddia', null, false, false);
    day_week_list_row(_("Dia:"), 'coddia', null,(!isset($_POST['coddia']) || $selected_id));
    //text_row(_("D�a:"), 'coddia', null, 2, 1);
   
}
//text_row(_("patron:"), 'patron', null, 20, 20);
type_patron_list_row(_("Seleccionar un patron:"), 'patron', null, false, true);
//date_row(_("Hora cita:"), 'hora_cita', null, null, 0, 0, 0, null, false);
//text_row(_("Cl�nica:"), 'clinica', null, 3, 3);
//text_row(_("Unidad funcional:"), 'codunidfunc', null, 3, 3);

 

end_table(1);

submit_add_or_update_center($selected_id == -1, '', true);
echo '</td>';


   if (list_updated('patron')) 
    {
    	  echo '<td valign="top" width="46%">';
    	  echo "<br><br>";
    	  $sql = get_numhor_x_patron($_POST['patron']);
	      //
				$sql = $sql . " order by p.patron, nc.tipo_cita";
				//------------------------------------------------------------------------------------------------
				
				/*$cols = array(
					_("Tipo cita") => array('align'=>'center'),
					_("Desc. tipo cita") => array('align'=>'left'),	
					_("N�mero") => array('align'=>'right'),
					_("Hora cita") => array('align'=>'center'), 
				  _("Unid. Funcional") => array('align'=>'center'));
				
				
				$table =& new_db_pager('doc_tbl2', $sql, $cols);
				//$table->set_marker('check_overdue', _("Marked items are overdue."));
				
				//if (get_post('selec_tabla') || get_post('tipo_cita_sel')) {
					$table->set_sql($sql);
					$table->set_columns($cols);
				//}
				$table->width = "60%";
				
				
				display_db_pager($table);	*/
				
				$result = get_res_numhor_x_patron($_POST['patron']);
				
				start_table($table_style);

	
						$th = array( _("Tipo de cita"), _("N�mero"),
							_("Hora cita"), _("Unid. Funcional"));
					table_header($th);	
					
					$k = 0; //row colour counter
					
					while ($myrow = db_fetch($result)) 
					{
					
						alt_table_row_color($k);
					
						//$last_visit_date = sql2date($myrow["last_visit_date"]);
					
						/*The security_headings array is defined in config.php */
					
						//label_cell($myrow["tipo_cita"]);
						label_cell($myrow["descripcion"]);
						label_cell($myrow["numero"]);
						label_cell($myrow["hora_cita"]);
						label_cell($myrow["codunidfunc"]);
					    
						end_row();
					
					} //END WHILE LIST LOOP
					
					end_table();
				  
				  echo '</td>';   
	     
	     
    }	



end_form();
echo '</tr>';
echo '</table>';
end_page();

function get_numhor_x_patron($patron)
{
$sql = "SELECT nc.tipo_cita, st.descripcion, nc.numero, nc.hora_cita, nc.codunidfunc  FROM ".TB_PREF.
   "cm_numeroscita nc left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = nc.tipo_cita".
   ", ".TB_PREF."cm_patron p where nc.patron = p.patron and nc.patron = '$patron'";
   return $sql;
}

function get_res_numhor_x_patron($patron)
{
  $sql = "SELECT st.descripcion, nc.numero, nc.hora_cita, nc.codunidfunc  FROM ".TB_PREF.
   "cm_numeroscita nc left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = nc.tipo_cita".
   ", ".TB_PREF."cm_patron p where nc.patron = p.patron and nc.patron = '$patron'";
  return db_query($sql,"Las Horas y Tipo de cita x patr�n no puedo ser retornado");
}

?>
