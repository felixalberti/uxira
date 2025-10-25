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
//Felix Alberti 03/07/2015 se agregaron los campos $solic_profesional
//Felix Alberti 02/07/2015 se agregaron los campos $trans_medico, $medico_no
function update_item($stock_id, $description, $long_description, $category_id, 
	$tax_type_id, $units='', $mb_flag='', $sales_account, $inventory_account, 
	$cogs_account, 	$adjustment_account, $assembly_account, $dimension_id, 
	$dimension2_id, $no_sale, $editable, $trans_medico, $medico_no, $solic_profesional,
        $source_code, $our_code, $referencia, $description3, $marca, $representante,
        $long_instructions, $recalculate_price=0, $paid_honorary=0)
{
        $trans_medico = ($trans_medico == NULL) ? 0 : $trans_medico;
        $medico_no = ($medico_no == NULL) ? 0 : $medico_no;
        $solic_profesional = ($solic_profesional == NULL) ? 0 : $solic_profesional;
        $recalculate_price = ($recalculate_price == NULL) ? 0 : $recalculate_price;
        $paid_honorary = ($paid_honorary == NULL) ? 0 : $paid_honorary;
	$sql = "UPDATE ".TB_PREF."stock_master SET long_description=".db_escape($long_description).",
		description=".db_escape($description).",
		category_id=".db_escape($category_id).",
		sales_account=".db_escape($sales_account).",
		inventory_account=".db_escape($inventory_account).",
		cogs_account=".db_escape($cogs_account).",
		adjustment_account=".db_escape($adjustment_account).",
		assembly_account=".db_escape($assembly_account).",
		dimension_id=".db_escape($dimension_id).",
		dimension2_id=".db_escape($dimension2_id).",
		tax_type_id=".db_escape($tax_type_id).",
		no_sale=".db_escape($no_sale).",
		editable=".db_escape($editable).",
                trans_medico=".db_escape($trans_medico).",
		medico_no=".db_escape($medico_no).",
                solic_profesional=".db_escape($solic_profesional).",
                source_code=".db_escape($source_code).",
                our_code=".db_escape($our_code).",
                referencia=".db_escape($referencia).",
                descripcion3=".db_escape($description3).",
                marca=".db_escape($marca).",
                representante=".db_escape($representante).",
                long_instructions=".db_escape($long_instructions).",
                recalculate_price=".db_escape($recalculate_price).",
                paid_honorary=".db_escape($paid_honorary);

	if ($units != '')
		$sql .= ", units='$units'";

	if ($mb_flag != '')
		$sql .= ", mb_flag='$mb_flag'";

	$sql .= " WHERE stock_id=".db_escape($stock_id);

	db_query($sql, "The item could not be updated");

	update_item_code(-1, $stock_id, $stock_id, $description, $category_id, 1, 0);
}

//Felix Alberti 03/07/2015 se agregaron los campos $solic_profesional
//Felix Alberti 02/07/2015 se agregaron los campos $trans_medico, $medico_no
function add_item($stock_id, $description, $long_description, $category_id, 
	$tax_type_id, $units, $mb_flag,	$sales_account, $inventory_account, 
	$cogs_account, $adjustment_account, $assembly_account, $dimension_id, 
	$dimension2_id, $no_sale, $editable, $trans_medico, $medico_no, $solic_profesional,
        $source_code, $our_code, $referencia, $description3, $marca, $representante,
        $long_instructions, $recalculate_price, $paid_honorary)
{
        $trans_medico = ($trans_medico == NULL) ? 0 : $trans_medico;
        $medico_no = ($medico_no == NULL) ? 0 : $medico_no;
        $solic_profesional = ($solic_profesional == NULL) ? 0 : $solic_profesional;
        $recalculate_price = ($recalculate_price == NULL) ? 0 : $recalculate_price;
        $paid_honorary = ($paid_honorary == NULL) ? 0 : $paid_honorary;
	$sql = "INSERT INTO ".TB_PREF."stock_master (stock_id, description, long_description, category_id,
		tax_type_id, units, mb_flag, sales_account, inventory_account, cogs_account,
		adjustment_account, assembly_account, dimension_id, dimension2_id, no_sale, editable,
                trans_medico, medico_no, solic_profesional, source_code, our_code, referencia, descripcion3, marca,
                representante, long_instructions, recalculate_price, paid_honorary)
		VALUES (".db_escape($stock_id).", ".db_escape($description).", ".db_escape($long_description).",
		".db_escape($category_id).", ".db_escape($tax_type_id).", "
		.db_escape($units).", ".db_escape($mb_flag).",
		".db_escape($sales_account).", ".db_escape($inventory_account)
		.", ".db_escape($cogs_account).",".db_escape($adjustment_account)
		.", ".db_escape($assembly_account).", "
		.db_escape($dimension_id).", ".db_escape($dimension2_id).","
		.db_escape($no_sale).","
		.db_escape($editable).","
                .db_escape($trans_medico).","
                .db_escape($medico_no).","
                .db_escape($solic_profesional).","
                .db_escape($source_code).","
                .db_escape($our_code).","
                .db_escape($referencia).","
                .db_escape($description3).","
                .db_escape($marca).","
                .db_escape($representante).","
                .db_escape($long_instructions).","
                .db_escape($recalculate_price).","
                .db_escape($paid_honorary).")";

	db_query($sql, "The item could not be added");

	$sql = "INSERT INTO ".TB_PREF."loc_stock (loc_code, stock_id)
		SELECT ".TB_PREF."locations.loc_code, ".db_escape($stock_id)
		." FROM ".TB_PREF."locations";

	db_query($sql, "The item locstock could not be added");

	add_item_code($stock_id, $stock_id, $description, $category_id, 1, 0);
}

