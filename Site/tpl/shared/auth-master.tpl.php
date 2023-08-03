<?php

	/* @var Stoic\Web\PageHelper $page */

?>
<!doctype html>

<html lang="en">
	<head>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
		<meta name="description" content="">
		<meta name="author" content="Mark Otto, Jacob Thornton, and Bootstrap contributors">
		<meta name="generator" content="Hugo 0.80.0">
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
		<link href="<?=$page->getAssetPath('~/assets/css/auth.css')?>" rel="stylesheet" />
<?=$this->section('stylesheets')?>
	</head>

	<body class="text-center">
		<?=$this->section('content')?>

<?=$this->section('scripts')?>
	</body>
</html>