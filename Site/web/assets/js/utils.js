class Utilities {
	ajaxHeaders() {
		return {
			"Authorization": 'Bearer ' + this.authToken
		};
	}

	constructor(apiBaseUrl, authToken) {
		this.apiBaseUrl = (apiBaseUrl.endsWith("/")) ? apiBaseUrl : apiBaseUrl + "/";
		this.authToken  = authToken;

		return;
	}

	makeApiUrl(path) {
		if (path.startsWith("/")) {
			path = path.substring(1);
		}

		if (location.port != 80 && location.port != 443) {
			path = "index.php?url=" + path;
		}

		return this.apiBaseUrl + path;
	}
}