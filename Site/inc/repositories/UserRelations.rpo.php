<?php

	namespace Zibings;

	use Stoic\Pdo\BaseDbQueryTypes;
	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbClass;

	/**
	 * Repository methods related to UserRelation data.
	 *
	 * @package Zibings
	 */
	class UserRelations extends StoicDbClass {
		const SQL_DELALLFORUSR        = 'userrelations-deleteallforuser';
		const SQL_DELREL              = 'userrelations-deleterelation';
		const SQL_GUSRRELS            = 'userrelations-getuserrelations';
		const SQL_GRELSBYSTAGE        = 'userrelations-getrelationsbystage';
		const SQL_GRELS               = 'userrelations-getrelations';
		const SQL_GINCRELS            = 'userrelations-getincomingrelations';
		const SQL_GINCRELSBYSTAGE     = 'userrelations-getincomingrelationsbystage';
		const SQL_GINCRELSEXCEPTSTAGE = 'userrelations-getincomingrelationsexceptingstage';
		const SQL_GOUTRELS            = 'userrelations-getoutgoingrelations';
		const SQL_GOUTRELSBYSTAGE     = 'userrelations-getoutgoingrelationsbystage';
		const SQL_GOUTRELSEXCEPTSTAGE = 'userrelations-getoutgoingrelationsexceptingstage';


		/**
		 * Internal UserRelation object.
		 *
		 * @var UserRelation
		 */
		protected UserRelation $urObj;


		/**
		 * Whether the stored queries have been initialized.
		 *
		 * @var bool
		 */
		private static bool $dbInitialized = false;


		/**
		 * Initializes the internal UserRelation object.
		 *
		 * @return void
		 */
		protected function __initialize() : void {
			$this->urObj = new UserRelation($this->db, $this->log);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_DELREL, "DELETE FROM {$this->urObj->getDbTableName()} WHERE ([UserID_One] = :userOne AND [UserID_Two] = :userTwo) OR ([UserID_One] = :userTwo AND [UserID_Two] = :userOne)");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_DELREL, "DELETE FROM {$this->urObj->getDbTableName()} WHERE (`UserID_One` = :userOne AND `UserID_Two` = :userTwo) OR (`UserID_One` = :userTwo AND `UserID_Two` = :userOne)");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_DELALLFORUSR, "DELETE FROM {$this->urObj->getDbTableName()} WHERE [UserID_One] = :userId OR [UserID_Two] = :userId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_DELALLFORUSR, "DELETE FROM {$this->urObj->getDbTableName()} WHERE `UserID_One` = :userId OR `UserID_Two` = :userId");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GUSRRELS, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [UserID_One] = :userId OR [UserID_Two] = :userId ORDER BY [Created] ASC");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GUSRRELS, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `UserID_One` = :userId OR `UserID_Two` = :userId ORDER BY `Created` ASC");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GRELSBYSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE ([UserID_One] = :userId OR [UserID_Two] = :userId) AND [Stage] = :stage ORDER BY [Created] ASC");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GRELSBYSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE (`UserID_One` = :userId OR `UserID_Two` = :userId) AND `Stage` = :stage ORDER BY `Created` ASC");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GRELS, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE ([UserID_One] = :userOne AND [UserID_Two] = :userTwo) OR ([UserID_One] = :userTwo AND [UserID_Two] = :userOne) ORDER BY [Origin] DESC");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GRELS, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE (`UserID_One` = :userOne AND `UserID_Two` = :userTwo) OR (`UserID_One` = :userTwo AND `UserID_Two` = :userOne) ORDER BY `Origin` DESC");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GINCRELS, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [UserID_Two] = :userId ORDER BY [Created] ASC");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GINCRELS, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `UserID_Two` = :userId ORDER BY `Created` ASC");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GINCRELSBYSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [UserID_Two] = :userId AND [Stage] = :stage ORDER BY [Created] ASC");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GINCRELSBYSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `UserID_Two` = :userId AND `Stage` = :stage ORDER BY `Created` ASC");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GINCRELSEXCEPTSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [UserID_Two] = :userId AND [Stage] != :stage ORDER BY [Created] ASC");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GINCRELSEXCEPTSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `UserID_Two` = :userId AND `Stage` != :stage ORDER BY `Created` ASC");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GOUTRELS, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [UserID_One] = :userId ORDER BY [Created] ASC");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GOUTRELS, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `UserID_One` = :userId ORDER BY `Created` ASC");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GOUTRELSBYSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [UserID_One] = :userId AND [Stage] = :stage ORDER BY [Created] ASC");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GOUTRELSBYSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `UserID_One` = :userId AND `Stage` = :stage ORDER BY `Created` ASC");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GOUTRELSEXCEPTSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [UserID_One] = :userId AND [Stage] != :stage ORDER BY [Created] ASC");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GOUTRELSEXCEPTSTAGE, $this->urObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `UserID_One` = :userId AND `Stage` != :stage ORDER BY `Created` ASC");

				static::$dbInitialized = true;
			}

			return;
		}

		/**
		 * Returns whether the two users are related.
		 *
		 * @param int $userOne Integer identifier of first potential relation.
		 * @param int $userTwo Integer identifier of second potential relation.
		 * @return bool
		 */
		public function areRelated(int $userOne, int $userTwo) : bool {
			$rel = $this->getRelation($userOne, $userTwo);

			if (count($rel) == 2) {
				return true;
			}

			return false;
		}

		/**
		 * Changes the stage of a user relation. If $stage is set to 'INVITE' and users are not yet related, a new relation
		 * will be created.
		 *
		 * @param int $userOne Integer identifier of first potential relation.
		 * @param int $userTwo Integer identifier of second potential relation.
		 * @param int $stage Integer stage specifier.
		 * @throws \Exception
		 * @return bool
		 */
		public function changeStage(int $userOne, int $userTwo, int $stage) : bool {
			if ($stage == UserRelationStages::ERROR || !UserRelationStages::validValue($stage)) {
				return false;
			}

			$rel = $this->getRelation($userOne, $userTwo);

			if (count($rel) != 2 && $stage != UserRelationStages::INVITED) {
				return false;
			}

			if (count($rel) != 2 && $stage == UserRelationStages::INVITED) {
				$tmp = new UserRelation($this->db, $this->log);
				$tmp->userOne = $userOne;
				$tmp->userTwo = $userTwo;
				$tmp->origin = true;
				$tmp->stage = new UserRelationStages($stage);

				if ($tmp->create()->isBad()) {
					return false;
				}

				$tmp->userOne = $userTwo;
				$tmp->userTwo = $userOne;
				$tmp->origin = false;

				if ($tmp->create()->isBad()) {
					return false;
				}

				return true;
			}

			if (!$rel[0]->stage->is($rel[1]->stage->getValue())) {
				return false;
			}

			if ($rel[0]->stage->getValue() > UserRelationStages::INVITED && $stage == UserRelationStages::INVITED) {
				return false;
			}

			if ($rel[0]->stage->is(UserRelationStages::INVITED) && $stage > UserRelationStages::INVITED && $rel[0]->userOne == $userOne) {
				return false;
			}

			if ($rel[0]->stage->is($stage)) {
				return true;
			}

			$rel[0]->stage = new UserRelationStages($stage);
			$rel[1]->stage = new UserRelationStages($stage);

			if ($rel[0]->update()->isBad() || $rel[1]->update()->isBad()) {
				return false;
			}

			return true;
		}

		/**
		 * Removes the given relation.
		 *
		 * @param int $userOne Integer identifier of first potential relation.
		 * @param int $userTwo Integer identifier of second potential relation.
		 * @return bool
		 */
		public function deleteRelation(int $userOne, int $userTwo) : bool {
			if ($userOne < 1 || $userTwo < 1 || $userOne == $userTwo) {
				return false;
			}

			$this->tryPdoExcept(function () use ($userOne, $userTwo) {
				$stmt = $this->db->prepareStored(self::SQL_DELREL);
				$stmt->bindParam(':userOne', $userOne);
				$stmt->bindParam(':userTwo', $userTwo);
				$stmt->execute();
			}, "Failed to delete user's relation");

			return true;
		}

		/**
		 * Removes all relations for the given user.
		 *
		 * @param int $userId Integer identifier for user in question.
		 * @return void
		 */
		public function deleteAllForUser(int $userId) : void {
			if ($userId < 1) {
				return;
			}

			$this->tryPdoExcept(function () use ($userId) {
				$stmt = $this->db->prepareStored(self::SQL_DELALLFORUSR);
				$stmt->bindParam(':userId', $userId);
				$stmt->execute();
			}, "Failed to delete user's relations");

			return;
		}

		/**
		 * Returns a list of a user's relations.
		 *
		 * @param int $userId Integer identifier of user asking about their relations.
		 * @return UserRelation[]
		 */
		public function getRelations(int $userId) : array {
			$ret = [];

			$this->tryPdoExcept(function () use (&$ret, $userId) {
				$stmt = $this->db->prepareStored(self::SQL_GUSRRELS);
				$stmt->bindParam(':userId', $userId);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = UserRelation::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to get user relations");

			return $ret;
		}

		/**
		 * Returns a list of a user's relations in the requested stage.
		 *
		 * @param int $userId Integer identifier of user looking for relations.
		 * @param int $stage Integer stage specifier.
		 * @throws \Exception
		 * @return UserRelation[]
		 */
		public function getRelationsByStage(int $userId, int $stage) : array {
			$ret = [];

			if ($stage == UserRelationStages::ERROR || !UserRelationStages::validValue($stage)) {
				return $ret;
			}

			$this->tryPdoExcept(function () use (&$ret, $userId, $stage) {
				$stmt = $this->db->prepareStored(self::SQL_GRELSBYSTAGE);
				$stmt->bindParam(':userId', $userId);
				$stmt->bindParam(':stage', $stage);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = UserRelation::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to get user relation");

			return $ret;
		}

		/**
		 * Retrieves user relation, if available. Will always have array ordered with 'origin' user as first element.
		 *
		 * @param int $userOne Integer identifier of the first potential relation.
		 * @param int $userTwo Integer identifier of the second potential relation.
		 * @return UserRelation[]
		 */
		public function getRelation(int $userOne, int $userTwo) : array {
			$ret = [];

			$this->tryPdoExcept(function () use (&$ret, $userOne, $userTwo) {
				$stmt = $this->db->prepareStored(self::SQL_GRELS);
				$stmt->bindParam(':userOne', $userOne);
				$stmt->bindParam(':userTwo', $userTwo);

				if ($stmt->execute()) {
					$rows = [];

					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$rows[] = UserRelation::fromArray($row, $this->db, $this->log);
					}

					if (count($rows) == 2) {
						$ret = $rows;
					}
				}
			}, "Failed to get user relation");

			return $ret;
		}

		/**
		 * Retrieves a relation's stage, if present. Stage with null value returned if not found.
		 *
		 * @param int $userOne Integer identifier of first potential relation.
		 * @param int $userTwo Integer identifier of second potential relation.
		 * @return UserRelationStages
		 */
		public function getRelationStage(int $userOne, int $userTwo) : UserRelationStages {
			$rel = $this->getRelation($userOne, $userTwo);

			if (count($rel) != 2 || $rel[0]->stage->getValue() !== $rel[1]->stage->getValue()) {
				return new UserRelationStages();
			}

			return $rel[0]->stage;
		}

		/**
		 * Retrieves relations of user by other users, if available.
		 *
		 * @param int $userId Integer identifier of user looking for relations.
		 * @return UserRelation[]
		 */
		public function getIncomingRelations(int $userId) : array {
			$ret = [];

			$this->tryPdoExcept(function () use (&$ret, $userId) {
				$stmt = $this->db->prepareStored(self::SQL_GINCRELS);
				$stmt->bindParam(':userId', $userId);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = UserRelation::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to get user relation");

			return $ret;
		}

		/**
		 * Retrieves relations of user by other users, if available, filtered by stage.
		 *
		 * @param int $userId Integer identifier of user looking for relations.
		 * @param int $stage Integer stage specifier to filter relations through.
		 * @throws \Exception
		 * @return UserRelation[]
		 */
		public function getIncomingRelationsByStage(int $userId, int $stage) : array {
			$ret = [];

			if ($stage == UserRelationStages::ERROR || !UserRelationStages::validValue($stage)) {
				return $ret;
			}

			$this->tryPdoExcept(function () use (&$ret, $userId, $stage) {
				$stmt = $this->db->prepareStored(self::SQL_GINCRELSBYSTAGE);
				$stmt->bindParam(':userId', $userId);
				$stmt->bindParam(':stage', $stage);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = UserRelation::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to get user relation");

			return $ret;
		}

		/**
		 * Retrieves relations of user by other users, if available, except in the given stage.
		 *
		 * @param int $userId Integer identifier of user looking for relations.
		 * @param int $stage Integer stage specifier to filter relations through.
		 * @throws \Exception
		 * @return UserRelation[]
		 */
		public function getIncomingRelationsExceptingStage(int $userId, int $stage) : array {
			$ret = [];

			if ($stage == UserRelationStages::ERROR || !UserRelationStages::validValue($stage)) {
				return $ret;
			}

			$this->tryPdoExcept(function () use (&$ret, $userId, $stage) {
				$stmt = $this->db->prepareStored(self::SQL_GINCRELSEXCEPTSTAGE);
				$stmt->bindParam(':userId', $userId, \PDO::PARAM_INT);
				$stmt->bindParam(':stage', $stage, \PDO::PARAM_INT);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = UserRelation::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to get user relation");

			return $ret;
		}

		/**
		 * Retrieves relations by user to other users, if available.
		 *
		 * @param int $userId Integer identifier of user looking for relations.
		 * @return UserRelation[]
		 */
		public function getOutgoingRelations(int $userId) : array {
			$ret = [];

			$this->tryPdoExcept(function () use (&$ret, $userId) {
				$stmt = $this->db->prepareStored(self::SQL_GOUTRELS);
				$stmt->bindParam(':userId', $userId, \PDO::PARAM_INT);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = UserRelation::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to get user relation");

			return $ret;
		}

		/**
		 * Retrieves by user to other users, if available, filtered by stage.
		 *
		 * @param int $userId Integer identifier of user looking for relations.
		 * @param int $stage Integer stage specifier to filter relations through.
		 * @throws \Exception
		 * @return array
		 */
		public function getOutgoingRelationsByStage(int $userId, int $stage) : array {
			$ret = [];

			if ($stage == UserRelationStages::ERROR || !UserRelationStages::validValue($stage)) {
				return $ret;
			}

			$this->tryPdoExcept(function () use (&$ret, $userId, $stage) {
				$stmt = $this->db->prepareStored(self::SQL_GOUTRELSBYSTAGE);
				$stmt->bindParam(':userId', $userId);
				$stmt->bindParam(':stage', $stage);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = UserRelation::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to get user relation");

			return $ret;
		}

		/**
		 * Retrieves by user to other users, if available, except in the given stage.
		 *
		 * @param int $userId Integer identifier of user looking for relations.
		 * @param int $stage Integer stage specifier to filter relations through.
		 * @throws \Exception
		 * @return array
		 */
		public function getOutgoingRelationsExceptingStage(int $userId, int $stage) : array {
			$ret = [];

			if ($stage == UserRelationStages::ERROR || !UserRelationStages::validValue($stage)) {
				return $ret;
			}

			$this->tryPdoExcept(function () use (&$ret, $userId, $stage) {
				$stmt = $this->db->prepareStored(self::SQL_GOUTRELSEXCEPTSTAGE);
				$stmt->bindParam(':userId', $userId, \PDO::PARAM_INT);
				$stmt->bindParam(':stage', $stage, \PDO::PARAM_INT);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = UserRelation::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to get user relation");

			return $ret;
		}
	}
