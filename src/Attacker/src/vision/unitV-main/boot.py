from fpioa_manager import fm
from machine import UART
import sensor
import image
import lcd
import time
import ustruct

# UART
fm.register(34, fm.fpioa.UART2_TX, force=True)
fm.register(35, fm.fpioa.UART2_RX, force=True)
uart = UART(UART.UART2, 115200, 8, 0, 1, timeout=1000, read_buf_len=4096)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_auto_whitebal(False, rgb_gain_db=(90, 55, 93))
sensor.set_auto_gain(False, 10)
sensor.set_auto_exposure(False, 350)
sensor.set_brightness(0)
sensor.set_contrast(1)
sensor.set_saturation(0)
sensor.skip_frames(time=2000)

lcd.init()
sensor.run(1)

threshold = (42, 100, 16, 127, 3, 127)

START_BYTE = 0xAA
END_BYTE = 0x55

while True:
    best_blob = None
    img = sensor.snapshot()
    blobs = img.find_blobs([threshold], pixels_threshold=10, area_threshold=50, merge=True)

    if blobs:
        blob = max(blobs, key=lambda b: b.pixels())
        best_blob = blob

    if best_blob:
        x = best_blob.cx() - 160
        y = best_blob.cy()

        print("X:", x)
        print("Y:", y)

        img.draw_cross(best_blob.cx(), best_blob.cy())
        img.draw_rectangle(best_blob.rect())
    else:
        x = 500
        y = 500

    # Empaquetar x,y en 4 bytes
    payload = ustruct.pack("<hh", x, y)

    # Checksum simple: suma de payload mod 256
    checksum = 0
    for b in payload:
        checksum = (checksum + b) & 0xFF

    # Trama completa:
    # [START][x low][x high][y low][y high][CHECKSUM][END]
    packet = bytes([START_BYTE]) + payload + bytes([checksum, END_BYTE])

    uart.write(packet)


