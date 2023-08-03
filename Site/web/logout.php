<?php

	const STOIC_CORE_PATH = '../';
	require(STOIC_CORE_PATH . 'inc/core.php');

	use Stoic\Utilities\ParameterHelper;
	use Stoic\Web\PageHelper;

	use Zibings\UserAuthHistoryLogoutNode;
	use Zibings\UserEvents;
	use Zibings\UserEventTypes;

	use function Zibings\isAuthenticated;

	global $Db, $Log, $Session, $Stoic, $Tpl;

	/**
	 * @var \Stoic\Pdo\PdoHelper $Db
	 * @var \Stoic\Log\Logger $Log
	 * @var \AndyM84\Config\ConfigContainer $Settings
	 * @var \Stoic\Web\Stoic $Stoic
	 * @var \League\Plates\Engine $Tpl
	 */

	$page = PageHelper::getPage('logout.php');
	$page->setTitle('Logout');

	if (!isAuthenticated($Db)) {
		$page->redirectTo('~/index.php');
	}

	$events = new UserEvents($Db, $Log);
	$events->linkToEvent(UserEventTypes::LOGOUT, new UserAuthHistoryLogoutNode($Db, $Log));

	$logout = $events->doLogout(new ParameterHelper());

	if ($logout->isBad()) {
		echo("<pre>");print_r($logout->getMessages());
	}

	$page->redirectTo('~/index.php');
