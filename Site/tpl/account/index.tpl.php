<?php

	/**
	 * @var \Zibings\User $user
	 * @var \Stoic\Web\PageHelper $page
	 * @var \Zibings\UserProfile $profile
	 * @var \Zibings\UserVisibilities $visibilities
	 * @var \Zibings\UserSettings $userSettings
	 */

	use Zibings\UserGenders;
	use Zibings\VisibilityState;

	$visibilitySettings = [
		[ 'id' => 'vis_birthday',    'label' => 'Show Birthday To..',      'selected' => $visibilities->birthday ],
		[ 'id' => 'vis_description', 'label' => "Show 'About Me' To..",    'selected' => $visibilities->description ],
		[ 'id' => 'vis_email',       'label' => 'Show Email Address To..', 'selected' => $visibilities->email ],
		[ 'id' => 'vis_gender',      'label' => 'Show Gender To..',        'selected' => $visibilities->gender ],
		[ 'id' => 'vis_profile',     'label' => 'Show Profile To..',       'selected' => $visibilities->profile ],
		[ 'id' => 'vis_realName',    'label' => 'Show Real Name To..',     'selected' => $visibilities->realName ],
		[ 'id' => 'vis_searches',    'label' => 'Show In Searches To..',   'selected' => $visibilities->searches ]
	];

?>
<?php $this->layout('shared::main-master', ['page' => $page]); ?>

		<div class="container">
<?php if (!empty($message) && !empty($messageState)): ?>			<div class="mb-3 text-center">
				<div class="alert alert-<?=$messageState?>" role="alert">
					<?=$message?>
					<button type="button" class="close" data-dismiss="alert" aria-label="Close">
						<span aria-hidden="true">&times;</span>
					</button>
				</div>
			</div>
