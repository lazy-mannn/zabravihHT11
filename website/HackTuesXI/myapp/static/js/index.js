function formatTime(seconds) {
    const min = Math.floor(seconds / 60);
    const sec = seconds % 60;
    console.log(`Formatting time: ${seconds} seconds -> ${min}:${sec}`);
    return `${min.toString().padStart(2, '0')}:${sec.toString().padStart(2, '0')}`;
}

function updateUI(data) {
    console.log('Updating UI with data:', data);
    document.getElementById("session-status").innerText = data.status;
    const remaining = data.status === "WORK_IN_PROGRESS" || data.status === "PAUSED"
        ? data.work_time
        : data.status === "REST"
            ? data.break_time
            : 0;
    console.log(`Remaining time: ${remaining} seconds`);

    const snooze = data.snooze_time;
    
    //document.getElementById("snz-timer").style.display = "none";
    //document.getElementById("snz-timer").innerText = formatTime(snooze);
    if (remaining === 0 && (data.status === "REST")) {
        document.getElementById("start-btn").style.display = "block";
        document.getElementById("timer").style.display = "none";
        document.getElementById("status-cont").style.display = "none";
    } else if (data.status === "INITIAL") {
        document.getElementById("start-btn").style.display = "block";
        document.getElementById("timer").style.display = "none";
        document.getElementById("status-cont").style.display = "none";
    } else if (data.status === "PAUSED") {
        document.getElementById("start-btn").style.display = "none";
        document.getElementById("timer").style.display = "block";
        document.getElementById("timer").innerText = formatTime(remaining);
        document.getElementById("status-cont").style.display = "block";
      //  document.getElementById("snz-timer").style.display = "block";
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
