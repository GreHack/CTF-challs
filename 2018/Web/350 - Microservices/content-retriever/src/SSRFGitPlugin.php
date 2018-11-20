<?php

namespace GreHack2018\ContentRetriever;

use fin1te\SafeCurl\Exception\InvalidURLException;
use fin1te\SafeCurl\Options;
use fin1te\SafeCurl\Url;
use Http\Client\Common\Plugin;
use Http\Client\Exception\RequestException;
use Psr\Http\Message\RequestInterface;

class SSRFGitPlugin implements Plugin
{
    /**
     * @var Options
     */
    private $options;

    public function __construct()
    {
        $this->options = new Options();
        $this->options->enableSendCredentials(); // Needed for private repositories
    }

    public function handleRequest(RequestInterface $request, callable $next, callable $first)
    {
        if ($request->getUri()->getScheme() !== 'https') {
            $this->options->enablePinDns();
        }

        $uri = $request->getUri();
        try {
            $validated_data = Url::validateUrl((string) $uri, $this->options);
        } catch (InvalidURLException $e) {
            throw new RequestException($e->getMessage(), $request, $e);
        }

        if ($uri->getScheme() !== 'https') {
            $uri = $uri->withHost($validated_data['parts']['ips'][0]);
        }

        return $next($request->withUri($uri));
    }
}
