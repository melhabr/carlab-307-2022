import math
import struct
import serial
import numpy as np

ser = serial.Serial('/dev/cu.usbmodem112401')
ser.flushInput()

lastLSMData = (0, 0, 0, 0, 0, 0)
lastBNOData = (0, 0, 0)

while True:
    
    data_type = ser.read()
    if data_type[0] == 127: # LSM
        data = ser.read(24)
        try:
            lastLSMData = struct.unpack("6f", data)
        except struct.error:
            print("Got bad data of length" + str(len(data)))
    elif data_type[0] == 255: # BNO
        data = ser.read(12)
        try:
            lastBNOData = struct.unpack("3f", data)
        except struct.error:
            print("Got bad data of length" + str(len(data)))
    else:
        continue

    # Use Accelerometer data to get pitch and roll
    ax, ay, az = lastLSMData[:3]
    roll = math.atan2(ay, az)
    pitch = - math.atan2(-ax, math.sqrt(ay * ay + az * az))

    # Get LSM magnetic data
    mag_vec = np.array(lastLSMData[3:]) - np.array([20.881348, 23.753662, 20.328979])
    lsm_mag_norm = mag_vec / np.linalg.norm(mag_vec)

    # Get BNO Magnetic data
    bno_mag_norm = np.array(lastBNOData)
    bno_mag_norm = bno_mag_norm / np.linalg.norm(bno_mag_norm)

    # Get vector angle, in degress
    diff = 180/math.pi * math.acos(np.dot(lsm_mag_norm, bno_mag_norm))

    acc_vec = np.array(lastLSMData[:3])
    acc_vec_norm = acc_vec / np.linalg.norm(acc_vec)
    pv = lsm_mag_norm - (np.dot(lsm_mag_norm, acc_vec_norm) * acc_vec_norm)

    mx, my, mz = mag_vec
    Xh = mx*math.cos(pitch) + my*math.sin(roll)*math.sin(pitch) - mz*math.cos(roll)*math.sin(pitch)
    Yh = my*math.cos(roll) + mz*math.sin(roll)
    azimuth = math.atan2(Yh, Xh) * 180/math.pi

    if Xh < 0:
        azimuth = 180 - math.atan(Yh/Xh) * 180/math.pi
    elif Xh > 0:
        if Yh < 0:
            azimuth = - math.atan(Yh/Xh) * 180/math.pi
        else:
            azimuth = 360 - math.atan(Yh/Xh) * 180/math.pi
    else:
        azimuth = 90 if Yh < 0 else 270

    roll *= 180/math.pi
    pitch *= 180/math.pi
    #print(math.atan2(pv[0], pv[1]) * 180/math.pi - roll)
    #print(f"Roll: {roll:.2f}\tPitch: {pitch:.2f}\t Azi: {azimuth:.2f}")
    servo = (roll + 80) / 100
    servo = min(servo, 1)
    servo = max(servo, -1)
    motor = abs(diff)
    motor = 0 if motor < 35 else motor
    
    #print(f"{roll}\t{diff}")
    print(f"Servo: {servo:.3f}\tMotor: {motor}")
