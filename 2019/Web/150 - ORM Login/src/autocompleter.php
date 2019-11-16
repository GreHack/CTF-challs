<?php

// TODO: finish implementation of the autocompleter on the login page

use Propel\Runtime\ActiveQuery\Criteria;

require_once __DIR__ . '/vendor/autoload.php';
require_once __DIR__ . '/generated-conf/config.php';

$value = $_GET['value'] ?? '';
$max  = $_GET['max'] ?? 10;
$offset = $_GET['offset'] ?? 0;

$q = \GrehackORMLogin\UserQuery::create();
$c = $q->filterByUsername($value . '%', Criteria::LIKE)->setLimit($max)->setOffset($offset);

$valid_usernames = [];
foreach ($c as $user) {
    $valid_usernames[] = $user->getUsername();
}

header('Content-Type: application/json');
echo json_encode($valid_usernames, JSON_THROW_ON_ERROR, 512);
