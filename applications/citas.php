<?php
/**********************************************************************
    Copyright (C) Uxira, CA.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
	class citas_app extends application 
	{
		function citas_app() 
		{
			global $installed_modules;
			$this->application("citas",T_gettext("Appoinment Medique"));
		
			$this->add_module(_("Transactions"));
			/*$this->add_lapp_function(0, _("Sales &Order Entry"),"sales/sales_order_entry.php?NewOrder=Yes");
			$this->add_lapp_function(0, _("Direct &Delivery"),"sales/sales_order_entry.php?NewDelivery=0");			
			$this->add_lapp_function(0, _("Direct &Invoice"),"sales/sales_order_entry.php?NewInvoice=0");
			$this->add_lapp_function(0, "","");
			$this->add_lapp_function(0, _("&Delivery Against Sales Orders"),"sales/inquiry/sales_orders_view.php?OutstandingOnly=1");
			$this->add_lapp_function(0, _("&Invoice Against Sales Delivery"),"sales/inquiry/sales_deliveries_view.php?OutstandingOnly=1");

			$this->add_rapp_function(0, _("&Template Delivery"),"sales/inquiry/sales_orders_view.php?DeliveryTemplates=Yes");
			$this->add_rapp_function(0, _("&Template Invoice"),"sales/inquiry/sales_orders_view.php?InvoiceTemplates=Yes");
			$this->add_rapp_function(0, _("&Create and Print Recurrent Invoices"),"sales/create_recurrent_invoices.php?");
			$this->add_rapp_function(0, "","");
			$this->add_rapp_function(0, _("Customer &Payments"),"sales/customer_payments.php?");
			$this->add_rapp_function(0, _("Customer &Credit Notes"),"sales/credit_note_entry.php?NewCredit=Yes");
			$this->add_rapp_function(0, _("&Allocate Customer Payments or Credit Notes"),"sales/allocations/customer_allocation_main.php?");*/

			$this->add_module(_("Inquiries and Reports"));
			/*$this->add_lapp_function(1, _("Sales Order &Inquiry"),"sales/inquiry/sales_orders_view.php?");
			$this->add_lapp_function(1, _("Customer Transaction &Inquiry"),"sales/inquiry/customer_inquiry.php?");
			//$this->add_lapp_function(1, "","");
			$this->add_lapp_function(1, _("Honorarios Medicos"),"sales/inquiry/honorarios_medicos.php?");
			$this->add_lapp_function(1, _("Liquidaci�n Honorarios Medicos"),"sales/inquiry/pago_honorarios_medicos.php?");
			$this->add_lapp_function(1, _("Customer Allocation &Inquiry"),"sales/inquiry/customer_allocation_inquiry.php?");
			$this->add_lapp_function(1, _("Cruce Pagos NC con Facturas de M�dicos"),"sales/inquiry/medico_allocation_inquiry.php?");
			
			$this->add_rapp_function(1, _("Customer and Sales &Reports"),"reporting/reports_main.php?Class=0");*/
			
			$this->add_module(_("Maintenance"));
			$this->add_lapp_function(2, _("Sala de Espera"),"citas/manage/libro_citas.php?", 'SA_MEDICAL_APPOINMENT_BOOK', MENU_MAINTENANCE);	  
                        
			//$this->add_lapp_function(2, _("Citas gesti�n total"),"citas/manage/citas_gestion_total.php?");
			//$this->add_lapp_function(2, _("Especialidades"),"citas/manage/especialidades.php?");	      				  
      $this->add_lapp_function(2, _("Maestro de patrones"),"citas/manage/patron.php?");	      			
      $this->add_lapp_function(2, _("Unidad funcional"),"citas/manage/unidad_funcional.php?");	      			      
      $this->add_lapp_function(2, _("Clinica"),"citas/manage/clinica.php?");	      			            
      $this->add_lapp_function(2, _("Tablas Sistema"),"citas/manage/tablas_grupo_pag.php?");			
      //$this->add_lapp_function(2, _("N�meros y Hora"),"citas/manage/numeros_x_hora.php?");	      
      $this->add_lapp_function(2, _("Numeros y Hora"),"citas/manage/numeros_x_hora_final.php?");
      //$this->add_lapp_function(2, _("N�meros y Hora GT"),"citas/manage/numeros_x_hora_final_gt.php?");	            	            
      $this->add_lapp_function(2, _("Tipo de cita por medico"),"citas/manage/tipocita_x_medico.php?");
      //$this->add_lapp_function(2, _("Tipo de cita por m�dico GT"),"citas/manage/tipocita_x_medico_gt.php?");
      $this->add_lapp_function(2, _("Dias de trabajo por medico"),"citas/manage/diatrabxmedico.php?");      
      //$this->add_lapp_function(2, _("D�as de trabajo por m�dico GT"),"citas/manage/diatrabxmedico_gt.php?");            
      $this->add_lapp_function(2, _("Relacion medico y patron por dia"),"citas/manage/relmedpatronxdia.php?");
      //$this->add_lapp_function(2, _("Relaci�n m�dico y patr�n por d�a GT"),"citas/manage/relmedpatronxdia_gt.php?");
      $this->add_lapp_function(2, _("Genera Calendario"),"citas/manage/genera_calen.php?");	 
      //$this->add_lapp_function(2, _("Genera Calendario GT"),"citas/manage/genera_calen_gt.php?");	 
			//$this->add_lapp_function(2, _("Add and Manage &Customers"),"sales/manage/customers.php?");
			//Begin Felix Alberti 21/05/2009
			//$this->add_lapp_function(2, _("Medicos"),"sales/manage/medicos.php?");			
			//$this->add_lapp_function(2, _("Responsable de Pago"),"sales/manage/responsable_pago.php?");				
			//End Felix Alberti 21/05/2009
			//$this->add_lapp_function(2, _("Customer &Branches"),"sales/manage/customer_branches.php?");
			//$this->add_lapp_function(2, _("Sales &Groups"),"sales/manage/sales_groups.php?");
			//$this->add_lapp_function(2, _("Recurrent &Invoices"),"sales/manage/recurrent_invoices.php?");
			//$this->add_rapp_function(2, _("Sales T&ypes"),"sales/manage/sales_types.php?");
			//$this->add_rapp_function(2, _("Sales &Persons"),"sales/manage/sales_people.php?");
			//$this->add_rapp_function(2, _("Sales &Areas"),"sales/manage/sales_areas.php?");
			//$this->add_rapp_function(2, _("Credit &Status Setup"),"sales/manage/credit_status.php?");
			if (count($installed_modules) > 0)
			{
				foreach ($installed_modules as $mod)
				{
					if ($mod["tab"] == "citas")
						$this->add_rapp_function(2, $mod["name"], "modules/".$mod["path"]."/".$mod["filename"]."?");
				}
			}	
		}
	}
	

?>