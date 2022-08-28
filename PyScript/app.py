#! /usr/bin/python3
# -*- coding: utf-8 -*
import logging
import time
from rpi_ws281x import PixelStrip, Color
import threading
import json
import sys
import utils
from utils import LED_H, LED_W

LOG_FORMAT = "%(asctime)s - %(levelname)s - %(message)s"
logging.basicConfig(format=LOG_FORMAT, level=logging.DEBUG)
 
LED_COUNT = 256        # LED灯的个数
LED_PIN = 18          # DI端接GPIO18
 
# 以下可以不用改
LED_FREQ_HZ = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA = 10          # DMA channel to use for generating signal (try 10)
LED_BRIGHTNESS = 1  # Set to 0 for darkest and 255 for brightest
LED_INVERT = False    # True to invert the signal (when using NPN transistor level shift)
LED_CHANNEL = 0       # set to '1' for GPIOs 13, 19, 41, 45 or 53

def update(strip):
    strip.show()

'''
    子线程

    整体亮度设置
'''
class LED_Light:
    # 设置初始状态
    def __init__(self, strip):
        self.strip = strip
        self.brightness = 1
        self.is_running = False
        self.thread = None
        self.IsChanged = False
        self.brightnessT = 0.01
        self.__start()

    # 开启任务线程
    def __start(self):
        self.is_running = True
        self.thread = threading.Thread(target=self.run)
        self.thread.start()
        logging.debug("bright thread start")

    # 结束任务并回收线程
    def stop(self):
        self.is_running = False
        self.thread.join()
        logging.debug("bright thread joined")

    # 默认任务函数
    def run(self):
        while self.is_running:
            if self.IsChanged:
                self.strip.setBrightness(self.brightness)
                self.IsChanged = False
            time.sleep(self.brightnessT)

    def set_ChangeFreq(self, T):
        self.brightnessT = T

    def set_brightness(self, brightness):
        self.brightness = brightness
        self.IsChanged = True

    def get_brightness(self):
        return self.brightness

class LED_Obj:
    def __init__(self, strip, library):
        # 读取本地json文件
        logging.debug("read json file "+ library)
        with open(library, 'r') as f:
            self.config = json.load(f)
        self.strip = strip
        self.w = self.config['w']
        self.h = self.config['h']
        self.datas = self.config['data']
        self.data = self.datas
        self.x = 0
        self.y = 0
        self.color = Color(205, 196, 208)

    def set(self, x, y):
        self.x = x
        self.y = y
        for i in range(self.w):
            for j in range(self.h):
                if self.data[j*self.w+i] == 1:
                    self.strip.setPixelColor(utils.pos(x+i, y+j), self.color)

    def set_color(self, color):
        self.color = color

    def clear(self):
        for i in range(self.w):
            for j in range(self.h):
                if self.data[j*self.w+i] == 1:
                    self.strip.setPixelColor(utils.pos(self.x+i, self.y+j), Color(0, 0, 0))
        self.strip.show()

class LED_NUM(LED_Obj):
    def __init__(self, strip, library):
        super().__init__( strip, library)
        # self.curNum = 0
        

    def __getPos(self):
        return self.x, self.y
    
    def getNum(self):
        return self.curNum

    def __setNum(self, num):
        self.curNum = num
        self.data = self.datas[self.curNum]
        # print(self.data)
        
    def setPos(self, x, y):
        self.set(x, y)
    
    def setNum(self, num):
        self.__setNum(int(num))

def SetBackground(strip, color):
    for i in range(256):
        strip.setPixelColor(i, color)

def ClearALL(strip):
    for i in range(256):
        strip.setPixelColor(i, Color(0, 0, 0))

def colorWipe(strip, color, wait_ms=20):
    """一次擦除显示像素的颜色."""
    for i in range(strip.numPixels()):
        strip.setPixelColor(i, color)

    strip.show()
        # time.sleep(0.01)

def Clock(strip):
    Num1 = LED_NUM(strip, 'num.json')
    Num2 = LED_NUM(strip, 'num.json')
    Num3 = LED_NUM(strip, 'num.json')
    Num4 = LED_NUM(strip, 'num.json')
    Num5 = LED_NUM(strip, 'num.json')
    Num6 = LED_NUM(strip, 'num.json')
     
    obj = LED_Obj(strip, 'utils.json')
    obj.set_color(Color(0, 0, 208))
    i = 0
    posx = 3
    posy = 1
    posa = 4
    while True:
        ClearALL(strip)
        # SetBackground(strip, Color(205, 196, 208))
        utc_timestamp = int(time.time())
        days = (utc_timestamp + 3600 * 8) % 86400
        hours = int(days / 3600)
        minutes = int(days % 3600 / 60)
        seconds = days % 60

        Num1.setNum(hours / 10)
        Num2.setNum(hours % 10)
        Num3.setNum(minutes / 10)
        Num4.setNum(minutes % 10)
        Num5.setNum(seconds / 10)
        Num6.setNum(seconds % 10)


        Num1.setPos(posx, posy)
        Num2.setPos(posx + posa, posy)
        Num3.setPos(posx + posa*2 + 1, posy)
        Num4.setPos(posx + posa*3 + 1, posy)
        Num5.setPos(posx + posa*4 + 2, posy)
        Num6.setPos(posx + posa*5 + 2, posy)

        obj.set(i % LED_W, 7)
        i += 1
        update(strip)
        time.sleep(0.1)




# Main program logic follows:
if __name__ == '__main__':
    strip = PixelStrip(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS, LED_CHANNEL)
    strip.begin()
 
    print('Press Ctrl-C to quit.')
    mybright = LED_Light(strip)
    try:
        Clock(strip)
  
 
    except:
        mybright.stop()
        colorWipe(strip, Color(0, 0, 0), 100)