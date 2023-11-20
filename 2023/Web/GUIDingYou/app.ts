require('dotenv').config();
import { NextFunction, Request, Response } from 'express';

const createError = require('http-errors');
const express = require('express');
const path = require('path');
const cors = require('cors');
const bodyParser = require('body-parser');

const messageRouter = require('./routes/message');
require('./database');

const app = express();

app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cors({
  origin: function (origin: any, callback: any) {
    callback(null, origin);
  },
  credentials: true,
}));
app.use(bodyParser.json());

app.get('/', function(req: Request, res: Response) {
  return res.json({ endpoints: [
      { method: 'GET', path: '/' },
      { method: 'GET', path: '/messages' },
      { method: 'GET', path: '/messages/<id>?uuid=<uuid>'},
      { method: 'POST', path: '/messages', body: [
          { message: "string" }
        ]
      }
    ]
  })
});

app.use('/messages', messageRouter);

// catch 404 and forward to error handler
app.use(function(req: Request, res: Response, next: NextFunction) {
  next(createError(404));
});

// error handler
app.use(function(err: any, req: Request, res: Response) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.send();
});

app.listen(3000);
