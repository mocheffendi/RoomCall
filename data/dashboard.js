let form = document.getElementById("form");
let roomnameInput = document.getElementById("roomnameInput");
let bednameInput = document.getElementById("bednameInput");
let statusInput = document.getElementById("statusInput");
let msg = document.getElementById("msg");
let tasks = document.getElementById("tasks");
let add = document.getElementById("add");


// form.addEventListener("submit", async (e) => {
//     e.preventDefault();
//     formValidation();
// });

let formValidation = () => {
    if (roomnameInput.value === "") {
        console.log("Validation Failed");
        msg.innerHTML = "Bed cannot be blank.";

    } else {
        console.log("Validation Success");
        msg.innerHTML = "";
        acceptData();
        add.setAttribute("data-bs-dismiss", "modal");
        add.click();

        (() => {
            add.setAttribute("data-bs-dismiss", "");
        })()
    }
}

let data = [{}];

let acceptData = async () => {
    data.push({
        roomname: roomnameInput.value,
        bedname: bednameInput.value,
        status: statusInput.value,
    });

    data.sort(function (a, b) {
        return a.bedname > b.bedname ? 1 : (a.bedname === b.bedname ? 0 : -1);
    });

    data.sort(function (a, b) {
        // return a.roomname.split(",")[0] - b.roomname.split(",")[0];
        return a.roomname > b.roomname ? 1 : (a.roomname === b.roomname ? 0 : -1);
    });

    console.log("sorting");
    console.log(JSON.stringify(data));

    localStorage.setItem("data", JSON.stringify(data));
    console.log('Data on localStorage saved successfully');

    try {
        const response = await fetch('/create', {
            method: 'POST',
            headers: {
                'Content-Type': 'text/plain'
            },
            body: JSON.stringify(data)
        });

        // const dataku = await response.json();
        const res = await response.text(); //sebab bukan json tapi text 
        console.log(res);

    } catch (e) {
        console.log('Error: ' + e.message);
    }

    console.log(data);

    createTasks();

}

let counter = 0;
let reloadInterval = 3000;

let createTasks = async () => {
    tasks.innerHTML = "";
    data.map((x, y) => {
        if (x.status === true) {
            counter++;
            console.log(counter);
            reloadInterval = 3000 * counter;

            console.log(reloadInterval);
            // let voice = new SpeechSynthesisUtterance();
            // const voices = window.speechSynthesis.getVoices();
            // console.log(voices);
            // voice.voice = voices[1];
            // voice.text = x.roomname + x.bedname;
            // voice.lang = 'id-ID';
            // window.speechSynthesis.speak(voice);
            // Create a new SpeechSynthesisUtterance object
            // const utterance = new SpeechSynthesisUtterance();

            // Set the text to be spoken
            // utterance.text = "Halo, apa kabar?";

            // Set the language to Indonesian
            // utterance.lang = "id-ID";

            // Set the voice
            // utterance.voice = /* Optional: set the desired voice */ ;

            // Call the speak() method to start speaking
            // window.speechSynthesis.speak(utterance);

            // responsiveVoice.speak(
            //     "Hey nama saya cahya, kang cahya dot kom merupakan web personal saya, terimakasih telah berkunjung kemari.",
            //     "Indonesian Female",
            //     {
            //         pitch: 1,
            //         rate: 1,
            //         volume: 1,
            //         lang: 'indonesian'
            //     }
            // );

            // Check if speech synthesis is supported
            // if ('speechSynthesis' in window) {
            //     // Create an instance of SpeechSynthesisUtterance
            //     const msg = new SpeechSynthesisUtterance();
            //     msg.lang = 'id-ID'; // set the language to Indonesian

            //     // Set the text to be spoken
            //     msg.text = 'Halo, dunia!';

            //     // Speak the text
            //     window.speechSynthesis.speak(msg);
            // }

            let speech = new SpeechSynthesisUtterance();
            speech.lang = "id-ID";
            let voices = [];

            // try {
            //     voices = window.speechSynthesis.getVoices();
            //     console.log("All voices:", voices);

            // } catch (e) {
            //     console.log('Error: ' + e.message);
            // }
            // voices = window.speechSynthesis.getVoices();
            speech.lang = "id-ID";
            speech.voiceURI = "Google Bahasa Indonesia";
            speech.name = "Google Bahasa Indonesia";
            // speech.voice = voices[160];
            speech.text = x.roomname + x.bedname;
            speech.pitch = 1;
            speech.volume = 1;
            speech.rate = 1;
            // window.speechSynthesis.cancel();
            window.speechSynthesis.speak(speech);
            console.log("harus e sudah play id: " + y);
            // const allVoicesObtained = new Promise(function (resolve, reject) {
            //     let voices = window.speechSynthesis.getVoices();
            //     if (voices.length !== 0) {
            //         resolve(voices);
            //     } else {
            //         window.speechSynthesis.addEventListener("voiceschanged", function () {
            //             voices = window.speechSynthesis.getVoices();
            //             resolve(voices);
            //         });
            //     }
            // });


            // allVoicesObtained.then(voices => {
            //     console.log("All voices:", voices);
            //     speech.lang = "id-ID";
            //     speech.voiceURI = "Google Bahasa Indonesia";
            //     speech.name = "Google Bahasa Indonesia";
            //     speech.voice = voices[160];
            //     speech.text = x.roomname + x.bedname;
            //     speech.pitch = 1;
            //     speech.volume = 1;
            //     speech.rate = 1;
            //     window.speechSynthesis.cancel();
            //     window.speechSynthesis.speak(speech);
            //     console.log("harus e sudah play");
            // });

            return (tasks.innerHTML += `
            <div id=blink>
            <div id=${y}>
                  <span class="fw-bold">${x.roomname}</span>
                  <span class="small text-secondary">${x.bedname}</span>
                  <p>${x.status}</p>
        
                  <span class="options">
                    <!--  
                    <i onClick="editTask(this)" data-bs-toggle="modal" data-bs-target="#form" class="fas fa-edit"></i>
                    <i onClick="deleteTask(this); createTasks()" class="fas fa-trash-alt"></i>
                    -->
                    <i onClick="cancelTask(this)" class="bi bi-check-circle-fill"></i>
                  </span>
                </div>
            </div>
            `)
        } else {
            return (tasks.innerHTML += `
            <div id=${y}>
                  <span class="fw-bold">${x.roomname}</span>
                  <span class="small text-secondary">${x.bedname}</span>
                  <p>${x.status}</p>
        
                  <span class="options">
                    <!--
                    <i onClick="editTask(this)" data-bs-toggle="modal" data-bs-target="#form" class="fas fa-edit"></i>
                    <i onClick="deleteTask(this); createTasks()" class="fas fa-trash-alt"></i>
                    
                    <i onClick="cancelTask(this)" class="bi bi-bookmark-check"></i>
                    -->
                  </span>
                </div>
            `)
        }

    })

    // setTimeout(() => {
    //     // document.location.reload();
    //     // $("#app").load(window.location.href + " #app");
    //     // $("#tasks").load(" #tasks > *");
    //     $("#container-fluid").load(" #container-fluid > *");
    //     createTasks();
    // }, reloadInterval);
    // resetForm();
}



