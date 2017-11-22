<?php
declare(strict_types=1);
namespace GreHack;

class Invitation
{
    private $id;
    private $user_id;
    private $comment;
    private $status;

    public function __construct(int $id, int $user_id, string $comment, int $status)
    {
        $this->id = $id;
        $this->user_id = $user_id;
        $this->comment = $comment;
        $this->status = $status;
    }

    public function getId(): int
    {
        return $this->id;
    }

    public function getUserId(): int
    {
        return $this->user_id;
    }

    public function getComment(): string
    {
        return $this->comment;
    }

    public function getStatus(): string
    {
        switch ($this->status) {
            case 1:
                return 'Approved';
            case -1;
                return 'Rejected';
            default:
                return 'Pending';
        }
    }

    public function hasAlreadyBeenProcessed(): bool
    {
        return $this->status !== 0;
    }
}
