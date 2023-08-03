<?php

	const STOIC_CORE_PATH = '../';
	require(STOIC_CORE_PATH . 'inc/core.php');

	use Stoic\Utilities\ParameterHelper;
	use Stoic\Utilities\StringHelper;
	use Stoic\Web\PageHelper;

	use Zibings\ErrorStrings;
	use Zibings\LoginKeyProviders;
	use Zibings\UserAuthHistoryLoginNode;
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

	$page = PageHelper::getPage('index.php');
	$page->setTitle('Login');

	if (isAuthenticated($Db)) {
		$page->redirectTo('~/home.php');
	}

	$msg  = [
		'good'     => false,
		'contents' => new StringHelper()
	];
	$get  = $Stoic->getRequest()->getGet();
	$post = $Stoic->getRequest()->getPost();

	if ($get->has('return')) {
		$_SESSION['loginReturn'] = $get->getString('return');
	}

	if ($post->hasAll('email', 'password')) {
		$events = new Zibings\UserEvents($Db, $Log);
		$events->linkToEvent(UserEventTypes::LOGIN, new UserAuthHistoryLoginNode($Db, $Log));
		$login  = $events->doLogin(new ParameterHelper([
			'email'    => $post->getString('email'),
			'key'      => $post->getString('password'),
			'provider' => LoginKeyProviders::PASSWORD
		]));

		if ($login->isBad()) {
			if ($login->hasMessages()) {
				$msg['contents']->append($login->getMessages()[0]);
			} else {
				$msg['contents']->append("There was an error logging you in, please try again");
			}
		} else {
			$location = 'home.php';

			if ($Session->has('loginReturn')) {
				$location = $Session->getString('loginReturn');
			}

			$page->redirectTo("~/{$location}");
		}
	}

	if ($get->has('error')) {
		if (!$msg['contents']->isEmptyOrNullOrWhitespace()) {
			$msg['contents']->append('<br />');
		}

		switch ($get->getString('error')) {
			case ErrorStrings::Login_LoggedOut:
				$msg['contents']->append("You were logged out");

				break;

			default:
				break;
		}
	}

	$Tpl->addFolder('page', STOIC_CORE_PATH . '/tpl/index');

	echo($Tpl->render('page::index', [
		'page'        => $page,
		'message'     => $msg['contents']->data(),
		'messageGood' => $msg['good']
	]));
