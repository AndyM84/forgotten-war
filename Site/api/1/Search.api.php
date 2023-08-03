<?php

	namespace Api1;

	use Stoic\Web\Api\Response;
	use Stoic\Web\Request;

	use Zibings\ApiController;
	use Zibings\RoleStrings;
	use Zibings\Users;
	use Zibings\UserRelations;
	use Zibings\UserRelationStages;
	use Zibings\UserRoles;
	use Zibings\VisibilityState;

	/**
	 * API controller that deals with user-related endpoints.
	 *
	 * @package Zibings\Api1
	 */
	class Search extends ApiController {
		/**
		 * Registers the controller endpoints.
		 *
		 * @return void
		 */
		protected function registerEndpoints() : void {
			$this->registerEndpoint('GET',  '/^Search\/Users\/?/i',      'searchUsers',  true);

			return;
		}

		/**
		 * Searches database for users who are visible.
		 *
		 * @param Request $request The current request which routed to the endpoint.
		 * @param array|null $matches Array of matches returned by endpoint regex pattern.
		 * @throws \Exception
		 * @return Response
		 */
		public function searchUsers(Request $request, array $matches = null) : Response {
			$data      = [];
			$user      = $this->getUser();
			$ret       = $this->newResponse();
			$params    = $request->getInput();
			$usersRepo = new Users($this->db, $this->log);

			if (!$params->has('query')) {
				$ret->setAsError("Must provide query in order to search");

				return $ret;
			}

			$userRels      = [];
			$userRelations = (new UserRelations($this->db, $this->log))->getRelations($user->id);
			$userIsAdmin   = (new UserRoles($this->db, $this->log))->userInRoleByName($user->id, RoleStrings::ADMINISTRATOR);

			foreach ($userRelations as $rel) {
				if ($rel->stage->is(UserRelationStages::ACCEPTED)) {
					$userRels[strval(($rel->userOne == $user->id) ? $rel->userTwo : $rel->userOne)] = true;
				}
			}

			foreach ($usersRepo->searchUsersByIdentifiers($params->getString('query'), !$userIsAdmin) as $usd) {
				$tmp = [
					'email'          => $usd->email,
					'emailConfirmed' => $usd->emailConfirmed,
					'id'             => $usd->id,
					'joined'         => $usd->joined->format('Y-m-d H:i:s'),
					'lastLogin'      => ($usd->lastLogin !== null) ? $usd->lastLogin->format('Y-m-d H:i:s') : '',
					'displayName'    => $usd->displayName,
					'birthday'       => $usd->birthday->format('Y-m-d H:i:s'),
					'realName'       => $usd->realName,
					'description'    => $usd->description,
					'gender'         => $usd->gender->getName()
				];

				if ($userIsAdmin) {
					$data[] = $tmp;

					continue;
				}

				unset($tmp['emailConfirmed']);
				unset($tmp['joined']);
				unset($tmp['lastLogin']);

				$areFriends = array_key_exists(strval($usd->id), $userRels) !== false;

				if (!$areFriends && $usd->visProfile->getValue() < VisibilityState::AUTHENTICATED) {
					continue;
				}

				$correlators = [
					'email'          => 'visEmail',
					'birthday'       => 'visBirthday',
					'realName'       => 'visRealName',
					'description'    => 'visDescription',
					'gender'         => 'visGender'
				];

				foreach ($correlators as $index => $visibility) {
					if (!$areFriends && $usd->$visibility->getValue() < VisibilityState::AUTHENTICATED) {
						unset($tmp[$index]);
					}
				}

				$data[] = $tmp;
			}

			$ret->setData($data);

			return $ret;
		}
	}
