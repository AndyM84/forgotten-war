<?php

	namespace Zibings;

	use Stoic\Log\Logger;
	use Stoic\Pdo\BaseDbQueryTypes;
	use Stoic\Pdo\BaseDbTypes;
	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbModel;
	use Stoic\Utilities\EnumBase;
	use Stoic\Utilities\ReturnHelper;

	/**
	 * Different genders a user can select.
	 *
	 * @package Zibings
	 */
	class UserGenders extends EnumBase {
		const NONE   = 0;
		const FEMALE = 1;
		const MALE   = 2;
		const OTHER  = 3;
	}

	/**
	 * Class for representing profile data for a single user.
	 *
	 * @package Zibings
	 */
	class UserProfile extends StoicDbModel {
		const SQL_SELBYDISPLAYNAME = 'userprofile-selectbydisplayname';
		const SQL_SELBYUID         = 'userprofile=selectybuserid';


		/**
		 * The user's birthday.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $birthday;
		/**
		 * The user's general description/about-me.
		 *
		 * @var string
		 */
		public string $description;
		/**
		 * The user's friendly display name.
		 *
		 * @var string
		 */
		public string $displayName;
		/**
		 * The user's selected gender.
		 *
		 * @var UserGenders
		 */
		public UserGenders $gender;
		/**
		 * The user's real name.
		 *
		 * @var string
		 */
		public string $realName;
		/**
		 * Integer identifier of the user.
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
		 * Static method to retrieve a user's profile via the display name.
		 *
		 * @param string $displayName Display name to use when searching database.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, default creates new instance.
		 * @return UserProfile
		 */
		public static function fromDisplayName(string $displayName, PdoHelper $db, Logger $log = null) : UserProfile {
			$ret = new UserProfile($db, $log);
			$ret->tryPdoExcept(function () use ($displayName, $db, $log, &$ret) {
				$stmt = $db->prepareStored(self::SQL_SELBYDISPLAYNAME);
				$stmt->bindParam(':displayName', $displayName);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret = UserProfile::fromArray($row, $db, $log);
					}
				}
			}, "Failed to retrieve profile by display name");

			return $ret;
		}

		/**
		 * Static method to retrieve a user's profile information.
		 *
		 * @param int $userId Integer identifier of user in question.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, default creates new instance.
		 * @throws \Exception
		 * @return UserProfile
		 */
		public static function fromUser(int $userId, PdoHelper $db, Logger $log = null) : UserProfile {
			$ret = new UserProfile($db, $log);
			$ret->userId = $userId;

			if ($ret->read()->isBad()) {
				$ret->userId = 0;
			}

			return $ret;
		}

		/**
		 * Static method to validate a string as a display name.
		 *
		 * @param string $string String to validate as display name.
		 * @return bool
		 */
		public static function validDisplayName(string $string) : bool {
			static $chars = [
				'a' => true, 'b' => true, 'c' => true, 'd' => true, 'e' => true, 'f' => true, 'g' => true, 'h' => true, 'i' => true,
				'j' => true, 'k' => true, 'm' => true, 'n' => true, 'o' => true, 'p' => true, 'q' => true, 'r' => true, 's' => true,
				't' => true, 'u' => true, 'v' => true, 'w' => true, 'x' => true, 'y' => true, 'z' => true, 'A' => true, 'B' => true,
				'C' => true, 'D' => true, 'E' => true, 'F' => true, 'G' => true, 'H' => true, 'I' => true, 'J' => true, 'K' => true,
				'L' => true, 'M' => true, 'N' => true, 'O' => true, 'P' => true, 'Q' => true, 'R' => true, 'S' => true, 'T' => true,
				'U' => true, 'V' => true, 'W' => true, 'X' => true, 'Y' => true, 'Z' => true, '0' => true, '1' => true, '2' => true,
				'3' => true, '4' => true, '5' => true, '6' => true, '7' => true, '8' => true, '9' => true, '_' => true, '.' => true
			];

			$len = strlen($string);

			if ($len < 3 || $len > 16) {
				return false;
			}

			for ($i = 0; $i < $len; $i++) {
				if (array_key_exists($string[$i], $chars) === false) {
					return false;
				}
			}

			return true;
		}


		/**
		 * Determines if the system should attempt to create a new UserProfile in the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			if ($this->userId < 1) {
				return false;
			}

			$ret = true;

			$this->tryPdoExcept(function () use (&$ret) {
				$stmt = $this->db->prepareStored(self::SQL_SELBYUID);
				$stmt->bindParam(':userId', $this->userId);

				if ($stmt->execute()) {
					while ($stmt->fetch()) {
						$ret = false;

						break;
					}
				}
			}, "Failed to check for duplicate user profile info");

			return $ret;
		}

		/**
		 * Determines if the system should attempt to delete a UserProfile from the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canDelete() : bool|ReturnHelper {
			if ($this->userId < 1) {
				return false;
			}

			return true;
		}

		/**
		 * Determines if the system should attempt to read a UserProfile from the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canRead() : bool|ReturnHelper {
			if ($this->userId < 1) {
				return false;
			}

			return true;
		}

		/**
		 * Determines if the system should attempt to update a UserProfile in the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canUpdate() : bool|ReturnHelper {
			if ($this->userId < 1) {
				return false;
			}

			return true;
		}

		/**
		 * Initializes a new UserProfile object.
		 *
		 * @throws \Exception
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[dbo].[UserProfile]');
			} else {
				$this->setTableName('UserProfile');
			}

			$this->setColumn('userId', 'UserID', BaseDbTypes::INTEGER, true, true, false);
			$this->setColumn('displayName', 'DisplayName', BaseDbTypes::STRING, false, true, true);
			$this->setColumn('birthday', 'Birthday', BaseDbTypes::DATETIME, false, true, true);
			$this->setColumn('realName', 'RealName', BaseDbTypes::STRING, false, true, true);
			$this->setColumn('description', 'Description', BaseDbTypes::STRING, false, true, true);
			$this->setColumn('gender', 'Gender', BaseDbTypes::INTEGER, false, true, true);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_SELBYDISPLAYNAME, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [DisplayName] = :displayName");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_SELBYDISPLAYNAME, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `DisplayName` = :displayName");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_SELBYUID, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [UserID] = :userId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_SELBYUID, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `UserID` = :userId");

				static::$dbInitialized = true;
			}

			$this->userId      = 0;
			$this->displayName = '';
			$this->birthday    = new \DateTimeImmutable('100 years ago', new \DateTimeZone('UTC'));
			$this->realName    = '';
			$this->description = '';
			$this->gender      = new UserGenders();

			return;
		}
	}
