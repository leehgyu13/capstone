<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8"/>
    </head>   
    <body>
		<form action="insert.php" method="POST">
		<p class="alert">* : 필수 입력 사항</p>
		<p>ParkID : <input type="text" id="ParkID" name="ParkID" required pattern="[0-3]+"><span class="alert"> * (0~3)</span></p>
		<p>CarID : <input type="text" id="CarID" name="CarID" required><span class="alert"> * </span></p>
            <p>CarExist : <input type="text" id="CarExist" name="CarExist"></p>  <!-- 센서와 연동-->
            <p>Chargestatus : <input type="text" id="Chargestatus" name="Chargestatus"></p>
            <p><input type="submit" /></p>            
        </form>
    </body>
</html>
