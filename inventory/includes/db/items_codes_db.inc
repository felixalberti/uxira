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
/*
	item_codes table is used to store both multiply foreign codes and 
	sale kits definition.
*/
function update_item_code($id, $item_code, $stock_id, $description, $category, $qty, $foreign=0, $only_quotation=0)
{
	$sql = "UPDATE ".TB_PREF."item_codes SET
	 	item_code = ".db_escape($item_code).",
	 	stock_id = ".db_escape($stock_id).",
	 	description = ".db_escape($description).",
	 	category_id = ".db_escape($category).",
	 	quantity = ".db_escape($qty).",
	 	is_foreign = ".db_escape($foreign).",
                only_quotation = ".db_escape($only_quotation)."
        	WHERE ";
			
	if ($id == -1) // update with unknown $id i.e. from items table editor
	 	$sql .= "item_code = ".db_escape($item_code)
		." AND stock_id = ".db_escape($stock_id);
	else
		$sql .= "id = ".db_escape($id);

	db_query($sql,"an item code could not be updated");
}

function add_item_code($item_code, $stock_id, $description, $category, $qty, $foreign=0, $only_quotation=0)
{
	$sql = "INSERT INTO ".TB_PREF."item_codes
			(item_code, stock_id, description, category_id, quantity, is_foreign, only_quotation) 
			VALUES( ".db_escape($item_code).",".db_escape($stock_id).",
	  		".db_escape($description).",".db_escape($category)
	  		.",".db_escape($qty).",".db_escape($foreign).",".db_escape($only_quotation).")";

	db_query($sql,"an item code could not be added");
}

function delete_item_code($id)
{
	$sql="DELETE FROM ".TB_PREF."item_codes WHERE id=".db_escape($id);
	db_query($sql,"an item code could not be deleted");
}

function get_item_code($id)
{
	$sql="SELECT * FROM ".TB_PREF."item_codes WHERE id=".db_escape($id);
        
	$result = db_query($sql,"item code could not be retrieved");

	return db_fetch($result);
}

function get_item_code_by_item_code($item_code)
{
	$sql="SELECT * FROM ".TB_PREF."item_codes WHERE item_code=".db_escape($item_code)." limit 1";

	$result = db_query($sql,"item code could not be retrieved");

	return db_fetch($result);
}


/*Function que verifica si se hace un recalculo en kit donde se encuentra el producto*/
function get_item_code_recalculate($kit,$stock_id)
{
	$sql = "SELECT recalculate
		FROM ".TB_PREF."item_codes
		WHERE
                item_code=".db_escape($kit)." and 
		stock_id=".db_escape($stock_id);
        //display_notification($sql);
	$result = db_query($sql,"an item could not be retrieved");

	return db_fetch($result);
}

function get_all_item_codes($stock_id, $foreign=1)
{
	$sql="SELECT i.*, c.description as cat_name FROM "
		.TB_PREF."item_codes as i,"
		.TB_PREF."stock_category as c
		WHERE stock_id=".db_escape($stock_id)."
		AND i.category_id=c.category_id
		AND i.is_foreign=".db_escape($foreign);

	$result = db_query($sql,"all item codes could not be retrieved");

	return $result;
}

function delete_item_kit($item_code)
{
	$sql="DELETE FROM ".TB_PREF."item_codes WHERE item_code=".db_escape($item_code);
	db_query($sql,"an item kit could not be deleted");
}

function get_item_kit($item_code)
{
	$sql="SELECT DISTINCT kit.*, item.units, comp.description as comp_name 
		FROM "
		.TB_PREF."item_codes kit,"
		.TB_PREF."item_codes comp
		LEFT JOIN "
		.TB_PREF."stock_master item
		ON 
			item.stock_id=comp.item_code
		WHERE
			kit.stock_id=comp.item_code
			AND kit.item_code=".db_escape($item_code);
      
	$result = db_query($sql,"item kit could not be retrieved");

	return $result;
}

//Begin Felix Alberti 23/08/2016
function get_item_kit_and_prices($item_code,$sales_type=false)
{
	$sql="SELECT DISTINCT kit.*, item.units, comp.description as comp_name, prices.price
		FROM "
		.TB_PREF."item_codes kit,"
		.TB_PREF."item_codes comp
		LEFT JOIN "
		.TB_PREF."stock_master item
		ON 
			item.stock_id=comp.item_code ".
                "LEFT JOIN ".
                TB_PREF."prices prices
		ON prices.stock_id=comp.item_code ";
                if ($sales_type)
                $sql.= " and prices.sales_type_id = ".$sales_type;
        $sql.=
		" WHERE
			kit.stock_id=comp.item_code
			AND kit.item_code=".db_escape($item_code);
        
        /*if ($item_code=='KIPRO-003')
        display_notification('get_item_kit_and_prices '.$sql);*/
	$result = db_query($sql,"item kit could not be retrieved");
    display_notification('get_item_kit_and_prices '.$sql);
	return $result;
}
//End Felix Alberti 23/08/2016
//Begin Felix Alberti 14/09/2016
function get_item_kit_qty($item_code,$sales_type=false)
{
	$sql="SELECT distinct comp.stock_id, (prices.price * comp.quantity) as price
		FROM "
		.TB_PREF."item_codes comp
		LEFT JOIN "
		.TB_PREF."stock_master item
		ON 
			item.stock_id=comp.item_code ".
                "LEFT JOIN ".
                TB_PREF."prices prices
		ON prices.stock_id=comp.stock_id ";
                if ($sales_type)
                $sql.= " and prices.sales_type_id = ".$sales_type;
        $sql.=
		" WHERE
			comp.item_code=".db_escape($item_code);
       
        //display_notification($sql);
	$result = db_query($sql,"item kit could not be retrieved");

	return $result;
}
//
function price_kit($kit,$sales_type){
    $sql = "SELECT sum(price) as price, sum(quantity) as qty  
		FROM ".TB_PREF."item_codes i LEFT JOIN ".TB_PREF."prices p ON
                    ( i.stock_id = p.stock_id and p.sales_type_id = $sales_type)
		WHERE i.item_code = ".db_escape($kit);
   $res = db_query($sql,"item kit prices could not be retreived");
   //display_notification($sql);
   $myrow = db_fetch($res);
   return $myrow;    
}
//End Felix Alberti 14/09/2016
//
//
//Begin Felix Alberti 15/09/2016
function get_data_item_price($item,$sales_type){
    $sql = "SELECT p.*  
		FROM ".TB_PREF."prices p                    
		WHERE p.stock_id = ".db_escape($item)." and  p.sales_type_id = ".db_escape($sales_type);
   $res = db_query($sql,"The data item price could not be retreived");
   $myrow = db_fetch($res);
   return $myrow;    
}
//End Felix Alberti 15/09/2016

function get_item_code_dflts($stock_id)
{
	$sql = "SELECT units, decimals, description, category_id
		FROM ".TB_PREF."stock_master,".TB_PREF."item_units
		WHERE stock_id=".db_escape($stock_id);

	$result = db_query($sql,"item code defaults could not be retrieved");
	return db_fetch($result);
}
//
//	Check if kit contains given item, optionally recursive.
//
function check_item_in_kit($old_id, $kit_code, $item_code, $recurse=false)
{
	$result = get_item_kit($kit_code);
	//if ($result != 0)
        if ($result && db_num_rows($result) != 0)
	{
		while ($myrow = db_fetch($result))
		{
			if ($myrow['id'] == $old_id) 
				continue;
				
			if ($myrow['stock_id'] == $item_code)
			{
				return 1;
			}

			if ($recurse && $myrow['item_code'] != $myrow['stock_id']
				&& check_item_in_kit($old_id, $item_code, $myrow['stock_id'], true))
			{
				return 1;
			}
		}
	}
	return 0;
}

function get_kit_props($kit_code)
{
	$sql = "SELECT description, category_id, recalculate, only_quotation, specialties_id FROM ".TB_PREF."item_codes "
		. " WHERE item_code=".db_escape($kit_code);
	$res = db_query($sql, "kit name query failed");
	return db_fetch($res);
}

function update_kit_props($kit_code, $name, $category, $recalculate=1, $only_quotation=0, $specialties_id=0)
{
	$sql = "UPDATE ".TB_PREF."item_codes SET description="
		. db_escape($name).",category_id=".db_escape($category).",recalculate=".db_escape($recalculate)
                .",only_quotation=".db_escape($only_quotation).",specialties_id=".db_escape($specialties_id)
		. " WHERE item_code=".db_escape($kit_code);
	db_query($sql, "kit name update failed");
}

function get_where_used($item_code)
{
	$sql = "SELECT item_code, description FROM "
		.TB_PREF."item_codes "
		. " WHERE stock_id=".db_escape($item_code)."
			AND item_code!=".db_escape($item_code);
	return db_query($sql, "where used query failed");
}

//BEGIN FELIX ALBERTI 26/06/2015
//Nueva función para saber si kit esta exento de IVA
//Si es 1 está exento, si es 0 algunos de los productos tiene IVA
function get_exempt_kit($item_code){
$sql="SELECT sum(exempt) as exempt FROM ".TB_PREF."item_codes kit,".TB_PREF."item_codes comp".
	" LEFT JOIN ".TB_PREF."stock_master item	ON item.stock_id=comp.item_code, ".TB_PREF.
 	"item_tax_types tasa
		WHERE	kit.stock_id=comp.item_code
			AND kit.item_code='$item_code' 
			AND	tasa.id = item.tax_type_id
			AND exempt = 1";
			
  $result = db_query($sql,"item_tax_types.exempt for the kit(All stock_id) could not be retrieved");
  $myrow = db_fetch_row($result);
  $iva = "(imp)";
  if ($myrow[0] >= 1) $iva = "(e)";
	return $iva;			
}
//END FELIX ALBERTI 07/05/2009
?>