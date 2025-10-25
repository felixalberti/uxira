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
// Prevent register_globals vulnerability
if (isset($_GET['path_to_root']) || isset($_POST['path_to_root']))
	die("Restricted access");
@include_once($path_to_root . "/lang/installed_languages.inc");
include_once($path_to_root . "/includes/lang/gettext.php");

class language 
{
	var $name;
	var $code;			// eg. ar_EG, en_GB
	var $encoding;		// eg. UTF-8, CP1256, ISO8859-1
	var	$dir;			// Currently support for Left-to-Right (ltr) and
						// Right-To-Left (rtl)
	var $version; // lang package version
	var $is_locale_file;
	
	function language($name, $code, $encoding, $dir = 'ltr') 
	{
		global $dflt_lang;
		
		$this->name = $name;
		$this->code = $code ? $code : ($dflt_lang ? $dflt_lang : 'C');
		$this->encoding = $encoding;
		$this->dir = $dir;
	}

	function get_language_dir() 
	{
		return "lang/" . $this->code;
	}

	function get_current_language_dir() 
	{
		$lang = $_SESSION['language'];
		return "lang/" . $lang->code;
	}

	function set_language($code) 
	{
	    global $path_to_root, $installed_languages, $GetText;

		$lang = array_search_value($code, $installed_languages, 'code');
		$changed = $this->code != $code || $this->version != @$lang['version'];

		if ($lang && $changed)
		{
		// flush cache as we can use several languages in one account
			flush_dir(company_path().'/js_cache');

			$this->name = $lang['name'];
			$this->code = $lang['code'];
			$this->encoding = $lang['encoding'];
			$this->version = @$lang['version'];
			$this->dir = (isset($lang['rtl']) && $lang['rtl'] === true) ? 'rtl' : 'ltr';
			$locale = $path_to_root . "/lang/" . $this->code . "/locale.inc";
			$this->is_locale_file = file_exists($locale);
		}

		$GetText->set_language($this->code, $this->encoding);
		$GetText->add_domain($this->code, $path_to_root . "/lang", $this->version);

		// Necessary for ajax calls. Due to bug in php 4.3.10 for this 
		// version set globally in php.ini
		ini_set('default_charset', $this->encoding);
//display_notification('call App->init() '.$this->encoding);
		if (isset($_SESSION['App']) && $changed)
			$_SESSION['App']->init(); // refresh menu
	}
}

if (!function_exists("_")) 
{   
	function _($text) 
	{   
		global $GetText;
		//echo $text;
		if (!isset($GetText)) // Don't allow using gettext if not is net.
			return $text;

		$retVal = $GetText->gettext($text);
		if ($retVal == "")
			return $text;
		return $retVal;
	}
}

//
function T_gettext($contenido,$path='lang',$val=0) {
//ECHO '<br>path; '.$path.'<br>';
	
//$path = 'lang';
global $language;
$language = "es";
 
 $translation_file = "";
    
    //if ($language == "en") { $translation_file = $path."\\es_VE\LC_MESSAGES\\en_en.po"; }
    if ($language == "es") { $translation_file = $path."\\es_VE\LC_MESSAGES\\es_VE.po"; }
	if ($val==1) ECHO '<br>path; '.$translation_file.'<br>';
	if ($val==1) display_notification($translation_file);
	$num_lineas = 0;
//ECHO '<br>'.$translation_file.'<br>';
 if (file_exists("$translation_file")) {
	 //ECHO 'existe: ';
        $IDIOMA_CONTENT = file("$translation_file");
        $num_lineas = count($IDIOMA_CONTENT);
    } else {
        return $contenido;
    }
	//ECHO 'LINEAS: '.$num_lineas;
	for ($i = 0; $i <= $num_lineas; $i++) {
        $linea1 = $IDIOMA_CONTENT[$i];
        $linea1 = rtrim($linea1);
        $string6 = substr($linea1, 0, 6);

        if ($string6 == "msgid ") {
			
            $orig = str_replace($string6, "", $linea1);
            $orig = str_replace("\"", "", $orig);
            //echo '<br>-->'.$orig;
            if ($orig == $contenido) {
                $linea2 = $IDIOMA_CONTENT[$i + 1];
                $linea2 = rtrim($linea2);
                $string7 = substr($linea2, 0, 7);
//echo '<br>'.$string7;
                if ($string7 == "msgstr ") {
					//echo 'si';
                    $trad = str_replace($string7, "", $linea2);
                    $trad = str_replace("\"", "", $trad);
                    return("$trad");
                }
            } else {
                $i = $i + 2;
            }
        }
    }

    return("$contenido");
}
?>