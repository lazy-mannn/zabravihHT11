let totalTime = 299; 
let timerInterval;
function startTimer() {
    var button = document.getElementById("timer-button")
    button.style.display="none"
    var title = document.getElementById("title")
    var timer =document.getElementById("timer")
    title.textContent="Time for a break";
    timer.style.color="#00cc66";
    if (timerInterval) {
        clearInterval(timerInterval); 
    }
    
    timerInterval = setInterval(function() {
        let minutes = Math.floor(totalTime / 60);
        let seconds = totalTime % 60;
        minutes = minutes < 10 ? "0" + minutes : minutes;
        seconds = seconds < 10 ? "0" + seconds : seconds;
        document.getElementById("timer").textContent = `${minutes}:${seconds}`;
        totalTime--;
        if (totalTime < 0) {
            clearInterval(timerInterval);
            alert("Time's up!");
        }
    }, 1000); }