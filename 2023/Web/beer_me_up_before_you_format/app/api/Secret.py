import os

SECRET_ENDPOINT = "admin-s3cr3t-3ndp01nt-Ungu3ss4ble"

class Secret:
	def __init__(self, secret):
		self.secret = secret

	def __repr__(self):
		return f"The secret endpoint is : /{self.secret} !"