// main.js
// XSS Checker - GreHack 2023
// Author       : Nishacid
// Node Version : 19.*

// imports
const express = require('express');
const puppeteer = require("/usr/src/app/node_modules/puppeteer");
const path = require('path');
const {
    check,
    validationResult
} = require('express-validator');

// express config
const app = express();
app.use(express.urlencoded({
    extended: true
}));
app.use(express.json());
app.use('/static', express.static(path.join(__dirname, 'static')));

// web routes
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.post('/xss-check', [
    check('html').exists()
], async (req, res, next) => {
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
        return res.status(400).json({
            errors: errors.array()
        });
    }

    const userHtml = req.body.html;
    let browser;

    try {
        browser = await puppeteer.launch({
            headless: true,
            args: [
                '--no-sandbox',
                '--disable-setuid-sandbox',
            ]
        });
        const page = await browser.newPage();

        await page.setDefaultTimeout(5000);

        await page.setContent(`<html><body>${userHtml}</body></html>`, {
            waitUntil: 'networkidle0'
        });

        res.json({
            result: 'Your payload will be checked.'
        });

    } catch (error) {
        console.error(error);
        next(error);
    } finally {
        if (browser) {
            await browser.close();
        }
    }
});

app.use((err, req, res, next) => {
    res.status(500).json({
        error: err.message || 'An unexpected error occurred. Please try again later.'
    });
});

app.listen(8000, () => {
    console.log("Server up and running");
});
