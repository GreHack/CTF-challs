<?php

require __DIR__ . '/../vendor/autoload.php';
header("Content-Security-Policy: default-src 'none'; style-src 'self'; form-action 'self'; child-src 'none'; frame-ancestors 'none'; disown-opener; reflected-xss block;");
header("Referrer-Policy: no-referrer");
header("X-Frame-Options: DENY");
header("X-Content-Type-Options: nosniff");
header("X-XSS-Protection: 1; mode=block");
