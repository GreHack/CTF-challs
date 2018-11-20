<?php

namespace App;

class GitReference
{
    private $name;
    private $reference;

    public function __construct(string $name, string $reference)
    {
        $this->name = $name;
        $this->reference = $reference;
    }

    public function getName(): string
    {
        return $this->name;
    }

    public function getReference(): string
    {
        return $this->reference;
    }
}
