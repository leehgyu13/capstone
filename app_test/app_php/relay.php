<?php
header("Content-Type: text/html;charset=UTF-8");

$db_host = 'localhost';
$db_user = "root";
$db_passwd = "";
$db_name = "parking";
$mysqli = new mysqli($db_host, $db_user, $db_passwd, $db_name);

if($mysqli){
    $ParkID = $_GET['ParkID'];
    $relay = $_GET['relay'];

    $query = "SELECT relay FROM `parking` WHERE ParkID = '$ParkID'";
    $find = mysqli_query($mysqli,$query);
    $data = mysqli_fetch_array($find);
    $db_relay = (bool)$data['relay'];

    if(!($db_relay)){
        $query = "UPDATE parking SET relay = $relay WHERE ParkID = '$ParkID'";
        mysqli_query($mysqli,$query);
    }else{
    }
}else{
    echo "MySQL could not be connected";
}
mysqli_close($mysqli);
?>
