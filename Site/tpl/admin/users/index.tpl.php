<?php

	/**
	 * @var Stoic\Web\PageHelper $page
	 * @var Zibings\User[] $users
	 */

?>
<?php $this->layout('shared::admin-master', ['page' => $page]); ?>

						<!-- start page title -->
						<div class="row">
							<div class="col-12">
								<div class="page-title-box">
									<div class="page-title-right">
										<form class="form-inline">
											<a href="<?=$page->getAssetPath('~/admin/users.php', ['action' => 'create'])?>" class="btn btn-outline-primary ml-1" title="Create User">
												Create User &nbsp;
												<i class="mdi mdi-account-plus"></i>
											</a>
										</form>
									</div>
									<h4 class="page-title">User Management</h4>
								</div>
							</div>
						</div>
						<!-- end page title -->

						<div class="row">
							<div class="col-12">
								<div class="card">
									<div class="card-body">
										<div class="table-responsive">
											<table id="user-list" class="table dt-responsive nowrap w-100">
												<thead>
													<tr>
														<th class="text-center">ID</th>
														<th class="text-center">Username</th>
														<th class="text-center">Email Address</th>
														<th class="text-center">Date Joined</th>
														<th class="text-center">Last Login</th>
														<th class="text-center">Actions</th>
													</tr>
												</thead>
												<tbody>
<?php foreach ($users as $usr): ?>													<tr>
														<td class="text-center">
															<?=$usr['user']->id?>
														</td>
														<td class="text-center">
															<a href="<?=$page->getAssetPath('~/admin/users.php', ['id' => $usr['user']->id, 'action' => 'edit'])?>">
																<?=$usr['profile']->displayName?>
															</a>
														</td>
														<td class="text-center">
															<a href="<?=$page->getAssetPath('~/admin/users.php', ['id' => $usr['user']->id, 'action' => 'edit'])?>">
																<?=$usr['user']->email?>
															</a>
														</td>
														<td class="text-center">
															<?=$usr['user']->joined->format('M j, Y H:i:s')?>
														</td>
														<td class="text-center">
															<?php if ($usr['user']->lastLogin !== null): ?><?=$usr['user']->joined->format('M j, Y H:i:s')?><?php else: ?>N/A<?php endif; ?>
														</td>
														<td class="text-center">
															<a href="<?=$page->getAssetPath('~/admin/users.php', ['id' => $usr['user']->id, 'action' => 'edit'])?>" class="btn btn-outline-primary btn-sm ml-1">
																<i class="mdi mdi-human-edit"></i>
															</a>

															<a href="javascript: doDelete(<?=$usr['user']->id?>);" class="btn btn-outline-primary btn-sm ml-1">
																<i class="mdi mdi-trash-can-outline"></i>
															</a>
														</td>
													</tr>
<?php endforeach; ?>
												</tbody>
											</table>
										</div> <!-- end table-responsive-->
									</div> <!-- end card-body-->
								</div> <!-- end card-->
							</div> <!-- end col-->
						</div>
						<!-- end row -->

<?php $this->push('scripts') ?>
		<!-- Datatables js -->
		<script src="<?=$page->getAssetPath('~/admin/assets/js/vendor/jquery.dataTables.min.js')?>"></script>
		<script src="<?=$page->getAssetPath('~/admin/assets/js/vendor/dataTables.bootstrap4.js')?>"></script>
		<script src="<?=$page->getAssetPath('~/admin/assets/js/vendor/dataTables.responsive.min.js')?>"></script>
		<script src="<?=$page->getAssetPath('~/admin/assets/js/vendor/responsive.bootstrap4.min.js')?>"></script>

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

			$('#user-list').DataTable({
				keys: !0,
				language: {
					paginate: {
						previous: "<i class=\"mdi mdi-chevron-left\">",
						next: "<i class=\"mdi mdi-chevron-right\">"
					}
				},
				drawCallback: function () {
					$(".dataTables_paginate > .pagination").addClass("pagination-rounded");

					return;
				}
			});
		</script>
<?php $this->end() ?>

<?php $this->push('stylesheets') ?>
		<!-- Datatables css -->
		<link href="<?=$page->getAssetPath('~/admin/assets/css/vendor/dataTables.bootstrap4.css')?>" rel="stylesheet" type="text/css" />
		<link href="<?=$page->getAssetPath('~/admin/assets/css/vendor/responsive.bootstrap4.css')?>" rel="stylesheet" type="text/css" />
<?php $this->end() ?>
