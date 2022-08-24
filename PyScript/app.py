import time
from rpi_ws281x import PixelStrip, Color
import threading
import json
import sys
import utils
from utils import LED_H, LED_W
 
LED_COUNT = 256        # LED灯的个数
LED_PIN = 18          # DI端接GPIO18
 
# 以下可以不用改
LED_FREQ_HZ = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA = 10          # DMA channel to use for generating signal (try 10)
LED_BRIGHTNESS = 1  # Set to 0 for darkest and 255 for brightest
LED_INVERT = False    # True to invert the signal (when using NPN transistor level shift)
LED_CHANNEL = 0       # set to '1' for GPIOs 13, 19, 41, 45 or 53

class LED_Light:
    def __init__(self, strip):
        self.strip = strip
        self.brightness = 1
        self.is_running = False
        self.thread = None
        self.IsChanged = False

    def start(self):
        self.is_running = True
        self.thread = threading.Thread(target=self.run)
        self.thread.start()

    def stop(self):
        self.is_running = False
        self.thread.join()

    def run(self):
        while self.is_running:
            if self.IsChanged:
                self.strip.setBrightness(self.brightness)
                self.IsChanged = False
            time.sleep(0.01)

    def set_brightness(self, brightness):
        self.brightness = brightness
        self.IsChanged = True

    def get_brightness(self):
        return self.brightness

class LED_Obj:
    def __init__(self, strip, library):
        # 读取本地json文件
        print("read")
        with open(library, 'r') as f:
            print("read")
            self.config = json.load(f)
        self.strip = strip
        self.w = self.config['w']
        print(self.w)
        self.h = self.config['h']
        self.data = self.config['data']
        self.x = 0
        self.y = 0

    def set(self, x, y):
        self.x = x
        self.y = y
        for i in range(self.w):
            for j in range(self.h):
                if self.data[j*self.w+i] == 1:
                    print(i)
                    self.strip.setPixelColor(utils.pos(x+i, y+j), Color(0, 0, 255))

    def clear(self):
        for i in range(self.w):
            for j in range(self.h):
                if self.data[j*self.w+i] == 1:
                    self.strip.setPixelColor(utils.pos(self.x+i, self.y+j), Color(0, 0, 0))
        self.strip.show()


def colorWipe(strip, color, wait_ms=20):
    """一次擦除显示像素的颜色."""
    for i in range(strip.numPixels()):
        strip.setPixelColor(i, color)

        strip.show()
        time.sleep(wait_ms / 1000.0)


# Main program logic follows:
if __name__ == '__main__':
    strip = PixelStrip(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS, LED_CHANNEL)
    strip.begin()
 
    print('Press Ctrl-C to quit.')
    mybright = LED_Light(strip)
    try:
        while True:
            mybright.start()
            Obj = LED_Obj(strip, "font.json")
            for i in range(LED_W + 1):
                Obj.clear()
                Obj.set(i,0)
                mybright.set_brightness(int(i/LED_W*255))
                strip.show()
                time.sleep(0.05)

            for i in range(LED_W + 1, 0, -1):
                Obj.clear()
                Obj.set(i,0)
                mybright.set_brightness(int(i/LED_W*255))
                strip.show()
                time.sleep(0.05)
 
    except:
        mybright.stop()
        colorWipe(strip, Color(0, 0, 0), 100)