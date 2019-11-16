import {Category} from "@/ticket";

export interface PaymentRequest {
    total: number;
    tickets: Record<number, number>
    billing: {
        first_name: string;
        last_name: string;
        email: string;
    }
}