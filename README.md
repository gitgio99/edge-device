# Serial Communication Protocol Specification

## 1. Purpose

본 프로토콜은 여러 보드 간 UART 시리얼 통신을 위한 데이터 프레임 포맷, 명령 구조, 오류 검출 방식(CRC16), DLE stuffing 방식 등을 정의합니다.

---

## 2. Serial Port Configuration

- **Baudrate:** 115200
- **Data bits:** 8
- **Parity:** None
- **Stop bits:** 1
- **Flow control:** None
- **Timeout:** 1 second (소프트웨어 구현에 따라 다를 수 있음)

---

## 3. Physical Mapping

| Board ID | UART Device      | Device Path     |
|----------|------------------|----------------|
| 1        | UART0            | /dev/ttyAMA0   |
| 2        | UART2            | /dev/ttyAMA2   |
| 3        | UART1            | /dev/ttyAMA1   |
| 4        | UART3            | /dev/ttyAMA3   |

---

## 4. Frame Structure

### 4.1. Frame Format

```
+-------+-------+---------+---------+---------+---------+-------+-------+
| DLE   | STX   | DST     | CMD     | CRC_H   | CRC_L   | DLE   | ETX   |
+-------+-------+---------+---------+---------+---------+-------+-------+
```

- **DLE**: 0x10 (Data Link Escape, 프레임 경계)
- **STX**: 0x02 (Start of Text, 프레임 시작)
- **DST**: 목적지 마스크 (bit mask, 아래 참조)
- **CMD**: 명령어(byte), 예: 0x01(LCD ON), 0x02(LCD OFF)
- **CRC_H/L**: CRC16 결과(상위/하위 바이트, 아래 참조)
- **DLE**: 0x10 (프레임 종료 전 DLE)
- **ETX**: 0x03 (End of Text, 프레임 종료)

### 4.2. DLE Stuffing

- **원칙:** 프레임 내 데이터 바이트(DST, CMD, CRC_H, CRC_L) 중 0x10(DLE) 값이 존재하면, **DLE를 한 번 더 추가**하여 두 번 연속(0x10 0x10)으로 송신합니다.
- **목적:** 데이터 내 DLE 바이트와 프레임 구분용 DLE를 혼동하지 않기 위함.

---

## 5. Field Details

### 5.1. DST (Destination Mask)

- **1 바이트(bit mask)**
- 자신의 보드 ID가 N이면: `DST_MASK = (1 << (N - 1))`
    - 예: Board 4 → 1 << (4-1) = 8 (0x08)
- 목적지 보드를 지정

### 5.2. CMD (Command)

- **1 바이트**
- 예시:
    - `0x01`: LCD ON
    - `0x02`: LCD OFF

### 5.3. CRC16

- **2 바이트 (상위, 하위)**
- **Polynomial:** 0x8005 (x^16 + x^15 + x^2 + 1)
- **초기값:** 0x0000
- **Bit Order:** 입력 및 결과 모두 비트 반전(reverse) 사용
- **계산 대상:** DST, CMD (payload만, DLE/STX/ETX 제외)
- **전송 순서:** CRC_H(상위), CRC_L(하위)

#### Pseudocode

```python
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
```

---

## 6. Example

### LCD ON 명령 (Board 4, /dev/ttyAMA3)

- **MY_ID:** 4
- **DST_MASK:** 0x08
- **CMD:** 0x01

#### 1. Payload: `[0x08, 0x01]`
#### 2. CRC16 계산: 예를 들어 0x1234 라고 가정 (실제 값은 코드로 계산)
#### 3. 프레임 (원본):  
    [0x10, 0x02, 0x08, 0x01, 0x12, 0x34, 0x10, 0x03]
- 데이터 구간에 0x10(DLE)이 있으면 [0x10, 0x10]으로 변환

---

## 7. 참고

- **프레임 송수신 시 DLE, STX, ETX 등은 프레임 구분용으로만 사용, CRC 계산/검증에는 포함하지 않음**
- **프레임 내 DLE stuffing 처리 필수**
- **CRC16 처리 시 비트 반전(reverse) 알고리즘 주의**

---

## 8. Typical Usage (Python)

```python
ser = serial.Serial('/dev/ttyAMA3', 115200, timeout=1)
ser.write(encode_frame(CMD_LCD_ON))
```
