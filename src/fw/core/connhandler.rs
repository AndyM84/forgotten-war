use std::net::{TcpStream, Shutdown};
use std::thread::JoinHandle;

pub struct ConnHandler {
	pub char_vnum: u32,
	pub recv_thread: JoinHandle<()>,
	pub send_thread: JoinHandle<()>,
	pub stream: TcpStream
}

impl ConnHandler {
	pub fn new(char_vnum: u32, recv_thread: JoinHandle<()>, send_thread: JoinHandle<()>, stream: TcpStream) -> Self {
		Self {
			char_vnum,
			recv_thread,
			send_thread,
			stream
		}
	}

	pub fn shutdown(&self) {
		self.stream.shutdown(Shutdown::Both).unwrap();
	}
}
