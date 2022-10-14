import { get } from 'axios.js';
import { game } from 'game.js';

get('https://api.sampleapis.com/switch/games').then((res) => console.log(res)).catch((err) => console.err(err));