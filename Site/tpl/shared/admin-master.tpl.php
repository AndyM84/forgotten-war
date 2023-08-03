<?php

	/**
	 * @var \Stoic\Web\PageHelper           $page
	 * @var \Zibings\UserProfile            $profile
	 * @var \AndyM84\Config\ConfigContainer $settings
	 * @var \Zibings\UserRoles              $userRoles
	 * @var \Zibings\User                   $user
	 */

	use Zibings\SettingsStrings;
	use Zibings\UserEvents;

?>
<!DOCTYPE html>

<html lang="en">
	<head>
		<meta charset="utf-8" />
		<title><?=$page->getTitle()?></title>
		<meta name="viewport" content="width=device-width, initial-scale=1.0">

		<!-- App favicon -->
		<link rel="shortcut icon" href="<?=$page->getAssetPath('~/admin/assets/images/favicon.ico')?>">

		<!-- third party css -->
		<link href="<?=$page->getAssetPath('~/admin/assets/css/vendor/jquery-jvectormap-1.2.2.css')?>" rel="stylesheet" type="text/css" />
		<!-- third party css end -->

		<!-- App css -->
		<link href="<?=$page->getAssetPath('~/admin/assets/css/icons.min.css')?>" rel="stylesheet" type="text/css" />
		<link href="<?=$page->getAssetPath('~/admin/assets/css/app.min.css')?>" rel="stylesheet" type="text/css" id="app-style" />

		<!-- Vite App -->
		<?=Zibings\viteInitApp('admin.js', $page, $settings)?>

