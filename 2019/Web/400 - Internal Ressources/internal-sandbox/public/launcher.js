document.addEventListener("DOMContentLoaded", function() {
    const url = new URL(window.location);
    const val = {
        'html': url.searchParams.get('html'),
        'css': url.searchParams.get('css')
    };
    const jotted = new Jotted(document.querySelector('#editor'), {
        files: [
            {type: 'html', content: val.html ? val.html : 'Internal sandbox'},
            {type: 'css', content: val.css ? val.css : ''}
        ]
    });

    document.getElementById('share-me').addEventListener('click', function() {
        let content = {};
        const editors = jotted.$container.querySelectorAll('.jotted-editor');
        for (let i = 0; i < editors.length; i++) {
            const textarea = editors[i].querySelector('textarea');
            content[textarea.getAttribute('data-jotted-type')] = textarea.value;
        }

        fetch('/share.php', {
              method: 'post',
              body: JSON.stringify(content)
        }).then(function(response) {
            return response.json();
        }).then(function(data) {
            if (data.is_success) {
                document.getElementById('share-url').innerHTML = 'URL generated: <pre>' + data.url + '</pre><br>URL for admin live preview: <pre>'  + data.admin_url + '</pre>';
            } else {
                document.getElementById('share-url').innerHTML = '<span class="icon-alert"></span> Can not generate URL to share. You might have reached the size limit.'
            }
        });
    });
});