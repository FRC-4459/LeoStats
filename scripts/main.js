async function createGame() {
    // let teamNum = document.getElementById("teamNum").value;
    // let eventCode = document.getElementById("eventCode").value;
    // const eventsByTeam = "https://www.thebluealliance.com/api/v3/event/" + eventCode + "/matches/simple";
    // let res = await axios.get(eventsByTeam);
    // console.log(res);
};

async function requestData() {
    let res = await axios.get("http://localhost:8000")
    console.log()
    res = JSON.parse(res.data);
    console.log(res)
};

requestData();

document.getElementById("submitButton").addEventListener("click", createGame);