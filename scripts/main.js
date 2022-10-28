import game from './game.js';

//Recompile this. Do it again.
async function createGames() {
    
    
    let teamNum = document.getElementById("teamNum").value;
    let eventCode = document.getElementById("eventCode").value;
    let res = await axios.get(`http://localhost:8000/events/byTeam?teamNum=${teamNum}&eventCode=${eventCode}`);
    let games = [];

    for (let i = 0; i < res.data.length; i++) { 
        let currentGame = new game;
        currentGame.matchTime = res.data[i].autoStartTime;
        currentGame.matchNumber = res.data[i].matchNumber;
        
        for (let l = 0; l < res.data[i].teams.length; l++) {
            if (l < 3) { 
                currentGame.red[l] = res.data[i].teams[l].teamNumber }
            else { 
                currentGame.blue[l - 3]  = res.data[i].teams[l].teamNumber }
        }
        
        currentGame.userTeam = currentGame.isParticipating(teamNum);
        games.push(currentGame)
    }
    
    console.log(games);
    console.log(`Got ${games.length} games from ${eventCode}.`)
    return games;


};

document.getElementById("submitButton").addEventListener("click", createGames);