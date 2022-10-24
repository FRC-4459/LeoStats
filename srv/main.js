const http = require("http");
const axios = require("axios");
const url = require("url");
require("dotenv").config()

const host = 'localhost';
const port = 8000;
let count = 0;

const headers = {headers: {"Authorization": `Basic ${process.env.auth}`}};
const requestListener = async function (req, res) 
{
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PATCH, PUT, DELETE, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Origin, Content-Type, X-TBA-Auth-Key, accept, Access-Control-Allow-Origin');
    res.writeHead(200, { 'Content-Type': 'application/json' });

    let headers = {"Authorization":`Basic ${process.env.auth}`};
    let r = await axios.get('https://frc-api.firstinspires.org/v3.0/2020', {headers: {"Authorization": `Basic ${process.env.auth}`}}).catch(function (error) {console.log(error.message)});

    console.log(r.data)
    // res.end(JSON.stringify(r.data));
    // count++;
    // console.log(`Request #${count} Fulfilled.`);
    
    // let parts = url.parse(req.url)
    // console.log(`A new request from ${parts.path}!`)
    // console.log(`Path: ${parts.pathname}`)
    res.end("Hello!");
};

const server = http.createServer(requestListener);
server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});

