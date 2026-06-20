import sensor
import time
from pyb import UART
from pyb import LED
goal_thresholds = [
    (26, 80, -31, 28, 19, 127)
]

START_BYTE_HIGH = 0xAA
START_BYTE_LOW = 0x55

uart = UART(1, 115200)

led = LED(1);
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)

sensor.set_auto_gain(False, gain_db=23.80)
sensor.set_auto_exposure(False, exposure_us=21450)
sensor.set_auto_whitebal(False, rgb_gain_db=(63.20937, 60.206, 62.60668))

sensor.set_brightness(0)
sensor.set_saturation(0)
sensor.set_contrast(0)

sensor.ioctl(sensor.IOCTL_SET_NIGHT_MODE, False)
sensor.skip_frames(time=2000)

clock = time.clock()

ballX = 500
ballY = 500
goalX = 500
goalY = 500
goalColor = 0

packet = bytearray(12)

COURT_X_MIN = 0
COURT_X_MAX = 320
COURT_Y_MIN = 10

COURT_CENTER_X = 160
COURT_HALF_WIDTH = 145

COURT_BOTTOM_CENTER = 240
COURT_BOTTOM_SIDE = 200

while True:
    clock.tick()
    img = sensor.snapshot()

    goals = img.find_blobs(
        goal_thresholds,
        pixels_threshold=15,
        area_threshold=15,
        merge=True
    )

    if goals:
        largest_goal = max(goals, key=lambda g: g.pixels())

        goalX = largest_goal.cx()
        goalY = largest_goal.cy()
        goalColor = 1

        img.draw_rectangle(largest_goal.rect(), color=(0, 255, 0), thickness=2)
        img.draw_cross(goalX, goalY, color=(0, 255, 0))
        img.draw_string(5, 5, "GOAL", color=(255, 255, 0), scale=1)

    else:
        goalX = 500
        goalY = 500
        goalColor = 0

    ballX = 500
    ballY = 500

    packet[0] = START_BYTE_HIGH
    packet[1] = START_BYTE_LOW

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
    led.on();
