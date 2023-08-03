<?php

	namespace Zibings;

	use Stoic\Log\Logger;
	use Stoic\Utilities\ParameterHelper;
	use Stoic\Utilities\ReturnHelper;
	use Stoic\Web\Api\BaseDbApi;
	use Stoic\Web\Api\Response;
	use Stoic\Web\Api\Stoic;

	/**
	 * Basic response data structure for including a status code and a message.
	 *
	 * @package Zibings
	 */
	class StatusResponseData {
		/**
		 * Constructor for the basic status response data structure.
		 *
		 * @param int $status Status code for response data.
		 * @param null|string $message Optional string message for response data.
		 */
		public function __construct(
			public int $status,
			public null|string $message = "") {
			return;
		}
	}

	/**
	 * Basic controller class that offers some helper methods on top of the Stoic API class.
	 *
	 * @package Zibings
	 */
	abstract class ApiController extends BaseDbApi {
		/**
		 * Default constructor for ApiController objects.
		 *
		 * @param Stoic $stoic Internal instance of Stoic API object.
		 * @param \PDO $db Internal instance of PDO object.
		 * @param Logger|null $log Optional internal instance of Logger object, new instance created if not supplied.
		 */
		public function __construct(protected Stoic $stoic, \PDO $db, Logger $log = null) {
			parent::__construct($db, $log);
			$this->registerEndpoints();

			return;
		}

		/**
		 * Attempts to assign the top message from a ReturnHelper object as the error to the Response object.
		 *
		 * @param Response $response Response object to set to error state.
		 * @param ReturnHelper $rh ReturnHelper object to try pulling messages from.
		 * @param string $defaultMessage Default message if ReturnHelper object has no messages.
		 * @throws \ReflectionException
		 * @return void
		 */
		protected function assignReturnHelperError(Response &$response, ReturnHelper $rh, string $defaultMessage) : void {
			if ($rh->hasMessages()) {
				$response->setAsError($rh->getMessages()[0]);
			} else {
				$response->setAsError($defaultMessage);
			}

			return;
		}

		/**
		 * Attempts to retrieve the User object from the authorization header.
		 *
		 * @throws \Exception
		 * @return User
		 */
		protected function getUser() : User {
			$ret     = new User($this->db, $this->log);
			$headers = getallheaders();

			if (array_key_exists('Authorization', $headers) === false) {
				return $ret;
			}

			$token    = explode(':', base64_decode(str_replace('Bearer ', '', $headers['Authorization'])));
			$session  = UserSession::fromToken($token[1], $this->db, $this->log);
			$expiryDt = (new \DateTimeImmutable('now', new \DateTimeZone('UTC')))->sub(new \DateInterval('P1Y'));

			if ($session->id < 1 || $session->created < $expiryDt) {
				return $ret;
			}

			return User::fromId($session->userId, $this->db, $this->log);
		}

		/**
		 * Returns a new StatusResponseData structure instance. Will create a new instance with '0' as the status and an
		 * empty string as the message if no arguments are provided.
		 *
		 * @param int|null $status Optional status code for response data, will be 0 if not supplied.
		 * @param null|string $message Optional status message for response data, will be an empty string if not supplied.
		 * @return StatusResponseData
		 */
		protected function newStatusResponseData(null|int $status = null, null|string $message = null) : StatusResponseData {
			return new StatusResponseData($status ?? 0, $message ?? '');
		}

		/**
		 * Helper method to register endpoints for member methods.
		 *
		 * @param null|string $verbs String value of applicable request verbs for endpoint, '*' for all verbs or use pipe (|) to combine multiple verbs.
		 * @param null|string $pattern String value of URL pattern for endpoint, `null` will set this endpoint as the 'default'.
		 * @param string $method Name of member method to use as callable for endpoint.
		 * @param mixed $authRoles Optional string, array of string values, or boolean value representing authorization requirements for endpoint.
		 * @return void
		 */
		protected function registerEndpoint(null|string $verbs, null|string $pattern, string $method, mixed $authRoles = null) : void {
			$this->stoic->registerEndpoint($verbs, $pattern, \Closure::fromCallable([$this, $method]), $authRoles);

			return;
		}

		/**
		 * Abstract method so child controllers register their endpoints.
		 *
		 * @return void
		 */
		abstract protected function registerEndpoints() : void;

		/**
		 * Helper method to perform basic ParameterHelper check around an action.
		 *
		 * @param ParameterHelper $params ParameterHelper instance to check keys against.
		 * @param array|string $keys String or array of strings for key(s) to check in ParameterHelper before executing action.
		 * @param callable $callable Callable to execute if key(s) pass existence/value guards.
		 * @param bool $canBeEmpty Optional toggle to allow empty values for key(s).
		 * @return void
		 */
		protected function tryParameterAction(ParameterHelper $params, string|array $keys, callable $callable, bool $canBeEmpty = false) : void {
			if (is_array($keys)) {
				foreach ($keys as $k) {
					if (!$params->has($k) || ($canBeEmpty === false && empty($params->get($k)))) {
						return;
					}
				}
			} else if (!$params->has($keys) || ($canBeEmpty === false && empty($params->get($keys)))) {
				return;
			}

			$callable();

			return;
		}
	}
