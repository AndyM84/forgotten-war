<?php

	/* @var Stoic\Web\PageHelper $page */

?>
<?php $this->layout('shared::admin-master', ['page' => $page]); ?>

						<!-- start page title -->
						<div class="row">
							<div class="col-12">
								<div class="page-title-box">
									<div class="page-title-right">
										<ol class="breadcrumb m-0">
											<li class="breadcrumb-item"><a href="javascript: void(0);">Administration</a></li>
											<li class="breadcrumb-item active">Dashboard</li>
										</ol>
									</div>

									<h4 class="page-title">Dashboard</h4>
								</div>
							</div>
						</div>
						<!-- end page title -->

						<div class="row">
							<div class="col-md-6 col-xl-3">
								<div class="card">
									<div class="card-body">
										<div class="row align-items-center">
											<div class="col-6">
												<h5 class="text-muted fw-normal mt-0 text-truncate" title="Daily Active Users">DAU (Daily Active Users)</h5>
												<h3 class="my-2 py-1"><?=number_format($dau ?? 0)?></h3>

												<p class="mb-0 text-muted">
													<span class="text-success me-2"><i class="mdi mdi-arrow-up-bold"></i> 3.27%</span>
												</p>
											</div>

											<div class="col-6">
												<div class="text-end">
													<div id="campaign-sent-chart" data-colors="#727cf5"></div>
												</div>
											</div>
										</div> <!-- end row-->
									</div> <!-- end card-body -->
								</div> <!-- end card -->
							</div> <!-- end col -->

							<div class="col-md-6 col-xl-3">
								<div class="card">
									<div class="card-body">
										<div class="row align-items-center">
											<div class="col-6">
												<h5 class="text-muted fw-normal mt-0 text-truncate" title="Monthly Active Users">MAU (Monthly Active Users)</h5>
												<h3 class="my-2 py-1"><?=number_format($mau ?? 0)?></h3>
												<p class="mb-0 text-muted">
													<span class="text-danger me-2"><i class="mdi mdi-arrow-down-bold"></i> 5.38%</span>
												</p>
											</div>
											<div class="col-6">
												<div class="text-end">
													<div id="new-leads-chart" data-colors="#0acf97"></div>
												</div>
											</div>
										</div> <!-- end row-->
									</div> <!-- end card-body -->
								</div> <!-- end card -->
							</div> <!-- end col -->

							<div class="col-md-6 col-xl-3">
								<div class="card">
									<div class="card-body">
										<div class="row align-items-center">
											<div class="col-6">
												<h5 class="text-muted fw-normal mt-0 text-truncate" title="Total Verified Users">Total Verified Users (TVU)</h5>
												<h3 class="my-2 py-1"><?=number_format($tvu ?? 0)?></h3>
												<p class="mb-0 text-muted">
													<span class="text-success me-2"><i class="mdi mdi-arrow-up-bold"></i> 4.87%</span>
												</p>
											</div>
											<div class="col-6">
												<div class="text-end">
													<div id="deals-chart" data-colors="#727cf5"></div>
												</div>
											</div>
										</div> <!-- end row-->
									</div> <!-- end card-body -->
								</div> <!-- end card -->
							</div> <!-- end col -->

							<div class="col-md-6 col-xl-3">
								<div class="card">
									<div class="card-body">
										<div class="row align-items-center">
											<div class="col-6">
												<h5 class="text-muted fw-normal mt-0 text-truncate" title="Total Users">TU (Total Users)</h5>
												<h3 class="my-2 py-1"><?=number_format($tu ?? 0)?></h3>
												<p class="mb-0 text-muted">
													<span class="text-success me-2"><i class="mdi mdi-arrow-up-bold"></i> 11.7%</span>
												</p>
											</div>
											<div class="col-6">
												<div class="text-end">
													<div id="booked-revenue-chart" data-colors="#0acf97"></div>
												</div>
											</div>
										</div> <!-- end row-->
									</div> <!-- end card-body -->
								</div> <!-- end card -->
							</div> <!-- end col -->
						</div>

						<div class="row">
							<div class="col-lg-5">
								<div class="card">
									<div class="card-body">
										<div class="d-flex justify-content-between align-items-center mb-1">
											<h4 class="header-title">Campaigns</h4>
											<div class="dropdown">
												<a href="#" class="dropdown-toggle arrow-none card-drop" data-bs-toggle="dropdown" aria-expanded="false">
													<i class="mdi mdi-dots-vertical"></i>
												</a>
												<div class="dropdown-menu dropdown-menu-end">
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Today</a>
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Yesterday</a>
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Last Week</a>
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Last Month</a>
												</div>
											</div>
										</div>

										<div id="dash-campaigns-chart" class="apex-charts" data-colors="#ffbc00,#727cf5,#0acf97"></div>

										<div class="row text-center mt-3">
											<div class="col-sm-4">
												<i class="mdi mdi-send widget-icon rounded-circle bg-light-lighten text-muted"></i>
												<h3 class="fw-normal mt-3">
													<span>6,510</span>
												</h3>
												<p class="text-muted mb-0 mb-2"><i class="mdi mdi-checkbox-blank-circle text-warning"></i> Total Sent</p>
											</div>
											<div class="col-sm-4">
												<i class="mdi mdi-flag-variant widget-icon rounded-circle bg-light-lighten text-muted"></i>
												<h3 class="fw-normal mt-3">
													<span>3,487</span>
												</h3>
												<p class="text-muted mb-0 mb-2"><i class="mdi mdi-checkbox-blank-circle text-primary"></i> Reached</p>
											</div>
											<div class="col-sm-4">
												<i class="mdi mdi-email-open widget-icon rounded-circle bg-light-lighten text-muted"></i>
												<h3 class="fw-normal mt-3">
													<span>1,568</span>
												</h3>
												<p class="text-muted mb-0 mb-2"><i class="mdi mdi-checkbox-blank-circle text-success"></i> Opened</p>
											</div>
										</div>
									</div>
									<!-- end card body-->
								</div>
								<!-- end card -->
							</div>
							<!-- end col-->

							<div class="col-lg-7">
								<div class="card">
									<div class="card-body">
										<div class="d-flex justify-content-between align-items-center mb-3">
											<h4 class="header-title">Revenue</h4>
											<div class="dropdown">
												<a href="#" class="dropdown-toggle arrow-none card-drop" data-bs-toggle="dropdown" aria-expanded="false">
													<i class="mdi mdi-dots-vertical"></i>
												</a>
												<div class="dropdown-menu dropdown-menu-end">
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Today</a>
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Yesterday</a>
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Last Week</a>
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Last Month</a>
												</div>
											</div>
										</div>

										<div class="chart-content-bg">
											<div class="row text-center">
												<div class="col-sm-6">
													<p class="text-muted mb-0 mt-3">Current Month</p>
													<h2 class="fw-normal mb-3">
														<span>$42,025</span>
													</h2>
												</div>
												<div class="col-sm-6">
													<p class="text-muted mb-0 mt-3">Previous Month</p>
													<h2 class="fw-normal mb-3">
														<span>$74,651</span>
													</h2>
												</div>
											</div>
										</div>

										<div dir="ltr">
											<div id="dash-revenue-chart" class="apex-charts" data-colors="#0acf97,#fa5c7c"></div>
										</div>

									</div>
									<!-- end card body-->
								</div>
								<!-- end card -->
							</div>
							<!-- end col-->
						</div>
						<!-- end row-->

						<div class="row">
							<div class="col-xl-4 col-lg-12">
								<div class="card">
									<div class="card-body">
										<div class="d-flex justify-content-between align-items-center mb-2">
											<h4 class="header-title">Top Performing</h4>
											<div class="dropdown">
												<a href="#" class="dropdown-toggle arrow-none card-drop" data-bs-toggle="dropdown" aria-expanded="false">
													<i class="mdi mdi-dots-vertical"></i>
												</a>
												<div class="dropdown-menu dropdown-menu-end">
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Settings</a>
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Action</a>
												</div>
											</div>
										</div>

										<div class="table-responsive">
											<table class="table table-striped table-sm table-nowrap table-centered mb-0">
												<thead>
													<tr>
														<th>User</th>
														<th>Leads</th>
														<th>Deals</th>
														<th>Tasks</th>
														<th></th>
													</tr>
												</thead>
												<tbody>
													<tr>
														<td>
															<h5 class="font-15 mb-1 fw-normal">Jeremy Young</h5>
															<span class="text-muted font-13">Senior Sales Executive</span>
														</td>
														<td>187</td>
														<td>154</td>
														<td>49</td>
														<td class="table-action">
															<a href="javascript: void(0);" class="action-icon"> <i class="mdi mdi-eye"></i></a>
														</td>
													</tr>
													<tr>
														<td>
															<h5 class="font-15 mb-1 fw-normal">Thomas Krueger</h5>
															<span class="text-muted font-13">Senior Sales Executive</span>
														</td>
														<td>235</td>
														<td>127</td>
														<td>83</td>
														<td class="table-action">
															<a href="javascript: void(0);" class="action-icon"> <i class="mdi mdi-eye"></i></a>
														</td>
													</tr>
													<tr>
														<td>
															<h5 class="font-15 mb-1 fw-normal">Pete Burdine</h5>
															<span class="text-muted font-13">Senior Sales Executive</span>
														</td>
														<td>365</td>
														<td>148</td>
														<td>62</td>
														<td class="table-action">
															<a href="javascript: void(0);" class="action-icon"> <i class="mdi mdi-eye"></i></a>
														</td>
													</tr>
													<tr>
														<td>
															<h5 class="font-15 mb-1 fw-normal">Mary Nelson</h5>
															<span class="text-muted font-13">Senior Sales Executive</span>
														</td>
														<td>753</td>
														<td>159</td>
														<td>258</td>
														<td class="table-action">
															<a href="javascript: void(0);" class="action-icon"> <i class="mdi mdi-eye"></i></a>
														</td>
													</tr>
													<tr>
														<td>
															<h5 class="font-15 mb-1 fw-normal">Kevin Grove</h5>
															<span class="text-muted font-13">Senior Sales Executive</span>
														</td>
														<td>458</td>
														<td>126</td>
														<td>73</td>
														<td class="table-action">
															<a href="javascript: void(0);" class="action-icon"> <i class="mdi mdi-eye"></i></a>
														</td>
													</tr>
												</tbody>
											</table>
										</div> <!-- end table-responsive-->

									</div> <!-- end card-body-->
								</div> <!-- end card-->
							</div>
							<!-- end col-->

							<div class="col-xl-4 col-lg-6">
								<div class="card">
									<div class="card-body">
										<div class="d-flex justify-content-between align-items-center mb-3">
											<h4 class="header-title">Recent Leads</h4>
											<div class="dropdown">
												<a href="#" class="dropdown-toggle arrow-none card-drop" data-bs-toggle="dropdown" aria-expanded="false">
													<i class="mdi mdi-dots-vertical"></i>
												</a>
												<div class="dropdown-menu dropdown-menu-end">
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Settings</a>
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Action</a>
												</div>
											</div>
										</div>

										<div class="d-flex align-items-start">
											<img class="me-3 rounded-circle" src="assets/images/users/avatar-2.jpg" width="40" alt="Generic placeholder image">
											<div class="w-100 overflow-hidden">
												<span class="badge badge-warning-lighten float-end">Cold lead</span>
												<h5 class="mt-0 mb-1">Risa Pearson</h5>
												<span class="font-13">richard.john@mail.com</span>
											</div>
										</div>

										<div class="d-flex align-items-start mt-3">
											<img class="me-3 rounded-circle" src="assets/images/users/avatar-3.jpg" width="40" alt="Generic placeholder image">
											<div class="w-100 overflow-hidden">
												<span class="badge badge-danger-lighten float-end">Lost lead</span>
												<h5 class="mt-0 mb-1">Margaret D. Evans</h5>
												<span class="font-13">margaret.evans@rhyta.com</span>
											</div>
										</div>

										<div class="d-flex align-items-start mt-3">
											<img class="me-3 rounded-circle" src="assets/images/users/avatar-4.jpg" width="40" alt="Generic placeholder image">
											<div class="w-100 overflow-hidden">
												<span class="badge badge-success-lighten float-end">Won lead</span>
												<h5 class="mt-0 mb-1">Bryan J. Luellen</h5>
												<span class="font-13">bryuellen@dayrep.com</span>
											</div>
										</div>

										<div class="d-flex align-items-start mt-3">
											<img class="me-3 rounded-circle" src="assets/images/users/avatar-5.jpg" width="40" alt="Generic placeholder image">
											<div class="w-100 overflow-hidden">
												<span class="badge badge-warning-lighten float-end">Cold lead</span>
												<h5 class="mt-0 mb-1">Kathryn S. Collier</h5>
												<span class="font-13">collier@jourrapide.com</span>
											</div>
										</div>

										<div class="d-flex align-items-start mt-3">
											<img class="me-3 rounded-circle" src="assets/images/users/avatar-1.jpg" width="40" alt="Generic placeholder image">
											<div class="w-100 overflow-hidden">
												<span class="badge badge-warning-lighten float-end">Cold lead</span>
												<h5 class="mt-0 mb-1">Timothy Kauper</h5>
												<span class="font-13">thykauper@rhyta.com</span>
											</div>
										</div>

										<div class="d-flex align-items-start mt-3">
											<img class="me-3 rounded-circle" src="assets/images/users/avatar-6.jpg" width="40" alt="Generic placeholder image">
											<div class="w-100 overflow-hidden">
												<span class="badge badge-success-lighten float-end">Won lead</span>
												<h5 class="mt-0 mb-1">Zara Raws</h5>
												<span class="font-13">austin@dayrep.com</span>
											</div>
										</div>

									</div>
									<!-- end card-body -->
								</div>
								<!-- end card-->
							</div>
							<!-- end col -->

							<div class="col-xl-4 col-lg-6">
								<div class="card cta-box bg-primary text-white">
									<div class="card-body">
										<div class="d-flex align-items-start align-items-center">
											<div class="w-100 overflow-hidden">
												<h2 class="mt-0"><i class="mdi mdi-bullhorn-outline"></i>&nbsp;</h2>
												<h3 class="m-0 fw-normal cta-box-title">Enhance your <b>Campaign</b> for better outreach <i class="mdi mdi-arrow-right"></i></h3>
											</div>
											<img class="ms-3" src="assets/images/email-campaign.svg" width="120" alt="Generic placeholder image">
										</div>
									</div>
									<!-- end card-body -->
								</div>
								<!-- end card-->

								<!-- Todo-->
								<div class="card">
									<div class="card-body pb-0">
										<div class="d-flex justify-content-between align-items-center">
											<h4 class="header-title">Todo</h4>
											<div class="dropdown float-end">
												<a href="#" class="dropdown-toggle arrow-none card-drop" data-bs-toggle="dropdown" aria-expanded="false">
													<i class="mdi mdi-dots-vertical"></i>
												</a>
												<div class="dropdown-menu dropdown-menu-end">
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Settings</a>
													<!-- item-->
													<a href="javascript:void(0);" class="dropdown-item">Action</a>
												</div>
											</div>
										</div>
									</div>

									<div class="todoapp">
										<div class="card-body py-0" data-simplebar style="max-height: 231px">
											<ul class="list-group list-group-flush todo-list" id="todo-list"></ul>
										</div>
									</div> <!-- end .todoapp-->
								</div> <!-- end card-->

							</div>
							<!-- end col -->
						</div>
						<!-- end row-->

