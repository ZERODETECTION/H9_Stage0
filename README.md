**H9_Stage0**

A minimalist stage-0 C2 designed to fly under the radar. It uses HTTPS and executes commands on the client-side.

Features:
- HTTPS used
- Random connection timer
- Runs in the background
- `ShellExecute` for command execution

## Server
```
git clone repo
```
```
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
```
```
python3 stage0_svr.py
```
To send commands you write direkt into the commands.txt
```
echo "cmd /c calc" > commands.txt
```

## Client
Open "x64 Native Tools Command Prompt for VS 2019"
```
compile.bat
```

