use crate::{enums, fw};

use fw::model::character::Character;
use fw::core::connhandler::ConnHandler;
use fw::core::mudmsg::{MudMsg, MudMsgTypes};

use std::collections::HashMap;
use crate::fw::model::location;

pub struct FW {
	pub char_index: u32,
	pub conn_index: u32,
	pub char_conn: HashMap<u32, u32>,
	pub conn_char: HashMap<u32, u32>,
	pub chars: HashMap<u32, Character>,
	pub conns: HashMap<u32, ConnHandler>
}

impl FW {
	pub fn new() -> Self {
		Self {
			char_index: 0,
			conn_index: 0,
			char_conn: HashMap::new(),
			conn_char: HashMap::new(),
			chars: HashMap::new(),
			conns: HashMap::new()
		}
	}

	fn handle_new_user(&self, ch: &mut Character) -> (Vec<MudMsg>, Vec<u32>) {
		let mut messages: Vec<MudMsg> = Vec::new();
		let mut disconnects: Vec<u32> = Vec::new();

		if ch.connection_state == enums::ConnectionStates::Connected {
			ch.connection_state = enums::ConnectionStates::NamePrompt;
			ch.mortality = enums::Mortalities::Mortal;
			ch.name = format!("NewUser{}", ch.vnum);
			ch.prompt = String::from("`n< %stime% >`n");
			ch.show_color = true;

			messages.push(MudMsg{
				msg_type: MudMsgTypes::Command,
				msg_owner: ch.vnum.clone(),
				msg_contents: String::from("Welcome to Forgotten War!`n"),
				msg_as_socket: true
			});
			messages.push(MudMsg{
				msg_type: MudMsgTypes::Command,
				msg_owner: ch.vnum.clone(),
				msg_contents: String::from("Welcome to Forgotten War!`n"),
				msg_as_socket: true
			});
		}

		(messages, disconnects)
	}

	pub fn process_tick(&self) -> (Vec<MudMsg>, Vec<u32>) {
		let mut messages: Vec<MudMsg> = Vec::new();
		let mut disconnects: Vec<u32> = Vec::new();

		for (vnum, mut ch) in &self.chars {
			if ch.connection_state == enums::ConnectionStates::Disconnected {
				disconnects.push(ch.vnum.clone());

				continue;
			}

			if ch.connection_state != enums::ConnectionStates::LoggedIn {
				self.handle_new_user(&mut ch);

				continue;
			}

			while ch.chan_recv.len() > 0 {
				let msg = ch.chan_recv.pop_front();

				if !self.conns.contains_key(&ch.vnum) {
					disconnects.push(ch.vnum.clone());

					continue;
				}

				if msg.msg.len() == 0 {
					self.conns[&ch.vnum].shutdown();
					disconnects.push(ch.vnum.clone());
					println!("Connection #{} was disconnected", ch.vnum.clone());

					messages.push(MudMsg {
						msg_type: MudMsgTypes::Disconnect,
						msg_owner: (*vnum).clone(),
						msg_contents: format!("{} has disconnected", ch.vnum.clone()),
						msg_as_socket: false
					});

					continue;
				}

				println!("#{}: {}", msg.fd.clone(), msg.msg);
				messages.push(MudMsg {
					msg_type: MudMsgTypes::Command,
					msg_owner: (*vnum).clone(),
					msg_contents: format!("{} sent: {}", (*vnum).clone(), msg.msg),
					msg_as_socket: false
				});
			}
		}

		(messages, disconnects)
	}
}