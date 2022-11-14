<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8"/>
    </head>   
    <body>
        <form action="insert.php" method="POST">
            <p>ParkID : <input type="text" id="ParkID" name="ParkID" required></p>
            <p>CarID : <input type="text" id="CarID" name="CarID" required></p>
            <p>CarExist : <input type="text" id="CarExist" name="CarExist"></p>  //센서와 연동
            <p>Chargestatus : <input type="text" id="Chargestatus" name="Chargestatus"></p>
            <p><input type="submit" /></p>            
        </form>
    </body>
</html>