setInterval(async function () {

    try {
        // console.log('try to get data');
        const response = await fetch('/get');
        data = await response.json();

    } catch (e) {

        console.log('Error: ' + e.message);
    }
    //data = JSON.parse(localStorage.getItem("data")) || [];
    console.log(data);
    createTasks();

    // $("#container-fluid").load(" #container-fluid > *");
    console.log("sudah load");
}, 3000);


let getTasks = async () => {
    try {
        // console.log('try to get data');
        const response = await fetch('/get');
        data = await response.json();

    } catch (e) {

        console.log('Error: ' + e.message);
    }
    //data = JSON.parse(localStorage.getItem("data")) || [];
    console.log(data);
    createTasks();
}

let cancelTask = async (e) => {
    try {

        const id = e.parentElement.parentElement.id;
        console.log(id);
        const url = "/call?id=" + id + "&status=0";
        const response = await fetch(url);

        // const dataku = await response.json();
        const res = await response.text(); //sebab bukan json tapi text 
        console.log(res);

        // createTasks();
        // $("#tasks").load(window.location.href + " #tasks");
        // $("#tasks").load(" #tasks > *");
        // $("#app").load(window.location.href + " #app");
        // document.location.reload();
        getTasks();

    } catch (e) {
        console.log('Error: ' + e.message);
    }

}

let deleteTask = async (e) => {
    e.parentElement.parentElement.remove();
    data.splice(e.parentElement.parentElement.id, 1);

    try {
        const response = await fetch('/create', {
            method: 'POST',
            headers: {
                'Content-Type': 'text/plain'
            },
            body: JSON.stringify(data)
        });

        // const dataku = await response.json();
        const res = await response.text(); //sebab bukan json tapi text 
        console.log(res);

    } catch (e) {
        console.log('Error: ' + e.message);
    }

    localStorage.setItem("data", JSON.stringify(data));
    console.log(e.parentElement.parentElement.id);
}

let editTask = (e) => {
    let selectedTask = e.parentElement.parentElement;

    roomnameInput.value = selectedTask.children[0].innerHTML;
    bednameInput.value = selectedTask.children[1].innerHTML;
    statusInput.value = selectedTask.children[2].innerHTML;

    deleteTask(e);
}

// let resetForm = () => {
//     roomnameInput.value = '';
//     bednameInput.value = '';
//     statusInput.value = '';
// }


(async () => {
    try {
        // console.log('try to get data');
        const response = await fetch('/get');
        data = await response.json();

    } catch (e) {

        console.log('Error: ' + e.message);
    }
    //data = JSON.parse(localStorage.getItem("data")) || [];
    console.log(data);
    createTasks();

})()
