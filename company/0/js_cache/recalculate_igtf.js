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

function blur_alloc_ult(i) {
    var last = +i.getAttribute('_last')
    console.log('saliendo');
    console.log(last);
    var current = (get_amount(i.name) * 3) / 100;
    console.log(current);
}

function calcular(){
    return true;
}

var efectivo_igtf = {
	'.amount': function(e) {
		 e.onkeydown = function(ev) {	// block unintentional page escape with 'history back' key pressed on buttons
			ev = ev||window.event;
 			key = ev.keyCode||ev.which;
	  		if(key == 13) {				
				var rate = document.getElementsByName('current_rate')[0].value
                console.log(rate);				
                var igtf = (get_amount(this.name) / (1 / rate)) ;
				console.log(igtf);			
			    price_format('amount_div', igtf, user.pdec);				
				ev.returnValue = false;
	  		    return false;				
            }
		}	
	}
}				

var allocations = {
	'.calc': function(e) {
		e.onblur = function() { //alert('onblur');
			//blur_alloc(this);                        
                        blur_alloc_ult(this);
		  };
		e.onfocus = function() { //alert('focus');
			focus_alloc(this);
		};
                e.onkeydown = function(ev) {	// block unintentional page escape with 'history back' key pressed on buttons
			ev = ev||window.event;
 			key = ev.keyCode||ev.which;
	  		if(key == 13) {                                       
                           //var thenum = this.name.match(/\d+$/)[0];
                           //alert('Fino');
                           //var next = document.getElementsByName('amount_igtf').value;
                           //setFocus(next);
                           
                           //var val = typeof(doc) == "string" ? 
			   //var value = document.getElementsByName('method_amount')[0].value;
                          
                          
                          /*var elementos = document.getElementsByName("type");
                          
                          for (x=0;x<elementos.length;x++){
                              console.log( x + "\n");  
                              var e = elementos[x];
                              
                              console.log( e.value + "\n");  
                              var strDescri = e.options[e.selectedIndex].text;
                              console.log( strDescri + "\n");
                          }*/
                          
                           var e = document.getElementsByName("type")[0];
                           var strDescri = e.options[e.selectedIndex].text;
                           console.log(strDescri);
                           if (strDescri.substr(0,4) == 'IGTF') {
                           
                            //var rate = get_amount('current_rate');
                            var rate = document.getElementsByName('current_rate')[0].value
                            console.log(rate);
                            var igtf = ((get_amount(this.name) * 3) / 100) / rate;
                            
                            price_format('number', get_amount(this.name), user.pdec);
							
							var igtf_div = igtf / (1 / rate);
							price_format('amount_div', igtf_div, user.pdec);

                            price_format('method_amount', igtf, user.pdec);

                            console.log(igtf);
                            
                            price_format(this.name, 0, user.pdec);
                           
                           }
						   else if (strDescri == 'Caja PB - Efectivo ($)') {
							   //alert('Falta programar');
						   }
                           else {
                              console.log('No es IGTF'); 
                           }
                           
		           ev.returnValue = false;
                           
                          
	  		   return false;
                           //ev.preventDefault();
                           
                           //alert(ev.value);
  			}
		}
	}
}

Behaviour.register(allocations);

Behaviour.register(efectivo_igtf);



