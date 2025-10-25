<?php
//M�dulo empleados
function empleados_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = empleados_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
	return $str;
}
function empleados_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT empleado_no as item, nombre, empleado_no FROM ".TB_PREF."empleado_master ";

	//$mode = get_company_pref('no_customer_list');
	$mode = get_company_pref('no_employment_list');

return combo_input($name, $selected_id, $sql, 'empleado_no', 'nombre',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todos los Empleados") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione Empleado')
	) );
}
function add_item_actfijos($stock_id, $description, $long_description, $category_id,
 $units, $mb_flag, $serial, $empleado_no, $cuenta, $modelo, $marca, $tipolinea, $estatus )
{
	$sql = "INSERT INTO ".TB_PREF."stock_master (stock_id, description, long_description, category_id,
		units, mb_flag, serial, empleado_no, cuenta, plan, modelo, marca, tipolinea, estatus)
		VALUES (".db_escape($stock_id).", ".db_escape($description).", ".db_escape($long_description).",
		'$category_id', '$units', '$mb_flag', ".db_escape($serial).", ".db_escape($numero).", 
		'$empleado_no', ".db_escape($cuenta).", $modelo, $marca, $tipolinea, $estatus".")";

	db_query($sql, "The item could not be added");

	$sql = "INSERT INTO ".TB_PREF."loc_stock (loc_code, stock_id)
		SELECT ".TB_PREF."locations.loc_code, '$stock_id' FROM ".TB_PREF."locations";

	db_query($sql, "The item locstock could not be added");

	add_item_code($stock_id, $stock_id, $description, $category_id, 1, 0);
}
function update_item_actfijos($stock_id, $description, $long_description, $category_id,
 $serial, $empleado_no, $cuenta,  $modelo, $marca, $tipolinea, $estatus )
{
	$sql = "UPDATE ".TB_PREF."stock_master SET long_description=".db_escape($long_description).",
		description=".db_escape($description).",
		category_id='$category_id',
		serial=".db_escape($serial).",
		empleado_no='$empleado_no',
		cuenta=".db_escape($cuenta).",		
		modelo=$modelo,		
		marca=$marca,		
		tipolinea=$tipolinea,		
		estatus=$estatus										
		WHERE stock_id='$stock_id'";

	db_query($sql, "The item could not be updated");

	update_item_code(-1, $stock_id, $stock_id, $description, $category_id, 1, 0);
}
//Agrega los celulares a la tabla de articulos, incluyendo el plan
function add_item_actfijos_archivo($stock_id, $description, $long_description, $category_id,
 $units, $mb_flag, $serial, $empleado_no, $cuenta, $modelo, $marca, $tipolinea, $estatus, $id_code, $plan, $min_libres )
{
	if (!get_exist_item($stock_id)) {	
		$sql = "INSERT INTO ".TB_PREF."stock_master (stock_id, description, long_description, category_id,
			units, mb_flag, serial, empleado_no, cuenta,  modelo, marca, tipolinea, estatus)
			VALUES (".db_escape($stock_id).", ".db_escape($description).", ".db_escape($long_description).",
			'$category_id', '$units', '$mb_flag', ".db_escape($serial).", ".   
			db_escape($empleado_no).", ".db_escape($cuenta).", $modelo, $marca, $tipolinea, $estatus".")";
		db_query($sql, "The item could not be added");	
  }

	if (!get_exist_asoc_item($stock_id, $plan)) {
		  $sql = "INSERT INTO ".TB_PREF."asoc_item_plan (id_code, stock_id, item_plan, min_libres)
		  VALUES (".db_escape($id_code).", ".db_escape($stock_id).", $plan, $min_libres)";	
			
			db_query($sql, "El item de la tabla asoc_item_plan no pudo ser agregado");
  }

}
function get_exist_item($stock_id)
{
	$sql = "SELECT count(*) as cant
		FROM ".TB_PREF."stock_master
		WHERE stock_id='$stock_id'";
	$result = db_query($sql,"an item could not be retreived");
  $myrow = db_fetch($result);
  if ($myrow['cant'] > 0) return true;
  else return false;
}
function get_exist_asoc_item($stock_id, $plan)
{
	$sql = "SELECT count(*) as cant
		FROM ".TB_PREF."asoc_item_plan
		WHERE stock_id='$stock_id' and item_plan=$plan";
	$result = db_query($sql,"Contando asoc_item_plan no pudo ser retornado");
  $myrow = db_fetch($result);
  if ($myrow['cant'] > 0) return true;
  else return false;
}
function stock_empleado_list_row($label, $name, $selected_id=null, $spec_option)
{
	echo "<tr>\n";
	stock_empleado_list_cells($label, $name, $selected_id, $spec_option);
	echo "</tr>\n";
}
function stock_empleado_list_cells($label, $name, $selected_id=null, $spec_option)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	stock_empleado_list($name, $selected_id, $spec_option);
	echo "</td>\n";
}
function stock_empleado_list($name, $selected_id=null, $spec_option)
{
	$sql = "SELECT empleado_no, nombre FROM ".TB_PREF."empleado_master";
	combo_input($name, $selected_id, $sql, 'empleado_no', 'nombre',
	array('order'=>'empleado_no',	'spec_option' => $spec_option === true ? '' : $spec_option ));
}
//
//
function tablas_list_row($label, $name, $selected_id=null, $spec_option, $tabla=null)
{
	echo "<tr>\n";
	tablas_list_cells($label, $name, $selected_id, $spec_option, $tabla);
	echo "</tr>\n";
}
function tablas_list_cells($label, $name, $selected_id=null, $spec_option, $tabla=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	tablas_list($name, $selected_id, $spec_option, $tabla);
	echo "</td>\n";
}
function tablas_list($name, $selected_id=null, $spec_option, $tabla=null)
{
	$sql = "SELECT id, nombre FROM ".TB_PREF."tablas_grupo WHERE tipotabla like '$tabla'";
	combo_input($name, $selected_id, $sql, 'id', 'nombre',
	array('order'=>'id',	'spec_option' => $spec_option === true ? '' : $spec_option ));
}
//
function tablas_master_list_row($label, $name, $selected_id=null, $spec_option)
{
	echo "<tr>\n";
	tablas_master_list_cells($label, $name, $selected_id, $spec_option);
	echo "</tr>\n";
}
function tablas_master_list_cells($label, $name, $selected_id=null, $spec_option)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	tablas_master_list($name, $selected_id, $spec_option);
	echo "</td>\n";
}
function tablas_master_list($name, $selected_id=null, $spec_option)
{
	$sql = "SELECT codigotabla, descripcion as nombre FROM ".TB_PREF."cm_type_tables";
	combo_input($name, $selected_id, $sql, 'codigotabla', 'nombre',
	array('order'=>'codigotabla',	'spec_option' => $spec_option === true ? '' : $spec_option ));
}
//
function tipotabla_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = tipotabla_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
	return $str;
}
function tipotabla_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT tabla, tabla, nombre FROM ".TB_PREF."tablas_master";

	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	

