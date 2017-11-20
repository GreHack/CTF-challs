<?php
declare(strict_types=1);
namespace GreHack;

use ParagonIE\Halite\HiddenString;
use ParagonIE\Halite\Symmetric\Crypto;
use ParagonIE\Halite\Symmetric\EncryptionKey;

class Link
{
    /**
     * @var string
     */
    private $id;
    /**
     * @var EncryptionKey
     */
    private $key;

    public function __construct(string $id, EncryptionKey $key)
    {
        $this->id = $id;
        $this->key = $key;
    }

    public function get(string $comment = ''): string
    {
        $v = $this->id;
        if ($comment !== '') {
            $v .= '|' . Crypto::encrypt(new HiddenString($comment), $this->key);
        }

        return '/show.php?v=' . urlencode($v);
    }
}
