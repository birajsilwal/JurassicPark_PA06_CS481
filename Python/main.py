import numpy
import matplotlib.pyplot as plt
from pathlib import Path

f = open("outputN2.txt", 'r')
lines = f.readlines()
data = {}

headers = {'timestep', 'arrived', 'rejected', 'wait-line'}

for header in headers:
    data[header] = []

for x in lines:
    line = x.split(' ')

    data['timestep'].append(line[0])
    data['arrived'].append(line[2])
    data['rejected'].append(line[4])
    data['wait-line'].append(line[6])

# convert from string to proper data type
for key in data.keys():
    data[key] = list(map(int, data[key]))


# for N = 4
f1 = open("outputN4.txt", 'r')
lines1 = f1.readlines()
data1 = {}

headers1 = {'timestep1', 'arrived1', 'rejected1', 'wait-line1'}

for header1 in headers1:
    data1[header1] = []

for x1 in lines1:
    line1 = x1.split(' ')

    data1['timestep1'].append(line1[0])
    data1['arrived1'].append(line1[2])
    data1['rejected1'].append(line1[4])
    data1['wait-line1'].append(line1[6])

# convert from string to proper data type
for key1 in data1.keys():
    data1[key1] = list(map(int, data1[key1]))

# for N = 6
f2 = open("outputN6.txt", 'r')
lines2 = f2.readlines()
data2 = {}

headers2 = {'timestep2', 'arrived2', 'rejected2', 'wait-line2'}

for header2 in headers2:
    data2[header2] = []

for x2 in lines2:
    line2 = x2.split(' ')

    data2['timestep2'].append(line2[0])
    data2['arrived2'].append(line2[2])
    data2['rejected2'].append(line2[4])
    data2['wait-line2'].append(line2[6])

# convert from string to proper data type
for key2 in data2.keys():
    data2[key2] = list(map(int, data2[key2]))




# people arrival
plt.plot(data['arrived'], "-g", label="25, 45, 35, 25")
plt.title("People Arrival")
plt.xlabel("Time (minutes)")
plt.ylabel("Arrival #")
plt.legend()
plt.show()

# waiting queue
plt.plot(data['wait-line'], "-r", label="car=2, person=10")
plt.plot(data1['wait-line1'], "-g", label="car=4, person=10")
plt.plot(data2['wait-line2'], "-b", label="car=6, person=10")
plt.title("Waiting Queue")
plt.xlabel("Time (minutes)")
plt.ylabel("# of persons in waiting")
plt.legend()
plt.show()


# GOAway People
plt.plot(data['rejected'], "-r", label="car=2, person=10")
plt.plot(data1['rejected1'], "-g", label="car=4, person=10")
plt.plot(data2['rejected2'], "-b", label="car=6, person=10")
plt.title("GoAway People")
plt.xlabel("Time (minutes)")
plt.ylabel("# of persons being rejected")
plt.legend()
plt.show()
