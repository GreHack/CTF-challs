<?php

require __DIR__ . '/../src/bootstrap.php';

$anticsrf = new \GreHack\AntiCSRF('/logout.php');

if ($anticsrf->isValid()) {
    $user_manager = new \GreHack\UserManager;
    $user_manager->logout();
}

header('Location: /');
exit;
