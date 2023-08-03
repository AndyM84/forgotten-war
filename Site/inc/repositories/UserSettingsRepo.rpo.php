<?php

	namespace Zibings;

	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbClass;

	/**
	 * Repository methods for dealing with UserSettings data.
	 *
	 * @package Zibings
	 */
	class UserSettingsRepo extends StoicDbClass {
		const SQL_DELFORUSER = 'usersettingsrepo-deleteforuser';


		/**
		 * Internal UserSettings instance.
		 *
		 * @var UserSettings
		 */
		protected UserSettings $usObj;


		/**
		 * Whether the stored queries have been initialized.
		 *
		 * @var bool
		 */
		private static bool $dbInitialized = false;


		/**
		 * Initializes the internal UserSettings instance.
		 *
		 * @return void
		 */
		protected function __initialize() : void {
			$this->usObj = new UserSettings($this->db, $this->log);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_DELFORUSER, "DELETE FROM {$this->usObj->getDbTableName()} WHERE [UserID] = :userId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_DELFORUSER, "DELETE FROM {$this->usObj->getDbTableName()} WHERE `UserID` = :userId");

				static::$dbInitialized = true;
			}

			return;
		}

		/**
		 * Removes all settings for the given user.
		 *
		 * @param int $userId Integer identifier for user in question.
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
			}, "Failed to delete user's contacts");

			return;
		}
	}
