# Painting Text

Category: Misc  
Points: 300  
Authors: Feelzor  
Description:
> You've always dreamed to create a wonderful `motd` for your server, and finally found a bot that creates them for you.
> What are you waiting for?

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{D1sc0rd_cmd_inj3ct1on_w1th_filt3rs}</p>
</details>

## How to run it?
You need to have Docker and Docker Compose installed.
Set a valid Discord bot token in `.env`
You can run: ``docker compose up`` inside this directory, the challenge will
be available on discord

## Expected resolution steps
<details>
    <summary>Click to discover/hide</summary>

    - Whitespaces are transformed to a no-break space (big.flf has been modified so that the no-break space shows as a simple space)
    - cat is not executable
    - less, more, tail, head, nl strings and flag are blacklisted, as well as ';', '&', '|', '<', '>', '$', '(', ')'
    - By default, the user string is surrounded by ' and can be escaped with another '
    - However, a \ is appended before every ' the user inputs, but the user can enter \' and it will result as \\' (the backslash being escaped)

    The solve is to send the command !beautify "\'`{ta\il,.fl\ag.txt}`\'" in dm to the bot (other payloads are possible)

</details>
