import game from './game.js';

let gameTemp = document.getElementById("gameTemp");
let infoArea = document.getElementById("infoArea");


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
                games.push(currentGame);
            }
            
            console.log(`Got ${games.length} games from ${eventCode}.`)
            
            for (let i = 0; i < games.length; i++) {
                renderGameDiv(games[i]);
            }
            console.log("Rendered Divs.");
            return games;
        };

function renderGameDiv(game) {
    let newGame = gameTemp.content.cloneNode(true);
    let gameDiv = newGame.childNodes.item(1);
    let blueTeam = gameDiv.childNodes.item(1);
    //let redTeam = gameDiv.childNodes.item(5);

    if (game.isParticipating === "blue") {
        
    }
    
    for (let i = 0; i < 3; i++) {
        blueTeam.childNodes.item((4 * i) + 1).innerText = game.blue[i];
        //redTeam.childNodes.item((4 * i) + 1).innerText = game.red[i];
    }

    infoArea.appendChild(newGame);
}

        document.getElementById("submitButton").addEventListener("click", createGames);