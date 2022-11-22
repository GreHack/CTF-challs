# Cafebabe

Category: Reverse  
Points: 250  
Author: Aleknight  
Description: We found this two files together on this old computer. Can you tell me the content of the text file ?  

<details>
    <summary>Flag (click to discover/hide)</summary>
    <p>GH22{IL0v3Th3Tr33HuffmanGaveT0M3}</p>
</details>

## Expected resolution steps

<details>
    <summary>Click to discover/hide</summary>
    
    The goal of this challenge was to uncompress a file, compressed with huffman encoding.
    To do so, the binary used to compress the file was available.
    
    The first step was to understand the file format:
    - Header
    - Compressed buffer
    - Map

    Reversing the binary, we understand data stored in each section.
    Once the map is parsed, it stays to iterate over the compressed buffer to decompress it.
    Data is written with chunck of 8 bytes, so we have to take care of the endianness.
</details>
