# Hold_My_B33r_2

In the list of the ingredients, we need to take the first char/number of all ingredients and base64 decode it.

```
Ingrédients : Raisins 0°, houblon, 7 Yuzu jaunes, Malts, zestes citron, miel, Infusions zestes, Mandarine, 3 Jasmins, 9 Céréales grenobloises
```

```bash
echo 'R0h7YjMzcmIzM3J9Cg' | base64 -d

GH{b33rb33r}
```

Flag : `GH{b33rb33r}`