import serial
import sys
with serial.Serial('/dev/tty.usbmodem613021', 9600, timeout=1) as ser:
  ser.write(sys.argv[1].encode('ascii'))
  if sys.argv[1] == '0' or sys.argv[1] == '8':
    ser.write((sys.argv[2]+'\n').encode('ascii'))
