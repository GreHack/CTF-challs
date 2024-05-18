#! /usr/bin/python3

# (Not So) Trusted Dealer
# Crypto challenge for GreHack 11 CTF
# Made by jellykernel

from random import randrange
from time import sleep

alice_secret = 0xb29a7f740ae34817b3500159fe55415f9bd5a1d7f385f04ddd5108fbcd994912abe96a4fc7ae9622f343d16cc54eea783d262ffce7206531e245ec959a6a5563
bob_secret = 0x80e28081ff5d232dcb99b57cbd91bedb84d39f754557f3fcae4cbeecd4519d46ed7c310fc2a01f908641b19d59a70c0f9738f2732307e358b3a3bea4e7bdcdc5

vault = dict()
beaver_store = dict()
d_store = dict()
e_store = dict()

def beaver_triplet_secshare(party_IDs, lambdo, mu, nu):
	lambdo_shares = [0 for _ in range(len(party_IDs))]
	mu_shares = [0 for _ in range(len(party_IDs))]
	nu_shares = [0 for _ in range(len(party_IDs))]

	for i in range(len(party_IDs)-1):
		lambdo_shares[i] = randrange(-(2**128), 2**128)
		mu_shares[i] = randrange(-(2**128), 2**128)
		nu_shares[i] = randrange(-(2**128), 2**128)

	lambdo_shares[-1] = lambdo - sum(lambdo_shares)
	mu_shares[-1] = mu - sum(mu_shares)
	nu_shares[-1] = nu - sum(nu_shares)

	assert sum(lambdo_shares) == lambdo
	assert sum(mu_shares) == mu
	assert sum(nu_shares) == nu

	for i,pid in enumerate(party_IDs):
		beaver_store[pid] = (lambdo_shares[i], mu_shares[i], nu_shares[i])


class MPC2Party():

	def __init__(self, ID, secret):
		self._secret = secret
		self.my_share = 0
		self.other_share = 0
		self.ID = ID
		self.beaver_shares = None
		self.d_value = 0
		self.e_value = 0
		self.xy_share = 0

	def __repr__(self):
		return f"<MPC2Party {hex(self.ID)}, my_share={self.my_share}, other_share={self.other_share}, bshares={self.beaver_shares}, d_value={self.d_value}, e_value={self.e_value}>"

	def sharesend_secret(self, other_id):
		share_1 = randrange(-self._secret+1, self._secret)
		share_2 = self._secret - share_1
		assert share_1 + share_2 == self._secret

		self.my_share = share_1
		vault[other_id] = share_2

	def retrieve_othershare(self):
		self.other_share = vault[self.ID]

	def retrieve_bshares(self):
		self.beaver_shares = beaver_store[self.ID]

	def publish_maskshares(self, other_id):
		if self.ID < other_id:
			d_store[self.ID] = self.my_share - self.beaver_shares[0]
			e_store[self.ID] = self.other_share - self.beaver_shares[1]
		elif self.ID > other_id:
			d_store[self.ID] = self.other_share - self.beaver_shares[0]
			e_store[self.ID] = self.my_share - self.beaver_shares[1]

	def compute_masks(self):
		self.d_value = sum(d_store.values())
		self.e_value = sum(e_store.values())

	def derive_productshare(self, add_common_factor=False):
		self.xy_share = self.d_value*self.e_value*int(add_common_factor) + self.d_value * self.beaver_shares[1] + self.e_value * self.beaver_shares[0] + self.beaver_shares[2]

