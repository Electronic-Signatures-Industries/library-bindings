import resolve from '@rollup/plugin-node-resolve';
import commonjs from '@rollup/plugin-commonjs';
// import pkg from './package.json';
import json from '@rollup/plugin-json';

import rust from "@wasm-tool/rollup-plugin-rust";
export default [
	{
		input: { 
			pem: 'Cargo.toml'
		},		
		// output: {
		// 	name: 'howLongUntilLunch',
		// 	file: pkg.browser,
		// 	format: 'umd'
		// },
		plugins: [
			rust(),
			json(),
			resolve(), // so Rollup can find `ms`
			commonjs() // so Rollup can convert `ms` to an ES module
		]
	},
];
