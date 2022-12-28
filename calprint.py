#!/usr/bin/env python3

import sys
import argparse
from datetime import datetime
from datetime import timedelta

# The code below configures the argparse module for use with
# assignment #2.



# Formats date into a readable format
def formatted_date(orig_date):
    int_year = int(orig_date[0:4])
    int_month = int(orig_date[4:6])
    int_day = int(orig_date[6:8])
    x = datetime(int_year, int_month, int_day, 0, 0, 0, 0)

    month = x.strftime("%B")
    day = x.strftime("%d")
    year = x.strftime("%Y")
    day_of_week = x.strftime("%A")
    string = month + " " + day + ", " + year + " (" + day_of_week[0:3] + ")"
    return string

# When a file is inputted, it returns a list of tuples of all of the events in the file
def extract(file):
    l2 = []

    for x in file:
        if(x.startswith("DTSTART")):
            l = [None]*6
            l[0] = (x[8:].replace('\n', ''))

            hour = int(x[17:19])
            minute = int(x[19:21])
            start_time = datetime(2000,1,1,hour, minute, 0, 0)
            time = start_time.strftime("%I:%M ")
            if((hour < 10) or (hour > 12 and hour < 22)):
                time = time[1:]
                time = " " + time
            am_or_pm = start_time.strftime("%p").lower()
            l[4] = time + am_or_pm

        if(x.startswith("DTEND")):
            hour = int(x[15:17])
            minute = int(x[17:19])
            end_time = datetime(2000,1,1,hour, minute, 0, 0)
            time = end_time.strftime("%I:%M ")
            if((hour < 10) or (hour > 12 and hour < 22)):
                time = time[1:]
                time = " " + time
            am_or_pm = end_time.strftime("%p").lower()
            l[5] = time + am_or_pm

        if(x.startswith("RRULE")):
            l[1] = (x[32:47].replace('\n', ''))
        if(x.startswith("LOCATION")):
            l[2] = (x[9:].replace('\n', ''))
        if(x.startswith("SUMMARY")):
            l[3] = (x[8:].replace('\n', ''))
            tup = (l[0], l[1], l[2], l[3], l[4], l[5])
            l2.append(tup)
    return l2

# Turns the start and end date into YYYYMMDD format
def args_to_date(args):
    int_day = int(args[0])
    int_month = int(args[1])
    str_day = args[0]
    str_month = args[1]
    str_year = args[2]
    L = [None] * 3

    if(int_day < 10):
        str_day = "0" + str(int_day)
    if(int_month < 10):
        str_month = "0" + str(int_month)
    str_year = args[2]

    L[0] = str_year
    L[1] = str_month
    L[2] = str_day
    str_joined = ''.join(str(x) for x in L)
    return str_joined

# Increment a date by 7 days
def increment(orig_date):
    int_year = int(orig_date[0:4])
    int_month = int(orig_date[4:6])
    int_day = int(orig_date[6:8])
    rest = orig_date[8:]
    x = datetime(int_year, int_month, int_day, 0, 0, 0, 0) + timedelta(days=7)

    year = x.strftime("%Y")
    month = x.strftime("%m")
    day = x.strftime("%d")
    incremented_date = year + month + day + rest
    return incremented_date

# Formats date into a readable format
def formatted_date(orig_date):
    int_year = int(orig_date[0:4])
    int_month = int(orig_date[4:6])
    int_day = int(orig_date[6:8])
    x = datetime(int_year, int_month, int_day, 0, 0, 0, 0)

    month = x.strftime("%B")
    day = x.strftime("%d")
    year = x.strftime("%Y")
    day_of_week = x.strftime("%A")
    string = month + " " + day + ", " + year + " (" + day_of_week[0:3] + ")"
    return string

# Adds every event from starting date to RRULE date, not accounting for main args yet
def add_events(orig_list):
	d = 0
	new_list = []

	for i in range(len(orig_list)):
		if(orig_list[i][1] != None):
			y = orig_list[i][0]
			while(y < orig_list[i][1]):
				l1 = [None]*6
				l1[0] = y
				l1[1] = orig_list[i][1]
				l1[2] = orig_list[i][2]
				l1[3] = orig_list[i][3]
				l1[4] = orig_list[i][4]
				l1[5] = orig_list[i][5]
				tup = (l1[0], l1[1], l1[2], l1[3], l1[4], l1[5])
				new_list.append(tup)
				y = increment(y)
		else:
			new_list.append(orig_list[i])
	#print(new_list)
	return new_list

# Reduces the amount of events according to start and end dates given by the main args
def real_events(orig_list, begin, end):
	l = []

	for x in range(len(orig_list)):
		if((orig_list[x][0][:8] >= begin and orig_list[x][0][:8] <= end)):
			l.append(orig_list[x])

	return l

def info(orig_list):
    l1 = []
    num = len(orig_list)
    for x in range(num):
        l = []
        dt = formatted_date(orig_list[x][0])
        hyphens = '-' * len(dt)
        time = orig_list[x][4] + " to " + orig_list[x][5] + ": " + orig_list[x][3] + " [" + orig_list[x][2] + "]"
        l.append(dt)
        l.append(hyphens)
        l.append(time)
        tup = (l[0], l[1], l[2])
        l1.append(tup)
    return l1

def print_events(orig_list):
    num = len(orig_list)
    l = []
    final_list = []

    for x in range(num):
        l1 = [None]*3
        l1[0] = orig_list[x][0]
        l1[1] = orig_list[x][1]
        l1[2] = orig_list[x][2]
        l.append(l1)
    #print(l)

    for item in l:
        for y, z in enumerate(final_list):
            if item[0]==z[0]:
                final_list[y] += item[2:]
                break
        else:
            final_list.append(item)

    n = 0
    for item in final_list:
        for w in range(len(item)):
            print(item[w])
        if(n < len(final_list) - 1):
            print()
            n = n + 1


def main():
    events = []
    start_date = []
    end_date = []
    parser = argparse.ArgumentParser()
    parser.add_argument('--file', type=str, help='file to be processed')
    parser.add_argument('--start', type=str, help='start of date range')
    parser.add_argument('--end', type=str, help='end of data range')

    args = parser.parse_args()

    #print ('file: {}; start: {}; end: {}'.format( args.file, args.start,
    #    args.end))

    if not args.file:
        print("Need --file=<ics filename>")

    if not args.start:
        print("Need --start=dd/mm/yyyy")

    if not args.end:
        print("Need --end=dd/mm/yyyy")

    # Turns original argument passed into YYYYMMDD format
    start_date_args = args.start.split('/')
    end_date_args = args.end.split('/')
    start_date = args_to_date(start_date_args)
    end_date = args_to_date(end_date_args)

    # Open file, extract contents, close file
    f = open(args.file, "r")
    print(type(args.file))
    events = extract(f)
    f.close()

    formatted_date(start_date)
    print(events)
    all_events = add_events(events)
    all_events.sort(key=lambda tup: tup[0])
    real = real_events(all_events, start_date, end_date)
    new = info(real)
    print_events(new)

if __name__ == "__main__":
    main()
