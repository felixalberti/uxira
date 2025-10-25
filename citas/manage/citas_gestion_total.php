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
//include($path_to_root . "/includes/db_pager.inc");
include($path_to_root . "/includes/db_pager_sqlsvr.inc");
include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas/includes/db/tipocita_db.inc");
include_once($path_to_root . "/includes/db/connect_db_sqlserver.inc");
$conn_sqlsvr = conectar_angiosgt();
include_once($path_to_root . "/citas/includes/db/control_sala_db.inc");
include_once($path_to_root . "/citas/includes/db/citas_db.inc");
include_once($path_to_root . "/citas/includes/db/pacientes_db.inc");
include_once($path_to_root . "/citas/includes/db/medico_db.inc");
include_once($path_to_root . "/citas/includes/db/especialidades_db.inc");
include_once($path_to_root . "/citas/includes/db/servicio_db.inc");
//
$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
page(_("Citas gestión total"), false, false, "", $js);


simple_page_mode(true);


//------------------------------------------------------------------------------------------------
if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{
  
	//initialise no input errors assumed initially before we test
	$input_error = 0;
  if (isset($_POST['codmed']) && strlen($_POST['codmed']) == 0) 
	{
		$input_error = 1;
		display_error(_("Seleccione un médico de la lista."));
		set_focus('codmed');
	}
	elseif (isset($_POST['tipocita']) && strlen($_POST['tipocita']) == 0) 
	{
		$input_error = 1;
		display_error(_("El tipo de cita no puede estar vacio."));
		set_focus('tipocita');
	}
	elseif (isset($_POST['ci_paciente']) && strlen($_POST['ci_paciente']) == 0) 
	{
		$input_error = 1;
		display_error(_("Debe indicar la cédula del paciente."));
		set_focus('ci_paciente');
	}
	/*elseif (strlen($_POST['numero']) == 0) 
	{
		$input_error = 1;
		display_error(_("El número no puede estar vacio."));
		set_focus('numero');
	}*/
	/*elseif (strlen($_POST['ci_usuario']) == 0) 
	{
		$input_error = 1;
		display_error(_("La cédula no puede estar vacía."));
		set_focus('ci_usuario');
	}*/
	/*elseif ($selected_id == -1 && !existe_usuario($_POST['ci_usuario'])){
    $input_error = 1;
		display_error(_("El usuario no está registrado."));
		set_focus('ci_usuario');		
  }	
  elseif ($selected_id == -1 && !cita_disponible($_POST['fecha_cita'], $_POST['numero'],$_POST['hora_cita'],$_POST['tipocita'],$_POST['codmed'])){
    $input_error = 1;
		display_error(_("El número o hora no está disponible."));
		set_focus('numero');  	
  }
  else if($selected_id == -1 && yatienecita($_POST['fecha_cita'],$_POST['codmed'],$_POST['ci_usuario'])){	
    $input_error = 1;
		display_error(_("El paciente ya tiene cita para la fecha indicada ".$_POST['fecha_cita']));
		set_focus('fecha_cita');  	
  }*/
	
	
	if ($input_error !=1)
	{
    	if ($selected_id != -1) 
    	{
		  //update_item_numeros($selected_id, $_POST['selected_tipo_cita'], $_POST['selected_numero'], $_POST['hora_cita'], $_POST['clinica'],
		  //$_POST['codunidfunc']);
		  //update_control_sala($_POST['fecha_cita'], $_POST['numero'], $_POST['tipocita'], $_POST['hora_cita'], $_POST['codestatus'], $_POST['observacion']);		
			//upd_users($_POST['ci_usuario'],$_POST['real_name'],$_POST['last_name'],$_POST['phone']);
			update_citas($conn_sqlsvr,$selected_id,$_POST['numero'],$_POST['motivo_consulta'],$_POST['observacion']);
			display_notification(_('El item ha sido actualizado: '));
    	} 
    	else 
    	{
    	//display_notification($_POST['numero']);
		  //add_control_sala($_POST['fecha_cita'], $_POST['numero'], $_POST['hora_cita'], $_POST['ci_usuario'], $_POST['codmed'], $_POST['tipocita'], $_POST['numero'], $_POST['observacion'],$_POST['horallegada']);                         
			//upd_fechasxsemana($_POST['fecha_cita'],$_POST['numero'],$_POST['hora_cita'],$_POST['tipocita'],$_POST['codmed']);		  
			crear_cita_gt($conn_sqlsvr, $_POST['serv_gt'], $_POST['ci_paciente'], $_POST['medico_gt'], 
			$_POST['especialidad_gt'], $_POST['fecha_cita'], $_POST['numero'], 2, $_POST['motivo_consulta'], $_POST['observacion']);
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
    //$param = explode(";", $selected_id);
		//$myrow = get_control_sala($param[0],$param[1]);
		//$_POST['fecha_cita'] = sql2date($myrow["fecha_cita"]);
	  //$_POST['numero'] = $myrow["numero"];	
		//delete_control_sala($_POST['fecha_cita'], $_POST['numero']);
		delete_control_sala_gt($selected_id);
		display_notification(_('El item seleccionado ha sido borrado'));
		  
  //}
	$Mode = 'RESET';
}
//----------------------------------------------------------------------------------
if ($Mode == 'RESET')
{
	$selected_id = -1;
	unset($_POST['fecha_cita']);	
	unset($_POST['numero']);			
  unset($_POST['hora_cita']);		
	unset($_POST['observacion']);
	unset($_POST['codestatus']);
	unset($_POST['ci_usuario']);
	unset($_POST['real_name']);
	unset($_POST['last_name']);
	unset($_POST['phone']);
	unset($_POST['medico_gt']);
	unset($_POST['serv_gt']);
	unset($_POST['ci_paciente']);
	unset($_POST['nombre_pac']);
	unset($_POST['especialidad_gt']);
	unset($_POST['motivo_consulta']);
	unset($_POST['observacion']);
	refresh_pager('doc_tbl');
}
//----------------------------------------------------------------------------------
start_form(false, true);

start_table("class='tablestyle_noborder'");
start_row();

    date_row(_("Fecha cita:"), 'fecha_cita_sel', null, null, 0, 0, 0, null, true);

    /*$param = explode(";", $selected_id);
    if ($selected_id != -1) $codmed = $param['2'];
    else*/
    if (isset($_POST['codmed'])){
    	$codmed = $_POST['codmed'];
    }	
    else $codmed = null;
    //
    if (isset($_POST['medico_gt']) && $_POST['medico_gt']!=''){
    	$codmed_gt = $_POST['medico_gt'];
    	//display_notification('Medico gt '.$codmed_gt);
    	$medico_gt = dame_medico($codmed_gt);
    }	
    else {
    	$codmed_gt = null;      
    	$medico_gt = 0;
    }	
    //
    /*if ($selected_id != -1) $tipocita = $param['3'];
    else*/
    if (isset($_POST['tipocita'])){
    	$tipocita = $_POST['tipocita'];
    }	
    else $tipocita = null; 
    //
    if (isset($_POST['especialidad']))
    $especialidad = $_POST['especialidad'];
    else $especialidad = null;
    //
    if (isset($_POST['servicio_sel_gt']))
    $servicio = $_POST['servicio_sel_gt'];
    else $servicio = null;
    //
    if (isset($_POST['especialidad_gt']) && $_POST['especialidad_gt']!='')
    $especialidad_gt = $_POST['especialidad_gt'];
    else $especialidad_gt = null;
    //
    if (isset($_POST['servicio_gt']) && $_POST['servicio_gt']!=''){
       $servicio_gt= $_POST['servicio_gt'];
       //display_notification("servicio: ".$servicio_gt);
       $tipocita_gt = dame_tipocita($servicio_gt);
    }
    else {
    	$servicio_gt = null;        
    	$tipocita_gt = null;   
    }
    
    if (isset($_POST['serv_gt']) && $_POST['serv_gt']!=''){
       $servicio_gt= $_POST['serv_gt'];
       //display_notification("servicio: ".$servicio_gt);
       $tipocita_gt = dame_tipocita($servicio_gt);
    }
    else {
    	$servicio_gt = null;        
    	$tipocita_gt = null; 
    }
    //display_notification();
    //
    especialidad_list_row_gt($conn_sqlsvr,_("Especialidad:"), "especialidad", $especialidad, true, true); 
   // medico_list_row(_("Seleccione un medico: "), 'medico_sel', $codmed, false, true);
    medico_list_row_gt($conn_sqlsvr, _("Seleccione un medico GT: "), 'medico_sel_gt', $codmed, false, true, $especialidad);
    servicio_list_row_gt($conn_sqlsvr, _("Seleccione un servicio GT: "), 'serv_sel_gt', $servicio, false, true, $especialidad); 
	  //type_patron_list_row(_("Seleccionar un patrón:"), 'selec_tabla', null, null, true);
	  //tipo_cita_list_row(_("Seleccionar un tipo de cita:"), 'tipo_cita_sel', $tipocita, null, true);
    if (isset($_POST['medico_sel']) && (($_POST['medico_sel'] != '')) ) $medico_sel = $_POST['medico_sel'];    
    else $medico_sel = '%';  
   if (isset($_POST['tipo_cita_sel']) && (($_POST['tipo_cita_sel'] != '')) ) $tipo_cita_sel = $_POST['tipo_cita_sel'];    
    else $tipo_cita_sel = '%';  
    //list_especialidad_gt($conn_sqlsvr,"Especialidad"); 
    //echo "<br>"; 

  

end_row();
end_table();
end_form();
//------------------------------------------------------------------------------------------------

function edit_tabla($row){
	  //$link = edit_button_cell2("Edit".$row["fecha_cita"].';'.$row["numero"].';'.$row["codmed"].';'.$row["tipocita"], _("Edit"));
	  $link = edit_button_cell2("Edit".$row["Z11_PLANIFICACION"], _("Edit"));
		return $link;
}	
function borrar_tabla($row){
	  //$link = delete_button_cell2("Delete".$row["fecha_cita"].';'.$row["numero"].';'.$row["codmed"].';'.$row["tipocita"], _("Delete"));
	  $link = delete_button_cell2("Delete".$row["Z11_PLANIFICACION"], _("Delete"));
		return $link;
}	
function obt_especialidad_gt($row){
	  return get_especialidad_gt($row['Z13_ESPECIALIDAD']);
}
function obt_medico_gt($row){
	  return get_medico_gt($row['Z14_PERSONALMEDICO']);
}	
function obt_paciente_gt($row){
	  return get_nomb_paciente_gt($row['Z19_PACIENTE']);
}
function obt_ci_paciente_gt($row){
	  return get_ci_paciente_gt($row['Z19_PACIENTE']);
}	
function obt_servicio_gt($row){
	  return get_servicio_gt($row['F13_PLANTILLA']);
}	
/*function col_hidden($row){
		$_POST['id1'] = $row["id"];
		$link = hidden('id1', $_POST['id1']);
		//$link = text_row(_("Id:"), 'id1', null, 6, 6);
		return $link;
}	*/
//------------------------------------------------------------------------------------------------
//display_notification('Medico sel '.$medico_sel);
//display_notification('Tipo cita sel '.$tipo_cita_sel);
/*if ($medico_sel == "%") {
   $sql = "SELECT st.descripcion, c.numero, c.hora_cita, u1.real_name, u1.last_name, u1.phone, ec.descripcion as desc_estatus, horaestatus, c.tipocita, fecha_cita, c.codmed  FROM ".TB_PREF.
   "cm_citas c left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = c.tipocita LEFT JOIN 0_users u1 ON u1.ci_usuario = c.ci_usuario".
   ", ".TB_PREF."cm_estatuscitas ec";
   if ($tipo_cita_sel != "%") $sql = $sql . " where c.tipocita like '$tipo_cita_sel' and c.codmed like '$medico_sel'".
   " and ec.codigo = c.codestatus and fecha_cita = '".date2sql($_POST['fecha_cita_sel'])."'";  
}  	
else {   	
   $sql = "SELECT st.descripcion, c.numero, c.hora_cita, u1.real_name, u1.last_name, u1.phone, ec.descripcion as desc_estatus, horaestatus, horallegada, c.tipocita, fecha_cita, c.codmed FROM ".TB_PREF.
   "cm_citas c left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = c.tipocita LEFT JOIN 0_users u1 ON u1.ci_usuario = c.ci_usuario".
   ", ".TB_PREF."cm_estatuscitas ec";   	
   $sql = $sql . " where c.tipocita like '$tipo_cita_sel' and c.codmed like '$medico_sel'".
   " and ec.codigo = c.codestatus and fecha_cita = '".date2sql($_POST['fecha_cita_sel'])."'";  
}*/   
//$sql = $sql . " order by c.tipocita";
$sql = "SELECT CONVERT(nvarchar(10), Z11_FECHAPLANIFICACION, 103) AS Z11_FECHAPLANIFICACION, 
CONVERT(nvarchar(30), Z11_HORAPLANIFICACION, 108) AS Z11_HORAPLANIFICACION,
 Z13_ESPECIALIDAD, Z14_PERSONALMEDICO,
 Z19_PACIENTE, Z19_PACIENTE, F13_PLANTILLA, Z11_PLANIFICACION 
FROM Z11_PLANIFICACION WHERE Z11_FECHAPLANIFICACION = '".date2sqlsvr($_POST['fecha_cita_sel'])."' AND Z11_ANULADO = 0";
//display_notification($_POST['fecha_cita_sel'].' - '.$sql);
//------------------------------------------------------------------------------------------------

$cols = array(
	_("Fecha cita") => array('align'=>'center'),	
	_("Hora cita") => array('align'=>'center'),	
	_("Especialidad") => array('align'=>'left','fun'=>'obt_especialidad_gt'),	
	_("Médico") => array('align'=>'left','fun'=>'obt_medico_gt'),
	_("C.I Paciente") => array('align'=>'left','fun'=>'obt_ci_paciente_gt'), 
	_("Paciente") => array('align'=>'left','fun'=>'obt_paciente_gt'), 
	_("Servicio") => array('align'=>'left','fun'=>'obt_servicio_gt'),	
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));

if (get_post('ADD_ITEM'))
  refresh_pager('doc_tbl');

$table =& new_db_pager('doc_tbl', $sql, $cols);
//$table->set_marker('check_overdue', _("Marked items are overdue."));

//if (get_post('selec_tabla') || get_post('tipo_cita_sel')) {
//if (get_post('codmed') || get_post('tipocita') || get_post('medico_sel') || get_post('tipo_cita_sel')){
	$table->set_sql($sql);
	//display_notification($cols);
	$table->set_columns($cols);
//}
$table->width = "80%";
start_form();

/*if (get_post('ADD_ITEM'))
{
	display_notification('aja');
	$Ajax->activate('doc_tbl');
}*/

display_db_pager($table);
//
//echo "Mode: $Mode - id = $selected_id";
echo "<br>";
//if (!isset($_POST['hora_cita'])) $_POST['hora_cita'] = '00:00:00';
if (!isset($_POST['horallegada'])) $_POST['horallegada'] = date("h:i:s");
$numerosxcita = null;
$motivoconsulta  = null;
$observacion = null;
start_table($table_style2);
if ($selected_id != -1) { 	
	if ($Mode == 'Edit') {
    //$param = explode(";", $selected_id);
    //$selected_id = $param['0'];
    $datos = get_datoscita($conn_sqlsvr,$selected_id);
		/*$myrow = get_control_sala($param[0],$param[1]);
		$_POST['fecha_cita'] = sql2date($myrow["fecha_cita"]);
	  $_POST['numero'] = $myrow["numero"];
	  $_POST['ci_usuario'] = $myrow["ci_usuario"];
	  $_POST['hora_cita'] = $myrow["hora_cita"];	  
		$_POST['codmed'] = $myrow["codmed"];
	  $_POST['tipocita']  = $myrow["tipocita"];		
	  $_POST['observacion']  = $myrow["observacion"];		
	  $_POST['codestatus']  = $myrow["codestatus"];
	  $_POST['phone']  = $myrow["phone"];
	  $_POST['real_name']  = $myrow["real_name"];
	  $_POST['last_name']  = $myrow["last_name"];*/

	  $cedula = get_cedula_paciente($conn_sqlsvr,$datos['Z19_PACIENTE']);
	  $_POST['ci_paciente'] = $cedula ;
	  $_POST['nombre_pac'] = get_paciente($conn_sqlsvr,$cedula);
	  //$_POST['ci_paciente'] = $datos['Z13_ESPECIALIDAD'];	
	  $especialidad_gt = $datos['Z13_ESPECIALIDAD'];	
	  $codmed_gt = $datos['Z14_PERSONALMEDICO'];	
	  $servicio_gt = $datos['F13_PLANTILLA'];	
	  $numerosxcita = $datos['Z11_HORAPLANIFICACION'];
	  $medico_gt = dame_medico($codmed_gt);	
	  $tipocita_gt = dame_tipocita($servicio_gt);
	  $_POST['fecha_cita'] = $datos['Z11_FECHAPLANIFICACION'];
	  $motivoconsulta = $datos['Z11_MOTIVOCONSULTA'];
	  $observacion = $datos['Z11_OBSERVACION'];
	  display_notification('Está editando el registro '.$selected_id.' -'.$especialidad_gt.' - '.$cedula. ' - '.$_POST['nombre_pac'].' - '.$_POST['fecha_cita']);
		
		
	}
	hidden('selected_id', $selected_id);
	
	//hidden('id');	
}
//

    especialidad_list_row_gt($conn_sqlsvr,_("Especialidad:"), "especialidad_gt", $especialidad_gt, true, true); 
    medico_list_row_gt($conn_sqlsvr, _("Seleccione un medico GT: "), 'medico_gt', $codmed_gt, false, true, $especialidad_gt);
    servicio_list_row_gt($conn_sqlsvr, _("Seleccione un servicio GT: "), 'serv_gt', $servicio_gt, false, true, $especialidad_gt);

//pacientes_list_cells_gt($conn_sqlsvr, _("Cédula usuario:"), 'ci_usuario2', null, "Nuevo", true);
$nombre_pac = "";
//pacientes_list_cells(_("Cédula usuario:"), 'ci_usuario', null, "Nuevo", true);
/*if (get_post('ci_usuario')){	
	$nombre_pac = get_paciente($conn_sqlsvr,$_POST['ci_paciente']);
} 	  
*/

//text_row(_("C.I paciente:"), 'ci_paciente', null, 18, 18);
//text_row_ex_submit(_("C.I paciente:"), 'ci_paciente', 18, 18, "titulo", null, null, null);
//

  

if ($selected_id == -1) {	  
	text_row_ex_submit(_("C.I paciente:"), 'ci_paciente', 18, 18, "titulo", null, null, null);
	if (list_updated('ci_paciente') || get_post('ci_paciente')){
		//$nombre_pac = get_paciente($conn_sqlsvr,$_POST['ci_paciente']);
		$_POST['nombre_pac'] = get_paciente($conn_sqlsvr,$_POST['ci_paciente']);
		$Ajax->activate('nombre_pac');		  
	}		
	text_row(_("Nomb paciente:"), 'nombre_pac', null, 30, 30);
}
else{
	label_row(_("C.I paciente:"), $_POST['ci_paciente']);
  hidden('ci_paciente', $_POST['ci_paciente']);
  label_row(_("Nomb paciente:"), $_POST['nombre_pac']);
  hidden('nombre_pac', $_POST['nombre_pac']);
  //label_row(_("Motivo consulta:"), $_POST['motivo_consulta']);
  //hidden('motivo_consulta', $_POST['motivo_consulta']);
}
	text_row(_("Motivo consulta:"), 'motivo_consulta', $motivoconsulta, 100, 200);
	text_row(_("Observación:"), 'observacion', $observacion, 120, 200);
//text_row(_("Teléfono:"), 'phone', null, 16, 30);	  
//text_row(_("Nombre:"), 'real_name', null, 16, 30);	  
//text_row(_("Apellido:"), 'last_name', null, 16, 30);



if ($selected_id != -1) {
    label_row(_("Fecha cita:"), $_POST['fecha_cita']);
    hidden('fecha_cita', $_POST['fecha_cita']); 	
	  //label_row(_("Medico:"), get_medico($_POST['codmed'])); 
    //hidden('codmed', $_POST['codmed']);
    //label_row(_("Tipo Cita:"), get_tipocita($_POST['tipocita']));
    //hidden('tipocita', $_POST['tipocita']);   
    //label_row(_("Numero:"), $_POST['numero']);
    //hidden('numero', $_POST['numero']);
     numerosxcita_list_row(_("Número:"), 'numero', $numerosxcita, false, true, 2, $medico_gt, $_POST['fecha_cita'], $tipocita_gt); 
    //estatuscitas_list_row(_("Seleccione un estatus: "), 'codestatus', null, false, false);
    //label_row(_("Hora cita:"), $_POST['hora_cita']);
    //hidden('hora_cita', $_POST['hora_cita']);         
}
else {
    date_row(_("Fecha cita:"), 'fecha_cita', null, null, 0, 0, 0, null, true);	
    //medico_list_row(_("Médico: "), 'codmed', null, false, true);  
    //tipo_cita_list_row(_("Tipo Cita:"), 'tipocita', null, false, true);
    //text_row(_("Número:"), 'numero', null, 4, 4);
    numerosxcita_list_row(_("Número:"), 'numero', null, false, true, 2, $medico_gt, $_POST['fecha_cita'], $tipocita_gt);
		/*if (list_updated('numero')){
			display_notification("Hora");
			$_POST['hora_cita'] = dame_hora_cita($_POST['fecha_cita'],$_POST['numero'],$_POST['tipocita'],$_POST['codmed']);
		}
		else 
		{
			$_POST['hora_cita'] = dame_hora_cita($_POST['fecha_cita'],$_POST['numero'],$_POST['tipocita'],$_POST['codmed']);
		}*/     
    //estatuscitas_list_row(_("Estatus: "), 'codestatus', null, false, false);
    //text_row(_("Hora cita:"), 'hora_cita', null, 20, 20);
    //label_row(_("Hora cita:"), $_POST['hora_cita']);
    //hidden('hora_cita', $_POST['hora_cita']); 
    //numerosxcita_list_row(_("Hora cita:"), 'hora_cita', null, false, false, 2, $_POST['codmed'], $_POST['fecha_cita'], $_POST['tipocita']);
    //date_row(_("Hora cita:"), 'hora_cita', null, null, 0, 0, 0, null, false);             
}
//horacita_list_row_gt($conn_sqlsvr, _("Hora cita: "), "horacita_gt", null, false, false);
//text_row(_("Observación:"), 'observacion', null, 60, 60);
//text_row(_("Hora llegada:"), 'horallegada', null, 8, 8);
/*pacientes_list_cells(_("Cédula usuario:"), 'ci_usuario', null,
	  "Nuevo", true);*/



end_table(1);


 
submit_add_or_update_center($selected_id == -1, '', true);

end_form();
end_page();

function dame_tipocita($serv){	
  $sql = "SELECT codigotipo FROM ".TB_PREF."cm_system_tables where codigotabla = 'TPCITA' and servicio_gt = $serv";
  $result = db_query($sql, "No se pudo retornar la hora");
	$mydata = mysql_fetch_assoc($result);	
	//display_notification("dame_tipocita ".$serv.'-->'.$mydata["codigotipo"]);
	return $mydata["codigotipo"];	  
}

function dame_medico($medico_gt){	
  $sql = "SELECT medico_no FROM ".TB_PREF."medico_master where medico_gt = $medico_gt";
  $result = db_query($sql, "No se pudo retornar la hora");
	$mydata = mysql_fetch_assoc($result);	
	//display_notification("dame_medico ".$medico_gt.'-->'.$mydata["medico_no"]);
	return $mydata["medico_no"];	 
}

function existe_usuario($cedula){
	$sql = "SELECT COUNT(*) FROM ".TB_PREF.
   "users where ci_usuario = '".$cedula."'";
  $result = db_query($sql, "No se pudo retornar el usuario");
	$myrow = db_fetch_row($result);	
	return ($myrow[0] > 0);	  
}	
function cita_disponible($fechacita,$numero,$hora,$tipocita,$codmed){
	$sql = "SELECT COUNT(*) FROM ".TB_PREF.
   "cm_fechasxsemana where fecha = '".date2sql($fechacita)."' and (numero = '".$numero."' and hora = '".$hora."') and tipocita = '".$tipocita."' and codmed = '".$codmed."' and tomado = 0";
  $result = db_query($sql, "No se pudo retornar la disponibilidad del horario");
	$myrow = db_fetch_row($result);	
	//display_notification($sql);
	return ($myrow[0] > 0);	  
}
function yatienecita($fechacita,$codmed,$cedula){
	$sql = "SELECT COUNT(*) FROM ".TB_PREF.
   "cm_citas where fecha_cita = '".date2sql($fechacita)."' and codmed = '$codmed' and ci_usuario = '$cedula'";
  $result = db_query($sql, "No se pudo retornar si el paciente tiene ya una cita");
	$myrow = db_fetch_row($result);	
	return ($myrow[0] > 0);	  
}
function upd_users($cedula,$real_name,$last_name,$phone){
  $sql = "UPDATE ".TB_PREF."users set real_name = '$real_name', last_name = '$last_name', phone = '$phone' where ci_usuario = '".$cedula."'";
  $result = db_query($sql, "No se pudo retornar el usuario");
}
function upd_fechasxsemana($fechacita,$numero,$hora,$tipocita,$codmed){
  $sql = "UPDATE ".TB_PREF."cm_fechasxsemana set tomado = 1 where fecha = '".date2sql($fechacita)."' and numero = '".$numero."' and hora = '".$hora."' and tipocita = '".$tipocita."' and codmed = '".$codmed."'";
  $result = db_query($sql, "No se pudo actualizar las fechas x semana");	
}
function dame_hora_cita($fechacita,$numero,$tipocita,$codmed){
	$sql = "SELECT hora FROM ".TB_PREF.
   "cm_fechasxsemana where fecha = '".date2sql($fechacita)."' and (numero = '".$numero."') and tipocita = '".$tipocita."' and codmed = '".$codmed."' and tomado = 0";
  $result = db_query($sql, "No se pudo retornar la hora");
	$mydata = mysql_fetch_assoc($result);	
	return $mydata["hora"];	  
}	
function actualizar_citas_angiosgt($conn_sqlsvr){
	$tsql = "SELECT TOP 20 Z28_ESTATUSPLANIFICACION, Z14_PERSONALMEDICO, Z19_PACIENTE, Z11_HORAPLANIFICACION, Z11_PRECIO, Z11_MOTIVOCONSULTA FROM Z11_PLANIFICACION";
	$result = sqlsrv_query($conn_sqlsvr,$tsql);
	if ($result){
		$cont = 0;
		while ($row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC)){
			$cont++; 
			//display_notification($row['Z28_ESTATUSPLANIFICACION'].' - '.$row['Z19_PACIENTE'].' - '.$row['Z11_MOTIVOCONSULTA'].' - '.$row['Z11_PRECIO']);
			if ($cont > 20) exit;
	  }		
  }
  if ( $result !== false ) { 
   sqlsrv_free_stmt( $result ); 
  }	
}	

//$conn_sqlsvr = conectar_angiosgt();
if ($conn_sqlsvr){
   //actualizar_citas_angiosgt($conn_sqlsvr);	
   sqlsrv_close($conn_sqlsvr);	
}

?>
