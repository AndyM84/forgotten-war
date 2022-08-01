use crate::fw;

use fw::model::character::Character;
use fw::core::connhandler::ConnHandler;

use std::collections::HashMap;

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
}