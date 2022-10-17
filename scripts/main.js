require.config({
    "packages": ["axios", "game"],
    "paths": 
    {
        axios: "/home/kingstonv/projects/LeoWeb/node_modules/axios/index",
        game: "/home/kingstonv/projects/LeoWeb/scripts/game"
    }
});

define(function (axios, game) {
    var Axios = require('axios');
    var game = require('./game');
});

var Axios = require(['axios'], function ());


Axios.get('https://api.sampleapis.com/switch/games').then((res) => console.log(res)).catch((err) => console.err(err));