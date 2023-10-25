# TrickOrTreaterCount

## About The Project
For years I have been manually counting the number of Trick-Or-Treaters, this year I decided to automate the process. I had been breaking the time up into 15 minute blocks and graphing the count for each block in Excel.  This project automates the data collection and allows for public monitoring of the count. 

The project uses hardware from an old project, which used a 900Mz radio interface, instead of a WIFI based interface.  

## Usage
Run the TrickOrTreaters python script
    python TrickOrTreaters.py -p COM7

The will look for button presses on the serial interface, or MQTT messages.  Every minute it will send data to [Adafruit IO](https://io.adafruit.com/dvanvolk/dashboards/2023-count-dashboard) IO for Live updates.  

When finish type "end" and it will stop logging, save a CSV file of the time stamped data and graph the output.

## Hardware
  [Adafruit Feather M0 RFM69HCW Packet Radio](https://www.adafruit.com/product/3176)
  