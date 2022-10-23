class game 
{
    matchNumber = 0;
    matchTime = 0;
    userTeam = "none";
    blue = [0, 0, 0];
    red = [0, 0, 0];
    matchType = 'N/A';

    isParticipating(teamNum) {
        for (let i = 0; i < 2; i++) { 
            if (blue[i].search(teamNum) != -1) 
                { return "blue"; }
            if (red[i].search(teamNum) != -1) 
                { return "red"; }
        }

        return "none";
    }

}

export default game;