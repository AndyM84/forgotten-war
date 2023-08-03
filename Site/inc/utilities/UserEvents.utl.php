<?php

	namespace Zibings;

	use Stoic\Chain\ChainHelper;
	use Stoic\Chain\DispatchBase;
	use Stoic\Chain\NodeBase;
	use Stoic\Log\Logger;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbClass;
	use Stoic\Utilities\EnumBase;
	use Stoic\Utilities\ParameterHelper;
	use Stoic\Utilities\ReturnHelper;
	use Stoic\Web\Resources\HttpStatusCodes;
	use Stoic\Web\Resources\ServerIndices as SI;

	/**
	 * Dispatch used for confirm event.
	 *
	 * @package Zibings
	 */
	class UserEventConfirmDispatch extends DispatchBase {
		/**
		 * Instantiates a new UserEventConfirmDispatch object.
		 *
		 * @param User $user User object for reference.
		 * @param PdoHelper $db PdoHelper object for reference.
		 * @param Logger $log Logger object for reference.
		 * @throws \Exception
		 * @return void
		 */
		public function __construct(
			public User      $user,
			public PdoHelper $db,
			public Logger    $log) {
			$this->makeValid();

			return;
		}

		/**
		 * Basic initialization method, unused by UserEvents dispatches.
		 *
		 * @param mixed $input Input used for initialization.
		 * @return void
		 */
		public function initialize(mixed $input) : void {
			return;
		}
	}

	/**
	 * Dispatch used for creation event.
	 *
	 * @package Zibings
	 */
	class UserEventCreateDispatch extends DispatchBase {
		/**
		 * Instantiates a new UserEventCreateDispatch object.
		 *
		 * @param User $user User object for reference.
		 * @param PdoHelper $db PdoHelper object for reference.
		 * @param Logger $log Logger object for reference.
		 * @throws \Exception
		 * @return void
		 */
		public function __construct(
			public User      $user,
			public PdoHelper $db,
			public Logger    $log) {
			$this->makeValid();

			return;
		}

		/**
		 * Basic initialization method, unused by UserEvents dispatches.
		 *
		 * @param mixed $input Input used for initialization.
		 * @return void
		 */
		public function initialize(mixed $input) : void {
			return;
		}
	}

	/**
	 * Dispatch used for deletion event.
	 *
	 * @package Zibings
	 */
	class UserEventDeleteDispatch extends DispatchBase {
		/**
		 * Instantiates a new UserEventDeleteDispatch object.
		 *
		 * @param User $user User object for reference.
		 * @param PdoHelper $db PdoHelper object for reference.
		 * @param Logger $log Logger object for reference.
		 * @throws \Exception
		 * @return void
		 */
		public function __construct(
			public User      $user,
			public PdoHelper $db,
			public Logger    $log) {
			$this->makeValid();

			return;
		}

		/**
		 * Basic initialization method, unused by UserEvents dispatches.
		 *
		 * @param mixed $input Input used for initialization.
		 * @return void
		 */
		public function initialize(mixed $input) : void {
			return;
		}
	}

	/**
	 * Dispatch used for login event.
	 *
	 * @package Zibings
	 */
	class UserEventLoginDispatch extends DispatchBase {
		/**
		 * Instantiates a new UserEventLoginDispatch object.
		 *
		 * @param User $user User object for reference.
		 * @param string $token Generated session token for user.
		 * @param PdoHelper $db PdoHelper object for reference.
		 * @param Logger $log Logger object for reference.
		 * @throws \Exception
		 * @return void
		 */
		public function __construct(
			public User      $user,
			public string    $token,
			public PdoHelper $db,
			public Logger    $log) {
			$this->makeValid();

			return;
		}

		/**
		 * Basic initialization method, unused by UserEvents dispatches.
		 *
		 * @param mixed $input Input used for initialization.
		 * @return void
		 */
		public function initialize(mixed $input) : void {
			return;
		}
	}

	/**
	 * Dispatch used for logout event.
	 *
	 * @package Zibings
	 */
	class UserEventLogoutDispatch extends DispatchBase {
		/**
		 * Instantiates a new UserEventLogoutDispatch object.
		 *
		 * @param UserSession $session UserSession object for reference.
		 * @param PdoHelper $db PdoHelper object for reference.
		 * @param Logger $log Logger object for reference.
		 * @throws \Exception
		 * @return void
		 */
		public function __construct(
			public UserSession $session,
			public PdoHelper   $db,
			public Logger      $log) {
			$this->makeValid();

			return;
		}

		/**
		 * Basic initialization method, unused by UserEvents dispatches.
		 *
		 * @param mixed $input Input used for initialization.
		 * @return void
		 */
		public function initialize(mixed $input) : void {
			return;
		}
	}

	/**
	 * Dispatch used for registration event.
	 *
	 * @package Zibings
	 */
	class UserEventRegisterDispatch extends DispatchBase {
		/**
		 * Instantiates a new UserEventRegisterDispatch object.
		 *
		 * @param User $user User object for reference.
		 * @param PdoHelper $db PdoHelper object for reference.
		 * @param Logger $log Logger object for reference.
		 * @throws \Exception
		 * @return void
		 */
		public function __construct(
			public User      $user,
			public PdoHelper $db,
			public Logger    $log) {
			$this->makeValid();

			return;
		}

		/**
		 * Basic initialization method, unused by UserEvents dispatches.
		 *
		 * @param mixed $input Input used for initialization.
		 * @return void
		 */
		public function initialize(mixed $input) : void {
			return;
		}
	}

	/**
	 * Dispatch used for password reset event.
	 *
	 * @package Zibings
	 */
	class UserEventResetPasswordDispatch extends DispatchBase {
		/**
		 * Instantiates a new UserEventResetPasswordDispatch object.
		 *
		 * @param User $user User object for reference.
		 * @param PdoHelper $db PdoHelper object for reference.
		 * @param Logger $log Logger object for reference.
		 * @throws \Exception
		 * @return void
		 */
		public function __construct(
			public User      $user,
			public PdoHelper $db,
			public Logger    $log) {
			$this->makeValid();

			return;
		}

		/**
		 * Basic initialization method, unused by UserEvents dispatches.
		 *
		 * @param mixed $input Input used for initialization.
		 * @return void
		 */
		public function initialize(mixed $input) : void {
			return;
		}
	}

	/**
	 * Dispatch used for update event.
	 *
	 * @package Zibings
	 */
	class UserEventUpdateDispatch extends DispatchBase {
		/**
		 * Instantiates a new UserEventUpdateDispatch object.
		 *
		 * @param User $user User object for reference.
		 * @param ParameterHelper $params Parameters provided for user update.
		 * @param PdoHelper $db PdoHelper object for reference.
		 * @param Logger $log Logger object for reference.
		 * @param bool $emailUpdated Optional toggle to show if the user's email was updated.
		 * @throws \Exception
		 * @return void
		 */
		public function __construct(
			public User            $user,
			public ParameterHelper $params,
			public PdoHelper       $db,
			public Logger          $log,
			public bool            $emailUpdated = false) {
			$this->makeValid();

			return;
		}

		/**
		 * Basic initialization method, unused by UserEvents dispatches.
		 *
		 * @param mixed $input Input used for initialization.
		 * @return void
		 */
		public function initialize(mixed $input) : void {
			return;
		}
	}

	/**
	 * Available types of user events.
	 *
	 * @package Zibings
	 */
	class UserEventTypes extends EnumBase {
		const CONFIRM       = 1;
		const CREATE        = 2;
		const DELETE        = 3;
		const LOGIN         = 4;
		const LOGOUT        = 5;
		const REGISTER      = 6;
		const RESETPASSWORD = 7;
		const UPDATE        = 8;
	}

	/**
	 * Utility class that provides the ability to subscribe to major user events.
	 *
	 * @package Zibings
	 */
	class UserEvents extends StoicDbClass {
		const STR_ACTOR          = 'actor';
		const STR_BEARER         = 'bearer';
		const STR_BIRTHDAY       = 'birthday';
		const STR_CONFIRM_EMAIL  = 'confirmEmail';
		const STR_CONFIRM_KEY    = 'confirmKey';
		const STR_DATA           = 'data';
		const STR_DESCRIPTION    = 'description';
		const STR_DISPLAY_NAME   = 'displayName';
		const STR_EMAIL          = 'email';
		const STR_EMAILCONFIRMED = 'emailConfirmed';
		const STR_GENDER         = 'gender';
		const STR_HTML_EMAILS    = 'htmlEmails';
		const STR_HTTP_CODE      = 'httpCode';
		const STR_ID             = 'id';
		const STR_KEY            = 'key';
		const STR_OLD_KEY        = 'oldKey';
		const STR_PLAY_SOUNDS    = 'playSounds';
		const STR_PROFILE        = 'profile';
		const STR_PROVIDER       = 'provider';
		const STR_REAL_NAME      = 'realName';
		const STR_SEARCHES       = 'searches';
		const STR_SESSION_USERID = 'zUserID';
		const STR_SESSION_TOKEN  = 'zToken';
		const STR_SETTINGS       = 'settings';
		const STR_TOKEN          = 'token';
		const STR_USERID         = 'userId';
		const STR_VISIBILITIES   = 'visibilities';


		/**
		 * Collection of event chains.
		 *
		 * @var ChainHelper[]
		 */
		protected array $events = [
			UserEventTypes::CONFIRM       => null,
			UserEventTypes::CREATE        => null,
			UserEventTypes::DELETE        => null,
			UserEventTypes::LOGIN         => null,
			UserEventTypes::LOGOUT        => null,
			UserEventTypes::REGISTER      => null,
			UserEventTypes::RESETPASSWORD => null,
			UserEventTypes::UPDATE        => null
		];


		/**
		 * Instantiates a new UserEvents object.
		 *
		 * @param \PDO $db PDO instance for use by object.
		 * @param null|Logger $log Logger instance for use by object, defaults to new instance.
		 */
		public function __construct(\PDO $db, Logger $log = null) {
			parent::__construct($db, $log);

			foreach (array_keys($this->events) as $evt) {
				$this->events[$evt] = new ChainHelper();
			}

			return;
		}

		/**
		 * Helper method to assign an error to the given ReturnHelper, log the error, and optionally assign an HTTP status code.
		 *
		 * @param ReturnHelper $ret ReturnHelper to assign error message to for reference.
		 * @param string $error Error message to reference in ReturnHelper and logs.
		 * @param int|HttpStatusCodes $status Optional HTTP status code, defaults to INTERNAL_SERVER_ERROR if not supplied.
		 * @throws \ReflectionException
		 * @return void
		 */
		protected function assignError(ReturnHelper &$ret, string $error, int|HttpStatusCodes $status = HttpStatusCodes::INTERNAL_SERVER_ERROR) : void {
			$code = HttpStatusCodes::tryGet($status);

			if ($code->getValue() === null) {
				return;
			}

			$ret->addMessage($error);
			$this->log->error($error);
			$ret->addResult([self::STR_HTTP_CODE => $code->getValue()]);

			return;
		}

		/**
		 * Performs email confirmation. If completed successfully, the UserEventTypes::CONFIRM chain is traversed with a new
		 * UserEventsConfirmDispatch object. The following parameters are required:
		 *
		 * [
		 *   'token' => (string) 'some-token' # encoded token and userId combo
		 * ]
		 *
		 * Resulting ReturnHelper will include a suggested HTTP status code in the 'httpCode' index.
		 *
		 * @param ParameterHelper $params Parameters provided to perform the event.
		 * @throws \ReflectionException|\Exception
		 * @return ReturnHelper
		 */
		public function doConfirm(ParameterHelper $params) : ReturnHelper {
			$ret = new ReturnHelper();

			if (!$params->has(self::STR_TOKEN)) {
				$this->assignError($ret, "Missing parameters for confirmation");

				return $ret;
			}

			$tok = explode(':', base64_decode($params->getString(self::STR_TOKEN)));
			$token = UserToken::fromToken($tok[1], intval($tok[0]), $this->db, $this->log);

			if ($token->userId < 1) {
				$this->assignError($ret, "Invalid confirmation supplied");

				return $ret;
			}

			$user = User::fromId($token->userId, $this->db, $this->log);

			if ($user->id < 1 || $user->emailConfirmed === true) {
				$this->assignError($ret, "Invalid user supplied");

				return $ret;
			}

			$user->emailConfirmed = true;
			$user->update();

			$token->delete();

			$this->touchEvent(UserEventTypes::CONFIRM, new UserEventConfirmDispatch($user, $this->db, $this->log));

			$ret->makeGood();
			$ret->addResult([self::STR_HTTP_CODE => HttpStatusCodes::OK]);

			return $ret;
		}

		/**
		 * Performs user creation. If completed successfully, the UserEventTypes::CREATE chain is traversed with a new
		 * UserEventsCreateDispatch object. The following parameters are required:
		 *
		 * [
		 *   'email'          => (string) 'user@domain.com', # the email address for the new user
		 *   'key'            => (string) 'someKey',         # the login key value for the new user
		 *   'confirmKey'     => (string) 'someKey',         # repeat of the login key value
		 *   'provider'       => (int|LoginKeyProviders) 1,  # the login key provider type
		 *   'emailConfirmed' => (bool) false                # whether the new user's email is confirmed
		 * ]
		 *
		 * After the User and their LoginKey have been created, the system will attempt to create (without guaranteeing) the
		 * following entries for the user before traversing the chain:
		 *
		 *   - UserProfile
		 *   - UserSettings
		 *   - UserVisibilities
		 * 
		 * Resulting ReturnHelper will include a suggested HTTP status code in the 'httpCode' index and the user object if the
		 * operation was successful:
		 *
		 * [
		 *   'httpCode' => 0,     # suggested HTTP status code
		 *   'data'     => User{} # User object with created user data
		 * ]
		 *
		 * @param ParameterHelper $params Parameters provided to perform the event.
		 * @throws \ReflectionException
		 * @return ReturnHelper
		 */
		public function doCreate(ParameterHelper $params) : ReturnHelper {
			$ret = new ReturnHelper();

			if (!$params->hasAll(self::STR_EMAIL, self::STR_KEY, self::STR_CONFIRM_KEY, self::STR_PROVIDER, self::STR_EMAILCONFIRMED)) {
				$this->assignError($ret, "Missing parameters for account creation");

				return $ret;
			}

			$key            = $params->getString(self::STR_KEY);
			$email          = $params->getString(self::STR_EMAIL);
			$provider       = $params->getInt(self::STR_PROVIDER);
			$confirmKey     = $params->getString(self::STR_CONFIRM_KEY);
			$emailConfirmed = $params->getBool(self::STR_EMAILCONFIRMED, false);

			if ($key !== $confirmKey || empty($key)) {
				$this->assignError($ret, "Invalid parameters for account creation");

				return $ret;
			}

			$user = new User($this->db, $this->log);

			try {
				$user->email          = $email;
				$user->emailConfirmed = $emailConfirmed;
				$user->joined         = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
				$create               = $user->create();

				if ($create->isBad()) {
					$ret->addMessage("Error creating user account");

					if ($create->hasMessages()) {
						$ret->addMessages($create->getMessages());
					}

					$ret->addResult([self::STR_HTTP_CODE => HttpStatusCodes::INTERNAL_SERVER_ERROR]);

					return $ret;
				}

				$login           = new LoginKey($this->db, $this->log);
				$login->userId   = $user->id;
				$login->provider = new LoginKeyProviders($provider);
				$login->key      = ($login->provider->is(LoginKeyProviders::PASSWORD)) ? password_hash($key, PASSWORD_DEFAULT) : $key;
				$create          = $login->create();

				if ($create->isBad()) {
					$ret->addMessage("Failed to create user login key, removing new user account");

					if ($create->hasMessages()) {
						$ret->addMessages($create->getMessages());
					}

					$user->delete();
					$ret->addResult([self::STR_HTTP_CODE => HttpStatusCodes::INTERNAL_SERVER_ERROR]);

					return $ret;
				}

				$profile = new UserProfile($this->db, $this->log);
				$profile->userId = $user->id;
				$profile->create();

				$settings = new UserSettings($this->db, $this->log);
				$settings->userId = $user->id;
				$settings->create();

				$visibilities = new UserVisibilities($this->db, $this->log);
				$visibilities->userId = $user->id;
				$visibilities->create();

				$this->touchEvent(UserEventTypes::CREATE, new UserEventCreateDispatch($user, $this->db, $this->log));

				$ret->makeGood();
				$ret->addResult([
					self::STR_HTTP_CODE => HttpStatusCodes::OK,
					self::STR_DATA      => $user
				]);
			} catch (\Exception $ex) {
				$this->assignError($ret, "Error while creating user account: " . $ex->getMessage());
			}

			return $ret;
		}

		/**
		 * Performs user deletion. If completed successfully, the UserEventTypes::DELETE chain is traversed with a new
		 * UserEventsDeleteDispatch object. The following parameters are required:
		 *
		 * [
		 *   'id'    => 1, # identifier for user being deleted
		 *   'actor' => 2  # identifier of the user performing the deletion
		 * ]
		 *
		 * If the 'actor' is the user (the user is deleting their own account), set the value of 'actor' to 0.
		 *
		 * The DELETE event will be called before the user is deleted to allow for cleanup before final user deletion and to respect
		 * foreign key constraints. After the hook has been called and returned, the system will clean up the following tables in
		 * order:
		 *
		 *   - UserVisibilities
		 *   - UserToken
		 *   - UserSettings
		 *   - UserSession
		 *   - UserRole
		 *   - UserRelation
		 *   - UserProfile
		 *   - UserDevice
		 *   - UserCustomVisibility
		 *   - UserContact
		 *   - LoginKey
		 *   - User
		 *
		 * Resulting ReturnHelper will include a suggested HTTP status code in the 'httpCode' index.
		 *
		 * @param ParameterHelper $params Parameters for performing operation.
		 * @throws \ReflectionException|\Exception
		 * @return ReturnHelper
		 */
		public function doDelete(ParameterHelper $params) : ReturnHelper {
			$ret = new ReturnHelper();

			if (!$params->hasAll(self::STR_ID, self::STR_ACTOR)) {
				$this->assignError($ret, "Failed to delete user, incomplete parameters");

				return $ret;
			}

			$id    = $params->getInt(self::STR_ID);
			$actor = $params->getInt(self::STR_ACTOR);

			if ($id === $actor) {
				$this->assignError($ret, "Failed to delete user, can't delete yourself");

				return $ret;
			}

			if ($actor > 0 && !(new UserRoles($this->db, $this->log))->userInRoleByName($actor, RoleStrings::ADMINISTRATOR)) {
				$this->assignError($ret, "Failed to delete user, only admins can delete other users");

				return $ret;
			}

			$user = User::fromId($id, $this->db, $this->log);

			if ($user->id < 1) {
				$this->assignError($ret, "Failed to delete user, invalid information provided");

				return $ret;
			}

			$this->touchEvent(UserEventTypes::DELETE, new UserEventDeleteDispatch($user, $this->db, $this->log));

			(new UserVisibilitiesRepo($this->db, $this->log))->deleteAllForUser($user->id);
			(new UserTokens($this->db, $this->log))->deleteAllForUser($user->id);
			(new UserSettingsRepo($this->db, $this->log))->deleteAllForUser($user->id);
			(new UserSessions($this->db, $this->log))->deleteAllForUser($user->id);
			(new UserRoles($this->db, $this->log))->deleteAllForUser($user->id);
			(new UserRelations($this->db, $this->log))->deleteAllForUser($user->id);
			(new UserProfiles($this->db, $this->log))->deleteAllForUser($user->id);
			(new UserDevices($this->db, $this->log))->deleteAllForUser($user->id);
			(new UserContacts($this->db, $this->log))->deleteAllForUser($user->id);
			(new LoginKeys($this->db, $this->log))->deleteAllForUser($user->id);

			if ($user->delete()->isGood()) {
				$ret->makeGood();
			}

			$ret->addResult([
				self::STR_HTTP_CODE => HttpStatusCodes::OK
			]);

			return $ret;
		}

		/**
		 * Performs user authentication. If completed successfully, the UserEventTypes::LOGIN chain is traversed with a new
		 * UserEventLoginDispatch object. The following parameters are required:
		 *
		 * [
		 *   'email'    => (string) 'user@domain.com', # the email address of the user in question
		 *   'key'      => (string) 'someKey',         # the login key value of the user in question
		 *   'provider' => (int|LoginKeyProviders) 1   # the login key provider type
		 * ]
		 *
		 * Resulting ReturnHelper will include a suggested HTTP status code in the 'httpCode' index and the session data if the
		 * operation was successful:
		 *
		 * [
		 *   'httpCode' => (int) 0,
		 *   'data'     => [
		 *     'userId' => (int) 0,     # the authenticated user's identifier
		 *     'token'  => (string) '', # the new session token generated for the user
		 *     'bearer' => (string) ''  # the bearer token to use in headers
		 *   ]
		 * ]
		 *
		 * @param ParameterHelper $params Parameters provided to perform the event.
		 * @throws \ReflectionException|\Exception
		 * @return ReturnHelper
		 */
		public function doLogin(ParameterHelper $params) : ReturnHelper {
			$ret = new ReturnHelper();

			if (!$params->hasAll(self::STR_EMAIL, self::STR_KEY, self::STR_PROVIDER)) {
				$this->assignError($ret, "Missing parameters for authorization");

				return $ret;
			}

			$email    = $params->getString(self::STR_EMAIL);
			$key      = $params->getString(self::STR_KEY);
			$provider = $params->getInt(self::STR_PROVIDER);
			$user     = User::fromEmail($email, $this->db, $this->log);

			if ($user->id < 1) {
				$this->assignError($ret, "Invalid credentials supplied");

				return $ret;
			}

			if (!$user->emailConfirmed) {
				$this->assignError($ret, "Cannot login without confirming your email");

				return $ret;
			}

			$login = LoginKey::fromUserAndProvider($user->id, $provider, $this->db, $this->log);

			if ($login->userId < 1) {
				$this->assignError($ret, "No login available for user");

				return $ret;
			}

			$challengePassed = false;

			switch ($login->provider->getValue()) {
				case LoginKeyProviders::PASSWORD:
					if (!password_verify($key, $login->key)) {
						$this->assignError($ret, "Invalid credentials provided");
						UserAuthHistory::createFromUserId($user->id, AuthHistoryActions::LOGIN, new ParameterHelper($_SERVER), "Failed login action from event system (bad password)", $this->db, $this->log);

						break;
					}

					if (password_needs_rehash($login->key, PASSWORD_DEFAULT)) {
						$login->key = password_hash($key, PASSWORD_DEFAULT);
						$login->update();
					}

					$challengePassed = true;

					break;
				case LoginKeyProviders::FACEBOOK:
				case LoginKeyProviders::TWITTER:
				case LoginKeyProviders::TWITCH:
				case LoginKeyProviders::GITHUB:
				case LoginKeyProviders::REDDIT:
					if ($key !== $login->key) {
						$this->assignError($ret, "Invalid credentials provided");
						UserAuthHistory::createFromUserId($user->id, AuthHistoryActions::LOGIN, new ParameterHelper($_SERVER), "Failed login action from event system (wrong provider)", $this->db, $this->log);

						break;
					}

					$challengePassed = true;

					break;
				default:
					break;
			}

			if (!$challengePassed) {
				$this->assignError($ret, "Failed to validate credentials");

				return $ret;
			}

			$user->lastLogin = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));

			if ($user->update()->isBad()) {
				$this->log->warning("Failed to update last login time for user '{$email}'");
			}

			if (array_key_exists(SI::REMOTE_ADDR, $_SERVER) === false) {
				$_SERVER[SI::REMOTE_ADDR] = '::1';
			}

			$session           = new UserSession($this->db, $this->log);
			$session->address  = $_SERVER[SI::REMOTE_ADDR];
			$session->hostname = gethostbyaddr($session->address);
			$session->token    = UserSession::generateGuid(false);
			$session->userId   = $user->id;
			$sCreate           = $session->create();

			if ($sCreate->isBad()) {
				if ($sCreate->hasMessages()) {
					foreach ($sCreate->getMessages() as $msg) {
						$this->log->error($msg);
					}
				} else {
					$this->log->error("Failed to create user session after authenticating");
					UserAuthHistory::createFromUserId($user->id, AuthHistoryActions::LOGIN, new ParameterHelper($_SERVER), "Failed login action from event system (failed session init)", $this->db, $this->log);
				}

				$ret->addMessage("Failed to authenticate user");
				$ret->addResult([self::STR_HTTP_CODE => HttpStatusCodes::INTERNAL_SERVER_ERROR]);

				return $ret;
			}

			if (!defined('STOIC_DISABLE_SESSION')) {
				$_SESSION[self::STR_SESSION_USERID] = $user->id;
				$_SESSION[self::STR_SESSION_TOKEN]  = $session->token;
			}

			$ret->makeGood();
			$ret->addResult([
				self::STR_HTTP_CODE => HttpStatusCodes::OK,
				self::STR_DATA      => [
					self::STR_USERID => $user->id,
					self::STR_TOKEN  => $session->token,
					self::STR_BEARER => base64_encode("{$user->id}:{$session->token}")
				]
			]);

			$this->touchEvent(UserEventTypes::LOGIN, new UserEventLoginDispatch($user, $session->token, $this->db, $this->log));

			return $ret;
		}

		/**
		 * Performs user logout. If completed successfully, the UserEventTypes::LOGOUT chain is traversed with a new
		 * UserEventLogoutDispatch object. The following parameters are optional:
		 *
		 * [
		 *   'userId' => (int) 1,              # user identifier
		 *   'token'  => (string) 'some-token' # user session token
		 * ]
		 *
		 * If the optional parameters are not included, the system will attempt to find the active user session and logout that
		 * session out.
		 *
		 * Resulting ReturnHelper will include a suggested HTTP status code in the 'httpCode' index and the invalidated session data
		 * if the operation was successful:
		 *
		 * [
		 *   'httpCode' => (int) 0,
		 *   'data'     => [
		 *     'userId' => (int) 0,    # the newly-logged-out user's identifier
		 *     'token'  => (string) '' # the session token invalidated for the user
		 *   ]
		 * ]
		 *
		 * @param ParameterHelper $params Parameters for performing operation.
		 * @throws \ReflectionException|\Exception
		 * @return ReturnHelper
		 */
		public function doLogout(ParameterHelper $params) : ReturnHelper {
			$ret         = new ReturnHelper();
			$userId      = null;
			$token       = null;
			$userSession = new UserSession($this->db, $this->log);

			if ($params->hasAll(self::STR_USERID, self::STR_TOKEN)) {
				$userId      = $params->getInt(self::STR_USERID);
				$token       = $params->getString(self::STR_TOKEN);
			} else {
				$session = new ParameterHelper($_SESSION);
				$userId  = $session->getInt(self::STR_SESSION_USERID);
				$token   = $session->getString(self::STR_SESSION_TOKEN);

				if (!defined('STOIC_DISABLE_SESSION')) {
					if ($session->has(self::STR_SESSION_USERID)) {
						unset($_SESSION[self::STR_SESSION_USERID]);
					}

					if ($session->has(self::STR_SESSION_TOKEN)) {
						unset($_SESSION[self::STR_SESSION_TOKEN]);
					}
				}
			}

			if ($userId === null || $token === null) {
				$this->assignError($ret, "Invalid session information");

				return $ret;
			}

			$userSession = UserSession::fromToken($token, $this->db, $this->log);

			if ($userSession->userId != $userId) {
				$this->assignError($ret, "Invalid session identifier");

				return $ret;
			}

			$delete = $userSession->delete();

			if ($delete->isBad()) {
				if ($delete->hasMessages()) {
					$this->assignError($ret, $delete->getMessages()[0]);
				} else {
					$this->assignError($ret, "Failed to delete session");
				}

				return $ret;
			}

			$this->touchEvent(UserEventTypes::LOGOUT, new UserEventLogoutDispatch($userSession, $this->db, $this->log));

			$ret->makeGood();
			$ret->addResult([
				self::STR_HTTP_CODE => HttpStatusCodes::OK,
				self::STR_DATA      => [
					self::STR_USERID  => $userSession->userId,
					self::STR_TOKEN   => $userSession->token
				]
			]);

			return $ret;
		}

		/**
		 * Performs user registration. If completed successfully, the UserEventTypes::CREATE chain is traversed with a new
		 * UserEventsRegisterDispatch object. The following parameters are required:
		 *
		 * [
		 *   'email'          => (string) 'user@domain.com', # the email address for the new user
		 *   'key'            => (string) 'someKey',         # the login key value for the new user
		 *   'confirmKey'     => (string) 'someKey',         # confirm the login key value for the new user
		 *   'provider'       => (int|LoginKeyProviders) 1   # the login key provider type
		 * ]
		 *
		 * After the User and their LoginKey have been created, the system will attempt to create (without guaranteeing) the
		 * following entries for the user before traversing the chain:
		 *
		 *   - UserProfile
		 *   - UserSettings
		 *   - UserVisibilities
		 *
		 * NOTE: This event does NOT automatically send any emails for confirmation.
		 * 
		 * Resulting ReturnHelper will include a suggested HTTP status code in the 'httpCode' index and the user object if the
		 * operation was successful:
		 *
		 * [
		 *   'httpCode' => 0,     # suggested HTTP status code
		 *   'data'     => User{} # User object with created user data
		 * ]
		 *
		 * @param ParameterHelper $params Parameters provided to perform the event.
		 * @throws \ReflectionException
		 * @return ReturnHelper
		 */
		public function doRegister(ParameterHelper $params) : ReturnHelper {
			$ret = new ReturnHelper();

			if (!$params->hasAll(self::STR_EMAIL, self::STR_KEY, self::STR_CONFIRM_KEY, self::STR_PROVIDER)) {
				$this->assignError($ret, "Missing parameters for account creation");

				return $ret;
			}

			$key            = $params->getString(self::STR_KEY);
			$email          = $params->getString(self::STR_EMAIL);
			$provider       = $params->getInt(self::STR_PROVIDER);
			$confirmKey     = $params->getString(self::STR_CONFIRM_KEY);

			if ($key !== $confirmKey || empty($key)) {
				$this->assignError($ret, "Invalid parameters for account creation");

				return $ret;
			}

			$user = new User($this->db, $this->log);

			try {
				$user->email          = $email;
				$user->emailConfirmed = false;
				$user->joined         = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
				$create               = $user->create();

				if ($create->isBad()) {
					$ret->addMessage("Error creating user account");

					if ($create->hasMessages()) {
						$ret->addMessages($create->getMessages());
					}

					$ret->addResult([self::STR_HTTP_CODE => HttpStatusCodes::INTERNAL_SERVER_ERROR]);

					return $ret;
				}

				$login           = new LoginKey($this->db, $this->log);
				$login->userId   = $user->id;
				$login->provider = new LoginKeyProviders($provider);
				$login->key      = ($login->provider->is(LoginKeyProviders::PASSWORD)) ? password_hash($key, PASSWORD_DEFAULT) : $key;
				$create          = $login->create();

				if ($create->isBad()) {
					$ret->addMessage("Failed to create user login key, removing new user account");

					if ($create->hasMessages()) {
						$ret->addMessages($create->getMessages());
					}

					$user->delete();
					$ret->addResult([self::STR_HTTP_CODE => HttpStatusCodes::INTERNAL_SERVER_ERROR]);

					return $ret;
				}

				$profile = new UserProfile($this->db, $this->log);
				$profile->userId = $user->id;
				$profile->create();

				$settings = new UserSettings($this->db, $this->log);
				$settings->userId = $user->id;
				$settings->create();

				$visibilities = new UserVisibilities($this->db, $this->log);
				$visibilities->userId = $user->id;
				$visibilities->create();

				$this->touchEvent(UserEventTypes::REGISTER, new UserEventRegisterDispatch($user, $this->db, $this->log));

				$ret->makeGood();
				$ret->addResult([
					self::STR_HTTP_CODE => HttpStatusCodes::OK,
					self::STR_DATA      => $user
				]);
			} catch (\Exception $ex) {
				$this->assignError($ret, "Error while creating user account: " . $ex->getMessage());
			}

			return $ret;
		}

		/**
		 * Performs user password reset. If completed successfully, the UserEventTypes::RESETPASSWORD chain is traversed with a new
		 * UserEventResetPasswordDispatch object. The following parameters are required:
		 *
		 * [
		 *   'id'         => (int) 1,            # user identifier
		 *   'key'        => (string) 'someKey', # new password
		 *   'confirmKey' => (string) 'someKey'  # confirmation of new password
		 * ]
		 *
		 * This method makes NO checks against the new password's complexity.
		 *
		 * Resulting ReturnHelper will include a suggested HTTP status code in the 'httpCode' index.
		 *
		 * @param ParameterHelper $params Parameters provided to perform the event.
		 * @throws \ReflectionException
		 * @return ReturnHelper
		 */
		public function doResetPassword(ParameterHelper $params) : ReturnHelper {
			$ret = new ReturnHelper();

			if (!$params->hasAll(self::STR_ID, self::STR_KEY, self::STR_CONFIRM_KEY)) {
				$this->assignError($ret, "Missing parameters for reset");

				return $ret;
			}

			$id         = $params->getInt(self::STR_ID);
			$key        = $params->getString(self::STR_KEY);
			$confirmKey = $params->getString(self::STR_CONFIRM_KEY);

			if ($key !== $confirmKey) {
				$this->assignError($ret, "Invalid keys provided");

				return $ret;
			}

			$user = User::fromId($id, $this->db, $this->log);

			if ($user->id < 1) {
				$this->assignError($ret, "Invalid account information");

				return $ret;
			}

			$login = LoginKey::fromUserAndProvider($user->id, LoginKeyProviders::PASSWORD, $this->db, $this->log);

			try {
				$eventDesc = '';
				$event     = new ReturnHelper();

				if ($login->userId < 1) {
					$login           = new LoginKey($this->db, $this->log);
					$login->userId   = $user->id;
					$login->provider = new LoginKeyProviders(LoginKeyProviders::PASSWORD);
					$login->key      = password_hash($key, PASSWORD_DEFAULT);
					$event           = $login->create();
					$eventDesc       = 'create';
				} else {
					$login->key = password_hash($key, PASSWORD_DEFAULT);
					$event      = $login->update();
					$eventDesc  = 'update';
				}

				if ($event->isBad()) {
					if ($event->hasMessages()) {
						foreach ($event->getMessages() as $msg) {
							$ret->addMessage($msg);
							$this->log->error($msg);
						}
					} else {
						$ret->addMessage("Failed to {$eventDesc} login key");
						$this->log->error("Failed to {$eventDesc} login key");
					}

					return $ret;
				}

				$ret->addResult([self::STR_HTTP_CODE => HttpStatusCodes::OK]);

				$this->touchEvent(UserEventTypes::RESETPASSWORD, new UserEventResetPasswordDispatch($user, $this->db, $this->log));

				$ret->makeGood();
			} catch (\Exception $ex) {
				$this->assignError($ret, "An exception occurred: " . $ex->getMessage());
			}

			return $ret;
		}

		/**
		 * Performs user update. If completed successfully, the UserEventTypes::UPDATE chain is traversed with a new
		 * UserEventUpdateDispatch object. The following parameters are required:
		 *
		 * [
		 *   'id' => (int) 1 # user identifier
		 * ]
		 *
		 * Additionally, any of the following can be supplied alongside the user identifier:
		 *
		 *   == User Info ==
		 *   'actor'          => (int) 2                    # user who is acting upon another user (check for admin)
		 *   'email'          => (string) 'user@domain.com' # new email address
		 *   'confirmEmail'   => (string) 'user@domain.com' # confirm new email address
		 *   'emailConfirmed' => (bool) true                # optionally used to pre-confirm a new email address
		 *   'key'            => (string) 'someKey'         # new password key
		 *   'oldKey'         => (string) 'oldKey'          # old password key for confirmation
		 *   'confirmKey'     => (string) 'someKey'         # confirm new password key
		 *
		 *   == User Profile ==
		 *   'profile'        => [
		 *     'birthday'     => (string) '1900-01-01',     # birthday value for user
		 *     'description'  => (string) 'My info',        # short description/about section for user
		 *     'displayName'  => (string) 'SomeName',       # display name for user
		 *     'gender'       => (int) 1,                   # user's preferred gender
		 *     'realName'     => (string) 'John Doe'        # user's real name
		 *   ]
		 *
		 *   == User Settings ==
		 *   'settings'       => [
		 *     'htmlEmails'   => (bool) true,               # sets the user's html email preference
		 *     'playSounds'   => (bool) true                # sets the user's sound preference
		 *   ]
		 *
		 *   == User Visibilities ==
		 *   'visibilities'   => [
		 *     'birthday'     => (int) 1,                   # visibility for user's birthday
		 *     'description'  => (int) 1,                   # visibility for user's description
		 *     'email'        => (int) 1,                   # visibility for user's email address
		 *     'gender'       => (int) 1,                   # visibility for user's gender
		 *     'profile'      => (int) 1,                   # visibility for user's profile
		 *     'realName'     => (int) 1,                   # visibility for user's real name
		 *     'searches'     => (int) 1                    # visibility for user in searches
		 *   ]
		 *
		 * Other parameters are passed along in case they are consumed by the chain nodes, but processing is the responsibility of
		 * linked nodes. If the 'key' and 'confirmKey' parameters are included, they will be used to change the user's password but do
		 * NOT make any checks against the password's complexity.
		 *
		 * Resulting ReturnHelper will include a suggested HTTP status code in the 'httpCode' index and the user object if the
		 * operation was successful:
		 *
		 * [
		 *   'httpCode' => 0,     # suggested HTTP status code
		 *   'data'     => User{} # User object with updated user data
		 * ]
		 *
		 * @param ParameterHelper $params Parameters provided to perform the event.
		 * @throws \ReflectionException|\Exception
		 * @return ReturnHelper
		 */
		public function doUpdate(ParameterHelper $params) : ReturnHelper {
			$ret = new ReturnHelper();

			if (!$params->has(self::STR_ID)) {
				$this->assignError($ret, "Missing parameters for update");

				return $ret;
			}

			$user = User::fromId($params->getInt(self::STR_ID), $this->db, $this->log);

			if ($user->id < 1) {
				$this->assignError($ret, "Invalid user for update");

				return $ret;
			}

			$dispatch = new UserEventUpdateDispatch($user, $params, $this->db, $this->log);

			if ($params->hasAll(self::STR_EMAIL, self::STR_CONFIRM_EMAIL)) {
				$email        = $params->getString(self::STR_EMAIL);
				$confirmEmail = $params->getString(self::STR_CONFIRM_EMAIL);

				if ($email !== $user->email && !empty($email) && $email === $confirmEmail) {
					$user->email          = $email;
					$user->emailConfirmed = $params->getBool(self::STR_EMAILCONFIRMED, false);

					$dispatch->emailUpdated = true;
					$ret->addMessage("Email address was changed");
				}
			}

			$update = $user->update();

			if ($update->isBad()) {
				$this->assignError($ret, $update->getMessages()[0]);

				return $ret;
			}

			if ($params->hasAll(self::STR_KEY, self::STR_OLD_KEY, self::STR_CONFIRM_KEY)) {
				$key        = $params->getString(self::STR_KEY);
				$confirmKey = $params->getString(self::STR_CONFIRM_KEY);
				$login      = LoginKey::fromUserAndProvider($user->id, LoginKeyProviders::PASSWORD, $this->db, $this->log);

				if (password_verify($params->getString(self::STR_OLD_KEY), PASSWORD_DEFAULT)) {
					if ($login->userId == $user->id && !empty($key) && $key === $confirmKey) {
						$login->key = password_hash($key, PASSWORD_DEFAULT);

						$update = $login->update();

						if ($update->isBad()) {
							$this->assignError($ret, $update->getMessages()[0]);

							return $ret;
						}

						$ret->addMessage("Password was changed");
					}
				}
			}

			if ($params->hasAll(self::STR_KEY, self::STR_ACTOR) && (new UserRoles($this->db, $this->log))->userInRoleByName($params->getInt(self::STR_ACTOR), RoleStrings::ADMINISTRATOR)) {
				$key        = $params->getString(self::STR_KEY);
				$login      = LoginKey::fromUserAndProvider($user->id, LoginKeyProviders::PASSWORD, $this->db, $this->log);

				if ($login->userId == $user->id && !empty($key)) {
					$login->key = password_hash($key, PASSWORD_DEFAULT);

					$update = $login->update();

					if ($update->isBad()) {
						$this->assignError($ret, $update->getMessages()[0]);

						return $ret;
					}

					$ret->addMessage("Password was changed");
				}
			}

			if ($params->has(self::STR_PROFILE)) {
				$pParams = new ParameterHelper($params->get(self::STR_PROFILE));
				$profile = UserProfile::fromUser($user->id, $this->db, $this->log);

				if ($profile->userId == $user->id) {
					$profile->birthday    = new \DateTimeImmutable($pParams->getString(self::STR_BIRTHDAY, $profile->birthday->format('Y-m-d G:i:s')), new \DateTimeZone('UTC'));
					$profile->description = $pParams->getString(self::STR_DESCRIPTION, $profile->description);

					$displayName = $pParams->getString(self::STR_DISPLAY_NAME);

					if ($displayName !== $profile->displayName && UserProfile::validDisplayName($displayName)) {
						$p = UserProfile::fromDisplayName($displayName, $this->db, $this->log);

						if ($p->userId < 1) {
							$profile->displayName = $displayName;
						} else {
							$ret->addMessage("Couldn't change display name, invalid or already in use");
						}
					}

					$profile->gender   = new UserGenders($pParams->getInt(self::STR_GENDER, $profile->gender->getValue()));
					$profile->realName = $pParams->getString(self::STR_REAL_NAME, $profile->realName);

					$update = $profile->update();

					if ($update->isBad()) {
						$this->assignError($ret, $update->getMessages()[0]);

						return $ret;
					}

					$ret->addMessage("Profile was updated");
				}
			}

			if ($params->has(self::STR_SETTINGS)) {
				$sParams  = new ParameterHelper($params->get(self::STR_SETTINGS));
				$settings = UserSettings::fromUser($user->id, $this->db, $this->log);

				if ($settings->userId == $user->id) {
					$settings->htmlEmails = $sParams->getBool(self::STR_HTML_EMAILS, $settings->htmlEmails);
					$settings->playSounds = $sParams->getBool(self::STR_PLAY_SOUNDS, $settings->playSounds);

					$update = $settings->update();

					if ($update->isBad()) {
						$this->assignError($ret, $update->getMessages()[0]);

						return $ret;
					}

					$ret->addMessage("Settings were updated");
				}
			}

			if ($params->has(self::STR_VISIBILITIES)) {
				$vis     = UserVisibilities::fromUser($user->id, $this->db, $this->log);
				$vParams = new ParameterHelper($params->get(self::STR_VISIBILITIES));

				if ($vis->userId == $user->id) {
					$vis->birthday    = new VisibilityState($vParams->get(self::STR_BIRTHDAY, $vis->birthday->getValue()));
					$vis->description = new VisibilityState($vParams->get(self::STR_DESCRIPTION, $vis->description->getValue()));
					$vis->email       = new VisibilityState($vParams->get(self::STR_EMAIL, $vis->email->getValue()));
					$vis->gender      = new VisibilityState($vParams->get(self::STR_GENDER, $vis->gender->getValue()));
					$vis->profile     = new VisibilityState($vParams->get(self::STR_PROFILE, $vis->profile->getValue()));
					$vis->realName    = new VisibilityState($vParams->get(self::STR_REAL_NAME, $vis->realName->getValue()));
					$vis->searches    = new VisibilityState($vParams->get(self::STR_SEARCHES, $vis->searches->getValue()));

					$update = $vis->update();

					if ($update->isBad()) {
						$this->assignError($ret, $update->getMessages()[0]);

						return $ret;
					}

					$ret->addMessage("Visibility was updated");
				}
			}

			$this->touchEvent(UserEventTypes::UPDATE, $dispatch);

			$ret->makeGood();
			$ret->addResult([self::STR_HTTP_CODE => HttpStatusCodes::OK]);

			return $ret;
		}

		/**
		 * Links a processing node to the provided event. If an invalid event type is supplied, nothing will be linked.
		 *
		 * @param UserEventTypes|int $event UserEventTypes object or value to link provided node with in object.
		 * @param NodeBase $node Valid processing node to notify of event.
		 * @throws \ReflectionException
		 * @return void
		 */
		public function linkToEvent(UserEventTypes|int $event, NodeBase $node) : void {
			$e = UserEventTypes::tryGet($event);

			if ($e->getValue() === null) {
				return;
			}

			$this->events[$e->getValue()]->linkNode($node);

			return;
		}

		/**
		 * Touches an event, traversing the related chain so all linked nodes receive notification the event was executed.
		 * If an invalid event type is supplied, nothing will be traversed.
		 *
		 * @param UserEventTypes|int $event UserEventTypes object or value to route dispatch to correct chain.
		 * @param DispatchBase $dispatch Dispatch with relevant information for the selected chain.
		 * @throws \ReflectionException
		 * @return void
		 */
		protected function touchEvent(UserEventTypes|int $event, DispatchBase $dispatch) : void {
			$e = UserEventTypes::tryGet($event);

			if ($e->getValue() === null) {
				return;
			}

			$this->events[$e->getValue()]->traverse($dispatch, $this);

			return;
		}
	}
