<?php
function getDiagCmdOutput(string $tool, string ...$params): ?string
{
    $process = proc_open(
        [$tool, ...$params],
        [
            ['pipe', 'r'],
            ['pipe', 'w'],
            ['pipe', 'w'],
        ],
        $pipes
    );

    if (! is_resource($process)) {
        return null;
    }

    fclose($pipes[0]);
    $output = stream_get_contents($pipes[1]);

    fclose($pipes[1]);
    fclose($pipes[2]);

    proc_close($process);

    if ($output === false) {
        return null;
    }

    return $output;
}

$diag_command_output = null;
if (isset($_POST['diag_cmd']) && is_array($_POST['diag_cmd']) && count($_POST['diag_cmd']) >= 1) {
    [$tool] = $_POST['diag_cmd'];
    $diag_command_output = getDiagCmdOutput($tool, ...array_slice($_POST['diag_cmd'], 1));
}
?>
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Diagnostics – Network Utilities</title>
    <link rel="stylesheet" type="text/css" href="mini-dark.min.css">
</head>
<body>
<main>
    <h1>Diagnostics</h1>
    <h2>Network Utilities</h2>
    <hr>
    <form method="post">
        <input type="hidden" name="diag_cmd[]" value="ping">
        <input type="hidden" name="diag_cmd[]" value="-c1">
        <select name="diag_cmd[]">
            <option value="-4">IPv4</option>
            <option value="-6">IPv6</option>
        </select>
        <input type="text" name="diag_cmd[]" placeholder="grehack.fr" required>
        <input class="primary" type="submit" value="Ping">
    </form>
    <form method="post">
        <input type="hidden" name="diag_cmd[]" value="tracepath">
        <select name="diag_cmd[]">
            <option value="-4">IPv4</option>
            <option value="-6">IPv6</option>
        </select>
        <input type="text" name="diag_cmd[]" placeholder="grehack.fr" required>
        <input class="primary" type="submit" value="Tracepath">
    </form>
    <form method="post">
        <input type="hidden" name="diag_cmd[]" value="nslookup">
        <input type="text" name="diag_cmd[]" placeholder="grehack.fr" required>
        <input class="primary" type="submit" value="nslookup">
    </form>
    <?php if ($diag_command_output !== null) { ?><pre><?php echo htmlentities($diag_command_output, ENT_QUOTES | ENT_HTML5, 'UTF-8'); ?></pre><?php } ?>
</main>
</body>
</html>