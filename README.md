# TrickOrTreaterCount

## About The Project
For years I have been manually counting the number of Trick-Or-Treaters, this year I decided to automate the process. I had been breaking the time up into 15 minute blocks and graphing the count for each block in Excel.

The original goal of the project was to use MQTT to send a message to a influxdb database and Graph the output into  grafana.  To do this I thought I needed a python script to convert the MQTT Message into a database entry.  I think there are several ways of doing this but that seemed easy.  Once I started the python script I just kept going with its graphing features.  

The original plan was to have a WIFI based feather send the MQTT message when I pressed a button.  But I already had some hardware from an unfinished project and decided to "finish" the software for that instead of unsolder the feather with the 900MHz Radio and replace it with a WIFI based module.  

## Usage
Run the python script, it will look for button presses on the serial interface, or MQTT messages.  Every minute it will send data to [Adafruit](https://www.adafruit.com/) IO for Live updates.  

When finish type "end" and it will stop logging, save a CSV file of the time stamped data and graph the output.

