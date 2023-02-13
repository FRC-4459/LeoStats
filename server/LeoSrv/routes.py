import os
import requests as HTTPRequests
from dotenv import load_dotenv
from LeoSrv import LeoStats
from flask import request, Response

load_dotenv()

headers = {
    'Authorization': f'Basic {os.getenv("FRC_KEY")}',
    'If-Modified-Since': ''
        }

@LeoStats.route('/')
def index():
    return "Hello, World!"

@LeoStats.route('/gamesByEvent')
def gamesByEvent():
    eventCode = request.args.get('event')
    
    if eventCode == None:
        return Response('You must include an event code in your querystring.', 400)
    
    res = HTTPRequests.get(f'https://frc-api.firstinspires.org/v3.0/2022/matches/{eventCode}', headers=headers)
    print(f'FRC Request returned {res.status_code} - {res.reason}')

    return Response(res.text, 200)