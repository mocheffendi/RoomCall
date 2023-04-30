let form = document.getElementById("form");
let textInput = document.getElementById("textInput");
let dateInput = document.getElementById("dateInput");
let textareaInput = document.getElementById("textareaInput");
let msg = document.getElementById("msg");
let tasks = document.getElementById("tasks");
let add = document.getElementById("add");

form.addEventListener("submit", async (e) => {
    e.preventDefault();
    formValidation();
});

let formValidation = () => {
    if (textInput.value === "") {
        console.log("Validation Failed");
        msg.innerHTML = "Task cannot be blank.";

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
        text: textInput.value,
        date: dateInput.value,
        description: textareaInput.value,
    });

    localStorage.setItem("data", JSON.stringify(data));
    console.log('Data on localStorage saved successfully');

    try {
        const response = await fetch('http://192.168.175.146/post-data', {
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

let createTasks = () => {
    tasks.innerHTML = "";
    data.map((x, y) => {
        return (tasks.innerHTML += `
        <div id=${y}>
              <span class="fw-bold">${x.text}</span>
              <span class="small text-secondary">${x.date}</span>
              <p>${x.description}</p>
    
              <span class="options">
                <i onClick="editTask(this)" data-bs-toggle="modal" data-bs-target="#form" class="fas fa-edit"></i>
                <i onClick="deleteTask(this); createTasks()" class="fas fa-trash-alt"></i>
              </span>
            </div>
        `)
    })

    resetForm();
}

let deleteTask = async (e) => {
    e.parentElement.parentElement.remove();
    data.splice(e.parentElement.parentElement.id, 1);

    try {
        const response = await fetch('http://192.168.175.146/post-data', {
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

    textInput.value = selectedTask.children[0].innerHTML;
    dateInput.value = selectedTask.children[1].innerHTML;
    textareaInput.value = selectedTask.children[2].innerHTML;

    deleteTask(e);
}

let resetForm = () => {
    textInput.value = '';
    dateInput.value = '';
    textareaInput.value = '';
}

(async () => {
    try {
        // console.log('try to get data');
        const response = await fetch('http://192.168.175.146/get-data');
        data = await response.json();

    } catch (e) {

        console.log('Error: ' + e.message);
    }
    //data = JSON.parse(localStorage.getItem("data")) || [];
    console.log(data);
    createTasks();
})()