# SSH Challenge
Category: Misc  
Points: 150  
Author: Marion  
Description:
> Pwn the session. FLAG HAS NOT GH19{} FORMAT

Available during the CTF: [b7bac819bddcd2d26dd8918290a1bc5334c04ce2.tar.gz](./b7bac819bddcd2d26dd8918290a1bc5334c04ce2.tar.gz)

## Hints / Write Up
Give me the user password
<details>
    <summary>Hint 1 (click to discover/hide)</summary>
    <p>It is not a cryptographic challenge</p>
</details>
<details>
    <summary>Hint 2(click to discover/hide)</summary>
    <p>It is a side channel attack</p>
</details>
<details>
    <summary>Hint 3(click to discover/hide)</summary>
    <p>It is a timing side channel attack</p>
</details>
<details>
    <summary>Solution(click to discover/hide)</summary>
    <p>You have to find the history in the pcap and guess which packet send
each letter. Then you have to find the time intervals between two chars.
At the end of history file you have the 
```
sudo su
``` 
command so next chars belong to
the password</p>
</details>

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>felicitation</p>
</details>

