<?php

	namespace Zibings;

	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbClass;

	/**
	 * Repository methods for dealing with UserToken data.
	 *
	 * @package Zibings
	 */
	class UserTokens extends StoicDbClass {
		const SQL_DELFORUSER = 'usertokens-deleteforuser';


		/**
		 * Internal UserToken instance.
		 *
		 * @var UserToken
		 */
		protected UserToken $utObj;


		/**
		 * Whether the stored queries have been initialized.
		 *
		 * @var bool
		 */
		private static bool $dbInitialized = false;


		/**
		 * Initializes the internal UserToken instance.
		 *
		 * @return void
		 */
		protected function __initialize() : void {
			$this->utObj = new UserToken($this->db, $this->log);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_DELFORUSER, "DELETE FROM {$this->utObj->getDbTableName()} WHERE [UserID] = :userId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_DELFORUSER, "DELETE FROM {$this->utObj->getDbTableName()} WHERE `UserID` = :userId");

				static::$dbInitialized = true;
			}

			return;
		}

		/**
		 * Removes all tokens for the given user.
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
