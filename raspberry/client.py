import socket

ip = '10.0.0.1'
port = 4359

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((ip, port))
str = ''
print ('connected to raspberry')
while True:
	str = input ('enter some string: ' )
	if str == 'end':
		break
	s.send(bytes(str,'utf-8'))
s.close()