import os
import set_env_vars
import sys
import time
import tweepy


# CONSTANTS
ACCESS_TOKEN = os.environ.get('TWT_ACCESS_TOKEN')
ACCESS_SECRET = os.environ.get('TWT_ACCESS_SECRET')
CONSUMER_KEY = os.environ.get('TWT_CONSUMER_KEY')
CONSUMER_SECRET = os.environ.get('TWT_CONSUMER_SECRET')

# authenticate API keys and return API object
def authenticate_api():
    twitter_auth_keys = {
        "consumer_key"        : CONSUMER_KEY,
        "consumer_secret"     : CONSUMER_SECRET,
        "access_token"        : ACCESS_TOKEN,
        "access_token_secret" : ACCESS_SECRET
    }

    auth = tweepy.OAuthHandler(
            twitter_auth_keys['consumer_key'],
            twitter_auth_keys['consumer_secret']
            )
    auth.set_access_token(
            twitter_auth_keys['access_token'],
            twitter_auth_keys['access_token_secret']
            )
    api = tweepy.API(auth)
    return api

# Creates api as a global variable
api = authenticate_api()


def main(image_url):
    image = api.media_upload(image_url)
    api.update_status(status = '', media_ids = [image.media_id]) # tweet the message with the image attached


image_url = sys.argv[1]
main(image_url)
