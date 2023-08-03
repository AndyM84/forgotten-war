<?php

	/**
	 * @var Zibings\User $user
	 * @var Stoic\Web\PageHelper $page
	 * @var \Zibings\UserProfile $profile
	 */

?>
<?php $this->layout('shared::main-master', ['page' => $page]); ?>

		<div class="container">
			<h1 class="mt-5">Welcome, <?=$profile->displayName?></h1>

			<p class="lead">
				Who do you want to fight today?
			</p>

			<div class="vue-app">
				<hello-world msg="component"></hello-world>
			</div>
		</div>