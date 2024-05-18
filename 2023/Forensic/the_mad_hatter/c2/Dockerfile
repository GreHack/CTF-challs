FROM python:3.10

RUN mkdir /app/
WORKDIR /app/

COPY ./src/ .

RUN pip3 install -r requirements.txt

CMD python3 app.py

EXPOSE 5000