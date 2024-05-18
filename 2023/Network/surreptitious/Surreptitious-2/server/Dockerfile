FROM fedora:latest

# install netcat 
RUN dnf -y update && \
    dnf install nc -y && \
    dnf clean all && \
    rm -rf /var/cache /var/log/dnf* /var/log/yum.*

# create low privileged user
RUN useradd user

USER user

ENTRYPOINT ["nc", "-lnvp", "4444"]
