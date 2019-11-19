# LPN - Active
Category: Crypto  
Points: 100  
Author: Nics  
Description:
> You found a tag in the parking lot. It grants access to a better parking spot than yours. It's hard to directly read the secret from its memory but you know the protocol used (`tag.py`).
> Why don't you try an active attack to retrieve the secret? You can interact with the tag using `nc TODO TODO`

Available during the CTF: [download.tar.gz](./download.tar.gz)

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH19{Trivial_active_attacks_:'(}</p>
</details>

## How to run it?
You need to have Docker installed.

```sh
docker build -t crypto:lpn-active .
docker run -d -p TODO:4361 crypto:lpn-active
```
