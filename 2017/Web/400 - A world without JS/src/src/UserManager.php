<?php
declare(strict_types=1);
namespace GreHack;

use \ParagonIE\ConstantTime\Encoding;

class UserManager
{
    private $db;

    public function __construct()
    {
        $this->db = DB::instance();
    }

    public function login(string $username, string $password): bool
    {
        $rows = $this->db->run('SELECT * FROM user WHERE username = ?', $username);
        if (empty($rows)) {
            return false;
        }
        if (! password_verify($password, $rows[0]['password'])) {
            return false;
        }
        $this->loginAs($username);
        return true;
    }

    public function register(string $username, string $password): bool
    {
        try {
            $this->db->insert('user', [
                'username' => $username,
                'password' => password_hash($password, PASSWORD_DEFAULT)
            ]);
        } catch(\PDOException $ex) {
            return false;
        }
        return true;
    }

    public function loginAs(string $username)
    {
        $rows = $this->db->run('SELECT * FROM user WHERE username = ?', $username);
        if (empty($rows)) {
            return;
        }
        $token = Encoding::hexEncode(random_bytes(32));
        $this->db->insert('session', [
            'user_id' => $rows[0]['id'],
            'token' => hash('sha256', $token)
        ]);
        $session_id = $this->db->lastInsertId();
        setcookie('araas_session', $session_id . '.' . $token);
        session_regenerate_id(true);
    }

    public function logout()
    {
        $session_pieces = explode('.', $_COOKIE['araas_session']);
        if (count($session_pieces) === 2) {
            $this->db->delete('session', [
                'id' => $session_pieces[0]
            ]);
        }
        setcookie('araas_session', '', time() - 3600);
        header('Clear-Site-Data: "cache", "cookies", "storage", "executionContexts"');
        session_regenerate_id(true);
    }

    public function getCurrentUser(): User
    {
        // Hardcode admin
        if (isset($_COOKIE['araas_session_admin']) && hash_equals($_COOKIE['araas_session_admin'], '9bBnEQ%k#Yb^2&c@M96f5AQrf2Ae7C')) {
            return new User(true, true, false);
        }
        if (! isset($_COOKIE['araas_session'])) {
            return new User(false, false, false);
        }
        $session_pieces = explode('.', $_COOKIE['araas_session']);
        if (count($session_pieces) !== 2) {
            return new User(false, false, false);
        }
        $rows = $this->db->run('SELECT * FROM session WHERE id = ?', $session_pieces[0]);
        if (empty($rows)) {
            return new User(false, false, false);
        }
        if (! hash_equals($rows[0]['token'], hash('sha256', $session_pieces[1]))) {
            return new User(false, false, false);
        }

        $rows_user = $this->db->run('SELECT * FROM user WHERE id = ?', $rows[0]['user_id']);
        if (empty($rows_user)) {
            return new User(true, false, false);
        }
        return new User(true, (bool) $rows_user[0]['admin'], (bool) $rows_user[0]['invited'], (int) $rows_user[0]['id']);
    }
}
