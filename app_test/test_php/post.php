<?php
$dbc = mysqli_connect("localhost", "root", "", "parking");
if(!$dbc) {
    die("DATABASE CONNECTION FAILED:" .mysqli_error($dbc));
    exit();
}
$db = "parking";
$dbs = mysqli_select_db($dbc, $db);
if(!$dbs) {
    die("DATABASE SELECTION FAILED:" .mysqli_error($dbc));
    exit();
}

$query = "SELECT * FROM `parking`";
$res = mysqli_query($dbc,$query);

if(mysqli_query($dbc,$query)){
} else {
    echo "ERROR: Could not able to execute". $query." ". mysqli_error($dbc);
}
?>

<?php
$search = "SELECT ParkID FROM `parking` WHERE relay=1";
$find = mysqli_query($dbc, $search);
$data = mysqli_fetch_array($find);
if($data){
    echo $data['ParkID'];
} else {
    echo "0";
}
mysqli_close($dbc);
?>
