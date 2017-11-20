<?php

require __DIR__ . '/../src/bootstrap.php';

$user_manager = new \GreHack\UserManager;
$current_user = $user_manager->getCurrentUser();

$engine_factory = new \GreHack\TemplateEngineFactory;
$engine = $engine_factory->getEngine();
$template = $engine->loadTemplate('home');
$anticsrf_logout = new \GreHack\AntiCSRF('/logout.php');
echo $template->render(['user' => $current_user, 'anticsrf_logout' => $anticsrf_logout]);
