<template>
<!--	<img alt="Vue logo" src="../assets/logo.png" height="40" />-->

	<h1>Vue {{ msg }}</h1>

	<button type="button" @click="state.count++">
		count is: {{ state.count }}
	</button>
</template>

<script setup>
import { reactive } from 'vue'

defineProps({
	msg: String
})

const state = reactive({ count: 0 })
</script>

<style scoped>
a {
	color: #42b983;
}
</style>