return combo_input($name, $selected_id, $sql, 'tabla', 'nombre',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todas las Tablas") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione Una Tabla') 
	) );
}

function stock_plan_input_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = stock_plan_input_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
	return $str;
}
function stock_plan_input_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

$sql = "SELECT stock_id, description, stock_id FROM ".TB_PREF."stock_master";
	

	$mode = get_company_pref('no_customer_list');

return combo_input($name, $selected_id, $sql, 'stock_id', 'description',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todas las Tablas") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione un articulo') 
	) );
	
}
//
function tabla_plan_input_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false, $tabla=null)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = tabla_plan_input_list($name, $selected_id, $all_option, $submit_on_change, $tabla);
	echo "</td>\n</tr>\n";
	return $str;
}
function tabla_plan_input_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, $tabla=null)
{
	global $all_items;

	$sql = "SELECT id, nombre, id FROM ".TB_PREF."tablas_grupo WHERE tipotabla='$tabla'";

	$mode = get_company_pref('no_customer_list');

return combo_input($name, $selected_id, $sql, 'id', 'nombre',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todas las Tablas") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione un plan') 
	) );
}
//
//Begin Felix Alberti 12/06/2009
function tablas_maestras_list_row($label, $name, $selected_id=null, $spec_option, $tabla=null)
{
	echo "<tr>\n";
	tablas_maestras_list_cells($label, $name, $selected_id, $spec_option, $tabla);
	echo "</tr>\n";
}
function tablas_maestras_list_cells($label, $name, $selected_id=null, $spec_option, $tabla=null)
{
	if ($label != null)
		echo "<td>$label</td>\n";
	echo "<td>";
	tablas_maestras_list($name, $selected_id, $spec_option, $tabla);
	echo "</td>\n";
}