<?php $this->push('scripts') ?>
		<!-- Apex js -->
		<script src="<?=$page->getAssetPath('~/admin/assets/js/vendor/apexcharts.min.js')?>"></script>

		<!-- Todo js -->
		<script src="<?=$page->getAssetPath('~/admin/assets/js/ui/component.todo.js')?>"></script>

		<script type="text/javascript">
			var colors = ["#727cf5", "#0acf97", "#fa5c7c", "#ffbc00"];
			var dataColors = $("#campaign-sent-chart").data("colors");
			dataColors && (colors = dataColors.split(","));

			var options1 = {
				chart: { type: "bar", height: 60, sparkline: { enabled: !0 } },
				plotOptions: { bar: { columnWidth: "60%" } },
				colors: colors,
				series: [{ data: [25, 66, 41, 89, 63, 25, 44, 12, 36, 9, 54] }],
				labels: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11],
				xaxis: { crosshairs: { width: 1 } },
				tooltip: {
					fixed: { enabled: !1 },
					x: { show: !1 },
					y: { title: { formatter: function(o) { return ""; } } },
					marker: { show: !1 }
				}
			};
			new ApexCharts(document.querySelector("#campaign-sent-chart"), options1).render();

			colors = ["#727cf5", "#0acf97", "#fa5c7c", "#ffbc00"];
			(dataColors = $("#new-leads-chart").data("colors")) && (colors = dataColors.split(","));

			var options2 = {
				chart: { type: "line", height: 60, sparkline: { enabled: !0 } },
				series: [{ data: [25, 66, 41, 89, 63, 25, 44, 12, 36, 9, 54] }],
				stroke: { width: 2, curve: "smooth" },
				markers: { size: 0 },
				colors: colors,
				tooltip: {
					fixed: { enabled: !1 },
					x: { show: !1 },
					y: { title: { formatter: function(o) { return ""; } } },
					marker: { show: !1 }
				}
			};

			new ApexCharts(document.querySelector("#new-leads-chart"), options2).render();

			colors = ["#727cf5", "#0acf97", "#fa5c7c", "#ffbc00"];
			(dataColors = $("#deals-chart").data("colors")) && (colors = dataColors.split(","));

			var options3 = {
				chart: { type: "bar", height: 60, sparkline: { enabled: !0 } },
				plotOptions: { bar: { columnWidth: "60%" } },
				colors: colors,
				series: [{ data: [12, 14, 2, 47, 42, 15, 47, 75, 65, 19, 14] }],
				labels: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11],
				xaxis: { crosshairs: { width: 1 } },
				tooltip: {
					fixed: { enabled: !1 },
					x: { show: !1 },
					y: { title: { formatter: function(o) { return ""; } } },
					marker: { show: !1 }
				}
			};

			new ApexCharts(document.querySelector("#deals-chart"), options3).render();

			colors = ["#727cf5", "#0acf97", "#fa5c7c", "#ffbc00"];
			(dataColors = $("#booked-revenue-chart").data("colors")) && (colors = dataColors.split(","));

			var options4 = {
				chart: { type: "bar", height: 60, sparkline: { enabled: !0 } },
				plotOptions: { bar: { columnWidth: "60%" } },
				colors: colors,
				series: [{ data: [47, 45, 74, 14, 56, 74, 14, 11, 7, 39, 82] }],
				labels: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11],
				xaxis: { crosshairs: { width: 1 } },
				tooltip: {
					fixed: { enabled: !1 },
					x: { show: !1 },
					y: { title: { formatter: function(o) { return ""; } } },
					marker: { show: !1 }
				}
			};

			new ApexCharts(document.querySelector("#booked-revenue-chart"), options4).render();

			colors = ["#727cf5", "#0acf97", "#fa5c7c", "#ffbc00"];
			(dataColors = $("#dash-campaigns-chart").data("colors")) && (colors = dataColors.split(","));

			var options = {
					chart: { height: 314, type: "radialBar" },
					colors: colors,
					series: [86, 36, 50],
					labels: ["Total Sent", "Reached", "Opened"],
					plotOptions: { radialBar: { track: { margin: 8 } } }
				};
			var chart = new ApexCharts(document.querySelector("#dash-campaigns-chart"), options);
			chart.render();

			colors = ["#727cf5", "#0acf97", "#fa5c7c", "#ffbc00"];
			(dataColors = $("#dash-revenue-chart").data("colors")) && (colors = dataColors.split(","));

			options = {
				chart: { height: 321, type: "line", toolbar: { show: !1 } },
				stroke: { curve: "smooth", width: 2 },
				series: [{
					name: "Total Revenue", type: "area", data: [44, 55, 31, 47, 31, 43, 26, 41, 31, 47, 33, 43]
				}, {
					name: "Total Pipeline", type: "line", data: [55, 69, 45, 61, 43, 54, 37, 52, 44, 61, 43, 56]
				}],
				fill: { type: "solid", opacity: [.35, 1] },
				labels: ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"],
				markers: { size: 0 },
				colors: colors,
				yaxis: [{ title: { text: "Revenue (USD)" }, min: 0 }],
				tooltip: { shared: !0, intersect: !1, y: { formatter: function(o) { return void 0 !== o ? o.toFixed(0) + "k" : o; } } },
				grid: { borderColor: "#f1f3fa", padding: { bottom: 5 } },
				legend: { fontSize: "14px", fontFamily: "14px", offsetY: 5 },
				responsive: [{
					breakpoint: 600,
					options: { yaxis: { show: !1 }, legend: { show: !1 } }
				}]
			};
			(chart = new ApexCharts(document.querySelector("#dash-revenue-chart"), options)).render();
		</script>
<?php $this->end() ?>

<?php $this->push('stylesheets') ?>

<?php $this->end() ?>