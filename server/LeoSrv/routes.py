from LeoSrv import LeoStats

@LeoStats.route('/')
def index():
    return "Hello, World!"

@LeoStats.route('/alternate')
def alternate():
    return "This is an alternate page :)"