function tablas_maestras_list($name, $selected_id=null, $spec_option, $tabla=null)
{
	$sql = "SELECT id, descripcion FROM ".TB_PREF.$tabla;
	combo_input($name, $selected_id, $sql, 'id', 'nombre',
	array('order'=>'id',	'spec_option' => $spec_option === true ? '' : $spec_option ));
}
//End Felix Alberti 12/06/2009
//Begin Felix Alberti 31/08/2009
function type_tables_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = typetables_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
	return $str;
}
function typetables_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT codigotabla, codigotabla, descripcion FROM ".TB_PREF."cm_type_tables";

	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	

return combo_input($name, $selected_id, $sql, 'codigotabla', 'descripcion',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todas las Tablas") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione Una Tabla') 
	) );
}
//End Felix Alberti 
//Begin Felix Alberti 23/02/2010
function type_patron_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	echo typepatron_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
}
function typepatron_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT patron, descripcion, patron FROM ".TB_PREF."cm_patron";

	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	

return combo_input($name, $selected_id, $sql, 'patron', 'descripcion',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todos los patrones") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione un patr�n') 
	) );
}
function tipo_cita_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	echo tipocita_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
}
function tipocita_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT codigotipo, descripcion, codigotipo FROM ".TB_PREF."cm_system_tables where codigotabla = 'TPCITA' ";

	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	

return combo_input($name, $selected_id, $sql, 'codigotipo', 'descripcion',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todos los tipos de citas") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione un Tipo de cita') 
	) );
}
/**/
function day_week_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	echo day_week_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
}
function day_week_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT codigo, nombre, codigo FROM ".TB_PREF."cm_tabla_dias ";

	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	

return combo_input($name, $selected_id, $sql, 'codigo', 'nombre',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		//'spec_option' => $spec_option === true ? _("Todos los dias") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione un dia') 
	) );
}
//End Felix Alberti 
//
function medico_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	echo medico_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
}
function medico_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT professional_id, description, address FROM ".TB_PREF."professional ";
        //display_notification($sql);
	$mode = get_company_pref('no_customer_list');

return combo_input($name, $selected_id, $sql, 'professional_id', 'description',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("All Customers") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Select medico')
	) );
}
//
function unid_func_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	echo unid_func_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
}
function unid_func_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT codunifun, descripcion, codunifun FROM ".TB_PREF."cm_unidad_func";

	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	

return combo_input($name, $selected_id, $sql, 'codunifun', 'descripcion',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todos las unidades funcionales") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione una unidad funcional') 
	) );
}
//
function estatuscitas_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	echo estatuscitas_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
}
function estatuscitas_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT codigo, descripcion, codigo FROM ".TB_PREF."cm_estatuscitas";

	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	

