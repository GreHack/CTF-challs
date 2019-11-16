export enum Category {
    Benefactor,
    ConfWorkshop,
    ConfWorkshopCTF
}

export function ticketPrice(c: Category): number {
    switch (c) {
        case Category.ConfWorkshop:
            return 69;
        case Category.ConfWorkshopCTF:
            return 70;
        default:
            return 300;
    }
}
