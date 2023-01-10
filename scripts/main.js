/* global axios */
import game from "./game.js";

let gameTemp = $(gameTempl);
let infoArea = $(infoAreaDiv);

//Used to shave the milliseconds off of game times
const msRE = /\.[\d]*/
//Used to take the timezone off of the date string after it's parsed

const dtRE = /:[\d]{2}/

function sortGames(games) {
	let qualifiers = [];
	let quarterfinals = [];
	let semifinals = [];
	let finals = [];

	for (let i = 0; i < games.length; i++) {
		const split = games[i].description.split(" ");
        
		switch (split[0]) {
		case "Qualification":
			qualifiers.push(games[i]);
			break;
		case "Quarterfinal":
			quarterfinals.push(games[i]);
			break;
		case "Semifinal":
			semifinals.push(games[i]);
			break;
		case "Final":
			finals.push(games[i]);
			break;
		default:
			break;
		}

	}

	let qq = qualifiers.concat(quarterfinals);
	let sf = semifinals.concat(finals);
	games = qq.concat(sf);

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
		currentGame.startTime = res.data[i].actualStartTime;
        
		try {
			currentGame.startTime = currentGame.startTime.replace(msRE, "");
			currentGame.startTime = Date.parse(currentGame.startTime).toString();
			let match = dtRE.exec(currentGame.startTime);
			currentGame.startTime.slice(match.index);
		} catch {
			continue;
		}
		
		for (let l = 0; l < res.data[i].teams.length; l++) {
        
			if (l < 3) 
			{ currentGame.red[l] = res.data[i].teams[l].teamNumber; } 
			else 
			{ currentGame.blue[l - 3]  = res.data[i].teams[l].teamNumber; }
		}
        
		games.push(currentGame);
	}
    
	console.log(`Got ${games.length} games from ${frcCode}.`);
	games = sortGames(games);

	for (let i = 0; i < games.length; i++) 
	{ renderGameDiv(games[i], frcNum); }

	console.log("Rendered Divs.");
	return games;
}

function renderGameDiv(game, frcNum) {
	let newGame = $(gameTemp).clone();
	let gameContainer = $(newGame.contents()[1]);
	let gameHead = $(gameContainer.contents()[1]);
	let blueTeam = $(gameContainer.contents()[3]);
	let redTeam = $(gameContainer.contents()[5]);

    
	for (let i = 0; i < 3; i++) {
		//The anchor elements we want occur at 1, 5, and 9 in the parent element,
		//So we must multiply the index by 4, then add 1.
		$(blueTeam.contents()[(i * 4) + 1]).text(game.blue[i]);
		$(blueTeam.contents()[(i * 4) + 1]).attr("href", `https://www.thebluealliance.com/team/${game.blue[i]}`);
		$(redTeam.contents()[(i * 4) + 1]).text(game.red[i]);
		$(redTeam.contents()[(i * 4) + 1]).attr("href", `https://www.thebluealliance.com/team/${game.red[i]}`);
		
		//If the team is your team, highlight the number
		
		if (game.blue[i] == frcNum) {
			$(blueTeam.contents()[(i * 4) + 1]).css("font-weight", "800");
		}
		if (game.red[i] == frcNum) {
			$(redTeam.contents()[(i * 4) + 1]).css("font-weight", "800");
		}

		$(gameHead.contents()[0]).text(game.description);
	}

	infoArea.append(gameContainer.get(0));
}

$(submitButton).on("click", createGames);

//Renders the games if the user hits "enter" in the code input field
$(eventCode).on("keydown", (e) => {
	if (e.code == "Enter") {
		createGames();
	}
});