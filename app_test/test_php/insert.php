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
    	$CarExist = $_GET['CarExist'];
	
		echo "<br/>ParkID = $ParkID";
		echo ", ";
		echo "CarExist = $CarExist";
		$query = "UPDATE parking SET CarExist = $CarExist WHERE ParkID = '$ParkID'";
		mysqli_query($mysqli,$query);

		echo "</br>success!!";
    } else{
		echo "MySQL could not be connected";
    }

mysqli_close($mysqli);
?>
