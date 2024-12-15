import serial
import time
import numpy as np
import matplotlib.pyplot as plt
import subprocess

def choose_device():
    command = "ls /dev/tty.*"
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = process.communicate()

    devices = output.decode('utf-8').split('\n')[:-1]

    print("Choose device:")
    for i in range(len(devices)):
        print(f"{i+1}) {devices[i]}")
    device = int(input("Enter number: ")) - 1
    while device < 0 or device >= len(devices):
        device = int(input("Incorrect. Enter number: ")) - 1
    print()
    return devices[device]



device = choose_device()
ser = serial.Serial()
ser.port = device
ser.baudrate = 115200
ser.dtr = 0
ser.open()

print("Init...")
time.sleep(2)
print()
for i in range(4):
    response = ser.readline().decode("utf-8").strip()
    print(response)

mode = input("Enter mode: ")
while mode not in ("r", 'w', 'p'):
    mode = input("Unknown mode. Repeat: ")
print()

ser.write(mode.encode("utf-8"))

if mode == 'r':
    temp = []
    hum = []
    response = ""

    while response != "begin":
        response = ser.readline().decode("utf-8").strip()

    print("Reading...")
    while response != "end":
        response = ser.readline().decode("utf-8").strip()
        if (response != "end"):
            temp.append(float(response.split(", ")[0]))
            hum.append(float(response.split(", ")[1]))

    ser.close()

    t = np.arange(len(temp))

    plt.plot(t, temp, label="temperature")
    plt.plot(t, hum, label="humidity")
    plt.title("Temperature and humidity scans")
    plt.xlabel("t, s")
    plt.legend()
    print("Success.")
    print("Total scans:", len(temp))
    print()
    plt.show()

elif mode == 'p' or mode == 'w':
    while True:
        response = ser.readline().decode("utf-8").strip()
        print(response)