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
function add_item_price($stock_id, $sales_type_id, $curr_abrev, $price)
{
	$sql = "INSERT INTO ".TB_PREF."prices (stock_id, sales_type_id, curr_abrev, price) 
		VALUES (".db_escape($stock_id).", ".db_escape($sales_type_id)
		.", ".db_escape($curr_abrev).", ".db_escape($price).")";
	
	db_query($sql,"an item price could not be added");		
}

function update_item_price($price_id, $sales_type_id, $curr_abrev, $price, $changed, $user_changed)
{
	$sql = "UPDATE ".TB_PREF."prices SET sales_type_id=".db_escape($sales_type_id).", 
		curr_abrev=".db_escape($curr_abrev).",changed=".$changed.",user_change=".db_escape($user_changed).", 
		price=".db_escape($price).",change_date=now()"." WHERE id=".db_escape($price_id);
     
	db_query($sql,"an item price could not be updated");		
}
/*Begin Felix Alberti 16/09/2016*/
function update_item_price_by_stock_id($stock_id, $sales_type_id, $curr_abrev, $price, $changed=null, $user_changed=null)
{
        if ($changed==null)
	$sql = "UPDATE ".TB_PREF."prices SET 
		price='".$price."', change_date=now() WHERE stock_id=".db_escape($stock_id).
                " and sales_type_id=".db_escape($sales_type_id)." and curr_abrev=".db_escape($curr_abrev);
        else
        $sql = "UPDATE ".TB_PREF."prices SET 
		price=".db_escape($price).",changed=".$changed.",user_change=".db_escape($user_changed).
                ",change_date=now()".
                " WHERE stock_id=".db_escape($stock_id).
                " and sales_type_id=".db_escape($sales_type_id)." and curr_abrev=".db_escape($curr_abrev);    
 
	db_query($sql,"an item price could not be updated");		
}
/*End Felix Alberti 15/09/2016*/
/*Begin Felix Alberti 15/09/2016*/
function update_item_only_price($price_id, $sales_type_id, $curr_abrev, $price)
{
	$sql = "UPDATE ".TB_PREF."prices SET 
		curr_abrev=".db_escape($curr_abrev).", 
		price=".db_escape($price)." WHERE stock_id=".db_escape($price_id). " and sales_type_id = ".db_escape($sales_type_id);
	db_query($sql,"an item price could not be updated");
        //display_notification($sql);
}
/*End Felix Alberti 15/09/2016*/

/*Begin Felix Alberti 31/08/2017*/
function update_item_price_by_id($price_id, $price)
{
	$sql = "UPDATE ".TB_PREF."prices SET
		price=".db_escape($price)." WHERE id=".db_escape($price_id);
        //display_notification('update_item_price_by_stock_id '.$sql);
	db_query($sql,"an item price could not be updated");		
}
/*End Felix Alberti 31/08/2017*/

function delete_item_price($price_id)
{
	$sql="DELETE FROM ".TB_PREF."prices WHERE id= ".db_escape($price_id);
	db_query($sql,"an item price could not be deleted");			
}

function get_prices($stock_id)
{
	$sql = "SELECT ".TB_PREF."sales_types.sales_type, ".TB_PREF."prices.* 
		FROM ".TB_PREF."prices, ".TB_PREF."sales_types 
		WHERE ".TB_PREF."prices.sales_type_id = ".TB_PREF."sales_types.id 
		AND stock_id=".db_escape($stock_id)
		." ORDER BY curr_abrev, sales_type_id";	
	return db_query($sql,"item prices could not be retreived");
}

function get_stock_price($price_id)
{
	$sql = "SELECT * FROM ".TB_PREF."prices WHERE id=".db_escape($price_id);
	
	$result = db_query($sql,"price could not be retreived");
	//display_notification($sql);
	return db_fetch($result);
}

function get_stock_price_type_currency($stock_id, $type, $currency)
{
	$sql = "SELECT * FROM ".TB_PREF."prices WHERE stock_id=".db_escape($stock_id)."
		AND sales_type_id=".db_escape($type)."
		AND curr_abrev=".db_escape($currency);
	
	$result = db_query($sql,"price could not be retreived");
	//display_notification($sql);
	return db_fetch($result);
}

/*Begin Felix Alberti 15/09/2016*/
function get_exist_price_type_currency($stock_id, $type)
{
	$sql = "SELECT count(*) as qty FROM ".TB_PREF."prices WHERE stock_id=".db_escape($stock_id)."
		AND sales_type_id=".db_escape($type);
        
	$result = db_query($sql,"exist price could not be retreived");
	
	$myrow = db_fetch($result);
        return $myrow['qty'];
}
/*End Felix Alberti 15/09/2016*/

/*Begin Felix Alberti 01/11/2016*/
function add_price_history($component, $sales_type_id, $stock_id, $last_qty, $qty, $action, $user){
    $sql = "INSERT INTO ".TB_PREF."prices_history"
            . " (trans_date, sales_type_id, kit, stock_id,"
            . " last_qty, quantity, action, user)"
            . " VALUES (Now(), ".db_escape($sales_type_id)." , ".db_escape($component).", ".db_escape($stock_id).", "
            . db_escape($last_qty).", ".db_escape($qty).", ".db_escape($action).", ".db_escape($user)." )";
    
    db_query($sql,"an item price could not be added in prices_history");		
}

function get_item_code_stock($item_code, $stock_id)
{
	$sql = "SELECT * FROM ".TB_PREF."item_codes WHERE item_code=".db_escape($item_code)."
		AND stock_id=".db_escape($stock_id);
	
	$result = db_query($sql,"The item code could not be retreived");
	
	return db_fetch($result);
}

function get_fec_max_price_history(){
    $sql = "SELECT DATE_FORMAT(max(trans_date),'%Y-%m-%d') as max_trans_date FROM ".TB_PREF."prices_history";
	
    $result = db_query($sql,"The max trans date could not be retreived");
	
    $row = db_fetch($result);
    
    return $row['max_trans_date'];    
}

function get_sum_kit($kit,$sales_type_id){
    $sql = "SELECT sum(p.price * i.quantity) as sum_price_kit FROM "
            . TB_PREF."item_codes i, ".TB_PREF."prices p"
            . " WHERE i.item_code = ".db_escape($kit)." and "
            . "p.stock_id = i.stock_id and p.sales_type_id = ".db_escape($sales_type_id);
    
    $result = db_query($sql,"The sum price kit could not be retreived");
	
    return db_fetch($result);  
}
/*End Felix Alberti 01/11/2016*/

function get_sum_component_kit($item,$sales_type,$curr_abrev){
    //Función que busca un item al que le cambiaron el precio y obtiene la sumatoria
    //de todos los demás componentes (Sin el precio del item mismo)
    $sql = "SELECT i1.item_code, sum(i2.quantity * p.price) as price, i1.recalculate, p.id 
        FROM ".TB_PREF."item_codes i1, 
             ".TB_PREF."item_codes i2, 
             ".TB_PREF."prices p where
        i1.stock_id = ".db_escape($item)." and i1.item_code != i1.stock_id and
        i2.item_code = i1.item_code and
        i2.stock_id != i1.stock_id and
        p.stock_id = i2.stock_id and
        p.sales_type_id = ".db_escape($sales_type)." and p.curr_abrev = ".db_escape($curr_abrev)." group by i1.item_code";
        display_notification($sql);
        return db_query($sql,"The sum component kit could not be retreived");
}

function get_component_kit($item,$sales_type,$curr_abrev){
    $sql = "SELECT i.item_code, (p.price) as price, p.id, i.quantity  "
            . "FROM ".TB_PREF."item_codes i, ".TB_PREF."prices p "
            . "where i.stock_id  = ".db_escape($item)." AND"
            . " i.item_code = p.stock_id AND "
            . " p.curr_abrev = ".db_escape($curr_abrev)." AND"
            . " p.sales_type_id = ".db_escape($sales_type)." and i.recalculate = 1";       
    
        //display_notification($sql);
        return db_query($sql,"The sum component kit could not be retreived");
}
?>