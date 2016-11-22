<?php
namespace Securichal;

class DBYoutube {
    protected static $db;

    private function __construct() {
        try {
            self::$db = new \PDO('sqlite:/db/youtube.db');
        } catch (\PDOException $e) {
            echo('DB connection error. Code: ' . $e->getCode());
            exit();
        }
    }

    public static function instance() {
        if (!isset(self::$db)) {
            new DBYoutube();
        }
        return self::$db;
    }
}
