const express = require("express");
const _ = require("lodash");
const fs = require('fs');
const path = require('path');
const cookieParser = require('cookie-parser');
const { execSync } = require('child_process');
const { readdirSync } = require('fs');
const jwt = require('jsonwebtoken');

// app configuration
const app = express();
app.use(express.json());
app.use(cookieParser());
app.use('/static', express.static(path.join(__dirname, 'static')))

// const variables
const port = 3000

let user = {
    username: 'Ronald Rivest',
    spec: 'RSA',
    CheckifAdmin: false
};


app.get('/', (req, res) => {
    if (req.query.source !== undefined) {
        const sourceCode = fs.readFileSync('./index.js', 'utf8');
        return res.send(`<pre>${sourceCode}</pre>`);
    } else {
        return res.sendFile('views/index.html', {root: __dirname});
    }
});

app.post('/api/add', (req, res) => {
    console.log('Add:', req.body);
    let {a, b} = req.body;
    return res.send(`${a+b}`)
});

app.post('/api/sub', (req, res) => {
    console.log('Sub:', req.body);
    let {a, b} = req.body;
    return res.send(`${a-b}`)
});

app.post('/api/mul', (req, res) => {
    console.log('Mul:', req.body);
    let {a, b} = req.body;
    return res.send(`${a*b}`)
});

app.post('/api/eq', (req, res) => {
    console.log('Eq:', req.body);
    let {a, b} = req.body;
    return res.send(`${a == b}`)
});

app.post('/api/polynome', (req, res) => {
    console.log('Data:', req.body);
    let array = {
        a: 0,
        b: 0,
        c: 0
    };
    _.merge(array, req.body);
    let { a, b, c } = array;

    if (a === 0) {
        return res.send("Coefficient 'a' must not be zero for a quadratic equation.");
    }

    const discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        const root1 = (-b + Math.sqrt(discriminant)) / (2 * a);
        const root2 = (-b - Math.sqrt(discriminant)) / (2 * a);
        return res.send(`The roots are: ${root1} and ${root2}`);
    } else if (discriminant === 0) {
        const root = -b / (2 * a);
        return res.send(`The root is: ${root}`);
    } else {
        return res.send("The quadratic equation has no real roots.");
    }
});

// Route admin
app.get('/admin', (req, res) => {
    if (user.CheckIfAdmin) {
        const token = req.cookies.token;
        // first login
        if (!token){
            console.log('No token provided. Giving one');

            const keyFiles = readdirSync('./keys');
            if (keyFiles.length === 0) {
                console.log('No key files found.');
                return res.status(500).json({ message: 'No key files found.' });
            }
            const kid = keyFiles[0];
            const jwtSecret = fs.readFileSync(`./keys/${kid}`, 'utf8');

            const token = jwt.sign({ username: user.username, admin: true }, jwtSecret, { expiresIn: '4h', header: { kid } });
            console.log(token);

            res.cookie('token', token, { httpOnly: true });
            return res.sendFile('views/admin.html', {root: __dirname});
        } else {
            // get the JWT Header
            let header;
            try {
                header = JSON.parse(Buffer.from(token.split('.')[0], 'base64').toString());
            } catch (error) {
                console.log('Failed to decode JWT header:', error.message);
                return res.status(400).json({ message: 'Failed to decode JWT header.' });
            }

            // get the KID 
            const kid = header.kid;
            if (!kid) {
                console.log('No kid found in JWT header.');
                return res.status(400).json({ message: 'No kid found in JWT header.' });
            }

            // check if only uuid are used
            const allowedChars = /^[a-zA-Z0-9_\s\.-]+$/ 
            if (!allowedChars.test(kid)) {
                console.log('Forbidden characters in kid.');
                return res.status(400).json({ message: 'Forbidden characters in kid.' });
            }

            // fetch the KID file
            let keyFile;
            try {
                keyFile = execSync(`find ./keys -name ${kid}`).toString().trim();
            } catch (error) {
                console.log('Failed to find key file:', error.message);
                return res.status(500).json({ message: 'Failed to find key file.' });
            }
        
            if (!keyFile) {
                console.log('No key file found.');
                return res.status(400).json({ message: 'No key file found.' });
            }

            // retrive secret from file
            const jwtSecret = fs.readFileSync(keyFile, 'utf8');
            try {
                jwt.verify(token, jwtSecret);
            } catch (error) {
                console.log('Failed to verify JWT:', error.message);
                return res.status(403).json({ message: 'Failed to verify JWT.' });
            }
        
            return res.sendFile('views/admin.html', {root: __dirname});
        }
    }
    return res.status(403).json({ message: 'You are not admin.' });
});


app.listen(port, () => console.log(`Server app listening on port ${port}`));