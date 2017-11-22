<?php
declare(strict_types=1);
namespace GreHack;

class DB
{
    protected static $db;

    private function __construct()
    {
        try {
            self::$db = \ParagonIE\EasyDB\Factory::create('sqlite:/tmp/db.sqlite');
        } catch (\PDOException $e) {
            echo('DB connection error. Code: ' . $e->getCode());
            exit();
        }
    }

    public static function instance(): \ParagonIE\EasyDB\EasyDB
    {
        if (! isset(self::$db)) {
            new DB();
        }
        return self::$db;
    }
}
