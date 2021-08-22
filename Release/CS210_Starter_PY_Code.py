import re
import string
import csv
from collections import Counter

def printsomething():
    print("Hello from python!")

def PrintMe(v):
    print("You sent me: " + v)
    return 100;

def SquareValue(v):
    return v * v

def displayTimesItemPurchased(item = ""):
    # open file
    file = open('CS210_Project_Three_Input_File.txt', 'r')
    Lines = file.read().splitlines() # read each line into Lines list
    file.close()

    c = Counter(Lines) # Counter counts the amount of times each item appears in the list, adds them to key/val pair dictionary.

    for key in c:
        val = c.get(key, 0) # get value of the key

        # if the item parameter is not empty (item is provided)
        if item != "":
            if key.lower() == item.lower():
                return val
        else:
            print("{}:{}".format(key, val)) # print key/val pair
    return 0;

def writeItemsSold():
    # open file
    file = open('CS210_Project_Three_Input_File.txt', 'r')
    Lines = file.read().splitlines() # read each line into Lines list
    file.close()

    c = Counter(Lines) # Counter counts the amount of times each item appears in the list, adds them to key/val pair dictionary.

    f = open("frequency.dat", "w")
    for key in c:
        val = c.get(key, 0) # get value of the key
        f.write("{}:{}\n".format(key, val)) # write key/val pair to file

    f.close()
