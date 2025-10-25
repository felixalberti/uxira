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
class customers_app extends application 
{
	function customers_app() 
	{       
                $hoy = fecha_hoy(AMERICA_CARACAS);
                $fecha = strtotime($hoy);
                $fecha_actual=date("Y-m-d",$fecha);
                $user = $_SESSION["wa_current_user"]->username;
		$this->application("orders", T_gettext($this->help_context = "&Sales"));
		//$prueba = T_gettext('Fee released');
	    //$prueba = $_SESSION['language']->get_current_language_dir();
		//display_notification($prueba);
		$this->add_module(T_gettext("Transactions"));
		/*$this->add_lapp_function(0, _("Sales Quotation Entry"),
			"sales/sales_order_entry.php?NewQuotation=Yes", 'SA_SALESQUOTE', MENU_TRANSACTION);*/
		$this->add_lapp_function(0, _("Entrada Presupuesto"),
			"sales/sales_order_entry.php?NewQuotation=Yes", 'SA_SALESQUOTE', MENU_TRANSACTION);
//		$this->add_lapp_function(0, _("Sales &Order Entry"),
//			"sales/sales_order_entry.php?NewOrder=Yes", 'SA_SALESORDER', MENU_TRANSACTION);
//		$this->add_lapp_function(0, _("Direct &Delivery"),
//			"sales/sales_order_entry.php?NewDelivery=0", 'SA_SALESDELIVERY', MENU_TRANSACTION);
                
                $pos = get_sales_point(user_pos());
                $paymcat = !$pos['cash_sale'] ? PM_CREDIT : (!$pos['credit_sale'] ? PM_CASH : PM_ANY);
               
                if ($paymcat == PM_CREDIT || $paymcat == PM_ANY)
		$this->add_lapp_function(0, T_gettext("Direct &Invoice"),
			"sales/sales_order_entry.php?NewInvoice=0", 'SA_SALESINVOICE', MENU_TRANSACTION);
                
                $this->add_lapp_function(0, T_gettext("Direct &Debit"),
			"sales/debit_note_entry.php?NewDebit=0", 'SA_SALESDEBIT', MENU_TRANSACTION);
                if ($paymcat != PM_CREDIT)
                $this->add_lapp_function(0, T_gettext("Direct &Invoice")." Fiscal",
			"sales/sales_order_entry.php?NewInvoice=0&Print_Fisc=1", 'SA_SALESINVOICEFISCAL', MENU_TRANSACTION);
		$this->add_lapp_function(0, "","");
                $this->add_lapp_function(0, T_gettext("Patient Account Opened"),
			"sales/inquiry/patient_accounts_opened.php?NewAdm=1", 'SA_ACCOUNT_OPENED', MENU_TRANSACTION);
                if ($_SESSION['wa_current_user']->is_user_invoice == 1)
                $this->add_lapp_function(0, _("APS-Patient Account Opened"),
			"sales/inquiry/patient_accounts_opened.php?NewAdm=1&aps=1", 'SA_ACCOUNT_OPENED', MENU_TRANSACTION);
                /*$this->add_lapp_function(0, _("Entry Charges"),
			"sales/sales_cargo_entrega_entry.php?NewOrder=Yes", 'SA_ENTRYCHARGES_MNU', MENU_TRANSACTION);*/
			$this->add_lapp_function(0, _("Cargos Facturacion Caja"),
			"sales/sales_cargo_entrega_entry.php?NewOrder=Yes", 'SA_ENTRYCHARGES_MNU', MENU_TRANSACTION);
                $this->add_lapp_function(0, T_gettext("Entry Delivery Notes"),
			"sales/sales_cargo_entrega_entry.php?NewDelivery=0", 'SA_ENTRYDELIVERYNOTES_MNU', MENU_TRANSACTION);
                 $this->add_lapp_function(0, _("Services Notes (APS)"),
			"sales/sales_cargo_entrega_entry.php?NewDelivery=0&aps=1", 'SA_ENTRYDELIVERYNOTES_MNU', MENU_TRANSACTION);
                $this->add_lapp_function(0, "","");
                
                  
                $this->add_lapp_function(0, T_gettext("Search All Sales Quotations"),
			"sales/inquiry/sales_orders_view.php?type=32", 'SA_SALESDELIVERY', MENU_TRANSACTION);
                
                
		$this->add_lapp_function(0, T_gettext("&Delivery Against Sales Orders")." *",
			"sales/inquiry/sales_orders_view.php?OutstandingOnly=1", 'SA_SALESDELIVERY', MENU_TRANSACTION);
                
              
                
                /*Begin Félix Alberti 23/06/2016*/
//                $this->add_lapp_function(0, _("&Delivery Against Sales Orders"),
//			"sales/inquiry/sales_orders_view_angios.php?OutstandingOnly=1", 'SA_SALESDELIVERY', MENU_TRANSACTION);
                /*End Félix Alberti 23/06/2016*/                
                $count_notifications = get_sql_for_sales_deliveries_view_count($fecha_actual,$fecha_actual);
		/*$this->add_lapp_function(0, _("&Invoice Against Sales Delivery"),
			"sales/inquiry/sales_deliveries_view.php?OutstandingOnly=1&cash=1", 'SA_SALESINVOICE', MENU_TRANSACTION, $count_notifications, 'red');*/
		$this->add_lapp_function(0, _("Facturacion Caja"),
			"sales/inquiry/sales_deliveries_view.php?OutstandingOnly=1&cash=1", 'SA_SALESINVOICE', MENU_TRANSACTION, $count_notifications, 'red');
                
                /*$this->add_lapp_function(0, _("&Invoice Against Sales Delivery2"),
			"sales/inquiry/sales_deliveries_view_fiscal.php?OutstandingOnly=1", 'SA_SALESINVOICE', MENU_TRANSACTION);*/
                $count_notifications = get_sql_patient_account_view_count($fecha_actual);
                $this->add_lapp_function(0, _("Cuenta Paciente"),
			"sales/inquiry/find_patient_accounts.php?NewAdm=1", 'SA_PATIENT_ACCOUNT', MENU_TRANSACTION, $count_notifications, 'red');
                
				 $this->add_lapp_function(0, _("Cuenta Paciente Facturadas"),
			"sales/inquiry/find_patient_accounts_fact.php?NewAdm=1", 'SA_PATIENT_ACCOUNT', MENU_TRANSACTION, 0, 'red');
			
			/*	 $this->add_lapp_function(0, _("Cuenta Paciente Sin Facturar"),
			"sales/inquiry/find_patient_accounts_sinfact.php?NewAdm=1", 'SA_PATIENT_ACCOUNT', MENU_TRANSACTION, 0, 'red');*/
                
				
                $this->add_lapp_function(0, _("Estado de Cuenta Pacientes"),
			"sales/inquiry/patient_accounts_list.php?NewAdm=1", 'SA_PATIENT_ACCOUNT', MENU_TRANSACTION);
                
                   
                $count_notifications = get_sql_for_sales_deliveries_view_count($fecha_actual,$fecha_actual);
                $this->add_lapp_function(0, _("Pacientes pago pendiente"),
			"sales/inquiry/customer_nohave_payment.php?", 'SA_SALESTRANSVIEW', MENU_INQUIRY,$count_notifications,'red');
                //$count_notifications = get_sql_for_sales_order_view_count($fecha_actual,$user);
                
                $this->add_lapp_function(0, _("Cuenta Paciente Anuladas"),
			"sales/inquiry/find_patient_accounts_voided.php?NewAdm=1", 'SA_PATIENT_ACCOUNT_VOIDED', MENU_TRANSACTION);
                /*Begin fap 24/11/2015*/
                /*Begin Félix Alberti 07/05/2016*/
                $this->add_lapp_function(0, T_gettext("Outstading Patient Accounts for Billing"),
			"sales/inquiry/inquiry_account_pend_alloc_sales_invoice.php?", 'SA_OUTPATIENTACCOUNTSBILL', MENU_MAINTENANCE);
                /*End Félix Alberti 07/05/2016*/
                /*Begin Félix Alberti 18/11/2016*/
                /*$this->add_lapp_function(0, _("Pending outstanding accounts to do accounting distribution"),
			"sales/inquiry/inquiry_account_invoice_pend_dist.php?", 'SA_ACCOUNTSETTDIST', MENU_MAINTENANCE);*/
                /*End Félix Alberti 18/11/2016*/
                /*Begin Félix Alberti 08/06/2016*/
                $this->add_lapp_function(0, T_gettext("Sales Pos Cash"),
			"sales/manage/sales_pos_cash.php?", 'SA_SALESPOSCASH', MENU_MAINTENANCE);
                /*End Félix Alberti 08/06/2016*/
                
                $count_notifications_aps = get_sql_for_sales_deliveries_aps_view_count($fecha_actual,$fecha_actual);
                
                if ($pos['type_impression']=='A'){
                   $this->add_lapp_function(0, T_gettext("&Invoice(aps-fisc) Against Sales Delivery"),
			"sales/inquiry/sales_deliveries_view_aps.php?OutstandingOnly=1&type_impr=F", 'SA_SALESINVOICE', MENU_TRANSACTION, $count_notifications_aps, 'red');
                   $this->add_lapp_function(0, T_gettext("&Invoice(aps-pdf) Against Sales Delivery"),
			"sales/inquiry/sales_deliveries_view_aps.php?OutstandingOnly=1&type_impr=P", 'SA_SALESINVOICE', MENU_TRANSACTION, $count_notifications_aps, 'red'); 
                }
                else
                $this->add_lapp_function(0, T_gettext("&Invoice(aps) Against Sales Delivery"),
			"sales/inquiry/sales_deliveries_view_aps.php?OutstandingOnly=1", 'SA_SALESINVOICE', MENU_TRANSACTION, $count_notifications_aps, 'red');
                
                $this->add_lapp_function(0, T_gettext("APS Paciente"),
			"sales/inquiry/find_patient_aps.php", 'SA_PATIENT_APS', MENU_TRANSACTION);
                
                $this->add_lapp_function(0, T_gettext("Patient APS Opened"),
			"sales/inquiry/patient_aps_opened.php?NewAdm=1", 'SA_APS_OPENED', MENU_TRANSACTION);
                
                 $this->add_lapp_function(0, T_gettext("Adjustment Porc. Honorary"),
			"sales/inquiry/adjusment_porc_honorary.php?NewAdm=1", 'SA_SALESPAYMNT', MENU_TRANSACTION);
                
                
//                $this->add_rapp_function(0, _("Search All Sales Quotation"),
//			"sales/inquiry/sales_orders_view.php?type=32", 'SA_SALESDELIVERY', MENU_TRANSACTION);
                /*End fap 24/11/2015*/
                /*Begin fap 30/12/2015*/
//                $this->add_rapp_function(0, _("&Quotation to Template Quotation"),
//			"sales/inquiry/sales_orders_view_tmpl.php?type=32&QuoteToNewQuote=Yes", 'SA_SALESTRANSVIEW_TMPL', MENU_TRANSACTION);
                /*End fap 30/12/2015*/
                /*Begin fap 11/08/2016*/
                /*$this->add_rapp_function(0, _("&Template Quotation to New Quotation"),
			"sales/inquiry/sales_orders_view_tmpl_to_quotation.php?type=32&QuoteToNewQuote=Yes", 'SA_SALESTRANSVIEW_TMPL_QTE', MENU_TRANSACTION);*/
			$this->add_rapp_function(0, _("Presupuesto desde template"),
			"sales/inquiry/sales_orders_view_tmpl_to_quotation.php?type=32&QuoteToNewQuote=Yes", 'SA_SALESTRANSVIEW_TMPL_QTE', MENU_TRANSACTION);
                /*End fap 11/08/2016*/
		$this->add_rapp_function(0, _("&Template Delivery"),
			"sales/inquiry/sales_orders_view.php?DeliveryTemplates=Yes", 'SA_SALESDELIVERY', MENU_TRANSACTION);
		$this->add_rapp_function(0, _("&Template Invoice"),
			"sales/inquiry/sales_orders_view.php?InvoiceTemplates=Yes", 'SA_SALESINVOICE', MENU_TRANSACTION);
		$this->add_rapp_function(0, _("&Create and Print Recurrent Invoices"),
			"sales/create_recurrent_invoices.php?", 'SA_SALESINVOICE', MENU_TRANSACTION);
		$this->add_rapp_function(0, "","");
		$this->add_rapp_function(0, _("Customer &Payments"),
			"sales/customer_payments.php?", 'SA_SALESPAYMNT', MENU_TRANSACTION);
                /*Begin fap 07/07/2015*/
                $this->add_rapp_function(0, _("Facturas por pagar profesionales"),
			"sales/inquiry/pay_medical_invoices.php", 'SA_OUTSTANDINGFEESPAY', MENU_TRANSACTION);
                /*End fap 07/07/2015*/
                /*Begin fap 07/07/2015*/
                $this->add_rapp_function(0, _("Generar Facturas por pagar profesionales"),
			"sales/inquiry/pay_medical_invoices_manual.php", 'SA_OUTSTANDINGFEESPAY', MENU_TRANSACTION);
                /*End fap 07/07/2015*/
                /*Begin fap 23/07/2015*/
                $this->add_rapp_function(0, _("Reversar Facturas por pagar profesionales"),
			"sales/inquiry/reversar_prenomina.php", 'SA_REVERSEFEESCALC', MENU_TRANSACTION);
                /*End fap 23/07/2015*/
		/*Begin fap 23/06/2015*/
		$this->add_rapp_function(0, "Pagos a profesionales",
			"sales/pagos_a_medicos.php", 'SA_OUTSTANDINGFEESPAY', MENU_TRANSACTION);	
                /*End fap 23/06/2015*/
                /*Begin fap 23/07/2015*/
		$this->add_rapp_function(0, "Reverso de Pagos a profesionales",
			"sales/reverso_pagos_a_medicos.php", 'SA_REVERSEFEESCALC', MENU_TRANSACTION);	
                /*End fap 23/07/2015*/	
		$this->add_rapp_function(0, _("Customer &Credit Notes"),
			"sales/credit_note_entry.php?NewCredit=Yes", 'SA_SALESCREDIT', MENU_TRANSACTION);
//		$this->add_rapp_function(0, _("&Allocate Customer Payments or Credit Notes"),
//			"sales/allocations/customer_allocation_main.php?", 'SA_SALESALLOC', MENU_TRANSACTION);
                $this->add_rapp_function(0, _("&Allocate Customer Payments or Credit Notes")." *",
			"sales/allocations/customer_allocation_main_uxira.php?", 'SA_SALESALLOC', MENU_TRANSACTION);
                
//                $this->add_rapp_function(0, _("&Allocate Patient Account Payments"),
//			"sales/allocations/patient_account_allocation.php?", 'SA_SALESALLOC', MENU_TRANSACTION);

		$this->add_module(T_gettext("Inquiries and Reports"));
//		$this->add_lapp_function(1, _("Sales Quotation I&nquiry"),
//			"sales/inquiry/sales_orders_view.php?type=32", 'SA_SALESTRANSVIEW', MENU_INQUIRY);
                //***************
                $count_notifications = get_sql_for_quotations_view_count($fecha_actual,null);
               /* $this->add_lapp_function(1, _("&Quotation to Template Quotation"),
			"sales/inquiry/sales_orders_view_tmpl.php?type=32&QuoteToNewQuote=Yes", 'SA_SALESTRANSVIEW_TMPL', MENU_INQUIRY, $count_notifications, 'green');*/
			 $this->add_lapp_function(1, _("Convierte Presupuesto enTemplate"),
			"sales/inquiry/sales_orders_view_tmpl.php?type=32&QuoteToNewQuote=Yes", 'SA_SALESTRANSVIEW_TMPL', MENU_INQUIRY, $count_notifications, 'green');
                
                /*Begin fap 14/10/2016*/
                /*$this->add_lapp_function(1, _("&Quotation to New Quotation"),
			"sales/inquiry/sales_orders_view_quotation_to_quotation.php?type=32&QuoteToNewQuote=Yes", 'SA_SALESTRANSVIEW_QTE_TO_QTE', MENU_TRANSACTION);*/
			$this->add_lapp_function(1, _("Nuevo presupuesto desde(otro)"),
			"sales/inquiry/sales_orders_view_quotation_to_quotation.php?type=32&QuoteToNewQuote=Yes", 'SA_SALESTRANSVIEW_QTE_TO_QTE', MENU_TRANSACTION);
                /*End fap 14/10/2016*/
				
					$this->add_lapp_function(1, _("Items Fuera Dimensiones"),
			"sales/inquiry/items_out_dimensions.php?type=32&QuoteToNewQuote=Yes", 'SA_SALESTRANSVIEW_QTE_TO_QTE', MENU_TRANSACTION);
                
		$this->add_lapp_function(1, T_gettext("Sales Order &Inquiry"),
			"sales/inquiry/sales_orders_view.php?type=30", 'SA_SALESTRANSVIEW', MENU_INQUIRY);
                $count_notifications = get_sql_for_sales_order_view_count($fecha_actual,null);
		$this->add_lapp_function(1, T_gettext("Customer Transaction &Inquiry"),
			"sales/inquiry/customer_inquiry.php?", 'SA_SALESTRANSVIEW', MENU_INQUIRY,$count_notifications,'green');
               
                
                
                $count_notifications = get_sql_for_sales_order_view_count($fecha_actual,$user);
                
                $this->add_lapp_function(1, T_gettext("My Transactions"),
			"sales/inquiry/customer_inquiry_pos.php?", 'SA_SALESTRANSVIEW_POS', MENU_INQUIRY, $count_notifications,'green');
		$this->add_lapp_function(1, "","");
		$this->add_lapp_function(1, T_gettext("Customer Allocation &Inquiry"),
			"sales/inquiry/customer_allocation_inquiry.php?", 'SA_SALESALLOC', MENU_INQUIRY);
                
                $this->add_lapp_function(1, T_gettext("Medicos Allocation Invoice"),
			"sales/inquiry/medicos_allocation_invoice.php?", 'SA_SALESALLOC', MENU_INQUIRY);

		$this->add_rapp_function(1, T_gettext("Customer and Sales &Reports"),
			"reporting/reports_main.php?Class=0", 'SA_SALESTRANSVIEW', MENU_REPORT);
                /*Begin Felix Alberti 27/06/2016*/
                $this->add_rapp_function(1, T_gettext("Print report Z"),
			"sales/manage/print_report_z.php", 'SA_PRINTREPZ', MENU_REPORT);
                $this->add_rapp_function(1, T_gettext("Read report X"),
			"sales/manage/print_read_x.php", 'SA_READREPX', MENU_REPORT);
                /*End Felix Alberti 27/06/2016*/
				
		$this->add_module(T_gettext("Maintenance"));
		/*$this->add_lapp_function(2, _("Add and Manage &Customers"),
			"sales/manage/customers.php?", 'SA_CUSTOMER', MENU_ENTRY);*/
                $this->add_lapp_function(2, T_gettext("Customers"),
			"sales/manage/customers.php?", 'SA_CUSTOMER', MENU_ENTRY);
		/*$this->add_lapp_function(2, _("Customer &Branches"),
			"sales/manage/customer_branches.php?", 'SA_CUSTOMER', MENU_ENTRY);*/
                $this->add_lapp_function(2, T_gettext("&Filiales o Sucursales"),
			"sales/manage/customer_branches.php?", 'SA_CUSTOMER', MENU_ENTRY);
		$this->add_lapp_function(2, _("Sales &Groups"),
			"sales/manage/sales_groups.php?", 'SA_SALESGROUP', MENU_MAINTENANCE);
		$this->add_lapp_function(2, _("Recurrent &Invoices"),
			"sales/manage/recurrent_invoices.php?", 'SA_SRECURRENT', MENU_MAINTENANCE);
                /*Begin Félix Alberti 28/07/2015*/
//                $this->add_lapp_function(2, _("Medicos"),
//			"sales/manage/medicos.php?", 'SA_SRECURRENT', MENU_MAINTENANCE);
                /*End Félix Alberti 28/07/2015*/
                /*Begin Félix Alberti 12/08/2015*/
                $this->add_lapp_function(2, utf8_decode(_("Profesionales y Técnicos")),                        
			"sales/manage/professional.php?", 'SA_SRECURRENT', MENU_MAINTENANCE);
                /*End Félix Alberti 12/08/2015*/
                 /*Begin Félix Alberti 12/08/2015*/
                $this->add_lapp_function(2, _("Compania de Seguros"),                        
			"sales/manage/insurance_company.php?", 'SA_SRECURRENT', MENU_MAINTENANCE);
                /*End Félix Alberti 12/08/2015*/
		$this->add_rapp_function(2, T_gettext("Sales T&ypes"),
			"sales/manage/sales_types.php?", 'SA_SALESTYPES', MENU_MAINTENANCE);
		$this->add_rapp_function(2, T_gettext("Sales &Persons"),
			"sales/manage/sales_people.php?", 'SA_SALESMAN', MENU_MAINTENANCE);
		$this->add_rapp_function(2, T_gettext("Sales &Areas"),
			"sales/manage/sales_areas.php?", 'SA_SALESAREA', MENU_MAINTENANCE);
                $this->add_rapp_function(2, T_gettext("Categoria de Ventas"),
			"sales/manage/category_sales.php?", 'SA_SALESTYPES', MENU_MAINTENANCE);
		$this->add_rapp_function(2, T_gettext("Credit &Status Setup"),
			"sales/manage/credit_status.php?", 'SA_CRSTATUS', MENU_MAINTENANCE);
                /*Begin Félix Alberti 03/05/2016*/
                $this->add_rapp_function(2, T_gettext("Method Payments"),
			"sales/manage/method_payments.php?", 'SA_METHODPAYMENTS', MENU_MAINTENANCE);
                $this->add_rapp_function(2, T_gettext("Metodos Pagos Assoc. Document"),
			"sales/manage/method_payments_assoc_doc.php?", 'SA_MET_PAY_ASSOC_DOC', MENU_MAINTENANCE);
                $this->add_rapp_function(2, T_gettext("Method Payments Group Role"),
			"sales/manage/method_payments_group_role.php?", 'SA_MET_PAY_GROUP_ROLE', MENU_MAINTENANCE);
                /*End Félix Alberti 03/05/2016*/
                /*Begin Félix Alberti 04/05/2016*/
                $this->add_rapp_function(2, T_gettext("Pending Applications for Authorization"),
			"sales/inquiry/inquiry_request_pend_authoriz.php?", 'SA_PENDAPPFORAUTHORIZ', MENU_MAINTENANCE);
                /*End Félix Alberti 04/05/2016*/
                 /*Begin Félix Alberti 14/06/2016*/
                $this->add_rapp_function(2, T_gettext("Application Pending Authorizations for Authorizing"),
			"sales/inquiry/inquiry_authorizations_credit_pend_authoriz.php?", 'SA_APPIPENDAUTHOR', MENU_MAINTENANCE);
                /*End Félix Alberti 14/06/2016*/
                /*Begin Félix Alberti 25/01/2017*/
                $this->add_rapp_function(2, T_gettext("Status Responsible"),
			"sales/manage/status_responsible.php?", 'SA_STATUS_RESP', MENU_MAINTENANCE);
                /*End Félix Alberti 25/01/2017*/
                 /*Begin Félix Alberti 27/01/2017*/
                $this->add_rapp_function(2, T_gettext("Status Responsible Secuencial"),
			"sales/manage/status_responsible_secuential.php?", 'SA_STATUS_RESP', MENU_MAINTENANCE);
                /*End Félix Alberti 27/01/2017*/
                
                /*$this->add_rapp_function(2, _("Specialties"),
			"sales/manage/medical_specialties.php?", 'SA_SALESAREA', MENU_MAINTENANCE);*/
                 $this->add_rapp_function(2, T_gettext("Especialidades Medicas"),
			"sales/manage/medical_specialties.php?", 'SA_SALESAREA', MENU_MAINTENANCE);
                
		$this->add_extensions();
	}
}


?>