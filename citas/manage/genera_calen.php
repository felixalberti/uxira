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
$page_security = "SA_MEDICAL_APPOINMENT_PROCESSCALENDARY";
$path_to_root="../..";
//include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

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

if (isset($_POST['ADD_ITEM'])) {
    display_notification('si');
    include_once($path_to_root . "/citas/manage/genera_calen2.php");
    /*
    // url a la que se llamara
		$url = "http://localhost/uxira/citas/manage/genera_calen2.php";
                
		// parametros que se enviaran
                global $db;
                $_SESSION['database'] = $db;
		$params = array( 'medico'=>100,'tipocita'=>1);
		// inicializamos la libreria cURL
		$ch = curl_init();
		// indicamos la URL
		curl_setopt( $ch, CURLOPT_URL, $url );
		// que no retorne las cabeceras en la respuesta
		curl_setopt( $ch, CURLOPT_HEADER, false );
		// indicamos que utilizaremos Headers		
		// indicamos que utilizaremos POST
		curl_setopt( $ch, CURLOPT_POST, true );
		// indicamos los parametros
		curl_setopt( $ch, CURLOPT_POSTFIELDS, $params );
		curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );		
		// ejecutamos la peticion
		$respuestacobro=curl_exec( $ch);
                $httpCode   = curl_getinfo($ch, CURLINFO_HTTP_CODE);                
                //print curl_error($ch);                
		curl_close($ch);		
                
                
		//Valido si fue o no efectivo el Cobro
		//$cobro_status=explode("-",$respuestacobro);
                
                //display_notification('resp = '.$cobro_status[0].'---'.$cobro_status[1]);
                //display_notification('resp = '.$httpCode);
                
                
                if ( $httpCode != 200 ){
                    display_notification( "Return code is {$httpCode} \n"
                        .curl_error($ch));
                } else {
                    display_notification("<pre>".htmlspecialchars($respuestacobro)."</pre>");
                }*/
                
    
    //$html_brand = "www.google.com";

    /*$html_brand="http://localhost/uxira/citas/manage/genera_calen2.php";
$ch = curl_init();

$options = array(
    CURLOPT_URL            => $html_brand,
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_HEADER         => true,
    CURLOPT_FOLLOWLOCATION => true,
    CURLOPT_ENCODING       => "",
    CURLOPT_AUTOREFERER    => true,
    CURLOPT_CONNECTTIMEOUT => 120,
    CURLOPT_TIMEOUT        => 120,
    CURLOPT_MAXREDIRS      => 10,
);
curl_setopt_array( $ch, $options );
$response = curl_exec($ch); 
$httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);

if ( $httpCode != 200 ){
    display_notification( "Return code is {$httpCode} \n"
        .curl_error($ch));
} else {
    display_notification("<pre>".htmlspecialchars($response)."</pre>");
}

curl_close($ch);*/

}                
//
simple_page_mode(true);



page(_("Generar Calendarios"));
start_form(true);
//echo '<form action="generar_calendario.php" method="POST">';
div_start('details');
start_table(TABLESTYLE2);
table_section_title(_("Calendarios"));
$name = "fecha_pago";
echo '<tr>'.'<td align="left">Grabar Fechas:</td>';
echo '<td colspan="2"><input name="generarfecha" type="checkbox" id="generarfecha" value="1" /></td></tr>';
echo '<tr>'.'<td align="right">A&ntilde;o:</td>'.
                          '<td><select name="year" class="style2">'.
                          '<option value="2018" selected="selected">2018</option>'.
                          '<option value="2019">2019</option>'.
                          '<option value="2020">2020</option>'.
                          '<option value="2021">2021</option>'.
                          '<option value="2022">2022</option>'.
                          '</select></td>'.
                          '</tr>';
$st = '<tr><td>Fecha Calendario:</td>'.'<td>'."<input type='text' name='$name' value='$date'>";
if ($use_date_picker)
	$st .= "<a href=\"javascript:date_picker(document.forms[0].$name);\">"
. "	<img src='$path_to_root/themes/default/images/cal.gif' width='16' height='16' border='0' alt='"._('Click Here to Pick up the date')."'></a>\n";
echo $st.'</td></tr>' ;
medico_list_row(_("Seleccione un medico: "), 'medico', null, false, false);
tipo_cita_list_row(_("Seleccionar un tipo de cita:"), 'tipocita', null, null, false);
//echo '<tr><td colspan="2" align="center"><input type="submit" value="Generar" /></td></tr>';
//if ($error != "" )  label_row(_(" "), $error);

end_table(1);

div_end();
div_start('controls');
submit_add_or_update_center($selected_id == -1, '', true);
div_end();
end_form();
//echo '</form>';

		  	
echo "<div align='center'><a href='../index.php?application=orders'>" . _("Back") . "</a></div><br>";		  	


end_page(false,true);

?>
