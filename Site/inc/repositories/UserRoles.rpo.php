<?php

	namespace Zibings;

	use Stoic\Pdo\BaseDbQueryTypes;
	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbClass;

	/**
	 * Repository methods related to user roles.
	 *
	 * @package Zibings
	 */
	class UserRoles extends StoicDbClass {
		const SQL_DELUSRALLROLES      = 'userroles-deleteuserfromroles';
		const SQL_DELUSRROLE          = 'userroles-deleteuserrole';
		const SQL_DELUSRROLEBYID      = 'userroles-deleteuserrolebyid';
		const SQL_DELUSRSINROLEBYNAME = 'userroles-deleteusersinrolebyname';
		const SQL_GROLEFORUSR         = 'userroles-getroleforuser';
		const SQL_GUSRSINROLEBYNAME   = 'userroles-getusersinrolebyname';
		const SQL_INSUSRROLE          = 'userroles-insertuserrole';
		const SQL_USRINROLEBYID       = 'userroles-userinrolebyid';


		/**
		 * Internal Role instance.
		 *
		 * @var Role
		 */
		protected Role $rlObj;


		/**
		 * Whether the stored queries have been initialized.
		 *
		 * @var bool
		 */
		private static bool $dbInitialized = false;


		/**
		 * Initializes the internal Role instance.
		 *
		 * @return void
		 */
		protected function __initialize() : void {
			$this->rlObj = new Role($this->db, $this->log);

			if (!static::$dbInitialized) {
				$usrObj = new User($this->db, $this->log);

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_INSUSRROLE, "INSERT INTO [dbo].[UserRole] ([UserID], [RoleID]) VALUES (:userId, :roleId)");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_INSUSRROLE, "INSERT INTO `UserRole` (`UserID`, `RoleID`) VALUES (:userId, :roleId)");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_DELUSRROLE, "DELETE FROM [dbo].[UserRole] WHERE [UserID] = :userId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_DELUSRROLE, "DELETE FROM `UserRole` WHERE `UserID` = :userId");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GROLEFORUSR, $this->rlObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [ID] IN (SELECT [RoleID] FROM [dbo].[UserRole] WHERE [UserID] = :userId)");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GROLEFORUSR, $this->rlObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `ID` IN (SELECT `RoleID` FROM `UserRole` WHERE `UserID` = :userId)");

				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_GUSRSINROLEBYNAME, $usrObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE `ID` IN (SELECT `UserID` FROM `UserRole` WHERE `RoleID` = :roleId)");
				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_GUSRSINROLEBYNAME, $usrObj->generateClassQuery(BaseDbQueryTypes::SELECT, false) . " WHERE [ID] IN (SELECT [UserID] FROM [dbo].[UserRole] WHERE [RoleID] = :roleId)");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_DELUSRSINROLEBYNAME, "DELETE FROM [dbo].[UserRole] WHERE [RoleID] = (SELECT [ID] FROM [dbo].[Role] WHERE [Name] = :name)");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_DELUSRSINROLEBYNAME, "DELETE FROM `UserRole` WHERE `RoleID` = (SELECT `ID` FROM `Role` WHERE `Name` = :name)");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_DELUSRALLROLES, "DELETE FROM [dbo].[UserRole] WHERE [UserID] = :userId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_DELUSRALLROLES, "DELETE FROM `UserRole` WHERE `UserID` = :userId");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_DELUSRROLEBYID, "DELETE FROM [dbo].[UserRole] WHERE [UserID] = :userId AND [RoleID] = :roleId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_DELUSRROLEBYID, "DELETE FROM `UserRole` WHERE `UserID` = :userId AND `RoleID` = :roleId");

				PdoHelper::storeQuery(PdoDrivers::PDO_SQLSRV, self::SQL_USRINROLEBYID, "SELECT * FROM [dbo].[UserRole] WHERE [UserID] = :userId AND [RoleID] = :roleId");
				PdoHelper::storeQuery(PdoDrivers::PDO_MYSQL,  self::SQL_USRINROLEBYID, "SELECT * FROM `UserRole` WHERE `UserID` = :userId AND `RoleID` = :roleId");

				static::$dbInitialized = true;
			}

			return;
		}

		/**
		 * Adds a user to the role identified by the provided string.
		 *
		 * @param int $userId Integer identifier of user to add to role.
		 * @param string $name String identifier of role to add user to.
		 * @throws \Exception
		 * @return bool
		 */
		public function addUserToRoleByName(int $userId, string $name) : bool {
			$user = User::fromId($userId, $this->db, $this->log);
			$role = Role::fromName($name, $this->db, $this->log);

			if ($user->id < 1 || $role->id < 1) {
				return false;
			}

			if ($this->userInRoleByName($user->id, $role->name)) {
				return true;
			}

			$this->tryPdoExcept(function () use ($userId, $role) {
				$stmt = $this->db->prepareStored(self::SQL_INSUSRROLE);
				$stmt->bindParam(':userId', $userId);
				$stmt->bindParam(':roleId', $role->id);
				$stmt->execute();
			}, "Failed to add user to role");

			return true;
		}

		/**
		 * Removes all roles for the given user.
		 *
		 * @param int $userId Integer identifier for user in question.
		 * @return void
		 */
		public function deleteAllForUser(int $userId) : void {
			if ($userId < 1) {
				return;
			}

			$this->tryPdoExcept(function () use ($userId) {
				$stmt = $this->db->prepareStored(self::SQL_DELUSRROLE);
				$stmt->bindParam(':userId', $userId);
				$stmt->execute();
			}, "Failed to delete user's contacts");

			return;
		}

		/**
		 * Retrieves all roles the specified user belongs to, empty array if not found.
		 *
		 * @param int $userId Integer identifier of user in question.
		 * @return Role[]
		 */
		public function getAllUserRoles(int $userId) : array {
			$ret = [];

			if ($userId < 1) {
				return $ret;
			}

			$this->tryPdoExcept(function () use (&$ret, $userId) {
				$stmt = $this->db->prepareStored(self::SQL_GROLEFORUSR);
				$stmt->bindParam(':userId', $userId);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$tmp               = Role::fromArray($row, $this->db, $this->log);
						$ret[$row['ID']]   = $tmp;
						$ret[$row['Name']] = $tmp;
					}
				}
			}, "Failed to get user roles");

			return $ret;
		}

		/**
		 * Retrieves any and all users assigned to a role, empty array if not found.
		 *
		 * @param string $name Name of the role in question.
		 * @return User[]
		 */
		public function getAllUsersInRoleByName(string $name) : array {
			$ret    = [];

			$this->tryPdoExcept(function () use (&$ret, $name) {
				$role = Role::fromName($name, $this->db, $this->log);
				$stmt = $this->db->prepareStored(self::SQL_GUSRSINROLEBYNAME);
				$stmt->bindParam(':roleId', $role->id);

				if ($stmt->execute()) {
					while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
						$ret[] = User::fromArray($row, $this->db, $this->log);
					}
				}
			}, "Failed to retrieve user list");

			return $ret;
		}

		/**
		 * Attempts to remove all users assigned to the given role.
		 *
		 * @param string $name Name of the role in question.
		 * @return void
		 */
		public function removeAllUsersFromRoleByName(string $name) : void {
			$this->tryPdoExcept(function () use ($name) {
				$stmt = $this->db->prepareStored(self::SQL_DELUSRSINROLEBYNAME);
				$stmt->bindParam(':name', $name);
				$stmt->execute();
			}, "Failed to remove users from role");

			return;
		}

		/**
		 * Removes all roles for the given users.
		 *
		 * @param int $userId Integer identifier of the user in question.
		 * @return void
		 */
		public function removeUserFromAllRoles(int $userId) : void {
			if ($userId < 1) {
				return;
			}

			$this->tryPdoExcept(function () use ($userId) {
				$stmt = $this->db->prepareStored(self::SQL_DELUSRALLROLES);
				$stmt->bindParam(':userId', $userId);
				$stmt->execute();
			}, "Failed to remove user from all roles");

			return;
		}

		/**
		 * Removes the role from the user in question.
		 *
		 * @param int $userId Integer identifier of the user in question.
		 * @param string $name Name of the role in question.
		 * @return void
		 */
		public function removeUserFromRoleByName(int $userId, string $name) : void {
			if ($userId < 1 || empty($name)) {
				return;
			}

			$role = Role::fromName($name, $this->db, $this->log);

			if ($role->id < 1) {
				return;
			}

			$this->tryPdoExcept(function () use ($userId, $role) {
				$stmt = $this->db->prepareStored(self::SQL_DELUSRROLEBYID);
				$stmt->bindParam(':userId', $userId);
				$stmt->bindParam(':roleId', $role->id);
				$stmt->execute();
			}, "Failed to remove user from role");

			return;
		}

		/**
		 * Checks whether the user is a member of the role in question.
		 *
		 * @param int $userId Integer identifier of the user in question.
		 * @param string $name Name of the role in question.
		 * @return bool
		 */
		public function userInRoleByName(int $userId, string $name) : bool {
			if ($userId < 1 || empty($name)) {
				return false;
			}

			$role = Role::fromName($name, $this->db, $this->log);

			if ($role->id < 1) {
				return false;
			}

			$ret = false;

			$this->tryPdoExcept(function () use (&$ret, $userId, $role) {
				$stmt = $this->db->prepareStored(self::SQL_USRINROLEBYID);
				$stmt->bindParam(':userId', $userId);
				$stmt->bindParam(':roleId', $role->id);

				if ($stmt->execute()) {
					while ($stmt->fetch()) {
						$ret = true;
					}
				}
			}, "Failed to check if user is in role.");

			return $ret;
		}
	}
