import math, struct, asyncio
import numpy as np
from bleak import BleakScanner, BleakClient

def process_data(session_data):

    # Use Accelerometer data to get pitch and roll
    ax, ay, az = session_data["lastLSMData"][:3]
    roll = math.atan2(ay, az) * 180/math.pi
    pitch = math.atan2(-ax, math.sqrt(ay * ay + az * az)) * 180/math.pi

    # Get LSM magnetic data
    mag_vec = np.array(session_data["lastLSMData"][3:]) - np.array([20.881348, 23.753662, 20.328979])
    lsm_mag_norm = mag_vec / np.linalg.norm(mag_vec)

    # Get BNO Magnetic data
    bno_mag_norm = np.array(session_data["lastBNOData"])
    bno_mag_norm = bno_mag_norm / np.linalg.norm(bno_mag_norm)

    # Get vector angle, in degress
    diff = 180/math.pi * math.acos(np.dot(lsm_mag_norm, bno_mag_norm))

    servo = (roll + 80) / 100
    servo = min(servo, 1)
    servo = max(servo, -1)
    motor = abs(diff)
    motor = 0 if motor < 50 else motor

    #print(session_data["lastBNOData"])
    print(f"Servo: {servo}\tMotor: {motor}")

def notify_callback(handle, data, session_data):

    if handle == 11: # LSM

        res = struct.unpack("6f", data)
        session_data["lastLSMData"] = res
    
    elif handle == 14: # BNO
        
        res = struct.unpack("3f", data)
        session_data["lastBNOData"] = res

    else:
        print("Caught extra data from handle", handle)

    process_data(session_data)


async def main():
    devices = await BleakScanner.discover()

    glove_address = None
    for d in devices:
        if d.name == "Carlab Glove":
            glove_address = d.address

    if not glove_address:
        print("Couldn't find glove")
        return

    session_data = {
        "lastLSMData" : (0, 0, 0, 0, 0, 0),
        "lastBNOData" : (0, 0, 0),
    }

    async with BleakClient(glove_address) as client:
        services = await client.get_services()
        for characteristic in services.characteristics.values():
            await client.start_notify(characteristic, lambda x, y: notify_callback(x, y, session_data))
        
        while client.is_connected:
            await asyncio.sleep(1)
    

asyncio.run(main())