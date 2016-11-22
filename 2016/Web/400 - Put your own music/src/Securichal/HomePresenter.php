<?php
namespace Securichal;

use DateTime;

class HomePresenter {
    private $playlist;

    public function __construct(array $youtube_playlist) {
        $this->playlist = $youtube_playlist;
    }

    public function data() {
        $res = array();
        foreach ($this->playlist as $video) {
            $res[] = array(
                'title' => $video['title'],
                'link' => 'https://www.youtube.com/watch?v=' . $video['id']
            );
        }
        return new \ArrayIterator($res);
    }
}
