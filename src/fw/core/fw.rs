use crate::{enums, fw};

use fw::model::character::Character;
use fw::core::connhandler::{ConnHandler, DisconnectHandler};
use fw::core::mudmsg::{MudMsg, MudMsgTypes};

use std::collections::HashMap;
use crate::enums::Mortalities;

pub struct FW {
	pub conn_index: u32,
	pub char_idx: HashMap<u32, u32>,
	pub char_conn: HashMap<u32, u32>,
	pub conn_char: HashMap<u32, u32>,
	pub chars: Vec<Character>,
	pub conns: HashMap<u32, ConnHandler>,
	pub disconnects: Vec<DisconnectHandler>,
	pub messages: Vec<MudMsg>
}

impl FW {
	pub fn disconnect_user(&mut self, ch: Character, include_char: bool) -> Character {
		for _ch in &self.chars {
			if _ch.socket_id == ch.socket_id {
				if !include_char {
					continue;
				}

				self.messages.push(MudMsg {
					msg_type: MudMsgTypes::Disconnect,
					msg_owner: ch.socket_id.clone(),
					msg_contents: String::from("You have disconnected"),
					msg_as_socket: true
				});

				continue;
			}

			if _ch.mortality == Mortalities::Admin {
				self.messages.push(MudMsg {
					msg_type: MudMsgTypes::Disconnect,
					msg_owner: _ch.socket_id.clone(),
					msg_contents: format!("{} has disconnected", ch.socket_id.clone()),
					msg_as_socket: true
				});
			}
		}

		self.disconnects.push(DisconnectHandler{
			identifier: ch.socket_id.clone(),
			is_socket: true
		});

		ch
	}

	fn handle_new_user<'a>(&mut self, mut ch: &'a mut Character) -> &'a mut Character {
		// Update lookups (char_idx, char_conn, conn_char) when successfully identified

		if ch.connection_state == enums::ConnectionStates::Connected {
			ch.connection_state = enums::ConnectionStates::NamePrompt;
			ch.mortality = enums::Mortalities::Mortal;
			ch.name = format!("NewUser{}", ch.vnum);
			ch.prompt = String::from("`n< %stime% >`n");
			ch.show_color = true;

			self.send_to_user(ch.socket_id.clone(),
												String::from("Welcome to Forgotten War!`n"),
												true);
			self.send_to_user(ch.socket_id.clone(),
												String::from("What is your name?"),
												true);
		}

		ch
	}

	pub fn new() -> Self {
		Self {
			conn_index: 0,
			char_idx: HashMap::new(),
			char_conn: HashMap::new(),
			conn_char: HashMap::new(),
			chars: Vec::new(),
			conns: HashMap::new(),
			disconnects: Vec::new(),
			messages: Vec::new()
		}
	}

	fn parse_color(&self, mut message: String, allow_color: bool) -> String {
		let mut replacements: HashMap<&str, &str> = HashMap::new();
		replacements.insert("0", "[0m");
		replacements.insert("w", "[0;37m");
		replacements.insert("W", "[1;37m");
		replacements.insert("g", "[0;32m");
		replacements.insert("G", "[1;32m");
		replacements.insert("b", "[0;34m");
		replacements.insert("B", "[1;34m");
		replacements.insert("r", "[0;31m");
		replacements.insert("R", "[1;31m");
		replacements.insert("c", "[0;36m");
		replacements.insert("C", "[1;36m");
		replacements.insert("y", "[0;33m");
		replacements.insert("Y", "[1;33m");
		replacements.insert("m", "[0;35m");
		replacements.insert("M", "[1;35m");
		replacements.insert("k", "[0;30m");
		replacements.insert("K", "[1;30m");

		message = message.replace("``", "___||``||___");

		for (key, repl) in replacements {
			let computed: String = format!("`{}", key);

			if !allow_color {
				message = message.replace(&computed, "");

				continue;
			}

			let val = format!("\u{001b}{}", repl);
			message = message.replace(&computed, &*val);
		}

		message = message.replace("___||``||___", "``");
		message = message.replace("`n", "\n");

		message
	}

	fn parse_prompt(&self, mut message: String) -> String {
		message
	}

	pub fn send_to_user(&mut self, identifier: u32, mut message: String, as_socket: bool) {
		let mut socket_id = identifier.clone();

		if !as_socket && !self.char_idx.contains_key(&identifier) {
			return;
		}

		if !as_socket {
			let idx = usize::try_from(self.char_idx[&identifier]).unwrap();
			socket_id = self.chars[idx].socket_id.clone();

			message = self.parse_prompt(message);
			message = self.parse_color(message, self.chars[idx].show_color);
		} else {
			message = self.parse_color(message, true);
		}

		self.messages.push(MudMsg {
			msg_type: MudMsgTypes::Command,
			msg_owner: socket_id,
			msg_contents: message.clone(),
			msg_as_socket: true
		});
	}

	pub fn tick(mut self) {
		self.disconnects = Vec::new();
		self.messages = Vec::new();

		for mut ch in self.chars {
			if ch.connection_state == enums::ConnectionStates::Disconnected {
				ch = self.disconnect_user(ch.clone(), false);

				continue;
			}

			if ch.connection_state != enums::ConnectionStates::LoggedIn {
				ch = self.handle_new_user(&mut ch);

				continue;
			}

			while ch.chan_recv.len() > 0 {
				let msg = ch.chan_recv.pop_front();

				if !self.conns.contains_key(&ch.vnum) {
					ch = self.disconnect_user(&mut ch, false);

					continue;
				}

				if msg.msg.len() == 0 {
					self.conns[&ch.vnum].shutdown();
					ch = self.disconnect_user(&mut ch, false);
					println!("Connection #{} was disconnected", ch.vnum.clone());

					continue;
				}

				println!("#{}: {}", msg.fd.clone(), msg.msg);


			}
		}
	}
}