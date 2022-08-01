use std::net::{TcpStream, Shutdown};
use std::thread::JoinHandle;

pub struct ConnHandler {
	pub char_socket: u32,
	pub recv_thread: JoinHandle<()>,
	pub send_thread: JoinHandle<()>,
	pub stream: TcpStream
}

pub struct DisconnectHandler {
	pub identifier: u32,
	pub is_socket: bool
}

impl ConnHandler {
	pub fn new(char_socket: u32, recv_thread: JoinHandle<()>, send_thread: JoinHandle<()>, stream: TcpStream) -> Self {
		Self {
			char_socket,
			recv_thread,
			send_thread,
			stream
		}
	}

	pub fn shutdown(&self) {
		self.stream.shutdown(Shutdown::Both).unwrap();
	}
}
