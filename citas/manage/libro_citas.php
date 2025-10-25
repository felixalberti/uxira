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
$page_security = 'SA_MEDICAL_APPOINMENT_BOOK';
$path_to_root="../..";
include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/reporting/includes/reporting.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
include_once($path_to_root . "/citas/includes/db/medico_db.inc");
include_once($path_to_root . "/citas/includes/db/tipocita_db.inc");
include_once($path_to_root . "/citas/includes/db/control_sala_db.inc");


//
$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
page(_("Medical appointment book"), false, false, "", $js);


simple_page_mode(true);


//------------------------------------------------------------------------------------------------
if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{
  
	//initialise no input errors assumed initially before we test
	$input_error = 0;
  if (strlen($_POST['codmed']) == 0) 
	{
		$input_error = 1;
		display_error(_("Seleccione un medico de la lista."));
		set_focus('codmed');
	}
	elseif (strlen($_POST['tipocita']) == 0) 
	{
		$input_error = 1;
		display_error(_("El tipo de cita no puede estar vacio."));
		set_focus('tipocita');
	}
	elseif (strlen($_POST['numero']) == 0) 
	{
		$input_error = 1;
		display_error(_("El numero no puede estar vacio."));
		set_focus('numero');
	}
	elseif (strlen($_POST['ci_usuario']) == 0) 
	{
		$input_error = 1;
		display_error(_("La cedula no puede estar vacia."));
		set_focus('ci_usuario');
	}
	elseif ($selected_id == -1 && !existe_usuario($_POST['ci_usuario'])){
                //$input_error = 1;
		display_warning(_("El paciente no esta registrado."));
		//set_focus('ci_usuario');		
  }	
  elseif ($selected_id == -1 && !cita_disponible($_POST['fecha_cita'], $_POST['numero'],$_POST['hora_cita'],$_POST['tipocita'],$_POST['codmed'])){
                $input_error = 1;
		display_error(_("El numero o hora no esta disponible."));
		set_focus('numero');  	
  }
  else if($selected_id == -1 && yatienecita($_POST['fecha_cita'],$_POST['codmed'],$_POST['ci_usuario'])){	
                $input_error = 1;
		display_error(_("El paciente ya tiene cita para la fecha indicada ".$_POST['fecha_cita']));
		set_focus('fecha_cita');  	
  }
	
	
	if ($input_error !=1)
	{
            if ($selected_id != -1) 
            {
                      //update_item_numeros($selected_id, $_POST['selected_tipo_cita'], $_POST['selected_numero'], $_POST['hora_cita'], $_POST['clinica'],
                      //$_POST['codunidfunc']);
                      update_control_sala($_POST['fecha_cita'], $_POST['numero'], $_POST['tipocita'], $_POST['hora_cita'],
                              $_POST['codestatus'], $_POST['observacion'], $_POST['real_name'], $_POST['last_name'],
                              $_POST['phone'], $_POST['tecnico'], $_POST['serial_invoice'], $_POST['amount']);		
                            //upd_users($_POST['ci_usuario'],$_POST['real_name'],$_POST['last_name'],$_POST['phone']);
                            display_notification(_('El item ha sido actualizado'));
            } 
            else 
            {
                      add_control_sala($_POST['fecha_cita'], $_POST['numero'], $_POST['hora_cita'], $_POST['ci_usuario'], $_POST['real_name'], $_POST['last_name'], $_POST['phone'], $_POST['codmed'], $_POST['tipocita'], $_POST['numero'], $_POST['observacion'],$_POST['horallegada'], $_POST['tecnico'], $_POST['serial_invoice'], $_POST['amount']);                         
                            upd_fechasxsemana($_POST['fecha_cita'],$_POST['numero'],$_POST['hora_cita'],$_POST['tipocita'],$_POST['codmed']);		  
                            display_notification(_('El nuevo item ha sido anadido'));
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
		$myrow = get_control_sala($param[0],$param[1]);
		$_POST['fecha_cita'] = sql2date($myrow["fecha_cita"]);
	  $_POST['numero'] = $myrow["numero"];	
		delete_control_sala($_POST['fecha_cita'], $_POST['numero']);
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
}

//----------------------------------------------------------------------------------
start_form(false, true);

start_table("class='tablestyle_noborder'");
start_row();

    date_row(_("Fecha cita:"), 'fecha_cita_sel', null, null, 0, 0, 0, null, true);

    $param = explode(";", $selected_id);
    if ($selected_id != -1) $codmed = $param['2'];
    elseif (isset($_POST['codmed'])){
    	$codmed = $_POST['codmed'];
    }	
    else $codmed = null; 
    //
    if ($selected_id != -1) $tipocita = $param['3'];
    /*elseif (isset($_POST['tipocita'])){
    	$tipocita = $_POST['tipocita'];
    }*/	
    else $tipocita = null; 
    // 
    medico_list_row(_("Seleccione un medico: "), 'medico_sel', $codmed, false, true);
	  //type_patron_list_row(_("Seleccionar un patr�n:"), 'selec_tabla', null, null, true);
	  tipo_cita_list_row(_("Seleccionar un tipo de cita:"), 'tipo_cita_sel', $tipocita, null, true);
          
          
    if (isset($_POST['medico_sel']) && (($_POST['medico_sel'] != '')) ) $medico_sel = $_POST['medico_sel'];    
    else $medico_sel = '%';  
    if (isset($_POST['tipo_cita_sel']) && (($_POST['tipo_cita_sel'] != '')) ) $tipo_cita_sel = $_POST['tipo_cita_sel'];    
    else $tipo_cita_sel = '%';     
  

end_row();
end_table();
end_form();
//------------------------------------------------------------------------------------------------

function dame_datos_paciente($row){
         $datos = get_data_paciente($row['ci_usuario']);
         return $datos['phone'];
}
function edit_tabla($row){
	  $link = edit_button_cell2("Edit".$row["fecha_cita"].';'.$row["numero"].';'.$row["codmed"].';'.$row["tipocita"], _("Edit"));
		return $link;
}	
function borrar_tabla($row){
	  $link = delete_button_cell2("Delete".$row["fecha_cita"].';'.$row["numero"].';'.$row["codmed"].';'.$row["tipocita"], _("Delete"));
		return $link;
}
/*function col_hidden($row){
		$_POST['id1'] = $row["id"];
		$link = hidden('id1', $_POST['id1']);
		//$link = text_row(_("Id:"), 'id1', null, 6, 6);
		return $link;
}	*/
function prt_link($row)
{  	
//    return print_document_link($row['trans_no']."-".ST_MEDICAL_APPOINTMENT, _("Print"), true, $row['type'], ICON_PRINT);
}
//------------------------------------------------------------------------------------------------
//display_notification('Medico sel '.$medico_sel);
//display_notification('Tipo cita sel '.$tipo_cita_sel);

if (list_updated('tipocita')) {
   $sql = "SELECT st.descripcion, c.numero, c.hora_cita, c.ci_usuario, c.name, c.name2, c.phone, ec.descripcion as desc_estatus, horaestatus, horallegada, fechora_reg, c.tipocita, fecha_cita, c.codmed, c.ci_usuario  FROM ".TB_PREF.
   "cm_citas c left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = c.tipocita".
   ", ".TB_PREF."cm_estatuscitas ec where fecha_cita = '".date2sql($_POST['fecha_cita_sel'])."' and ec.codigo = c.codestatus";
   if ($tipo_cita_sel != "%") $sql = $sql . " and c.tipocita like '$tipo_cita_sel' and c.codmed like '$medico_sel'";  
}
else
if ($medico_sel == "%") {
   $sql = "SELECT st.descripcion, c.numero, c.hora_cita, c.ci_usuario, c.name, c.name2, c.phone, ec.descripcion as desc_estatus, horaestatus, horallegada, fechora_reg, c.tipocita, fecha_cita, c.codmed, c.ci_usuario  FROM ".TB_PREF.
   "cm_citas c left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = c.tipocita".
   ", ".TB_PREF."cm_estatuscitas ec where fecha_cita = '".date2sql($_POST['fecha_cita_sel'])."' and ec.codigo = c.codestatus";
   if ($tipo_cita_sel != "%") $sql = $sql . " and c.tipocita like '$tipo_cita_sel' and c.codmed like '$medico_sel'";  
}  	
else {   	
   $sql = "SELECT st.descripcion, c.numero, c.hora_cita, c.ci_usuario, c.name, c.name2, c.phone, ec.descripcion as desc_estatus, horaestatus, horallegada, fechora_reg, c.tipocita, fecha_cita, c.codmed, c.ci_usuario FROM ".TB_PREF.
   "cm_citas c left join ".TB_PREF."cm_system_tables st on st.codigotabla = 'TPCITA' and st.codigotipo = c.tipocita".
   ", ".TB_PREF."cm_estatuscitas ec";   	
   $sql = $sql . " where c.tipocita like '$tipo_cita_sel' and c.codmed like '$medico_sel'".
   " and ec.codigo = c.codestatus and fecha_cita = '".date2sql($_POST['fecha_cita_sel'])."'";  
}   
$sql = $sql . " order by c.tipocita";
//display_notification($sql);
//------------------------------------------------------------------------------------------------
if (get_post('ADD_ITEM'))
{
	$Ajax->activate('doc_tbl');
}


$cols = array(
	_("Tipo cita") => array('align'=>'left'),	
	_("Numero") => array('align'=>'center'),
	_("Hora cita") => array('align'=>'center'), 
        _("Cedula") => array('align'=>'right'), 
	_("Nombre") => array('align'=>'left'), 
	_("Apellido") => array('align'=>'left'),
	_("Telefono") => array('align'=>'right'), 	 
	_("Estatus") => array('align'=>'center'), 
	_("Hora estatus") => array('align'=>'center'), 
	_("Hora llegada") => array('align'=>'center'),
        _("Fecha Reg.") => array('align'=>'center'),
	Array('insert'=>true, 'fun'=>'edit_tabla'), 
	Array('insert'=>true, 'fun'=>'borrar_tabla'));


$table =& best_new_db_pager('doc_tbl', $sql, $cols);
//$table->set_marker('check_overdue', _("Marked items are overdue."));

//if (get_post('selec_tabla') || get_post('tipo_cita_sel')) {
//if (get_post('codmed') || get_post('tipocita') || get_post('medico_sel') || get_post('tipo_cita_sel')){
	$table->set_sql($sql);
	//display_notification($cols);
	$table->set_columns($cols);
//}
$table->width = "80%";
start_form();

display_db_pager($table);
//
echo "<br>";
//
 start_outer_table(TABLESTYLE, 5);

    table_section(1);
    
    table_section_title(_("Impresion Libro de Citas"));
    echo "<tr>";
    
    echo "<td>".book_appoinment_link("Prefactura",$_POST['fecha_cita_sel'])."</td>";
    
    echo "</tr>";

 end_outer_table(1);    
//
//echo "Mode: $Mode - id = $selected_id";
echo "<br>";
//if (!isset($_POST['hora_cita'])) $_POST['hora_cita'] = '00:00:00';
$datehoy = new DateTime('now', new DateTimeZone(AMERICA_CARACAS));
$time = $datehoy->format('h:i:s');
if (!isset($_POST['horallegada'])) $_POST['horallegada'] = $time;
start_table(TABLESTYLE2);
if ($selected_id != -1) { 	
	if ($Mode == 'Edit') {
    $param = explode(";", $selected_id);
    //$selected_id = $param['0'];
		$myrow = get_control_sala($param[0],$param[1]);
		$_POST['fecha_cita'] = sql2date($myrow["fecha_cita"]);
	  $_POST['numero'] = $myrow["numero"];          
	  $_POST['ci_usuario'] = $myrow["ci_usuario"];          
	  $_POST['hora_cita'] = $myrow["hora_cita"];	  
		$_POST['codmed'] = $myrow["codmed"];
	  $_POST['tipocita']  = $myrow["tipocita"];		
	  $_POST['observacion']  = $myrow["observacion"];		
	  $_POST['codestatus']  = $myrow["codestatus"];
          
          $myrow2 =get_data_paciente($_POST['ci_usuario']);
          if (isset($myrow2["phone"])) {
            $_POST['phone']  = $myrow2["phone"];
            $_POST['real_name']  = $myrow2["name"];
            $_POST['last_name'] = $myrow2["name2"];
          }
          else {
            $_POST['phone']  = $myrow["phone"];
            $_POST['real_name']  = $myrow["name"];
            $_POST['last_name'] = $myrow["name2"];
          }
	  
	}
	hidden('selected_id', $selected_id);
	
	//hidden('id');	
}	


customer_list_row(_("Customer:"), 'customer_id', null, false, true, false, false);


if (list_updated('customer_id') || (isset($_POST['ci_usuario'])) && !list_updated('customer_id')){
    
    if (list_updated('customer_id'))
    $ci_usuario = $_POST['customer_id'];
    else {
        $datos = get_datos_paciente($_POST['ci_usuario']);
        $ci_usuario = $datos['debtor_no'];
    } 
    
	//$Ajax->activate('_page_body');
	/*$sql = "SELECT * FROM ".TB_PREF.
   "debtors_master where tax_id = '".$_POST['ci_usuario']."'";*/
        $sql = "SELECT t.*, p.*, r.id as contact_id, dt.tax_id 
        FROM 0_debtors_master dt, 0_crm_persons p,0_crm_categories t, 0_crm_contacts r 
        WHERE dt.debtor_no = ". db_escape($ci_usuario)." and
        r.type=t.type AND
        r.action=t.action AND
        r.person_id=p.id AND
        t.type='customer' AND
        r.entity_id=dt.debtor_no limit 1";        
        //display_notification($sql);
  $result = db_query($sql,"Fallo la consulta en la tabla de usuario");
  $myrow = db_fetch($result);
  
        if (isset($myrow["phone"])) {
            $_POST['phone'] = $myrow["phone"];
            $_POST['real_name'] = $myrow["name"];
            $_POST['last_name'] = $myrow["name2"];
            $_POST['ci_usuario'] = $myrow["tax_id"];
        }    
        $Ajax->activate('ci_usuario');
	$Ajax->activate('phone');
	$Ajax->activate('real_name');
	$Ajax->activate('last_name');
	
  
} 	

text_row(_("Cedula:"), 'ci_usuario', null, 12, 12);
text_row(_("Nombre:"), 'real_name', null, 16, 30);	  
text_row(_("Apellido:"), 'last_name', null, 16, 30);
text_row(_("Telefono:"), 'phone', null, 16, 30);


if ($selected_id != -1) {
    label_row(_("Fecha cita:"), $_POST['fecha_cita']);
    hidden('fecha_cita', $_POST['fecha_cita']); 	
	  label_row(_("Medico:"), get_medico($_POST['codmed'])); 
    hidden('codmed', $_POST['codmed']);
    label_row(_("Tipo Cita:"), get_tipocita($_POST['tipocita']));
    hidden('tipocita', $_POST['tipocita']);   
    label_row(_("Numero:"), $_POST['numero']);
    hidden('numero', $_POST['numero']); 
    estatuscitas_list_row(_("Seleccione un estatus: "), 'codestatus', null, false, false);
    label_row(_("Hora cita:"), $_POST['hora_cita']);
    hidden('hora_cita', $_POST['hora_cita']);         
}
else {
    date_row(_("Fecha cita:"), 'fecha_cita', null, null, 0, 0, 0, null, true);	
    medico_list_row(_("Medico: "), 'codmed', null, false, true);  
    tipo_cita_list_row(_("Tipo Cita:"), 'tipocita', null, false, true);
    //text_row(_("N�mero:"), 'numero', null, 4, 4);
    numerosxcita_list_row(_("Numero:"), 'numero', null, false, true, 1, $_POST['codmed'], $_POST['fecha_cita'], $_POST['tipocita']);
		if (list_updated('numero')){
			display_notification("Hora");
			$_POST['hora_cita'] = dame_hora_cita($_POST['fecha_cita'],$_POST['numero'],$_POST['tipocita'],$_POST['codmed']);
		}
		else 
		{
			$_POST['hora_cita'] = dame_hora_cita($_POST['fecha_cita'],$_POST['numero'],$_POST['tipocita'],$_POST['codmed']);
		}     
    estatuscitas_list_row(_("Estatus: "), 'codestatus', null, false, false);
    //text_row(_("Hora cita:"), 'hora_cita', null, 20, 20);
    label_row(_("Hora cita:"), $_POST['hora_cita']);
    hidden('hora_cita', $_POST['hora_cita']); 
    //numerosxcita_list_row(_("Hora cita:"), 'hora_cita', null, false, false, 2, $_POST['codmed'], $_POST['fecha_cita'], $_POST['tipocita']);
    //date_row(_("Hora cita:"), 'hora_cita', null, null, 0, 0, 0, null, false);             
}
text_row(_("Observacion:"), 'observacion', null, 60, 60);
text_row(_("Hora llegada:"), 'horallegada', null, 8, 8);
text_row(_("Tecnico:"), 'tecnico', null, 30, 50);
text_row(_("Factura:"), 'serial_invoice', null, 20, 20);
amount_row(_("Amount:"), 'amount', null, null, null, 2);
/*pacientes_list_cells(_("C�dula usuario:"), 'ci_usuario', null,
	  "Nuevo", true);*/



end_table(1);


 
submit_add_or_update_center($selected_id == -1, '', true);

end_form();
end_page();

function get_datos_paciente($tax_id){
	$sql = "SELECT * FROM ".TB_PREF.
   "debtors_master where tax_id = ".db_escape($tax_id);

  $result = db_query($sql, "No se pudo retornar el paciente");
	$myrow = db_fetch($result);	
	return $myrow;	  
}

function existe_usuario($cedula){
	$sql = "SELECT COUNT(*) FROM ".TB_PREF.
   "debtors_master where tax_id = ".db_escape($cedula);
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
	$mydata = db_fetch_assoc($result);	
	return $mydata["hora"];	  
}

function get_data_paciente($cedula)
{    
    $sql = "SELECT p.name, p.name2, p.phone, p.phone2, d.debtor_no FROM ".TB_PREF."debtors_master d ".
                            "LEFT JOIN ". TB_PREF."cust_branch b ON (d.debtor_no = b.debtor_no) ".
                            "LEFT JOIN ".TB_PREF."crm_contacts c
                                       ON c.entity_id=b.branch_code AND c.type='cust_branch' AND c.action='general'
                            LEFT JOIN ".TB_PREF."crm_persons p ".
                            "ON c.person_id=p.id ".                 

             "where d.cedula = '$cedula'";
     //display_notification($sql);
     $result = db_query($sql,"The cedula in debtors_master could not be found");
     $myrow = db_fetch_assoc($result);
     if ($myrow)
     return $myrow;
     else
     return null;    
}

/*Begin Felix Alberti 04/09/2018*/
function book_appoinment_link($fecha_cita)
{	
	return  get_book_appointment_view_str('Libro citas',$fecha_cita,8,120);
}
/*End Felix Alberti 04/09/2018*/


?>
