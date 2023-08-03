<?php

	if (!defined('STOIC_CORE_PATH')) {
		define('STOIC_CORE_PATH', './');
	}

	$corePath       = STOIC_CORE_PATH;
	$corePathSuffix = $corePath[strlen($corePath) - 1];

	if ($corePathSuffix != '/') {
		$corePath .= '/';
	}

	require(STOIC_CORE_PATH . 'vendor/autoload.php');

	use AndyM84\Config\ConfigContainer;

	use League\Plates\Engine;

	use Stoic\Log\Logger;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Utilities\ParameterHelper;
	use Stoic\Web\PageHelper;
	use Stoic\Web\Resources\PageVariables;
	use Stoic\Web\Stoic;

	use Zibings\SettingsStrings;
	use Zibings\UserProfile;
	use Zibings\UserRoles;

	use function Zibings\getUserFromSessionToken;

	global $Db, $Log, $Settings, $Stoic, $Tpl, $User, $Profile;

	/**
	 * @var PdoHelper $Db
	 * @var Logger $Log
	 * @var ConfigContainer $Settings
	 * @var Stoic $Stoic
	 * @var Engine $Tpl
	 * @var \Zibings\User $User
	 * @var \Zibings\UserProfile $Profile
	 */

	if (PHP_SAPI == 'cli') {
		$Stoic = Stoic::getInstance(STOIC_CORE_PATH, new PageVariables([], [], [], [], [], [], ['REQUEST_METHOD' => 'GET'], []));
	} else {
		$Stoic = Stoic::getInstance(STOIC_CORE_PATH);
	}

	$Log      = $Stoic->getLog();
	$Db       = $Stoic->getDb();
	$Session  = new ParameterHelper($_SESSION);
	$Settings = $Stoic->getConfig();
	$User     = getUserFromSessionToken($Session, $Db, $Log);
	$Profile  = ($User->id > 0) ? UserProfile::fromUser($User->id, $Db, $Log) : new UserProfile($Db, $Log);

	if ($User->id > 0) {
		if (empty($Profile->displayName)) {
			$Profile->displayName = "*{$User->email}";
		}

		$User->markActive();
	}

	$Tpl = new Engine(null, 'tpl.php');
	$Tpl->addFolder('shared', STOIC_CORE_PATH . '/tpl/shared');
	$Tpl->addData([
		'get'       => $Stoic->getRequest()->getGet(),
		'post'      => $Stoic->getRequest()->getPost(),
		'profile'   => $Profile,
		'request'   => $Stoic->getRequest(),
		'session'   => $Session,
		'settings'  => $Settings,
		'user'      => $User,
		'userRoles' => new UserRoles($Db, $Log)
	]);

	$pages = [
		'account.php',
		'confirm-email.php',
		'home.php',
		'index.php',
		'register.php',
		'reset-password.php'
	];

	foreach ($pages as $pg) {
		PageHelper::getPage($pg)->setTitlePrefix($Settings->get(SettingsStrings::SITE_NAME, 'ZSF'));
	}
