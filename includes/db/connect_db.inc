<?php
/**********************************************************************
    Copyright (C) FrontAccounting, LLC.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/

function set_global_connection($company=-1)
{
	global $db, $transaction_level, $db_connections;

	cancel_transaction(); // cancel all aborted transactions if any
	$transaction_level = 0;

	if ($company == -1) 
		$company = $_SESSION["wa_current_user"]->company;

	$_SESSION["wa_current_user"]->cur_con = $company;

	$connection = $db_connections[$company];

	/*$db = mysql_connect($connection["host"], $connection["dbuser"], $connection["dbpassword"]);
		mysql_select_db($connection["dbname"], $db);*/
        $db = mysqli_connect($connection["host"], $connection["dbuser"], $connection["dbpassword"], $connection["dbname"]);
	return $db;
}

$db_duplicate_error_code = 1062;

//DB wrapper functions to change only once for whole application

function db_query($sql, $err_msg=null)
{
	global $db, $show_sql, $sql_trail, $select_trail, $go_debug, $sql_queries, $Ajax,
		$db_connections, $db_last_inserted_id;
	
	// set current db prefix
	$cur_prefix = $db_connections[$_SESSION["wa_current_user"]->cur_con]['tbpref'];
	$sql = str_replace(TB_PREF, $cur_prefix, $sql);

	if ($show_sql)
	{
		$Ajax->activate('footer_debug');
		$sql_queries .= "<pre>$sql</pre>\n<hr>";
	}

	//$result = mysql_query($sql, $db);
        $result = mysqli_query($db, $sql);
	
	if($sql_trail) {
		$db_last_inserted_id = mysql_insert_id($db);	// preserve in case trail insert is done
		if ($select_trail || (strstr($sql, 'SELECT') === false)) {
			/*mysql_query(
			"INSERT INTO ".$cur_prefix."sql_trail
				(`sql`, `result`, `msg`)
				VALUES(".db_escape($sql).",".($result ? 1 : 0).",
				".db_escape($err_msg).")", $db);*/
                        mysqli_query($db, "INSERT INTO ".$cur_prefix."sql_trail
				(`sql`, `result`, `msg`)
				VALUES(".db_escape($sql).",".($result ? 1 : 0).",
				".db_escape($err_msg).")" );
		}
	}

	if ($err_msg != null || $go_debug) {
		$exit = $err_msg != null;
		if (function_exists('xdebug_call_file'))
			check_db_error('<br>At file '.xdebug_call_file().':'.xdebug_call_line().':<br>'.$err_msg, $sql, $exit);
		else
			check_db_error($err_msg, $sql, $exit);
	}
	return $result;
}

function db_fetch_row ($result)
{

	//return mysql_fetch_row($result);
        return mysqli_fetch_row($result); 
}

function db_fetch_assoc ($result)
{

	//return mysql_fetch_assoc($result);
        return mysqli_fetch_assoc($result);
}

function db_fetch ($result)
{

	//return mysql_fetch_array($result);
        return mysqli_fetch_array($result);
}

function db_seek (&$result,$record)
{
	//return mysql_data_seek($result, $record);
        return mysqli_data_seek($result, $record);;
}

function db_free_result ($result)
{
	if ($result)
		//mysql_free_result($result);
                mysqli_free_result($result);
}

function db_num_rows ($result)
{
	//return mysql_num_rows($result);
        return mysqli_num_rows($result);
}

function db_num_fields ($result)
{
	//return mysql_num_fields($result);
        return mysqli_num_fields($result);
}

function db_escape($value = "", $nullify = false)
{
        global $db;//fap 01/07/2015
	$value = @html_entity_decode($value, ENT_QUOTES, $_SESSION['language']->encoding);
	$value = @htmlspecialchars($value, ENT_QUOTES, $_SESSION['language']->encoding);

  	//reset default if second parameter is skipped
	$nullify = ($nullify === null) ? (false) : ($nullify);

  	//check for null/unset/empty strings
	if ((!isset($value)) || (is_null($value)) || ($value === "")) {
		$value = ($nullify) ? ("NULL") : ("''");
	} else {
		if (is_string($value)) {
      		//value is a string and should be quoted; determine best method based on available extensions
			if (function_exists('mysql_real_escape_string')) {
		  		//$value = "'" . mysql_real_escape_string($value) . "'";
                                $value = "'" . mysqli_real_escape_string ($db, $value) . "'";
			} else {
			  //$value = "'" . mysql_escape_string($value) . "'";
                          $value = "'" . mysqli_real_escape_string($db, $value) . "'";
			}
		} else if (!is_numeric($value)) {
			//value is not a string nor numeric
			display_error("ERROR: incorrect data type send to sql query");
			echo '<br><br>';
			exit();
		}
	}
	return $value;
}

function db_error_no ()
{
	global $db;
	//return mysql_errno($db);
        return mysqli_errno ($db);
}

function db_error_msg($conn)
{
	//return mysql_error($conn);
        return mysqli_error($conn);
}

function db_insert_id()
{
	global $db_last_inserted_id, $sql_trail, $db;

	//return $sql_trail ? $db_last_inserted_id : mysql_insert_id($db);
        return $sql_trail ? $db_last_inserted_id : mysqli_insert_id($db);
}

function db_num_affected_rows()
{
	global $db;
	return mysql_affected_rows($db);
}

function mysqli_field_name($result, $field_offset)
{
    $properties = mysqli_fetch_field_direct($result, $field_offset);
    return is_object($properties) ? $properties->name : null;
}

function db_field_name($result, $n)
{
	//return mysql_field_name($result, $n);//fap
        return mysqli_field_name($result, $n);
}

function db_create_db($connection)
{
	/*$db = mysql_connect($connection["host"] ,
		$connection["dbuser"], $connection["dbpassword"]);*/
        $db = mysqli_connect($connection["host"], $connection["dbuser"], $connection["dbpassword"], $connection["dbname"]);
	//if (!mysql_select_db($connection["dbname"], $db))
        if (!mysqli_select_db($db,$connection["dbname"]))
	{
		$sql = "CREATE DATABASE IF NOT EXISTS " . $connection["dbname"] . "";
		/*if (!mysql_query($sql) || !mysql_select_db($connection["dbname"], $db))*/
                  if (!mysqli_query($db, $sql) || !mysqli_select_db($db,$connection["dbname"]))
			return 0;
	}
	return $db;
}

function db_drop_db($connection)
{
        global $db;//fap 01/07/2015
	if ($connection["tbpref"] == "")
	{
		$sql = "DROP DATABASE IF EXISTS " . $connection["dbname"] . "";
		//return mysql_query($sql);
                return mysqli_query($db, $sql);
	}
	else
	{
    	$res = db_query("show table status");
    	$all_tables = array();
    	while($row = db_fetch($res))
    		$all_tables[] = $row;
        // get table structures
		foreach ($all_tables as $table)
		{
			if (strpos($table['Name'], $connection["tbpref"]) === 0)
				db_query("DROP TABLE `".$table['Name'] . "`");
		}
		//deleting the tables, how??
		return true;
	}
}

function db_close($dbase = null)
{
	global $db;
	
	if (!$dbase)
		$dbase = $db;
	//return mysql_close($dbase);
        return mysqli_close($dbase);
}

?>