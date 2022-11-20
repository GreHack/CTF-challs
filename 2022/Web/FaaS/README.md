# GH22 FaaS
Category: Web  
Points: 150  
Authors: Le_suisse  
Description:
> We just rolled out our FaaS service. We expect to start a new era in microservices architecture with this work.

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{broken_💩_restrictions}</p>
</details>

## How to run it?
You need to have Docker and Docker Compose installed.
You can run: ``docker compose up`` inside this directory, the challenge will
be available at http://127.0.0.1:3150/

## Expected resolution steps
<details>
    <summary>Click to discover/hide</summary>

    The endpoint `POST /compute_file` is a bit too permissive and allow to access more files than absolutely required.

    See the [possible_solution folder](./possible_solution) for code snippet showing how it was possible to find
    the flag and to read it.
</details>
