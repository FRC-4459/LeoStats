const http = require("http");
const axios = require("axios");
const url = require("url");
const fs = require('fs')
require("dotenv").config();

const port = 8000;
let count = 0;

function cleanUp (params) {
	count++;
	console.log(`Fulfilled request #${count} of type ${params.pathname}.`);
}

const requestListener = async function (req, res)
{
	res.setHeader("Access-Control-Allow-Origin", "*");
	res.setHeader("Access-Control-Allow-Methods", "GET, POST, PATCH, PUT, DELETE, OPTIONS");
	res.setHeader("Access-Control-Allow-Headers", "Origin, Content-Type, X-TBA-Auth-Key, accept, Access-Control-Allow-Origin");
	res.setHeader("Content-Type", "application/json");
    
	
	let params = url.parse(req.url, true);
	
	try {
		switch (params.pathname) {
		case "/events": {
			const headers = {headers: {"Authorization": `Basic ${process.env.auth}`}};
            
			let season = params.query.eventCode.substring(0, 4);
			let eventCode = params.query.eventCode.substring(4, params.query.eventCode.substring.end);
            
			let r = await axios.get(
				`https://frc-api.firstinspires.org/v3.0/${season}/matches/${eventCode.toUpperCase()}`, headers)
				.catch(function (error) {console.log(error.message);});

			//r is an object of json objects; convert it to a string
			let out = "[";
			for (let i = 0; i < r.data["Matches"].length; i++) {
				out += JSON.stringify(r.data["Matches"][i]);
				out += ",";
			}
            
			//Remove an unneccesary last comma
			out = out.slice(0, -1);
			out += "]";

			cleanUp(params);
			res.writeHead(200);
			res.end(out);
			break;
		}

		case "/": {
			console.log("Got request to base url.");
			res.writeHead(200, {'content-type': 'text/html'});
			fs.createReadStream('../index.html').pipe(res);
			break;
		}

		default: {
			res.writeHead(400);
			res.end("Invalid request URL.");
			break;
		}
		}
	} catch (err) {console.log(err);}
};

const server = http.createServer(requestListener);
server.listen(port, () => {
	console.log(`Server is running on port ${port}`);
});

