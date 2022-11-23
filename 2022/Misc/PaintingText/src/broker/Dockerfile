FROM python:3

# User config
ARG user=broker
ARG group=broker
ARG uid=1000
ARG gid=1000
RUN groupadd -g ${gid} ${group}
RUN useradd -u ${uid} -g ${group} -s /bin/sh -m ${user}
USER ${uid}:${gid}

# broker config
COPY broker /broker
WORKDIR /broker
RUN pip install -r requirements.txt
