<?php

	const STOIC_CORE_PATH = '../';
	require(STOIC_CORE_PATH . 'inc/core.php');

	use Stoic\Utilities\ParameterHelper;
	use Stoic\Web\PageHelper;

	use Zibings\AuthHistoryActions;
	use Zibings\UserAuthHistory;

	use function Zibings\isAuthenticated;

	global $Db, $Log, $Stoic, $Tpl, $User;

	$page = PageHelper::getPage('home.php');
	$page->setTitle('Homepage');

	if (!isAuthenticated($Db)) {
		$page->redirectTo('~/index.php');
	}

	$Tpl->addFolder('page', STOIC_CORE_PATH . '/tpl/home');

	echo($Tpl->render('page::index', ['page' => $page]));
