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
	 * Class for representing a single device associated with a user.
	 *
	 * @package Zibings
	 */
	class UserDevice extends StoicDbModel {
		const SQL_SELBYLINK = 'userdevice-selectbylinkphrase';


		/**
		 * Date and time the device was created in the system.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $created;
		/**
		 * Unique integer identifier of the device.
		 *
		 * @var int
		 */
		public int $id;
		/**
		 * String 'identifier' for device, used for distinguishing the type of device.
		 *
		 * @var string
		 */
		public string $identifier;
		/**
		 * Last date and time the device was active on the platform.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $lastActive;
		/**
		 * Date and time the device was successfully linked.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $linked;
		/**
		 * String phrase to use when linking device w/o user identifier.
		 *
		 * @var string
		 */
		public string $linkPhrase;
		/**
		 * Integer identifier of the user who owns this device.  Initial value is set to 0 before a user claims the device.
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
		 * Static method to retrieve a device by ID.
		 *
		 * @param int $id Integer identifier of device.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param null|Logger $log Optional Logger instance for internal use, new instance created if not supplied.
		 * @throws \Exception
		 * @return UserDevice
		 */
		public static function fromId(int $id, PdoHelper $db, Logger $log = null) : UserDevice {
			$ret = new UserDevice($db, $log);

			if ($id > 0) {
				$ret->id = $id;

				if ($ret->read()->isBad()) {
					$ret->id = 0;
				}
			}

			return $ret;
		}

		/**
		 * Static method to retrieve a device by its link phrase.
		 *
		 * @param string $phrase The link phrase to search for in the database.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param null|Logger $log Optional Logger instance for internal use, new instance created if not supplied.
		 * @return UserDevice
		 */
		public static function fromLinkPhrase(string $phrase, PdoHelper $db, Logger $log = null) : UserDevice {
			$ret = new UserDevice($db, $log);

			if (empty($phrase)) {
				return $ret;
			}

			$ret->tryPdoExcept(function () use (&$ret, $phrase) {
				$stmt = $ret->db->prepareStored(self::SQL_SELBYLINK);
				$stmt->bindParam(':linkPhrase', $phrase);

				if ($stmt->execute()) {
					$ret = UserDevice::fromArray($stmt->fetch(\PDO::FETCH_ASSOC), $ret->db, $ret->log);
				}
			}, "Failed to get user device by link phrase");

			return $ret;
		}


		/**
		 * Determines if the system should attempt to create a UserDevice in the database.
		 *
		 * @throws \Exception
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			if ($this->id > 0 || empty($this->linkPhrase) || empty($this->identifier)) {
				return false;
			}

			$this->created = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));

			return true;
		}
		
		/**
		 * Determines if the system should attempt to delete a UserDevice from the database.
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
		 * Determines if the system should attempt to read a UserDevice from the database.
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
		 * Determines if the system should attempt to update a UserDevice in the database.
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
		 * Initializes a new UserDevice object.
		 *
		 * @throws \Exception
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[UserDevice]');
			} else {
				$this->setTableName('UserDevice');
			}

			$this->setColumn('created', '[Created]', BaseDbTypes::DATETIME, false, true, false);
			$this->setColumn('id', '[ID]', BaseDbTypes::INTEGER, true, false, false, false, true);
			$this->setColumn('identifier', '[Identifier]', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('lastActive', '[LastActive]', BaseDbTypes::DATETIME, false, false, true, true);
			$this->setColumn('linked', '[Linked]', BaseDbTypes::DATETIME, false, false, true, true);
			$this->setColumn('linkPhrase', '[LinkPhrase]', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('userId', '[UserID]', BaseDbTypes::INTEGER, false, true, true);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_SELBYLINK, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [LinkPhrase] = :linkPhrase");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_SELBYLINK, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `LinkPhrase` = :linkPhrase");

				static::$dbInitialized = true;
			}

			$this->created    = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->id         = 0;
			$this->identifier = '';
			$this->lastActive = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->linked     = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->linkPhrase = '';
			$this->userId     = 0;
			
			return;
		}

		/**
		 * Helper method to update the lastActive property.
		 *
		 * @throws \Exception
		 * @return void
		 */
		public function touch() : void {
			$this->lastActive = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->update();

			return;
		}
	}
