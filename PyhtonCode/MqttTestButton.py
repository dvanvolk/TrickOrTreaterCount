# python 3.6
# -----------------------------------------------------------
# Command line script which sends MQTT "button pressed" events 
#  to the Counting script, this used to test the Trick Count
#  script
#  
# (C) 2023 Daniel VanVolkinburg 
# Released under GNU Public License (GPL)
# email dvanvolk@ieee.org
# -----------------------------------------------------------
import argparse
import json
import os
import time
import cmd
from mqtt_interface import mqtt

class ButtonTest(cmd.Cmd):
    def __init__(self, mqtt_config):
        super().__init__()
        """Init Class and create MQTT interface"""
        self.send_topic = "Halloween\ButtonPress"
        self.mqtt_interface = mqtt(mqtt_config, None, [])
        self.intro = '\n'
        self.prompt = '> '
        # use_rawinput = 0
        
    def send_button_press(self):
        """Send a Button Press MQTT Message"""
        print("Button Press")
        self.mqtt_interface.publish(self.send_topic, 1)

    def emptyline(self):
        """Sending an empty line will simulate a button press"""
        self.send_button_press()

    def default(self, line):
        """Sending anything will simulate a button press"""
        self.send_button_press()

    def do_x(self, count):
        """Send X number of presses in a row (example: x 5, will send 5)"""
        for x in range(int(count)):
            self.send_button_press()

    def do_exit(self, line):
        return 1
        

def is_file(value):
    """Called from argparse to ensure the file exists"""
    if os.path.isfile(value):
        if value.endswith('.json'):
            return value
        else:
         raise argparse.ArgumentTypeError(f"File must have the json extension")   
    else:
         raise argparse.ArgumentTypeError(f"File Does not exist: {value}")
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='MqttTestButton.py',
                                     description='Command Shell, used to send simulated button presses, to test the counting script')
    parser.add_argument('-fm', required=False, type = is_file, default="mqtt_keys.json", help="Json Config File for local MQTT broker")
    args = parser.parse_args()

    mqtt_config = None

    with open(args.fm) as config_file:
        mqtt_config = json.load(config_file)

    if mqtt_config is not None:
        bt = ButtonTest(mqtt_config).cmdloop()       
    

        