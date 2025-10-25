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
$page_security = 1;
$path_to_root="../..";
//include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/includes/db/connect_db_sqlserver.inc");
$conn_sqlsvr = conectar_angiosgt();
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/ui_list_citas.inc");
//include_once($path_to_root . "/citas//includes/db/maestropatron_db.inc");


$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(900, 500);
if ($use_date_picker)
	$js .= get_js_date_picker();
	
$date = "";
$error = "";
$verif = 0; 
if (isset($_GET['error'])) {
   $error = $_GET['error'];
}
if (isset($_GET['coderror']) && isset($_GET['msj'])) {
	 if ($_GET['coderror'] == 0 )
      display_notification($_GET['msj']);    
   else
      display_error($_GET['msj']);   
}	


if (isset($_POST['especialidad']))
    $especialidad = $_POST['especialidad'];
else $especialidad = null;

if (isset($_POST['medico']))
    $codmed = $_POST['medico'];
else $codmed = null;

if (isset($_POST['tipocita']))
    $servicio = $_POST['tipocita'];
else $servicio = null;

if (isset($_POST['fecha_pago'])){
	$date = $_POST['fecha_pago'];	
}	

//



//page(_("Generar Calendarios gestión total"));
page(_("Generar Calendarios gestión total"), false, false, "", $js);

simple_page_mode(true);

start_form(false, true);

start_table("class='tablestyle_noborder'");
start_row();   
 especialidad_list_row_gt($conn_sqlsvr,_("Especialidad:"), "especialidad", $especialidad, true, true);
 //date_row(_("Fecha inicial:"), 'fecha_ini', '', null, 0, 0, 1001, null, true);
  
end_row();
end_table();
end_form();
//start_form(true);
echo '<form action="generar_calendario_gt.php" method="POST">';
div_start('details');
start_table($table_style2);
table_section_title(_("Calendarios"));
$name = "fecha_pago";
echo '<tr>'.'<td align="left">Grabar Fechas:</td>';
echo '<td colspan="2"><input name="generarfecha" type="checkbox" id="generarfecha" value="1" /></td></tr>';
echo '<tr>'.'<td align="right">A&ntilde;o:</td>'.
                          '<td><select name="year" class="style2">'.
                          '<option value="1999">1999</option>'.
                          '<option value="2000">2000</option>'.
                          '<option value="2001">2001</option>'.
                          '<option value="2002">2002</option>'.
                          '<option value="2003">2003</option>'.
                          '<option value="2004">2004</option>'.
                          '<option value="2005">2005</option>'.
                          '<option value="2006">2006</option>'.
                          '<option value="2007">2007</option>'.
                          '<option value="2008">2008</option>'.
                          '<option value="2009">2009</option>'.
                          '<option value="2010">2010</option>'.
                          '<option value="2011" selected="selected">2011</option>'.
                          '<option value="2012">2012</option>'.
                          '<option value="2013">2013</option>'.
                          '<option value="2014">2014</option>'.
                          '<option value="2015">2015</option>'.
                          '</select></td>'.
                          '</tr>';
$st = '<tr><td>Fecha Calendario:</td>'.'<td>'."<input type='text' name='$name' value='$date'>";
if ($use_date_picker)
	$st .= "<a href=\"javascript:date_picker(document.forms[1].$name);\">"
. "	<img src='$path_to_root/themes/default/images/cal.gif' width='16' height='16' border='0' alt='"._('Click Here to Pick up the date')."'></a>\n";
echo $st.'</td></tr>' ;

/*if (isset($fechaini)){
	$date = $fechaini;
	//echo '<tr><td></td>'."<input type='text' name='$name' value='$date'>"-'</tr>';
	$st = '<tr><td>Fecha Calendario:</td>'.'<td>'."<input type='text' name='$name' value='$date'>";
  echo $st.'</td></tr>' ;
}*/
//medico_list_row(_("Seleccione un medico: "), 'medico', null, false, false);
//tipo_cita_list_row(_("Seleccionar un tipo de cita:"), 'tipocita', null, null, false);
medico_list_row_gt($conn_sqlsvr, _("Seleccione un medico GT: "), 'medico', $codmed, false, true, $especialidad);
servicio_list_row_gt($conn_sqlsvr, _("Seleccione un servicio GT: "), 'tipocita', $servicio, false, true, $especialidad); 
echo '<tr><td colspan="2" align="center"><input type="submit" value="Generar" /></td></tr>';
//if ($error != "" )  label_row(_(" "), $error);

end_table(1);

div_end();
//div_start('controls');
//submit_add_or_update_center($selected_id == -1, '', true);
//div_end();
//end_form();
echo '</form>';

		  	
echo "<div align='center'><a href='../index.php?application=citas'>" . _("Back") . "</a></div><br>";		  	


end_page(false,true);

if ($conn_sqlsvr){
   sqlsrv_close($conn_sqlsvr);	
}
?>
