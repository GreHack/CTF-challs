chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
  if (message.action === 'fetchHTML') {
      chrome.tabs.query({ active: true, currentWindow: true }, (tabs) => {
          const activeTab = tabs[0];
          chrome.tabs.sendMessage(activeTab.id, { action: 'getHTML' }, (response) => {
              sendResponse(response);
          });
      });
      return true;
  }
});
