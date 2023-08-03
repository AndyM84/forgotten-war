<?php

	const STOIC_CORE_PATH = '../';
	require(STOIC_CORE_PATH . 'inc/core.php');

	use Stoic\Utilities\ParameterHelper;
	use Stoic\Web\PageHelper;

	use Zibings\LoginKeyProviders;
	use Zibings\UserEvents;
	use Zibings\UserEventTypes;

	use function Zibings\isAuthenticated;

	global $Db, $Log, $Settings, $Stoic, $Tpl;

	/**
	 * @var \Stoic\Pdo\PdoHelper $Db
	 * @var \Stoic\Log\Logger $Log
	 * @var \AndyM84\Config\ConfigContainer $Settings
	 * @var \Stoic\Web\Stoic $Stoic
	 * @var \League\Plates\Engine $Tpl
	 */

	$page = PageHelper::getPage('register.php');
	$page->setTitle('Account Creation');

	if (isAuthenticated($Db)) {
		$page->redirectTo('~/home.php');
	}

	$message = "";
	$tplFile = "index";
	$post    = $Stoic->getRequest()->getPost();

	if ($post->hasAll('email', 'confirmEmail', 'password', 'confirmPassword', 'tosAgreed', 'ppAgreed')) {
		$postData = [
			'email'      => $post->getString('email'),
			'key'        => $post->getString('password'),
			'confirmKey' => $post->getString('confirmPassword'),
			'provider'   => LoginKeyProviders::PASSWORD
		];

		if ($postData['email'] === $post->getString('confirmEmail')) {
			$events   = new UserEvents($Db, $Log);
			$events->linkToEvent(UserEventTypes::REGISTER,  new Zibings\EmailUserRegisterNode($page, $Settings, $Db, $Log));

			$register = $events->doRegister(new ParameterHelper($postData));

			if ($register->isBad()) {
				$message = $register->getMessages()[0];
			} else {
				$tplFile = "complete";
			}
		}
	}

	$Tpl->addFolder('page', STOIC_CORE_PATH . '/tpl/register');

	echo($Tpl->render("page::{$tplFile}", [
		'page'    => $page,
		'message' => $message
	]));
