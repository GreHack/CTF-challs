# Lucky Luks

Category: Forensic  
Points: 150  
Author: Aleknight  
Description: I think I was backdoored, so I change my passphrase. Now it is safe !  

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{DoN0t0nlyCh4ngeYourKeysReencrypt}</p>
</details>

## Expected resolution steps

<details>
    <summary>Click to discover/hide</summary>
    
    In this challenge, we have access to two encrypted disk images.
    Using `cryptsetup luksDump`, we can see that the two files have different passphrases but have the same master key.
    We have access to the passphrase of the first image disk, `ThisIsSuchAGreatPassword` we can extract the master key.
    
    ```
    cryptsetup luksDump --dump-volume-key disk-17-11-22.img
    ```
    
    Then we can just reuse the dumped key to decrypt the second disk with the flag
    
    ```
    cryptsetup --master-key-file=key.bin open disk-19-11-22.img disk
    ```
    
    Finally, it stays to mount the disk and read the flag.
    
</details>
