import socket
import sys

class Server:
	def __init__(self, host="127.0.0.1", port=9999):
		self.host = host
		self.port = port
		self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	def start(self):
		self.server_socket.bind((self.host, self.port))
		self.server_socket.listen(5)
		print(f"Listening on {self.host}:{self.port}...")

		conn, addr = self.server_socket.accept()
		print(f"Connected to lient at {addr}")

		while True:
			try:
				data = conn.recv(1024)
				if not data:
					print("Client disconnected")
					break
				print(f"Received: {data}")
			except:
				   sys.exit(0)
