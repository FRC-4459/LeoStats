const http = require("http");
const axios = require("axios");
require("dotenv").config()

const host = 'localhost';
const port = 8000;
let count = 0;

const headers = {headers: {"X-TBA-Auth-Key":process.env.authkey, "accept":"application/json"}}

const requestListener = async function (req, res) 
{
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PATCH, PUT, DELETE, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Origin, Content-Type, X-TBA-Auth-Key, accept, Access-Control-Allow-Origin');
    res.writeHead(200, { 'Content-Type': 'application/json' });

    let r = await axios.get("https://www.thebluealliance.com/api/v3/team/frc4459/event/2022gacar/matches/simple", headers);
    res.end(JSON.stringify(r.data));
    count++;
    console.log(`Request #${count} Fulfilled.`)
};

const server = http.createServer(requestListener);
server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});
