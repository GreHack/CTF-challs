<?php
declare(strict_types=1);
namespace GreHack;

class Extractor
{
    public static function extract(string $v): array
    {
        $extracted_val = explode('|', $v,2);
        if (count($extracted_val) === 1) {
            $extracted_val[] = '';
        }
        return $extracted_val;
    }
}