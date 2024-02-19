<?php
    $bookName = array_keys($_GET)[0] ?? ""
?>

<!DOCTYPE html>
<html lang="en-US">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width">
        <title>monlang - Book "<?php echo($bookName) ?>"</title>
    </head>
    <body>
        <script>
            let urlSearchParams = new URLSearchParams(window.location.search)
            let code = urlSearchParams.keys().next()?.value
            console.log(code)
        </script>
    </body>
</html>
