<template>
  <div id="app">
    <TicketForm />
  </div>
</template>

<script lang="ts">
import { Component, Vue } from "vue-property-decorator";
import TicketForm from "./components/TicketForm.vue";

@Component({
  components: {
    TicketForm
  }
})
export default class App extends Vue {}
</script>
