<?php

	namespace Zibings;

	use Stoic\Log\Logger;
	use Stoic\Pdo\BaseDbTypes;
	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Pdo\StoicDbModel;
	use Stoic\Utilities\EnumBase;
	use Stoic\Utilities\ReturnHelper;

	/**
	 * Visibility states for custom visibilities.
	 *
	 * @package Zibings
	 */
	class VisibilityState extends EnumBase {
		const PRV           = 0;
		const FRIENDS       = 1;
		const AUTHENTICATED = 2;
		const PUB           = 3;
	}

	/**
	 * Class for representing the system-level visibility settings for a specific user.
	 *
	 * @package Zibings
	 */
	class UserVisibilities extends StoicDbModel {
		/**
		 * How visible the user's birthday should be.
		 *
		 * @var VisibilityState
		 */
		public VisibilityState $birthday;
		/**
		 * How visible the user's description/about-me should be.
		 *
		 * @var VisibilityState
		 */
		public VisibilityState $description;
		/**
		 * How visible the user's email should be.
		 *
		 * @var VisibilityState
		 */
		public VisibilityState $email;
		/**
		 * How visible the user's gender should be.
		 *
		 * @var VisibilityState
		 */
		public VisibilityState $gender;
		/**
		 * How visible the user's profile should be.
		 *
		 * @var VisibilityState
		 */
		public VisibilityState $profile;
		/**
		 * How visible the user's real name should be.
		 *
		 * @var VisibilityState
		 */
		public VisibilityState $realName;
		/**
		 * How visible the user's search history should be.
		 *
		 * @var VisibilityState
		 */
		public VisibilityState $searches;
		/**
		 * Integer identifier of user who owns these visibilities.
		 *
		 * @var int
		 */
		public int $userId;


		/**
		 * Static method to retrieve a user's visibility settings.
		 *
		 * @param integer $userId Integer identifier of user whose visibilities to retrieve.
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use, new instance created by default.
		 * @throws \Exception
		 * @return UserVisibilities
		 */
		public static function fromUser(int $userId, PdoHelper $db, Logger $log = null) : UserVisibilities {
			$ret = new UserVisibilities($db, $log);
			$ret->userId = $userId;

			if ($ret->read()->isBad()) {
				$ret->userId = 0;
			}

			return $ret;
		}


		/**
		 * Determines if the system should attempt to create a UserVisibilities in the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			if ($this->userId < 1) {
				return false;
			}

			return true;
		}
		
		/**
		 * Determines if the system should attempt to delete a UserVisibilities from the database.
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
		 * Determines if the system should attempt to read a UserVisibilities from the database.
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
		 * Determines if the system should attempt to update a UserVisibilities in the database.
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
		 * Initializes a new UserVisibilities object.
		 *
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[dbo].[UserVisibilities]');
			} else {
				$this->setTableName('UserVisibilities');
			}

			$this->setColumn('birthday', 'Birthday', BaseDbTypes::INTEGER, false, true, true);
			$this->setColumn('description', 'Description', BaseDbTypes::INTEGER, false, true, true);
			$this->setColumn('email', 'Email', BaseDbTypes::INTEGER, false, true, true);
			$this->setColumn('gender', 'Gender', BaseDbTypes::INTEGER, false, true, true);
			$this->setColumn('profile', 'Profile', BaseDbTypes::INTEGER, false, true, true);
			$this->setColumn('realName', 'RealName', BaseDbTypes::INTEGER, false, true, true);
			$this->setColumn('searches', 'Searches', BaseDbTypes::INTEGER, false, true, true);
			$this->setColumn('userId', 'UserID', BaseDbTypes::INTEGER, true, true, false);

			$this->birthday    = new VisibilityState(VisibilityState::FRIENDS);
			$this->description = new VisibilityState(VisibilityState::AUTHENTICATED);
			$this->email       = new VisibilityState(VisibilityState::FRIENDS);
			$this->gender      = new VisibilityState(VisibilityState::FRIENDS);
			$this->profile     = new VisibilityState(VisibilityState::AUTHENTICATED);
			$this->realName    = new VisibilityState(VisibilityState::FRIENDS);
			$this->searches    = new VisibilityState(VisibilityState::AUTHENTICATED);
			$this->userId      = 0;

			return;
		}
	}
