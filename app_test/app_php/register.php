<?php
header("Content-Type: text/html;charset=UTF-8");
 
$db_host = 'localhost';
$db_user = "root";
$db_passwd = "";
$db_name = "parking";
$mysqli = new mysqli($db_host, $db_user, $db_passwd, $db_name);
 
    if($mysqli){
		$ParkID = $_GET['ParkID'];
		$CarID = $_GET['CarID'];
		$query = "SELECT CarExist FROM `parking` WHERE ParkID = '$ParkID'";
		$find = mysqli_query($mysqli,$query);
		$data = mysqli_fetch_array($find);
		$CarExist = (bool)$data['CarExist'];

		if($CarExist){
			$query = "UPDATE parking SET CarID = $CarID WHERE ParkID = '$ParkID'";
			mysqli_query($mysqli,$query);
		} else {
			echo "Please park first";
		}
    } else{
		echo "MySQL could not be connected";
    }

mysqli_close($mysqli);
?>
