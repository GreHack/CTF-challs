<?php

ini_set('display_errors', 1); ini_set('display_startup_errors', 1); error_reporting(E_ALL);

$jsonData = json_decode(file_get_contents('php://input'), true);

$host = $jsonData['dbhost'];
$dbname = $jsonData['dbname'];
$dbuser = $jsonData['dbuser'];
$dbpassword = $jsonData['dbpassword'];

header("Content-type: application/json");

try {
    $pdo = new PDO('mysql:host='.$host.';dbname='.$dbname.'', $dbuser, $dbpassword, array(PDO::ATTR_TIMEOUT => 3, PDO::MYSQL_ATTR_LOCAL_INFILE => true,));
    $pdo->exec('SET NAMES "utf8";');
    $pdo->exec('SET NAMES "utf8";');
    
    $pdo = null;

    echo json_encode(['message' => 'Connection successfull.']);

} catch (PDOException $e) {
    http_response_code(500);
    echo json_encode(['error' => 'Error during database connection with local_infile : ' . $e]);
}
?>
