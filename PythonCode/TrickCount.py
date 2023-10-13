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
import pandas as pd

import time
import random
from random import randrange

class trick_count:
    def __init__(self):
        self.raw_timestamp_df = pd.DataFrame({'count': 0}, index = [datetime.now()])

    def increase_fake(self, min, max):
        time_now = datetime.now()
        current = time_now - timedelta(minutes=randrange(min,max))
        self.raw_timestamp_df = pd.concat([self.raw_timestamp_df, pd.DataFrame({'count': 1}, index = [current])])
        print(f"{current} New Trick-or-Treater")

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
        one_min_bar_plot = min_data.plot.bar(
            color='blue' 
            )
        one_min_bar_plot.set_xlabel("Time Stamp")
        one_min_bar_plot.set_ylabel("Count (Min)")
        one_min_bar_plot.set_title("Count by Min")

        fifteen_min_data = self.raw_timestamp_df['count'].resample('15min').sum().fillna(0)  
        fifteen_min_bar_plot = fifteen_min_data.plot.bar(
            color='red'
            )
        
        raw_bar_plot = self.raw_timestamp_df.plot.bar(
            color='green' 
            )
        
        file_name = f"{datetime.today().year}_RawTrickOrTreatData.csv"
        self.raw_timestamp_df.to_csv(file_name, encoding='utf-8')

        plt.show()

        
if __name__ == '__main__':
    """TEST the Class"""
    tc = trick_count()
    for x in range(0,100):
        time.sleep(random.randint(1, (60*10)))
        tc.increase()

        # min = x
        # max = 60 - x
        # tc.increase_fake(min, max)
        

    print("Done")
    print(f"Time Now: {datetime.now()} Last Min Count: {tc.get_last_minute_count()}")
    print(f"Total Sum: {tc.get_total_count()}")
    tc.plot_output()
