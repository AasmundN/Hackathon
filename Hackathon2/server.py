import eventlet
import socketio

# https://python-socketio.readthedocs.io/en/latest/server.html
# https://snyk.io/blog/how-to-build-a-secure-websocket-server-in-python/
# https://medium.com/unity-nodejs/websocket-client-server-unity-nodejs-e33604c6a006

sio = socketio.Server(cors_allowed_origins='*')
app = socketio.WSGIApp(sio)

# Triggered when a client connects to our socket. 
@sio.on("connect")
def connect(sid, socket):    
    print(sid, 'connected')
    
@sio.on("connection")
def connect(sid, socket):    
    print(sid, 'connected2')

# Triggered when a client disconnects from our socket
@sio.on("disconnect")
def disconnect(sid):
    print(sid, 'disconnected')

@sio.on("message")
def message(sid, data):
    """
    Key arguments:
    sid - the session_id, which is unique to each client
    data - payload sent from the client
    """
    sio.emit("message", data, to=sid)

@sio.on('*')
def catch_all(event, sid, data):
    print("event: ", event)
    print("sid: ", sid)
    print("data: ", data)

if __name__ == '__main__':
    eventlet.wsgi.server(eventlet.listen(('', 5000)), app)
