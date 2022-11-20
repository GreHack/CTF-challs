# Shitposting (with a bit of trolling)

Category: Misc  
Points: 100  
Author: Le_suisse  
Description/URL:  
> https://twitter.com/el_suisse/status/1589352912857501696

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{not_all_chall_were_written_in_the_last_48_hours}</p>
</details>

## Expected resolution steps
<details>
    <summary>Click to discover/hide</summary>

    The tweet contains an image with embedded data.  
    Data are at the end of IDAT chunk which [Twitter does not strip for some reasons (?!)](https://github.com/DavidBuchanan314/tweetable-polyglot-png).  
    Two different things are added into the PNG:
    * a PDF with the flag
    * a [ZIP file which is a quine](https://research.swtch.com/zip) with a troll comment

    Solving it is straightforward:

    ```
    $ curl 'https://pbs.twimg.com/media/Fg6EBhaXwAAptvU?format=png&name=4096x4096' --output a.png
    $ binwalk a.png 

    DECIMAL       HEXADECIMAL     DESCRIPTION
    --------------------------------------------------------------------------------
    0             0x0             PNG image, 2560 x 1707, 8-bit colormap, non-interlaced
    821           0x335           Zlib compressed data, best compression
    1378594       0x150922        PDF document, version: "1.5"
    1378670       0x15096E        Zlib compressed data, best compression
    1378932       0x150A74        Zlib compressed data, best compression
    1404041       0x156C89        Zlib compressed data, best compression
    1405378       0x1571C2        Zlib compressed data, best compression
    1406420       0x1575D4        Zlib compressed data, best compression
    1406528       0x157640        Zip archive data, at least v2.0 to extract, compressed size: 328, uncompressed size: 440, name: r/r.zip
    1406946       0x1577E2        End of Zip archive, footer length: 53, comment: "GH22{...} is inside me (maybe?)"

    $ dd bs=1 skip=1378594 if=a.png of=flag.pdf
    ```

    Possible issues encountered by players:
    * they download a compressed image (Twitter shows resized version of images unless you open it in full), which will not contain the hidden information
    * they think the comment on the ZIP file is the flag
    * they complain about the ZIP file being broken

    An archived version of the tweet is available here:
    https://web.archive.org/web/20221107182202/https://twitter.com/el_suisse/status/1589352912857501696
</details>
