import pygame
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *

import math
import struct
import serial
import numpy as np

ser = serial.Serial('/dev/cu.usbmodem112401')
ser.flushInput()


def get_vector():
    data_type = ser.read()
    if data_type[0] == 127: # LSM
        data = ser.read(24)
        try:
            lastLSMData = struct.unpack("6f", data)
        except struct.error:
            print("Got bad data of length" + str(len(data)))
    else:
        return get_vector()

    # Use Accelerometer data to get pitch and roll
    ax, ay, az = lastLSMData[:3]
    roll = math.atan2(ay, az)
    pitch = - math.atan2(-ax, math.sqrt(ay * ay + az * az))

    # Get LSM magnetic data
    mag_vec = np.array(lastLSMData[3:]) - np.array([20.881348, 23.753662, 20.328979])
    lsm_mag_norm = mag_vec / np.linalg.norm(mag_vec)

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

    acc_vec = np.array(lastLSMData[:3])
    acc_vec_norm = acc_vec / np.linalg.norm(acc_vec)

    pv = lsm_mag_norm - (np.dot(lsm_mag_norm, acc_vec_norm) * acc_vec_norm)
    return acc_vec_norm, lsm_mag_norm, pv

def vector(v, c):
    glBegin(GL_LINES)
    glColor3fv(c)
    glVertex3fv((0, 0, 0))
    glVertex3fv(v)
    glEnd()


def main():
    pygame.init()
    display = (800,600)
    pygame.display.set_mode(display, DOUBLEBUF|OPENGL)

    gluPerspective(45, (display[0]/display[1]), 0.1, 50.0)

    glTranslatef(0.0,0.0, -5)

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        av, mv, pv = get_vector()
        av = (av[0], -av[2], av[1])
        mv = (mv[0], -mv[2], mv[1])
        pv = (pv[0], -pv[2], pv[1])
        vector(av, (255, 255, 255))
        vector(mv, (255, 0, 0))
        vector(pv, (0, 0, 255))
        pygame.display.flip()
        pygame.time.wait(10)


main()