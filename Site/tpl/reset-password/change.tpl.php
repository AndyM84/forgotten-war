<?php

	/**
	 * @var Stoic\Utilities\ParameterHelper $get
	 * @var Stoic\Utilities\ParameterHelper $post
	 * @var Stoic\Web\PageHelper $page
	 */

?>
<?php $this->layout('shared::auth-master', ['page' => $page]); ?>

		<form class="form-register" method="post" action="<?=$page->getAssetPath('~/reset-password.php')?>">
			<input type="hidden" name="token" value="<?=$get->getString('token', '00000000-0000-0000-0000-000000000000')?>" />

			<img class="mb-4" src="<?=$page->getAssetPath('~/assets/img/bootstrap-solid.svg')?>" alt="" width="72" height="72">

			<h1 class="h3 mb-3 font-weight-normal">Reset Your Password</h1>

<?php if (isset($message)): ?>			<div class="mb-3 text-center">
				<div class="alert alert-warning" role="alert">
					<?=$message?>
				</div>
			</div>
<?php endif; ?>

			<label for="inputPassword" class="sr-only">Password</label>
			<input type="password" id="inputPassword" name="password" class="form-control top" placeholder="Password" required />
			
			<label for="inputConfirmPassword" class="sr-only">Confirm Password</label>
			<input type="password" id="inputConfirmPassword" name="confirmPassword" class="form-control bottom" placeholder="Confirm password" required />
			
			<button class="btn btn-lg btn-primary btn-block" type="submit">Reset</button>
		</form>