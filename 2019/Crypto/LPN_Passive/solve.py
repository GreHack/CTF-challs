if __name__ == "__main__":
    debug = False
    
    with open("capture", "r") as f:
        lines = f.read().split("\n")[1:-1]
    
    
    print("Parse trace...")
    data_set = []
    i = 0 
    while "Welcome" not in lines[i]:
        line_c = lines[i]
        line_r = lines[i+1]
        
        c = int(line_c.split(' ')[-1], 16)
        r = int(line_r.split(' ')[-1])
    
        data_set.append((c, r))
        i += 2
    
    
    print("Sort challenges...")
    unit_left = {}
    unit_right = {}
    for b in range(256):
        for c, r in data_set:
            if (c >> b & 1):
                if not (c >> ((b + 1)%256)) & 1:
                    if b not in unit_left:
                        unit_left[b] = []
                    unit_left[b].append((c, r))
                if not (c >> ((b - 1) % 256)) & 1:
                    if b not in unit_right:
                        unit_right[b] = []
                    unit_right[b].append((c,r))
    
    
    print("Search for good pairs...")
    # Search pairs for left
    res = {}
    flag_b = ''
    i = 0
    for b in unit_left:
        i-=-1
    
        if b not in res:
            res[b] = [0, 0]
    
        c = 0
        for lb1, r1 in unit_left[b]:
            for lb2, r2 in unit_left[(b - 1)%256]:
                if (lb1 ^ lb2) == (1 << b):
                    res[b][r1 ^ r2] += 1
                    c += 1
    
        if debug:
            print("I found {} pairs".format(c))
        flag_b += '0' if res[b][0] > res[b][1] else '1'
        if i % 8 == 0:
            print(int(flag_b[::-1], 2).to_bytes(i//8, "big"))