if __name__ == '__main__':

	try:
		print("------------------------------------------------")
		print("Noble2PC Multi-Party Computation System - v20.23")
		print("Trusted Dealer and Network Manager component")
		print("------------------------------------------------")
		print("Initializing...")

		# Cosmetic sleeps to make it more "realistic"
		sleep(1.2)

		print("1 computation(s) in queue - next computation ID: #181123")
		sleep(0.1)
		print("Waiting for parties to join...")
	except KeyboardInterrupt:
		print("\n[ ERROR ] Caught signal SIGINT, exiting...")
		print("[ INFO ] Logs written to /var/log/noble2pc_error.log")
		exit(1)

	try:
		sleep(0.7)

		alice = MPC2Party(0x00, alice_secret)

		print("[ SYNC ] Host online: Alice @ 10.18.11.1")
		sleep(1)

		bob = MPC2Party(0x01, bob_secret)

		print("[ SYNC ] Host online: Bob @ 10.18.11.2")

		print("[ INFO ] Looking for ./circuits/beaver_product.mpc...")
		sleep(0.2)

		print("[ INFO ] Circuit ready. Running preprocessing phase...")
		sleep(0.3)
		print("[ INFO ] Secret-sharing mode set to ADDITIVE")
		sleep(0.1)

		alice.sharesend_secret(bob.ID)

		print("[ PREPROCESS ] Alice: secret-sharing and exchange completed.")

		bob.sharesend_secret(alice.ID)

		sleep(0.4)
		print("[ PREPROCESS ] Bob: secret-sharing and exchange completed.")

		alice.retrieve_othershare()
		bob.retrieve_othershare()

		print("[ INFO ] Beaver triplet is required to continue. Looking for ./beaver_triplet.btsf...")
		print("[ INFO ] File not found, switching to interactive mode")
		print("[ INTERACTIVE ] Enter the plain values for the triplet (signed 128-bit integers)")

		lambdo = input("lambda: ")
		mu = input("mu: ")
		nu = input("nu: ")

		try:
			bt = [int(lambdo, base=0),int(mu, base=0),int(nu, base=0)]
		except ValueError:
			print("[ ERROR ] Invalid input(s), can't proceed further. Exiting...")
			print("[ DISCONNECT ] Connection closed with 10.18.11.1")
			print("[ DISCONNECT ] Connection closed with 10.18.11.2")
			print("[ INFO ] Logs written to /var/log/noble2pc_error.log")
			exit(1)
		else:
			for i in range(3):
				if bt[i] >= 2**128 or bt[i] <= -(2**128):
					bt[i] %= 2**128

		beaver_triplet_secshare([alice.ID, bob.ID], *bt)

		print("[ INFO ] Beaver triplet secret-sharing complete.")
		sleep(0.6)

		alice.retrieve_bshares()

		print("[ PREPROCESS ] Alice: triplet shares retrieved.")

		sleep(0.5)
		bob.retrieve_bshares()

		print("[ PREPROCESS ] Bob: triplet shares retrieved.")

		print("[ INFO ] Preprocessing phase complete. Running online phase...")

		alice.publish_maskshares(bob.ID)

		sleep(0.9)
		print("[ ONLINE ] Alice: pushed mask shares to the share store")

		bob.publish_maskshares(alice.ID)

		sleep(1)
		print("[ ONLINE ] Bob: pushed mask shares to the share store")

		alice.compute_masks()
		bob.compute_masks()

		sleep(0.3)
		print(f"[ ONLINE ] Mask computation complete. Mask product:\n{hex(sum(d_store.values())*sum(e_store.values()))}")

		try:
			assert alice.d_value == bob.d_value
			assert alice.e_value == bob.e_value
		except AssertionError:
			# Should not ever happen, but just in case...
			print("[ ERROR ] Critical internal error (code:ASRTERR), exiting...")
			exit(2)

		alice.derive_productshare(True)

		sleep(0.5)
		print("[ ONLINE ] Alice: resulting share computation complete.")

		bob.derive_productshare()

		sleep(0.4)
		print("[ ONLINE ] Bob: resulting share computation complete.")

		sleep(0.6)
		print("[ PUBLISH ] Alice: resulting share released.")
		print("[ PUBLISH ] Bob: resulting share released.")

		print("[ INFO ] Online phase complete.")

		print(f"[ RELEASE ] Final result:\n{hex(alice.xy_share + bob.xy_share)}")

		sleep(0.1)
		print("[ DISCONNECT ] Connection closed with 10.18.11.1")
		sleep(0.1)
		print("[ DISCONNECT ] Connection closed with 10.18.11.2")

		print("------------------------------------------------")
		print("No more computations in queue, writing logs to /var/log/noble2pc.log")
		print("Exiting...")
		exit(0)

		# Debugging prints

		# print(f"Alice final share: {hex(alice.xy_share)}")
		# print(f"Bob final share: {hex(bob.xy_share)}")

		# print(f"Computed product: {hex(alice.xy_share + bob.xy_share)}")
		# print(f"Expected product: {hex(alice_secret*bob_secret)}")

		# print(f"Mask product: {hex(sum(d_store.values())*sum(e_store.values()))}")
		# assert alice.xy_share + bob.xy_share == alice_secret*bob_secret

	except KeyboardInterrupt:
		print("\n[ ERROR ] Caught signal SIGINT, exiting...")
		print("[ DISCONNECT ] Connection closed with 10.18.11.1")
		print("[ DISCONNECT ] Connection closed with 10.18.11.2")
		print("[ INFO ] Logs written to /var/log/noble2pc_error.log")
		exit(1)
