<!doctype html>
<html>
<head>
    <meta charset = "utf-8">
    <link rel="stylesheet" href="/mini-dark.min.css">
    <title>Git Branches</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
<main>
    <h1>Git References</h1>
    <hr>
    <form>
        <label for="url">Repository URL</label>
        <input type="url" pattern="https?://.+" id="url" name="url" value="{{ $url }}" placeholder="https://…" required>
        <input class="primary" type="submit" value="Get all references">
    </form>

    @if (isset ($git_references) && count($git_references) > 0)
        <table>
            <thead>
            <tr>
                <th>Name</th>
                <th>Reference</th>
            </tr>
            </thead>
            <tbody>
            @foreach ($git_references as $git_reference)
                <tr>
                    <td data-label="Name">{{ $git_reference->getName() }}</td>
                    <td data-label="Reference">{{ $git_reference->getReference() }}</td>
                </tr>
            @endforeach
            </tbody>
        </table>
    @endif
    @if (isset ($invalid_response) && ! empty($invalid_response))
        <h4><span class="icon-alert"></span> Invalid content received, is this a Git repository?</h4>
        <pre>{{  $invalid_response }}</pre>
    @endif
    @if (isset ($processing_failure))
        <h4><span class="icon-alert"></span> We have not been able to process your request, please try again later</h4>
    @endif
</main>
</body>
</html>