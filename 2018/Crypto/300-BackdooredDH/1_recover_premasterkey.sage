#Recupere dans le s3cr3t.txt
p = 496576885634448546561329259190237430092764246374042364680948723920533217176305491630862210331103813059677699375673567619020059824819177888026449668075060319 
q = 335757967279107536162274884801677644837708561908040408100508731066171702397620675200356523463331251495191289697549450204757053989689423002606061698922563 

# Dans wireshark, dans le server key exchange, le serveur envoie les parametres du groupe pour l'echange DH
n = int("ed:6e:4f:3f:5b:ab:6d:ed:96:42:3f:99:3b:f8:cf:33:f9:e7:6c:62:63:8c:77:0f:99:5e:18:8c:52:af:f4:ec:02:42:36:ae:87:7e:96:d6:0b:5e:e7:8b:c0:92:38:3f:7f:ed:e0:df:62:e7:27:cc:58:7e:f7:cd:f6:f2:eb:2c:1b:f6:c8:7a:a0:8a:97:6a:43:99:73:d3:d2:7a:9a:7e:c1:54:52:c4:2d:bb:3d:75:6d:b6:6c:5c:94:14:98:43:f1:a1:b1:b2:29:7c:ef:c2:08:ee:7e:aa:c6:7a:90:b4:f1:9e:46:e1:e8:32:3d:97:9c:46:ca:b3:a5:93:d4:dd".replace(":", ""), 16)
g = 2

assert(n == p*q)

#client pubkey, y = g^b % n
#A recuperer a la main dans wireshark, dans le paquet TLS client key exchange
y = int("7e:56:13:7f:a7:7e:10:ed:06:01:57:3e:6d:2f:07:57:5f:18:85:00:d6:fc:5e:75:ad:d1:3d:c6:03:d9:12:a9:43:04:bb:03:52:4e:b5:cc:56:3f:61:90:32:89:dd:ff:97:c1:0e:ec:48:d3:16:a7:dd:2e:07:47:50:47:a7:69:c5:a8:5e:00:d7:ea:f7:61:00:19:4d:c1:ae:d3:e5:8f:68:29:7d:df:c3:f0:be:5e:ee:1d:0c:51:6e:b2:70:c5:be:67:c8:1a:90:1e:76:0e:79:5e:ec:0e:06:16:cf:7d:c0:60:dd:6f:63:30:0c:cf:50:d7:2e:9b:01:54:aa:0c".replace(":", ""), 16)

#server pubkey, y2 = g^a % n
#A recuperer a la main dans wireshark, dans le paquet TLS server key exchange
y2 = int("b2:fc:f1:b2:77:2c:12:9b:26:58:c0:2e:36:e9:cc:91:4f:6b:20:e4:bc:e8:99:02:d1:4c:0e:d2:80:80:7b:88:51:e5:52:be:d5:b6:28:a9:0b:f3:9f:27:a2:db:7d:e8:eb:98:08:6d:de:ff:62:8c:49:8a:36:d7:75:89:af:0f:e8:63:18:bf:58:7f:b9:e7:31:ad:9e:9d:b6:8b:31:20:22:5c:54:13:b3:9e:ca:d6:87:a3:54:76:cf:9c:53:81:c1:6f:aa:b5:c8:d4:16:c3:d2:31:77:9a:d0:39:2a:b9:4c:46:7a:28:44:6e:68:05:01:c6:aa:6d:97:67:30:45".replace(":", ""), 16)

# Si on casse le 1er DLP en retrouvant b, il suffit de faire y = y2^b % n = g^ab % n
# y est notre shared secret, la "premasterkey"

# On peut commencer par reduire le probleme dans p et dans q vu que n = p*q
# On remarque ensuite que p-1 et q-1 sont facilement factorisables (ils sont "smooth", ie tout leurs facteurs sont petits)
# Grace a polhig-hellman on peut reduire le probleme dans des sous groupes , ayant pour ordre chacun des facteurs
# le DLP est facile dans ses sous groupes vu que leur ordre est tres petit, on peut donc solve chacun des petits DLP avec pollard_rho
# Et on retrouve notre x de depart a l'aide du theoreme des restes chinois, d'abord pour p et q, puis pour n.

# viens de https://github.com/mimoo/Diffie-Hellman_Backdoor/blob/master/backdoor_generator/backdoor_generator_tests.sage

p_factors = [f[0] for f in factor(p-1)]
q_factors = [f[0] for f in factor(q-1)]

yp = y % p
xp = 0
xp_mod = 1

for order in p_factors[1:]:
	new_problem = power_mod(yp, (p-1)//order, p)
	new_generator = power_mod(g, (p-1)//order, p)
	new_problem = GF(p)(new_problem)
	new_generator = GF(p)(new_generator)
	new_xp = discrete_log_rho(new_problem, new_generator, order)
	xp = CRT(xp, new_xp, xp_mod, order)
	xp_mod *= order

yq = y % q
xq = 0
xq_mod = 1

for order in q_factors:
	new_problem = power_mod(yq, (q-1)//order, q)
	new_generator = power_mod(g, (q-1)//order, q)
	new_problem = GF(q)(new_problem)
	new_generator = GF(q)(new_generator)
	new_xq = discrete_log_rho(new_problem, new_generator, order)
	xq = CRT(xq, new_xq, xq_mod, order)
	xq_mod *= order

b = CRT(xp, xq, xp_mod, xq_mod)

if pow(g, b, n) == y:
	premasterkey = pow(y2, b, n)
	print premasterkey 
else:
	print "Attack failed"



