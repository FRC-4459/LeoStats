import os
import requests as HTTPRequests
from dotenv import load_dotenv
from LeoSrv import LeoStats
from flask import request, Response, send_from_directory

load_dotenv()

headers = {
    'Authorization': f'Basic {os.getenv("FRC_KEY")}',
    'If-Modified-Since': ''
        }


@LeoStats.route('/')
def index():
    return send_from_directory('../../site/markup/', 'index.html')



@LeoStats.route('/gamesByEvent')
def gamesByEvent():
    eventCode = request.args.get('event')
    
    if eventCode == None:
        return Response('You must include an event code in your querystring.', 400)
    
    res = HTTPRequests.get(f'https://frc-api.firstinspires.org/v3.0/2022/matches/{eventCode}', headers=headers)
    print(f'FRC Request returned {res.status_code} - {res.reason}')

    return Response(res.text, 200)



@LeoStats.route('/styles/<path:path>')
def send_styles(path):
    return send_from_directory('../../site/styles/', path)

@LeoStats.route('/scripts/<path:path>')
def send_scripts(path):
    return send_from_directory('../../site/scripts/', path)

@LeoStats.route('/assets/<path:path>')
def send_assets(path):
    return send_from_directory('../../assets/', path)

@LeoStats.route('/assets/favicon.ico')
def fav():
    return send_from_directory(LeoStats.static_folder, 'favicon.ico')
