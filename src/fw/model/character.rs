use crate::fw::model::{ enums, location };

use chrono::{DateTime, Utc};
use std::collections::HashMap;
use std::fmt;
use std::time::Duration;

pub struct Character {
    // Attribute properties
    pub Attributes: HashMap<String, u32>,
    pub Class: enums::Classes,
    pub Drunk: f32,
    pub Fatigue: f32,
    pub Luck: f32,
    pub Mental: f32,
    pub Pose: enums::Poses,
    pub Race: enums::Races,
    // Connection properties
    pub ConnectionState: enums::ConnectionStates,
    pub Created: DateTime::<Utc>,
    pub PlayedTime: Duration,
    pub Prompt: String,
    pub ShowColor: bool,
    pub SocketID: i32,
    // Ident properties
    pub Aliveness: enums::Alivenesses,
    pub Birthdate: DateTime::<Utc>,
    pub Citizenship: enums::Citizenships,
    pub Location: location::Location,
    pub Mortality: enums::Mortalities,
    pub Name: String,
    pub Vnum: u32
}

impl fmt::Display for Character {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "Char '{}' #{}", self.Name, self.Vnum)
    }
}

pub fn build_empty_character() -> Character {
    Character {
        Attributes: HashMap::new(),
        Class: enums::Classes::None,
        Drunk: 0.0,
        Fatigue: 0.0,
        Luck: 0.0,
        Mental: 0.0,
        Pose: enums::Poses::Standing,
        Race: enums::Races::Ebban,
        ConnectionState: enums::ConnectionStates::Disconnected,
        Created: Utc::now(),
        PlayedTime: Duration::new(0, 0),
        Prompt: String::from(""),
        ShowColor: false,
        SocketID: 0,
        Aliveness: enums::Alivenesses::Dead,
        Birthdate: Utc::now(),
        Citizenship: enums::Citizenships::None,
        Location: location::Location::empty(),
        Mortality: enums::Mortalities::Mortal,
        Name: String::from(""),
        Vnum: 0
    }
}

pub fn build_character_from_vnum(vnum: u32) -> Character {
    let mut tmp = build_empty_character();

    tmp
}
