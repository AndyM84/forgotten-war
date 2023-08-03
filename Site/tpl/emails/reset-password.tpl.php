<?php

	/**
	 * @var Stoic\Web\PageHelper $page
	 * @var AndyM84\Config\ConfigContainer $settings
	 * @var string $token
	 */

	use Zibings\SettingsStrings;

?>
<?php $this->layout('shared::email', ['page' => $page, 'settings' => $settings]) ?>

								<!-- HERO IMAGE -->
								<table width="100%" border="0" cellspacing="0" cellpadding="0">
									<tr>
										<td>
											<!-- COPY -->
											<table width="100%" border="0" cellspacing="0" cellpadding="0">
												<tr>
													<td align="center" style="font-size: 25px; font-family: Helvetica, Arial, sans-serif; color: #333333; padding-top: 30px;" class="padding"><?=$settings->get(SettingsStrings::SITE_NAME)?> Password Reset</td>
												</tr>
												<tr>
													<td align="center" style="padding: 20px 0 0 0; font-size: 16px; line-height: 25px; font-family: Helvetica, Arial, sans-serif; color: #666666;" class="padding">Click the link below and set a new password.</td>
												</tr>
											</table>
										</td>
									</tr>
									<tr>
										<td align="center">
											<!-- BULLETPROOF BUTTON -->
											<table width="100%" border="0" cellspacing="0" cellpadding="0">
												<tr>
													<td align="center" style="padding-top: 25px;" class="padding">
														<table border="0" cellspacing="0" cellpadding="0" class="mobile-button-container">
															<tr>
																<td align="center" style="border-radius: 3px;" bgcolor="#256F9C"><a href="<?=$page->getAssetPath('~/reset-password.php', ['token' => $token], true)?>" target="_blank" style="font-size: 16px; font-family: Helvetica, Arial, sans-serif; color: #ffffff!important; text-decoration: none; border-radius: 3px; padding: 15px 25px; border: 1px solid #256F9C; display: inline-block;" class="mobile-button">Reset Your Password &rarr;</a></td>
															</tr>
														</table>
													</td>
												</tr>
											</table>
										</td>
									</tr>
								</table>