#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt

def main():
    csv_file = './log.csv'
    data = pd.read_csv(csv_file)
    npages = data["NPAGES"]
    total_time = data["TIME"]
    x = list(npages)
    y = list(total_time)
    plt.scatter(x, y)
    plt.xlabel("Number of pages")
    plt.ylabel("Time")
    plt.title("TLB Access Time")
    plt.show()


if __name__ == '__main__':
    main()
