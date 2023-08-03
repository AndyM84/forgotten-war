<?php

	namespace Api1;

	use Stoic\Log\Logger;
	use Stoic\Utilities\EnumBase;
	use Stoic\Utilities\ParameterHelper;
	use Stoic\Web\Api\Response;
	use Stoic\Web\PageHelper;
	use Stoic\Web\Request;
	use Stoic\Web\Resources\HttpStatusCodes;
	use Stoic\Web\Api\Stoic;

	use Zibings\ApiController;
	use Zibings\AuthHistoryActions;
	use Zibings\RoleStrings;
	use Zibings\User;
	use Zibings\UserAuthHistory;
	use Zibings\UserAuthHistoryLoginNode;
	use Zibings\UserAuthHistoryLogoutNode;
	use Zibings\UserEvents;
	use Zibings\UserEventTypes;
	use Zibings\UserProfile;
	use Zibings\UserRelations;
	use Zibings\UserRelationStages;
	use Zibings\UserRoles;
	use Zibings\UserSession;
	use Zibings\UserSettings;

	use function Zibings\sendResetEmail;

	/**
	 * API controller that deals with account-related endpoints.
	 *
	 * @package Zibings\Api1
	 */
	class Account extends ApiController {
		/**
		 * Instantiates a new Account object.
		 *
		 * @param Stoic $stoic Internal instance of Stoic API object.
		 * @param \PDO $db Internal instance of PDO object.
		 * @param Logger|null $log Optional Logger object for internal use.
		 * @param UserEvents|null $events Optional UserEvent object for internal use.
		 * @param UserRoles|null $userRoles Optional UserRoles object for internal use.
		 * @param UserRelations|null $userRels Optional UserRelations object for internal use.
		 * @throws \ReflectionException
		 * @return void
		 */
		public function __construct(
			Stoic $stoic,
			\PDO $db,
			Logger $log = null,
			protected UserEvents|null    $events    = null,
			protected UserRoles|null     $userRoles = null,
			protected UserRelations|null $userRels  = null) {
			parent::__construct($stoic, $db, $log);

			if ($this->events === null) {
				$this->events = new UserEvents($this->db, $this->log);
			}

			if ($this->userRoles === null) {
				$this->userRoles = new UserRoles($this->db, $this->log);
			}

			if ($this->userRels === null) {
				$this->userRels = new UserRelations($this->db, $this->log);
			}

			// NOTE: Add UserEvent nodes here if needed
			$this->events->linkToEvent(UserEventTypes::LOGIN, new UserAuthHistoryLoginNode($this->db, $this->log));
			$this->events->linkToEvent(UserEventTypes::LOGOUT, new UserAuthHistoryLogoutNode($this->db, $this->log));

			return;
		}

		/**
		 * Attempts to change an existing relation or establish a new relation between users.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function changeRelation(Request $request, array $matches = null) : Response {
			$user    = $this->getUser();
			$ret     = $this->newResponse();
			$params  = $request->getInput();

			if (!$params->hasAll('relatedUserId', 'stage')) {
				$ret->setAsError("Invalid parameters provided");

				return $ret;
			}

			$userOne = $params->getInt('id', 0);
			$userTwo = $params->getInt('relatedUserId');
			$stage   = EnumBase::tryGetEnum($params->getInt('stage'), UserRelationStages::class);

			if ($stage->getValue() === null) {
				$ret->setAsError("Invalid stage provided");

				return $ret;
			}

			if ($userOne == 0) {
				$userOne = $user->id;
			}

			if (($user->id != $userOne && !$this->userRoles->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR)) || $userOne == $userTwo) {
				$ret->setAsError("Invalid profile identifier");

				return $ret;
			}

			if ($this->userRels->areRelated($userOne, $userTwo)) {
				$rel = $this->userRels->getRelation($userOne, $userTwo);

				if ($rel[0]->origin && $rel[0]->stage->is(UserRelationStages::DECLINED)) {
					return $ret;
				}

				$this->userRels->changeStage($userOne, $userTwo, $stage->getValue());

				return $ret;
			}

			if (!$this->userRels->changeStage($userOne, $userTwo, UserRelationStages::INVITED)) {
				$ret->setAsError("Failed to invite relation");
			}

			return $ret;
		}

		/**
		 * Checks if an email is valid and in-use, returning a status of 0 only if it is both valid and currently not in use.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException
		 * @return Response
		 */
		public function checkEmail(Request $request, array $matches = null) : Response {
			$ret    = new Response(HttpStatusCodes::OK);
			$params = $request->getInput();
			$usr    = User::fromEmail($params->getString('email'), $this->db, $this->log);

			if ($usr->id > 0) {
				$ret->setData($this->newStatusResponseData(1, "Invalid email, already in use"));

				return $ret;
			}

			$ret->setData($this->newStatusResponseData(0, "Good and available email"));

			return $ret;
		}

		/**
		 * Checks if the given token is valid for the given user identifier.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException
		 * @return Response
		 */
		public function checkToken(Request $request, array $matches = null) : Response {
			$ret    = $this->newResponse();
			$params = $request->getInput();

			if (!$params->hasAll('userId', 'token')) {
				$ret->setAsError("Invalid parameters provided");

				return $ret;
			}

			$userId      = $params->getInt('userId');
			$userSession = UserSession::fromToken($params->getString('token'), $this->db, $this->log);
			UserAuthHistory::createFromUserId($userId, AuthHistoryActions::TOKEN_CHECK, new ParameterHelper($_SERVER), "Token checked for user #{$userId}", $this->db, $this->log);

			if ($userSession->userId != $userId) {
				$ret->setAsError("Invalid session parameters");

				return $ret;
			}

			$ret->setData("Token is valid");

			return $ret;
		}

		/**
		 * Attempts to create a user in the system, only callable by administrators.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\Exception
		 * @return Response
		 */
		public function createUser(Request $request, array $matches = null) : Response {
			$user    = $this->getUser();
			$ret     = $this->newResponse();
			$params  = $request->getInput();
			$evtData = [
				'email'          => $params->getString('email'),
				'key'            => $params->getString('password'),
				'confirmKey'     => $params->getString('confirmPassword'),
				'provider'       => $params->getInt('provider'),
				'emailConfirmed' => false
			];

			if ($this->userRoles->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR) && $params->has('emailConfirmed')) {
				$evtData['emailConfirmed'] = $params->getBool('emailConfirmed');
			}

			$this->processEvent($ret, 'doCreate', new ParameterHelper($evtData));

			return $ret;
		}

		/**
		 * Attempts to remove an existing relation between users.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function deleteRelation(Request $request, array $matches = null) : Response {
			$user    = $this->getUser();
			$ret     = $this->newResponse();
			$params  = $request->getInput();

			if (!$params->hasAll('relatedUserId')) {
				$ret->setAsError("Invalid parameters provided");

				return $ret;
			}

			$userOne = $params->getInt('id', 0);
			$userTwo = $params->getInt('relatedUserId');

			if ($userOne == 0) {
				$userOne = $user->id;
			}

			if (($user->id != $userOne && !$this->userRoles->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR)) || $userOne == $userTwo) {
				$ret->setAsError("Invalid profile identifier");

				return $ret;
			}

			if ($this->userRels->areRelated($userOne, $userTwo)) {
				$rel = $this->userRels->getRelation($userOne, $userTwo);

				if ($rel[0]->origin && $rel[0]->stage->is(UserRelationStages::DECLINED)) {
					return $ret;
				}
			}

			if (!$this->userRels->deleteRelation($userOne, $userTwo)) {
				$ret->setAsError("Failed to remove relation");
			}

			return $ret;
		}

		/**
		 * Attempts to remove a user from the system.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function deleteUser(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();
			$userId = $params->getInt('id', 0);
			$roles  = new UserRoles($this->db, $this->log);

			if ($userId == 0) {
				$userId = $user->id;
			}

			if ($userId != $user->id && !$roles->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR)) {
				$ret->setAsError("Invalid user identifier ({$userId}:{$user->id})");

				return $ret;
			}

			if ($userId != $user->id && $roles->userInRoleByName($userId, RoleStrings::ADMINISTRATOR)) {
				$ret->setAsError("Cannot delete other admins via API");

				return $ret;
			}

			$this->processEvent($ret, 'doDelete', new ParameterHelper([
				'id'    => $userId,
				'actor' => $user->id
			]));

			return $ret;
		}

		/**
		 * Attempts to retrieve a user's account information, only works for current user and administrators.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function get(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();
			$userId = $params->getInt('id', 0);

			if ($userId == 0) {
				$userId = $user->id;
			}

			if ($user->id != $userId && !$this->userRoles->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR)) {
				$ret->setAsError("Invalid profile identifier");

				return $ret;
			}

			$ret->setData(User::fromId($userId, $this->db, $this->log));

			return $ret;
		}

		/**
		 * Attempts to retrieve a user's profile information, only works for current user and administrators.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function getProfile(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();
			$userId = $params->getInt('id', 0);

			if ($userId == 0) {
				$userId = $user->id;
			}

			if ($user->id != $userId && !$this->userRoles->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR)) {
				$ret->setAsError("Invalid profile identifier");

				return $ret;
			}

			$ret->setData(UserProfile::fromUser($userId, $this->db, $this->log));

			return $ret;
		}

		/**
		 * Attempts to retrieve any relations for the given user.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function getRelations(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();
			$userId = $params->getInt('id', 0);

			if ($userId == 0) {
				$userId = $user->id;
			}

			if ($user->id != $userId && !$this->userRoles->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR)) {
				$ret->setAsError("Invalid profile identifier");

				return $ret;
			}

			$ret->setData($this->userRels->getRelations($userId));

			return $ret;
		}

		/**
		 * Attempts to retrieve a user's settings, only works for current user and administrators.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function getSettings(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();
			$userId = $params->getInt('id', 0);

			if ($userId == 0) {
				$userId = $user->id;
			}

			if ($user->id != $userId && !$this->userRoles->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR)) {
				$ret->setAsError("Invalid profile identifier");

				return $ret;
			}

			$ret->setData(UserSettings::fromUser($userId, $this->db, $this->log));

			return $ret;
		}

		/**
		 * Attempts to log the user into the system, returning either error information or the user ID and token.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException
		 * @return Response
		 */
		public function login(Request $request, array $matches = null) : Response {
			$ret = new Response(HttpStatusCodes::OK);
			$this->processEvent($ret, 'doLogin', $request->getInput());

			return $ret;
		}

		/**
		 * Attempts to log the current user out, returning error information or a success message.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException
		 * @return Response
		 */
		public function logout(Request $request, array $matches = null) : Response {
			$ret = $this->newResponse();
			$this->processEvent($ret, 'doLogout', $request->getInput());

			return $ret;
		}

		/**
		 * Attempts to process a UserEvents event and assign results to the Response object.
		 *
		 * @param Response $ret Response object for request.
		 * @param string $event Name of UserEvents method to call.
		 * @param ParameterHelper $params ParameterHelper object to supply to UserEvents method.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException
		 * @return void
		 */
		protected function processEvent(Response &$ret, string $event, ParameterHelper $params) : void {
			$evt = $this->events->$event($params);

			$ret->setStatus($evt->getResults()[0][UserEvents::STR_HTTP_CODE]);

			if ($evt->isBad()) {
				$ret->setData($evt->getMessages()[0]);
			} else {
				$ret->setData($evt->getResults()[0][UserEvents::STR_DATA]);
			}

			return;
		}

		/**
		 * Registers the controller endpoints.
		 *
		 * @return void
		 */
		protected function registerEndpoints() : void {
			$this->registerEndpoint('GET',  '/^Account\/CheckEmail\/?/i',        'checkEmail',        null);
			$this->registerEndpoint('POST', '/^Account\/CheckToken\/?/i',        'checkToken',        null);
			$this->registerEndpoint('POST', '/^Account\/Create\/?/i',            'createUser',        RoleStrings::ADMINISTRATOR);
			$this->registerEndpoint('POST', '/^Account\/Delete\/?/i',            'deleteUser',        true);
			$this->registerEndpoint('POST', '/^Account\/Login\/?/i',             'login',             null);
			$this->registerEndpoint('POST', '/^Account\/Logout\/?/i',            'logout',            true);
			$this->registerEndpoint('GET',  '/^Account\/Profile\/?/i',           'getProfile',        true);
			$this->registerEndpoint('POST', '/^Account\/Register\/?/i',          'registerUser',      null);
			$this->registerEndpoint('GET',  '/^Account\/Relations\/?/i',         'getRelations',      true);
			$this->registerEndpoint('GET',  '/^Account\/RelatedTo\/?/i',         'relatedTo',         true);
			$this->registerEndpoint('POST', '/^Account\/RemoveRelation\/?/i',    'removeRelation',    true);
			$this->registerEndpoint('POST', '/^Account\/ResetPassword\/?/i',     'resetPassword',     false);
			$this->registerEndpoint('POST', '/^Account\/SendPasswordReset\/?/i', 'sendPasswordReset', false);
			$this->registerEndpoint('GET',  '/^Account\/Settings\/?/i',          'getSettings',       true);
			$this->registerEndpoint('POST', '/^Account\/SetRelation\/?/i',       'setRelation',       true);
			$this->registerEndpoint('GET',  '/^Account\/?/i',                    'get',               true);

			return;
		}

		/**
		 * Attempts to register a user with the system.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException
		 * @return Response
		 */
		public function registerUser(Request $request, array $matches = null) : Response {
			$ret = $this->newResponse();
			$this->processEvent($ret, 'doRegister', $request->getInput());

			return $ret;
		}

		/**
		 * Determines if the user is related to the given identifier.
		 *
		 * @param \Stoic\Web\Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function relatedTo(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();

			if (!$params->has('id')) {
				$ret->setAsError('Invalid parameters supplied for request');

				return $ret;
			}

			$ret->setData((new UserRelations($this->db, $this->log))->areRelated($user->id, $params->getInt('id')));

			return $ret;
		}

		/**
		 * Attempts to remove a relationship between the authenticated user and another.
		 *
		 * @param \Stoic\Web\Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function removeRelation(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();

			if (!$params->has('id')) {
				$ret->setAsError('Invalid parameters supplied');

				return $ret;
			}

			$ret->setData((new UserRelations($this->db, $this->log))->deleteRelation($user->id, $params->getInt('id')));

			return $ret;
		}

		/**
		 * Attempts to reset the user's password.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException
		 * @return Response
		 */
		public function resetPassword(Request $request, array $matches = null) : Response {
			$ret = $this->newResponse();
			$this->processEvent($ret, 'doResetPassword', $request->getInput());

			return $ret;
		}

		/**
		 * Attempts to send the user a password reset token.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function sendPasswordReset(Request $request, array $matches = null) : Response {
			global $Settings;

			$ret    = $this->newResponse();
			$params = $request->getInput();

			if (!$params->has('email')) {
				$ret->setAsError('Invalid parameters supplied for request');

				return $ret;
			}

			if (!sendResetEmail($params->getString('email'), PageHelper::getPage('api/1/index.php'), $Settings, $this->db, $this->log)) {
				$ret->setAsError("Failed to send reset email, check spelling and try again");

				return $ret;
			}

			$ret->setData(true);

			return $ret;
		}

		/**
		 * Sets the stage of relationship between two users.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Stoic\Web\Resources\InvalidRequestException|\Stoic\Web\Resources\NonJsonInputException|\ReflectionException|\Exception
		 * @return Response
		 */
		public function setRelation(Request $request, array $matches = null) : Response {
			$user   = $this->getUser();
			$ret    = $this->newResponse();
			$params = $request->getInput();
			$rels   = new UserRelations($this->db, $this->log);

			if (!$params->hasAll('id', 'stage')) {
				$ret->setAsError('Invalid parameters supplied');

				return $ret;
			}

			$ret->setData($rels->changeStage($user->id, $params->getInt('id'), $params->getInt('stage')));

			return $ret;
		}
	}
