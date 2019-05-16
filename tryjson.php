<html>
<head>
<title>Try Session JSON</title>
</head>
<body>
<?php
	include ('connection.php');
	$sql="SELECT uid FROM cards";
	$records=mysqli_query($con,$sql);
	$json_array=array();
	
	while($row=mysqli_fetch_assoc($records))
	{
		$json_array[]=$row;
		
	}
		/*echo '<pre>';
		print_r($json_array);
		echo '</pre>';*/
	echo json_encode($json_array);
?>
</body>
</html>