return combo_input($name, $selected_id, $sql, 'codigo', 'descripcion',
	array(
	  'order'=>'codigo',
	  'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todos los estatus de citas") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione un estatus de cita') 
	) );
}
//*********************************
function numerosxcita_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false, $numohora, $codmed, $fecha_cita, $tipocita)
{
	echo "<tr><td>$label</td><td nowrap>";
	echo numerosxcita_list($name, $selected_id, $all_option, $submit_on_change, $numohora, $codmed, $fecha_cita, $tipocita);
	echo "</td>\n</tr>\n";
}
function numerosxcita_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false, $numohora, $codmed, $fecha_cita, $tipocita)
{
	global $all_items;

  $fecha_cita = date2sql($fecha_cita);
  if ($numohora == 1) $etiq_numhora = 'numero';
  else $etiq_numhora = 'hora';
	$sql = "SELECT $etiq_numhora, fecha, concat(numero,' - ',hora) as numero FROM ".TB_PREF."cm_fechasxsemana where codmed = '$codmed' and fecha = '$fecha_cita' and tipocita = '$tipocita' and tomado = 0";
	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	
  //display_notification($sql);
return combo_input($name, $selected_id, $sql, $etiq_numhora, 'fecha',
	array(
	  'order'=> 'numero',
	  'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 30,
		'spec_option' => $spec_option === true ? _("Todos los n�meros u horas x citas") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione un numero u horas de cita') 
	) );
}

function list_especialidad_row_gt($conn_sqlsvr,$label,$name)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = list_especialidad_gt($conn_sqlsvr,$label,$name);
	echo "</td>\n</tr>\n";
	return $str;
}

function list_especialidad_gt($conn_sqlsvr,$name){
	
  echo "<select name='$name' size='1' id='$name' onChange=''>";
  $sql = "SELECT Z13_ESPECIALIDAD,Z13_NOMBRE FROM AngiosGTDesa.dbo.Z13_ESPECIALIDAD WHERE Z13_FECHAANULACION IS NULL ORDER BY Z13_NOMBRE;";
	$result = sqlsrv_query($conn_sqlsvr,$sql);
	if ($result){
		while ($row = sqlsrv_fetch_array($result, SQLSRV_FETCH_ASSOC)){
			echo "<option value='".$row['Z13_ESPECIALIDAD']."'>".$row['Z13_NOMBRE']."</option>";
	  }
	  sqlsrv_free_stmt( $result);		
  }	    
  echo "</select>";
}

