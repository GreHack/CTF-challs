<?php

require __DIR__ . '/../src/bootstrap.php';

$user_manager = new \GreHack\UserManager;
$current_user = $user_manager->getCurrentUser();
if ($current_user->isLoggedIn()) {
    header('Location: /');
    exit();
}

$anticsrf = new \GreHack\AntiCSRF('/login.php');

$has_errors = false;
if (isset($_REQUEST['username']) && isset($_REQUEST['password']) &&
    !empty($_REQUEST['username']) && !empty($_REQUEST['password'])) {
        if (! $anticsrf->isValid()) {
            header('Location: /login.php');
            exit();
        }
        $has_errors = !$user_manager->login($_REQUEST['username'], $_REQUEST['password']);
        if (! $has_errors) {
            header('Location: /');
            exit();
        }
}

$engine_factory = new \GreHack\TemplateEngineFactory;
$engine = $engine_factory->getEngine();
$template = $engine->loadTemplate('login');
echo $template->render(['has_errors' => $has_errors, 'anticsrf' => $anticsrf]);
