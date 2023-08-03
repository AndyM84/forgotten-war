<?php

	namespace Zibings;

	use Stoic\Pdo\BaseDbTypes;
	use Stoic\Pdo\PdoDrivers;
	use Stoic\Pdo\StoicDbModel;
	use Stoic\Utilities\EnumBase;
	use Stoic\Utilities\ReturnHelper;

	/**
	 * Different contact types.
	 *
	 * @package Zibings
	 */
	class UserContactTypes extends EnumBase {
		const EMAIL    = 1;
		const PHONE    = 2;
		const TWITTER  = 3;
		const WEBSITE  = 4;
	}

	/**
	 * Class for representing user contact entries.
	 *
	 * @package Zibings
	 */
	class UserContact extends StoicDbModel {
		/**
		 * Date and time the contact was created.
		 *
		 * @var \DateTimeInterface
		 */
		public \DateTimeInterface $created;
		/**
		 * Whether this is the user's primary method of contact.
		 *
		 * @var bool
		 */
		public bool $primary;
		/**
		 * Type of contact.
		 *
		 * @var UserContactTypes
		 */
		public UserContactTypes $type;
		/**
		 * Integer identifier of the user this contact belongs to.
		 *
		 * @var int
		 */
		public int $userId;
		/**
		 * Value of the contact.
		 *
		 * @var string
		 */
		public string $value;


		/**
		 * Determines if the system should attempt to create a new UserContact in the database.
		 *
		 * @throws \Exception
		 * @return bool|ReturnHelper
		 */
		protected function __canCreate() : bool|ReturnHelper {
			if ($this->userId < 1 || empty($this->value) || $this->type->getValue() === null) {
				return false;
			}

			$this->created = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));

			return true;
		}

		/**
		 * Determines if the system should attempt to delete a UserContact from the database.
		 *
		 * @return bool|ReturnHelper
		 */
		protected function __canDelete() : bool|ReturnHelper {
			if ($this->userId < 1 || $this->type->getValue() === null) {
				return false;
			}

			return true;
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
		 * Initializes a new UserContact object.
		 *
		 * @throws \Exception
		 * @return void
		 */
		protected function __setupModel() : void {
			if ($this->db->getDriver()->is(PdoDrivers::PDO_SQLSRV)) {
				$this->setTableName('[dbo].[UserContact]');
			} else {
				$this->setTableName('UserContact');
			}

			$this->setColumn('created', 'Created', BaseDbTypes::DATETIME, true, true, false);
			$this->setColumn('primary', 'Primary', BaseDbTypes::BOOLEAN, true, true, false);
			$this->setColumn('type', 'Type', BaseDbTypes::INTEGER, true, true, false);
			$this->setColumn('userId', 'UserID', BaseDbTypes::INTEGER, true, true, false);
			$this->setColumn('value', 'Value', BaseDbTypes::STRING, true, true, false);

			$this->created = new \DateTimeImmutable('now', new \DateTimeZone('UTC'));
			$this->primary = false;
			$this->type    = new UserContactTypes();
			$this->userId  = 0;
			$this->value   = '';

			return;
		}
	}