<?=$this->section('stylesheets')?>
	</head>

	<body class="loading" data-layout-color="dark" data-leftbar-theme="dark" data-layout-mode="fluid" data-rightbar-onstart="true">
		<!-- Begin page -->
		<div class="wrapper">
			<!-- ========== Left Sidebar Start ========== -->
			<div class="leftside-menu">
				<!-- LOGO -->
				<a href="<?=$page->getAssetPath('~/admin/')?>" class="logo text-center logo-light">
					<span class="logo-lg">
						<img src="<?=$page->getAssetPath('~/admin/assets/images/logo.png')?>" alt="" height="16">
					</span>
					<span class="logo-sm">
						<img src="<?=$page->getAssetPath('~/admin/assets/images/logo_sm.png')?>" alt="" height="16">
					</span>
				</a>

				<!-- LOGO -->
				<a href="<?=$page->getAssetPath('~/admin/')?>" class="logo text-center logo-dark">
					<span class="logo-lg">
						<img src="<?=$page->getAssetPath('~/admin/assets/images/logo-dark.png')?>" alt="" height="16">
					</span>
					<span class="logo-sm">
						<img src="<?=$page->getAssetPath('~/admin/assets/images/logo_sm_dark.png')?>" alt="" height="16">
					</span>
				</a>

				<div class="h-100" id="leftside-menu-container" data-simplebar>
					<!--- Sidemenu -->
					<ul class="side-nav">
						<li class="side-nav-title side-nav-item">Navigation</li>

						<li class="side-nav-item">
							<a href="<?=$page->getAssetPath('~/admin/')?>" class="side-nav-link">
								<i class="uil-home-alt"></i>
								<span> Dashboard </span>
							</a>
						</li>

						<li class="side-nav-item">
							<a href="<?=$page->getAssetPath('~/admin/users.php')?>" class="side-nav-link">
								<i class="uil-user-circle"></i>
								<span> User Management </span>
							</a>
						</li>

						<li class="side-nav-title side-nav-item">External</li>

						<li class="side-nav-item">
							<a href="<?=$page->getAssetPath('~/')?>" class="side-nav-link">
								<i class="uil-window-maximize"></i>
								<span> Front-End </span>
							</a>
						</li>
					</ul>

					<!-- End Sidebar -->
					<div class="clearfix"></div>
				</div>
				<!-- Sidebar -left -->

			</div>
			<!-- Left Sidebar End -->

			<!-- ============================================================== -->
			<!-- Start Page Content here -->
			<!-- ============================================================== -->

			<div class="content-page">
				<div class="content">
					<!-- Topbar Start -->
					<div class="navbar-custom">
						<ul class="list-unstyled topbar-menu float-end mb-0">
							<li class="dropdown notification-list d-lg-none">
								<a class="nav-link dropdown-toggle arrow-none" data-bs-toggle="dropdown" href="#" role="button" aria-haspopup="false" aria-expanded="false">
									<i class="dripicons-search noti-icon"></i>
								</a>
								<div class="dropdown-menu dropdown-menu-animated dropdown-lg p-0">
									<form class="p-3">
										<input type="text" class="form-control" placeholder="Search ..." aria-label="Recipient's username">
									</form>
								</div>
							</li>
							<li class="dropdown notification-list topbar-dropdown">
								<a class="nav-link dropdown-toggle arrow-none" data-bs-toggle="dropdown" href="#" role="button" aria-haspopup="false" aria-expanded="false">
									<img src="assets/images/flags/us.jpg" alt="user-image" class="me-0 me-sm-1" height="12">
									<span class="align-middle d-none d-sm-inline-block">English</span> <i class="mdi mdi-chevron-down d-none d-sm-inline-block align-middle"></i>
								</a>
								<div class="dropdown-menu dropdown-menu-end dropdown-menu-animated topbar-dropdown-menu">

									<!-- item-->
									<a href="javascript:void(0);" class="dropdown-item notify-item">
										<img src="assets/images/flags/germany.jpg" alt="user-image" class="me-1" height="12"> <span class="align-middle">German</span>
									</a>

									<!-- item-->
									<a href="javascript:void(0);" class="dropdown-item notify-item">
										<img src="assets/images/flags/italy.jpg" alt="user-image" class="me-1" height="12"> <span class="align-middle">Italian</span>
									</a>

									<!-- item-->
									<a href="javascript:void(0);" class="dropdown-item notify-item">
										<img src="assets/images/flags/spain.jpg" alt="user-image" class="me-1" height="12"> <span class="align-middle">Spanish</span>
									</a>

									<!-- item-->
									<a href="javascript:void(0);" class="dropdown-item notify-item">
										<img src="assets/images/flags/russia.jpg" alt="user-image" class="me-1" height="12"> <span class="align-middle">Russian</span>
									</a>

								</div>
							</li>

							<li class="dropdown notification-list">
								<a class="nav-link dropdown-toggle arrow-none" data-bs-toggle="dropdown" href="#" role="button" aria-haspopup="false" aria-expanded="false">
									<i class="dripicons-bell noti-icon"></i>
									<span class="noti-icon-badge"></span>
								</a>
								<div class="dropdown-menu dropdown-menu-end dropdown-menu-animated dropdown-lg">

									<!-- item-->
									<div class="dropdown-item noti-title px-3">
										<h5 class="m-0">
                                            <span class="float-end">
                                                <a href="javascript: void(0);" class="text-dark">
                                                    <small>Clear All</small>
                                                </a>
                                            </span>Notification
										</h5>
									</div>

									<div class="px-3" style="max-height: 300px;" data-simplebar>

										<h5 class="text-muted font-13 fw-normal mt-0">Today</h5>
										<!-- item-->
										<a href="javascript:void(0);" class="dropdown-item p-0 notify-item card unread-noti shadow-none mb-2">
											<div class="card-body">
												<span class="float-end noti-close-btn text-muted"><i class="mdi mdi-close"></i></span>
												<div class="d-flex align-items-center">
													<div class="flex-shrink-0">
														<div class="notify-icon bg-primary">
															<i class="mdi mdi-comment-account-outline"></i>
														</div>
													</div>
													<div class="flex-grow-1 text-truncate ms-2">
														<h5 class="noti-item-title fw-semibold font-14">Datacorp <small class="fw-normal text-muted ms-1">1 min ago</small></h5>
														<small class="noti-item-subtitle text-muted">Caleb Flakelar commented on Admin</small>
													</div>
												</div>
											</div>
										</a>

										<!-- item-->
										<a href="javascript:void(0);" class="dropdown-item p-0 notify-item card read-noti shadow-none mb-2">
											<div class="card-body">
												<span class="float-end noti-close-btn text-muted"><i class="mdi mdi-close"></i></span>
												<div class="d-flex align-items-center">
													<div class="flex-shrink-0">
														<div class="notify-icon bg-info">
															<i class="mdi mdi-account-plus"></i>
														</div>
													</div>
													<div class="flex-grow-1 text-truncate ms-2">
														<h5 class="noti-item-title fw-semibold font-14">Admin <small class="fw-normal text-muted ms-1">1 hours ago</small></h5>
														<small class="noti-item-subtitle text-muted">New user registered</small>
													</div>
												</div>
											</div>
										</a>

										<h5 class="text-muted font-13 fw-normal mt-0">Yesterday</h5>

										<!-- item-->
										<a href="javascript:void(0);" class="dropdown-item p-0 notify-item card read-noti shadow-none mb-2">
											<div class="card-body">
												<span class="float-end noti-close-btn text-muted"><i class="mdi mdi-close"></i></span>
												<div class="d-flex align-items-center">
													<div class="flex-shrink-0">
														<div class="notify-icon">
															<img src="assets/images/users/avatar-2.jpg" class="img-fluid rounded-circle" alt="" />
														</div>
													</div>
													<div class="flex-grow-1 text-truncate ms-2">
														<h5 class="noti-item-title fw-semibold font-14">Cristina Pride <small class="fw-normal text-muted ms-1">1 day ago</small></h5>
														<small class="noti-item-subtitle text-muted">Hi, How are you? What about our next meeting</small>
													</div>
												</div>
											</div>
										</a>

										<h5 class="text-muted font-13 fw-normal mt-0">30 Dec 2021</h5>

										<!-- item-->
										<a href="javascript:void(0);" class="dropdown-item p-0 notify-item card read-noti shadow-none mb-2">
											<div class="card-body">
												<span class="float-end noti-close-btn text-muted"><i class="mdi mdi-close"></i></span>
												<div class="d-flex align-items-center">
													<div class="flex-shrink-0">
														<div class="notify-icon bg-primary">
															<i class="mdi mdi-comment-account-outline"></i>
														</div>
													</div>
													<div class="flex-grow-1 text-truncate ms-2">
														<h5 class="noti-item-title fw-semibold font-14">Datacorp</h5>
														<small class="noti-item-subtitle text-muted">Caleb Flakelar commented on Admin</small>
													</div>
												</div>
											</div>
										</a>

										<!-- item-->
										<a href="javascript:void(0);" class="dropdown-item p-0 notify-item card read-noti shadow-none mb-2">
											<div class="card-body">
												<span class="float-end noti-close-btn text-muted"><i class="mdi mdi-close"></i></span>
												<div class="d-flex align-items-center">
													<div class="flex-shrink-0">
														<div class="notify-icon">
															<img src="assets/images/users/avatar-4.jpg" class="img-fluid rounded-circle" alt="" />
														</div>
													</div>
													<div class="flex-grow-1 text-truncate ms-2">
														<h5 class="noti-item-title fw-semibold font-14">Karen Robinson</h5>
														<small class="noti-item-subtitle text-muted">Wow ! this admin looks good and awesome design</small>
													</div>
												</div>
											</div>
										</a>

										<div class="text-center">
											<i class="mdi mdi-dots-circle mdi-spin text-muted h3 mt-0"></i>
										</div>
									</div>

									<!-- All-->
									<a href="javascript:void(0);" class="dropdown-item text-center text-primary notify-item border-top border-light py-2">
										View All
									</a>

								</div>
							</li>

							<li class="dropdown notification-list d-none d-sm-inline-block">
								<a class="nav-link dropdown-toggle arrow-none" data-bs-toggle="dropdown" href="#" role="button" aria-haspopup="false" aria-expanded="false">
									<i class="dripicons-view-apps noti-icon"></i>
								</a>
								<div class="dropdown-menu dropdown-menu-end dropdown-menu-animated dropdown-lg p-0">

									<div class="p-2">
										<div class="row g-0">
											<div class="col">
												<a class="dropdown-icon-item" href="#">
													<img src="assets/images/brands/slack.png" alt="slack">
													<span>Slack</span>
												</a>
											</div>
											<div class="col">
												<a class="dropdown-icon-item" href="#">
													<img src="assets/images/brands/github.png" alt="Github">
													<span>GitHub</span>
												</a>
											</div>
											<div class="col">
												<a class="dropdown-icon-item" href="#">
													<img src="assets/images/brands/dribbble.png" alt="dribbble">
													<span>Dribbble</span>
												</a>
											</div>
										</div>

										<div class="row g-0">
											<div class="col">
												<a class="dropdown-icon-item" href="#">
													<img src="assets/images/brands/bitbucket.png" alt="bitbucket">
													<span>Bitbucket</span>
												</a>
											</div>
											<div class="col">
												<a class="dropdown-icon-item" href="#">
													<img src="assets/images/brands/dropbox.png" alt="dropbox">
													<span>Dropbox</span>
												</a>
											</div>
											<div class="col">
												<a class="dropdown-icon-item" href="#">
													<img src="assets/images/brands/g-suite.png" alt="G Suite">
													<span>G Suite</span>
												</a>
											</div>
										</div> <!-- end row-->
									</div>

								</div>
							</li>

							<li class="notification-list">
								<a class="nav-link end-bar-toggle" href="javascript: void(0);">
									<i class="dripicons-gear noti-icon"></i>
								</a>
							</li>

							<li class="dropdown notification-list">
								<a class="nav-link dropdown-toggle nav-user arrow-none me-0" data-bs-toggle="dropdown" href="#" role="button" aria-haspopup="false" aria-expanded="false">
									<span class="account-user-avatar">
										<img src="<?=$page->getAssetPath('~/admin/assets/images/users/avatar-1.jpg')?>" alt="user-image" class="rounded-circle">
									</span>
									<span>
										<span class="account-user-name"><?=$profile->displayName?></span>
										<span class="account-position"><?=$user->email?></span>
									</span>
								</a>
								<div class="dropdown-menu dropdown-menu-end dropdown-menu-animated topbar-dropdown-menu profile-dropdown">
									<!-- item-->
									<div class=" dropdown-header noti-title">
										<h6 class="text-overflow m-0">Welcome <?=$profile->displayName?>!</h6>
									</div>

									<!-- item-->
									<a href="<?=$page->getAssetPath('~/account.php')?>" class="dropdown-item notify-item">
										<i class="mdi mdi-account-circle me-1"></i>
										<span>My Account</span>
									</a>

									<!-- item-->
									<a href="<?=$page->getAssetPath('~/logout.php')?>" class="dropdown-item notify-item">
										<i class="mdi mdi-logout me-1"></i>
										<span>Logout</span>
									</a>
								</div>
							</li>

						</ul>
						<button class="button-menu-mobile open-left">
							<i class="mdi mdi-menu"></i>
						</button>
						<div class="app-search dropdown d-none d-lg-block">
							<form>
								<div class="input-group">
									<!--<input type="text" class="form-control dropdown-toggle"  placeholder="Search..." id="top-search">
									<span class="mdi mdi-magnify search-icon"></span>
									<button class="input-group-text btn-primary" type="submit">Search</button>-->
								</div>
							</form>

							<div class="dropdown-menu dropdown-menu-animated dropdown-lg" id="search-dropdown">
								<!--
								<div class="dropdown-header noti-title">
									<h5 class="text-overflow mb-2">Found <span class="text-danger">17</span> results</h5>
								</div>

								<a href="javascript:void(0);" class="dropdown-item notify-item">
									<i class="uil-notes font-16 me-1"></i>
									<span>Analytics Report</span>
								</a>

								<a href="javascript:void(0);" class="dropdown-item notify-item">
									<i class="uil-life-ring font-16 me-1"></i>
									<span>How can I help you?</span>
								</a>

								<a href="javascript:void(0);" class="dropdown-item notify-item">
									<i class="uil-cog font-16 me-1"></i>
									<span>User profile settings</span>
								</a>

								<div class="dropdown-header noti-title">
									<h6 class="text-overflow mb-2 text-uppercase">Users</h6>
								</div>

								<div class="notification-list">
									<a href="javascript:void(0);" class="dropdown-item notify-item">
										<div class="d-flex">
											<img class="d-flex me-2 rounded-circle" src="<?=$page->getAssetPath('~/admin/assets/images/users/avatar-2.jpg')?>" alt="Generic placeholder image" height="32">
											<div class="w-100">
												<h5 class="m-0 font-14">Erwin Brown</h5>
												<span class="font-12 mb-0">UI Designer</span>
											</div>
										</div>
									</a>

									<a href="javascript:void(0);" class="dropdown-item notify-item">
										<div class="d-flex">
											<img class="d-flex me-2 rounded-circle" src="<?=$page->getAssetPath('~/admin/assets/images/users/avatar-5.jpg')?>" alt="Generic placeholder image" height="32">
											<div class="w-100">
												<h5 class="m-0 font-14">Jacob Deo</h5>
												<span class="font-12 mb-0">Developer</span>
											</div>
										</div>
									</a>
								</div>
								-->
							</div>
						</div>
					</div>
					<!-- end Topbar -->

					<!-- Start Content-->
					<div class="container-fluid">
