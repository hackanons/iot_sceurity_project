<html>
<body>
<script type="text/javascript">
	  
		function runEncrypt(){
			var key = 5
			var pass = document.getElementById("pass").value;
			
			if(pass.length==0){
				alert("Password cannot be empty!");
				return;
		}
			
		
		//if (isDecrypt)
		//	key = (26 - key) % 26;
			var pass = document.getElementById("pass");
			pass.value = caesarShift(pass.value, key);
		
	}
		
		
		function caesarShift(text, shift) {
			var ciphertext = "";
			
			for (var i = 0; i < text.length; i++){
				var ch = text.charCodeAt(i);
				
				if(65 <= ch && ch <=  90) {
					ciphertext += String.fromCharCode((ch - 65 + shift) % 26 + 65); // for Uppercase Unicode
				} 
				else if(97 <= ch && ch <= 122){
					ciphertext += String.fromCharCode((ch - 97 + shift) % 26 + 97);  // for Lowercase Unicode
				}
				else{
					ciphertext += text.charAt(i);
				}
		}
		return ciphertext;
	}  
	  
</script>

<?php
        // start session
        session_start();

        // create unique token
        $form_token = uniqid();
	//$hash=md5($form_token);
 
        // commit token to session
        $_SESSION['user_token'] = $form_token;
	//$fp = fopen('lidn.txt', 'w+');
	//fwrite($fp, $hash);
	//fclose($fp);
?>
<center><form action="process.php" method="GET">
<input type="text" name="username" value="username" /><br>
<input type="password" name="pass" id="pass" value="pass" required/><br>
<input type="hidden" name="user_token" value="<?php echo $_SESSION['user_token'];  ?>" /><br>
<input type="submit" value="Submit" onclick="runEncrypt()"/>
</form></center>

</body>
</html>
