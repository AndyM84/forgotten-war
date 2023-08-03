<?php

	/**
	 * Credit to https://github.com/andrefelipe/vite-php-setup for the source material to make this work.
	 */

	namespace Zibings;

	use AndyM84\Config\ConfigContainer;

	use Stoic\Utilities\FileHelper;
	use Stoic\Utilities\StringHelper;
	use Stoic\Web\PageHelper;

	function viteAssetUrl(string $fileName, PageHelper $ph) : string {
		$manifest = viteGetManifest();

		return array_key_exists($fileName, $manifest) ? $ph->getAssetPath("~/web/assets/js/vue/" . $manifest[$fileName]['file']) : '';
	}

	function viteCssTag(string $fileName, PageHelper $ph, ConfigContainer $settings) : string {
		if (viteIsDev($fileName, $settings)) {
			return '';
		}

		$tags = '';

		foreach (viteCssUrls($fileName, $ph) as $url) {
			$tags .= "<link rel=\"stylesheet\" href=\"{$url}\">";
		}

		return $tags;
	}

	function viteCssUrls(string $fileName, PageHelper $ph) : array {
		$urls     = [];
		$manifest = viteGetManifest();

		if (!empty($manifest[$fileName]['css'])) {
			foreach ($manifest[$fileName]['css'] as $css) {
				$urls[] = $ph->getAssetPath('~/web/assets/js/vue/' . $css);
			}
		}

		return $urls;
	}

	function viteGetManifest() : array {
		$fh      = new FileHelper(STOIC_CORE_PATH);
		$content = $fh->getContents('~/web/assets/js/vue/manifest.json');

		return json_decode($content, true);
	}

	function viteImportsUrls(string $fileName, PageHelper $ph) : array {
		$urls     = [];
		$manifest = viteGetManifest();

		if (!empty($manifest[$fileName]['imports'])) {
			foreach ($manifest[$fileName]['imports'] as $imports) {
				$urls[] = $ph->getAssetPath('~/web/assets/js/vue/' . $manifest[$imports]['file']);
			}
		}

		return $urls;
	}

	function viteInitApp(string $fileName, PageHelper $ph, ?ConfigContainer $settings = null) : string {
		if ($settings === null) {
			global $Settings;

			$settings = $Settings;
		}

		$sh = new StringHelper();

		$sh->append("\n");
		$sh->append(viteJsTag($fileName, $ph, $settings));
		$sh->append("\n");
		$sh->append(vitePreloadImports($fileName, $ph, $settings));
		$sh->append("\n");
		$sh->append(viteCssTag($fileName, $ph, $settings));

		return $sh->data();
	}

	function viteIsDev(string $fileName, ConfigContainer $settings) : bool {
		static $devCheck = null;

		if ($devCheck !== null) {
			return $devCheck;
		}

		if ($settings->get(SettingsStrings::VITE_IS_PROD, false)) {
			$devCheck = false;

			return false;
		}

		$fh     = new FileHelper(STOIC_CORE_PATH);
		$url    = $fh->pathJoin($settings->get(SettingsStrings::VITE_DEV_HOST, 'http://localhost:3000/'), $fileName);
		$handle = curl_init($url);

		curl_setopt_array($handle, [
			CURLOPT_RETURNTRANSFER => true,
			CURLOPT_NOBODY         => true
		]);

		curl_exec($handle);
		$error = curl_errno($handle);
		curl_close($handle);

		return $devCheck = !$error;
	}

	function viteJsTag(string $fileName, PageHelper $ph, ConfigContainer $settings) : string {
		$url = viteIsDev($fileName, $settings) ? $settings->get(SettingsStrings::VITE_DEV_HOST, 'http://localhost:3000/') . $fileName : viteAssetUrl($fileName, $ph);

		if (!$url) {
			return '';
		}

		return "<script type=\"module\" crossorigin src=\"{$url}\"><" . "/script>";
	}

	function vitePreloadImports(string $fileName, PageHelper $ph, ConfigContainer $settings) : string {
		if (viteIsDev($fileName, $settings)) {
			return '';
		}

		$res = '';

		foreach (viteImportsUrls($fileName, $ph) as $url) {
			$res .= "<link rel=\"modulepreload\" href=\"{$url}\">";
		}

		return $res;
	}
