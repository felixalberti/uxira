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
class inventory_app extends application
{
	function inventory_app()
	{
		$this->application("stock", T_gettext($this->help_context = "&Items and Inventory"));

		$this->add_module(T_gettext("Transactions"));
		$this->add_lapp_function(0, T_gettext("Inventory Location &Transfers"),
			"inventory/transfers.php?NewTransfer=1", 'SA_LOCATIONTRANSFER', MENU_TRANSACTION);
		$this->add_lapp_function(0, T_gettext("Inventory &Adjustments"),
			"inventory/adjustments.php?NewAdjustment=1", 'SA_INVENTORYADJUSTMENT', MENU_TRANSACTION);

		$this->add_module(T_gettext("Inquiries and Reports"));
		$this->add_lapp_function(1, T_gettext("Inventory Item &Movements"),
			"inventory/inquiry/stock_movements.php?", 'SA_ITEMSTRANSVIEW', MENU_INQUIRY);
		$this->add_lapp_function(1, T_gettext("Inventory Item &Status"),
			"inventory/inquiry/stock_status.php?", 'SA_ITEMSSTATVIEW', MENU_INQUIRY);
		$this->add_lapp_function(1, T_gettext("Inventory Item &Resumen"),
			"inventory/inquiry/stock_movements_resumen.php?", 'SA_ITEMSTRANSVIEW', MENU_INQUIRY);
		$this->add_rapp_function(1, T_gettext("Inventory &Reports"),
			"reporting/reports_main.php?Class=2", 'SA_ITEMSTRANSVIEW', MENU_REPORT);

		$this->add_module(T_gettext("Maintenance"));
		$this->add_lapp_function(2, T_gettext("&Items"),
			"inventory/manage/items.php?", 'SA_ITEM', MENU_ENTRY);
                /*Begin Felix Alberti 02/09/2016*/
		$this->add_lapp_function(2, T_gettext("&Item Updates"),
			"inventory/inquiry/items_update.php?", 'SA_ITEM', MENU_MAINTENANCE);
                /*End Felix Alberti 02/09/2016*/
		$this->add_lapp_function(2, T_gettext("Sales &Kits"),
			"inventory/manage/sales_kits.php?", 'SA_SALESKIT', MENU_MAINTENANCE);
		$this->add_lapp_function(2, T_gettext("Item &Categories"),
			"inventory/manage/item_categories.php?", 'SA_ITEMCATEGORY', MENU_MAINTENANCE);
		$this->add_lapp_function(2, T_gettext("Inventory &Locations"),
			"inventory/manage/locations.php?", 'SA_INVENTORYLOCATION', MENU_MAINTENANCE);
		$this->add_rapp_function(2, T_gettext("Inventory &Movement Types"),
			"inventory/manage/movement_types.php?", 'SA_INVENTORYMOVETYPE', MENU_MAINTENANCE);
		$this->add_rapp_function(2, T_gettext("&Units of Measure"),
			"inventory/manage/item_units.php?", 'SA_UOM', MENU_MAINTENANCE);
		$this->add_rapp_function(2, T_gettext("&Reorder Levels"),
			"inventory/reorder_level.php?", 'SA_REORDER', MENU_MAINTENANCE);

		$this->add_module(T_gettext("Pricing and Costs"));
		$this->add_lapp_function(3, T_gettext("Sales &Pricing"),
			"inventory/prices.php?", 'SA_SALESPRICE', MENU_MAINTENANCE);
                $this->add_lapp_function(3, T_gettext("&Batch change prices"),
			"inventory/batch_change_prices.php?", 'SA_BATCHCHANGEPRICE', MENU_MAINTENANCE);
		$this->add_lapp_function(3, T_gettext("Purchasing &Pricing"),
			"inventory/purchasing_data.php?", 'SA_PURCHASEPRICING', MENU_MAINTENANCE);
		$this->add_rapp_function(3, T_gettext("Standard &Costs"),
			"inventory/cost_update.php?", 'SA_STANDARDCOST', MENU_MAINTENANCE);

		$this->add_extensions();
	}
}


?>