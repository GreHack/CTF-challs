<?php
require __DIR__ . '/vendor/autoload.php';

use Securichal\TemplateEngine;
use Securichal\HomePresenter;
use Securichal\DBYoutube;

date_default_timezone_set('Europe/Paris');

$mustache = TemplateEngine::getMustache();

$db = DBYoutube::instance();
$stmt = $db->query('SELECT id, title FROM video ORDER BY rowid DESC');
$playlist = $stmt->fetchAll();

$home_presenter = new HomePresenter($playlist);
echo($mustache->render('home', $home_presenter));
