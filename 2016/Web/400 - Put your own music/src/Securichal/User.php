<?php
namespace Securichal;

class User {
    const ANONYMOUS   = 'no_cookie';
    const ADMIN       = 'c3d1553ce763b64bfa97442bd761de338758e85fbfab8efc43dfe24f0db4fbd6ca649cbaf7edc81e02ec0137af22b94403929ab56f8cb231f07dd9c8272c8fc9';

    private $status;

    private function __construct($status) {
        $this->status = $status;
    }

    public function isAnonymous() {
        return $this->status === self::ANONYMOUS;
    }

    public function isAdministrator() {
        return $this->status === self::ADMIN;
    }

    public function getName() {
        switch ($this->status) {
            case self::ADMIN:
                return 'admin';
            default:
                return 'visitor';
        }
    }

    public function changeStatusTo($status) {
        switch ($status) {
            case self::ANONYMOUS:
                $this->logout();
                break;
            case self::ADMIN:
                $this->status = $status;
                setcookie('flag', $status);
                break;
            default:
                break;
        }
    }

    public function logout() {
        $this->status = self::ANONYMOUS;
        setcookie('flag', '', time()-3600);
    }

    public static function getCurrentUser() {
        if (isset($_COOKIE['flag']) && $_COOKIE['flag'] === self::ADMIN) {
            return new User($_COOKIE['flag']);
        } else {
            return new User(self::ANONYMOUS);
        }
    }
}
