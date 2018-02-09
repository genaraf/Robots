import machine
import utime

leftDir = machine.Pin(0, machine.Pin.OUT)
rightDir = machine.Pin(2, machine.Pin.OUT)
leftPWM = machine.PWM(machine.Pin(5), freq=1000)
rightPWM = machine.PWM(machine.Pin(4), freq=1000)

def motors(left, right):
  if left < 0:
    leftDir.high()
  else:
    leftDir.low()
  if right < 0:
    rightDir.high()
  else:
    rightDir.low()

  leftPWM.duty(abs(left))
  rightPWM.duty(abs(right))
    
  
def start():
  speedL = 600
  speedR = 600
  dirL = 1;
  dirR = 1;
  SPEED_INC = 50
  # waiting for a connection and then sends a simple message
  import socket
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.bind(('', 9001))
  s.listen(1)	# just queue up a single request
  while True:
	  conn, addr = s.accept()
	  print("Got a connection from %s" % str(addr))
	  conn.send(b'Hello, WiPy calling .... ')
	  cl_file = conn.makefile('rwb', 0)
	  while True:
	    line = cl_file.readline()
	    if line :
	      cmd = line[0]
	      print(cmd)
	      if cmd == 48:
		print("stop")
		dirL = dirR = 0
	      elif cmd == 49:
		print("forward")		  
		dirL = dirR = 1
	      elif cmd == 50:
		print("back")		  
		dirL = dirR = -1
	      elif cmd == 51:
		print("left")
		dirL = -1
		dirR = 1
	      elif cmd == 52:
		print("right")
		dirL = 1
		dirR = -1
	      elif cmd == 54:
		print("L-speenUp")
		speedL += SPEED_INC
		if speedL > 1000:
		  speedL = 1000
	      elif cmd == 55:
		print("L-slowDown")
		speedL -= SPEED_INC
		if speedL < 500:
		  speedL = 500
	      elif cmd == 56:
		print("R-speenUp")
		speedR += SPEED_INC
		if speedR > 1000:
		  speedR = 1000
	      elif cmd == 57:
		print("R-slowDown")
		speedR -= SPEED_INC
		if speedR < 500:
		  speedR = 500
	      motors(speedL * dirL, speedR * dirR)
	      conn.send("ok\r\n")
	    else:
	      break
	  conn.close()
	  motors(0, 0)
	  print("connection closed.")
  print('The End')

