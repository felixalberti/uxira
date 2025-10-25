<?php

/* How to make new entries here for non-packaged languages:

-- 'code' should match the name of the directory for the language under \lang
.-- 'name' is the name that will be displayed in the language selection list (in Users and Display Setup)
-- 'rtl' only needs to be set for right-to-left languages like Arabic and Hebrew
-- 'encoding' used in translation file
-- 'version' always set to '' for manually installed languages.
-- 'path' installation path related to FA root (e.g. 'lang/en_US').
*/


$installed_languages = array (
  0 => 
  array (
    'code' => 'C',
    'name' => 'English',
    'encoding' => 'iso-8859-1',
  ),
  1 => 
  array (
    'name' => 'Spanish (Argentina)',
    'package' => 'es_AR',
    'code' => 'es_AR',
    'encoding' => 'iso-8859-1',
    'version' => '2.3.0-2',
    'path' => 'lang/es_AR',
  ),
  2 => 
  array (
    'name' => 'Spanish (Colombia)',
    'package' => 'es_CO',
    'code' => 'es_CO',
    'encoding' => 'iso-8859-1',
    'version' => '2.3.0-2',
    'path' => 'lang/es_CO',
  ),
  3 => 
  array (
    'code' => 'es_VE',
    'name' => 'Spanish (Venezuela)',
    'path' => 'lang/es_VE',
    'encoding' => 'iso-8859-1',
    'rtl' => false,
    'package' => '',
    'version' => '',
  ),
  4 => 
  array (
    'code' => 'es_ve',
    'name' => 'spanish ven',
    'path' => 'lang/es_VE',
    'encoding' => 'iso-8859-1',
    'rtl' => false,
    'package' => '',
    'version' => '',
  ),
  5 => 
  array (
    'code' => 'es_ve',
    'name' => 'spanish_ven',
    'path' => 'lang/es_VE',
    'encoding' => 'iso-8859-1',
    'rtl' => false,
    'package' => '',
    'version' => '',
  ),
  6 => 
  array (
    'name' => 'French',
    'package' => 'fr_FR',
    'code' => 'fr_FR',
    'encoding' => 'iso-8859-1',
    'version' => '2.3.0-2',
    'path' => 'lang/fr_FR',
  ),
  7 => 
  array (
    'name' => 'Italian',
    'package' => 'it_IT',
    'code' => 'it_IT',
    'encoding' => 'iso-8859-1',
    'version' => '2.3.0-1',
    'path' => 'lang/it_IT',
  ),
  8 => 
  array (
    'name' => 'Portuguese',
    'package' => 'pt_PT',
    'code' => 'pt_PT',
    'encoding' => 'iso-8859-1',
    'version' => '2.3.17-1',
    'path' => 'lang/pt_PT',
  ),
);

$dflt_lang = 'es_VE';
?>
