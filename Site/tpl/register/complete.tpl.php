<?php

	/* @var Stoic\Web\PageHelper $page */

?>
<?php $this->layout('shared::auth-master', ['page' => $page]); ?>

		<form class="form-register">
			<img class="mb-4" src="<?=$page->getAssetPath('~/assets/img/bootstrap-solid.svg')?>" alt="" width="72" height="72">

			<div class="mt-3 text-muted text-center">
				Thanks for registering!  You will receive an email shortly asking you to confirm your email address, so check your inbox and get ready
				to enjoy all that we have to offer.
			</div>

			<div class="mt-3 text-muted text-center">
				If you don't receive your email within a few minutes, check your SPAM folder before contacting us.
			</div>

			<div class="mt-5 text-center">
				<button type="button" class="btn btn-outline-dark" onclick="location.href = '<?=$page->getAssetPath('~/index.php')?>';">Back to Login</button>
			</div>
		</form>