<?php endif; ?>

			<form class="form-account" method="post" action="<?=$page->getAssetPath('~/account.php')?>">
				<input type="hidden" name="action" value="save" />

				<div class="form-group text-right">
					<button type="submit" class="btn btn-outline-dark">Save</button>
				</div>

				<div class="row">
					<div class="col-4">
						<h5>Account Info</h5>

						<div class="form-group">
							<label for="email">Email Address</label>
							<input type="email" name="email" id="email" class="form-control" aria-describedby="emailHelp" value="<?=$user->email?>" required />
							<small id="emailHelp" class="form-text text-muted">Your email address, must confirm if changing</small>
						</div>

						<div class="form-group">
							<label for="confirmEmail">Confirm Email</label>
							<input type="email" name="confirmEmail" id="confirmEmail" class="form-control" aria-describedby="confirmEmailHelp" />
							<small id="confirmEmailHelp" class="form-text text-muted">Only required if changing your email address</small>
						</div>

						<div class="form-group">
							<label for="oldPassword">Old Password</label>
							<input type="password" name="oldPassword" id="oldPassword" class="form-control" aria-describedby="oldPasswordHelp" />
							<small id="oldPasswordHelp" class="form-text text-muted">Only required if changing your password</small>
						</div>

						<div class="form-group">
							<label for="password">New Password</label>
							<input type="password" name="password" id="password" class="form-control" aria-describedby="passwordHelp" />
							<small id="passwordHelp" class="form-text text-muted">Only required if changing your password</small>
						</div>

						<div class="form-group">
							<label for="confirmPassword">Confirm Password</label>
							<input type="password" name="confirmPassword" id="confirmPassword" class="form-control" aria-describedby="confirmPasswordHelp" />
							<small id="confirmPasswordHelp" class="form-text text-muted">Only required if changing your password</small>
						</div>

						<h5 class="pt-3">Settings</h5>

						<div class="form-group">
							<div class="custom-control custom-switch">
								<input type="checkbox" class="custom-control-input" name="set_htmlEmails" id="set_htmlEmails"<?php if ($userSettings->htmlEmails): ?> checked<?php endif; ?> />
								<label class="custom-control-label" for="set_htmlEmails">Receive HTML emails</label>
							</div>

							<div class="custom-control custom-switch">
								<input type="checkbox" class="custom-control-input" name="set_playSounds" id="set_playSounds"<?php if ($userSettings->playSounds): ?> checked<?php endif; ?> />
								<label class="custom-control-label" for="set_playSounds">Play sounds in browser</label>
							</div>
						</div>
					</div>

					<div class="col-4">
						<h5>Profile Info</h5>

						<div class="form-group">
							<label for="displayName">Username</label>
							<input type="text" name="displayName" id="displayName" class="form-control" aria-describedby="displayNameHelp" value="<?=$profile->displayName?>" />
							<small id="displayNameHelp" class="form-text text-muted">Name you'll appear with on the website</small>
						</div>

						<div class="form-group">
							<label for="realName">Real Name</label>
							<input type="text" name="realName" id="realName" class="form-control" aria-describedby="realNameHelp" value="<?=$profile->realName?>" />
							<small id="realNameHelp" class="form-text text-muted">Your real name, if you'd like to share</small>
						</div>

						<div class="form-group">
							<label for="description">About Me</label>
							<textarea name="description" id="description" class="form-control" aria-describedby="descriptionHelp"><?=$profile->description?></textarea>
							<small id="descriptionHelp" class="form-text text-muted">A little bit about yourself</small>
						</div>

						<div class="form-group">
							<label for="gender">Gender</label>
							<select name="gender" id="gender" class="form-control" aria-describedby="genderHelp">
								<option value="<?=UserGenders::NONE?>"<?php if ($profile->gender->is(UserGenders::NONE)): ?> selected<?php endif; ?>>None</option>
								<option value="<?=UserGenders::FEMALE?>"<?php if ($profile->gender->is(UserGenders::FEMALE)): ?> selected<?php endif; ?>>Female</option>
								<option value="<?=UserGenders::MALE?>"<?php if ($profile->gender->is(UserGenders::MALE)): ?> selected<?php endif; ?>>Male</option>
								<option value="<?=UserGenders::OTHER?>"<?php if ($profile->gender->is(UserGenders::OTHER)): ?> selected<?php endif; ?>>Other</option>
							</select>
							<small id="genderHelp" class="form-text text-muted">Your preferred gender identification</small>
						</div>

						<div class="form-group">
							<label for="birthday">Birthday</label>
							<input type="date" name="birthday" id="birthday" class="form-control" value="<?=$profile->birthday->format('Y-m-d')?>" />
							<small id="birthdayHelp" class="form-text text-muted">Your date of birth, if you'd like to share</small>
						</div>
					</div>

					<div class="col-4">
						<h5>Visibility</h5>

<?php foreach ($visibilitySettings as $vis): ?>						<div class="form-group">
							<label for="<?=$vis['id']?>"><?=$vis['label']?></label>
							<select name="<?=$vis['id']?>" id="<?=$vis['id']?>" class="form-control">
								<option value="<?=VisibilityState::PRV?>"<?php if ($vis['selected']->is(VisibilityState::PRV)): ?> selected<?php endif; ?>>Only Me</option>
								<option value="<?=VisibilityState::FRIENDS?>"<?php if ($vis['selected']->is(VisibilityState::FRIENDS)): ?> selected<?php endif; ?>>Friends</option>
								<option value="<?=VisibilityState::AUTHENTICATED?>"<?php if ($vis['selected']->is(VisibilityState::AUTHENTICATED)): ?> selected<?php endif; ?>>Any Authenticated Users</option>
								<option value="<?=VisibilityState::PUB?>"<?php if ($vis['selected']->is(VisibilityState::PUB)): ?> selected<?php endif; ?>>Anyone</option>
							</select>
						</div>
<?php endforeach; ?>
					</div>
				</div>

				<div class="form-group text-right">
					<button type="submit" class="btn btn-outline-dark">Save</button>
				</div>
			</form>
		</div>