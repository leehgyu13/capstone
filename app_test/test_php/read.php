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
$row = mysqli_fetch_array($res);



if(mysqli_query($dbc, $query)){


} 
else{
echo "ERROR: Could not able to execute". $query." ". mysqli_error($dbc);
}
?>

<h2>Student Details</h2>

<table border="2">
  <tr>
    <td>ParkID</td>
    <td>CarID</td>
    <td>CarExist</td>
    <td>Chargestatus</td>
  </tr>

<?php

while($data = mysqli_fetch_array($res))
{
?>
  <tr>
    <td><?php echo $data['ParkID']; ?></td>
    <td><?php echo $data['CarID']; ?></td>
    <td><?php echo $data['CarExist']; ?></td>
    <td><?php echo $data['Chargestatus']; ?></td>
  </tr>	
<?php
}
?>
</table>



<php
mysqli_close($dbc);
?>