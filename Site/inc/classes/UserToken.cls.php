<?php

	namespace Zibings;

	use Stoic\Log\Logger;
	use Stoic\Pdo\BaseDbQueryTypes;
	use Stoic\Pdo\BaseDbTypes;
	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbModel;
	use Stoic\Utilities\ReturnHelper;

	/**
	 * Class for representing a single token associated with a user.
	 *
	 * @package Zibings
	 */
	class UserToken extends StoicDbModel {
		const SQL_SELBYTOKENUID = 'usertoken-selectbytokenanduserid';


		/**
		 * Date and time the token was created.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $created;
		/**
		 * General context of token, optional data field.
		 *
		 * @var string
		 */
		public string $context;
		/**
		 * Integer identifier of token.
		 *
		 * @var int
		 */
		public int $id;
		/**
		 * String identifier of token.
		 *
		 * @var string
		 */
		public string $token;
		/**
		 * Integer identifier of token owner.
		 *
		 * @var int
		 */
		public int $userId;


		/**
		 * Whether the stored queries have been initialized.
		 *
		 * @var bool
		 */
		private static bool $dbInitialized = false;


		/**
		 * Static method to retrieve a token by its unique integer identifier.
		 *
		 * @param int $id Integer identifier of token to retrieve.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created by default.
		 * @throws \Exception
		 * @return UserToken
		 */
		public static function fromId(int $id, PdoHelper $db, Logger $log = null) : UserToken {
			$ret = new UserToken($db, $log);
			$ret->id = $id;

			if ($ret->read()->isBad()) {
				$ret->id = 0;
			}

			return $ret;
		}

		/**
		 * Static method to retrieve a token by its string and user identifiers.
		 *
		 * @param string $token String identifier of token to retrieve.
		 * @param int $userId Integer identifier of user who owns token.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created by default.
		 * @return UserToken
		 */
		public static function fromToken(string $token, int $userId, PdoHelper $db, Logger $log = null) : UserToken {
			$ret = new UserToken($db, $log);

			if (empty($token) || $userId < 1) {
				return $ret;
			}

			$ret->tryPdoExcept(function () use (&$ret, $token, $userId) {
				$stmt = $ret->db->prepareStored(self::SQL_SELBYTOKENUID);
				$stmt->bindParam(':token', $token);
				$stmt->bindParam(':userId', $userId);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret = UserToken::fromArray($row, $ret->db, $ret->log);
					}
				}
			}, "Failed to search for user token");

			return $ret;
		}


		/**
		 * Determines if the system should attempt to create a UserToken in the database.
		 *
		 * @throws \Exception
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			if ($this->id > 0 || $this->userId < 1 || empty($this->token)) {
				return false;
			}

			$this->created = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));

			return true;
		}
		
		/**
		 * Determines if the system should attempt to delete a UserToken from the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canDelete() : bool|ReturnHelper {
			if ($this->id < 1) {
				return false;
			}

			return true;
		}
		
		/**
		 * Determines if the system should attempt to read a UserToken from the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canRead() : bool|ReturnHelper {
			if ($this->id < 1) {
				return false;
			}

			return true;
		}
		
		/**
		 * Determines if the system should attempt to update a UserToken in the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canUpdate() : bool|ReturnHelper {
			if ($this->id < 1) {
				return false;
			}

			return true;
		}
		
		/**
		 * Initializes a new UserToken object.
		 *
		 * @throws \Exception
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[dbo].[UserToken]');
			} else {
				$this->setTableName('UserToken');
			}

			$this->setColumn('created', 'Created', BaseDbTypes::DATETIME, false, true, false);
			$this->setColumn('context', 'Context', BaseDbTypes::STRING, false, true, true);
			$this->setColumn('id', 'ID', BaseDbTypes::INTEGER, true, false, false, false, true);
			$this->setColumn('token', 'Token', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('userId', 'UserID', BaseDbTypes::INTEGER, false, true, false);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_SELBYTOKENUID, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [Token] = :token AND [UserID] = :userId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_SELBYTOKENUID, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `Token` = :token AND `UserID` = :userId");

				static::$dbInitialized = true;
			}

			$this->created = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->context = '';
			$this->id      = 0;
			$this->token   = '';
			$this->userId  = 0;
			
			return;
		}
	}
