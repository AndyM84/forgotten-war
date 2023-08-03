<?php

	namespace Zibings;

	use Stoic\Pdo\BaseDbQueryTypes;
	use Stoic\Pdo\StoicDbClass;

	/**
	 * Repository methods related to the UserContact table/data.
	 *
	 * @package Zibings
	 */
	class UserContacts extends StoicDbClass {
		/**
		 * Internal instance of a UserContact object, used for query generation.
		 *
		 * @var UserContact
		 */
		protected UserContact $ucObj;


		/**
		 * Initializes the internal UserContact object.
		 *
		 * @return void
		 */
		protected function __initialize() : void {
			$this->ucObj = new UserContact($this->db, $this->log);

			return;
		}

		/**
		 * Removes all contacts for the given user.
		 *
		 * @param int $userId Integer identifier for user in question.
		 * @return void
		 */
		public function deleteAllForUser(int $userId) : void {
			if ($userId < 1) {
				return;
			}

			$this->tryPdoExcept(function () use ($userId) {
				$stmt = $this->db->prepare("DELETE FROM {$this->ucObj->getDbTableName()} WHERE [UserID] = :userId");
				$stmt->bindParam(':userId', $userId);
				$stmt->execute();
			}, "Failed to delete user's contacts");

			return;
		}

		/**
		 * Retrieves all current contacts for a user.
		 *
		 * @param int $userId Integer identifier of user in question.
		 * @return UserContact[]
		 */
		public function getUserContacts(int $userId) : array {
			$ret = [];

			$this->tryPdoExcept(function () use (&$ret, $userId) {
				$stmt = $this->db->prepare($this->ucObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [UserID] = :userId");
				$stmt->bindParam(':userId', $userId);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = UserContact::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to retrieve user contacts");

			return $ret;
		}
	}
