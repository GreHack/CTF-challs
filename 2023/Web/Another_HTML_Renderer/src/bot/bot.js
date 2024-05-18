// required packages
const puppeteer = require("puppeteer");

// variables
const host = "127.0.0.1:5000";

// sleep
const delay = (time) => {
    return new Promise(resolve => setTimeout(resolve, time));
}

// navigate
async function goto(url) {
	const browser = await puppeteer.launch({
		headless: "new",
		ignoreHTTPSErrors: true,
		args: ["--no-sandbox", "--ignore-certificate-errors" ],
		executablePath: "/usr/bin/chromium-browser"
	});

	const page = await browser.newPage();
    await page.setCookie({
		"name" : "flag",
		"value" : "GH{WtF_1s_Th4t_C00ki3_P4rS1ng}",
		"domain" : host,
		"httpOnly": true,
		"sameSite": "Lax"
	});
	await page.setDefaultNavigationTimeout(5000);

    // Go to provided URL
	try {
	    await page.goto(url);
	} catch {}

    await delay(1000);

    browser.close();
	return;
}

if (process.argv.length === 2) {
    console.error("No URL provided!");
    process.exit(1);
}

goto(process.argv[2]);
