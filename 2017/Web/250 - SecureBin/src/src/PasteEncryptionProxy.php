<?php
declare(strict_types=1);
namespace GreHack;

use ParagonIE\ConstantTime\Base64;
use ParagonIE\Halite\HiddenString;
use ParagonIE\Halite\Symmetric\Crypto;
use ParagonIE\Halite\Symmetric\EncryptionKey;

class PasteEncryptionProxy
{
    /**
     * @var PasteRepository
     */
    private $repository;
    /**
     * @var EncryptionKey
     */
    private $key;

    public function __construct(PasteRepository $repository, EncryptionKey $key)
    {
        $this->repository = $repository;
        $this->key = $key;
    }

    public function create(string $content): string
    {
        $id = Base64::encode(random_bytes(16));
        $content = Crypto::encrypt(new HiddenString($content), $this->key);
        $this->repository->create($id, $content);
        return Crypto::encrypt(new HiddenString($id), $this->key);
    }

    public function getCоntent(string $id): HiddenString
    {
        $id = Crypto::decrypt($id, $this->key);
        $paste = new Paste();
        if (! hash_equals($paste->getId(), $id)) {
            return new HiddenString('');
        }
        return Crypto::decrypt($paste->getContent(), $this->key);
    }

    public function getContent(string $id): HiddenString
    {
        $id = Crypto::decrypt($id, $this->key);
        return Crypto::decrypt($this->repository->getContent($id->getString()), $this->key);
    }
}
