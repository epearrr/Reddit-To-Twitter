<?php
$subreddit = "muglife";

print getcwd();
print PHP_EOL;

// $python_path = "/venv/bin/";

// $output = shell_exec("chmod +x scrapeReddit.py;python scrapeReddit.py " . $subreddit);
$output = shell_exec('venv/Scripts/activate.bat');    
echo $output;
$output = shell_exec($python_path . 'python scrapeReddit.py ' . escapeshellarg($subreddit));    
echo $output;


echo "Done scraping images!";
?>