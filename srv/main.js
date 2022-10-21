const http = require("http");
const axios = require("axios");
const fs = require("fs");
require("dotenv").config()

const host = 'localhost';
const port = 8000;

const headers = {headers: {"X-TBA-Auth-Key":process.env.authkey}}

const requestListener = async function (req, res) 
{
    let output = "";
    res.setHeader("Content-Type", "application/json");
    res.writeHead(200);

    let r = await axios.get("https://www.thebluealliance.com/api/v3/team/frc4459/event/2022gacar/matches/simple", headers);
    
    //Data is an array of JSON objects; iterate through it.
    for (let i = 0; i < r.data.length; i++) 
    {
        output += JSON.stringify(r.data[i], null, 4);
    }
    console.log(`Data: ${output}`)
    res.end("\n" + output + "\n")
};

const server = http.createServer(requestListener);
server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});

