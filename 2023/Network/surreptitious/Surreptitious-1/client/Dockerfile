FROM fedora:latest

# install netcat 
RUN dnf -y update && \
    dnf install -y nc && \
    dnf clean all && \
    rm -rf /var/cache /var/log/dnf* /var/log/yum.*

# create low privileged user
RUN useradd user

# copy flag
COPY flag /flag
RUN chmod 400 /flag
RUN chown user:user /flag

# copy script sending regularly flag content to server
COPY script.sh /script.sh
RUN chmod 500 /script.sh
RUN chown user:user /script.sh

USER user

ENTRYPOINT ["/script.sh"]
