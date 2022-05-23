#[derive(Debug, PartialEq, Eq)]
pub enum MudMsgTypes {
	Connect,
	Disconnect,
	Command
}

pub struct MudMsg {
	pub msg_type: MudMsgTypes,
	pub msg_owner: u32,
	pub msg_contents: String
}
