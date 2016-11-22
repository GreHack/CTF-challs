<?php
namespace Securichal;

class TemplateEngine {
    public static function getMustache() {
        return new \Mustache_Engine(
                        array(
                            'loader' => new \Mustache_Loader_FilesystemLoader(dirname(__FILE__).'/views')
                        )
                    );
    }
}
