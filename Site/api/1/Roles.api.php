<?php

	namespace Api1;

	use Stoic\Web\Api\Response;
	use Stoic\Web\Request;

	use Zibings\ApiController;
	use Zibings\Role;
	use Zibings\Roles as ZibRoles;
	use Zibings\RoleStrings;
	use Zibings\UserRoles;

	/**
	 * API controller that deals with role-related endpoints.
	 *
	 * @package Zibings\Api1
	 */
	class Roles extends ApiController {
		/**
		 * Attempts to add a new role to the system.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Exception
		 * @return Response
		 */
		public function addRole(Request $request, array $matches = null) : Response {
			$ret    = $this->newResponse();
			$params = $request->getInput();
			
			$role       = new Role($this->db, $this->log);
			$role->name = $params->getString('name');
			$create     = $role->create();

			if ($create->isBad()) {
				$this->assignReturnHelperError($ret, $create, "Failed to create new role");

				return $ret;
			}

			$ret->setData($role);

			return $ret;
		}

		/**
		 * Retrieves all roles in the database.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @return Response
		 */
		public function getRoles(Request $request, array $matches = null) : Response {
			$ret = $this->newResponse();
			$ret->setData((new ZibRoles($this->db, $this->log))->getAll());

			return $ret;
		}

		/**
		 * Retrieves any roles in the database for the given user.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @return Response
		 */
		public function getUserRoles(Request $request, array $matches = null) : Response {
			$ret = $this->newResponse();
			$ret->setData((new UserRoles($this->db, $this->log))->getAllUserRoles(intval($matches[1][0])));

			return $ret;
		}

		/**
		 * Registers the controller endpoints.
		 *
		 * @return void
		 */
		protected function registerEndpoints() : void {
			$this->registerEndpoint('POST', '/^Roles\/Add\/?/i',                       'addRole',         RoleStrings::ADMINISTRATOR);
			$this->registerEndpoint('GET',  '/^Roles\/GetRoles\/?/i',                  'getRoles',        RoleStrings::ADMINISTRATOR);
			$this->registerEndpoint('GET',  '/^Roles\/GetUserRoles\/([0-9]{1,})\/?/i', 'getUserRoles',    RoleStrings::ADMINISTRATOR);
			$this->registerEndpoint('POST', '/^Roles\/RemoveUserRoles\/?/i',           'removeUserRoles', RoleStrings::ADMINISTRATOR);
			$this->registerEndpoint('POST', '/^Roles\/RemoveUserRole\/?/i',            'removeUserRole',  RoleStrings::ADMINISTRATOR);
			$this->registerEndpoint('POST', '/^Roles\/Remove\/?/i',                    'removeRole',      RoleStrings::ADMINISTRATOR);
			$this->registerEndpoint('POST', '/^Roles\/SetUserRole\/?/i',               'setUserRole',     RoleStrings::ADMINISTRATOR);
			$this->registerEndpoint('POST', '/^Roles\/UserInRole\/?/i',                'userInRole',      RoleStrings::ADMINISTRATOR);
			$this->registerEndpoint('GET',  '/^Roles\/UsersInRole\/([a-z]{3,})\/?/i',  'usersInRole',     RoleStrings::ADMINISTRATOR);

			return;
		}

		/**
		 * Attempts to remove a role from the system.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Exception
		 * @return Response
		 */
		public function removeRole(Request $request, array $matches = null) : Response {
			$ret    = $this->newResponse();
			$params = $request->getInput();

			$role = Role::fromName($params->getString('name'), $this->db, $this->log);

			if ($role->id < 1) {
				$ret->setAsError("Invalid role supplied");

				return $ret;
			}

			(new UserRoles($this->db, $this->log))->removeAllUsersFromRoleByName($role->name);
			$delete = $role->delete();

			if ($delete->isBad()) {
				$this->assignReturnHelperError($ret, $delete, "Failed to delete role");

				return $ret;
			}

			$ret->setData($role);

			return $ret;
		}

		/**
		 * Removes role for given user, if present.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException
		 * @return Response
		 */
		public function removeUserRole(Request $request, array $matches = null) : Response {
			$ret    = $this->newResponse();
			$params = $request->getInput();

			if (!$params->hasAll('userId', 'role')) {
				$ret->setAsError("Invalid parameters supplied");

				return $ret;
			}

			$userId = $params->getInt('userId');
			$role   = $params->getString('role');
			(new UserRoles($this->db, $this->log))->removeUserFromRoleByName($userId, $role);

			$ret->setData(true);

			return $ret;
		}

		/**
		 * Removes all roles for given user.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\Exception
		 * @return Response
		 */
		public function removeUserRoles(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();

			(new UserRoles($this->db, $this->log))->removeUserFromAllRoles(intval($params->getInt('userId', $user->id)));

			$ret->setData(true);

			return $ret;
		}

		/**
		 * Assigns a user to a given role.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function setUserRole(Request $request, array $matches = null) : Response {
			$ret    = $this->newResponse();
			$params = $request->getInput();

			if (!$params->hasAll('userId', 'role')) {
				$ret->setAsError("Invalid parameters supplied");

				return $ret;
			}

			if (!(new UserRoles($this->db, $this->log))->addUserToRoleByName($params->getInt('userId'), $params->getString('role'))) {
				$ret->setAsError("Failed to assign user role");

				return $ret;
			}

			$ret->setData(Role::fromName($params->getString('role'), $this->db, $this->log));

			return $ret;
		}

		/**
		 * Checks if a user is assigned a role.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\Exception
		 * @return Response
		 */
		public function userInRole(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();

			$ret->setData((new UserRoles($this->db, $this->log))->userInRoleByName($params->getInt('userId', $user->id), $params->getString('role')));

			return $ret;
		}

		/**
		 * Retrieves any and all users assigned to a specific role.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @return Response
		 */
		public function usersInRole(Request $request, array $matches = null) : Response {
			$ret = $this->newResponse();
			$ret->setData((new UserRoles($this->db, $this->log))->getAllUsersInRoleByName($matches[1][0]));

			return $ret;
		}
	}
