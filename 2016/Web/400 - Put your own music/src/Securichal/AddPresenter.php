<?php
namespace Securichal;

class AddPresenter {
    public $error;
    public $success;

    public function __construct($error, $success) {
        $this->error = $error;
        $this->success = $success;
    }
}
