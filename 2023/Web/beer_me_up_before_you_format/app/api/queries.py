import sqlite3
import json

def connect():
	conn = None
	try:
		conn = sqlite3.connect("db/database.db")
	except Error as e:
		print(e)
	return conn

def get_user(id_user):
	conn = connect()
	cur = conn.cursor()
	cur.execute("SELECT id, username, email, phone, address, city, postalZip, region, role, token FROM users WHERE id=?;", (id_user,))
	rows = cur.fetchall()
	result = []
	for row in rows:
	    result.append({cur.description[i][0]: value for i, value in enumerate(row)})
	return result

def update_password(token, password):
	conn = connect()
	cur = conn.cursor()
	cur.execute("UPDATE users SET pwd = ? WHERE token = ?", (password, token,))
	conn.commit()
	return cur.rowcount

def api_login(username, password):
	conn = connect()
	cur = conn.cursor()
	cur.execute("SELECT id, role FROM users WHERE username = ? AND pwd = ?", (username, password,))
	row = cur.fetchone()
	return {cur.description[i][0]: value for i, value in enumerate(row)}