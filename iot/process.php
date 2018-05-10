<?php

session_start();
//$var1 = $_GET['user_token']
//$var2 = $_SESSION['user_token']

//if($_SERVER["HTTP_REFERER"] == "http://127.0.0.1/login.php"){
//	header("Refresh:0");
//	header("Refresh:0");
//}
$var1=$_GET['pass'];
if($_GET['user_token'] == $_SESSION['user_token']) {
	if($_GET["pass"]=="ufxx"){
		echo "You got access to IOT control Console.";
		$message = 'Click <a href="gpio.php"> Here </a> to access the Console Panel';
		echo $message;
	}
	else{
		echo "Invalid Password!!";}
    
} else {
    echo "Your Session Expired!!, else invalid Token.....Try Again";
}


// invalidate the token so it expires on view, important!
unset($_SESSION['user_token']);


?>
