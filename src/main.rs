mod fw;

use fw::model::character::{Character, build_empty_character, build_character_from_vnum};

fn main() {
    println!("Hello, world!");
    let mut a_char = build_empty_character();
    a_char.Vnum = 202;

    a_char.Name = String::from("Xitan");
    println!("Your name is: {}", a_char.Name);

    a_char.Name = String::from("Tralen");
    println!("Your name is: {}", a_char.Name);

    println!("{}", a_char);
}
