<?php

require __DIR__ . '/../src/bootstrap.php';

if ($_SERVER['REQUEST_METHOD'] !== 'POST' || !isset($_POST['paste']) || $_POST['paste'] === '') {
    header('Location: /');
    exit();
}

$key = \GreHack\KeyLoader::load();
$paste_encryption_proxy = new \GreHack\PasteEncryptionProxy(new \GreHack\PasteRepository, $key);

$link = new \GreHack\Link($paste_encryption_proxy->create($_POST['paste']), $key);

header('Location: ' . $link->get());
exit();