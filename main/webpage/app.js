/**
 * Add gobals here
 */
var seconds 	= null;
var otaTimerVar =  null;
var wifiConnectInterval = null;

/**
 * Initialize functions here.
 */
$(document).ready(function(){
	getUpdateStatus();
    setTemperatureInterval();
    $("#connect_wifi").on("click", function(){
        checkCredentials();
    });
});   

/**
 * Gets file name and size for display on the web page.
 */        
function getFileInfo() 
{
    var x = document.getElementById("selected_file");
    var file = x.files[0];

    document.getElementById("file_info").innerHTML = "<h4>File: " + file.name + "<br>" + "Size: " + file.size + " bytes</h4>";
}

/**
 * Handles the firmware update.
 */
function updateFirmware() 
{
    // Form Data
    var formData = new FormData();
    var fileSelect = document.getElementById("selected_file");
    
    if (fileSelect.files && fileSelect.files.length == 1) 
	{
        var file = fileSelect.files[0];
        formData.set("file", file, file.name);
        document.getElementById("ota_update_status").innerHTML = "Uploading " + file.name + ", Firmware Update in Progress...";

        // Http Request
        var request = new XMLHttpRequest();

        request.upload.addEventListener("progress", updateProgress);
        request.open('POST', "/OTAupdate");
        request.responseType = "blob";
        request.send(formData);
    } 
	else 
	{
        window.alert('Select A File First')
    }
}

/**
 * Progress on transfers from the server to the client (downloads).
 */
function updateProgress(oEvent) 
{
    if (oEvent.lengthComputable) 
	{
        getUpdateStatus();
    } 
	else 
	{
        window.alert('total size is unknown')
    }
}

/**
 * Posts the firmware udpate status.
 */
function getUpdateStatus() 
{
    var xhr = new XMLHttpRequest();
    var requestURL = "/OTAstatus";
    xhr.open('POST', requestURL, false);
    xhr.send('ota_update_status');

    if (xhr.readyState == 4 && xhr.status == 200) 
	{		
        var response = JSON.parse(xhr.responseText);
						
	 	document.getElementById("latest_firmware").innerHTML = response.compile_date + " - " + response.compile_time

		// If flashing was complete it will return a 1, else -1
		// A return of 0 is just for information on the Latest Firmware request
        if (response.ota_update_status == 1) 
		{
    		// Set the countdown timer time
            seconds = 10;
            // Start the countdown timer
            otaRebootTimer();
        } 
        else if (response.ota_update_status == -1)
		{
            document.getElementById("ota_update_status").innerHTML = "!!! Upload Error !!!";
        }
    }
}

/**
 * Displays the reboot countdown.
 */
function otaRebootTimer() 
{	
    document.getElementById("ota_update_status").innerHTML = "OTA Firmware Update Complete. This page will close shortly, Rebooting in: " + seconds;

    if (--seconds == 0) 
	{
        clearTimeout(otaTimerVar);
        window.location.reload();
    } 
	else 
	{
        otaTimerVar = setTimeout(otaRebootTimer, 1000);
    }
}

/**
 * Gets Temperature from Temperate Sensor and displays it on the web page.
 */
function getTemperature(){
    $.getJSON("/temperature.json", function(data){
        $("#temperature_reading").text(data["temp"]);
    }); 
}

/**
 * Sets Interval for Temperature Sensor to update every 5 seconds.
 */
function setTemperatureInterval(){
    setInterval(getTemperature, 5000);
}

/**
 * Clears the connection status interval.
 */
 function stopWifiConnectStatusInterval(){
    if(wifiConnectInterval != null){
        clearInterval(wifiConnectInterval);
        wifiConnectInterval = null;
    }   
}

/**
 * Gets the WiFI connection status.
 */
function getWifiConnectStatus(){
    var xhr = new XMLHttpRequest();
    var requestURL = "/wifiConnectStatus";
    xhr.open('POST', requestURL, false);
    xhr.send('wifi_connect_status');

    if (xhr.readyState == 4 && xhr.status == 200){
        var response = JSON.parse(xhr.responseText);
        document.getElementById("wifi_connect_status").innerHTML = "Connecting...";
    }
    if(response.wifi_connect_status == 2){
        document.getElementById("wifi_connect_status").innerHTML = "<h4 class='rd'>Check credentials and compatibility!</h4>";
        stopWifiConnectStatusInterval();
    }
    else if(response.wifi_connect_status == 3){
        document.getElementById("wifi_connect_status").innerHTML = "<h4 class='gr'>Connection Success!!</h4>";
        stopWifiConnectStatusInterval();
    }
}

/**
 * Starts the interval for checking the connection status.
 */

function startWifiConnectStatusInterval(){
    wifiConnectInterval = setInterval(getWifiConnectStatus, 2800);
}

/**
 * Connect WiFi function called using the credentials entered on the web page.
 */
function connectWifi(){
    // Get the SSID and Password from the web page
    selectSSID = $("#connect_ssid").val();
    password = $("#connect_password").val();

    $.ajax({
        url: '/wifiConnect.json',
        dataType: 'json',
        method: 'POST',
        cache: false,
        headers : {'my-connect-ssid': selectSSID, 'my-connect-password': password},
        data: {'timestamp': Date.now()}
    });
    startWifiConnectStatusInterval();
}

/**
 * Checks the credentials entered on the web page.
 */

function checkCredentials(){
    errorList = "";
    credsOk = true;

    selectSSID = $("#connect_ssid").val();
    password = $("#connect_password").val();

    if(selectSSID == ""){
        errorList += "<h4 class='rd'> SSID is required </h4>";
        credsOk = false;
    }
    if(password == ""){
        errorList += "<h4 class='rd'> Password is required </h4>";
        credsOk = false;
    }
    if(credsOK == false){
        $("#wifi_connect_credentials_errors").html(errorList);
    }
    else{
        $("#wifi_connect_credentials_errors").html("");
        connectWifi();
    }
}

/**
 * Shows the WiFi password field if the box is checked.
 */
function showPassword(){
    var x = document.getElementById("connect_password");
    if (x.type === "password") {
        x.type = "text";
    } else {
        x.type = "password";
    }
} 