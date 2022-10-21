const http = require("http");
const axios = require("axios");
const fs = require("fs");
require("dotenv").config()

const host = 'localhost';
const port = 8000;

const headers = {headers: {"X-TBA-Auth-Key":process.env.authkey, "accept":"application/json"}}

const requestListener = async function (req, res) 
{
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PATCH, PUT, DELETE, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Origin, Content-Type, X-TBA-Auth-Key, accept, Access-Control-Allow-Origin');
    res.writeHead(200);
    
    let output = "";

    let r = await axios.get("https://www.thebluealliance.com/api/v3/team/frc4459/event/2022gacar/matches/simple", headers);
    
    //Data is an array of JSON objects; iterate through it.
    for (let i = 0; i < r.data.length; i++) 
        { output += JSON.stringify(r.data[i], null, 4); }
    
    res.end(output)
};

const server = http.createServer(requestListener);
server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});

