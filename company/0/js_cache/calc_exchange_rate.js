/**********************************************************************
    Copyright (C) Septiembre 2020 Uxira, C.A
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
function blur_calc(e) {
    
    var amount = 1 / get_amount(e.name);
                           
    /*console.log(amount);*/
                                                      
    if(document.getElementsByName('BuyRate') && amount != Infinity)
    document.getElementsByName('BuyRate')[0].value = amount;                       
                           
    var val = document.getElementsByName('BuyRate')[0].value;
    /*console.log(val)*/
    if (val.match(/\./)) {
        val = val.replace(/\./g, ',');                         
        document.getElementsByName('BuyRate')[0].value=val;
    }                                 
                         
    
}


var amount_provider = {
	'.amount_provider': function(e) {
		
		e.onblur = function() {
			blur_calc(this);
		};
                e.onkeydown = function(ev) {	// block unintentional page escape with 'history back' key pressed on buttons
			ev = ev||window.event;
 			key = ev.keyCode||ev.which;
	  		if(key == 13) {
                           var amount = 1 / get_amount(e.name);
                           
                           /*console.log(amount);*/
                                                      
                           if(document.getElementsByName('BuyRate') && amount != Infinity)
                           document.getElementsByName('BuyRate')[0].value = amount;                       
                           
                           var val = document.getElementsByName('BuyRate')[0].value;
                           /*console.log(val)*/
                           if (val.match(/\./)) {
                            val = val.replace(/\./g, ',');                         
                            document.getElementsByName('BuyRate')[0].value=val;
                           }                           
                         
		           ev.returnValue = false;
	  		   return false;
  			}
		}
	}
}

Behaviour.register(amount_provider);