<?php
$is_admin = isset($_COOKIE['admin']) && hash_equals('bDj9qE@r4aMxUZPKKjh#zcE8!u4m5gN4', $_COOKIE['admin']);
?>
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Admin News</title>
    <link rel="stylesheet" type="text/css" href="mini-dark.min.css">
    <?php
        if (isset($_GET['css'])) {
            echo '<link rel="stylesheet" type="text/css" href="css.php?c=' . urlencode($_GET['css']) . '">';
        }
    ?>
</head>
<body>
<main>
    <h1>Admin News</h1>
    <?php if (! $is_admin) {?> <div><span class="icon-alert"></span> Area is restricted to administrators</div><?php } ?>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <div id="secret-news"><?php if ($is_admin) { echo 'GH19{css_leaks}'; } ?></div>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
    <hr>
</main>
</body>
</html>