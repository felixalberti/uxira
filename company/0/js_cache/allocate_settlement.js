/**********************************************************************
    Copyright (C) Agosto 2017 Uxira, C.A
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

	var last = +i.getAttribute('_last')
	var left = get_amount('left_to_allocate', 1);
        var amount = get_amount(i.name);
        //console.log(amount);
        //if (amount < 0)
        //amount = amount * -1;
       // console.log('maxval: '+get_amount('maxval'+i.name.substr(6), 1));
	//var cur = Math.min(amount, get_amount('maxval'+i.name.substr(6), 1), last+left)
        var cur = Math.min(amount, get_amount('maxval'+i.name.substr(6), 1), last+left)
        //var cur = last;
        //console.log('left: '+left);
        //console.log('last: '+last);
        //console.log('cur: '+cur);

        if(cur < 0) cur = cur * -1;
        
	price_format(i.name, cur, user.pdec);
        console.log('cur: '+cur);
	change = cur-last;

	var total = get_amount('total_allocated', 1)+change;
		left -= change;
	if (left < 0) left = left * -1; 
	price_format('left_to_allocate', left, user.pdec, 1, 1);
	price_format('total_allocated', total, user.pdec, 1, 1);
        console.log('total_allocated: '+total);
        //console.log(i.name);
        //if(document.getElementsByName('amount'+i.toString())[0] && amount < 0)
        //price_format(i.name, amount, user.pdec, 1, 1);
        
        //if (amount < 0){
        var thenum = i.name.match(/\d+$/)[0];
        var descuento = get_amount('descuento_amount'+thenum);
        var tax = get_amount('tax_amount'+thenum);
        amount = get_amount(i.name);
        price_format('tot_amount'+thenum, (amount+(descuento+tax)), user.pdec);
        //console.log('amount: '+amount);
        var tot_num_all  = get_amount('TotalNumberOfAllocs');
        
        var tot_amount = 0;    
        for (var i = 0; i < tot_num_all; i++) {
           if(document.getElementsByName('tot_amount'+i.toString())[0])
           tot_amount += get_amount('tot_amount'+i.toString());
        }        
        price_format('total_final', tot_amount, user.pdec, 1, 1);
        //}
}

function enter_alloc(event){
    //console.log('xyz: ');
     //alert('enter ');
    /*if (i.keyCode == 13) {
        alert('enter');
    }*/
    event = event||window.event;
    key = event.keyCode||event.which;
    if(key == '13'){
        console.log('You pressed a "enter" key in textbox'); 
    }
}

function focus_tot(i) {
        save_focus(i);
        var atbegin = get_amount(i.name)
	i.setAttribute('_last_tot', atbegin);
}

function blur_tot(i) {
        var last = i.getAttribute('_last_tot')
	var current = get_amount(i.name);
        if (last != 0 && current != last)
        price_format(i.name, last, user.pdec);
}

function focus_tax(i) {
        save_focus(i);
        var atbegin = get_amount(i.name)
	i.setAttribute('_last_tax', atbegin);
}

function blur_tax(i) {
        var thenum = i.name.match(/\d+$/)[0];
        var amount = get_amount('amount'+thenum);
        var tax = get_amount(i.name);
        if (tax < 0) tax = tax * -1; 
        var descuento = get_amount('descuento_amount'+thenum);
        
        //console.log('tax'+thenum+': '+tax);
        
        var min_amount = get_amount('minimum_amount'+thenum)
        var last = i.getAttribute('_last_tax')
        //if (last != 0 && tax != last && amount > 0 && amount >= min_amount){
        if (amount > 0){
           price_format('tot_amount'+thenum, (amount+(descuento+tax)), user.pdec);
           price_format(i.name, tax, user.pdec);
           var total = get_amount('total_allocated', 1);
           var tot_num_all  = get_amount('TotalNumberOfAllocs');
        
           var tot_amount = 0;    
           for (var i = 0; i < tot_num_all; i++) {
                if(document.getElementsByName('tot_amount'+i.toString())[0])
                tot_amount += get_amount('tot_amount'+i.toString());
           }
           //
           price_format('total_final', tot_amount, user.pdec, 1, 1);
        }
}

function focus_descuento(i) {
        save_focus(i);
        var atbegin = get_amount(i.name)
	i.setAttribute('_last_descuento', atbegin);
}

function blur_descuento(i) {
        var thenum = i.name.match(/\d+$/)[0];
        var amount = get_amount('amount'+thenum);
        var descuento = get_amount(i.name);        
        if (descuento < 0) descuento = descuento * -1; 
           
        var tax = get_amount('tax_amount'+thenum);
        //var min_amount = get_amount('minimum_amount'+thenum)
        var last = i.getAttribute('_last_descuento')
        
        if ( amount > 0 ){
           price_format('tot_amount'+thenum, (amount+(descuento+tax)), user.pdec);
           price_format(i.name, descuento, user.pdec);
           var total = get_amount('total_allocated', 1);
           var tot_num_all  = get_amount('TotalNumberOfAllocs');
        
           var tot_amount = 0;    
           for (var i = 0; i < tot_num_all; i++) {
                if(document.getElementsByName('tot_amount'+i.toString())[0])
                tot_amount += get_amount('tot_amount'+i.toString());
           }
           //
           price_format('total_final', tot_amount, user.pdec, 1, 1);
        }
}


