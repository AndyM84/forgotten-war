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
	 * List of available roles in system.
	 *
	 * @package Zibings
	 */
	class RoleStrings {
		const ADMINISTRATOR = 'Administrator';


		/**
		 * Internal static cache of constants.
		 *
		 * @var null|array
		 */
		protected static ?array $constCache = null;


		/**
		 * Retrieves the internal cache of constants.
		 *
		 * @return array
		 */
		public static function getConstList() : array {
			if (static::$constCache === null) {
				$ref = new \ReflectionClass(get_called_class());
				static::$constCache = $ref->getConstants();
			}

			return static::$constCache;
		}
	}

	/**
	 * Class for representing an access control role within the system.
	 *
	 * @package Zibings
	 */
	class Role extends StoicDbModel {
		const SQL_SELBYNAME   = 'role-selectbyname';
		const SQL_SELBYNAMEID = 'role-selectbynameandid';


		/**
		 * Date and time the role was created.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $created;
		/**
		 * Integer identifier of role.
		 *
		 * @var int
		 */
		public int $id;
		/**
		 * Friendly name of role.
		 *
		 * @var string
		 */
		public string $name;


		/**
		 * Whether the stored queries have been initialized.
		 *
		 * @var bool
		 */
		private static bool $dbInitialized = false;


		/**
		 * Static method to retrieve a role from the database using its identifier. Returns an empty Role object if no role is
		 * found.
		 *
		 * @param int $id Integer identifier of role to retrieve from database.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created if not supplied.
		 * @throws \Exception
		 * @return Role
		 */
		public static function fromId(int $id, PdoHelper $db, Logger $log = null) : Role {
			$ret = new Role($db, $log);

			if ($id > 0) {
				$ret->id = $id;

				if ($ret->read()->isBad()) {
					$ret->id = 0;
				}
			}

			return $ret;
		}

		/**
		 * Static method to retrieve a role from the database using its name. Returns an empty Role object if no role is found.
		 *
		 * @param string $name Friendly name of role to retrieve from database.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created if not supplied. 
		 * @return Role
		 */
		public static function fromName(string $name, PdoHelper $db, Logger $log = null) : Role {
			$ret = new Role($db, $log);

			if (!empty($name)) {
				$ret->tryPdoExcept(function () use ($name, &$ret) {
					$stmt = $ret->db->prepareStored(self::SQL_SELBYNAME);
					$stmt->bindParam(':name', $name);
					
					if ($stmt->execute()) {
						$ret = Role::fromArray($stmt->fetch(\PDO::FETCH_ASSOC), $ret->db, $ret->log);
					}
				}, "Failed to retrieve role from database");
			}

			return $ret;
		}


		/**
		 * Determines if the system should attempt to create a new Role in the database.
		 *
		 * @throws \Exception
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			if ($this->id > 0 || empty($this->name)) {
				return false;
			}

			$ret = true;

			$this->tryPdoExcept(function () use (&$ret) {
				$stmt = $this->db->prepareStored(self::SQL_SELBYNAME);
				$stmt->bindParam(':name', $this->name);

				if ($stmt->execute() && $stmt->fetch() !== false) {
					$ret = false;
				}
			}, "Failed to guard against role duplicate");

			if ($ret) {
				$this->created = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			}

			return $ret;
		}

		/**
		 * Determines if the system should attempt to delete a Role from the database.
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
		 * Determines if the system should attempt to read a Role from the database.
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
		 * Determines if the system should attempt to update a Role in the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canUpdate() : bool|ReturnHelper {
			$ret = new ReturnHelper();
			$ret->makeBad();

			if ($this->id < 1 || empty($this->name)) {
				$ret->addMessage("Invalid name or identifier for update");

				return $ret;
			}

			$this->tryPdoExcept(function () use (&$ret) {
				$stmt = $this->db->prepareStored(self::SQL_SELBYNAMEID);
				$stmt->bindValue(':name', $this->name);
				$stmt->bindValue(':id', $this->id);
				$stmt->execute();

				if ($stmt->fetch()[0] > 0) {
					$ret->addMessage("Found duplicate role with name {$this->name} in database");
				} else {
					$ret->makeGood();
				}
			}, "Failed to guard against role duplicate");

			return $ret;
		}

		/**
		 * Initializes a new Role object before use.
		 *
		 * @throws \Exception
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[dbo].[Role]');
			} else {
				$this->setTableName('Role');
			}

			$this->setColumn('created', 'Created', BaseDbTypes::DATETIME, false, true, false);
			$this->setColumn('id', 'ID', BaseDbTypes::INTEGER, true, false, false, false, true);
			$this->setColumn('name', 'Name', BaseDbTypes::STRING, false, true, true);

			if (!static::$dbInitialized) {
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_SELBYNAME, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [Name] = :name");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_SELBYNAME, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `Name` = :name");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_SELBYNAME, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [Name] = :name AND [ID] <> :id");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_SELBYNAME, $this->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `Name` = :name AND `ID` <> :id");

				static::$dbInitialized = true;
			}

			$this->created = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->id      = 0;
			$this->name    = '';

			return;
		}
	}
