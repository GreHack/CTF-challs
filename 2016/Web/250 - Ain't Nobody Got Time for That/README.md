# Ain't Nobody Got Time for That
Category: Web  
Points: 250  
Author: Le_suisse  
Description:
> Random images, what a concept! I'm using a framework to develop this new amazing website but I'm not used to it yet. > I hope nobody can access the management area.

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH16{nobody_cares_about_race_conditions}</p>
</details>

## How to run it?
You need to have Docker and Docker Compose installed.
Run ``composer install`` inside the image_randomizer directory
(yeah, I'm too lazy to do that when I build the container) and then
``docker-compose up`` inside this directory, the challenge will
be available at http://localhost:2250/

``image_randomizer.zip`` file was the file given with the challenge during the CTF.
