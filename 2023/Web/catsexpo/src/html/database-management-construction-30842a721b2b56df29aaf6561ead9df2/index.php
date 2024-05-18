<?php 

// Not sure about this feature but anyway, nobody will access to it : 
// - /var/www/html/database-management-construction-30842a721b2b56df29aaf6561ead9df2/private-admin-panel-4e2c720f8ddb2d0f12e395528263fc96/

?>

<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="Admin application">
    <meta name="keywords" content="Admin application">
    <meta name="author" content="Admin application">

    <title>Admin application</title>

    <!-- Bootstrap core CSS -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css" integrity="sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh" crossorigin="anonymous">
  </head>

  <!-- Navigation -->
  <nav class="navbar navbar-expand-lg navbar-dark bg-dark fixed-top">
    <div class="container">
      <a class="navbar-brand" href="/">Admin application.</a>
      <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
            <span class="navbar-toggler-icon"></span>
          </button>
      <div class="collapse navbar-collapse" id="navbarResponsive">
        <ul class="navbar-nav ml-auto">
          <li class="nav-item active">
            <a class="nav-link" href="/">Home
                  <span class="sr-only">(current)</span>
            </a>
          </li>
        </ul>
      </div>
    </div>
  </nav>
  <body>

    <main role="main" class="container">
      <div class="container">
        <div class="starter-template">
          <hr style="padding-top: 100px;">
          <div class="jumbotron">
              <h1 class="display-6">Connect to the SQL Server</h1>
              <hr class="my-4">
              <form id="jsonForm">
                <label for="host">Host:</label>
                <input type="text" id="dbhost" name="dbhost" required><br>

                <label for="dbname">Database Name:</label>
                <input type="text" id="dbname" name="dbname" required><br>

                <label for="dbuser">Database User:</label>
                <input type="text" id="dbuser" name="dbuser" required><br>

                <label for="dbpassword">Database Password:</label>
                <input type="password" id="dbpassword" name="dbpassword" required><br>

                <input type="submit" value="Submit">
            </form>
            </div>
        </div>
      </div>
    </main><!-- /.container -->

    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://code.jquery.com/jquery-3.4.1.slim.min.js" integrity="sha384-J6qa4849blE2+poT4WnyKhv5vZF5SrPo0iEjwBvKU7imGFAV0wwj1yYfoRSJoZ+n" crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js" integrity="sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo" crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js" integrity="sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6" crossorigin="anonymous"></script>
    <script src="https://code.jquery.com/jquery-3.1.1.min.js"></script>

    <script>
    document.getElementById("jsonForm").addEventListener("submit", function(event) {
        event.preventDefault();

        const formData = new FormData(event.target);
        const jsonData = {};

        formData.forEach((value, key) => {
            jsonData[key] = value;
        });

        const xhr = new XMLHttpRequest();
        xhr.open("POST", "/database-management-construction-30842a721b2b56df29aaf6561ead9df2/database.php", true);
        xhr.setRequestHeader("Content-Type", "application/json");
        xhr.onreadystatechange = function() {
            if (xhr.readyState === 4 && xhr.status === 200) {
                console.log(xhr.responseText);
            }
        };

        xhr.send(JSON.stringify(jsonData));
    });
    </script>
  </body>
</html>
