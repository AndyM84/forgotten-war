<?php

	namespace Zibings;

	use Stoic\Pdo\BaseDbTypes;
	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\StoicDbModel;
	use Stoic\Utilities\EnumBase;
	use Stoic\Utilities\ReturnHelper;

	/**
	 * Types of actions that can be recorded for user relationships.
	 *
	 * @package Zibings
	 */
	class UserRelationEventActions extends EnumBase {
		const INVITE  = 1;
		const ACCEPT  = 2;
		const DECLINE = 3;
		const DELETE  = 4;
	}

	/**
	 * Class for representing an action/event involving user relationships.
	 *
	 * @package Zibings
	 */
	class UserRelationEvent extends StoicDbModel {
		/**
		 * The action that was performed on the relationship.
		 *
		 * @var UserRelationEventActions
		 */
		public UserRelationEventActions $action;
		/**
		 * Any notes relevant to this action.
		 *
		 * @var string
		 */
		public string $notes;
		/**
		 * Date and time this action was recorded.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $recorded;
		/**
		 * The recorded stage of this relationship.
		 *
		 * @var UserRelationStages
		 */
		public UserRelationStages $stage;
		/**
		 * Integer identifier of the first friend.
		 *
		 * @var int
		 */
		public int $userOne;
		/**
		 * Integer identifier of the second friend.
		 *
		 * @var int
		 */
		public int $userTwo;


		/**
		 * Determines if the system should attempt to create a UserRelationEvent in the database.
		 *
		 * @throws \Exception
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			if ($this->userOne < 1 || $this->userTwo < 1 || $this->action->getValue() === null || $this->stage->getValue() === null) {
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
		 * Initializes a new UserRelationEvent object.
		 *
		 * @throws \Exception
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[dbo].[UserRelationEvent]');
			} else {
				$this->setTableName('UserRelationEvent');
			}

			$this->setColumn('action', 'Action', BaseDbTypes::INTEGER, false, true, false);
			$this->setColumn('notes', 'Notes', BaseDbTypes::STRING, false, true, false);
			$this->setColumn('recorded', 'Recorded', BaseDbTypes::DATETIME, false, true, false);
			$this->setColumn('stage', 'Stage', BaseDbTypes::INTEGER, false, true, false);
			$this->setColumn('userOne', 'UserID_One', BaseDbTypes::INTEGER, false, true, false);
			$this->setColumn('userTwo', 'UserID_Two', BaseDbTypes::INTEGER, false, true, false);

			$this->action   = new UserRelationEventActions();
			$this->notes    = '';
			$this->recorded = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->stage    = new UserRelationStages();
			$this->userOne  = 0;
			$this->userTwo  = 0;
			
			return;
		}
	}
