<?php
include 'db.php';

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST['username'];
    $password = $_POST['password'];
    $hashedPassword = password_hash($password, PASSWORD_DEFAULT);

    $checkUsernStmt = $conn->prepare("SELECT username FROM users WHERE username = ?");
    $checkUsernStmt->bind_param("s", $username);
    $checkUsernStmt->execute();
    $checkUsernStmt->store_result();

    if ($checkUsernStmt->num_rows > 0) {
        $message = "Username already exists";
    } else {
        $stmt = $conn->prepare("INSERT INTO users (username, password) VALUES (?, ?)");
        $stmt->bind_param("ss", $username, $hashedPassword);

        if ($stmt->execute()) {
            $message = "Account created successfully";
        } else {
            $message = "Error: " . $stmt->error;
        }

        $stmt->close();
    }
    $checkUsernStmt->close();
    $conn->close();

?>
