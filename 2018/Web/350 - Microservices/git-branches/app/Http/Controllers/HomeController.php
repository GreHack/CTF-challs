<?php

namespace App\Http\Controllers;

use App\GitReference;
use GuzzleHttp\Client;
use GuzzleHttp\Exception\RequestException;
use Illuminate\Http\Request;

class HomeController extends \Laravel\Lumen\Routing\Controller
{
    public function home(Request $request)
    {
        if ($request->get('url', '') === '') {
            return view('home', ['url' => $request->get('url')]);
        }

        $client = new Client();
        try {
            $response_git_content = $client->get('http://microservices-content-retriever:8080/?url=' . urlencode($request->get('url', '')));
        } catch (RequestException $ex) {
            return view('home',
                [
                    'url' => $request->get('url'),
                    'processing_failure' => true
                ]
            );
        }
        $git_content = json_decode($response_git_content->getBody());
        if ($git_content->status_code !== 200) {
            return view('home',
                [
                    'url' => $request->get('url'),
                    'invalid_response' => $git_content->serialized_response
                ]
            );
        }

        try {
            $response_git_references = $client->request('GET', 'http://microservices-content-parser:8082/', [
                'body' => $git_content->body
            ]);
        } catch (RequestException $ex) {
            return view('home',
                [
                    'url' => $request->get('url'),
                    'processing_failure' => true
                ]
            );
        }

        $decoded_references = json_decode($response_git_references->getBody(), true);
        $references = [];

        foreach ($decoded_references as $name => $reference) {
            $references[] = new GitReference($name, $reference);
        }

        return view('home',
            [
                'url' => $request->get('url'),
                'git_references' => collect($references)
            ]
        );
    }
}
