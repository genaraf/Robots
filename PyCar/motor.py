import machine
leftDir = machine.Pin(0, machine.Pin.OUT)
rightDir = machine.Pin(2, machine.Pin.OUT)
leftPWM = machine.PWM(machine.Pin(5), freq=1000)
rightPWM = machine.PWM(machine.Pin(4), freq=1000)

leftDir.low()
rightDir.low()
leftPWM.duty(500)
leftPWM.duty(500)