function medico_list_row_gt($conn_sqlsvr, $label, $name, $selected_id=null, $all_option = false, $submit_on_change=false, $parametro)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = medico_list_gt($conn_sqlsvr, $name, $selected_id, $all_option, $submit_on_change, $parametro);
	echo "</td>\n</tr>\n";
	return $str;
}
function medico_list_gt($conn_sqlsvr, $name, $selected_id=null, $spec_option=false, $submit_on_change=false, $parametro)
{
	global $all_items;

	$sql = "SELECT Z14_PERSONALMEDICO,  CASE Z42_CONSULTORIO WHEN 0 THEN
       Z14_NOMBRE ELSE Z42_NOMBRE + ' - ' + Z14_NOMBRE END AS Z14_NOMBRE
       FROM VZ26_PERSONALMEDICO_ESPECIALIDAD_HORARIO_ACTIVO WHERE
       Z14_ANULADO = 0 AND Z13_ESPECIALIDAD = $parametro AND E10_DIASEMANA IN (-1,2) 
       GROUP BY Z14_PERSONALMEDICO,  Z42_CONSULTORIO, Z42_NOMBRE, Z14_NOMBRE";

	$mode = get_company_pref('no_customer_list');

return combo_input_sqlsvr($conn_sqlsvr, $name, $selected_id, $sql, 'Z14_PERSONALMEDICO', 'Z14_NOMBRE',
	array(
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todos los m�dicos") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Select customer')
	) );
}
//Begin Felix Alberti 11/07/2011
function especialidad_list_row($label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = especialidad_list($name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
	return $str;
}
function especialidad_list($name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT codigo, codigo, nombre FROM ".TB_PREF."cm_especialidad";

	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	

return combo_input($name, $selected_id, $sql, 'codigo', 'nombre',
	array(
	    'format' => '_format_add_curr',
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todas las Tablas") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione Una Tabla') 
	) );
}
//
function especialidad_list_row_gt($conn_sqlsvr,$label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = especialidad_list_gt($conn_sqlsvr,$name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
	return $str;
}
function especialidad_list_gt($conn_sqlsvr,$name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT Z13_ESPECIALIDAD, Z13_NOMBRE FROM Z13_ESPECIALIDAD WHERE Z13_ANULADO = 0";

	//$mode = get_company_pref('no_customer_list');
  $mode = 0;	

return combo_input_sqlsvr($conn_sqlsvr, $name, $selected_id, $sql, 'Z13_ESPECIALIDAD', 'Z13_NOMBRE',
	array(
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todas las Especialidades") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Seleccione Una Tabla') 
	) );
}
function servicio_list_row_gt($conn_sqlsvr, $label, $name, $selected_id=null, $all_option = false, $submit_on_change=false, $parametro)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = servicio_list_gt($conn_sqlsvr, $name, $selected_id, $all_option, $submit_on_change, $parametro);
	echo "</td>\n</tr>\n";
	return $str;
}
function servicio_list_gt($conn_sqlsvr, $name, $selected_id=null, $spec_option=false, $submit_on_change=false, $parametro)
{
	global $all_items;

	$sql = "SELECT F13_PLANTILLA, F13_NOMBRE 
  FROM F13_PLANTILLA INNER JOIN F15_GRUPO 
  ON F13_PLANTILLA.F15_GRUPO = F15_GRUPO.F15_GRUPO 
  WHERE F13_ANULADO = 0 AND F35_TIPOSERVICIO = 2 AND
  F13_PLANTILLA.Z13_ESPECIALIDAD = $parametro";
  //display_notification($sql);
	//$mode = get_company_pref('no_customer_list');
	$mode = 0;	

return combo_input_sqlsvr($conn_sqlsvr, $name, $selected_id, $sql, 'F13_PLANTILLA', 'F13_NOMBRE',
	array(
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("All Customers") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Select customer')
	) );
}
//Paciente
function paciente_list_row_gt($conn_sqlsvr, $label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = paciente_list_gt($conn_sqlsvr, $name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
	return $str;
}
function paciente_list_gt($conn_sqlsvr, $name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT Z19_NROCEDULA, Z19_NOMBRE FROM Z19_PACIENTE WHERE Z19_ANULADO = 0";
  //display_notification($sql);
	//$mode = get_company_pref('no_customer_list');
	$mode = 0;	

return combo_input_sqlsvr($conn_sqlsvr, $name, $selected_id, $sql, 'Z19_NROCEDULA', 'Z19_NOMBRE',
	array(
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("All Customers") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Select customer')
	) );
}
//Hora cita
function horacita_list_row_gt($conn_sqlsvr, $label, $name, $selected_id=null, $all_option = false, $submit_on_change=false)
{
	echo "<tr><td>$label</td><td nowrap>";
	$str = horacita_list_gt($conn_sqlsvr, $name, $selected_id, $all_option, $submit_on_change);
	echo "</td>\n</tr>\n";
	return $str;
}
function horacita_list_gt($conn_sqlsvr, $name, $selected_id=null, $spec_option=false, $submit_on_change=false)
{
	global $all_items;

	$sql = "SELECT Z45_HORA, substring(CONVERT(nvarchar(30), Z45_HORA, 126),12,10) AS Z45_HORA_FORMAT FROM Z45_HORACITA";
  //display_notification($sql);
	//$mode = get_company_pref('no_customer_list');
	$mode = 0;	

return combo_input_sqlsvr($conn_sqlsvr, $name, $selected_id, $sql, 'Z45_HORA', 'Z45_HORA_FORMAT',
	array(
		'search_box' => $mode!=0,
		'type' => 1,
		'size' => 20,
		'spec_option' => $spec_option === true ? _("Todas las horas") : $spec_option,
		'spec_id' => $all_items,
		'select_submit'=> $submit_on_change,
		'edit_submit' => true, // call editor on F4
		'async' => false,
		'sel_hint' => $mode ? _('Press Space tab to filter by name fragment; F4 - entry new customer') :
		_('Selecciona una hora')
	) );
}
//End Felix Alberti 11/07/2011
?>