**H9_Stage0**

A minimalist stage-0 C2 designed to fly under the radar. It uses HTTPS and executes commands on the client-side.

Features:
- HTTPS used
- Random connection timer
- Runs in the background
- `ShellExecute` for command execution


## Server
```bash
# Clone the repository
git clone <repo_url>
```

```bash
# Generate SSL certificates (self-signed)
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
```

```bash
# Start the Stage-0 server
python3 stage0_svr.py
```

To send commands, write directly into `commands.txt`:

```bash
echo "cmd /c calc" > commands.txt
```

## Client
Open "x64 Native Tools Command Prompt for VS 2019" and run:

```bash
compile.bat
```


