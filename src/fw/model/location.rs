use vector3d::Vector3d;

pub struct Location {
	pub coordinate: Vector3d<f32>,
	pub vnum: u32
}

impl Location {
	pub fn empty() -> Location {
		let vec = Vector3d::new(0.0, 0.0, 0.0);

		Location {
			coordinate: vec,
			vnum: 0
		}
	}

	pub fn new(coordinate: Vector3d<f32>, vnum: u32) -> Location {
		Location {
			coordinate,
			vnum
		}
	}
}