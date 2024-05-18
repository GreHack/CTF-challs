FROM fedora:latest

# install ssh server 
RUN dnf -y update && \
    dnf install -y openssh-server && \
    dnf clean all && \
    rm -rf /var/cache /var/log/dnf* /var/log/yum.*

# create simpleuser with simpleuser as password
RUN useradd -ms /bin/bash simpleuser -p '$6$PK1Sk3bEAnhovxNN$VhkeFxSUWkVc3Bw3G2EHyTpB1HJxPAdgYpoaebrvXOWn9PLZ1pgMPyIOjucPh1SWdf5JTMRUTyblyob16OMBN.'
RUN usermod -a -G wheel simpleuser
# generate ssh server keys
RUN ssh-keygen -A

ENTRYPOINT ["/usr/sbin/sshd", "-D"]
