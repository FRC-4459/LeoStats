import game from './game.js';

async function createGames() {
    let teamNum = document.getElementById("teamNum").value;
    let eventCode = document.getElementById("eventCode").value;
    let res = await axios.get("http://localhost:8000");
    let games = [];

    for (let i = 0; i < res.data.length; i++) { 
        let currentGame = new game;
        currentGame.matchTime = res.data[i].actual_time;
        currentGame.matchNumber = res.data[i].match_number;
        currentGame.blue = res.data[i].alliances.blue.team_keys;
        currentGame.red = res.data[i].alliances.red.team_keys;
        currentGame.userTeam = currentGame.isParticipating(teamNum);
        
        games.push(currentGame)
    }

    return games;

};

document.getElementById("submitButton").addEventListener("click", createGames);