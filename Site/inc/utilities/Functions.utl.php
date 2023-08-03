<?php

	namespace Zibings;

	use AndyM84\Config\ConfigContainer;

	use League\Plates\Engine;

	use PHPMailer\PHPMailer\PHPMailer;

	use Stoic\Log\Logger;
	use Stoic\Pdo\PdoHelper;
	use Stoic\Utilities\ParameterHelper;
	use Stoic\Web\PageHelper;

	/**
	 * Initializes a new curl handler resource and does its best to automatically include the user authentication variables.
	 *
	 * @param string $url The URL suffix to append to the API path from settings.
	 * @param integer $userId The unique identifier for the user making this request.
	 * @param string $token The unique session token for the user making this request.
	 * @param ConfigContainer $settings The site settings to use for building this request.
	 * @param boolean $isPost Optional toggle to mark this as a POST request.
	 * @param array|null $postFields Optional array of fields to merge with the authentication variables.
	 * @return bool|\CurlHandle
	 */
	function getCurlApiResource(string $url, int $userId, string $token, ConfigContainer $settings, bool $isPost = false, array $postFields = null) : bool|\CurlHandle {
		$curlUrl = $settings->get(SettingsStrings::API_PATH, 'http://localhost/api/1') . $url;

		if ($isPost === false && $userId > 0 && !empty($token)) {
			$curlUrl .= (stripos($curlUrl, '?') === false) ? '?' : '&';
			$curlUrl .= "UserID={$userId}&Token={$token}";
		}

		$ch = curl_init();
		curl_setopt_array($ch, [
			CURLOPT_URL => $curlUrl,
			CURLOPT_RETURNTRANSFER => true,
			CURLOPT_HEADER => false,
		]);

		if (!$settings->get(SettingsStrings::CURL_SECURE, false)) {
			curl_setopt_array($ch, [
				CURLOPT_SSL_VERIFYPEER => false,
				CURLOPT_SSL_VERIFYHOST => false
			]);
		}

		if ($isPost) {
			$fields = [];

			if ($userId > 0 && !empty($token)) {
				$fields['UserID'] = $userId;
				$fields['Token'] = $token;
			}

			if ($postFields !== null) {
				$fields = array_merge($fields, $postFields);
			}

			curl_setopt_array($ch, [
				CURLOPT_POST => true,
				CURLOPT_POSTFIELDS => json_encode($fields, JSON_INVALID_UTF8_SUBSTITUTE),
				CURLOPT_HTTPHEADER => [
					"Content-Type: application/json"
				]
			]);
		}

		return $ch;
	}

	/**
	 * Retrieves a PHPMailer object, using provided settings if available, ready to use for sending mail.
	 *
	 * @param \AndyM84\Config\ConfigContainer|null $settings Optional system settings to use for SMTP/mail setup.
	 * @return \PHPMailer\PHPMailer\PHPMailer
	 */
	function getPhpMailer(ConfigContainer $settings = null) : PHPMailer {
		$mail = new PHPMailer();

		if ($settings === null || !$settings->get('smtpHost')) {
			return $mail;
		}

		if ($settings->get('enableSmtp', false)) {
			$mail->isSMTP();
			$mail->Host = $settings->get('smtpHost');

			if ($settings->has('smtpUser') && !empty($settings->get('smtpUser'))) {
				$mail->SMTPAuth = true;
				$mail->Username = $settings->get('smtpUser');
				$mail->Password = $settings->get('smtpPass');
			}

			if ($settings->has('smtpPort') && $settings->get('smtpPort', 0) > 0) {
				$mail->Port = $settings->get('smtpPort');
			}
		}

		if ($settings->has('defaultFromAddress') && !empty($settings->get('defaultFromAddress'))) {
			$mail->From = $settings->get('defaultFromAddress');
		}

		if ($settings->has('defaultFromName') && !empty($settings->get('defaultFromName'))) {
			$mail->FromName = $settings->get('defaultFromName');
		}

		return $mail;
	}

	/**
	 * Returns the list of timezone identifiers with identifiers or a single timezone identifier if a key is provided.
	 * Returns "ERROR" if an invalid key is supplied.
	 *
	 * @param null|string $key Optional key to use for timezone lookup
	 * @return array|int|string
	 */
	function getTimezones(?string $key = null) : array|int|string {
		$ret = [
			"Africa/Abidjan"                 => 0,
			"0"                              => "Africa/Abidjan",
			"Africa/Accra"                   => 1,
			"1"                              => "Africa/Accra",
			"Africa/Addis_Ababa"             => 2,
			"2"                              => "Africa/Addis_Ababa",
			"Africa/Algiers"                 => 3,
			"3"                              => "Africa/Algiers",
			"Africa/Asmara"                  => 4,
			"4"                              => "Africa/Asmara",
			"Africa/Bamako"                  => 5,
			"5"                              => "Africa/Bamako",
			"Africa/Bangui"                  => 6,
			"6"                              => "Africa/Bangui",
			"Africa/Banjul"                  => 7,
			"7"                              => "Africa/Banjul",
			"Africa/Bissau"                  => 8,
			"8"                              => "Africa/Bissau",
			"Africa/Blantyre"                => 9,
			"9"                              => "Africa/Blantyre",
			"Africa/Brazzaville"             => 10,
			"10"                             => "Africa/Brazzaville",
			"Africa/Bujumbura"               => 11,
			"11"                             => "Africa/Bujumbura",
			"Africa/Cairo"                   => 12,
			"12"                             => "Africa/Cairo",
			"Africa/Casablanca"              => 13,
			"13"                             => "Africa/Casablanca",
			"Africa/Ceuta"                   => 14,
			"14"                             => "Africa/Ceuta",
			"Africa/Conakry"                 => 15,
			"15"                             => "Africa/Conakry",
			"Africa/Dakar"                   => 16,
			"16"                             => "Africa/Dakar",
			"Africa/Dar_es_Salaam"           => 17,
			"17"                             => "Africa/Dar_es_Salaam",
			"Africa/Djibouti"                => 18,
			"18"                             => "Africa/Djibouti",
			"Africa/Douala"                  => 19,
			"19"                             => "Africa/Douala",
			"Africa/El_Aaiun"                => 20,
			"20"                             => "Africa/El_Aaiun",
			"Africa/Freetown"                => 21,
			"21"                             => "Africa/Freetown",
			"Africa/Gaborone"                => 22,
			"22"                             => "Africa/Gaborone",
			"Africa/Harare"                  => 23,
			"23"                             => "Africa/Harare",
			"Africa/Johannesburg"            => 24,
			"24"                             => "Africa/Johannesburg",
			"Africa/Juba"                    => 25,
			"25"                             => "Africa/Juba",
			"Africa/Kampala"                 => 26,
			"26"                             => "Africa/Kampala",
			"Africa/Khartoum"                => 27,
			"27"                             => "Africa/Khartoum",
			"Africa/Kigali"                  => 28,
			"28"                             => "Africa/Kigali",
			"Africa/Kinshasa"                => 29,
			"29"                             => "Africa/Kinshasa",
			"Africa/Lagos"                   => 30,
			"30"                             => "Africa/Lagos",
			"Africa/Libreville"              => 31,
			"31"                             => "Africa/Libreville",
			"Africa/Lome"                    => 32,
			"32"                             => "Africa/Lome",
			"Africa/Luanda"                  => 33,
			"33"                             => "Africa/Luanda",
			"Africa/Lubumbashi"              => 34,
			"34"                             => "Africa/Lubumbashi",
			"Africa/Lusaka"                  => 35,
			"35"                             => "Africa/Lusaka",
			"Africa/Malabo"                  => 36,
			"36"                             => "Africa/Malabo",
			"Africa/Maputo"                  => 37,
			"37"                             => "Africa/Maputo",
			"Africa/Maseru"                  => 38,
			"38"                             => "Africa/Maseru",
			"Africa/Mbabane"                 => 39,
			"39"                             => "Africa/Mbabane",
			"Africa/Mogadishu"               => 40,
			"40"                             => "Africa/Mogadishu",
			"Africa/Monrovia"                => 41,
			"41"                             => "Africa/Monrovia",
			"Africa/Nairobi"                 => 42,
			"42"                             => "Africa/Nairobi",
			"Africa/Ndjamena"                => 43,
			"43"                             => "Africa/Ndjamena",
			"Africa/Niamey"                  => 44,
			"44"                             => "Africa/Niamey",
			"Africa/Nouakchott"              => 45,
			"45"                             => "Africa/Nouakchott",
			"Africa/Ouagadougou"             => 46,
			"46"                             => "Africa/Ouagadougou",
			"Africa/Porto-Novo"              => 47,
			"47"                             => "Africa/Porto-Novo",
			"Africa/Sao_Tome"                => 48,
			"48"                             => "Africa/Sao_Tome",
			"Africa/Tripoli"                 => 49,
			"49"                             => "Africa/Tripoli",
			"Africa/Tunis"                   => 50,
			"50"                             => "Africa/Tunis",
			"Africa/Windhoek"                => 51,
			"51"                             => "Africa/Windhoek",
			"America/Adak"                   => 52,
			"52"                             => "America/Adak",
			"America/Anchorage"              => 53,
			"53"                             => "America/Anchorage",
			"America/Anguilla"               => 54,
			"54"                             => "America/Anguilla",
			"America/Antigua"                => 55,
			"55"                             => "America/Antigua",
			"America/Araguaina"              => 56,
			"56"                             => "America/Araguaina",
			"America/Argentina/Buenos_Aires" => 57,
			"57"                             => "America/Argentina/Buenos_Aires",
			"America/Argentina/Catamarca"    => 58,
			"58"                             => "America/Argentina/Catamarca",
			"America/Argentina/Cordoba"      => 59,
			"59"                             => "America/Argentina/Cordoba",
			"America/Argentina/Jujuy"        => 60,
			"60"                             => "America/Argentina/Jujuy",
			"America/Argentina/La_Rioja"     => 61,
			"61"                             => "America/Argentina/La_Rioja",
			"America/Argentina/Mendoza"      => 62,
			"62"                             => "America/Argentina/Mendoza",
			"America/Argentina/Rio_Gallegos" => 63,
			"63"                             => "America/Argentina/Rio_Gallegos",
			"America/Argentina/Salta"        => 64,
			"64"                             => "America/Argentina/Salta",
			"America/Argentina/San_Juan"     => 65,
			"65"                             => "America/Argentina/San_Juan",
			"America/Argentina/San_Luis"     => 66,
			"66"                             => "America/Argentina/San_Luis",
			"America/Argentina/Tucuman"      => 67,
			"67"                             => "America/Argentina/Tucuman",
			"America/Argentina/Ushuaia"      => 68,
			"68"                             => "America/Argentina/Ushuaia",
			"America/Aruba"                  => 69,
			"69"                             => "America/Aruba",
			"America/Asuncion"               => 70,
			"70"                             => "America/Asuncion",
			"America/Atikokan"               => 71,
			"71"                             => "America/Atikokan",
			"America/Bahia"                  => 72,
			"72"                             => "America/Bahia",
			"America/Bahia_Banderas"         => 73,
			"73"                             => "America/Bahia_Banderas",
			"America/Barbados"               => 74,
			"74"                             => "America/Barbados",
			"America/Belem"                  => 75,
			"75"                             => "America/Belem",
			"America/Belize"                 => 76,
			"76"                             => "America/Belize",
			"America/Blanc-Sablon"           => 77,
			"77"                             => "America/Blanc-Sablon",
			"America/Boa_Vista"              => 78,
			"78"                             => "America/Boa_Vista",
			"America/Bogota"                 => 79,
			"79"                             => "America/Bogota",
			"America/Boise"                  => 80,
			"80"                             => "America/Boise",
			"America/Cambridge_Bay"          => 81,
			"81"                             => "America/Cambridge_Bay",
			"America/Campo_Grande"           => 82,
			"82"                             => "America/Campo_Grande",
			"America/Cancun"                 => 83,
			"83"                             => "America/Cancun",
			"America/Caracas"                => 84,
			"84"                             => "America/Caracas",
			"America/Cayenne"                => 85,
			"85"                             => "America/Cayenne",
			"America/Cayman"                 => 86,
			"86"                             => "America/Cayman",
			"America/Chicago"                => 87,
			"87"                             => "America/Chicago",
			"America/Chihuahua"              => 88,
			"88"                             => "America/Chihuahua",
			"America/Costa_Rica"             => 89,
			"89"                             => "America/Costa_Rica",
			"America/Creston"                => 90,
			"90"                             => "America/Creston",
			"America/Cuiaba"                 => 91,
			"91"                             => "America/Cuiaba",
			"America/Curacao"                => 92,
			"92"                             => "America/Curacao",
			"America/Danmarkshavn"           => 93,
			"93"                             => "America/Danmarkshavn",
			"America/Dawson"                 => 94,
			"94"                             => "America/Dawson",
			"America/Dawson_Creek"           => 95,
			"95"                             => "America/Dawson_Creek",
			"America/Denver"                 => 96,
			"96"                             => "America/Denver",
			"America/Detroit"                => 97,
			"97"                             => "America/Detroit",
			"America/Dominica"               => 98,
			"98"                             => "America/Dominica",
			"America/Edmonton"               => 99,
			"99"                             => "America/Edmonton",
			"America/Eirunepe"               => 100,
			"100"                            => "America/Eirunepe",
			"America/El_Salvador"            => 101,
			"101"                            => "America/El_Salvador",
			"America/Fort_Nelson"            => 102,
			"102"                            => "America/Fort_Nelson",
			"America/Fortaleza"              => 103,
			"103"                            => "America/Fortaleza",
			"America/Glace_Bay"              => 104,
			"104"                            => "America/Glace_Bay",
			"America/Godthab"                => 105,
			"105"                            => "America/Godthab",
			"America/Goose_Bay"              => 106,
			"106"                            => "America/Goose_Bay",
			"America/Grand_Turk"             => 107,
			"107"                            => "America/Grand_Turk",
			"America/Grenada"                => 108,
			"108"                            => "America/Grenada",
			"America/Guadeloupe"             => 109,
			"109"                            => "America/Guadeloupe",
			"America/Guatemala"              => 110,
			"110"                            => "America/Guatemala",
			"America/Guayaquil"              => 111,
			"111"                            => "America/Guayaquil",
			"America/Guyana"                 => 112,
			"112"                            => "America/Guyana",
			"America/Halifax"                => 113,
			"113"                            => "America/Halifax",
			"America/Havana"                 => 114,
			"114"                            => "America/Havana",
			"America/Hermosillo"             => 115,
			"115"                            => "America/Hermosillo",
			"America/Indiana/Indianapolis"   => 116,
			"116"                            => "America/Indiana/Indianapolis",
			"America/Indiana/Knox"           => 117,
			"117"                            => "America/Indiana/Knox",
			"America/Indiana/Marengo"        => 118,
			"118"                            => "America/Indiana/Marengo",
			"America/Indiana/Petersburg"     => 119,
			"119"                            => "America/Indiana/Petersburg",
			"America/Indiana/Tell_City"      => 120,
			"120"                            => "America/Indiana/Tell_City",
			"America/Indiana/Vevay"          => 121,
			"121"                            => "America/Indiana/Vevay",
			"America/Indiana/Vincennes"      => 122,
			"122"                            => "America/Indiana/Vincennes",
			"America/Indiana/Winamac"        => 123,
			"123"                            => "America/Indiana/Winamac",
			"America/Inuvik"                 => 124,
			"124"                            => "America/Inuvik",
			"America/Iqaluit"                => 125,
			"125"                            => "America/Iqaluit",
			"America/Jamaica"                => 126,
			"126"                            => "America/Jamaica",
			"America/Juneau"                 => 127,
			"127"                            => "America/Juneau",
			"America/Kentucky/Louisville"    => 128,
			"128"                            => "America/Kentucky/Louisville",
			"America/Kentucky/Monticello"    => 129,
			"129"                            => "America/Kentucky/Monticello",
			"America/Kralendijk"             => 130,
			"130"                            => "America/Kralendijk",
			"America/La_Paz"                 => 131,
			"131"                            => "America/La_Paz",
			"America/Lima"                   => 132,
			"132"                            => "America/Lima",
			"America/Los_Angeles"            => 133,
			"133"                            => "America/Los_Angeles",
			"America/Lower_Princes"          => 134,
			"134"                            => "America/Lower_Princes",
			"America/Maceio"                 => 135,
			"135"                            => "America/Maceio",
			"America/Managua"                => 136,
			"136"                            => "America/Managua",
			"America/Manaus"                 => 137,
			"137"                            => "America/Manaus",
			"America/Marigot"                => 138,
			"138"                            => "America/Marigot",
			"America/Martinique"             => 139,
			"139"                            => "America/Martinique",
			"America/Matamoros"              => 140,
			"140"                            => "America/Matamoros",
			"America/Mazatlan"               => 141,
			"141"                            => "America/Mazatlan",
			"America/Menominee"              => 142,
			"142"                            => "America/Menominee",
			"America/Merida"                 => 143,
			"143"                            => "America/Merida",
			"America/Metlakatla"             => 144,
			"144"                            => "America/Metlakatla",
			"America/Mexico_City"            => 145,
			"145"                            => "America/Mexico_City",
			"America/Miquelon"               => 146,
			"146"                            => "America/Miquelon",
			"America/Moncton"                => 147,
			"147"                            => "America/Moncton",
			"America/Monterrey"              => 148,
			"148"                            => "America/Monterrey",
			"America/Montevideo"             => 149,
			"149"                            => "America/Montevideo",
			"America/Montserrat"             => 150,
			"150"                            => "America/Montserrat",
			"America/Nassau"                 => 151,
			"151"                            => "America/Nassau",
			"America/New_York"               => 152,
			"152"                            => "America/New_York",
			"America/Nipigon"                => 153,
			"153"                            => "America/Nipigon",
			"America/Nome"                   => 154,
			"154"                            => "America/Nome",
			"America/Noronha"                => 155,
			"155"                            => "America/Noronha",
			"America/North_Dakota/Beulah"    => 156,
			"156"                            => "America/North_Dakota/Beulah",
			"America/North_Dakota/Center"    => 157,
			"157"                            => "America/North_Dakota/Center",
			"America/North_Dakota/New_Salem" => 158,
			"158"                            => "America/North_Dakota/New_Salem",
			"America/Ojinaga"                => 159,
			"159"                            => "America/Ojinaga",
			"America/Panama"                 => 160,
			"160"                            => "America/Panama",
			"America/Pangnirtung"            => 161,
			"161"                            => "America/Pangnirtung",
			"America/Paramaribo"             => 162,
			"162"                            => "America/Paramaribo",
			"America/Phoenix"                => 163,
			"163"                            => "America/Phoenix",
			"America/Port-au-Prince"         => 164,
			"164"                            => "America/Port-au-Prince",
			"America/Port_of_Spain"          => 165,
			"165"                            => "America/Port_of_Spain",
			"America/Porto_Velho"            => 166,
			"166"                            => "America/Porto_Velho",
			"America/Puerto_Rico"            => 167,
			"167"                            => "America/Puerto_Rico",
			"America/Punta_Arenas"           => 168,
			"168"                            => "America/Punta_Arenas",
			"America/Rainy_River"            => 169,
			"169"                            => "America/Rainy_River",
			"America/Rankin_Inlet"           => 170,
			"170"                            => "America/Rankin_Inlet",
			"America/Recife"                 => 171,
			"171"                            => "America/Recife",
			"America/Regina"                 => 172,
			"172"                            => "America/Regina",
			"America/Resolute"               => 173,
			"173"                            => "America/Resolute",
			"America/Rio_Branco"             => 174,
			"174"                            => "America/Rio_Branco",
			"America/Santarem"               => 175,
			"175"                            => "America/Santarem",
			"America/Santiago"               => 176,
			"176"                            => "America/Santiago",
			"America/Santo_Domingo"          => 177,
			"177"                            => "America/Santo_Domingo",
			"America/Sao_Paulo"              => 178,
			"178"                            => "America/Sao_Paulo",
			"America/Scoresbysund"           => 179,
			"179"                            => "America/Scoresbysund",
			"America/Sitka"                  => 180,
			"180"                            => "America/Sitka",
			"America/St_Barthelemy"          => 181,
			"181"                            => "America/St_Barthelemy",
			"America/St_Johns"               => 182,
			"182"                            => "America/St_Johns",
			"America/St_Kitts"               => 183,
			"183"                            => "America/St_Kitts",
			"America/St_Lucia"               => 184,
			"184"                            => "America/St_Lucia",
			"America/St_Thomas"              => 185,
			"185"                            => "America/St_Thomas",
			"America/St_Vincent"             => 186,
			"186"                            => "America/St_Vincent",
			"America/Swift_Current"          => 187,
			"187"                            => "America/Swift_Current",
			"America/Tegucigalpa"            => 188,
			"188"                            => "America/Tegucigalpa",
			"America/Thule"                  => 189,
			"189"                            => "America/Thule",
			"America/Thunder_Bay"            => 190,
			"190"                            => "America/Thunder_Bay",
			"America/Tijuana"                => 191,
			"191"                            => "America/Tijuana",
			"America/Toronto"                => 192,
			"192"                            => "America/Toronto",
			"America/Tortola"                => 193,
			"193"                            => "America/Tortola",
			"America/Vancouver"              => 194,
			"194"                            => "America/Vancouver",
			"America/Whitehorse"             => 195,
			"195"                            => "America/Whitehorse",
			"America/Winnipeg"               => 196,
			"196"                            => "America/Winnipeg",
			"America/Yakutat"                => 197,
			"197"                            => "America/Yakutat",
			"America/Yellowknife"            => 198,
			"198"                            => "America/Yellowknife",
			"Antarctica/Casey"               => 199,
			"199"                            => "Antarctica/Casey",
			"Antarctica/Davis"               => 200,
			"200"                            => "Antarctica/Davis",
			"Antarctica/DumontDUrville"      => 201,
			"201"                            => "Antarctica/DumontDUrville",
			"Antarctica/Macquarie"           => 202,
			"202"                            => "Antarctica/Macquarie",
			"Antarctica/Mawson"              => 203,
			"203"                            => "Antarctica/Mawson",
			"Antarctica/McMurdo"             => 204,
			"204"                            => "Antarctica/McMurdo",
			"Antarctica/Palmer"              => 205,
			"205"                            => "Antarctica/Palmer",
			"Antarctica/Rothera"             => 206,
			"206"                            => "Antarctica/Rothera",
			"Antarctica/Syowa"               => 207,
			"207"                            => "Antarctica/Syowa",
			"Antarctica/Troll"               => 208,
			"208"                            => "Antarctica/Troll",
			"Antarctica/Vostok"              => 209,
			"209"                            => "Antarctica/Vostok",
			"Arctic/Longyearbyen"            => 210,
			"210"                            => "Arctic/Longyearbyen",
			"Asia/Aden"                      => 211,
			"211"                            => "Asia/Aden",
			"Asia/Almaty"                    => 212,
			"212"                            => "Asia/Almaty",
			"Asia/Amman"                     => 213,
			"213"                            => "Asia/Amman",
			"Asia/Anadyr"                    => 214,
			"214"                            => "Asia/Anadyr",
			"Asia/Aqtau"                     => 215,
			"215"                            => "Asia/Aqtau",
			"Asia/Aqtobe"                    => 216,
			"216"                            => "Asia/Aqtobe",
			"Asia/Ashgabat"                  => 217,
			"217"                            => "Asia/Ashgabat",
			"Asia/Atyrau"                    => 218,
			"218"                            => "Asia/Atyrau",
			"Asia/Baghdad"                   => 219,
			"219"                            => "Asia/Baghdad",
			"Asia/Bahrain"                   => 220,
			"220"                            => "Asia/Bahrain",
			"Asia/Baku"                      => 221,
			"221"                            => "Asia/Baku",
			"Asia/Bangkok"                   => 222,
			"222"                            => "Asia/Bangkok",
			"Asia/Barnaul"                   => 223,
			"223"                            => "Asia/Barnaul",
			"Asia/Beirut"                    => 224,
			"224"                            => "Asia/Beirut",
			"Asia/Bishkek"                   => 225,
			"225"                            => "Asia/Bishkek",
			"Asia/Brunei"                    => 226,
			"226"                            => "Asia/Brunei",
			"Asia/Chita"                     => 227,
			"227"                            => "Asia/Chita",
			"Asia/Choibalsan"                => 228,
			"228"                            => "Asia/Choibalsan",
			"Asia/Colombo"                   => 229,
			"229"                            => "Asia/Colombo",
			"Asia/Damascus"                  => 230,
			"230"                            => "Asia/Damascus",
			"Asia/Dhaka"                     => 231,
			"231"                            => "Asia/Dhaka",
			"Asia/Dili"                      => 232,
			"232"                            => "Asia/Dili",
			"Asia/Dubai"                     => 233,
			"233"                            => "Asia/Dubai",
			"Asia/Dushanbe"                  => 234,
			"234"                            => "Asia/Dushanbe",
			"Asia/Famagusta"                 => 235,
			"235"                            => "Asia/Famagusta",
			"Asia/Gaza"                      => 236,
			"236"                            => "Asia/Gaza",
			"Asia/Hebron"                    => 237,
			"237"                            => "Asia/Hebron",
			"Asia/Ho_Chi_Minh"               => 238,
			"238"                            => "Asia/Ho_Chi_Minh",
			"Asia/Hong_Kong"                 => 239,
			"239"                            => "Asia/Hong_Kong",
			"Asia/Hovd"                      => 240,
			"240"                            => "Asia/Hovd",
			"Asia/Irkutsk"                   => 241,
			"241"                            => "Asia/Irkutsk",
			"Asia/Jakarta"                   => 242,
			"242"                            => "Asia/Jakarta",
			"Asia/Jayapura"                  => 243,
			"243"                            => "Asia/Jayapura",
			"Asia/Jerusalem"                 => 244,
			"244"                            => "Asia/Jerusalem",
			"Asia/Kabul"                     => 245,
			"245"                            => "Asia/Kabul",
			"Asia/Kamchatka"                 => 246,
			"246"                            => "Asia/Kamchatka",
			"Asia/Karachi"                   => 247,
			"247"                            => "Asia/Karachi",
			"Asia/Kathmandu"                 => 248,
			"248"                            => "Asia/Kathmandu",
			"Asia/Khandyga"                  => 249,
			"249"                            => "Asia/Khandyga",
			"Asia/Kolkata"                   => 250,
			"250"                            => "Asia/Kolkata",
			"Asia/Krasnoyarsk"               => 251,
			"251"                            => "Asia/Krasnoyarsk",
			"Asia/Kuala_Lumpur"              => 252,
			"252"                            => "Asia/Kuala_Lumpur",
			"Asia/Kuching"                   => 253,
			"253"                            => "Asia/Kuching",
			"Asia/Kuwait"                    => 254,
			"254"                            => "Asia/Kuwait",
			"Asia/Macau"                     => 255,
			"255"                            => "Asia/Macau",
			"Asia/Magadan"                   => 256,
			"256"                            => "Asia/Magadan",
			"Asia/Makassar"                  => 257,
			"257"                            => "Asia/Makassar",
			"Asia/Manila"                    => 258,
			"258"                            => "Asia/Manila",
			"Asia/Muscat"                    => 259,
			"259"                            => "Asia/Muscat",
			"Asia/Nicosia"                   => 260,
			"260"                            => "Asia/Nicosia",
			"Asia/Novokuznetsk"              => 261,
			"261"                            => "Asia/Novokuznetsk",
			"Asia/Novosibirsk"               => 262,
			"262"                            => "Asia/Novosibirsk",
			"Asia/Omsk"                      => 263,
			"263"                            => "Asia/Omsk",
			"Asia/Oral"                      => 264,
			"264"                            => "Asia/Oral",
			"Asia/Phnom_Penh"                => 265,
			"265"                            => "Asia/Phnom_Penh",
			"Asia/Pontianak"                 => 266,
			"266"                            => "Asia/Pontianak",
			"Asia/Pyongyang"                 => 267,
			"267"                            => "Asia/Pyongyang",
			"Asia/Qatar"                     => 268,
			"268"                            => "Asia/Qatar",
			"Asia/Qostanay"                  => 269,
			"269"                            => "Asia/Qostanay",
			"Asia/Qyzylorda"                 => 270,
			"270"                            => "Asia/Qyzylorda",
			"Asia/Riyadh"                    => 271,
			"271"                            => "Asia/Riyadh",
			"Asia/Sakhalin"                  => 272,
			"272"                            => "Asia/Sakhalin",
			"Asia/Samarkand"                 => 273,
			"273"                            => "Asia/Samarkand",
			"Asia/Seoul"                     => 274,
			"274"                            => "Asia/Seoul",
			"Asia/Shanghai"                  => 275,
			"275"                            => "Asia/Shanghai",
			"Asia/Singapore"                 => 276,
			"276"                            => "Asia/Singapore",
			"Asia/Srednekolymsk"             => 277,
			"277"                            => "Asia/Srednekolymsk",
			"Asia/Taipei"                    => 278,
			"278"                            => "Asia/Taipei",
			"Asia/Tashkent"                  => 279,
			"279"                            => "Asia/Tashkent",
			"Asia/Tbilisi"                   => 280,
			"280"                            => "Asia/Tbilisi",
			"Asia/Tehran"                    => 281,
			"281"                            => "Asia/Tehran",
			"Asia/Thimphu"                   => 282,
			"282"                            => "Asia/Thimphu",
			"Asia/Tokyo"                     => 283,
			"283"                            => "Asia/Tokyo",
			"Asia/Tomsk"                     => 284,
			"284"                            => "Asia/Tomsk",
			"Asia/Ulaanbaatar"               => 285,
			"285"                            => "Asia/Ulaanbaatar",
			"Asia/Urumqi"                    => 286,
			"286"                            => "Asia/Urumqi",
			"Asia/Ust-Nera"                  => 287,
			"287"                            => "Asia/Ust-Nera",
			"Asia/Vientiane"                 => 288,
			"288"                            => "Asia/Vientiane",
			"Asia/Vladivostok"               => 289,
			"289"                            => "Asia/Vladivostok",
			"Asia/Yakutsk"                   => 290,
			"290"                            => "Asia/Yakutsk",
			"Asia/Yangon"                    => 291,
			"291"                            => "Asia/Yangon",
			"Asia/Yekaterinburg"             => 292,
			"292"                            => "Asia/Yekaterinburg",
			"Asia/Yerevan"                   => 293,
			"293"                            => "Asia/Yerevan",
			"Atlantic/Azores"                => 294,
			"294"                            => "Atlantic/Azores",
			"Atlantic/Bermuda"               => 295,
			"295"                            => "Atlantic/Bermuda",
			"Atlantic/Canary"                => 296,
			"296"                            => "Atlantic/Canary",
			"Atlantic/Cape_Verde"            => 297,
			"297"                            => "Atlantic/Cape_Verde",
			"Atlantic/Faroe"                 => 298,
			"298"                            => "Atlantic/Faroe",
			"Atlantic/Madeira"               => 299,
			"299"                            => "Atlantic/Madeira",
			"Atlantic/Reykjavik"             => 300,
			"300"                            => "Atlantic/Reykjavik",
			"Atlantic/South_Georgia"         => 301,
			"301"                            => "Atlantic/South_Georgia",
			"Atlantic/St_Helena"             => 302,
			"302"                            => "Atlantic/St_Helena",
			"Atlantic/Stanley"               => 303,
			"303"                            => "Atlantic/Stanley",
			"Australia/Adelaide"             => 304,
			"304"                            => "Australia/Adelaide",
			"Australia/Brisbane"             => 305,
			"305"                            => "Australia/Brisbane",
			"Australia/Broken_Hill"          => 306,
			"306"                            => "Australia/Broken_Hill",
			"Australia/Currie"               => 307,
			"307"                            => "Australia/Currie",
			"Australia/Darwin"               => 308,
			"308"                            => "Australia/Darwin",
			"Australia/Eucla"                => 309,
			"309"                            => "Australia/Eucla",
			"Australia/Hobart"               => 310,
			"310"                            => "Australia/Hobart",
			"Australia/Lindeman"             => 311,
			"311"                            => "Australia/Lindeman",
			"Australia/Lord_Howe"            => 312,
			"312"                            => "Australia/Lord_Howe",
			"Australia/Melbourne"            => 313,
			"313"                            => "Australia/Melbourne",
			"Australia/Perth"                => 314,
			"314"                            => "Australia/Perth",
			"Australia/Sydney"               => 315,
			"315"                            => "Australia/Sydney",
			"Europe/Amsterdam"               => 316,
			"316"                            => "Europe/Amsterdam",
			"Europe/Andorra"                 => 317,
			"317"                            => "Europe/Andorra",
			"Europe/Astrakhan"               => 318,
			"318"                            => "Europe/Astrakhan",
			"Europe/Athens"                  => 319,
			"319"                            => "Europe/Athens",
			"Europe/Belgrade"                => 320,
			"320"                            => "Europe/Belgrade",
			"Europe/Berlin"                  => 321,
			"321"                            => "Europe/Berlin",
			"Europe/Bratislava"              => 322,
			"322"                            => "Europe/Bratislava",
			"Europe/Brussels"                => 323,
			"323"                            => "Europe/Brussels",
			"Europe/Bucharest"               => 324,
			"324"                            => "Europe/Bucharest",
			"Europe/Budapest"                => 325,
			"325"                            => "Europe/Budapest",
			"Europe/Busingen"                => 326,
			"326"                            => "Europe/Busingen",
			"Europe/Chisinau"                => 327,
			"327"                            => "Europe/Chisinau",
			"Europe/Copenhagen"              => 328,
			"328"                            => "Europe/Copenhagen",
			"Europe/Dublin"                  => 329,
			"329"                            => "Europe/Dublin",
			"Europe/Gibraltar"               => 330,
			"330"                            => "Europe/Gibraltar",
			"Europe/Guernsey"                => 331,
			"331"                            => "Europe/Guernsey",
			"Europe/Helsinki"                => 332,
			"332"                            => "Europe/Helsinki",
			"Europe/Isle_of_Man"             => 333,
			"333"                            => "Europe/Isle_of_Man",
			"Europe/Istanbul"                => 334,
			"334"                            => "Europe/Istanbul",
			"Europe/Jersey"                  => 335,
			"335"                            => "Europe/Jersey",
			"Europe/Kaliningrad"             => 336,
			"336"                            => "Europe/Kaliningrad",
			"Europe/Kiev"                    => 337,
			"337"                            => "Europe/Kiev",
			"Europe/Kirov"                   => 338,
			"338"                            => "Europe/Kirov",
			"Europe/Lisbon"                  => 339,
			"339"                            => "Europe/Lisbon",
			"Europe/Ljubljana"               => 340,
			"340"                            => "Europe/Ljubljana",
			"Europe/London"                  => 341,
			"341"                            => "Europe/London",
			"Europe/Luxembourg"              => 342,
			"342"                            => "Europe/Luxembourg",
			"Europe/Madrid"                  => 343,
			"343"                            => "Europe/Madrid",
			"Europe/Malta"                   => 344,
			"344"                            => "Europe/Malta",
			"Europe/Mariehamn"               => 345,
			"345"                            => "Europe/Mariehamn",
			"Europe/Minsk"                   => 346,
			"346"                            => "Europe/Minsk",
			"Europe/Monaco"                  => 347,
			"347"                            => "Europe/Monaco",
			"Europe/Moscow"                  => 348,
			"348"                            => "Europe/Moscow",
			"Europe/Oslo"                    => 349,
			"349"                            => "Europe/Oslo",
			"Europe/Paris"                   => 350,
			"350"                            => "Europe/Paris",
			"Europe/Podgorica"               => 351,
			"351"                            => "Europe/Podgorica",
			"Europe/Prague"                  => 352,
			"352"                            => "Europe/Prague",
			"Europe/Riga"                    => 353,
			"353"                            => "Europe/Riga",
			"Europe/Rome"                    => 354,
			"354"                            => "Europe/Rome",
			"Europe/Samara"                  => 355,
			"355"                            => "Europe/Samara",
			"Europe/San_Marino"              => 356,
			"356"                            => "Europe/San_Marino",
			"Europe/Sarajevo"                => 357,
			"357"                            => "Europe/Sarajevo",
			"Europe/Saratov"                 => 358,
			"358"                            => "Europe/Saratov",
			"Europe/Simferopol"              => 359,
			"359"                            => "Europe/Simferopol",
			"Europe/Skopje"                  => 360,
			"360"                            => "Europe/Skopje",
			"Europe/Sofia"                   => 361,
			"361"                            => "Europe/Sofia",
			"Europe/Stockholm"               => 362,
			"362"                            => "Europe/Stockholm",
			"Europe/Tallinn"                 => 363,
			"363"                            => "Europe/Tallinn",
			"Europe/Tirane"                  => 364,
			"364"                            => "Europe/Tirane",
			"Europe/Ulyanovsk"               => 365,
			"365"                            => "Europe/Ulyanovsk",
			"Europe/Uzhgorod"                => 366,
			"366"                            => "Europe/Uzhgorod",
			"Europe/Vaduz"                   => 367,
			"367"                            => "Europe/Vaduz",
			"Europe/Vatican"                 => 368,
			"368"                            => "Europe/Vatican",
			"Europe/Vienna"                  => 369,
			"369"                            => "Europe/Vienna",
			"Europe/Vilnius"                 => 370,
			"370"                            => "Europe/Vilnius",
			"Europe/Volgograd"               => 371,
			"371"                            => "Europe/Volgograd",
			"Europe/Warsaw"                  => 372,
			"372"                            => "Europe/Warsaw",
			"Europe/Zagreb"                  => 373,
			"373"                            => "Europe/Zagreb",
			"Europe/Zaporozhye"              => 374,
			"374"                            => "Europe/Zaporozhye",
			"Europe/Zurich"                  => 375,
			"375"                            => "Europe/Zurich",
			"Indian/Antananarivo"            => 376,
			"376"                            => "Indian/Antananarivo",
			"Indian/Chagos"                  => 377,
			"377"                            => "Indian/Chagos",
			"Indian/Christmas"               => 378,
			"378"                            => "Indian/Christmas",
			"Indian/Cocos"                   => 379,
			"379"                            => "Indian/Cocos",
			"Indian/Comoro"                  => 380,
			"380"                            => "Indian/Comoro",
			"Indian/Kerguelen"               => 381,
			"381"                            => "Indian/Kerguelen",
			"Indian/Mahe"                    => 382,
			"382"                            => "Indian/Mahe",
			"Indian/Maldives"                => 383,
			"383"                            => "Indian/Maldives",
			"Indian/Mauritius"               => 384,
			"384"                            => "Indian/Mauritius",
			"Indian/Mayotte"                 => 385,
			"385"                            => "Indian/Mayotte",
			"Indian/Reunion"                 => 386,
			"386"                            => "Indian/Reunion",
			"Pacific/Apia"                   => 387,
			"387"                            => "Pacific/Apia",
			"Pacific/Auckland"               => 388,
			"388"                            => "Pacific/Auckland",
			"Pacific/Bougainville"           => 389,
			"389"                            => "Pacific/Bougainville",
			"Pacific/Chatham"                => 390,
			"390"                            => "Pacific/Chatham",
			"Pacific/Chuuk"                  => 391,
			"391"                            => "Pacific/Chuuk",
			"Pacific/Easter"                 => 392,
			"392"                            => "Pacific/Easter",
			"Pacific/Efate"                  => 393,
			"393"                            => "Pacific/Efate",
			"Pacific/Enderbury"              => 394,
			"394"                            => "Pacific/Enderbury",
			"Pacific/Fakaofo"                => 395,
			"395"                            => "Pacific/Fakaofo",
			"Pacific/Fiji"                   => 396,
			"396"                            => "Pacific/Fiji",
			"Pacific/Funafuti"               => 397,
			"397"                            => "Pacific/Funafuti",
			"Pacific/Galapagos"              => 398,
			"398"                            => "Pacific/Galapagos",
			"Pacific/Gambier"                => 399,
			"399"                            => "Pacific/Gambier",
			"Pacific/Guadalcanal"            => 400,
			"400"                            => "Pacific/Guadalcanal",
			"Pacific/Guam"                   => 401,
			"401"                            => "Pacific/Guam",
			"Pacific/Honolulu"               => 402,
			"402"                            => "Pacific/Honolulu",
			"Pacific/Kiritimati"             => 403,
			"403"                            => "Pacific/Kiritimati",
			"Pacific/Kosrae"                 => 404,
			"404"                            => "Pacific/Kosrae",
			"Pacific/Kwajalein"              => 405,
			"405"                            => "Pacific/Kwajalein",
			"Pacific/Majuro"                 => 406,
			"406"                            => "Pacific/Majuro",
			"Pacific/Marquesas"              => 407,
			"407"                            => "Pacific/Marquesas",
			"Pacific/Midway"                 => 408,
			"408"                            => "Pacific/Midway",
			"Pacific/Nauru"                  => 409,
			"409"                            => "Pacific/Nauru",
			"Pacific/Niue"                   => 410,
			"410"                            => "Pacific/Niue",
			"Pacific/Norfolk"                => 411,
			"411"                            => "Pacific/Norfolk",
			"Pacific/Noumea"                 => 412,
			"412"                            => "Pacific/Noumea",
			"Pacific/Pago_Pago"              => 413,
			"413"                            => "Pacific/Pago_Pago",
			"Pacific/Palau"                  => 414,
			"414"                            => "Pacific/Palau",
			"Pacific/Pitcairn"               => 415,
			"415"                            => "Pacific/Pitcairn",
			"Pacific/Pohnpei"                => 416,
			"416"                            => "Pacific/Pohnpei",
			"Pacific/Port_Moresby"           => 417,
			"417"                            => "Pacific/Port_Moresby",
			"Pacific/Rarotonga"              => 418,
			"418"                            => "Pacific/Rarotonga",
			"Pacific/Saipan"                 => 419,
			"419"                            => "Pacific/Saipan",
			"Pacific/Tahiti"                 => 420,
			"420"                            => "Pacific/Tahiti",
			"Pacific/Tarawa"                 => 421,
			"421"                            => "Pacific/Tarawa",
			"Pacific/Tongatapu"              => 422,
			"422"                            => "Pacific/Tongatapu",
			"Pacific/Wake"                   => 423,
			"423"                            => "Pacific/Wake",
			"Pacific/Wallis"                 => 424,
			"424"                            => "Pacific/Wallis",
			"UTC"                            => 425,
			"425"                            => "UTC"
		];

		if ($key !== null) {
			return array_key_exists($key, $ret) !== false ? $ret[$key] : "ERROR";
		}

		return $ret;
	}

	/**
	 * Attempts to return a User object based off of the Authorization bearer token, if present.
	 *
	 * @param \Stoic\Pdo\PdoHelper $db PdoHelper instance for internal use.
	 * @param null|\Stoic\Log\Logger $log Optional Logger instance for internal use, new instance created by default.
	 * @throws \Exception
	 * @return User
	 */
	function getUserFromBearerToken(PdoHelper $db, Logger|null $log = null) : User {
		$headers = [];
		$ret     = new User($db, $log);

		if (function_exists('getallheaders')) {
			$headers = getallheaders();
		}

		if (array_key_exists('Authorization', $headers) === false) {
			return $ret;
		}

		$session = UserSession::fromToken(base64_decode(str_replace('Bearer ', '', $headers['Authorization'])), $db, $log);

		if ($session->id < 1) {
			return $ret;
		}

		return User::fromId($session->userId, $db, $log);
	}

	/**
	 * Attempts to return a User object based off of the session token, if present.
	 *
	 * @param \Stoic\Utilities\ParameterHelper $session ParameterHelper for session variables.
	 * @param \Stoic\Pdo\PdoHelper $db PdoHelper instance for internal use.
	 * @param null|\Stoic\Log\Logger $log Optional Logger instance for internal use, new instance created by default.
	 * @throws \Exception
	 * @return User
	 */
	function getUserFromSessionToken(ParameterHelper $session, PdoHelper $db, Logger|null $log = null) : User {
		$ret = new User($db, $log);

		if (!$session->hasAll(UserEvents::STR_SESSION_TOKEN, UserEvents::STR_SESSION_USERID)) {
			return $ret;
		}

		$userSession = UserSession::fromToken($session->getString(UserEvents::STR_SESSION_TOKEN), $db, $log);

		if ($userSession->id < 1) {
			return $ret;
		}

		return User::fromId($userSession->userId, $db, $log);
	}

	/**
	 * Determines if the given user is currently authenticated via the session.
	 *
	 * @param \Stoic\Pdo\PdoHelper $db PdoHelper instance for internal use.
	 * @param mixed|null $roles Optional roles to check against a logged-in user.
	 * @param \Stoic\Utilities\ParameterHelper|null $session Optional session data, pulls from $_SESSION if not provided.
	 * @param \Stoic\Log\Logger|null $log Optional Logger instance for internal use, new instance created by default.
	 * @return bool
	 */
	function isAuthenticated(PdoHelper $db, mixed $roles = null, ParameterHelper|null $session = null,  Logger|null $log = null) : bool {
		if ($session === null) {
			$session = new ParameterHelper($_SESSION);
		}

		if (!$session->hasAll(UserEvents::STR_SESSION_TOKEN, UserEvents::STR_SESSION_USERID)) {
			return false;
		}

		$token       = $session->getString(UserEvents::STR_SESSION_TOKEN);
		$userId      = $session->getInt(UserEvents::STR_SESSION_USERID);
		$userSession = UserSession::fromToken($token, $db, $log);

		if ($userSession->userId != $userId) {
			return false;
		}

		if ($roles !== null) {
			if (!is_array($roles)) {
				$roles = [$roles];
			}

			$userRolesRepo = new UserRoles($db, $log);
			$userRoles     = $userRolesRepo->getAllUserRoles($userId);

			if (count($userRoles) < 1) {
				return false;
			}

			foreach ($roles as $r) {
				if (array_key_exists($r, $userRoles) === false) {
					return false;
				}
			}
		}

		return true;
	}

	/**
	 * Attempts to send a password reset email.
	 *
	 * @param string $email Email address to use for finding user in question.
	 * @param \Stoic\Web\PageHelper $page PageHelper instance for resolving current domain in email links.
	 * @param \AndyM84\Config\ConfigContainer $settings Settings container for site.
	 * @param \Stoic\Pdo\PdoHelper $db PdoHelper instance for internal use.
	 * @param \Stoic\Log\Logger|null $log Optional Logger instance for internal use, new instance created by default.
	 * @throws \PHPMailer\PHPMailer\Exception
	 * @return bool
	 */
	function sendResetEmail(string $email, PageHelper $page, ConfigContainer $settings, PdoHelper $db, Logger|null $log = null) : bool {
		$user = User::fromEmail($email, $db, $log);

		if ($user->id < 1) {
			return false;
		}

		$ut          = new UserToken($db, $log);
		$ut->context = "PASSWORD RESET";
		$ut->token   = UserSession::generateGuid(false);
		$ut->userId  = $user->id;

		if ($ut->create()->isBad()) {
			return false;
		}

		$tpl = new Engine(null, 'tpl.php');
		$tpl->addFolder('shared', STOIC_CORE_PATH . '/tpl/shared');
		$tpl->addFolder('emails', STOIC_CORE_PATH . '/tpl/emails');

		$mail          = getPhpMailer($settings);
		$mail->Subject = "[{$settings->get(SettingsStrings::SITE_NAME)}] Password Reset Request";
		$mail->isHTML(true);
		$mail->Body    = $tpl->render('emails::reset-password', [
			'page'  => $page,
			'token' => base64_encode("{$ut->userId}:{$ut->token}")
		]);
		$mail->addAddress($email);

		if (!$mail->send()) {
			return false;
		}

		return true;
	}
