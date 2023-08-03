<?php

	/**
	 * @var \Stoic\Web\PageHelper $page
	 * @var \Zibings\UserProfile $profile
	 * @var \AndyM84\Config\ConfigContainer $settings
	 * @var \Zibings\UserRoles $userRoles
	 * @var \Zibings\User $user
	 */

	use Zibings\RoleStrings;
	use Zibings\SettingsStrings;

?>
<!doctype html>

<html lang="en" class="h-100">
	<head>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
		<meta name="description" content="" />
		<title><?=$page->getTitle()?></title>

		<!-- Bootstrap core CSS -->
		<link href="<?=$page->getAssetPath('~/assets/css/bootstrap.min.css')?>" rel="stylesheet" crossorigin="anonymous" />

		<!-- Favicons -->
		<link rel="apple-touch-icon" href="<?=$page->getAssetPath('~/assets/img/favicons/apple-touch-icon.png')?>" sizes="180x180" />
		<link rel="icon" href="<?=$page->getAssetPath('~/assets/img/favicons/favicon-32x32.png')?>" sizes="32x32" type="image/png" />
		<link rel="icon" href="<?=$page->getAssetPath('~/assets/img/favicons/favicon-16x16.png')?>" sizes="16x16" type="image/png" />
		<link rel="mask-icon" href="<?=$page->getAssetPath('~/assets/img/favicons/safari-pinned-tab.svg')?>" color="#563d7c" />
		<link rel="icon" href="<?=$page->getAssetPath('~/assets/img/favicons/favicon.ico')?>" />
		<meta name="msapplication-config" content="<?=$page->getAssetPath('~/assets/img/favicons/browserconfig.xml')?>" />
		<meta name="theme-color" content="#563d7c" />

		<style>
			.bd-placeholder-img {
        font-size: 1.125rem;
        text-anchor: middle;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
      }

      @media (min-width: 768px) {
        .bd-placeholder-img-lg {
          font-size: 3.5rem;
        }
      }
		</style>

		<!-- Custom styles for this template -->
		<link href="<?=$page->getAssetPath('~/assets/css/main.css')?>" rel="stylesheet" />

		<!-- Vite App -->
		<?=Zibings\viteInitApp('front.js', $page, $settings)?>

<?=$this->section('stylesheets')?>
	</head>

	<body class="d-flex flex-column h-100">
		<header>
			<!-- Fixed navbar -->
			<nav class="navbar navbar-expand-md navbar-dark fixed-top bg-dark">
				<a class="navbar-brand me-2" href="<?=$page->getAssetPath('~/home.php')?>"><?=$settings->get(SettingsStrings::SITE_NAME, 'ZSF')?></a>
				
				<button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarCollapse" aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
					<span class="navbar-toggler-icon"></span>
				</button>
				
				<div class="collapse navbar-collapse" id="navbarCollapse">
					<ul class="navbar-nav mr-auto me-auto">
						<li class="nav-item active">
							<a class="nav-link" href="#">Home <span class="sr-only">(current)</span></a>
						</li>
						
						<li class="nav-item">
							<a class="nav-link" href="#">Link</a>
						</li>

<?php if ($user->id > 0 && $userRoles->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR)): ?>						<li class="nav-item">
							<a class="nav-link" href="<?=$page->getAssetPath('~/admin/')?>" tabindex="-1">Administration</a>
						</li>
<?php endif; ?>
					</ul>
					
					<form class="d-flex form-inline mt-2 mt-md-0">
						<input class="form-control mr-sm-2 me-2" type="text" placeholder="Search" aria-label="Search">
						<button class="btn btn-outline-success my-2 my-sm-0 me-2" type="submit">Search</button>

						<button type="button" class="btn btn-outline-info my-2 my-sm-0 ml-3 me-2" onclick="location.href = '<?=$page->getAssetPath('~/account.php')?>';">Account</button>
						<button type="button" class="btn btn-outline-danger my-2 my-sm-0 ml-3 me-2" onclick="location.href = '<?=$page->getAssetPath('~/logout.php')?>';">Logout</button>
					</form>
				</div>
			</nav>
		</header>

		<!-- Begin page content -->
		<main role="main" class="flex-shrink-0">
			<?=$this->section('content')?>
		</main>

		<footer class="footer mt-auto py-3">
			<div class="container">
				<span class="text-muted">&copy; <?=(new \DateTime('now', new DateTimeZone('UTC')))->format('Y')?> <?=$settings->get(SettingsStrings::SITE_NAME, 'ZSF')?> - System Version <?=$settings->get(SettingsStrings::SYSTEM_VERSION)?></span>
			</div>
		</footer>

		<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js" crossorigin="anonymous"></script>
		<script>window.jQuery || document.write('<script src="<?=$page->getAssetPath('~/assets/js/vendor/jquery.slim.min.js')?>"><\/script>')</script>
		<script src="<?=$page->getAssetPath('~/assets/js/bootstrap.bundle.min.js')?>" crossorigin="anonymous"></script>
		<script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>

<?=$this->section('scripts')?>
	</body>
</html>