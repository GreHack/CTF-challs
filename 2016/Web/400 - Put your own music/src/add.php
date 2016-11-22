<?php
require __DIR__ . '/vendor/autoload.php';

use Securichal\TemplateEngine;
use Securichal\AddPresenter;
use Securichal\DBFlag;
use Securichal\DBYoutube;
use Alaouy\Youtube\Youtube;

const API_KEY = 'YOUTUBE_API_KEY';
const MASK = 24;
const MAX_DURATION = 600;

function filter_input_sql($input) {
    $res = strtolower($input);
    // Remove some comments
    $res = str_replace(array('--', '/*'), '', $res);
    // Filter whitespaces, that should be enough to stop bad guy
    // %09 missing
    $res = str_replace(array(' ', "\n", "\r", "\x0c"), '', $res);
    return $res;
}
date_default_timezone_set('Europe/Paris');
$is_video_added = false;
$error_message = '';

if (isset($_POST['id']) && isset($_POST['flag'])) {
    $db_flag = DBFlag::instance();

    $stmt = $db_flag->query('SELECT flag FROM flag WHERE flag = "' . filter_input_sql($_POST['flag']) . '"');
    $res = $stmt->fetchAll();

    // Artificially forces full recovery of the flag
    if ($_POST['flag'] !== 'GH16{admin_youtube_player}') {
        $error_message = 'Your validation code is invalid';
    } else {
        $db_youtube = DBYoutube::instance();

        $ip_client = ip2long($_SERVER['REMOTE_ADDR']);
        $low_ip = $ip_client & ~((1 << (32 - MASK)) - 1);
        $high_ip = $ip_client | ((1 << (32 - MASK)) - 1);

        $stmt_check = $db_youtube->prepare('SELECT ip FROM video WHERE ip BETWEEN ? AND ?');
        $stmt_check->execute(array($low_ip, $high_ip));
        $nb_hits = $stmt_check->fetchAll();

        if (count($nb_hits) > 0) {
            $error_message = 'You are only allowed to add one video';
        } else {
            $youtube_api = new Youtube(API_KEY);
            $video = $youtube_api->getVideoInfo($_POST['id']);
            if ($video) {
                $d1 = new DateTime();
                $d2 = new DateTime();
                $duration_inter = new DateInterval($video->contentDetails->duration);
                $d2->add($duration_inter);
                $duration = $d2->getTimestamp() - $d1->getTimestamp();
                if ($duration > MAX_DURATION) {
                    $error_message = 'Video too long. ' . MAX_DURATION . ' seconds maximum';
                } else {
                    //Add video
                    $stmt_add = $db_youtube->prepare('INSERT INTO video VALUES(?, ?, ?)');
                    $stmt_add->bindParam(1, $_POST['id']);
                    $stmt_add->bindParam(2, $ip_client);
                    $stmt_add->bindParam(3, $video->snippet->title);
                    $stmt_add->execute();
                    $is_video_added = true;
                }
            } else {
                $error_message = 'Video not found';
            }
        }
    }
}

$mustache  = TemplateEngine::getMustache();

$success_img = '';
if ($is_video_added) {
    $success_img = '<div>
        Success! Have this nice picture of a baby alpaca as a reward!<br>
        <img src="img/baby_alpaca.jpg" width="450px">
    </div>';
}

$add_presenter = new AddPresenter($error_message, $success_img);
echo($mustache->render('add', $add_presenter));
