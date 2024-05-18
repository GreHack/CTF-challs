FROM debian:11

RUN apt update && apt install  openssh-server sudo ansible vim nano dos2unix -y

RUN useradd -ms /bin/bash student

RUN echo 'student:student' |chpasswd

RUN ln -sf /dev/null /home/student/.bash_history

COPY the_flag_is_here_gg.txt /root/the_flag_is_here_gg.txt

COPY sudoers /etc/sudoers
RUN service ssh start

EXPOSE 22

CMD ["/usr/sbin/sshd","-D"]