<?php

	/**
	 * @var \Zibings\User $user
	 * @var \Zibings\User $currentUser
	 * @var \Stoic\Utilities\ParameterHelper $get
	 * @var \Stoic\Web\PageHelper $page
	 * @var \Zibings\UserProfile $profile
	 * @var \Zibings\UserVisibilities $visibilities
	 * @var \Zibings\UserSettings $userSettings
	 * @var \Zibings\Roles $roles
	 * @var \Zibings\Role[] $userRoles
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
<?php $this->layout('shared::admin-master', ['page' => $page]); ?>

						<!-- start page title -->
						<div class="row">
							<div class="col-12">
								<div class="page-title-box">
									<div class="page-title-right">
										<form class="form-inline">
											<a href="javascript: document.forms['form-account'].submit();" class="btn btn-outline-success ml-1">
												<?=($get->getString('action') == 'create') ? 'Create' : 'Save'?>
												<i class="mdi mdi-content-save"></i>
											</a>
<?php if ($get->getString('action') == 'edit'): ?>											<a href="javascript: void(0);" class="btn btn-outline-danger ml-1">
												Delete
												<i class="mdi mdi-trash-can-outline"></i>
											</a>
<?php endif; ?>
										</form>
									</div>
									<h4 class="page-title">
										<a href="<?=$page->getAssetPath('~/admin/users.php')?>">User Management</a>
										&rarr; <?=($get->getString('action') == 'create') ? 'Create New' : "Edit {$profile->displayName}"?></h4>
								</div>
							</div>
						</div>
						<!-- end page title -->

<?php if (!empty($message) && !empty($messageState)): ?>			<div class="mb-3 text-center">
								<div class="alert alert-<?=$messageState?>" role="alert">
									<?=$message?>
									<button type="button" class="close" data-dismiss="alert" aria-label="Close">
										<span aria-hidden="true">&times;</span>
									</button>
								</div>
							</div>
<?php endif; ?>

						<div class="row">

							<div class="col-12">
								<div class="card">
									<div class="card-body">
										<form class="form-account" name="form-account" method="post" action="<?=$page->getAssetPath('~/admin/users.php', ['id' => $currentUser->id, 'action' => $get->getString('action')])?>">
											<input type="hidden" name="action" value="<?=$get->getString('action')?>" />

											<div class="row">
												<div class="col-4">
													<h5>Account Info</h5>

													<div class="form-group">
														<label for="email">Email Address</label>
														<input type="email" name="email" id="email" class="form-control" aria-describedby="emailHelp" value="<?=$currentUser->email?>" required />
														<small id="emailHelp" class="form-text text-muted">User's email address</small>
													</div>

													<div class="form-group">
														<label for="oldPassword">Password</label>
														<input type="password" name="password" id="password" class="form-control" aria-describedby="passwordHelp" />
														<small id="passwordHelp" class="form-text text-muted">Set/change user's password</small>
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

													<h5 class="pt-3">Roles</h5>

													<div class="form-group">
														<label for="userRoles">Active Roles</label>
														<select name="userRoles[]" id="userRoles" class="form-control select2 select2-multiple" data-toggle="select2" multiple="multiple">
<?php foreach ($roles->getAll() as $role): ?>															<option value="<?=$role->name?>"<?php if (array_key_exists($role->id, $userRoles) !== false): ?> selected<?php endif; ?>><?=$role->name?></option>
<?php endforeach; ?>
														</select>
													</div>
												</div>

												<div class="col-4">
													<h5>Profile Info</h5>

													<div class="form-group">
														<label for="displayName">Username</label>
														<input type="text" name="displayName" id="displayName" class="form-control" aria-describedby="displayNameHelp" value="<?=$profile->displayName?>" />
														<small id="displayNameHelp" class="form-text text-muted">Name they'll appear with on the website</small>
													</div>

													<div class="form-group">
														<label for="realName">Real Name</label>
														<input type="text" name="realName" id="realName" class="form-control" aria-describedby="realNameHelp" value="<?=$profile->realName?>" />
														<small id="realNameHelp" class="form-text text-muted">Their real name</small>
													</div>

													<div class="form-group">
														<label for="description">About Me</label>
														<textarea name="description" id="description" class="form-control" aria-describedby="descriptionHelp"><?=$profile->description?></textarea>
														<small id="descriptionHelp" class="form-text text-muted">A little bit about them</small>
													</div>

													<div class="form-group">
														<label for="gender">Gender</label>
														<select name="gender" id="gender" class="form-control" aria-describedby="genderHelp">
															<option value="<?=UserGenders::NONE?>"<?php if ($profile->gender->is(UserGenders::NONE)): ?> selected<?php endif; ?>>None</option>
															<option value="<?=UserGenders::FEMALE?>"<?php if ($profile->gender->is(UserGenders::FEMALE)): ?> selected<?php endif; ?>>Female</option>
															<option value="<?=UserGenders::MALE?>"<?php if ($profile->gender->is(UserGenders::MALE)): ?> selected<?php endif; ?>>Male</option>
															<option value="<?=UserGenders::OTHER?>"<?php if ($profile->gender->is(UserGenders::OTHER)): ?> selected<?php endif; ?>>Other</option>
														</select>
														<small id="genderHelp" class="form-text text-muted">Their preferred gender identification</small>
													</div>

													<div class="form-group">
														<label for="birthday">Birthday</label>
														<input type="date" name="birthday" id="birthday" class="form-control" value="<?=$profile->birthday->format('Y-m-d')?>" />
														<small id="birthdayHelp" class="form-text text-muted">Their date of birth</small>
													</div>
												</div>

												<div class="col-4">
													<h5>Visibility</h5>

<?php foreach ($visibilitySettings as $vis): ?>						<div class="form-group">
														<label for="<?=$vis['id']?>"><?=$vis['label']?></label>
														<select name="<?=$vis['id']?>" id="<?=$vis['id']?>" class="form-control" disabled>
															<option value="<?=VisibilityState::PRV?>"<?php if ($vis['selected']->is(VisibilityState::PRV)): ?> selected<?php endif; ?>>Only Them</option>
															<option value="<?=VisibilityState::FRIENDS?>"<?php if ($vis['selected']->is(VisibilityState::FRIENDS)): ?> selected<?php endif; ?>>Friends</option>
															<option value="<?=VisibilityState::AUTHENTICATED?>"<?php if ($vis['selected']->is(VisibilityState::AUTHENTICATED)): ?> selected<?php endif; ?>>Any Authenticated Users</option>
															<option value="<?=VisibilityState::PUB?>"<?php if ($vis['selected']->is(VisibilityState::PUB)): ?> selected<?php endif; ?>>Anyone</option>
														</select>
													</div>
<?php endforeach; ?>
												</div>
											</div>
										</form>
									</div> <!-- end card-body-->
								</div> <!-- end card-->
							</div> <!-- end col-->
						</div>
						<!-- end row -->

<?php $this->push('scripts') ?>
		<script type="text/javascript">
			const doDelete = function (id) {
				if (confirm("This action cannot be undone, are you sure?")) {
					$.ajax({
						type: 'POST',
						url: utils.makeApiUrl('/Account/Delete'),
						headers: utils.ajaxHeaders(),
						data: JSON.stringify({ "id": id }),
						dataType: 'json',
						success: function (dat) {
							location.href = "<?=$page->getAssetPath('~/admin/users.php')?>";

							return;
						},
						error: function (xhr) {
							console.log(xhr);

							return;
						}
					});
				}
			};
		</script>
<?php $this->end() ?>

<?php $this->push('stylesheets') ?>

<?php $this->end() ?>
