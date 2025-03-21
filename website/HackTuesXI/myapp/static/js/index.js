function formatTime(seconds) {
    const min = Math.floor(seconds / 60);
    const sec = seconds % 60;
    console.log(`Formatting time: ${seconds} seconds -> ${min}:${sec}`);
    return `${min.toString().padStart(2, '0')}:${sec.toString().padStart(2, '0')}`;
}

function updateUI(data) {
    console.log('Updating UI with data:', data);
    document.getElementById("session-status").innerText = data.status;
    const remaining = data.status === "Work in progress"
        ? data.work_time
        : data.status === "Paused" || data.status === "Break"
            ? data.break_time
            : 0;
    console.log(`Remaining time: ${remaining} seconds`);
    
    if (remaining === 0 && (data.status === "Break")) {
        document.getElementById("start-btn").style.display = "block";
        document.getElementById("timer").style.display = "none";
        document.getElementById("status-cont").style.display = "none";
    } else if (data.status === "Initial") {
        document.getElementById("start-btn").style.display = "block";
        document.getElementById("timer").style.display = "none";
        document.getElementById("status-cont").style.display = "none";
    } else {
        document.getElementById("start-btn").style.display = "none";
        document.getElementById("timer").style.display = "block";
        document.getElementById("timer").innerText = formatTime(remaining);
        document.getElementById("status-cont").style.display = "block";
    }
}

function fetchStatus() {
    console.log('Fetching status...');
    fetch('/get-status/')
        .then(res => res.json())
        .then(data => {
            console.log('Received data:', data);
            updateUI(data);
        });
}

function startWork() {
    console.log('Starting work...');
    document.getElementById("start-btn").style.display = "none";
    fetch('/start-work/')
        .then(() => {
            console.log('Work started, fetching status...');
            fetchStatus();
        });
}

setInterval(fetchStatus, 1000);
window.onload = fetchStatus;
