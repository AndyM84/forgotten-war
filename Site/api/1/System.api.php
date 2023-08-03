<?php

	namespace Api1;

	use Stoic\Web\Api\Response;
	use Stoic\Web\Request;

	use Zibings\ApiController;
	use Zibings\SettingsStrings;

	/**
	 * API controller that provides basic system endpoints.
	 *
	 * @package Zibings\Api1
	 */
	class System extends ApiController {
		/**
		 * Registers the controller endpoints.
		 *
		 * @return void
		 */
		protected function registerEndpoints() : void {
			$this->registerEndpoint('GET',  '/^System\/Version\/?/i',      'getVersion');

			return;
		}

		/**
		 * Returns the currently configured system version.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @return Response
		 */
		public function getVersion(Request $request, array $matches = null) : Response {
			global $Settings;

			$ret = $this->newResponse();
			$ret->setData($Settings->get(SettingsStrings::SYSTEM_VERSION));

			return $ret;
		}
	}
