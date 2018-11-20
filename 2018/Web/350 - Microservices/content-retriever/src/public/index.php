<?php

use GreHack2018\ContentRetriever\SSRFGitPlugin;
use function GuzzleHttp\Psr7\str;
use Http\Client\Common\PluginClient;
use Http\Discovery\HttpClientDiscovery;
use Http\Discovery\MessageFactoryDiscovery;

require_once __DIR__ . '/../../vendor/autoload.php';

const INFO_REFS = '/info/refs?service=git-upload-pack';

if (! isset($_GET['url'])) {
    http_response_code(400);
    return;
}

$url = $_GET['url'];

$client = new PluginClient(
    HttpClientDiscovery::find(),
    [
        new SSRFGitPlugin
    ]
);

$message_factory = MessageFactoryDiscovery::find();
try {
    $response = $client->sendRequest(
        $message_factory->createRequest('GET', $url . INFO_REFS)
    );
} catch (\Http\Client\Exception $e) {
    http_response_code(400);
    return;
}

function addViaHeader($response) {
    $interfaces = net_get_interfaces();
    foreach ($interfaces as $name => $interface) {
        if (! isset($interface['unicast'][1]['address'])) {
            continue;
        }
        $address = $interface['unicast'][1]['address'];
        if ($address !== '127.0.0.1' && $address !== '::1') {
            return $response->withAddedHeader('Via', $response->getProtocolVersion() . ' ' . $address);
        }
    }
    return $response;
}

$response = addViaHeader($response);
header('Content-Type: application/json;charset=utf-8');
echo json_encode([
    'status_code' => $response->getStatusCode(),
    'body' => (string) $response->getBody(),
    'serialized_response' => str($response)
]);