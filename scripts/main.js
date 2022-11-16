import game from './game.js';

let gameTemp = $(gameTempl);
let infoArea = $(infoAreaDiv);

function sortGames(games) {





    return games;
}

async function createGames() {
    let frcNum = $(teamNum).val();
    let frcCode = $(eventCode).val();
    let res = await axios.get(`http://localhost:8000/events?teamNum=${frcNum}&eventCode=${frcCode}`);
    let games = [];
    
    for (let i = 0; i < res.data.length; i++) { 
        let currentGame = new game;
        currentGame.matchTime = res.data[i].autoStartTime;
        currentGame.matchNumber = res.data[i].matchNumber;
        currentGame.description = res.data[i].description;
        
        for (let l = 0; l < res.data[i].teams.length; l++) {
        
            if (l < 3) 
                { currentGame.red[l] = res.data[i].teams[l].teamNumber }
            
            else 
                { currentGame.blue[l - 3]  = res.data[i].teams[l].teamNumber }
        }
        
        currentGame.userTeam = currentGame.isParticipating(frcNum);
        games.push(currentGame);
    }
            
    console.log(`Got ${games.length} games from ${frcCode}.`)
    
    games = sortGames(games);

    console.log(`Sorthed ${games.length} games.`)

    for (let i = 0; i < games.length; i++) 
        { renderGameDiv(games[i]); console.log(games[i].description); }

    console.log("Rendered Divs.");
    return games;
};

function renderGameDiv(game) {
    let newGame = $(gameTemp).clone();
    let gameContainer = $(newGame.contents()[1]);
    let blueTeam = $(gameContainer.contents()[3]);
    let redTeam = $(gameContainer.contents()[5]);

    
    for (let i = 0; i < 3; i++) {
        //The h1 elements we want occur at 1, 5, and 9 in the parent element,
        //So we must multiply the index by 4, then add 1.
        $(blueTeam.contents()[(i * 4) + 1]).text(game.blue[i]);
        $(redTeam.contents()[(i * 4) + 1]).text(game.red[i]);
    }

    infoArea.append(gameContainer.get(0));
}

$(submitButton).on("click", createGames);
let newGame = $(gameTemp).clone();
let gameDiv = $(newGame.contents()[1])
infoArea.append(gameDiv.get(0));