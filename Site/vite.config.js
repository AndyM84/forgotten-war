// View your website at your own local server
// for example http://vite-php-setup.test

// http://localhost:3000 is serving Vite on development
// but accessing it directly will be empty
// TIP: consider changing the port for each project, see below

// IMPORTANT image urls in CSS works fine
// BUT you need to create a symlink on dev server to map this folder during dev:
// ln -s {path_to_project_source}/src/assets {path_to_public_html}/assets
// on production everything will work just fine

import { defineConfig, splitVendorChunkPlugin } from 'vite'
import vue from '@vitejs/plugin-vue'
import liveReload from 'vite-plugin-live-reload'
import path from 'path'

// https://vitejs.dev/config/
export default defineConfig({
	plugins: [
		vue(),
		liveReload([
			//__dirname + '/tpl/admin/**/*.tpl.php',
			//__dirname + '/tpl/vue/**/*.',
			__dirname + '/tpl/**/*.tpl.php'
		]),
		splitVendorChunkPlugin(),
	],

	// config
	root: 'tpl/vue',
	base: process.env.APP_ENV === 'development' ? '/' : 'web/assets/js/vue',

	build: {
		// output dir for production build
		outDir: path.resolve(__dirname, 'web/assets/js/vue'),
		emptyOutDir: true,

		// emit manifest so PHP can find the hashed files
		manifest: true,

		// our entry
		rollupOptions: {
			input: path.resolve(__dirname, 'tpl/vue/front.js'),
		}
	},

	server: {
		// required to load scripts from custom host
		cors: true,

		// we need a strict port to match on PHP side
		// change freely, but update on PHP to match the same port
		strictPort: true,
		port: 3000
	},

	// required for in-browser template compilation
	// https://v3.vuejs.org/guide/installation.html#with-a-bundler
	resolve: {
		alias: {
			vue: 'vue/dist/vue.esm-bundler.js'
		}
	}
})
