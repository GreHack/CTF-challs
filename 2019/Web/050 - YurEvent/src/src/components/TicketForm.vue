<template>
    <form-wizard title="AlpacaCon tickets" subtitle="" @on-complete="onComplete">
        <tab-content title="Tickets">
            <div>
                <div class="field">
                    <label class="label">Conference (Sold Out)</label>
                    <div class="control">
                        <div class="select">
                            <select>
                                <option disabled>0</option>
                            </select>
                        </div>
                    </div>
                </div>
                <div class="field">
                    <label class="label">Conference and Workshop (Sold Out)</label>
                    <div class="control">
                        <div class="select">
                            <select>
                                <option disabled>0</option>
                            </select>
                        </div>
                    </div>
                </div>
                <div class="field">
                    <label class="label">Benefactor</label>
                    <div class="control">
                        <div class="select">
                            <select v-model="nbBenefactors">
                                <option value="0">0</option>
                                <option value="1">1 - 300€</option>
                                <option value="2">2 - 600€</option>
                                <option value="3">3 - 900€</option>
                                <option value="4">4 - 1200€</option>
                                <option value="5">5 - 1600€</option>
                            </select>
                        </div>
                    </div>
                </div>
            </div>
        </tab-content>
        <tab-content title="Billing Info">
            <div>
                <div class="field">
                    <label class="label">First name</label>
                    <div class="control">
                        <input class="input" type="text" placeholder="First name" required v-model="firstName">
                    </div>
                </div>

                <div class="field">
                    <label class="label">Last name</label>
                    <div class="control">
                        <input class="input" type="text" placeholder="Last name" required v-model="lastName">
                    </div>
                </div>
                <div class="field">
                    <label class="label">Email</label>
                    <div class="control">
                        <input class="input" type="email" placeholder="Email" required v-model="email">
                    </div>
                </div>
            </div>
        </tab-content>
        <div v-if="successMsg !== null">
            <span class="success">{{successMsg}}</span>
        </div>
        <div v-if="errorMsg !== null">
            <span class="error">{{errorMsg}}</span>
        </div>
    </form-wizard>
</template>

<script lang="ts">
    import {Component, Vue} from "vue-property-decorator";
    import { PaymentRequest } from "@/payment";
    import {Category, ticketPrice} from "@/ticket";

    @Component
    export default class TicketForm extends Vue {
        // No need to check for conf or conf+workshop tickets, they are sold out
        nbBenefactors: string = "0";
        firstName: string = '';
        lastName: string = '';
        email: string = '';

        successMsg: string | null = null;
        errorMsg: string | null = null;

        async onComplete(): Promise<void> {
            this.successMsg = null;
            this.errorMsg = null;

            const benefactorTickets = Number.parseInt(this.nbBenefactors, 10);
            if (benefactorTickets === 0) {
                this.errorMsg = "No ticket has been selected, please try again";
                return;
            }

            const paymentRequest: PaymentRequest = {
                total: benefactorTickets * ticketPrice(Category.Benefactor),
                tickets: {
                    [Category.Benefactor]: benefactorTickets
                },
                billing: {
                    first_name: this.firstName,
                    last_name: this.lastName,
                    email: this.email
                }
            };

            try {
                const response = await fetch(
                    '/payment.php',
                    {
                        method: "POST",
                        headers: {
                            'Content-Type': 'application/json',
                        },
                        body: JSON.stringify(paymentRequest)
                    }
                );
                if (response.ok) {
                    this.successMsg = await response.text();
                } else {
                    this.errorMsg = "An error occurred, please try again or contact the support team";
                }
            } catch (e) {
                this.errorMsg = "An error occurred";
            }
        }
    }
</script>
<style scoped>
    .success {
        color: hsl(141, 71%, 48%);
    }

    .error {
        color: hsl(348, 100%, 61%);
    }
</style>
