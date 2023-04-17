import sys
from sys import platform
import requests
import urllib.request
from bs4 import BeautifulSoup
from selenium import webdriver
from selenium.webdriver.common.by import By
from webdriver_manager.chrome import ChromeDriverManager
from selenium.common.exceptions import NoSuchElementException
import os

def download_pics(subreddit):
    url = 'https://www.reddit.com/r/' + subreddit
    
    # Different OS will have different chrome driver
    if platform == 'linux' or platform == 'linux2':
        chrome_driver_path = "drivers/chromedriver_linux/chromedriver"
    elif platform == 'darwin':
        chrome_driver_path = "drivers/chromedriver_mac/chromedriver"
    if platform == 'win32':
        chrome_driver_path = "images/chromedriver_win/chromedriver.exe"
    
    # Headless option means the process will happen in an invisible chrome window
    chromeOptions = webdriver.ChromeOptions()
    # chromeOptions.add_argument("--headless=new")
    
    print("PATH:  " + chrome_driver_path)
    # The driver object will be used to create and interact with an instance of Chrome
    driver = webdriver.Chrome(executable_path=chrome_driver_path, chrome_options=chromeOptions)
    
    # Opens the url in a chrome window
    driver.get(url)
    # Implicitly_wait(5) acts as a sleep method that wait 5 seconds for the contents of a page to load
    driver.implicitly_wait(5)
    
    # Each post is contained in a div with the specified class, so by using the find_elements_by_xpath
    # method, we can get a list of all the posts
    posts = driver.find_elements(By.XPATH, '//div[@class="_3JgI-GOrkmyIeDeyzXdyUD _2CSlKHjH7lsjx0IpjORx14"]')
    
    # Create a directory to store the images if one doesn't already exist
    if not os.path.exists('images/' + subreddit):
        os.makedirs('images/' + subreddit)
    
    for post in posts:
        # post.get_attribute('innerHTML') will get the source code of each particular post
        soup = BeautifulSoup(post.get_attribute('innerHTML'))
        # A TypeError likely means an ad was examined, so we can just ignore it with this try/except
        try:
            title_split = soup.find('a', href=True)['href'].split('/')
        except TypeError:
            pass
        
        # Using our BeautifulSoup object we can search for an img tag that is defined by a certain class
        image = soup.find('img', {'class': '_2_tDEnGMLxpM6uOa2kaDB3 ImageBox-image media-element _1XWObl-3b9tPy64oaG6fax'})
        
        # Check to make sure the image exists and that the title belongs to a post, not an ad
        if not image or title_split[1] != 'r':
            print('image does not exist, moving on!')
            continue
        
        image_url = image['src']
        title = title_split[5]
        print('DOWNLOADED IMAGE: ' + title)
        
        # downloads the an image given the image url
        req = requests.get(image_url)
        with open(f'images/{subreddit}/{title}.jpg', 'wb') as f:
            f.write(req.content)

    # Close the Chrome webdriver
    driver.quit()
    

def main():
    subreddit = input('Give the name of a subreddit you want to scrape from: ')
    
    download_pics(subreddit)
    

main()