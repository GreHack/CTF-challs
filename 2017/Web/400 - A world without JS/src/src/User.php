<?php
declare(strict_types=1);
namespace GreHack;

class User
{
    private $is_logged_in;
    private $is_admin;
    private $is_invited;
    private $user_id;

    public function __construct(bool $is_logged_in, bool $is_admin, bool $is_invited, int $user_id = 0)
    {
        $this->is_logged_in = $is_logged_in;
        $this->is_admin = $is_admin;
        $this->is_invited = $is_invited;
        $this->user_id = $user_id;
    }

    public function isLoggedIn(): bool
    {
        return $this->is_logged_in;
    }

    public function isAdmin(): bool
    {
        return $this->is_admin;
    }

    public function isInvited(): bool
    {
        return $this->is_invited;
    }

    public function getUserId(): int
    {
        return $this->user_id;
    }
}
