use crate::fw::model::{ enums, location };

use chrono::{DateTime, Utc};
use std::collections::HashMap;
use std::fmt;
use std::sync::Arc;
use std::time::Duration;
use crate::{SafeQueue, SockMsg};

pub struct Character {
    // Attribute properties
    pub attributes: HashMap<String, u32>,
    pub class: enums::Classes,
    pub drunk: f32,
    pub fatigue: f32,
    pub luck: f32,
    pub mental: f32,
    pub pose: enums::Poses,
    pub race: enums::Races,
    // Connection properties
    pub connection_state: enums::ConnectionStates,
    pub created: DateTime::<Utc>,
    pub played_time: Duration,
    pub prompt: String,
    pub show_color: bool,
    pub socket_id: i32,
    // Ident properties
    pub aliveness: enums::Alivenesses,
    pub birthdate: DateTime::<Utc>,
    pub citizenship: enums::Citizenships,
    pub location: location::Location,
    pub mortality: enums::Mortalities,
    pub name: String,
    pub vnum: u32,
    pub cmd_queue: Vec<String>,
    pub chan_send: Arc<SafeQueue<SockMsg>>,
    pub chan_recv: Arc<SafeQueue<SockMsg>>,
}

impl fmt::Display for Character {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Char '{}' #{}", self.name, self.vnum)
    }
}

impl Character {
    pub fn new() -> Character {
        Character {
            attributes: HashMap::new(),
            class: enums::Classes::None,
            drunk: 0.0,
            fatigue: 0.0,
            luck: 0.0,
            mental: 0.0,
            pose: enums::Poses::Standing,
            race: enums::Races::Ebban,
            connection_state: enums::ConnectionStates::Disconnected,
            created: Utc::now(),
            played_time: Duration::new(0, 0),
            prompt: String::from(""),
            show_color: false,
            socket_id: 0,
            aliveness: enums::Alivenesses::Dead,
            birthdate: Utc::now(),
            citizenship: enums::Citizenships::None,
            location: location::Location::empty(),
            mortality: enums::Mortalities::Mortal,
            name: String::from(""),
            vnum: 0,
            cmd_queue: Vec::new(),
            chan_send: Arc::new(SafeQueue::<SockMsg>::new()),
            chan_recv: Arc::new(SafeQueue::<SockMsg>::new()),
        }
    }

    pub fn new_from_vnum(vnum: u32) -> Character {
        let mut tmp = Character::new();
        tmp.vnum = vnum;

        tmp
    }
}
