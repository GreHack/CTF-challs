function getCookie(name) {
    const value = `; ${document.cookie}`;
    const parts = value.split(`; ${name}=`);
    if (parts.length === 2) return parts.pop().split(';').shift();
}

const saveSettings = (settings) => {
    document.cookie = `settings=${settings}`;
}

const renderSettings = (settings) => {
    // backgound color
    var backgroundColor = window.settings["backgroundColor"] ? window.settings["backgroundColor"] : "#ffde8c";
    document.documentElement.style.setProperty("--background-color", backgroundColor);
    document.getElementById("backgroundColor").value = backgroundColor;

    // text color
    var textColor = window.settings["textColor"] ? window.settings["textColor"] : "#000000";
    document.documentElement.style.setProperty("--text-color", textColor);
    document.getElementById("textColor").value = textColor;
}

// can't load HTML from qs.
const getSettings = (d) => {
    try {
        s = JSON.parse(d);
        delete s.html;
        return JSON.stringify(s);
    } catch {
        while (d != d.replaceAll("html", "")) {
            d = d.replaceAll("html", "");
        }
        return d;
    }
}

window.onload = () => {
    // settings init
    const params = (new URLSearchParams(window.location.search));
    if (params.get("settings")) {
        window.settings = getSettings(params.get("settings"));
        saveSettings(window.settings);
        renderSettings(window.settings);
    } else {
        window.settings = getCookie("settings");
    }
    window.settings = JSON.parse(window.settings);

    // colors init
    document.getElementById("backgroundColor").onchange = function() {
        window.settings["backgroundColor"] = this.value;
        document.documentElement.style.setProperty("--background-color", window.settings["backgroundColor"]);
        saveSettings(JSON.stringify(window.settings));
    } 
    document.getElementById("textColor").onchange = function() {
        window.settings["textColor"] = this.value;
        document.documentElement.style.setProperty("--text-color", window.settings["textColor"]);
        saveSettings(JSON.stringify(window.settings));
    }

    // renderer init
    document.getElementById("userInput").onkeyup = function() {
        window.settings["html"] = this.value;
        document.getElementById("render").srcdoc = `<style>* { text-align: center; }</style>${this.value}`;
        saveSettings(JSON.stringify(window.settings));
    }
}
