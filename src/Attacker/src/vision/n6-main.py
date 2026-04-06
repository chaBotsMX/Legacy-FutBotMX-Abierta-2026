import sensor
import time
from pyb import UART

thresholds = [
    (3, 100, -15, 112, 12, 127),
]

goalTresholds = [
    (0, 0, 0, 0, 0, 0),
]

START_BYTE_1 = 0xAA
START_BYTE_2 = 0x55

uart = UART(4, 115200)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_auto_gain(False,gain_db=30)
sensor.set_auto_exposure(False, exposure_us=2000)
sensor.skip_frames(time=2000)

clock = time.clock()

ballX = 2000
ballY = 2000
goalX = 2000
goalY = 2000
goalColor = 0

packet = bytearray(12)

while True:
    clock.tick()
    img = sensor.snapshot()

    blobs = img.find_blobs(thresholds, pixels_threshold=100, area_threshold=100, merge=True)
    goals = img.find_blobs(goalTresholds, pixels_threshold=100, area_threshold=100, merge=True)

    if blobs:
        largest_blob = max(blobs, key=lambda b: b.pixels())
        ballX = largest_blob.cx()
        ballY = largest_blob.cy()
    else:
        ballX = 2000
        ballY = 2000

    if goals:
        largest_goal = max(goals, key=lambda b: b.pixels())
        goalX = largest_goal.cx()
        goalY = largest_goal.cy()
        goalColor = largest_goal.code()
    else:
        goalX = 2000
        goalY = 2000
        goalColor = 0

    packet[0] = START_BYTE_1
    packet[1] = START_BYTE_2

    packet[2] = (ballX >> 8) & 0xFF
    packet[3] = ballX & 0xFF

    packet[4] = (ballY >> 8) & 0xFF
    packet[5] = ballY & 0xFF

    packet[6] = (goalX >> 8) & 0xFF
    packet[7] = goalX & 0xFF

    packet[8] = (goalY >> 8) & 0xFF
    packet[9] = goalY & 0xFF

    packet[10] = goalColor & 0xFF

    checksum = 0
    for i in range(2, 11):
        checksum ^= packet[i]
    packet[11] = checksum

    uart.write(packet)

    print(clock.fps())
