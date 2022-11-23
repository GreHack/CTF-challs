FROM python:3
RUN apt-get update && apt-get install -y figlet fortune

# User config
ARG user=chall
ARG group=chall
ARG uid=1000
ARG gid=1000
RUN groupadd -g ${gid} ${group}
RUN useradd -u ${uid} -g ${group} -s /bin/sh -m ${user}
RUN chmod a-x /bin/cat
USER ${uid}:${gid}

# Chall setup
COPY chall /chall
COPY figlet-fonts/ /usr/share/figlet/
WORKDIR /chall
RUN pip install -r requirements.txt
