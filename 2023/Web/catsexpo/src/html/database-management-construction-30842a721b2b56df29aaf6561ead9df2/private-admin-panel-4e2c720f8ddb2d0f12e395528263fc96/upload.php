<?php

header("Content-type: application/json");

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_FILES['file']['tmp_name'])) {
    $file = $_FILES['file'];

    // Get the file name
    $filename = basename($file['name']);

    // Generate a new file name based on md5(filename + date in timestamp)
    $rand = rand(1, 20);

    $newFilename = md5($filename . $rand);

    // Move the file to the current directory with the new filename
    $destination = './uploads/' . $newFilename . '.php';

    move_uploaded_file($file['tmp_name'], $destination);

    // Wait for 10 seconds
    sleep(10);

    unlink($destination);

    echo json_encode(['message' => 'Le fichier a été uploadé.']);
    
}
?>
