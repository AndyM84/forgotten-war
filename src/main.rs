mod fw;

use fw::model::character::{ Character };

fn change_name(curr_char: &mut Character) {
    curr_char.name = String::from("testing");
}

fn main() {
    println!("Hello, world!");

    let mut a_char = Character::new();
    a_char.vnum = 202;

    a_char.name = String::from("Xitan");
    println!("Your name is: {}", a_char.name);

    a_char.name = String::from("Tralen");
    println!("Your name is: {}", a_char.name);

    change_name(&mut a_char);
    println!("Your name is: {}", a_char.name);

    println!("{}", a_char);
}
