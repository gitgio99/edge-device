import serial

DLE = 0x10
STX = 0x02
ETX = 0x03

CMD_LCD_ON  = 0x01
CMD_LCD_OFF = 0x02

MY_ID = 2
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

# 예제: /dev/ttyUSB0 사용 시
ser = serial.Serial('/dev/ttyAMA0', 115200, timeout=1)
# ser.write(encode_frame(CMD_LCD_ON))  # LD2 ON 명령 전송
ser.write(encode_frame(CMD_LCD_OFF))  # LD2 OFF 명령 전송
