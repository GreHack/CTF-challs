<?php
declare(strict_types=1);
namespace GreHack;

class TemplateEngineFactory
{
    public function getEngine(): \Mustache_Engine
    {
        return new \Mustache_Engine(
            array(
                'cache' => '/tmp/mustache',
                'loader' => new \Mustache_Loader_FilesystemLoader(__DIR__ . '/../templates'),
                'strict_callables' => true,
            )
        );
    }
}
