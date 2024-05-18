# Subuser

Someone from outer space once told me: before becoming superuser, you should become a subuser. What a wise man!

## instructions

Connect to machine `ssh simpleuser@localhost -p 2222` with password `simpleuser`

# To run the challenge

```
$ podman build -t subuser . 
$ podman run -d --rm -p 2222:22 --name subuser subuser
```

# flag

The flag is:
```
GH{LkLW4WsHPnkoMo61mxsWOCXlMrveStW988ootJX9mf2ZlWrUxL}
```
