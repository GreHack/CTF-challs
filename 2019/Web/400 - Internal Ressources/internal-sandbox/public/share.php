<?php

header('Content-Type: application/json');

$data = json_decode(file_get_contents('php://input'), true, 512, JSON_THROW_ON_ERROR);

if (! isset($data['css'], $data['html'])) {
    echo json_encode(
        [
            'url' => '',
            'admin_url' => '',
            'is_success' => false
        ],
        JSON_THROW_ON_ERROR,
        512
    );
    return;
}

$css = $data['css'];
$html = $data['html'];

$admin_url = 'http://admin.internal.test:2400/?css=' . urlencode($css);

echo json_encode(
    [
        'url' => 'http://sandbox.internal.test:2400/?css=' . urlencode($css) . '&html=' . urlencode($html),
        'admin_url' => 'http://admin.internal.test:2400/?css=' . urlencode($css),
        'is_success' => true
    ],
    JSON_THROW_ON_ERROR,
    512
);

$redis = new Redis();
$redis->connect('internal-resources-redis');
$redis->auth('EJH8Xv1jeZKRjN66F4PcQA4AE65592pF');
$redis->lpush('browse', $admin_url);
