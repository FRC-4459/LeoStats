class game 
{
    matchTime = 0;
    userTeam = "none";
    blue = [0, 0, 0];
    red = [0, 0, 0];
    description = 'N/A';

    isParticipating(teamNum) {
        for (let i = 0; i < 2; i++) { 
            if (this.blue[i] === parseInt(teamNum)) 
                { return "blue"; }
            if (this.red[i] === parseInt(teamNum)) 
                { return "red"; }
        }

        return "none";
    }

    print()
        { console.log(this); }

}

export default game;