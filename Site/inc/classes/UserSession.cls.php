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
	 * Class for representing a single session for a user.
	 *
	 * @package Zibings
	 */
	class UserSession extends StoicDbModel {
		const SQL_SELBYTOKEN = 'usersession-selectbytoken';


		/**
		 * Network address of user when this session was created.
		 *
		 * @var string
		 */
		public string $address;
		/**
		 * Date and time this session was created.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $created;
		/**
		 * Network hostname of user when this session was created.
		 *
		 * @var string
		 */
		public string $hostname;
		/**
		 * Unique integer identifier for this session.
		 *
		 * @var int
		 */
		public int $id;
		/**
		 * Unique string identifier for this session.
		 *
		 * @var string
		 */
		public string $token;
		/**
		 * Integer identifier for the user who owns this session.
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
		 * Static method to retrieve a session by its integer identifier.
		 *
		 * @param int $id Integer identifier of session.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created by default.
		 * @throws \Exception
		 * @return UserSession
		 */
		public static function fromId(int $id, PdoHelper $db, Logger $log = null) : UserSession {
			$ret = new UserSession($db, $log);
			$ret->id = $id;

			if ($ret->read()->isBad()) {
				$ret->id = 0;
			}

			return $ret;
		}

		/**
		 * Static method to retrieve a session by its string identifier.
		 *
		 * @param string $token String identifier of session.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created by default.
		 * @return UserSession
		 */
		public static function fromToken(string $token, PdoHelper $db, Logger $log = null) : UserSession {
			$ret = new UserSession($db, $log);

			if (empty($token)) {
				return $ret;
			}

			$ret->tryPdoExcept(function () use (&$ret, $token) {
				$stmt = $ret->db->prepareStored(self::SQL_SELBYTOKEN);
				$stmt->bindParam(':token', $token);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret = UserSession::fromArray($row, $ret->db, $ret->log);
					}
				}
			}, "Failed to get session from token");

			return $ret;
		}

		/**
		 * Returns a (usually) unique	GUID in the typical 8-4-4-4-12 character format.
		 *
		 * @param bool $withBrackets Whether to surround the GUID with curly brackets ({})
		 * @return string
		 */
		public static function generateGuid(bool $withBrackets = true) : string {
			$ret = '';

			// @codeCoverageIgnoreStart
			if (function_exists('com_create_guid')) {
				$ret = com_create_guid();
			} else {
				mt_srand((double)microtime()*10000);//optional for php 4.2.0 and up.
				$charid = strtoupper(md5(uniqid(rand(), true)));
				$hyphen = chr(45);// "-"
				$ret = (chr(123)
					.substr($charid, 0, 8).$hyphen
					.substr($charid, 8, 4).$hyphen
					.substr($charid,12, 4).$hyphen
					.substr($charid,16, 4).$hyphen
					.substr($charid,20,12)
					.chr(125));
			}
			// @codeCoverageIgnoreEnd

			if ($withBrackets) {
				return $ret;
			}

			return trim($ret, '{}');
		}


		/**
		 * Determines if the system should attempt to create a UserSession in the database.
		 *
		 * @throws \Exception
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			if ($this->userId < 1 || empty($this->token)) {
				return false;
			}

			$this->created = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));

			return true;
		}
		
		/**
		 * Determines if the system should attempt to delete a UserSession from the database.
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
		 * Determines if the system should attempt to read a UserSession from the database.
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
		 * Determines if the system should attempt to update a UserSession in the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canUpdate() : bool|ReturnHelper {
			if ($this->id < 1 || $this->userId < 1 || empty($this->token)) {
				return false;
			}

			return true;
		}
		
		/**
		 * Initializes a new UserSession object.
		 *
		 * @throws \Exception
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[dbo].[UserSession]');
			} else {
				$this->setTableName('UserSession');
			}

			$this->setColumn('address', 'Address', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('created', 'Created', BaseDbTypes::DATETIME, false, true, false);
			$this->setColumn('hostname', 'Hostname', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('id', 'ID', BaseDbTypes::INTEGER, true, false, false, false, true);
			$this->setColumn('token', 'Token', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('userId', 'UserID', BaseDbTypes::INTEGER, false, true, false);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_SELBYTOKEN, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [Token] = :token");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_SELBYTOKEN, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `Token` = :token");

				static::$dbInitialized = true;
			}

			$this->address  = '';
			$this->created  = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->hostname = '';
			$this->id       = 0;
			$this->token    = '';
			$this->userId   = 0;
			
			return;
		}
	}
