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
function focus_alloc(i) {
    save_focus(i);
	i.setAttribute('_last', get_amount(i.name));
}

function blur_alloc(i) {
		var change = get_amount(i.name);
                
                 /*Felix Alberti 14/10/2016 i.name != 'method_amount' && i.name != 'method_discount' */
		if (i.name != 'amount' && i.name != 'charge' && i.name != 'discount' && i.name != 'method_amount')
			change = Math.min(change, get_amount('maxval'+i.name.substr(6), 1))

                /*Begin Felix Alberti 14/10/2016*/
                if (i.name == 'method_amount'){
                    var value = get_amount('amount')+change;
		    price_format('amount', value, user.pdec, 0);
                }
                /*End Felix Alberti 14/10/2016*/

                
                /*Begin Felix Alberti 14/10/2016*/
                if (i.name == 'amount'){
                    //var value_amt = change - get_amount('method_discount');
                    var value_amt = change;
		    price_format('method_amount', value_amt, user.pdec, 0);
                    price_format('amount', value_amt, user.pdec, 0);
                }
                else
                /*End Felix Alberti 14/10/2016*/                
		price_format(i.name, change, user.pdec);
                
                /*Felix Alberti 14/10/2016 i.name != 'method_amount' */
		if (i.name != 'amount' && i.name != 'charge' && i.name != 'method_amount') {
                /*End Felix Alberti 14/10/2016*/
			if (change<0) change = 0;
			change = change-i.getAttribute('_last');
			if (i.name == 'discount') change = -change;
                        
                        /*Begin Felix Alberti 14/10/2016*/
                        //if (i.name == 'method_discount') change = -change;
                        /*End Felix Alberti 14/10/2016*/

			var total = get_amount('amount')+change;
			price_format('amount', total, user.pdec, 0);
                        
                        /*Begin Felix Alberti 14/10/2016*/
                        total = get_amount('method_amount')+change;
			price_format('method_amount', total, user.pdec, 0);
                        /*End Felix Alberti 14/10/2016*/
		}
                
               
}

function allocate_all(doc) {
	var amount = get_amount('amount'+doc);
	var unallocated = get_amount('un_allocated'+doc);
	var total = get_amount('amount');
	var left = 0;
	total -=  (amount-unallocated);
	left -= (amount-unallocated);
	amount = unallocated;
	if(left<0) {
		total  += left;
		amount += left;
		left = 0;
	}
	price_format('amount'+doc, amount, user.pdec);
	price_format('amount', total, user.pdec);
}

function allocate_none(doc) {
	amount = get_amount('amount'+doc);
	total = get_amount('amount');
	price_format('amount'+doc, 0, user.pdec);
	price_format('amount', total-amount, user.pdec);
}

var allocations = {
	'.amount': function(e) {
 		if(e.name == 'allocated_amount' || e.name == 'bank_amount')
 		{
  		  e.onblur = function() {
			var dec = this.getAttribute("dec");
			price_format(this.name, get_amount(this.name), dec);
		  };
 		} else {
			e.onblur = function() {
				blur_alloc(this);
			};
			e.onfocus = function() {
				focus_alloc(this);
			};
		}
	}
}

Behaviour.register(allocations);
