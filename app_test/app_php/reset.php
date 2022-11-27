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
    $query = "UPDATE parking SET CarID=0 WHERE ParkID = '$ParkID'";
    mysqli_query($mysqli,$query);
}
mysqli_close($mysqli);
?>
