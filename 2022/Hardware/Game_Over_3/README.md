# Game Over 3
Category: Web  
Points: 400  
Authors: Phil & Le_suisse  
Description:
> 4 identical devices are located beside the staff's desk.  
> It's a fully functional device, press A to switch it ON, and B to cut it OFF.  
> Hack-back!
>  
> Hint: the malware is fully functional on Windows 10, run it at your own risk!  
> Hint: you don't need the device for this last part, only the firmware.  

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{ThisWasTheLastStepCongratz}</p>
</details>

## How to run it?
You need to have Docker and Docker Compose installed.
You can run: ``docker compose up`` inside this directory, the challenge will
be available at http://127.0.0.1/

Note: during the event the service was located at `10.0.202.1`.

## Expected resolution steps
<details>
    <summary>Click to discover/hide</summary>

    Reversing the firmware is mandatory.

    The dropper is typed in base64, converted and run with an id from 1 to 255.

    The dropper MD5_sum the ID and use it to communicate to server.

    The server send the payload, a Powershell stealing the WIFI credentials of the guest.

    The credential are sent back to the server.

    The trick is to understand only 255 MD5_sum are valid, and to use action=read gives back the collected datas.

    Brute-forcing the 255 MD5_sum are mandatory to read all the already stored datas, and obtaining the flag.
</details>
