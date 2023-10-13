# -----------------------------------------------------------
# File listens for Button MQTT message and handles all of the coordination events
#  including notifying all parties of the new count
#
# (C) 2023 Daniel VanVolkinburg 
# Released under GNU Public License (GPL)
# email dvanvolk@ieee.org
# -----------------------------------------------------------

import argparse
import os
import json
import time
import schedule
from datetime import datetime
from threading import Thread
from TrickCount import trick_count
from serial_interface import RadioInterface
from mqtt_interface import mqtt
from adafruit_io import adafruit_io_interface

REPORT_TIME_SEC = 60

class TrickOrTreaterTracker:
    def __init__(self, mqtt_config, aio_config, serial_port):
        """Init Class and create MQTT interface and Monitoring Thread"""
        self.tc = trick_count()
        subscribe_list = ["Halloween\ButtonPress"]
        self.mqtt_interface = mqtt(mqtt_config, self.__msg_callback, subscribe_list)

        self.monitor = True
        self.report_schedule = schedule.every().minute.at(":00").do(self.__report_status)
        self.daemon = Thread(target=self.__schedule_thread, args=(), daemon=True, name='TrickStatus')

        self.feedlist = ['totalcount', 'count']
        self.aio = adafruit_io_interface(aio_config, self.feedlist)

        self.serial_port = serial_port
        self.serial_radio = RadioInterface()
        button_callback = [self.__count_btn_callback, None, self.__down_btn_callback]
        self.serial_radio.start(self.serial_port, button_callbacks = button_callback)
        
    def start(self):
        """Start taking numbers, wait here till the user types "end" """
        self.daemon.start()

        print("Start Counting")
        
        continue_thread = True
        while continue_thread:
            input_str = input()
            if input_str == "end":
                continue_thread = False
                self.__process_finish()
                print("End Program")
            time.sleep(1)
    
    def __msg_callback(self, msg_topic):
        """Callback from the MQTT class when a new message is received"""
        self.tc.increase()
    
    def __count_btn_callback(self):
        """Callback from the button interface when the count should be increased"""
        print(f"Button Press")
        self.tc.increase()
        
    def __down_btn_callback(self):
        """Incase of too many button presses, remove the last entry"""
        self.tc.decrease()

    def __process_finish(self):
        """Complete the counting process and show the graph"""
        self.monitor = False
        total_count = self.tc.get_total_count()
        print(f"Total Count: {total_count}")
        if total_count > 0:
            self.tc.plot_output()

    def __schedule_thread(self):
        """Thread to report the latest count every minuit, thread runs the scheduler to do the event on the min"""
        while self.monitor:
            sleep_time = schedule.idle_seconds()
            if sleep_time is None:
                # no more jobs
                break
            elif sleep_time > 0:
                # sleep exactly the right amount of time
                time.sleep(sleep_time) # close enough?
            schedule.run_pending()
        schedule.cancel_job(self.report_schedule)

    def __report_status(self):
        """Report the latest count every minuit"""
        min_count = self.tc.get_last_minute_count()
        total_count = self.tc.get_total_count() 
        self.aio.send_status(self.feedlist[0], int(total_count))
        self.aio.send_status(self.feedlist[1], int(min_count))
        print(f"Report Time Now: {datetime.now()} Last Min Count: {min_count}, Total: {total_count}")
        # self.tc.mark()


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
    parser = argparse.ArgumentParser(prog='Trick-Or-Treater Counter',
                                     description='Timestamp Button Presses, counting Trick-Or-Treaters and publish results')
    parser.add_argument('-fm', required=False, type = is_file, default="mqtt_keys.json", help="Json Config File for local MQTT broker")
    parser.add_argument('-fa', required=False, type = is_file, default="adafruit_info.json", help="Json Config File for Adafrut Io Interface")
    parser.add_argument('-p', required=False, default="COM10", help="Serial port, the radio interface is connected to")
    args = parser.parse_args()

    aio_config = None
    mqtt_config = None

    with open(args.fm) as config_file:
        mqtt_config = json.load(config_file)

    with open(args.fa) as config_file:
        aio_config = json.load(config_file)

    if mqtt_config is not None:    
        TrickOrTreaterTracker(mqtt_config, aio_config, args.p).start()
        
