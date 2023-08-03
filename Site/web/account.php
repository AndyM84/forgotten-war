<?php

	const STOIC_CORE_PATH = '../';
	require(STOIC_CORE_PATH . 'inc/core.php');

	use Stoic\Utilities\ParameterHelper;
	use Stoic\Web\PageHelper;

	use Zibings\EmailUserUpdateNode;
	use Zibings\User;
	use Zibings\UserEvents;
	use Zibings\UserEventTypes;
	use Zibings\UserProfile;
	use Zibings\UserSettings;
	use Zibings\UserVisibilities;

	use function Zibings\isAuthenticated;

	global $Db, $Log, $Settings, $Stoic, $Tpl, $User;

	/**
	 * @var \Stoic\Pdo\PdoHelper $Db
	 * @var \Stoic\Log\Logger $Log
	 * @var \AndyM84\Config\ConfigContainer $Settings
	 * @var \Stoic\Web\Stoic $Stoic
	 * @var \League\Plates\Engine $Tpl
	 */

	$page = PageHelper::getPage('account.php');
	$page->setTitle('Homepage');

	if (!isAuthenticated($Db)) {
		$page->redirectTo('~/index.php');
	}

	$message      = "";
	$messageState = 'warn';
	$post         = $Stoic->getRequest()->getPost();

	if ($post->has('action')) {
		$events = new UserEvents($Db, $Log);
		$events->linkToEvent(UserEVentTypes::UPDATE, new EmailUserUpdateNode($page, $Settings, $Db, $Log));

		$postData = [
			'id'             => $User->id,
			'emailConfirmed' => false,
			'profile'        => [
				'birthday'     => $post->getString('birthday'),
				'description'  => $post->getString('description'),
				'displayName'  => $post->getString('displayName'),
				'gender'       => $post->getInt('gender'),
				'realName'     => $post->getString('realName')
			],
			'settings'       => [
				'htmlEmails'   => $post->has('set_htmlEmails'),
				'playSounds'   => $post->has('set_playSounds')
			],
			'visibilities'   => [
				'birthday'     => $post->getInt('vis_birthday'),
				'description'  => $post->getInt('vis_description'),
				'email'        => $post->getInt('vis_email'),
				'gender'       => $post->getInt('vis_gender'),
				'profile'      => $post->getInt('vis_profile'),
				'realName'     => $post->getInt('vis_realName'),
				'searches'     => $post->getInt('vis_searches')
			]
		];

		if (!empty($post->getString('confirmEmail'))) {
			$postData['email']        = $post->getString('email');
			$postData['confirmEmail'] = $post->getString('confirmEmail');
		}

		if (!empty($post->getString('password')) && !empty($post->getString('confirmPassword'))) {
			$postData['key']        = $post->getString('password');
			$postData['oldKey']     = $post->getString('oldPassword');
			$postData['confirmKey'] = $post->getString('confirmPassword');
		}

		$update = $events->doUpdate(new ParameterHelper($postData));

		if ($update->isGood()) {
			$messageState = 'success';
		}

		$message = implode("<br />", $update->getMessages());
	}

	$Tpl->addFolder('page', STOIC_CORE_PATH . '/tpl/account');

	echo($Tpl->render('page::index', [
		'message'      => $message,
		'messageState' => $messageState,
		'page'         => $page,
		'user'         => User::fromId($User->id, $Db, $Log),
		'profile'      => UserProfile::fromUser($User->id, $Db, $Log),
		'userSettings' => UserSettings::fromUser($User->id, $Db, $Log),
		'visibilities' => UserVisibilities::fromUser($User->id, $Db, $Log)
	]));
