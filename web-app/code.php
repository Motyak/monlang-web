<?php
    $srcFile = array_keys($_GET)[0] ?? "";
?>

<!DOCTYPE html>
<html lang="en-US">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width">
        <title>monlang - Code "<?php echo($srcFile) ?>"</title>
    </head>

    <?php
        $srcFile = escapeshellarg($srcFile);
        error_log("cmd = `bash test.sh $srcFile`");
        $exitCode = null;
    ?>

    <body>
        <pre><?php system("bash test.sh $srcFile", $exitCode) ?></pre>
    </body>

    <?php
    error_log("exited with $exitCode");
    ?>
</html>
