FROM debian:11

RUN apt update && apt install  openssh-server sudo ansible vim nano -y

RUN useradd -ms /bin/bash student

RUN echo 'student:student' |chpasswd

RUN ln -sf /dev/null /home/student/.bash_history

COPY flag.txt /root/flag.txt

COPY sudoers /etc/sudoers
RUN service ssh start

EXPOSE 22

CMD ["/usr/sbin/sshd","-D"]