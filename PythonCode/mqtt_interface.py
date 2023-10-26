# -----------------------------------------------------------
# File manages the Mqtt Connection, to send and receive messages
#  mqtt class is just a wrapper for mqtt interface, which can 
#  send and receive message
#
# (C) 2023 Daniel VanVolkinburg 
# Released under GNU Public License (GPL)
# email dvanvolk@ieee.org
# -----------------------------------------------------------
from paho.mqtt import client as mqtt_client
import random
import logging
import time
import json

PORT = 1883
FIRST_RECONNECT_DELAY = 1
RECONNECT_RATE = 2
MAX_RECONNECT_COUNT = 12
MAX_RECONNECT_DELAY = 60

class mqtt:
    def __init__(self, mqtt_config, message_callback, subscribe_list):
        """Init The Class, 
           mqtt_config - dictionary, with MQTT Configuration information,broker,username,password
           message_callback - Callback function for new messages, None if None, function takes the topic as an argument
           subscribe_list - List of messages to subscribe to """
        self.msg_cb = message_callback
        self.sub_list = subscribe_list
        self.client = self.__connect_mqtt(mqtt_config.get('broker'), mqtt_config.get('username'), mqtt_config.get('password'))
        self.client.loop_start()

    def __on_connect(self, client, userdata, flags, rc):
        if rc == 0 and client.is_connected():
            print("Connected to MQTT Broker!")
            for topic in self.sub_list:
                client.subscribe(topic)
        else:
            print(f'Failed to connect, return code {rc}')

    def __on_disconnect(self, client, userdata, rc):
        logging.info("Disconnected with result code: %s", rc)
        reconnect_count, reconnect_delay = 0, FIRST_RECONNECT_DELAY
        while reconnect_count < MAX_RECONNECT_COUNT:
            logging.info("Reconnecting in %d seconds...", reconnect_delay)
            time.sleep(reconnect_delay)

            try:
                client.reconnect()
                logging.info("Reconnected successfully!")
                return
            except Exception as err:
                logging.error("%s. Reconnect failed. Retrying...", err)

            reconnect_delay *= RECONNECT_RATE
            reconnect_delay = min(reconnect_delay, MAX_RECONNECT_DELAY)
            reconnect_count += 1
        logging.info("Reconnect failed after %s attempts. Exiting...", reconnect_count)
        

    def __on_message(self, client, userdata, msg):
        if self.msg_cb is not None:
            self.msg_cb( msg.topic )
        # print(f'Received `{msg.payload.decode()}` from `{msg.topic}` topic')

    def __connect_mqtt(self, broker, username, password):
        client_id = f'python-mqtt-trick-{random.randint(0, 1000)}'
        client = mqtt_client.Client(client_id)
        client.username_pw_set(username, password)
        client.on_connect = self.__on_connect
        client.on_message = self.__on_message
        client.connect(broker, PORT, keepalive=120)
        client.on_disconnect = self.__on_disconnect
        return client
    
    def publish(self, topic, msg_data):
        if self.client.is_connected():
            result = self.client.publish(topic, msg_data)
            # result: [0, 1]
            status = result[0]
            if status == 0:
                print(f'Send `{msg_data}` to topic `{topic}`')
            else:
                print(f'Failed to send message to topic {topic}')
        else:
            logging.error("publish: MQTT client is not connected!")

