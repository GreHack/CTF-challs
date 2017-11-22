<?php

require __DIR__ . '/../src/bootstrap.php';

$engine_factory = new \GreHack\TemplateEngineFactory;
$engine = $engine_factory->getEngine();
$template = $engine->loadTemplate('index');
echo $template->render();
