let sidebar = document.querySelector(".sidebar");
let closeBtn = document.querySelector("#btn");
let searchBtn = document.querySelector(".bx-search");

closeBtn.addEventListener("click", () => {
  sidebar.classList.toggle("open");
  menuBtnChange();//calling the function(optional)
});

// searchBtn.addEventListener("click", () => { // Sidebar open when you click on the search iocn
//   sidebar.classList.toggle("open");
//   menuBtnChange(); //calling the function(optional)
// });

// following are the code to change sidebar button(optional)
function menuBtnChange() {
  if (sidebar.classList.contains("open")) {
    closeBtn.classList.replace("bx-menu", "bx-menu-alt-right");//replacing the iocns class
  } else {
    closeBtn.classList.replace("bx-menu-alt-right", "bx-menu");//replacing the iocns class
  }
}

let dashboard = document.getElementById("dashboard");

let createDashboard = async () => {
  dashboard.innerHTML += `
    <table class="table table-striped">
          <thead>
            <tr>
              <th>Device</th>
              <th>Info</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>CHIP ID</td>
              <td>${data.FlashChipID}</td>
            </tr>
            <tr>
              <td>CHIP Frequency</td>
              <td>${data.CPUFreqMhz} Mhz</td>
            </tr>
            <tr>
              <td>CHIP VCC</td>
              <td>${data.VCC} Volt</td>
            </tr>
            <tr>
              <td>Heap Uses %</td>
              <td>
                <div class="progress">
                  <div
                    class="progress-bar progress-bar-success"
                    role="progressbar"
                    aria-valuenow="80"
                    aria-valuemin="0"
                    aria-valuemax="100"
                    style="width: 80%"
                  >
                  ${data.FreeHeap} Bytes
                  </div>
                </div>
              </td>
            </tr>
            <tr>
              <td>Free Heap</td>
              <td>
                <div class="progress">
                  <div
                    class="progress-bar progress-bar-danger"
                    role="progressbar"
                    aria-valuenow="90"
                    aria-valuemin="0"
                    aria-valuemax="100"
                    style="width: 90%"
                  >
                  ${data.FreeHeap} Bytes
                  </div>
                </div>
              </td>
            </tr>
            <tr>
              <td>Sketch Uses</td>
              <td>
                <div class="progress">
                  <div
                    class="progress-bar progress-bar-success"
                    role="progressbar"
                    aria-valuenow="70"
                    aria-valuemin="0"
                    aria-valuemax="100"
                    style="width: 70%"
                  >
                  ${data.SketchSize} Bytes
                  </div>
                </div>
              </td>
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
              <td>SSID</td>
              <td>${data.WiFiSSID}</td>
            </tr>
            <tr>
              <td>IP Address</td>
              <td>${data.LocalIP}</td>
            </tr>
            <tr>
              <td>Gateway</td>
              <td>${data.GateWay}</td>
            </tr>
            <tr>
              <td>Mask</td>
              <td>${data.SubnetMask}</td>
            </tr>
            <tr>
              <td>DNS</td>
              <td>${data.DNSIP}</td>
            </tr>
            <tr>
              <td>MAC</td>
              <td>${data.MACAddressHW}</td>
            </tr>
          </tbody>
        </table >
    `
}


(async () => {
  try {
    // console.log('try to get data');
    const response = await fetch('http://192.168.0.4/system');
    data = await response.json();

  } catch (e) {

    console.log('Error: ' + e.message);
  }
  //data = JSON.parse(localStorage.getItem("data")) || [];
  console.log(data);
  createDashboard();
})()


