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
	Security sections groups various areas on both functionality and privilege levels.
	Often analytic inquires are available only for management, and configuration
	for administration or management staff. This is why we have those three
	section type inside near every FA module.

	Section codes 0-99 are reserved for core FA functionalities.
	Every security section can contain up to 256 different areas.
	External modules can extend security roles system by adding rows to 
	$security_sections and $security_areas using section codes >=100.
	Security areas and sections created by extension modules/plugins
	have dynamically assigned 3-byte integer codes. The highest byte is zero
	for sections/areas defined in this file, and extid+1 for those defined 
	by extensions 
*/
define('SS_SADMIN',	1<<8);	// site admin
define('SS_SETUP',	2<<8);	// company level setup
define('SS_SPEC',	3<<8);	// special administration

define('SS_SALES_C',11<<8); // configuration
define('SS_SALES',	12<<8);	// transactions
define('SS_SALES_A',13<<8); // analytic functions/reports/inquires
define('SS_SALES_POS_CASH',14<<8);

define('SS_PURCH_C',21<<8);
define('SS_PURCH',	22<<8);
define('SS_PURCH_A',23<<8);

define('SS_ITEMS_C',31<<8);
define('SS_ITEMS',	32<<8);
define('SS_ITEMS_A',33<<8);

define('SS_MANUF_C',41<<8);
define('SS_MANUF',	42<<8);
define('SS_MANUF_A',43<<8);

define('SS_DIM_C',	51<<8);
define('SS_DIM',	52<<8);
define('SS_DIM_A',	53<<8);

define('SS_GL_C',	61<<8);
define('SS_GL',		62<<8);
define('SS_GL_A',	63<<8);

define('SS_ADM',	71<<8);
define('SS_MENU_SALES',	81<<8);
define('SS_CUSTOMERS_C',91<<8);
define('SS_ACC_PAT',101<<8);
define('SS_MEDICAL_APPOIN',111<<8);
define('SS_SALES_APS',121<<8);


$security_sections = array(
 SS_SADMIN => _("System administration"),
 SS_SETUP => _("Company setup"),
 SS_SPEC => _("Special maintenance"),
 SS_SALES_C => _("Sales configuration"),
 SS_SALES => _("Sales transactions"),
 SS_SALES_A => _("Sales related reports"),
 SS_PURCH_C => _("Purchase configuration"),
 SS_PURCH => _("Purchase transactions"),
 SS_PURCH_A => _("Purchase analytics"),
 SS_ITEMS_C => _("Inventory configuration"),
 SS_ITEMS => _("Inventory operations"),
 SS_ITEMS_A => _("Inventory analytics"),
 SS_MANUF_C => _("Manufacturing configuration"),
 SS_MANUF => _("Manufacturing transactions"),
 SS_MANUF_A => _("Manufacturing analytics"),
 SS_DIM_C => _("Dimensions configuration"),
 SS_DIM => _("Dimensions"),
 SS_GL_C => _("Banking & GL configuration"),
 SS_GL => _("Banking & GL transactions"),
 SS_GL_A => _("Banking & GL analytics"),
 SS_ADM => _("Admision Configurations"),
 SS_MENU_SALES => _("Sales Menus"),
 SS_CUSTOMERS_C => _("Customer Configurations"),
 SS_ACC_PAT => _("Account Patient"), 
 SS_MEDICAL_APPOIN => _("Medical Appointment"),
 SS_SALES_APS => _("APS Configurations"),
);

