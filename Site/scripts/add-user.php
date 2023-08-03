<?php

	error_reporting(E_ALL);
	ini_set('display_errors', 'On');

	define('CORE_PATH', './');
	require(CORE_PATH . 'inc/core.php');

	use Stoic\Utilities\ConsoleHelper;
	use Stoic\Utilities\LogFileAppender;
	use Stoic\Utilities\ParameterHelper;
	use Zibings\CliScriptHelper;
	use Zibings\LoginKey;
	use Zibings\LoginKeyProviders;
	use Zibings\RoleStrings;
	use Zibings\User;
	use Zibings\UserEvents;
	use Zibings\UserProfile;
	use Zibings\UserRoles;

	global $Db, $Log, $Settings, $Stoic;

	/**
	 * @var \Stoic\Pdo\PdoHelper            $Db
	 * @var \Stoic\Log\Logger               $Log
	 * @var \AndyM84\Config\ConfigContainer $Settings
	 * @var \Stoic\Web\Stoic                $Stoic
	 */

	$ch     = new ConsoleHelper($argv);
	$script = (new CliScriptHelper(
		"ZSF Add User Script",
		"Script to add a user to the system without having access to the site's administrative pages."
	))->addExample(
		<<< EXAMPLE
- Run script so it prompts for input

   php scripts/add-user.php
EXAMPLE
	)->addExample(
		<<< EXAMPLE
- Run script with all values supplied, creating new user and making them an
     administrator without asking for input

   php scripts/add-user.php --non-interactive --email test@domain.com \
      --name JohnDoe --password P@55word --make-admin
EXAMPLE
	)->addOption(
		"non-interactive",
		"ni",
		"non-interactive",
		"Runs script without asking for user input",
		"Attempts to add user without prompting user for input. Requires "
	)->addOption(
		"email",
		"e",
		"email",
		"Sets user's email address",
		"Sets the user's email address, but does not validate or request confirmation"
	)->addOption(
		"name",
		"n",
		"name",
		"Sets user's display name",
		"Sets the user's display name, shouldn't have spaces and should be longer than 3 characters"
	)->addOption(
		"password",
		"p",
		"password",
		"Sets user's password",
		"Sets the user's password, should be at least 6 characters long"
	)->addOption(
		"make-admin",
		"ma",
		"make-admin",
		"Makes user an administrator",
		"Gives the user the administrator role"
	);

	$data       = [];
	$makeAdmin  = false;
	$user       = new User($Db, $Log);
	$userRoles  = new UserRoles($Db, $Log);
	$userEvents = new UserEvents($Db, $Log);
	$opts       = $script->startScript($ch)->getOptions($ch);

	if ($ch->hasShortLongArg('ni', 'non-interactive')) {
		if (!$ch->hasShortLongArg('e', 'email') || !$ch->hasShortLongArg('n', 'name') || !$ch->hasShortLongArg('p', 'password')) {
			$script->showBasicHelp($ch);

			exit;
		}

		$data['email']          = $opts['email'];
		$data['key']            = $opts['password'];
		$data['displayName']    = $opts['name'];
		$makeAdmin              = $ch->getParameterWithDefault('ma', 'make-admin', false);
	} else {
		$yesNoSanitation = function (mixed $input) : bool { return strtolower($input) == 'y'; };
		$yesNoValidation = function (mixed $input) : bool { return strtolower($input) == 'y' || strtolower($input) == 'n'; };

		$email = $ch->getQueriedInput("User's Email Address", null, "Invalid email address", 5, function (mixed $input) : bool { return User::validEmail($input); });

		if ($email->isBad()) {
			$ch->putLine();
			$ch->putLine("Aborting after input error");

			exit;
		}

		$data['email'] = $email->getResults()[0];

		$name = $ch->getQueriedInput("Username", null, "Invalid name", 5, function ($input) { return UserProfile::validDisplayName($input); });

		if ($name->isBad()) {
			$ch->putLine();
			$ch->putLine("Aborting after input error.");

			exit;
		}

		$data['displayName'] = $name->getResults()[0];

		$password = $ch->getQueriedInput("User's Password", null, "Invalid password", 5, function ($input) { return !empty($input); });

		if ($password->isBad()) {
			$ch->putLine();
			$ch->putLine("Aborting after input error");

			exit;
		}

		$data['key'] = $password->getResults()[0];

		$admin = $ch->getQueriedInput("Make Admin (Y/n)", 'N', "Invalid value for admin option, can be 'y' or 'n'", 5, $yesNoValidation, $yesNoSanitation);

		if ($admin->isBad()) {
			$ch->putLine();
			$ch->putLine("Aborting after input error");

			exit;
		}

		$makeAdmin = $admin->getResults()[0];

		$ch->putLine();
	}

	$data['emailConfirmed'] = true;
	$data['confirmKey']     = $data['key'];
	$data['provider']       = LoginKeyProviders::PASSWORD;

	$ch->put("Creating user account.. ");
	$uCreate = $userEvents->doCreate(new ParameterHelper($data));

	if ($uCreate->isBad()) {
		$ch->putLine("ERROR");
		$ch->putLine($uCreate->getMessages()[0]);
		$ch->putLine();

		exit;
	}

	$ch->putLine("DONE");

	$user = $uCreate->getResults()[0]['data'];

	$ch->put("Creating user profile.. ");

	$profile              = UserProfile::fromUser($user->id, $Db, $Log);
	$profile->displayName = $data['displayName'];
	$pUpdate              = $profile->update();

	if ($pUpdate->isBad()) {
		$ch->putLine("ERROR");
		$ch->putLine($pUpdate->getMessages()[0]);
		$ch->putLine();

		exit;
	}

	$ch->putLine("DONE");

	if ($makeAdmin) {
		$ch->put("Assigning administrator role.. ");

		if (!$userRoles->addUserToRoleByName($user->id, RoleStrings::ADMINISTRATOR)) {
			$ch->putLine("ERROR");
			$ch->putLine();

			exit;
		}

		$ch->putLine("DONE");
	}

	$ch->putLine();
	$ch->putLine("Finished creating user");
	$ch->putLine();
