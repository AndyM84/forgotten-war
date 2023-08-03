<?php

	const STOIC_CORE_PATH = '../';
	require(STOIC_CORE_PATH . 'inc/core.php');

	use Stoic\Utilities\ParameterHelper;
	use Stoic\Web\PageHelper;

	use Zibings\UserEvents;

	use function Zibings\isAuthenticated;

	global $Db, $Log, $Settings, $Stoic, $Tpl;

	/**
	 * @var \Stoic\Pdo\PdoHelper $Db
	 * @var \Stoic\Log\Logger $Log
	 * @var \AndyM84\Config\ConfigContainer $Settings
	 * @var \Stoic\Web\Stoic $Stoic
	 * @var \League\Plates\Engine $Tpl
	 */

	$page = PageHelper::getPage('confirm-email.php');
	$page->setTitle('Email Confirmation');

	if (isAuthenticated($Db)) {
		$page->redirectTo('~/home.php');
	}

	$message = "";
	$tplFile = "error";
	$get     = $Stoic->getRequest()->getGet();

	if ($get->has('token') && !empty($get->getString('token'))) {
		$events  = new UserEvents($Db, $Log);
		$confirm = $events->doConfirm(new ParameterHelper(['token' => $get->getString('token')]));

		if ($confirm->isBad()) {
			$message = $confirm->getMessages()[0];
		} else {
			$tplFile = 'index';
		}
	}

	$Tpl->addFolder('page', STOIC_CORE_PATH . '/tpl/confirm-email');

	echo($Tpl->render("page::{$tplFile}", [
		'page'    => $page,
		'message' => $message
	]));