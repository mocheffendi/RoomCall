let systems = document.getElementById("systems");

let system = [{}];

let createSystem = async () => {
  systems.innerHTML = "";
  systems.innerHTML += `
    <div class="container2">
        <h2>System Info</h2>
        <!--<p>
          <span class="reading"><span id="uhari">%UPHARI%</span> :</span>
          <span class="reading"><span id="ujam">%UPJAM%</span> :</span>
          <span class="reading"><span id="umenit">%UPMENIT%</span> :</span>
          <span class="reading"><span id="udetik">%UPDETIK%</span> :</span>
          <span class="reading"
            ><span id="umilidetik">%UPMILIDETIK%</span></span
          >
        </p>-->

        <table class="table table-striped">
          <thead>
            <tr>
              <th>Device</th>
              <th>Info</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td><b>NTP Time :</b></td>
              <td>
              <div><p id="ntptime"></p></div>
              <!--${system.Days} Days : ${system.Hours} Hours : ${system.Minutes} Minutes : ${system.Seconds} Seconds : ${system.MilliSeconds} MilliSeconds-->
              </td>
            </tr>
            <tr>
              <td><b>Up Time :</b></td>
              <td>
              <div><p id="mrdiy_value"></p></div>
              <!--${system.Days} Days : ${system.Hours} Hours : ${system.Minutes} Minutes : ${system.Seconds} Seconds : ${system.MilliSeconds} MilliSeconds-->
              </td>
            </tr>
            <tr>
              <td><b>CHIP ID</b></td>
              <td>${system.FlashChipID}</td>
            </tr>
            <tr>
              <td><b>CHIP Frequency</b></td>
              <td>${system.CPUFreqMhz} Mhz</td>
            </tr>
            <tr>
              <td><b>CHIP VCC</b></td>
              <td>${system.VCC} Volt</td>
            </tr>
            <tr>
              <td><b>Chip Real Size</b></td>
              <td>
                <div class="progress">
                  <div class="progress-bar bg-success" role="progressbar" style="width: ${system.ChipRealSize}%;" aria-valuenow="${system.ChipRealSize}" aria-valuemin="0" aria-valuemax="${system.ChipRealSize}">${system.ChipRealSize} Bytes</div>
                </div>
              </td>
            </tr>
            <tr>
              <td><b>Chip Real Size</b></td>
              <td>
              <div class="progress">
                <div class="progress-bar" role="progressbar" style="width: ${system.SketchSizePercent}%" aria-valuenow="${system.SketchSizePercent}" aria-valuemin="0" aria-valuemax="100">SketchSize ${system.SketchSize} Bytes</div>
                <div class="progress-bar bg-success" role="progressbar" style="width: ${system.TotalSizePercent}%" aria-valuenow="${system.TotalSizePercent}" aria-valuemin="0" aria-valuemax="100">LittleFS Size ${system.TotalSize} Bytes</div>
                <div class="progress-bar bg-info" role="progressbar" style="width: ${system.FreeSpacesPercent}%" aria-valuenow="${system.FreeSpacesPercent}" aria-valuemin="0" aria-valuemax="100">Free Space ${system.FreeSpace} Bytes</div>
              </div>
              </td>
            </tr>
            <tr>
              <td><b>Free Space</b></td>
              <td>
                <div class="progress">
                  <div class="progress-bar" role="progressbar" style="width: ${system.FreeSpacesPercent}%" aria-valuenow="${system.FreeSpacesPercent}" aria-valuemin="0" aria-valuemax="100">${system.FreeSpace} Bytes</div>
                </div>
              </td>
            </tr>
            <tr>
              <td><b>Free Heap</b></td>
              <td>${system.FreeHeap} Bytes</td>
            </tr>
            <tr>
              <td><b>Sketch Uses</b></td>
              <td>
                <div class="progress">
                  <div class="progress-bar" role="progressbar" style="width: ${system.SketchSizePercent}%" aria-valuenow="${system.SketchSizePercent}" aria-valuemin="0" aria-valuemax="100">${system.SketchSize} Bytes</div>
                </div>
              </td>
            </tr>
          </tbody>
          <thead>
            <tr>
              <th>LittleFS</th>
              <th>Info</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td><b>LittleFS Total Size</b></td>
              <td>${system.TotalSize} Bytes</td>
            </tr>
            <tr>
              <td><b>LittleFS Used Size</b></td>
              <td>${system.UsedSize} Bytes</td>
            </tr>
          </tbody>
          <thead>
            <tr>
              <th>Network</th>
              <th>Info</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td><b>SSID</b></td>
              <td>${system.WiFiSSID}</td>
            </tr>
            <tr>
              <td><b>IP Address</b></td>
              <td>${system.LocalIP}</td>
            </tr>
            <tr>
              <td><b>Gateway</b></td>
              <td>${system.GateWay}</td>
            </tr>
            <tr>
              <td><b>Subnet Mask</b></td>
              <td>${system.SubnetMask}</td>
            </tr>
            <tr>
              <td><b>DNS</b></td>
              <td>${system.DNSIP}</td>
            </tr>
            <tr>
              <td><b>MAC Address</b></td>
              <td>${system.MACAddressHW}</td>
            </tr>
            <tr>
              <td><b>WiFi Strength</b></td>
              <td>
              <div class="progress">
                <div class="progress-bar bg-success" role="progressbar" style="width: ${system.WiFiStrength}%" aria-valuenow="${system.WiFiStrength}" aria-valuemin="0" aria-valuemax="100%"></div>
              </div>
              ${system.WiFiStrength}%</td>
            </tr>
          </tbody>
        </table>
      </div>
    `;

}

(async () => {
  try {
    // console.log('try to get data');
    const response = await fetch('/system');
    system = await response.json();

  } catch (e) {

    console.log('Error: ' + e.message);
  }

  console.log(system);
  createSystem();
})()

socket = new WebSocket("ws:/" + "/" + location.host + ":81");
// socket = new WebSocket("ws:/192.168.0.18:81");
socket.onopen = function (e) {
  // console.log("[socket] socket.onopen "); 
};
socket.onerror = function (e) {
  // console.log("[socket] socket.onerror "); 
};
socket.onmessage = function (e) {
  // console.log("[socket] " + e.data);
  document.getElementById("mrdiy_value").innerHTML = e.data;
};

socket2 = new WebSocket("ws:/" + "/" + location.host + ":82");
// socket = new WebSocket("ws:/192.168.0.18:81");
socket2.onopen = function (e) {
  // console.log("[socket] socket.onopen "); 
};
socket2.onerror = function (e) {
  // console.log("[socket] socket.onerror "); 
};
socket2.onmessage = function (e) {
  // console.log("[socket] " + e.data);
  document.getElementById("ntptime").innerHTML = e.data;
};
