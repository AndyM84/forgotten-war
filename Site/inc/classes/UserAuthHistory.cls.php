<?php

	namespace Zibings;

	use Stoic\Log\Logger;
	use Stoic\Pdo\BaseDbTypes;
	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbModel;
	use Stoic\Utilities\EnumBase;
	use Stoic\Utilities\ParameterHelper;
	use Stoic\Utilities\ReturnHelper;
	use Stoic\Web\Resources\ServerIndices;

	/**
	 * Enumerated authentication actions that are tracked by history.
	 *
	 * @package Zibings
	 */
	class AuthHistoryActions extends EnumBase {
		const LOGIN       = 1;
		const LOGOUT      = 2;
		const TOKEN_CHECK = 3;
	}

	/**
	 * Class for representing an authentication action made by/for a user account.
	 *
	 * @package Zibings
	 */
	class UserAuthHistory extends StoicDbModel {
		/**
		 * The authentication action that is being recorded.
		 *
		 * @var AuthHistoryActions
		 */
		public AuthHistoryActions $action;
		/**
		 * Network address of the user when this action occurred.
		 *
		 * @var string
		 */
		public string $address;
		/**
		 * Network hostname of the user when this action occurred.
		 *
		 * @var string
		 */
		public string $hostname;
		/**
		 * Any relevant notes from the system when the action occurred.
		 *
		 * @var string
		 */
		public string $notes;
		/**
		 * Date and time the action occurred.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $recorded;
		/**
		 * Integer identifier of the associated user. If not matched, value will be 0.
		 *
		 * @var int
		 */
		public int $userId;


		/**
		 * Static method to create a new history record from a user.
		 *
		 * @param User $user User object to use for identifier.
		 * @param int|AuthHistoryActions $action The authentication action that is being recorded.
		 * @param ParameterHelper $server The server array which needs to contain the 'REMOTE_ADDR' index.
		 * @param string $notes Optional notes for the recorded action.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created if not supplied.
		 * @throws \ReflectionException
		 * @return UserAuthHistory
		 */
		public static function createFromUser(User $user, int|AuthHistoryActions $action, ParameterHelper $server, string $notes, PdoHelper $db, Logger $log = null) : UserAuthHistory {
			return static::createFromUserId($user->id, $action, $server, $notes, $db, $log);
		}

		/**
		 * Static method to create a new history record.
		 *
		 * @param int $userId User identifier to use for recorded action.
		 * @param int|AuthHistoryActions $action The authentication action that is being recorded.
		 * @param ParameterHelper $server The server array which needs to contain the 'REMOTE_ADDR' index.
		 * @param string $notes Optional notes for the recorded action.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created if not supplied.
		 * @throws \ReflectionException
		 * @return UserAuthHistory
		 */
		public static function createFromUserId(int $userId, int|AuthHistoryActions $action, ParameterHelper $server, string $notes, PdoHelper $db, Logger $log = null) : UserAuthHistory {
			$ret = new UserAuthHistory($db, $log);
			$action = AuthHistoryActions::tryGetEnum($action, AuthHistoryActions::class);

			if ($userId < 1 || !$server->has(ServerIndices::REMOTE_ADDR) || $action->getValue() === null) {
				return $ret;
			}

			$ret->action   = $action;
			$ret->address  = $server->getString(ServerIndices::REMOTE_ADDR);
			$ret->hostname = gethostbyaddr($ret->address);
			$ret->notes    = $notes ?? '';
			$ret->userId   = $userId;

			if ($ret->create()->isBad()) {
				$ret = new UserAuthHistory($db, $log);
			}

			return $ret;
		}


		/**
		 * Determines if the system should attempt to create a new UserAuthHistory record in the database.
		 *
		 * @throws \Exception
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			if ($this->action->getValue() === null || $this->userId < 1) {
				return false;
			}

			$this->recorded = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));

			return true;
		}

		/**
		 * Disabled for this model.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canDelete() : bool|ReturnHelper {
			return false;
		}

		/**
		 * Disabled for this model.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canRead() : bool|ReturnHelper {
			return false;
		}

		/**
		 * Disabled for this model.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canUpdate() : bool|ReturnHelper {
			return false;
		}

		/**
		 * Initializes a new UserAuthHistory object.
		 *
		 * @throws \Exception
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[dbo].[UserAuthHistory]');
			} else {
				$this->setTableName('UserAuthHistory');
			}

			$this->setColumn('action', 'Action', BaseDbTypes::INTEGER, false, true, false);
			$this->setColumn('address', 'Address', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('hostname', 'Hostname', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('notes', 'Notes', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('recorded', 'Recorded', BaseDbTypes::DATETIME, false, true, false);
			$this->setColumn('userId', 'UserID', BaseDbTypes::INTEGER, false, true, false);

			$this->action   = new AuthHistoryActions();
			$this->address  = '';
			$this->hostname = '';
			$this->notes    = '';
			$this->recorded = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->userId   = 0;

			return;
		}
	}
