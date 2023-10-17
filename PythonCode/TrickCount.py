# -----------------------------------------------------------
# Class tracks new trick-or-treaters by time stamping the data when it comes in
#  and adding it to a data frame
#
# (C) 2023 Daniel VanVolkinburg 
# Released under GNU Public License (GPL)
# email dvanvolk@ieee.org
# -----------------------------------------------------------

from datetime import datetime, timedelta
import matplotlib.pyplot as plt
from matplotlib.dates import DateFormatter
import matplotlib.dates as mdates
import pandas as pd

import time
import random
from random import randrange

import numpy as np #used for testing only

class trick_count:
    def __init__(self):
        self.raw_timestamp_df = pd.DataFrame({'count': 0}, index = [datetime.now()])

    # def increase_fake(self, min, max):
    #     time_now = datetime.now()
    #     current = time_now - timedelta(minutes=randrange(min,max))
    #     self.raw_timestamp_df = pd.concat([self.raw_timestamp_df, pd.DataFrame({'count': 1}, index = [current])])
    #     print(f"{current} New Trick-or-Treater")

    def increase(self):
        """increase the trick-or-treater count and time stamp a new entry into the data frame"""
        time_now = datetime.now()
        self.raw_timestamp_df = pd.concat([self.raw_timestamp_df, pd.DataFrame({'count': 1}, index = [time_now])])
        print(f"{time_now} New Trick-or-Treater")

    def decrease(self):
        """Remove the last entry incase the button is accidentally pressed"""
        time_now = datetime.now()
        self.raw_timestamp_df.drop(self.raw_timestamp_df.tail(1).index,inplace=True)
        print(f"{time_now} Remove -- Trick-or-Treater")

    def mark(self):
        """set the new min to zero, which is a lazy way of inserting some zero into the graph"""
        time_now = datetime.now()
        self.raw_timestamp_df = pd.concat([self.raw_timestamp_df, pd.DataFrame({'count': 0}, index = [time_now])])
        
    def get_last_minute_count(self):
        """returns the count of th previous minute's worth of data, by analyzing the """
        output_df = self.raw_timestamp_df
        # print(output_df)

        min_data = self.raw_timestamp_df['count'].resample('min').sum()

        last_min = (datetime.now() - timedelta(hours=0, minutes=1)).strftime('%H:%M')
        last_min_data = min_data.between_time(last_min, last_min)
        count = 0
        if len(last_min_data):
                count = last_min_data.values[0]
        return count
            
    def get_total_count(self):
        """Return the Total Count of all Trick-or-Treaters"""
        sum = self.raw_timestamp_df['count'].sum()
        return sum
    
    def plot_output(self): 
        """Create some graphs and save the data"""  
        min_data = self.raw_timestamp_df['count'].resample('min').sum()
        min_df = pd.DataFrame(min_data)
        min_df.index = pd.to_datetime(min_df.index, format = '%I:%M:%S').strftime('%I:%M')
        min_plot = min_df.plot.bar(color = 'purple')
        min_plot.set_xlabel("Time Stamp")
        # min_plot.xaxis.set_major_locator(mdates.MinuteLocator(interval=1)) 
        min_plot.set_ylabel("Count (Min)")
        min_plot.set_title("Count by Min")
    
        fifteen_min_data = self.raw_timestamp_df['count'].resample('15min').sum()
        fifteen_min_df = pd.DataFrame(fifteen_min_data)
        fifteen_min_df.index = pd.to_datetime(fifteen_min_df.index, format = '%I:%M:%S').strftime('%I:%M')
        fifteen_min_bar_plot = fifteen_min_df.plot.bar( color='red' )
        fifteen_min_bar_plot.set_xlabel("Time Stamp")
        fifteen_min_bar_plot.set_ylabel("Count (Min)")
        fifteen_min_bar_plot.set_title("Count by 15 Mins")
        
        # raw_bar_plot = self.raw_timestamp_df.plot.bar( color='green' )
        # raw_bar_plot.set_title("Raw Timestamped data")
        self.raw_timestamp_df['total_count'] = self.raw_timestamp_df['count'].cumsum()
        
        # Create a timeline plot of the total count
        plt.figure(figsize=(10, 3))
        plt.plot(self.raw_timestamp_df.index, range(len(self.raw_timestamp_df)), marker='x', linestyle='-')
        plt.yticks(range(len(self.raw_timestamp_df)), self.raw_timestamp_df['total_count'])
        plt.gca().xaxis.set_major_formatter(DateFormatter('%I:%M'))
        plt.gca().xaxis.set_major_locator(mdates.MinuteLocator(interval=15))
        plt.xlabel('Time')
        plt.xticks(minor=True)
        plt.title('Total Trick-or-Treaters')
        plt.grid(axis='x')
        plt.tight_layout()
        
        raw_file_name = f"{datetime.today().year}_RawTrickOrTreatData.csv"
        self.raw_timestamp_df.to_csv(raw_file_name, encoding='utf-8')

        min_file_name = f"{datetime.today().year}_MinTrickOrTreatData.csv"
        min_df.to_csv(min_file_name, encoding='utf-8')

        fifteen_file_name = f"{datetime.today().year}_15minTrickOrTreatData.csv"
        fifteen_min_df.to_csv(fifteen_file_name, encoding='utf-8')

        plt.show()

def generate_test_data():
    """Generate a data set to test graphing with"""
    number_of_hours = 3
    number_of_trick_or_treaters = random.randint(30, 100)
    end_time = datetime.now()
    start_time = end_time - timedelta(hours=number_of_hours)
        
    random_times_list = []
    
    for _ in range(number_of_trick_or_treaters):
        random_min = random.randint(0, (number_of_hours * 60))
        random_time = (start_time + timedelta(minutes=random_min))
        random_times_list.append(random_time)
    df = pd.DataFrame({'Time': random_times_list, 'count': 1}).sort_values('Time')
    # df['total_count'] = df['count'].cumsum()
    df = df.set_index('Time')
    # print(df)

    tc = trick_count()
    tc.raw_timestamp_df = df
    
    tc.plot_output()
    

if __name__ == '__main__':
    """TEST the Class"""
    print("Start Test Counting")
    generate_test_data()

    """simulate data, in real time"""
    # tc = trick_count()
    # for x in range(0,15):
    #     time.sleep(random.randint(1, (60*2)))
    #     tc.increase()      
    # print("Done")
    # print(f"Time Now: {datetime.now()} Last Min Count: {tc.get_last_minute_count()}")
    # print(f"Total Sum: {tc.get_total_count()}")
    # tc.plot_output()
