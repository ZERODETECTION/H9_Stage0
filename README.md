# H9_Stage0

A minimalistic stage0 c2 to fly under the radar. It is using https and will execute commands on the client-side.

* https
* random connect timer
* runs in background
* shellexecute

## Server
```
git clone repo
```
```
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes
```


## Client
Open "x64 Native Tools Command Prompt for VS 2019"
```
compile.bat
```

