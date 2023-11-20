FROM python:3-slim-buster

# Install socat and libc6-compat
RUN apt update && \
    apt install -y socat && \
    rm -rf /var/lib/apt/lists/*

# Create a group and user
RUN adduser --disabled-password pwn

# Import files from build
WORKDIR /pwn
COPY app.py admin.py requirements.txt /pwn/
RUN chmod 555 /pwn/app.py /pwn/admin.py

USER pwn
RUN pip install -r requirements.txt
CMD socat TCP-LISTEN:4242,fork system:"python /pwn/app.py",pty,stderr,echo=0
