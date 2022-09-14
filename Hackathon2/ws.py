# import asyncio
# import websockets
# 
# async def hello(websocket, path):
#     name = await websocket.recv()
#     print(f"< {name}")
# 
#     greeting = f"Hello {name}!"
# 
#     await websocket.send(greeting)
#     print(f"> {greeting}")
# 
# start_server = websockets.serve(hello, 'localhost', 8765)
# 
# asyncio.get_event_loop().run_until_complete(start_server)
# asyncio.get_event_loop().run_forever()
import logging
from websocket_server import WebsocketServer

def new_client(client, server):
	server.send_message_to_all("Hey all, a new client has joined us")

server = WebsocketServer(host='127.0.0.1', port=8765, loglevel=logging.INFO)
server.set_fn_new_client(new_client)
server.run_forever()