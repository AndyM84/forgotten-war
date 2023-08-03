<?php

	namespace Zibings;

	use Stoic\Log\Logger;
	use Stoic\Pdo\BaseDbTypes;
	use Stoic\Pdo\BaseDbQueryTypes;
	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbModel;
	use Stoic\Utilities\ReturnHelper;

	/**
	 * Class for representing the basic information that comprises a user.
	 *
	 * @package Zibings
	 */
	class User extends StoicDbModel {
		const SQL_SELBYEMAIL       = 'user-selectbyemail';
		const SQL_SELBYEMAILNOTID  = 'user-selectbyemailandnotid';
		const SQL_UPDATELASTACTIVE = 'user-updatelastactivetime';


		/**
		 * The email address for the user.
		 *
		 * @var string
		 */
		public string $email;
		/**
		 * Whether the user's current email address has been confirmed as 'real'.
		 *
		 * @var bool
		 */
		public bool $emailConfirmed;
		/**
		 * Integer identifier for the user within the system.
		 *
		 * @var int
		 */
		public int $id;
		/**
		 * Date and time the user joined the site.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $joined;
		/**
		 * Last date and time the user was active on the site.  Null if the user has not yet logged in.
		 *
		 * @var \DateTimeInterface|null
		 */
		public ?\DateTimeInterface $lastActive;
		/**
		 * Last date and time the user logged into the site.  Null if the user has not yet logged in.
		 *
		 * @var null|\DateTimeInterface
		 */
		public ?\DateTimeInterface $lastLogin;


		/**
		 * Whether the stored queries have been initialized.
		 *
		 * @var bool
		 */
		private static bool $dbInitialized = false;


		/**
		 * Static method to retrieve a user by their email address. Returns blank user if user not found.
		 *
		 * @param string $email Email address value to search for in database.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param null|Logger $log Optional Logger instance for internal use, new instance created if not supplied.
		 * @return User
		 */
		public static function fromEmail(string $email, PdoHelper $db, Logger $log = null) : User {
			$ret = new User($db, $log);

			if (!static::validEmail($email)) {
				$ret->log->error("Invalid email address");

				return $ret;
			}

			$ret->tryPdoExcept(function () use (&$ret, $email) {
				$stmt = $ret->db->prepareStored(self::SQL_SELBYEMAIL);
				$stmt->bindParam(':email', $email);

				if ($stmt->execute()) {
					$row = $stmt->fetch(\PDO::FETCH_ASSOC);

					if ($row !== false) {
						$ret = User::fromArray($row, $ret->db, $ret->log);
					}
				}
			}, "Failed to get user by email address");

			return $ret;
		}

		/**
		 * Static method to retrieve a user by their integer identifier. Returns blank user if user not found.
		 *
		 * @param int $id Integer identifier to use when searching the database.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created if not supplied.
		 * @throws \Exception
		 * @return User
		 */
		public static function fromId(int $id, PdoHelper $db, Logger $log = null) : User {
			$ret = new User($db, $log);
			$ret->id = $id;

			if ($ret->read()->isBad()) {
				$ret->id = 0;
			}

			return $ret;
		}

		/**
		 * Static method to validate the format of an email address.
		 *
		 * @param string $email The email address to validate.
		 * @return bool
		 */
		public static function validEmail(string $email) : bool {
			return filter_var($email, FILTER_VALIDATE_EMAIL) !== false;
		}


		/**
		 * Determines if system should attempt to create a new User in the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			$ret = new ReturnHelper();

			if ($this->id > 0 || !static::validEmail($this->email)) {
				$ret->addMessage("Cannot create a User with invalid email or id fields");

				return $ret;
			}

			$this->tryPdoExcept(function () use (&$ret) {
				$stmt = $this->db->prepareStored(self::SQL_SELBYEMAIL);
				$stmt->bindValue(':email', $this->email);
				$stmt->execute();

				if ($stmt->fetch() !== false) {
					$ret->addMessage("Found duplicate User by email, unable to create (Email: {$this->email})");
				} else {
					$ret->makeGood();
					$this->joined = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
				}
			}, "Failed to check for duplicate users");

			return $ret;
		}

		/**
		 * Determines if the system should attempt to delete a User from the database.
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
		 * Determines if the system should attempt to read a User from the database.
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
		 * Determines if the system should attempt to update a User in the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canUpdate() : bool|ReturnHelper {
			$ret = new ReturnHelper();

			if ($this->id < 1 || !static::validEmail($this->email)) {
				$ret->addMessage("Invalid data for User update (check ID and Email for valid values.");

				return $ret;
			}

			$this->tryPdoExcept(function () use (&$ret) {
				$stmt = $this->db->prepareStored(self::SQL_SELBYEMAILNOTID);
				$stmt->bindValue(':email', $this->email);
				$stmt->bindValue(':id', $this->id);
				$stmt->execute();

				if ($stmt->fetch()[0] > 0) {
					$ret->addMessage("Found duplicate User by email, unable to update (Email: {$this->email})");
				} else {
					$ret->makeGood();
				}
			}, "Failed to check for duplicate users");

			return $ret;
		}

		/**
		 * Initializes a new User object.
		 *
		 * @throws \Exception
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[dbo].[User]');
			} else {
				$this->setTableName('User');
			}

			$this->setColumn('email', 'Email', BaseDbTypes::STRING, false, true, true);
			$this->setColumn('emailConfirmed', 'EmailConfirmed', BaseDbTypes::BOOLEAN, false, true, true);
			$this->setColumn('id', 'ID', BaseDbTypes::INTEGER, true, false, false, false, true);
			$this->setColumn('joined', 'Joined', BaseDbTypes::DATETIME, false, true, false);
			$this->setColumn('lastActive', 'LastActive', BaseDbTypes::DATETIME, false, true, true, true);
			$this->setColumn('lastLogin', 'LastLogin', BaseDbTypes::DATETIME, false, true, true, true);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_SELBYEMAIL, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [Email] = :email");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_SELBYEMAIL, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `Email` = :email");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_SELBYEMAILNOTID, "SELECT COUNT(*) FROM {$this->dbTable} WHERE [Email] = :email AND [ID] <> :id");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_SELBYEMAILNOTID, "SELECT COUNT(*) FROM {$this->dbTable} WHERE `Email` = :email AND `ID` <> :id");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_UPDATELASTACTIVE, "UPDATE {$this->dbTable} SET [LastActive] = :today WHERE [ID] = :userId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_UPDATELASTACTIVE, "UPDATE {$this->dbTable} SET `LastActive` = :today WHERE `ID` = :userId");

				static::$dbInitialized = true;
			}

			$this->id             = 0;
			$this->lastActive     = null;
			$this->lastLogin      = null;
			$this->email          = '';
			$this->emailConfirmed = false;
			$this->joined         = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));

			return;
		}

		/**
		 * Attempts to update the LastActive time for the given user.
		 *
		 * @return void
		 */
		public function markActive() : void {
			if ($this->id < 1) {
				return;
			}

			$this->tryPdoExcept(function () {
				$stmt = $this->db->prepareStored(self::SQL_UPDATELASTACTIVE);
				$stmt->bindValue(':today', (new \DateTimeImmutable('now', new \DateTimeZone('UTC')))->format('Y-m-d H:i:s'));
				$stmt->bindValue(':userId', $this->id);
				$stmt->execute();
			}, "Failed to mark user as active");

			return;
		}
	}