function delete_item($stock_id)
{
	$sql="DELETE FROM ".TB_PREF."stock_master WHERE stock_id=".db_escape($stock_id);
	db_query($sql, "could not delete stock item");

	/*and cascade deletes in loc_stock */
	$sql ="DELETE FROM ".TB_PREF."loc_stock WHERE stock_id=".db_escape($stock_id);
	db_query($sql, "could not delete stock item loc stock");

	/*and cascade deletes in purch_data */
	$sql ="DELETE FROM ".TB_PREF."purch_data WHERE stock_id=".db_escape($stock_id);
	db_query($sql, "could not delete stock item purch data");

	/*and cascade deletes in prices */
	$sql ="DELETE FROM ".TB_PREF."prices WHERE stock_id=".db_escape($stock_id);
	db_query($sql, "could not delete stock item prices");

	/*and cascade delete the bill of material if any */
	$sql = "DELETE FROM ".TB_PREF."bom WHERE parent=".db_escape($stock_id);
	db_query($sql, "could not delete stock item bom");

	delete_item_kit($stock_id);
}

function get_item($stock_id)
{
	$sql = "SELECT ".TB_PREF."stock_master.*,".TB_PREF."item_tax_types.name AS tax_type_name
		FROM ".TB_PREF."stock_master,".TB_PREF."item_tax_types
		WHERE ".TB_PREF."item_tax_types.id=".TB_PREF."stock_master.tax_type_id
		AND stock_id=".db_escape($stock_id);
	$result = db_query($sql,"an item could not be retrieved");

	return db_fetch($result);
}

function get_items()
{
	$sql = "SELECT * FROM ".TB_PREF."stock_master";
	return db_query($sql,"items could not be retreived");
}

function get_items_by_filters($category,$sales_type,$stock_id)
{
	$sql = "SELECT sm.stock_id, sm.category_id, p.sales_type_id, p.curr_abrev, p.price FROM ".
                TB_PREF."stock_master sm LEFT JOIN ".
                TB_PREF."prices p on (sm.stock_id = p.stock_id and p.sales_type_id = ".db_escape($sales_type)." ) "
                . " WHERE inactive = 0";
                if ($category != "" && $category != -1)
                $sql .= " and category_id = ".db_escape($category);
                if ($stock_id != "" && $stock_id != -1)
                $sql .= " and sm.stock_id = ".db_escape($stock_id);
            //display_notification($sql);     
	return db_query($sql,"items by category could not be retreived");
}

function items_source_code_exist($id,$item)
{
	$sql = "SELECT count(*) as exits FROM ".TB_PREF."stock_master WHERE stock_id != ".db_escape($id)." and source_code = ".db_escape($item);
	$result = db_query($sql,"The source_code on stock_master could not be retreived");
        $row = db_fetch($result);
        if ($row['exits'] > 0) return true;
        else false;
}

function items_our_code_exist($id,$item)
{
	$sql = "SELECT count(*) as exits FROM ".TB_PREF."stock_master WHERE stock_id != ".db_escape($id)." and our_code = ".db_escape($item);
	$result = db_query($sql,"The source_code on stock_master could not be retreived");
        $row = db_fetch($result);
        if ($row['exits'] > 0) return true;
        else false;
}

function item_in_foreign_codes($stock_id)
{
	$sqls=  array(
	"SELECT COUNT(*) FROM "
		.TB_PREF."stock_moves WHERE stock_id=".db_escape($stock_id) =>
	 _('Cannot delete this item because there are stock movements that refer to this item.'),
	"SELECT COUNT(*) FROM "
		.TB_PREF."bom WHERE component=".db_escape($stock_id)=>
	 _('Cannot delete this item record because there are bills of material that require this part as a component.'),
	"SELECT COUNT(*) FROM "
		.TB_PREF."sales_order_details WHERE stk_code=".db_escape($stock_id) =>
	 _('Cannot delete this item because there are existing purchase order items for it.'),
	"SELECT COUNT(*) FROM "
		.TB_PREF."purch_order_details WHERE item_code=".db_escape($stock_id)=>
	 _('Cannot delete this item because there are existing purchase order items for it.')
	);

	$msg = '';

	foreach($sqls as $sql=>$err) {
		$result = db_query($sql, "could not query stock usage");
		$myrow = db_fetch_row($result);
		if ($myrow[0] > 0) 
		{
			$msg = $err; break;
		}
	}
	if ($msg == '') {	

		$kits = get_where_used($stock_id);
		$num_kits = db_num_rows($kits);
		if ($num_kits) {
			$msg = _("This item cannot be deleted because some code aliases 
				or foreign codes was entered for it, or there are kits defined 
				using this item as component")
				.':<br>';

			while($num_kits--) {
				$kit = db_fetch($kits);
				$msg .= "'".$kit[0]."'";
				if ($num_kits) $msg .= ',';
			}

		}
	}
	return $msg;
}

/*Function que verifica si se hace recalculo*/
function get_stock_recalculate($stock_id)
{
	$sql = "SELECT recalculate_price
		FROM ".TB_PREF."stock_master
		WHERE
		stock_id=".db_escape($stock_id);
        //display_notification($sql);
	$result = db_query($sql,"the item could not be retrieved");

	return db_fetch($result);
}
?>