<?php
header("Content-Type: text/html;charset=UTF-8");
 
$db_host = 'localhost';
$db_user = "root";
$db_passwd = "";
$db_name = "parking";
$mysqli = new mysqli($db_host, $db_user, $db_passwd, $db_name);
 
    if($mysqli){
		echo "MySQL successfully connected!<br/>";

		$ParkID = $_GET['ParkID'];
    	$Chargestatus = $_GET['Chargestatus'];
	
		echo "<br/>ParkID = $ParkID";
		echo ", ";
		echo "Chargestatus = $Chargestatus</br>";

		$query = "UPDATE parking SET Chargestatus = $Chargestatus WHERE ParkID = '$ParkID'";
		mysqli_query($mysqli,$query);

		echo "</br>success!!";
    } else{
		echo "MySQL could not be connected";
    }

mysqli_close($mysqli);
?>
