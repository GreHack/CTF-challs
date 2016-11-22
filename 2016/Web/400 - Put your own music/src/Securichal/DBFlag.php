<?php
namespace Securichal;

class DBFlag {
    protected static $db;

    private function __construct() {
        try {
            self::$db = new \PDO('sqlite:/db/flag.db');
        } catch (\PDOException $e) {
            echo('DB connection error. Code: ' . $e->getCode());
            exit();
        }
    }

    public static function instance() {
        if (!isset(self::$db)) {
            new DBFlag();
        }
        return self::$db;
    }
}
