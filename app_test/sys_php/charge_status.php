<?php
header("Content-Type: text/html;charset=UTF-8");
 
$db_host = 'localhost';
$db_user = "root";
$db_passwd = "";
$db_name = "parking";
$mysqli = new mysqli($db_host, $db_user, $db_passwd, $db_name);
 
if($mysqli){
    $ParkID = $_GET['ParkID'];
    $ChargeStatus = $_GET['ChargeStatus'];

    $query = "UPDATE parking SET ChargeStatus = $ChargeStatus WHERE ParkID = '$ParkID'";
    mysqli_query($mysqli,$query);
} else{
    echo "MySQL could not be connected";
}
?>
