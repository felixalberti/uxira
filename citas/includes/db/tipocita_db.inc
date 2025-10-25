<?php
/**********************************************************************
    Copyright (C) FELIX ALBERTI, C.A.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
//Begin Felix Alberti 24/07/2010
function get_tipocita($codigotipo)
{
	$sql = "SELECT descripcion FROM ".TB_PREF."cm_system_tables where codigotabla = 'TPCITA' and codigotipo = $codigotipo";
	$result = db_query($sql,"La tabla no puedo ser retornada");  
	$row = db_fetch($result);
	return $row['descripcion'];	
}
//End Felix Alberti 24/07/2010
?>