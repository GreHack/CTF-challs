# Icons Bank
Category: Web  
Points: 150  
Authors: Feelzor  
Description:
> You've seen this popular website that allows anyone to upload their own icon for everyone to use.
>  
>  Participate in the hype and become well-known in the icon industry.

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{c0mpression_does_not_prot3ct_from_png_execut1on}</p>
</details>

## How to run it?
You need to have Docker and Docker Compose installed.
You can run: ``docker compose up`` inside this directory, the challenge will
be available at http://127.0.0.1:8080/
Directory /public/uploads may require 777 permissions.

## Expected resolution steps
<details>
    <summary>Click to discover/hide</summary>

    Follow the method 4 on https://www.synacktiv.com/publications/persistent-php-payloads-in-pngs-how-to-inject-php-code-in-an-image-and-keep-it-there.html with IDAT Chunks to create your png file with the proper size on.
    
    Upload it.
    
    Use the lfi `http://localhost:8080/?page=uploads/yourupload.png?0=shell_exec` and POST arg 1=ls to execute a shell command.
    
    (with Header `Content-Type` `application/x-www-form-urlencoded`)
    
    Enjoy (`cat ../.flag` to retrieve the flag).
</details>
