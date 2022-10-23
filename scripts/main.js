async function createGame() {
    let teamNum = document.getElementById("teamNum").value;
    let eventCode = document.getElementById("eventCode").value;
    let res = await axios.get("http://localhost:8000");
    
};

//console.log("Actual Time: " + res.data[0].actual_time)

document.getElementById("submitButton").addEventListener("click", createGame);