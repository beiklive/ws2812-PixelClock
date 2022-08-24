"""
工具函数
"""

LED_H = 7
LED_W = 31

# 坐标转换
def pos(x, y):
    if x % 2 == 0:
        _pos = (LED_W - x) * (LED_H + 1) + y
    else:
        _pos = (LED_W - x) * (LED_H + 1) + LED_H - y
    if _pos > 255 or _pos < 0:
        return -1
    return _pos 