/*
	This table stores security areas available in FA. 
	Key is area identifier used to check user rights, values are
	codes stored for each role in security_roles table and description used
	in roles editor.

	Set of allowed access areas codes is retrieved during user login from
	security_roles table, and cached in user profile.

	Special value 'SA_OPEN' is used for publicly available pages like login/logout.
*/
$security_areas =array(
//
//	Site administration
//
	'SA_CREATECOMPANY' =>array(SS_SADMIN|1, _("Install/update companies")),
	'SA_CREATELANGUAGE' => array(SS_SADMIN|2, _("Install/update languages")),
	'SA_CREATEMODULES' => array(SS_SADMIN|3, _("Install/upgrade modules")),
	'SA_SOFTWAREUPGRADE' => array(SS_SADMIN|4, _("Software upgrades")),
//
//	Company setup
//
	'SA_SETUPCOMPANY' => array(SS_SETUP|1, _("Company parameters")),
	'SA_SECROLES' => array(SS_SETUP|2, _("Access levels edition")),
	'SA_USERS' => array(SS_SETUP|3, _("Users setup")),
	'SA_POSSETUP' => array(SS_SETUP|4, _("Point of sales definitions")),
	'SA_PRINTERS' => array(SS_SETUP|5, _("Printers configuration")),
	'SA_PRINTPROFILE' => array(SS_SETUP|6, _("Print profiles")),
	'SA_PAYTERMS' => array(SS_SETUP|7, _("Payment terms")),
	'SA_SHIPPING' => array(SS_SETUP|8, _("Shipping ways")),
	'SA_CRSTATUS' => array(SS_SETUP|9, _("Credit status definitions changes")),
	'SA_INVENTORYLOCATION' => array(SS_SETUP|10, _("Inventory locations changes")),
	'SA_INVENTORYMOVETYPE'  => array(SS_SETUP|11, _("Inventory movement types")),
	'SA_WORKCENTRES' => array(SS_SETUP|12, _("Manufacture work centres")),
	'SA_FORMSETUP' => array(SS_SETUP|13, _("Forms setup")),
	'SA_CRMCATEGORY' => array(SS_SETUP|14, _("Contact categories")),
//
// Special and common functions
//
	'SA_VOIDTRANSACTION' => array(SS_SPEC|1, _("Voiding transactions")),
	'SA_BACKUP' => array(SS_SPEC|2, _("Database backup/restore")),
	'SA_VIEWPRINTTRANSACTION' => array(SS_SPEC|3, _("Common view/print transactions interface")),
	'SA_ATTACHDOCUMENT' => array(SS_SPEC|4, _("Attaching documents")),
	'SA_SETUPDISPLAY' => array(SS_SPEC|5, _("Display preferences")), //???
	'SA_CHGPASSWD' => array(SS_SPEC|6, _("Password changes")), //???

//
// Sales related functionality
//
	'SA_SALESTYPES' => array(SS_SALES_C|1, _("Sales types")),
	'SA_SALESPRICE' => array(SS_SALES_C|2, _("Sales prices edition")),
	'SA_SALESMAN' => array(SS_SALES_C|3, _("Sales staff maintenance")),
	'SA_SALESAREA' => array(SS_SALES_C|4, _("Sales areas maintenance")),
	'SA_SALESGROUP' => array(SS_SALES_C|5, _("Sales groups changes")),
	'SA_STEMPLATE' => array(SS_SALES_C|6, _("Sales templates")),
	'SA_SRECURRENT' => array(SS_SALES_C|7, _("Recurrent invoices definitions")),

	'SA_SALESTRANSVIEW' => array(SS_SALES|1,  _("Sales transactions view")),
	'SA_CUSTOMER' => array(SS_SALES|2,  _("Sales customer and branches changes")),
	'SA_SALESQUOTE' => array(SS_SALES|10, _("Sales quotations")),
	'SA_SALESORDER' => array(SS_SALES|3, _("Sales orders edition")),
	'SA_SALESDELIVERY' => array(SS_SALES|4, _("Sales deliveries edition")),
	'SA_SALESINVOICE' => array(SS_SALES|5, _("Sales invoices edition")),
	'SA_SALESCREDITINV' => array(SS_SALES|6, _("Sales credit notes against invoice")),
	'SA_SALESCREDIT' => array(SS_SALES|7, _("Sales freehand credit notes")),
	'SA_SALESPAYMNT' => array(SS_SALES|8, _("Customer payments entry")),
	'SA_SALESALLOC' => array(SS_SALES|9, _("Customer payments allocation")),
        'SA_CUSTOMER_SEC_SALES' => array(SS_SALES|11,  _("Sales customer and branches changes section sales")),
        'SA_SALESQUOTE_EDIT' => array(SS_SALES|12,  _("Sales quotations edition")),        
        'SA_SALESPOSCASH' => array(SS_SALES|13, _("Sales Pos Cash")),
        'SA_PRINTREPZ' => array(SS_SALES|14, _("Print Report Z")),
        'SA_READREPX' => array(SS_SALES|15, _("Read Report X")),
        'SA_OUTSTANDINGFEESPAY' => array(SS_SALES|16, _("Outstanding fees payable")),
        'SA_REVERSEFEESCALC' => array(SS_SALES|17, _("Reverse fees calculated")),
        'SA_OUTPATIENTACCOUNTSBILL' => array(SS_SALES|18, _("Outstading Patient Accounts for Billing")),
        'SA_PENDAPPFORAUTHORIZ' => array(SS_SALES|19, _("Pending Applications for Authorization")),
        'SA_APPIPENDAUTHOR' => array(SS_SALES|20, _("Application Pending Authorizations for Authorizing")),
        'SA_ENTRYCHARGES' => array(SS_SALES|21, _("Entry Charges")),
        'SA_ENTRYDELIVERYNOTES' => array(SS_SALES|22, _("Entry Delivery Notes")),
        'SA_METHODPAYMENTS' => array(SS_SALES|23, _("Method Payments")),
        'SA_MET_PAY_GROUP_ROLE' => array(SS_SALES|24, _("Method Payments Group Role")),
        'SA_SALESTRANSVIEW_POS' => array(SS_SALES|25,  _("Sales transactions POS view")),
        'SA_FIND_ITEM_STOCK' => array(SS_SALES|26,  _("Find Item Stock in sales orders")),
        'SA_FIND_RESPONSIBLE' => array(SS_SALES|27,  _("Find Responsible in sales orders")),
        'SA_SALESTRANSVIEW_TMPL' => array(SS_SALES|28,  _("Sales transactions view TMPL")),
        'SA_SALESTRANSVIEW_TMPL_QTE' => array(SS_SALES|29,  _("Sales transactions view TMPL QTE")),
        'SA_SALESTRANSVIEW_QTE_TO_QTE' => array(SS_SALES|30,  _("Sales transactions view Quotation to Quotation")),
        'SA_MET_PAY_ASSOC_DOC' => array(SS_SALES|31, _("Method Payments Association With Documents")),
        'SA_ACCOUNTSETTDIST' => array(SS_SALES|32, _("Pending outstanding accounts to do accounting distribution")),
        'SA_STATUS_RESP' => array(SS_SALES|33, _("Status responsible account patient")),
        'SA_SALESINVOICEFISCAL' => array(SS_SALES|34, _("Sales invoices edition")." - Fiscal"),
        'SA_SALESDEBIT' => array(SS_SALES|35, _("Sales debits edition")),
        'SA_SALESINVOICE_FISCAL' => array(SS_SALES|36, _("Sales invoices fiscal")),    
        'SA_SALESPOSREP' => array(SS_SALES|37, _("POS reports")),
    
	'SA_SALESANALYTIC' => array(SS_SALES_A|1, _("Sales analytical reports")),
	'SA_SALESBULKREP' => array(SS_SALES_A|2, _("Sales document bulk reports")),
	'SA_PRICEREP' => array(SS_SALES_A|3, _("Sales prices listing")),
	'SA_SALESMANREP' => array(SS_SALES_A|4, _("Sales staff listing")),
	'SA_CUSTBULKREP' => array(SS_SALES_A|5, _("Customer bulk listing")),
	'SA_CUSTSTATREP' => array(SS_SALES_A|6, _("Customer status report")),
	'SA_CUSTPAYMREP' => array(SS_SALES_A|7, _("Customer payments report")),
        'SA_MYTRANSACREP' => array(SS_SALES_A|8, _("My Transactions report")),
        'SA_REQUESTAUTH_UNCHK' => array(SS_SALES_A|9, _("Request Authorization for uncheck orders belong invoice")),
        'SA_REQUESTAUTH_CREDIT_NOTE' => array(SS_SALES_A|10, _("Request Authorization for Credit Note")),

//
// Purchase related functions
//
	'SA_PURCHASEPRICING' => array(SS_PURCH_C|1, _("Purchase price changes")),

	'SA_SUPPTRANSVIEW' => array(SS_PURCH|1, _("Supplier transactions view")),
	'SA_SUPPLIER' => array(SS_PURCH|2, _("Suppliers changes")),
	'SA_PURCHASEORDER' => array(SS_PURCH|3, _("Purchase order entry")),
	'SA_GRN' => array(SS_PURCH|4, _("Purchase receive")),
	'SA_SUPPLIERINVOICE' => array(SS_PURCH|5, _("Supplier invoices")),
	'SA_GRNDELETE' => array(SS_PURCH|9, _("Deleting GRN items during invoice entry")),
	'SA_SUPPLIERCREDIT' => array(SS_PURCH|6, _("Supplier credit notes")),
	'SA_SUPPLIERPAYMNT' => array(SS_PURCH|7, _("Supplier payments")),
	'SA_SUPPLIERALLOC' => array(SS_PURCH|8, _("Supplier payments allocations")),

	'SA_SUPPLIERANALYTIC' => array(SS_PURCH_A|1, _("Supplier analytical reports")),
	'SA_SUPPBULKREP' => array(SS_PURCH_A|2, _("Supplier document bulk reports")),
	'SA_SUPPPAYMREP' => array(SS_PURCH_A|3, _("Supplier payments report")),
//
// Inventory 
//
	'SA_ITEM' => array(SS_ITEMS_C|1, _("Stock items add/edit")),
	'SA_SALESKIT' => array(SS_ITEMS_C|2, _("Sales kits")),
	'SA_ITEMCATEGORY' => array(SS_ITEMS_C|3, _("Item categories")),
	'SA_UOM' => array(SS_ITEMS_C|4, _("Units of measure")),
        'SA_BATCHCHANGEPRICE' => array(SS_ITEMS_C|5, _("Batch change price")),

	'SA_ITEMSSTATVIEW' => array(SS_ITEMS|1, _("Stock status view")),
	'SA_ITEMSTRANSVIEW' => array(SS_ITEMS|2, _("Stock transactions view")),
	'SA_FORITEMCODE' => array(SS_ITEMS|3, _("Foreign item codes entry")),
	'SA_LOCATIONTRANSFER' => array(SS_ITEMS|4, _("Inventory location transfers")),
	'SA_INVENTORYADJUSTMENT' => array(SS_ITEMS|5, _("Inventory adjustments")),

	'SA_REORDER' => array(SS_ITEMS_A|1, _("Reorder levels")),
	'SA_ITEMSANALYTIC' => array(SS_ITEMS_A|2, _("Items analytical reports and inquiries")),
	'SA_ITEMSVALREP' => array(SS_ITEMS_A|3, _("Inventory valuation report")),

//
// Manufacturing module 
//
	'SA_BOM' => array(SS_MANUF_C|1, _("Bill of Materials")),

	'SA_MANUFTRANSVIEW' => array(SS_MANUF|1, _("Manufacturing operations view")),
	'SA_WORKORDERENTRY' => array(SS_MANUF|2, _("Work order entry")),
	'SA_MANUFISSUE' => array(SS_MANUF|3, _("Material issues entry")),
	'SA_MANUFRECEIVE' => array(SS_MANUF|4, _("Final product receive")),
	'SA_MANUFRELEASE' => array(SS_MANUF|5, _("Work order releases")),

	'SA_WORKORDERANALYTIC' => array(SS_MANUF_A|1, _("Work order analytical reports and inquiries")),
	'SA_WORKORDERCOST' => array(SS_MANUF_A|2, _("Manufacturing cost inquiry")),
	'SA_MANUFBULKREP' => array(SS_MANUF_A|3, _("Work order bulk reports")),
	'SA_BOMREP' => array(SS_MANUF_A|4, _("Bill of materials reports")),
//
// Dimensions
//
	'SA_DIMTAGS' => array(SS_DIM_C|1, _("Dimension tags")),

	'SA_DIMTRANSVIEW' => array(SS_DIM|1, _("Dimension view")),

	'SA_DIMENSION' => array(SS_DIM|2, _("Dimension entry")),

	'SA_DIMENSIONREP' => array(SS_DIM|3, _("Dimension reports")),
//
// Banking and General Ledger
//
	'SA_ITEMTAXTYPE' => array(SS_GL_C|1, _("Item tax type definitions")),
	'SA_GLACCOUNT' => array(SS_GL_C|2, _("GL accounts edition")),
	'SA_GLACCOUNTGROUP' => array(SS_GL_C|3, _("GL account groups")),
	'SA_GLACCOUNTCLASS' => array(SS_GL_C|4, _("GL account classes")),
	'SA_QUICKENTRY' => array(SS_GL_C|5, _("Quick GL entry definitions")),
	'SA_CURRENCY' => array(SS_GL_C|6, _("Currencies")),
	'SA_BANKACCOUNT' => array(SS_GL_C|7, _("Bank accounts")),
	'SA_TAXRATES' => array(SS_GL_C|8, _("Tax rates")),
	'SA_TAXGROUPS' => array(SS_GL_C|12, _("Tax groups")),
	'SA_FISCALYEARS' => array(SS_GL_C|9, _("Fiscal years maintenance")),
	'SA_GLSETUP' => array(SS_GL_C|10, _("Company GL setup")),
	'SA_GLACCOUNTTAGS' => array(SS_GL_C|11, _("GL Account tags")),
	'SA_MULTIFISCALYEARS' => array(SS_GL_C|13, _("Allow entry on non closed Fiscal years")),

	'SA_BANKTRANSVIEW' => array(SS_GL|1, _("Bank transactions view")),
	'SA_GLTRANSVIEW' => array(SS_GL|2, _("GL postings view")),
	'SA_EXCHANGERATE' => array(SS_GL|3, _("Exchange rate table changes")),
	'SA_PAYMENT' => array(SS_GL|4, _("Bank payments")),
	'SA_DEPOSIT' => array(SS_GL|5, _("Bank deposits")),
	'SA_BANKTRANSFER' => array(SS_GL|6, _("Bank account transfers")),
	'SA_RECONCILE' => array(SS_GL|7, _("Bank reconciliation")),
	'SA_JOURNALENTRY' => array(SS_GL|8, _("Manual journal entries")),
	'SA_BANKJOURNAL' => array(SS_GL|11, _("Journal entries to bank related accounts")),
	'SA_BUDGETENTRY' => array(SS_GL|9, _("Budget edition")),
	'SA_STANDARDCOST' => array(SS_GL|10, _("Item standard costs")),
	'SA_ACCRUALS' => array(SS_GL|12, _("Revenue / Cost Accruals")),

	'SA_GLANALYTIC' => array(SS_GL_A|1, _("GL analytical reports and inquiries")),
	'SA_TAXREP' => array(SS_GL_A|2, _("Tax reports and inquiries")),
	'SA_BANKREP' => array(SS_GL_A|3, _("Bank reports and inquiries")),
	'SA_GLREP' => array(SS_GL_A|4, _("GL reports and inquiries")),
//
// Administracion
//  
        'SA_PATIENT_ACCOUNT' => array(SS_ADM|1, _("Cuentas Paciente")),
        'SA_ACCOUNT_OPENED' => array(SS_ADM|2, _("Patient Account Opened")),
        'SA_PATIENT_ACCOUNT_VOIDED' => array(SS_ADM|3, _("Cuentas Paciente Anuladas")),
//
// Sales Menus
// Created by Felix Alberti On November 2016
//
        'SA_ENTRYDELIVERYNOTES_MNU' => array(SS_MENU_SALES|1, "Menu "._("Entry Delivery Notes")),
        'SA_ENTRYCHARGES_MNU' => array(SS_MENU_SALES|2, "Menu "._("Entry Charges")),
    
        'SA_CUST_MODIFY_BDT' => array(SS_CUSTOMERS_C|1, _("Modify Basic Data")),
        'SA_CUST_MODIFY_MEDPRI' => array(SS_CUSTOMERS_C|2, _("Modify Medico Primario")),
    
//Account Patient    
        'SA_ALLOW_TRANSF_ORDER_ACC' => array(SS_ACC_PAT|1, _("Allow Transfer An Order From One Account To Other")),
        'SA_ALLOW_TRANSF_PAYMENT_ACC' => array(SS_ACC_PAT|2, _("Allow Transfer Payment From One Account To Other")),
    
//Medical Appoinment   
        'SA_MEDICAL_APPOINMENT_BOOK' => array(SS_MEDICAL_APPOIN|1, _("Medical appointment book")),
        'SA_PATRON' => array(SS_MEDICAL_APPOIN|2, _("Patron")),
        'SA_MEDICAL_APPOINMENT_NUMBERXHOUR' => array(SS_MEDICAL_APPOIN|3, _("Number/Hour")),
        'SA_MEDICAL_APPOINMENT_SPECIALITY' => array(SS_MEDICAL_APPOIN|4, _("Especialidad")),
        'SA_MEDICAL_APPOINMENT_UNIDFUNC' => array(SS_MEDICAL_APPOIN|5, _("Unidad Funcional")),
        'SA_MEDICAL_APPOINMENT_CLINIQUE' => array(SS_MEDICAL_APPOIN|6, _("Clinique")),
        'SA_MEDICAL_APPOINMENT_SYSTEMTABLES' => array(SS_MEDICAL_APPOIN|7, _("System Tables")),
        'SA_MEDICAL_APPOINMENT_TYPEAPPOINTMED' => array(SS_MEDICAL_APPOIN|8, _("Type Appointment by medico")),
        'SA_MEDICAL_APPOINMENT_DAYSWORKMEDICO' => array(SS_MEDICAL_APPOIN|9, _("Days Work by Medico")),
        'SA_MEDICAL_APPOINMENT_RELMEDPATRONBYDAY' => array(SS_MEDICAL_APPOIN|10, _("Relation Medico Patron By Day")),
        'SA_MEDICAL_APPOINMENT_PROCESSCALENDARY' => array(SS_MEDICAL_APPOIN|10, _("Process Calendary")),
        'SA_MEDICAL_APPOINMENT_REP_BOOK' => array(SS_MEDICAL_APPOIN|11, _("Book Appointment")),
    
        'SA_APS_OPENED'  => array(SS_SALES_APS|1,  _("Patient APS Opened")),
        'SA_PATIENT_APS' => array(SS_SALES_APS|2, _("Patient APS")),
);
/*
	This function should be called whenever we want to extend core access level system
	with new security areas and/or sections i.e.: 
	. on any page with non-standard security areas
	. in security roles editor
	The call should be placed between session.inc inclusion and page() call.
	Up to 155 security sections and 155 security areas for any extension can be installed.
*/
function add_access_extensions()
{
	global $security_areas, $security_sections, $installed_extensions;

	foreach($installed_extensions as $extid => $ext) {
		$accext = hook_invoke($ext['package'], 'install_access', $dummy);
		if ($accext == null) continue;

		$scode = 100;
		$acode = 100;
		$extsections = $accext[1];
		$extareas = $accext[0];
		$extcode = $extid<<16;
		
		$trans = array();
		foreach($extsections as $code =>$name) {
			$trans[$code] = $scode<<8;
			// reassign section codes
			$security_sections[$trans[$code]|$extcode] = $name;
			$scode++;
		}
		foreach($extareas as $code => $area) {
			$section = $area[0]&0xff00;
			// extension modules:
			// if area belongs to nonstandard section
			// use translated section codes and
			// preserve lower part of area code
			if (isset($trans[$section])) {
				$section = $trans[$section];
			} 
				// otherwise assign next available
				// area code >99
			$area[0] = $extcode | $section | ($acode++);
			$security_areas[$code] = $area;
		}
	}
}
/*
	Helper function to retrieve extension access definitions in isolated environment.
*/
/*
function get_access_extensions($id) {
	global $path_to_root, $installed_extensions;
	
	$ext = $installed_extensions[$id];
	
	$security_sections = $security_areas = array();
	
	if (isset($ext['acc_file']))
		include_once($path_to_root.'/'.$ext['path'].'/'.$ext['acc_file']);

	return array($security_areas, $security_sections);
}
*/
?>