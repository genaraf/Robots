import network
import webrepl
import utime
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect('upsaHot', '1234567890')
while not wlan.isconnected():
    utime.sleep_ms(50)
wlan.ifconfig()
webrepl.start()

