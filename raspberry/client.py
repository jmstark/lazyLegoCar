import socket


ip = '10.0.0.1'
port = 4359
MSGLEN = 64

'''
def __init__:
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect(
'''

class clSock:
	def __init__(self,sock=None):
		if sock is None:
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		else:
			self.sock = sock
			
	def connect(self, address, port):
		self.sock.connect((address,port))
		
	def send(self, string):
		#tLen = 0
		#msg = bytes(string, 'utf-8')
		self.sock.sendall(bytes(string,'utf-8'))
			
	def recv(self):
		msg = ''
		while len(msg) < MSGLEN:
			chunk = self.sock.recv(MSGLEN-len(msg))
			msg = msg + chunk
		return msg.decode('utf-8')
		
	def close(self):
		self.sock.close()
		
		
sock = clSock()
sock.connect('in.tum.de',80)
sock.send('GET / HTTP/1.1\r\nHost: www.in.tum.de\r\n')
print(sock.recv())
sock.close()