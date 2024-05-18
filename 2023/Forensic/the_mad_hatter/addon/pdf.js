chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
    if (message.action === 'getHTML') {
        sendResponse(document.documentElement.outerHTML);
    }
});
