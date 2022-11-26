<?php

$dbc = mysqli_connect("localhost", "root",  "", "parking");
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

if(mysqli_query($dbc, $query)){
} 
else{
echo "ERROR: Could not able to execute". $query." ". mysqli_error($dbc);
}
?>

<?php

while($data = mysqli_fetch_array($res))
{
    echo $data['ParkID'] .",";
    echo $data['CarID'] .",";
    echo $data['CarExist'] .",";
	echo $data['ChargeStatus'];
	echo "/";
}

mysqli_close($dbc);
?>
