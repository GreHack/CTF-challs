FROM fedora:latest

# install ssh server and reset capabilities of shadow-utils (this adds required capabilities for newuidmap)
RUN dnf -y update && \
    rpm --setcaps shadow-utils && \
    dnf install -y openssh-server && \
    dnf clean all && \
    rm -rf /var/cache /var/log/dnf* /var/log/yum.*

# create simpleuser with simpleuser as password
RUN useradd -ms /bin/bash simpleuser -p '$6$PK1Sk3bEAnhovxNN$VhkeFxSUWkVc3Bw3G2EHyTpB1HJxPAdgYpoaebrvXOWn9PLZ1pgMPyIOjucPh1SWdf5JTMRUTyblyob16OMBN.'
# create flaguser
RUN adduser flaguser
# add subuid and subguid to grant simpleuser access to flaguser (that has uid and gid set to 1001)
RUN usermod --add-subuids 1001-1001 simpleuser
RUN usermod --add-subgids 1001-1001 simpleuser

# add the flag file
COPY flag.txt /home/flaguser/.flag.txt
RUN chmod 400 /home/flaguser/.flag.txt
RUN chown flaguser:flaguser /home/flaguser/.flag.txt

# generate ssh server keys
RUN ssh-keygen -A

ENTRYPOINT ["/usr/sbin/sshd", "-D"]

# podman build -t subuser .
# podman run -d --rm -p 2222:22 --name subuser subuser
