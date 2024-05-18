document.getElementById('convertBtn').addEventListener('click', function() {
  document.getElementById('status').textContent = 'Converting...';

  chrome.runtime.sendMessage({ action: 'fetchHTML' }, function(htmlContent) {
      let opt = {
          margin:       10,
          filename:     'myfile.pdf',
          image:        { type: 'jpeg', quality: 0.98 },
          html2canvas:  { scale: 2 },
          jsPDF:        { unit: 'mm', format: 'a4', orientation: 'portrait' }
      };

      html2pdf().from(htmlContent).outputPdf().then(function(pdf) {
          let url = URL.createObjectURL(new Blob([pdf], {type: 'application/pdf'}));
          
          chrome.downloads.download({
              url: url,
              filename: 'converted_page.pdf'
          }, function(downloadId) {
              if (chrome.runtime.lastError) {
                  console.error(chrome.runtime.lastError);
                  return;
              }
              console.log("Download started with ID:", downloadId);
              document.getElementById('status').textContent = 'Conversion completed! Downloading...';
          });
      }).catch(function(error) {
          console.error("Failed to convert HTML to PDF:", error);
          document.getElementById('status').textContent = 'Error during conversion!';
      });
  });
});
