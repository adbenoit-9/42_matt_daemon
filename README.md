# MATT_DAEMON (@42Paris)
*A Unix project to create a daemon of type server.*

## Usage
```
$ git clone https://github.com/adbenoit-9/42_matt_daemon.git
$ cd 42_matt_daemon/
$ make
$ sudo ./Matt_daemon
```

## Description

**Daemon**: A program that runs as a background process.

Matt_daemon is a daemon that listens on the port 4242. Each action is shown in its log file `/var/log/matt_daemon/matt_daemon.log`.

You can shutdown the daemon by sending of a simple "quit" on the port 4242.
```
$ nc localhost 4242
quit
```


*In collaboration with [codebrai](https://github.com/codebrai) 🔥*
