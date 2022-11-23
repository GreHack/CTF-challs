<?php
function getPageName(string $page): string {
  if (str_contains($page, '..')) {
    return './errors/400.php';
  }

  if (preg_match('/^[\w.\-\/]+$/', $page) === false) {
    return './errors/400.php';
  }

  $page = './' . $page;

  if (is_file($page)) {
    return $page;
  }

  $page = $page . '.php';
  if (is_file($page)) {
    return $page;
  }

  return './errors/404.php';
}

$page = './gallery.php';

if (isset($_GET['page'])) {
  $page = getPageName($_GET['page']);
}
?>

<!DOCTYPE html>
<html lang="en" data-theme="coffee" class="h-full">
<head>
  <title>Icons Gallery</title>

  <link href="./style.css" rel="stylesheet" type="text/css" />

</head>
<body class="flex flex-col h-full">
  <nav class="navbar bg-base-100">
    <div class="flex-1">
      <a class="btn btn-ghost normal-case text-xl" href="/">Icons Gallery</a>
    </div>
    <div class="flex-none">
      <ul class="menu menu-horizontal p-0">
        <li><a href="?page=gallery">Gallery</a></li>
        <li><a href="?page=upload">Upload</a></li>
      </ul>
    </div>
  </nav>

  <main class="flex-1">
    <?php
    require_once($page);
    ?>
  </main>

  <script>
    if (window.history.replaceState) {
      window.history.replaceState(null, null, window.location.href);
    }
  </script>
</body>
</html>
