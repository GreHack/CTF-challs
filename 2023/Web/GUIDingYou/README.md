# Guiding You

## Information to set up the chall on CTFd

_Category_: Web

_Points_: 200 (medium)

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH{Using_GU1D_v1_is_not_th3_best!}</p>
</details>

## Description to put on CTFd

A friend told me to ask for an audit before creating a front-end for my API.

Please audit it, I'm sure it is very secure but who knows...

I've created the first message, to make sure there are no bugs with my database being empty.

Author: `Feelzor`

## How to access

Via a link, the chall is set up on port 8080 in docker-compose.yml

## Configuration

Everything is set inside the `.env` and `docker-compose.yml` files.
The database is postgres

## WU

<details>
    <summary>Click to discover/hide</summary>

    Go to `/` to list all available endpoints.
    From `/messages` it is possible to list all existing messages. Number 1 is important since it is the admin message. A bit guessy, but note its date.

    Send a POST on `/messages`, eg with `Content-Type: application/json` and body `{"message":"oink"}. Retrieve the GUID and notice it is a v1 GUID, so based on time.

    You can calculate the admin message's GUID manually, but I've been using guidtool to generate multiple candidate GUIDs. The shown time may differ from the GUID generation time (a few ms).
    Using a fuzz on `/messages/1?uuid=FUZZ` with generated GUIDs (and removing all reponses with length 2) you can read the message and retrieve the flag. GJ!

</details>
