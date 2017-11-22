<?php
declare(strict_types=1);
namespace GreHack;

use ParagonIE\Halite\KeyFactory;
use ParagonIE\Halite\Symmetric\EncryptionKey;

class KeyLoader
{
    public static function load(): EncryptionKey
    {
        return KeyFactory::loadEncryptionKey('/tmp/encryption.key');
    }
}
