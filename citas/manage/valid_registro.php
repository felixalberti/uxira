<?php session_start(); ?><?php
// If para saber si el usuario ingreso el codigo de seguridad correctamente
if($_POST['codigo'] != $_SESSION['turing']){
  $_SESSION['turing'] = '';
  //session_destroy();
  header("Location: registrar.php?mensaje=Codigo de seguridad incorrecto");}
else{?>
<?php require_once('connections/conexion2.php');
function GetSQLValueString($theValue, $theType, $theDefinedValue = "", $theNotDefinedValue = "") 
{
  $theValue = (!get_magic_quotes_gpc()) ? addslashes($theValue) : $theValue;

  switch ($theType) {
    case "text":
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;    
    case "long":
    case "int":
      $theValue = ($theValue != "") ? intval($theValue) : "NULL";
      break;
    case "double":
      $theValue = ($theValue != "") ? "'" . doubleval($theValue) . "'" : "NULL";
      break;
    case "date":
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;
    case "defined":
      $theValue = ($theValue != "") ? $theDefinedValue : $theNotDefinedValue;
      break;
  }
  return $theValue;
}
$editFormAction = $_SERVER['PHP_SELF'];
$mensaje = '';
if ((isset($_POST["MM_insert"])) && ($_POST["MM_insert"] == "form1")) {
   $pattern = '/^[^@]+@[^\s\r\n\'";,@%]+$/';
   if (!preg_match($pattern, trim($_POST['email']))) {
      $mensaje = 'Email no valido';
	  $redireccionar = "registrar.php?mensaje=".$mensaje ;
	  header("Location: " .$redireccionar );
	  exit();
   }
   $loginUsername=$_POST['user_id'];
   $LoginRS__query=sprintf("SELECT user_id FROM 0_users WHERE user_id='%s' or ci_usuario='%s' or email='%s'",  get_magic_quotes_gpc() ? $loginUsername : addslashes($loginUsername), $_POST['cedula'], trim($_POST['email'])); 
   mysql_select_db($database_conexion2, $conexion2);   
   $LoginRS = mysql_query($LoginRS__query, $conexion2) or die(mysql_error());
   $rows = mysql_num_rows($LoginRS);
   if ($rows > 0) {
   $mensaje = 'El usuario ya existe';	  $redireccionar = "registrar.php?mensaje=".$mensaje ;
	  header("Location: " .$redireccionar );
	  exit();
   }
   else {
      $mensaje = '';  
      //$clave = "siparano34247";
      //$password=crypt($_POST['password'],$clave);
	   $password = md5($_POST['password']);
      $insertSQL = sprintf("INSERT INTO 0_users (user_id, password, ci_usuario, real_name, last_name, sexo, phone, celular, telefono_ofc, referencia, nomreferido,  email, pregunta1, resp1, fecharegistro) 
	   VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s,  %s, %s, %s, %s, %s, %s)",
                           GetSQLValueString($_POST['user_id'], "text"),
						   GetSQLValueString($password, "text"),
						   GetSQLValueString($_POST['cedula'], "text"),	
						   GetSQLValueString($_POST['real_name'], "text"),	
						   GetSQLValueString($_POST['last_name'], "text"),	
						   GetSQLValueString($_POST['sexo'], "text"),	
						   GetSQLValueString($_POST['telefono_hab'], "text"),	
						   GetSQLValueString($_POST['celular'], "text"),
						   GetSQLValueString($_POST['telefono_ofc'], "text"),						   
						   GetSQLValueString($_POST['referencia'], "text"),
						   GetSQLValueString($_POST['nomreferido'], "text"),
						   GetSQLValueString($_POST['email'], "text"),
						   GetSQLValueString($_POST['pregunta1'], "text"),
						   GetSQLValueString($_POST['resp1'], "text"),
						   GetSQLValueString(date("Y/m/d h:i"), "date"));
   mysql_select_db($database_conexion2, $conexion2);
   $Result1 = mysql_query($insertSQL, $conexion2) or die(mysql_error());
   if ($Result1 == 1) {
      //$redireccionar = "ingreso_usuario.php?cedula=".$_POST['cedula'];
	  $_SESSION['nombreuser'] = $_POST['real_name'].", ".$_POST['last_name'];
  	  $email = $_POST['email'];
  	  $motivo = "Registro en Angios - Centro Vascular y Cuidado Integral de Heridas";
	  $texto = "Hola ".$_POST['real_name'].", "
	  .$_POST['last_name'].".\n Tu Login es: ".$_POST['user_id']
	  ."\nTe has Registrado Satisfactoriamente.\n"
	  ."\nTu claves es: ".$_POST['password']
	  ."\nNota: La clave es generada de forma aleatoria y guardada de forma encriptada"
	  ."\nSolamente usted conoce su clave.";	  
	  mail($email,$motivo, $texto," FROM: info@angios.com");
	  $host = $_SERVER['HTTP_HOST'];
      $uri = rtrim(dirname($_SERVER['PHP_SELF']), '/\\');
      $extra = 'entrada.php?mensaje=Gracias por Registrarse. Use su usuario y password para entrar al portal';
      header("Location: http://$host$uri/$extra");
    }
    }
}
} ?>