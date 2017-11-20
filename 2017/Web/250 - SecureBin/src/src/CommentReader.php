<?php
declare(strict_types=1);
namespace GreHack;

use ParagonIE\Halite\Symmetric\Crypto;
use ParagonIE\Halite\Symmetric\EncryptionKey;

class CommentReader
{
    /**
     * @var EncryptionKey
     */
    private $key;

    public function __construct(EncryptionKey $key)
    {
        $this->key = $key;
    }

    public function read(string $comment): string
    {
        if ($comment === '') {
            return '';
        }
        return Crypto::decrypt($comment, $this->key)->getString();
    }
}