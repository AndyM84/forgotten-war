<?php

	const STOIC_CORE_PATH = '../';
	require(STOIC_CORE_PATH . 'inc/core.php');

	use Stoic\Utilities\ParameterHelper;
	use Stoic\Web\PageHelper;

	use Zibings\UserEvents;
	use Zibings\UserToken;

	use function Zibings\isAuthenticated;
	use function Zibings\sendResetEmail;

	global $Db, $Log, $Settings, $Stoic, $Tpl;

	/**
	 * @var \Stoic\Pdo\PdoHelper $Db
	 * @var \Stoic\Log\Logger $Log
	 * @var \AndyM84\Config\ConfigContainer $Settings
	 * @var \Stoic\Web\Stoic $Stoic
	 * @var \League\Plates\Engine $Tpl
	 */

	$page = PageHelper::getPage('reset-password.php');
	$page->setTitle('Reset Password');

	if (isAuthenticated($Db)) {
		$page->redirectTo('~/home.php');
	}

	$message = "";
	$tplFile = "index";
	$get     = $Stoic->getRequest()->getGet();
	$post    = $Stoic->getRequest()->getPost();

	if ($get->has('token')) {
		$tok = explode(':', base64_decode($get->getString('token')));
		$ut  = UserToken::fromToken($tok[1], intval($tok[0]), $Db, $Log);

		if ($ut->userId > 0) {
			$tplFile = 'change';
		}
	}

	if ($post->hasAll('email')) {
		if (sendResetEmail($post->getString('email'), $page, $Settings, $Db, $Log)) {
			$tplFile = 'sent';
		} else {
			$tplFile = 'error';
		}
	}

	if ($post->hasAll('token', 'password', 'confirmPassword')) {
		$tok    = explode(':', base64_decode($post->getString('token')));
		$ut     = UserToken::fromToken($tok[1], intval($tok[0]), $Db, $Log);
		$events = new UserEvents($Db, $Log);

		if ($ut->userId > 0) {
			$reset = $events->doResetPassword(new ParameterHelper([
				'id'         => $ut->userId,
				'key'        => $post->getString('password'),
				'confirmKey' => $post->getString('confirmPassword')
			]));

			if ($reset->isGood()) {
				$tplFile = 'complete';
				$ut->delete();
			} else {
				$tplFile = 'error';
				$message = $reset->getMessages()[0];
			}
		}
	}

	$Tpl->addFolder('page', STOIC_CORE_PATH . '/tpl/reset-password');

	echo($Tpl->render("page::{$tplFile}", [
		'page' => $page
	]));
