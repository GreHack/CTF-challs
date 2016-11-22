@extends('layouts.app')

@section('content')
<div class="container">
    <div class="row">
        <div class="col-md-8 col-md-offset-2">
            <div class="panel panel-default">
                <div class="panel-heading">Dashboard</div>

                <div class="panel-body">
                    <p>
                    Number of images seen: {{ Auth::user()->nb_images }}
                    </p>
                    <p>
                        New features are coming soon, stay tuned!
                    </p>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-md-8 col-md-offset-5">
            <a href="{{ url('/home') }}" class="btn btn-primary">
                Get a new picture!
            </a>
        </div>
    </div>
    <img class="random-img" src="/img/{{ $img_id }}.jpg">
</div>
@endsection
