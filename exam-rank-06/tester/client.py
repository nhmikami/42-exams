import socket
import time

class Client:
	def __init__(self, host="127.0.0.1", port=8080):
		self.host = host
		self.port = port
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	def connect(self):
		self.sock.connect((self.host, self.port))

	def send(self, msg, delay=0):
		time.sleep(delay)
		print(f"Sending: {msg}")
		self.sock.send(msg)

	def close(self):
		self.sock.close()

if __name__ == "__main__":
	client = Client("127.0.0.1", 8080)
	client.connect()

	client.send(b"oi! ")
	client.send(b"tudo bem?\n", delay=2)

	time.sleep(5)

	client.send(b":")
	client.send(b")", delay=1)
	client.send(b"\n", delay=1)

	time.sleep(5)

	client.send(b"oi\nola\nbom dia\n")

	time.sleep(5)
	
	client.send(b"hello ")
	time.sleep(10)
	client.send(b"world\n")

	client.close()