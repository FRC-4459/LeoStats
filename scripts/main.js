import Axios from 'axios';
import game from './game.js';

Axios.get('https://api.sampleapis.com/switch/games').then((res) => console.log(res)).catch((err) => console.err(err));