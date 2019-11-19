import socket
from binascii import hexlify 



if __name__ == "__main__":
    debug = False
    
    s = socket.socket()
    s.connect(("localhost", 4361))
    
    
    def recv_until(s, m):
        res = b''
        while res[-len(m):] != m:
            res += s.recv(1)
    
        return res
    
    
    
    flag_b = ''
    
    p = float(recv_until(s, b'E').split(b'\n')[0])
    print(p)
    bias = 1 - 2*p
    
    recv_until(s, b"encoded): ")
    
    # For each bit in the flag
    for i in range(32*8):
        c = 1 << (8*32 - i - 1)
    
        c = hexlify(c.to_bytes(32, "big"))
    
        # Debias loop
        n = 16*int((1/(bias**2)))
        if debug:
            print("[*] Doing {} debias loops.".format(n))
        c0 = 0
        c1 = 0
        for j in range(n):
            s.send(c + b'\n')
            r = recv_until(s, b"encoded): ").split(b'\n')[0]
            if r == b'1':
                c1 += 1
            else:
                c0 += 1
    
        if c0 > c1:
            flag_b += '0'
        else:
            flag_b += '1'
    
        if not (i + 1) % 8:
            print(int(flag_b, 2).to_bytes((i + 1)//8, "big"))
    
    
    s.close()