function allocate_all(doc) {
	var amount = get_amount('amount'+doc);
        var bank_amount = get_amount('bank_amount');
      
    
	var unallocated = get_amount('un_allocated'+doc);
	var total = get_amount('total_allocated', 1);
	//var left = get_amount('left_to_allocate', 1);
        var left = 0;
        var tax = get_valor('tax_base_amount');
        var descuento = get_amount('descuento_amount'+doc);
        
        var min_amount = get_amount('minimum_amount'+doc)
        
	total -=  (amount-unallocated);
	//left += (amount-unallocated);        
        left = (bank_amount-total);
        
        /*alert(amount + ' - '+ unallocated + ' - ' + amount);  
        if (amount == 0 && bank_amount < unallocated)
        amount = bank_amount;
        else*/
	amount = unallocated;
       
	if(left<0) {
		//total  += left;
		//amount += left;
		//left = 0;
	}
	price_format('amount'+doc, amount, user.pdec);
        
        if (amount > 0 && amount >= min_amount){
            price_format('tax_amount'+doc, ((amount+descuento)*tax)/100, user.pdec);
            price_format('tot_amount'+doc, (amount+descuento)+((amount+descuento)*tax)/100, user.pdec);
          
        }
	price_format('left_to_allocate', left, user.pdec, 1,1);
	price_format('total_allocated', total, user.pdec, 1, 1);
        
        //console.log('amount '+amount);
        //console.log('total_allocated '+total);
        //console.log('left '+left);
          
        var tot_num_all  = get_amount('TotalNumberOfAllocs');
        
        var tot_amount = 0;    
        for (var i = 0; i < tot_num_all; i++) {
            if(document.getElementsByName('tot_amount'+i.toString())[0])
            tot_amount += get_amount('tot_amount'+i.toString());           
        }
        //
        price_format('total_final', tot_amount, user.pdec, 1, 1);        
        
}

function allocate_none(doc) {
	amount = get_amount('amount'+doc);
	left = get_amount('left_to_allocate', 1);
	total = get_amount('total_allocated', 1);
        bank_amount = get_amount('bank_amount');
	//price_format('left_to_allocate',amount+left, user.pdec, 1, 1);
        price_format('left_to_allocate',bank_amount-(total-amount), user.pdec, 1, 1);
	price_format('amount'+doc, 0, user.pdec);
	price_format('total_allocated', total-amount, user.pdec, 1, 1);
        //
        price_format('tot_amount'+doc, 0, user.pdec);
        price_format('tax_amount'+doc, 0, user.pdec);
        
        var tot_num_all  = get_amount('TotalNumberOfAllocs');
        
        var tot_amount = 0;    
        for (var i = 0; i < tot_num_all; i++) {
            if(document.getElementsByName('tot_amount'+i.toString())[0])
            tot_amount += get_amount('tot_amount'+i.toString());
        }
        //
        price_format('total_final', tot_amount, user.pdec, 1, 1);
}

var allocations = {
	'.amount': function(e) {
		e.onblur = function() {
			blur_alloc(this);
		  };
		e.onfocus = function() {
			focus_alloc(this);
		};
                e.onkeydown = function(ev) {	// block unintentional page escape with 'history back' key pressed on buttons
			ev = ev||window.event;
 			key = ev.keyCode||ev.which;
	  		if(key == 13) {                                       
                           var thenum = this.name.match(/\d+$/)[0];
                           var next = document.getElementsByName('descuento_amount'+thenum)[0];
                           setFocus(next);
		           ev.returnValue = false;
	  		   return false;
  			}
		}
	}
}

Behaviour.register(allocations);


var tax_amount = {
	'.tax_amount': function(e) {
		e.onblur = function() {
			blur_tax(this);
		  };
		e.onfocus = function() {
			focus_tax(this);
		};
                e.onkeydown = function(ev) {	// block unintentional page escape with 'history back' key pressed on buttons
			ev = ev||window.event;
 			key = ev.keyCode||ev.which;
	  		if(key == 13) {                                       
                           var thenum = this.name.match(/\d+$/)[0];
                           var next = document.getElementsByName('tot_amount'+thenum)[0];
                           setFocus(next);
		           ev.returnValue = false;
	  		   return false;
  			}
		}
	}
}

Behaviour.register(tax_amount);


var descuento_amount = {
	'.descuento_amount': function(e) {
		e.onblur = function() {
			blur_descuento(this);
		  };
		e.onfocus = function() {
			focus_descuento(this);
		};
                e.onkeydown = function(ev) {	// block unintentional page escape with 'history back' key pressed on buttons
			ev = ev||window.event;
 			key = ev.keyCode||ev.which;
	  		if(key == 13) {                                       
                           var thenum = this.name.match(/\d+$/)[0];
                           var next = document.getElementsByName('tax_amount'+thenum)[0];
                           setFocus(next);
		           ev.returnValue = false;
	  		   return false;
  			}
		}
	}
}

Behaviour.register(descuento_amount);


var total = {
	'.tot_amount': function(e) {
		e.onblur = function() {
			blur_tot(this);
		  };
		e.onfocus = function() {
			focus_tot(this);
		};
	}
}

Behaviour.register(total);
