<?php

require __DIR__ . '/../src/bootstrap.php';

$user_manager = new \GreHack\UserManager;
$current_user = $user_manager->getCurrentUser();

if (! $current_user->isLoggedIn()) {
    header('Location: /login.php');
    exit();
}

$invitation_manager = new \GreHack\InvitationManager;
if (isset($_REQUEST['comment']) && !empty($_REQUEST['comment'])) {
    $invitation_manager->request($current_user, $_REQUEST['comment']);
    header('Location: /invit.php');
    exit();
}

$invitations = $invitation_manager->getInvitations($current_user);

$engine_factory = new \GreHack\TemplateEngineFactory;
$engine = $engine_factory->getEngine();
$template = $engine->loadTemplate('invit');
$anticsrf_logout = new \GreHack\AntiCSRF('/logout.php');
echo $template->render(['user' => $current_user, 'invitations' => $invitations, 'anticsrf_logout' => $anticsrf_logout]);
