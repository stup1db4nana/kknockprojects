<?php
$servername = "localhost";
$username = "dbadmin";
$password = "P@$$Word1234567";
$dbname = "board";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error)
{
	die("connection failure" . $conn->connect_error);
}
echo "connected";
?>
