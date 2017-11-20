<?php
declare(strict_types=1);
namespace GreHack;

use \ParagonIE\ConstantTime\Base64;

class AntiCSRF
{
    const MAX_TOKENS = 1024;

    private $current_user;
    private $token;
    private $url;

    public function __construct(string $url)
    {
        //Hardcode admin
        $user_manager = new UserManager();
        $this->current_user = $user_manager->getCurrentUser();
        $this->url = $url;

        if ($this->url === '/admin.php' && $this->current_user->isAdmin()) {
            $this->token = 'SOtBxHRoQ1ixP8fr/OPTFogNWCHBgLghJ/C2mza/Os0=';
        } elseif (isset($_SESSION['anticsrf'][$this->url])) {
            $this->token = $_SESSION['anticsrf'][$this->url]['token'];
        }
        if (! $this->token) {
            $this->generateToken();
        }
    }

    public function isValid(): bool
    {
        if (! isset($_REQUEST[$this->getTokenName()]) || empty($_REQUEST[$this->getTokenName()])) {
            return false;
        }
        return hash_equals($this->token, $_REQUEST[$this->getTokenName()]);
    }

    public function getToken(): string
    {
        return $this->token;
    }

    public function getTokenName(): string
    {
        return 'csrf_token';
    }

    private function generateToken()
    {
        $this->token = Base64::encode(random_bytes(32));
        $_SESSION['anticsrf'][$this->url] = array(
            'token'   => $this->token,
            'created' => time()
        );
        $this->recycleTokens();
    }

    private function recycleTokens()
    {
        if (self::MAX_TOKENS > count($_SESSION['anticsrf'])) {
            return;
        }

        uasort(
            $this->storage,
            function ($token_1, $token_2) {
                return $token_1['created'] - $token_2['created'];
            }
        );

        while (count($_SESSION['anticsrf']) > self::MAX_TOKENS) {
            array_shift($_SESSION['anticsrf']);
        }
    }
}
