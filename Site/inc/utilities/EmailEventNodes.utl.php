<?php

	namespace Zibings;

	use AndyM84\Config\ConfigContainer;

	use League\Plates\Engine;

	use Stoic\Chain\DispatchBase;
	use Stoic\Chain\NodeBase;
	use Stoic\Log\Logger;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Web\PageHelper;

	/**
	 * Processing node to send registration email for new users.
	 *
	 * @package Zibings
	 */
	class EmailUserRegisterNode extends NodeBase {
		/**
		 * Instantiates a new EmailUserRegisterNode object for use with the UserEvents system.
		 *
		 * @param \Stoic\Web\PageHelper $page PageHelper instance for internal use.
		 * @param \AndyM84\Config\ConfigContainer $settings ConfigContainer instance for internal use.
		 * @param \Stoic\Pdo\PdoHelper $db PdoHelper instance for internal use.
		 * @param \Stoic\Log\Logger|null $log Optional Logger instance for internal use, defaults to new instance created.
		 */
		public function __construct(
			public PageHelper $page,
			public ConfigContainer $settings,
			public PdoHelper $db,
			public Logger|null $log = null) {
			$this->setKey('EmailUserRegisterNode')->setVersion('1.0.0');

			if ($this->log === null) {
				$this->log = new Logger();
			}

			return;
		}

		/**
		 * Processes the dispatch when touched on a chain.
		 *
		 * @param mixed $sender Sender data, optional and thus can be 'null'.
		 * @param \Stoic\Chain\DispatchBase $dispatch Dispatch object to process.
		 * @throws \PHPMailer\PHPMailer\Exception
		 * @return void
		 */
		public function process(mixed $sender, DispatchBase &$dispatch) : void {
			if (!($dispatch instanceof UserEventRegisterDispatch) || $dispatch->user->id < 1) {
				return;
			}

			$ut          = new UserToken($this->db, $this->log);
			$ut->context = "REGISTRATION EMAIL CONFIRMATION";
			$ut->token   = UserSession::generateGuid(false);
			$ut->userId  = $dispatch->user->id;
			$create      = $ut->create();

			if ($create->isBad()) {
				return;
			}

			$tpl = new Engine(null, 'tpl.php');
			$tpl->addFolder('shared', STOIC_CORE_PATH . '/tpl/shared');
			$tpl->addFolder('emails', STOIC_CORE_PATH . '/tpl/emails');

			$mail = getPhpMailer($this->settings);
			$mail->Subject = "[{$this->settings->get(SettingsStrings::SITE_NAME)}] Account Setup: Please Confirm Your Email";
			$mail->isHTML(true);
			$mail->Body = $tpl->render('emails::confirm-email', [
				'page'  => $this->page,
				'token' => base64_encode("{$ut->userId}:{$ut->token}")
			]);
			$mail->addAddress($dispatch->user->email);

			$mail->send();

			return;
		}
	}

	/**
	 * Processing node to send email confirmation for users who update their email address.
	 *
	 * @package Zibings
	 */
	class EmailUserUpdateNode extends NodeBase {
		/**
		 * Instantiates a new EmailUserUpdateNode object.
		 *
		 * @param \Stoic\Web\PageHelper $page PageHelper instance for internal use.
		 * @param \AndyM84\Config\ConfigContainer $settings ConfigContainer instance for internal use.
		 * @param \Stoic\Pdo\PdoHelper $db PdoHelper instance for internal use.
		 * @param \Stoic\Log\Logger|null $log Optional Logger instance for internal use, defaults to new instance created.
		 */
		public function __construct(
			public PageHelper $page,
			public ConfigContainer $settings,
			public PdoHelper $db,
			public Logger|null $log = null) {
			$this->setKey('EmailUserUpdateNode')->setVersion('1.0.0');

			if ($this->log === null) {
				$this->log = new Logger();
			}

			return;
		}

		/**
		 * Processes the dispatch when touched on a chain.
		 *
		 * @param mixed $sender Sender data, optional and thus can be 'null'.
		 * @param \Stoic\Chain\DispatchBase $dispatch Dispatch object to process.
		 * @throws \PHPMailer\PHPMailer\Exception
		 * @return void
		 */
		public function process(mixed $sender, DispatchBase &$dispatch) : void {
			if (!($dispatch instanceof UserEventUpdateDispatch) || $dispatch->user->id < 1 || !$dispatch->emailUpdated) {
				return;
			}

			$ut          = new UserToken($this->db, $this->log);
			$ut->context = "UPDATED EMAIL CONFIRMATION";
			$ut->token   = UserSession::generateGuid(false);
			$ut->userId  = $dispatch->user->id;
			$create      = $ut->create();

			if ($create->isBad()) {
				return;
			}

			$tpl = new Engine(null, 'tpl.php');
			$tpl->addFolder('shared', STOIC_CORE_PATH . '/tpl/shared');
			$tpl->addFolder('emails', STOIC_CORE_PATH . '/tpl/emails');

			$mail = getPhpMailer($this->settings);
			$mail->Subject = "[{$this->settings->get(SettingsStrings::SITE_NAME)}] Account Updated: Please Confirm Your New Email";
			$mail->isHTML(true);
			$mail->Body = $tpl->render('emails::confirm-email', [
				'page' => $this->page,
				'token' => base64_encode("{$ut->userId}:{$ut->token}")
			]);
			$mail->addAddress($dispatch->user->email);

			$mail->send();

			return;
		}
	}
