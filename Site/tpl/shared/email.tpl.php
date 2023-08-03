<?php

	/**
	 * @var Stoic\Web\PageHelper $page
	 * @var AndyM84\Config\ConfigContainer $settings
	 */

	use Zibings\SettingsStrings;

?>
<!DOCTYPE html>

<html>
	<head>
		<title></title>
		<!--

				An email present from your friends at Litmus (@litmusapp)

				Email is surprisingly hard. While this has been thoroughly tested, your mileage may vary.
				It's highly recommended that you test using a service like Litmus (http://litmus.com) and your own devices.

				Enjoy!

		 -->
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<meta http-equiv="X-UA-Compatible" content="IE=edge" />
		<style type="text/css">
			/* CLIENT-SPECIFIC STYLES */
			body, table, td, a {
				-webkit-text-size-adjust: 100%;
				-ms-text-size-adjust: 100%;
			}
			/* Prevent WebKit and Windows mobile changing default text sizes */
			table, td {
				mso-table-lspace: 0pt;
				mso-table-rspace: 0pt;
			}
			/* Remove spacing between tables in Outlook 2007 and up */
			img {
				-ms-interpolation-mode: bicubic;
			}
			/* Allow smoother rendering of resized image in Internet Explorer */

			/* RESET STYLES */
			img {
				border: 0;
				height: auto;
				line-height: 100%;
				outline: none;
				text-decoration: none;
			}

			table {
				border-collapse: collapse !important;
			}

			body {
				height: 100% !important;
				margin: 0 !important;
				padding: 0 !important;
				width: 100% !important;
			}

			a {
				text-decoration: none;
			}

			td a {
				color: #6c757d!important;
			}

			td a:hover {
				text-decoration: underline;
				color:#209fca!important;
			}

			/* iOS BLUE LINKS */
			a[x-apple-data-detectors] {
				color: inherit !important;
				text-decoration: none !important;
				font-size: inherit !important;
				font-family: inherit !important;
				font-weight: inherit !important;
				line-height: inherit !important;
			}

			/* MOBILE STYLES */
			@media screen and (max-width: 525px) {

				/* ALLOWS FOR FLUID TABLES */
				.wrapper {
					width: 100% !important;
					max-width: 100% !important;
				}

				/* ADJUSTS LAYOUT OF LOGO IMAGE */
				.logo img {
					margin: 0 auto !important;
				}

				/* USE THESE CLASSES TO HIDE CONTENT ON MOBILE */
				.mobile-hide {
					display: none !important;
				}

				.img-max {
					max-width: 100% !important;
					width: 100% !important;
					height: auto !important;
				}

				/* FULL-WIDTH TABLES */
				.responsive-table {
					width: 100% !important;
				}

				/* UTILITY CLASSES FOR ADJUSTING PADDING ON MOBILE */
				.padding {
					padding: 10px 5% 15px 5% !important;
				}

				.padding-meta {
					padding: 30px 5% 0px 5% !important;
					text-align: center;
				}

				.padding-copy {
					padding: 10px 5% 10px 5% !important;
					text-align: center;
				}

				.no-padding {
					padding: 0 !important;
				}

				.section-padding {
					padding: 50px 15px 50px 15px !important;
				}

				/* ADJUST BUTTONS ON MOBILE */
				.mobile-button-container {
					margin: 0 auto;
					width: 100% !important;
				}

				.mobile-button {
					padding: 15px !important;
					border: 0 !important;
					font-size: 16px !important;
					display: block !important;
				}
			}

			/* ANDROID CENTER FIX */
			div[style*="margin: 16px 0;"] {
				margin: 0 !important;
			}
		</style>
	</head>
	<body style="margin: 0 !important; padding: 0 !important;">

		<!-- HIDDEN PREHEADER TEXT -->
		<div style="display: none; font-size: 1px; color: #fefefe; line-height: 1px; font-family: Helvetica, Arial, sans-serif; max-height: 0px; max-width: 0px; opacity: 0; overflow: hidden;">
			<?=$settings->get(SettingsStrings::SITE_NAME)?>
		</div>

		<!-- HEADER -->
		<table border="0" cellpadding="0" cellspacing="0" width="100%">
			<tr>
				<td bgcolor="#ffffff" align="center">
					<!--[if (gte mso 9)|(IE)]>
					<table align="center" border="0" cellspacing="0" cellpadding="0" width="500">
						<tr>
							<td align="center" valign="top" width="500">
					<![endif]-->
					<table border="0" cellpadding="0" cellspacing="0" width="100%" style="max-width: 500px;" class="wrapper">
						<tr>
							<td align="center" valign="top" style="padding: 15px 0;" class="logo">
								<a href="<?=$page->getAssetPath('~/', null, true)?>" target="_blank">
									<img alt="" src="<?=$page->getAssetPath('~/assets/img/emails/logo.png', null, true)?>" height="72" style="display: block; font-family: Helvetica, Arial, sans-serif; color: #ffffff; font-size: 16px;" border="0">
								</a>
							</td>
						</tr>
					</table>
					<!--[if (gte mso 9)|(IE)]>
					</td>
					</tr>
					</table>
					<![endif]-->
				</td>
			</tr>
			<tr>
				<td bgcolor="#ffffff" align="center" style="padding: 20px 15px 20px 15px;" class="section-padding">
					<!--[if (gte mso 9)|(IE)]>
					<table align="center" border="0" cellspacing="0" cellpadding="0" width="500">
						<tr>
							<td align="center" valign="top" width="500">
					<![endif]-->
					<table border="0" cellpadding="0" cellspacing="0" width="100%" style="max-width: 500px;" class="responsive-table">
						<tr>
							<td>
								<?=$this->section('content')?>
							</td>
						</tr>
					</table>
					<!--[if (gte mso 9)|(IE)]>
					</td>
					</tr>
					</table>
					<![endif]-->
				</td>
			</tr>
			<tr>
				<td bgcolor="#ffffff" align="center" style="padding: 20px 0px;">
					<!--[if (gte mso 9)|(IE)]>
					<table align="center" border="0" cellspacing="0" cellpadding="0" width="500">
						<tr>
							<td align="center" valign="top" width="500">
					<![endif]-->
					<!-- UNSUBSCRIBE COPY -->
					<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="max-width: 500px;" class="responsive-table">
						<tr>
							<td align="center" style="font-size: 12px; line-height: 18px; font-family: Helvetica, Arial, sans-serif; color:#666666;">
								158 Plymouth Dr Apt 2A, Norwood, MA 02062
								<br>
								<!-- NOT REQUIRED FOR TRANSACTIONAL EMAILS <a href="<?=$page->getAssetPath('~/', null, true)?>" target="_blank" style="color: #666666; text-decoration: none;">Unsubscribe</a> -->
							</td>
						</tr>
					</table>
					<!--[if (gte mso 9)|(IE)]>
					</td>
					</tr>
					</table>
					<![endif]-->
				</td>
			</tr>
		</table>

	</body>
</html>