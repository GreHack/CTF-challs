<?php
declare(strict_types=1);
namespace GreHack;

class InvitationManager
{
    private $db;

    public function __construct()
    {
        $this->db = DB::instance();
    }

    public function request(User $user, string $comment)
    {
        $this->db->insert('invit_request', [
            'user_id' => $user->getUserId(),
            'comment' => $comment
        ]);
        $this->notifyAdmins((int) $this->db->lastInsertId());
    }

    private function notifyAdmins(int $request_id)
    {
        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, 'http://bot:8080/?id='.urlencode((string) $request_id));
        curl_exec($curl);
        curl_close($curl);
    }

    public function getInvitations(User $user): array
    {
        $invitations = [];

        $rows = $this->db->run('SELECT * FROM invit_request WHERE user_id = ? ORDER BY id DESC', $user->getUserId());
        foreach($rows as $row) {
            $invitations[] = new Invitation((int) $row['id'], (int) $row['user_id'], $row['comment'], (int) $row['accepted']);
        }

        return $invitations;
    }

    public function getInvitation(int $request_id): Invitation
    {
        $rows = $this->db->run('SELECT * FROM invit_request WHERE id = ?', $request_id);
        if (empty($rows)) {
            throw new InvitationNotFoundException();
        }
        return new Invitation((int) $rows[0]['id'], (int) $rows[0]['user_id'], $rows[0]['comment'], (int) $rows[0]['accepted']);
    }

    public function accept(Invitation $invitation)
    {
        $this->db->beginTransaction();
        $this->db->update('invit_request', ['accepted' => 1], ['id' => $invitation->getId()]);
        $this->db->update('user', ['invited' => 1], ['id' => $invitation->getUserId()]);
        $this->db->commit();
    }

    public function reject(Invitation $invitation)
    {
        $this->db->update('invit_request', ['accepted' => -1], ['id' => $invitation->getId()]);
    }
}
