from flask import Flask

LeoStats = Flask(__name__, static_folder="../../assets/")

from LeoSrv import routes