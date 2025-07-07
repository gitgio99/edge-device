import serial
import time

DLE = 0x10
STX = 0x02
ETX = 0x03
ACK = 0xAA
NACK = 0x55

CMD_LCD_ON  = 0x01
CMD_LCD_OFF = 0x02

MY_ID = 4  #1 <-> AMA0, 2 <-> AMA2 , 3<-> AMA1 , 4<-> AMA3
DST_MASK = (1 << (MY_ID - 1))  # 0x01

def reverse(val, bits):
    res = 0
    for i in range(bits):
        res = (res << 1) | ((val >> i) & 1)
    return res

def crc16(data):
    crc = 0x0000
    for b in data:
        crc ^= reverse(b, 8) << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ 0x8005
            else:
                crc <<= 1
    return reverse(crc, 16) & 0xFFFF

def encode_frame(cmd):
    payload = [DST_MASK, cmd]
    crc = crc16(payload)
    payload += [(crc >> 8) & 0xFF, crc & 0xFF]

    frame = [DLE, STX]
    for b in payload:
        if b == DLE:
            frame += [DLE, DLE]
        else:
            frame.append(b)
    frame += [DLE, ETX]
    return bytes(frame)

def wait_for_ack(ser, timeout=1.0):
    start = time.time()
    while time.time() - start < timeout:
        if ser.in_waiting:
            resp = ser.read(1)
            if resp:
                if resp[0] == ACK:
                    print("ACK received")
                    return True
                elif resp[0] == NACK:
                    print("NACK received")
                    return False
    print("Timeout waiting for ACK/NACK")
    return False

def send_cmd_with_ack(ser, cmd, retries=3):
    frame = encode_frame(cmd)
    for attempt in range(retries):
        print(f"Sending command (attempt {attempt+1})...")
        ser.write(frame)
        if wait_for_ack(ser):
            print("Command acknowledged by device.")
            return True
        else:
            print("No ACK or NACK received, retrying...")
            time.sleep(0.1)
    print("Failed to get ACK after retries!")
    return False

if __name__ == "__main__":
    ser = serial.Serial('/dev/ttyAMA3', 115200, timeout=0.1)
    send_cmd_with_ack(ser, CMD_LCD_ON)
    # send_cmd_with_ack(ser, CMD_LCD_OFF)