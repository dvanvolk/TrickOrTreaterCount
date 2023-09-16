# -----------------------------------------------------------
# Wrapper class for the Adafruit IO interface, based on the 
#  Adafruit digital_in example. Class only sends digital data
#   and does not receive data.
#
# (C) 2023 Daniel VanVolkinburg 
# Released under GNU Public License (GPL)
# email dvanvolk@ieee.org
# -----------------------------------------------------------

# import Adafruit IO REST client.
from Adafruit_IO import Client, Feed, RequestError

class adafruit_io_interface:
    def __init__(self, config, feed_list):
        """Connect the Adafruit IO interface
        config - dictionary containing username and api_key 
        feed_list - list of feeds to connect to"""
        self.aio = Client(config.get('username'), config.get('api_key'))
        self.feed_list = []

        for feed_name in feed_list:
            try: # if we have a 'digital' feed
                aio_feed = self.aio.feeds(feed_name)
            except RequestError: # create a digital feed
                feed = Feed(name=feed_name)
                aio_feed = self.aio.create_feed(feed)
            self.feed_list.append(aio_feed)
            
    def send_status(self, feed_name, data):
        """Send feed data"""
        for items in self.feed_list:
            if items.key == feed_name:
                self.aio.send_data(items.key, data)
