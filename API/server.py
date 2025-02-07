from flask import Flask, render_template
from flask_socketio import SocketIO
import time

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

@app.route('/')
def index():
    return render_template('controller.html')

@socketio.on('command')
def handle_command(command):
    print(f"Ricevuto comando: {command}")
    # Qui inserisci il codice per muovere il robot
    if command == "avanti":
        print("Robot va avanti")
    elif command == "indietro":
        print("Robot va indietro")

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000)
