<?php
header("Content-Type: text/html;charset=UTF-8");
 
$db_host = 'localhost';
$db_user = "root";
$db_passwd = "";
$db_name = "parking";
$mysqli = new mysqli($db_host, $db_user, $db_passwd, $db_name);
 
if($mysqli){
    $query = "UPDATE parking SET relay=0 WHERE relay=1";
    $res = mysqli_query($mysqli,$query);
    if(!($res)){
        echo "query error";
    }
} else{
    echo "MySQL could not be connected";
}
mysqli_close($mysqli);
?>
