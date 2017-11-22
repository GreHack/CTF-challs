<?php
declare(strict_types=1);
namespace GreHack;

class PasteRepository
{
    private $db;

    public function __construct()
    {
        $this->db = DB::instance();
    }

    public function create(string $id, string $content): void
    {
        $this->db->run("INSERT INTO paste VALUES('$id', '$content')");
    }

    public function getContent(string $id): string
    {
        return $this->db->cell("SELECT content FROM paste WHERE id = '$id'");
    }
}