#[derive(Debug, PartialEq, Eq)]
pub enum SockMsgStates {
	Active,
	Disconnect
}

pub struct SockMsg {
	pub fd: u32,
	pub msg: String,
	pub state: SockMsgStates
}