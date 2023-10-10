# -----------------------------------------------------------
# Serial Interface to communicate with the Arduino Button
#  Interface.
#
# (C) 2023 Daniel VanVolkinburg 
# Released under GNU Public License (GPL)
# email dvanvolk@ieee.org
# -----------------------------------------------------------
import serial
from serial.threaded import LineReader, ReaderThread
import time
import sys

class SerialRadioInterface(LineReader):
    """This class overrides the LineReader class and is responsible for getting information from the serial interface
        The radio interface does not need to write, but if needed the function write_line can be used"""

    def __init__(self, button_event_callback):
        self.event_callback = button_event_callback

    def connection_made(self, transport):
        """Function overrides the base class and reports when the serial por is open"""
        super(SerialRadioInterface, self).connection_made(transport)
        sys.stdout.write('Serial Port opened\n')
        # self.write_line('hello world')

    def handle_line(self, data):
        """Function overrides the base class and handles incoming data from the serial port, it will check the response and signal what button was pressed"""
        sys.stdout.write('line received: {}\n'.format(repr(data)))


    def connection_lost(self, exc):
        """Function overrides the base class and reports when the connection is lost"""
        if exc:
            traceback.print_exc(exc)
        sys.stdout.write('port closed\n')

    def process_button(self, id):
        pass
            

def CreateInterface(com_port):
    ser = serial.Serial(port=com_port, baudrate=115200, timeout=1)
    serial_interface = ReaderThread(ser, SerialRadioInterface)
    # serial_interface.write_line('hello')
        

if __name__ == '__main__':
    """Test the Serial Interface"""
    print("Start")
    CreateInterface("COM1")