<?php

require __DIR__ . '/../src/bootstrap.php';

if (!isset($_REQUEST['v'])) {
    header('Location: /');
    exit();
}

$key = \GreHack\KeyLoader::load();
$paste_encryption_proxy = new \GreHack\PasteEncryptionProxy(new \GreHack\PasteRepository, $key);

try {
    [$id, $c_e] = \GreHack\Extractor::extract($_REQUEST['v']);
    $content = $paste_encryption_proxy->getContent($id);
    $comment_reader = new \GreHack\CommentReader($key);
    $comment = $comment_reader->read($c_e);
} catch (Throwable $tr) {
    header('Location: /');
    exit();
}

if (isset($_POST['comment'])) {
    $link = new \GreHack\Link($id, $key);
    header('Location: ' . $link->get($_POST['comment']));
    exit();
}

$engine_factory = new \GreHack\TemplateEngineFactory;
$engine = $engine_factory->getEngine();
$template = $engine->loadTemplate('show');
echo $template->render(new class($content, new \GreHack\Link($id, $key), $comment) {
    public $content;
    public $url;
    public $comment;
    public function __construct(string $content, \GreHack\Link $link, string $comment)
    {
        $this->content = $content;
        $this->url = $link->get($comment);
        $this->comment = $comment;
    }
});