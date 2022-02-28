use vector3d::Vector3d;

pub struct Location {
	pub Coordinate: Vector3d<f32>,
	pub Vnum: u32
}

impl Location {
	pub fn empty() -> Location {
		let mut vec = Vector3d::new(0.0, 0.0, 0.0);

		Location {
			Coordinate: vec,
			Vnum: 0
		}
	}

	pub fn new(coordinate: Vector3d<f32>, vnum: u32) -> Location {
		Location {
			Coordinate: coordinate,
			Vnum: vnum
		}
	}
}