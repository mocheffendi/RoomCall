// socket = new WebSocket("ws:/" + "/" + location.host + ":81");
socket = new WebSocket("ws:/192.168.0.18:81");
socket.onopen = function (e) { console.log("[socket] socket.onopen "); };
socket.onerror = function (e) { console.log("[socket] socket.onerror "); };
socket.onmessage = function (e) {
    console.log("[socket] " + e.data);
    document.getElementById("mrdiy_value").innerHTML = e.data;
};
