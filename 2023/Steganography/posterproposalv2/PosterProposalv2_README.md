# Poster Proposal v2

## Description

Le staff m'a finalement demandé de "substituer" les couleurs sombres pour quelque chose
de plus... marquant. J'imagine que cette version 2 (avec de la 3D, wow) devrait
être la bonne, non ?
The staff finally asked me to "substitute" darker colors for more... striking ones.
I guess this version 2 (with 3D, wow) should be the good one, right ?

Created by jellykernel

## Difficulty

6.5/10 - Teenage Medium / Caramel Hard

## Flag

GH{AES'_SB0x_1s_v3ry_c0lorful,d03sn't_i7?_#rgba}

## Attached files

posterv2.pdf

## Solve

You'll need to a color picker as well for the first one. But this time,
picking the 16 colors will not be enough. In fact, you'll need to use
the inverse S-Box of AES (the 'subtitute' between quotes isn't there by chance.)
to recover the original bytes.
And, if you get a look at the metadata, you'll see the title of the PDF is "⇧R",
which you should read as "Shift R", or "SR"; depending on the player's knowledge
about AES, he may understand that I added a ShiftRows operation on top of the
substitution. (The player can, of course, try to manually reassemble the flag as
it does have a meaning. Things could've been way harder if I used a hash
instead for the flag.)
