<?php
require_once __DIR__ . '/vendor/autoload.php';
require_once __DIR__ . '/generated-conf/config.php';

$is_authenticated = false;
if (isset($_POST['username'], $_POST['password'])) {
    $q = \GrehackORMLogin\UserQuery::create();
    $user = $q->findOneByUsername($_POST['username']);

    $is_authenticated = $user !== null  && hash_equals($user->getPassword(), $_POST['password']);
}

?>
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Login</title>
    <link rel="stylesheet" type="text/css" href="mini-dark.min.css">
</head>
<body>
<main>
    <h1>Restricted Area - Login</h1>
    <?php if (! $is_authenticated) { ?>
    <form method="post">
        <!-- Finish implementation of the live username existence verification -->
        <label for="username">Username</label>
        <input type="text" id="username" name="username" placeholder="Username" required/>
        <label for="password">Password</label>
        <input type="password" id="password" name="password" placeholder="Password" required/>
        <input type="submit" class="primary">
    </form>
    <?php } else { ?>
    <hr>
    <p>Authentication successful</p>
    <?php
    }
    ?>
</main>
</body>
</html>