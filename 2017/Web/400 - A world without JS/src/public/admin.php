<?php

require __DIR__ . '/../src/bootstrap.php';

$user_manager = new \GreHack\UserManager;
$current_user = $user_manager->getCurrentUser();

if (! $current_user->isLoggedIn()) {
    header('Location: /login.php');
    exit();
}

if (! $current_user->isAdmin()) {
    header('Location: /');
    exit();
}

if (! isset($_REQUEST['id'])) {
    header('Location: /');
    exit();
}

$invitation_manager = new \GreHack\InvitationManager;
try {
    $invitation = $invitation_manager->getInvitation($_REQUEST['id']);
} catch (\GreHack\InvitationNotFoundException $ex) {
    header('Location: /');
    exit();
}

if ($invitation->hasAlreadyBeenProcessed()) {
    header('Location: /');
    exit();
}

$anticsrf = new \GreHack\AntiCSRF('/admin.php');

if (isset($_REQUEST['status']) && ! empty($_REQUEST['status'])) {
    if (! $anticsrf->isValid()) {
        header('Location: /admin.php');
        exit();
    }
    if ($_REQUEST['status'] == 1) {
        $invitation_manager->accept($invitation);
    } else {
        $invitation_manager->reject($invitation);
    }
    header('Location: /');
    exit();
}

$engine_factory = new \GreHack\TemplateEngineFactory;
$engine = $engine_factory->getEngine();
$template = $engine->loadTemplate('admin');
$anticsrf_logout = new \GreHack\AntiCSRF('/logout.php');
echo $template->render(['user' => $current_user, 'invitation' => $invitation, 'anticsrf' => $anticsrf, 'anticsrf_logout' => $anticsrf_logout]);
