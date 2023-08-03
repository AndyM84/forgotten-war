<?php

	require('inc/core.php');

	use Stoic\Web\PageHelper;

	use Zibings\SettingsStrings;
	use function Zibings\getCurlApiResource;

	global $Db, $Settings, $Stoic, $Tpl, $User;

	/**
	 * @var AndyM84\Config\ConfigContainer $Settings
	 * @var Stoic\Web\Stoic $Stoic
	 * @var League\Plates\Engine $Tpl
	 * @var Zibings\User $User
	 */

	$get = $Stoic->getRequest()->getGet();
	$page = PageHelper::getPage('callback.php');

	if ($User->id > 0) {
		$page->redirectTo('~/');
	}

	if (!$get->has('provider')) {
		$page->redirectTo('~/?err=CALLBACK_ONE');
	}

	$provider = $get->getString('provider', '');

	if ($provider !== 'TwitchTV' && $provider !== 'Twitter' && $provider !== 'Facebook') {
		$page->redirectTo('~/?err=CALLBACK_TWO');
	}

	$usernamePrefixes = [
		'TwitchTV' => 'TTV_',
		'Twitter'  => 'TW_',
		'Facebook' => 'FB_',
		'Github'   => 'GH_'
	];

	$config = [
		'callback' => $page->getAssetPath('~/callback.php?provider=' . $provider, null, true)->data(),

		'providers'    => [
			'TwitchTV'   => [
				'enabled'  => false,
				'keys'     => [
					'key'    => $Settings->get(SettingsStrings::TWITCH_KEY, ''),
					'secret' => $Settings->get(SettingsStrings::TWITCH_SECRET, '')
				]
			],
			'Twitter'    => [
				'enabled'  => false,
				'keys'     => [
					'key'    => $Settings->get(SettingsStrings::TWITTER_KEY, ''),
					'secret' => $Settings->get(SettingsStrings::TWITTER_SECRET, '')
				]
			],
			'Facebook'   => [
				'enabled'  => false,
				'keys'     => [
					'id'     => $Settings->get(SettingsStrings::FACEBOOK_KEY, ''),
					'secret' => $Settings->get(SettingsStrings::FACEBOOK_SECRET, '')
				]
			],
			'GitHub'     => [
				'enabled'  => false,
				'keys'     => [
					'id'     => $Settings->get(SettingsStrings::GITHUB_KEY, ''),
					'secret' => $Settings->get(SettingsStrings::GITHUB_SECRET, '')
				]
			],
			'Reddit'     => [
				'enabled'  => false,
				'keys'     => [
					'id'     => $Settings->get(SettingsStrings::REDDIT_KEY, ''),
					'secret' => $Settings->get(SettingsStrings::REDDIT_SECRET, '')
				]
			]
		]
	];

	try {
		$hybridauth = new Hybridauth\Hybridauth($config);

		$adapter = $hybridauth->authenticate($provider);

		if (!$adapter->isConnected()) {
			echo("There was a serious issue");

			exit;
		}

		$userProfile = $adapter->getUserProfile();
		$adapter->disconnect();

		if (empty($userProfile->email)) {
			$page->redirectTo('~/index.php?error=CALLBACK_INVALID_EMAIL');
		}

		$postData = [
			'Username'    => "{$usernamePrefixes[$provider]}{$userProfile->displayName}",
			'Description' => $userProfile->description,
			'Email'       => $userProfile->email,
			'Password'    => 'ea-' . sha1($userProfile->identifier . "--" . $config['providers'][$provider]['keys']['id'])
		];

		$ch = getCurlApiResource("Account/ExternalAuth", 0, '', $Settings, true, $postData);
		$resp = json_decode(curl_exec($ch), true);
		$meta = curl_getinfo($ch);
		curl_close($ch);

		if ($meta['http_code'] != 200 || $resp['status'] > 0) {
			$page->redirectTo('~/Account/Login?error=CALLBACK_BAD_RESPONSE&code=' . $meta['http_code'] . '&status=' . $resp['status']);
		}

		// TODO: Fix this, same as getCurlApiResource approach (use SS as example)
		setcookie(CookieStrings::SESSION_TOKEN, base64_encode("{$resp['userID']}:{$resp['token']}"), (time() + 60*60*24*30), '/', $_SERVER['HTTP_HOST'], (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off') ? true : false);

		$page->redirectTo('~/');
	} catch(\Exception $e) {
		echo('Oops, we ran into an issue! ' . $e->getMessage());
	}
