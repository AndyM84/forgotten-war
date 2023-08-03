<?php

	const STOIC_CORE_PATH = '../../';
	require(STOIC_CORE_PATH . 'inc/core.php');

	use Stoic\Web\PageHelper;

	use Zibings\RoleStrings;
	use Zibings\Users;

	use function Zibings\isAuthenticated;

	global $Db, $Log, $Settings, $Stoic, $Tpl, $User;

	/**
	 * @var \Stoic\Pdo\PdoHelper $Db
	 * @var \Stoic\Log\Logger $Log
	 * @var \AndyM84\Config\ConfigContainer $Settings
	 * @var \Stoic\Web\Stoic $Stoic
	 * @var \League\Plates\Engine $Tpl
	 */

	$page = PageHelper::getPage('admin/index.php');
	$page->setTitle('Site Administration');

	if (!isAuthenticated($Db, RoleStrings::ADMINISTRATOR)) {
		$page->redirectTo('~/index.php');
	}

	$users = new Users($Db, $Log);

	$Tpl->addFolder('page', STOIC_CORE_PATH . '/tpl/admin/index');

	echo($Tpl->render('page::index', [
		'page' => $page,
		'dau'  => $users->getDailyActiveUserCount(),
		'mau'  => $users->getMonthlyActiveUserCount(),
		'tu'   => $users->getTotalUsers(),
		'tvu'  => $users->getTotalVerifiedUsers()
	]));
