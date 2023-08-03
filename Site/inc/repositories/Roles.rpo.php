<?php

	namespace Zibings;

	use Stoic\Pdo\BaseDbQueryTypes;
	use Stoic\Pdo\StoicDbClass;

	/**
	 * Repository methods for dealing with roles.
	 *
	 * @package Zibings
	 */
	class Roles extends StoicDbClass {
		/**
		 * Internal Role instance.
		 *
		 * @var Role
		 */
		protected Role $rlObj;


		/**
		 * Initializes the internal Role instance.
		 *
		 * @return void
		 */
		protected function __initialize() : void {
			$this->rlObj = new Role($this->db, $this->log);

			return;
		}

		/**
		 * Returns all roles in database.
		 *
		 * @return Role[]
		 */
		public function getAll() : array {
			$ret = [];

			$this->tryPdoExcept(function () use (&$ret) {
				echo($this->rlObj->generateClassQuery(BaseDbQueryTypes::SELECT, false));
				$stmt = $this->db->query($this->rlObj->generateClassQuery(BaseDbQueryTypes::SELECT, false));

				while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
					$ret[] = Role::fromArray($row, $this->db, $this->log);
				}
			}, "Failed to retrieve all roles from database");

			return $ret;
		}
	}
