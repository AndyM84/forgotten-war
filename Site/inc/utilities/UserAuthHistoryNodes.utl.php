<?php

	namespace Zibings;

	use Stoic\Chain\DispatchBase;
	use Stoic\Chain\NodeBase;
	use Stoic\Log\Logger;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Utilities\ParameterHelper;

	/**
	 * Processing node to record a UserAuthHistory entry for event-based logins.
	 *
	 * @package Zibings
	 */
	class UserAuthHistoryLoginNode extends NodeBase {
		/**
		 * Instantiates a new UserAuthHistoryLoginNode object.
		 *
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use.
		 * @return void
		 */
		public function __construct(
			protected PdoHelper $db,
			protected Logger|null $log = null) {
			$this->setKey('UAHLogin');
			$this->setVersion('1.0.0');

			return;
		}

		/**
		 * Handles processing of a provided dispatch.
		 * 
		 * @param mixed $sender Sender data, optional and thus can be 'null'.
		 * @param DispatchBase $dispatch Dispatch object to process.
		 * @return void
		 */
		public function process(mixed $sender, DispatchBase &$dispatch) : void {
			if (!($dispatch instanceof UserEventLoginDispatch)) {
				return;
			}

			UserAuthHistory::createFromUserId($dispatch->user->id, AuthHistoryActions::LOGIN, new ParameterHelper($_SERVER), "Login action from event system", $this->db, $this->log);

			return;
		}
	}

	/**
	 * Processing node to record a UserAuthHistory entry for event-based logouts.
	 *
	 * @package Zibings
	 */
	class UserAuthHistoryLogoutNode extends NodeBase {
		/**
		 * Instantiates a new UserAuthHistoryLogoutNode object.
		 *
		 * @param PdoHelper $db PdoHelper instance for internal use.
		 * @param Logger|null $log Optional Logger instance for internal use.
		 * @return void
		 */
		public function __construct(
			protected PdoHelper $db,
			protected Logger|null $log = null) {
			$this->setKey('UAHLogout');
			$this->setVersion('1.0.0');

			return;
		}

		/**
		 * Handles processing of a provided dispatch.
		 * 
		 * @param mixed $sender Sender data, optional and thus can be 'null'.
		 * @param DispatchBase $dispatch Dispatch object to process.
		 * @return void
		 */
		public function process(mixed $sender, DispatchBase &$dispatch) : void {
			if (!($dispatch instanceof UserEventLogoutDispatch)) {
				return;
			}

			UserAuthHistory::createFromUserId($dispatch->session->userId, AuthHistoryActions::LOGOUT, new ParameterHelper($_SERVER), "Logout action from event system", $this->db, $this->log);

			return;
		}
	}
