<!DOCTYPE html>
<html>
<head>
    <title>Save User Input</title>
</head>
<body>
    <?php
    session_start();
    $_SESSION['image_index'] = 0;
    // Get the number of files uploaded by the user
    $num_files = count($_FILES['images']['name']);
    // Iterate through and insert all the files into the images folder
    for ($i = 0; $i < $num_files; $i++) {
        $tmpFilePath = $_FILES['images']['tmp_name'][$i];

        // Verify that tmpFilePath exists before continuing
        if ($tmpFilePath != "") {
            $newFilePath = "images/" . $_FILES['images']['name'][$i];

            // Upload the file into the images folder ($newFilePath)
            if(move_uploaded_file($tmpFilePath, $newFilePath)) {
                print 'Successfully transferred file ' . $i;
            } else {
                print 'Failed to transfer file ' . $i;
            }
            print '<br>';
        } else {
            print 'empty file path';
        }
    }

    // Redirect to the postImages.php page after the images have finished uploading
    header('Location: postImages.php');
    ?>
  
</body>
</html>

