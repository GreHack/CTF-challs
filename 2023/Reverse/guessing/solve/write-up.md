# Write up (lightweight)

## Flag 1 

Basic reverse enumeration :
```
strings ./guessing 
//FLAG
```

## Flag 2 

After a few minutes of reversing, we've uncovered this intriguing code in your preferred decompiler : 
```
    if ((int)uVar3 < 0) {
      if (uVar3 == 0xffffffff) {
        FUN_000113d5("/app/flag.txt");
        uVar2 = 2;
      }
      else {
        uVar2 = 5;
      }
    }
```

We then understand that uvar3 is the value entered by the user. The nonsense of 'uVar3 < 0' and 'uVar3' == 0xffffffff comes from the fact that uVar3 is a signed integer. We have an integer overflow : 

The solution is simply to enter the value 4294967295 (0xffffffff in decimal)

## Flag 3 (bonus)

Use dichotomy to obtain the flag of the challenge.