<?=$this->section('content')?>
					</div>
					<!-- end Content -->
				</div>
				<!-- content -->

				<!-- Footer Start -->
				<footer class="footer">
					<div class="container-fluid">
						<div class="row">
							<div class="col-md-6">
								<?=(new \DateTime('now', new \DateTimeZone('UTC')))->format('Y')?> &copy; <?=$settings->get(SettingsStrings::SITE_NAME, 'ZSF')?>
								-
								System Version: v<?=$settings->get(SettingsStrings::SYSTEM_VERSION)?>
							</div>

							<div class="col-md-6">
								<div class="text-md-end footer-links d-none d-md-block">
									<a href="<?=$page->getAssetPath('~/')?>">Front-End</a>
								</div>
							</div>
						</div>
					</div>
				</footer>
				<!-- end Footer -->
			</div>

			<!-- ============================================================== -->
			<!-- End Page content -->
			<!-- ============================================================== -->


		</div>
		<!-- END wrapper -->

		<!-- Right Sidebar -->
		<div class="end-bar">
			<div class="rightbar-title">
				<a href="javascript:void(0);" class="end-bar-toggle float-end">
					<i class="dripicons-cross noti-icon"></i>
				</a>

				<h5 class="m-0">Settings</h5>
			</div>

			<div class="rightbar-content h-100" data-simplebar>
				<div class="p-3">
					<div class="alert alert-warning" role="alert">
						<strong>Customize </strong> the overall color scheme, sidebar menu, etc.
					</div>

					<!-- Settings -->
					<h5 class="mt-3">Color Scheme</h5>
					<hr class="mt-1" />

					<div class="form-check form-switch mb-1">
						<input class="form-check-input" type="checkbox" name="color-scheme-mode" value="light" id="light-mode-check" checked>
						<label class="form-check-label" for="light-mode-check">Light Mode</label>
					</div>

					<div class="form-check form-switch mb-1">
						<input class="form-check-input" type="checkbox" name="color-scheme-mode" value="dark" id="dark-mode-check">
						<label class="form-check-label" for="dark-mode-check">Dark Mode</label>
					</div>


					<!-- Width -->
					<h5 class="mt-4">Width</h5>
					<hr class="mt-1" />
					<div class="form-check form-switch mb-1">
						<input class="form-check-input" type="checkbox" name="width" value="fluid" id="fluid-check" checked>
						<label class="form-check-label" for="fluid-check">Fluid</label>
					</div>

					<div class="form-check form-switch mb-1">
						<input class="form-check-input" type="checkbox" name="width" value="boxed" id="boxed-check">
						<label class="form-check-label" for="boxed-check">Boxed</label>
					</div>


					<!-- Left Sidebar-->
					<h5 class="mt-4">Left Sidebar</h5>
					<hr class="mt-1" />
					<div class="form-check form-switch mb-1">
						<input class="form-check-input" type="checkbox" name="theme" value="default" id="default-check">
						<label class="form-check-label" for="default-check">Default</label>
					</div>

					<div class="form-check form-switch mb-1">
						<input class="form-check-input" type="checkbox" name="theme" value="light" id="light-check" checked>
						<label class="form-check-label" for="light-check">Light</label>
					</div>

					<div class="form-check form-switch mb-3">
						<input class="form-check-input" type="checkbox" name="theme" value="dark" id="dark-check">
						<label class="form-check-label" for="dark-check">Dark</label>
					</div>

					<div class="form-check form-switch mb-1">
						<input class="form-check-input" type="checkbox" name="compact" value="fixed" id="fixed-check" checked>
						<label class="form-check-label" for="fixed-check">Fixed</label>
					</div>

					<div class="form-check form-switch mb-1">
						<input class="form-check-input" type="checkbox" name="compact" value="condensed" id="condensed-check">
						<label class="form-check-label" for="condensed-check">Condensed</label>
					</div>

					<div class="form-check form-switch mb-1">
						<input class="form-check-input" type="checkbox" name="compact" value="scrollable" id="scrollable-check">
						<label class="form-check-label" for="scrollable-check">Scrollable</label>
					</div>

					<div class="d-grid mt-4">
						<button class="btn btn-primary" id="resetBtn">Reset to Default</button>
					</div>
				</div> <!-- end padding-->

			</div>
		</div>

		<div class="rightbar-overlay"></div>
		<!-- /End-bar -->

		<!-- bundle -->
		<script src="<?=$page->getAssetPath('~/admin/assets/js/vendor.min.js')?>"></script>
		<script src="<?=$page->getAssetPath('~/admin/assets/js/app.min.js')?>"></script>

		<!-- third party js -->
		<!-- <script src="<?=$page->getAssetPath('~/admin/assets/js/vendor/chart.min.js')?>"></script> -->
		<script src="<?=$page->getAssetPath('~/admin/assets/js/vendor/apexcharts.min.js')?>"></script>
		<script src="<?=$page->getAssetPath('~/admin/assets/js/vendor/jquery-jvectormap-1.2.2.min.js')?>"></script>
		<script src="<?=$page->getAssetPath('~/admin/assets/js/vendor/jquery-jvectormap-world-mill-en.js')?>"></script>
		<!-- third party js ends -->

		<!-- page variables -->
		<script type="text/javascript">
			const apiBaseUrl = "<?=$page->getAssetPath('~/api/1/')?>";
			const authToken  = "<?=base64_encode("{$_SESSION[UserEvents::STR_SESSION_USERID]}:{$_SESSION[UserEvents::STR_SESSION_TOKEN]}")?>";
		</script>
		<!-- page variables end -->

		<!-- utility js -->
		<script src="<?=$page->getAssetPath('~/assets/js/utils.js')?>"></script>
		<script type="text/javascript">
			const utils = new Utilities(apiBaseUrl, authToken);
		</script>
		<!-- utility js ends -->

		<?=$this->section('scripts')?>
	</body>
</html>