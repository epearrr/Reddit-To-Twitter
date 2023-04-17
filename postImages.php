<?php
$python_path = "/users/e/j/ejperry/www-root/cs120/M3OEP-ejperry-nsmith34/venv/bin/";
   
session_start();

if (!isset($_SESSION['image_index'])) {
    $_SESSION['image_index'] = 0;
}

$image_locations = array();
    if ($handle = opendir('images')) {
        while (false !== ($file = readdir($handle))) {
            if ('.' === $file) continue;
            if ('..' === $file) continue;
            
            array_push($image_locations, 'images/' . $file);
        }
        closedir($handle);
    }

// Get the current image using the image_index session variable
$image = $image_locations[$_SESSION['image_index']];

// Display the image
print '<img src='. $image . ' height=500/>';
print '<br>';
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="author" content="Ethan Perry, Nate Smith">
    <title>Post Images</title>
    <meta name="description" content="This page is for uploading images to Twitter">
    <link href="style.css" rel="stylesheet" type="text/css" media="screen" />
</head>
<body>
    <br>
    <!-- Post button -->
    <form method="post">
        <input type="submit" name="post_image" value="Post!">
    </form>

    <!-- Skip button -->
    <form method="post">
        <input type="submit" name="skip_image" value="Skip...">
    </form>


    <?php
    // Check if the post button is clicked
    if (isset($_POST['post_image'])) {
        // Upload the picture to Twitter using the uploadToTwitter script and $image as an argument
        $output = shell_exec($python_path . 'python uploadToTwitter.py ' . $image);
        print $output;

        $_SESSION['image_index']++;

        // If the image_index >= the number of images, go back to the home page.
        if ($_SESSION['image_index'] >= count($image_locations)) {
            header("Location: index.html");
            exit;
        }

        // Reload the page to display the next image
        header("Location: postImages.php");
        exit;
    }
    
    // Check if the skip button is clicked; operates the same as the post button except nothing gets posted
    if (isset($_POST['skip_image'])) {
        $_SESSION['image_index']++;

        // If the image_index >= the number of images, go back to the home page.
        if ($_SESSION['image_index'] >= count($image_locations)) {
            header("Location: index.html");
            exit;
        }

        // Reload the page to display the next image
        header("Location: postImages.php");
        exit;
    }

    ?>
</body>
</html>