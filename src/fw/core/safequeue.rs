// Credit to https://www.reddit.com/r/rust/comments/u7fuze/creating_a_thread_safe_queue_in_rust/

use std::any::Any;
use std::collections::VecDeque;
use std::ops::Deref;
use std::sync::{Mutex, Condvar, Arc};

use self::size_limits::DEQUE_SIZE_LIMIT_BYTES;

mod size_limits {
	pub const KILOBYTE: usize = 1000;
	pub const MEGABYTE: usize = 1000 * KILOBYTE;
	pub const FILE_SIZE_LIMIT_BYTES: usize = 10 * MEGABYTE;
	pub const DEQUE_SIZE_LIMIT_BYTES: usize = 100 * MEGABYTE;
}

#[derive(Debug)]
pub struct SafeQueue<T: Any> {
	deque: Mutex<VecDeque<T>>,
	cv_empty: Condvar,
	cv_full: Condvar,
	t_size: usize
}

unsafe impl<T: Any> Sync for SafeQueue<T> {}
unsafe impl<T: Any> Send for SafeQueue<T> {}

impl<T: Any> SafeQueue<T> {
	pub fn new() -> Self {
		Self {
			deque: Mutex::new(VecDeque::new()),
			cv_empty: Condvar::new(),
			cv_full: Condvar::new(),
			t_size: std::mem::size_of::<T>()
		}
	}

	pub fn len(&self) -> usize {
		return self.deque.lock().unwrap().len();
	}

	pub fn push_back(&self, elem: T) {
		let mut guard = self.cv_full.wait_while(self.deque.lock().unwrap(), |deque| {
			deque.len() * self.t_size >= DEQUE_SIZE_LIMIT_BYTES
		}).unwrap();

		guard.push_back(elem);
		self.cv_empty.notify_one();
	}

	pub fn push_front(&self, elem: T) {
		let mut guard = self.cv_full.wait_while(self.deque.lock().unwrap(), |deque| {
			deque.len() * self.t_size >= DEQUE_SIZE_LIMIT_BYTES
		}).unwrap();

		guard.push_front(elem);
		self.cv_empty.notify_one();
	}

	pub fn pop_back(&self) -> T {
		let mut guard = self.cv_empty.wait_while(self.deque.lock().unwrap(), |deque| {
			deque.is_empty()
		}).unwrap();

		let popped_el = guard.pop_back().unwrap();
		self.cv_full.notify_one();

		return popped_el;
	}

	pub fn pop_front(&self) -> T {
		let mut guard = self.cv_empty.wait_while(self.deque.lock().unwrap(), |deque| {
			deque.is_empty()
		}).unwrap();

		let popped_el = guard.pop_front().unwrap();
		self.cv_full.notify_one();

		return popped_el;
	}
}