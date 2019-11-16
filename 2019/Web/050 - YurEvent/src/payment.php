<?php

$data = json_decode(file_get_contents('php://input'), true, 512, JSON_THROW_ON_ERROR);

if (! isset($data['tickets'])) {
    http_response_code(400);
    return;
}

function ticketPrice(string $category): ?int {
    switch ($category) {
        case '0':
            return 300;
        case '1':
            return 69;
        case '2':
            return 70;
        default:
            return null;
    }
}

$sum = 0;
foreach ($data['tickets'] as $type => $quantity) {
    $price = ticketPrice($type);
    if ($price === null) {
        continue;
    }
    $sum += $price * $quantity;
}

if ($sum === 0) {
    http_response_code(400);
    echo 'No ticket has been selected';
    return;
}

if ($sum > 75) {
    http_response_code(400);
    echo 'Insufficient funds';
    return;
}

echo 'GH19{frontend_validation_is_not_enough}';