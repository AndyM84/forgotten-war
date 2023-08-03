<?php

	namespace Zibings;

	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbClass;

	/**
	 * Repository methods related to the UserDevice table/data.
	 *
	 * @package Zibings
	 */
	class UserDevices extends StoicDbClass {
		const SQL_DELFORUSER = 'userdevices-deleteforuser';


		/**
		 * Internal instance of a UserDevice object, used for query generation.
		 *
		 * @var UserDevice
		 */
		protected UserDevice $udObj;


		/**
		 * Whether the stored queries have been initialized.
		 *
		 * @var bool
		 */
		private static bool $dbInitialized = false;


		/**
		 * Initializes the internal UserDevice object.
		 *
		 * @return void
		 */
		protected function __initialize() : void {
			$this->udObj = new UserDevice($this->db, $this->log);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_DELFORUSER, "DELETE FROM {$this->udObj->getDbTableName()} WHERE [UserID] = :userId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_DELFORUSER, "DELETE FROM {$this->udObj->getDbTableName()} WHERE `UserID` = :userId");

				static::$dbInitialized = true;
			}

			return;
		}

		/**
		 * Removes all devices for the given user.
		 *
		 * @param int $userId Integer identifier for user.
		 * @return void
		 */
		public function deleteAllForUser(int $userId) : void {
			if ($userId < 1) {
				return;
			}

			$this->tryPdoExcept(function () use ($userId) {
				$stmt = $this->db->prepareStored(self::SQL_DELFORUSER);
				$stmt->bindParam(':userId', $userId);
				$stmt->execute();
			}, "Failed to delete user's devices");

			return;
		}
	}
