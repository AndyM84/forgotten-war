#[derive(Debug, PartialEq, Eq)]
pub enum Alivenesses {
    Alive,
    Dead
}

#[derive(Debug, PartialEq, Eq)]
pub enum Biomes {
    Glacier,
    Tundra,
    Taiga,
    TemperateRainforest,
    TemperateDeciduousForest,
    Wetlands,
    Grasslands,
    TropicalRainforest,
    TropicalSeasonalForest,
    Savannah,
    HotDesert,
    ColdDesert,
    Underground,
    Interior,
    Urban,
    Air,
    DeepOcean,
    ShallowOcean,
    SaltwaterRiver,
    SaltwaterLake,
    FreshwaterRiver,
    FreshwaterLake
}

#[derive(Debug, PartialEq, Eq)]
pub enum Citizenships {
    None
}

#[derive(Debug, PartialEq, Eq)]
pub enum Classes {
    None,
    Cleric,
    Shaman,
    Merchant,
    Rogue,
    Ranger,
    Monk,
    Mage,
    Paladin,
    Barbarian,
    Artificer,
    Sentinel,
    Mercenary
}

#[derive(Debug, PartialEq, Eq)]
pub enum ConnectionStates {
    Disconnected,
    NamePrompt,
    PasswordPrompt,
    ColorPrompt,
    Connected
}

#[derive(Debug, PartialEq, Eq)]
pub enum Directions {
    North,
    South,
    East,
    West,
    Up,
    Down
}

#[derive(Debug, PartialEq, Eq)]
pub enum Mortalities {
    Mortal,
    Immortal,
    Admin
}

#[derive(Debug, PartialEq, Eq)]
pub enum Poses {
    Standing,
    Sitting,
    Laying,
    Crouching,
    Turtling,
    Sleeping
}

#[derive(Debug, PartialEq, Eq)]
pub enum Races {
    Fae,
    Satyr,
    Dwarf,
    Teganu,
    Elf,
    Gnoll,
    Gnome,
    Harpy,
    Orc,
    Giant,
    Kopal,
    Dryad,
    Ebban
}

#[derive(Debug, PartialEq, Eq)]
pub enum Terrains {
    Air,
    Beach,
    Cliff,
    DeepWater,
    Delta,
    Desert,
    Dune,
    DryLake,
    Glacier,
    Gorge,
    Hill,
    Interior,
    Mountain,
    Plain,
    Plateau,
    Rocks,
    ShallowWater,
    Shore,
    Summit,
    Urban,
    Valley
}
