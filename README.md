*This project has been created as part of the 42 curriculum by rzamolo-, alfsanch

# ft_irc — Internet Relay Chat Server

## Description

`ft_irc` is a fully functional IRC server written in C++ 98. It implements the core IRC protocol, allowing real IRC clients (such as HexChat or WeeChat) to connect, authenticate, join channels, exchange messages and manage channel settings through operators.

The server handles multiple simultaneous clients without blocking, using a single `poll()` call for all I/O operations. No forking is used.

Implemented commands: `PASS`, `NICK`, `USER`, `JOIN`, `PART`, `PRIVMSG`, `KICK`, `INVITE`, `TOPIC`, `MODE`, `QUIT`, `PING/PONG`.

Supported channel modes: `i` (invite-only), `t` (topic restricted to operators), `k` (channel key/password), `o` (give/take operator privilege), `l` (user limit).

## Instructions

### Compilation

```bash
make
```

Requires a C++ 98 compatible compiler (`g++` or `clang++`). No external libraries needed.

### Running the server

```bash
./ircserv <port> <password>
```

- `port`: listening port (e.g. `6667`)
- `password`: connection password required by IRC clients

Example:

```bash
./ircserv 6667 mypassword
```

### Connecting with HexChat

1. Open HexChat and go to **HexChat → Network List**
2. Click **Add**, name the network (e.g. `ft_irc`) and click **Edit**
3. Under **Servers**, add `localhost/6667`
4. Set **Server password** to your chosen password
5. Disable SSL if enabled
6. Click **Connect**

Or directly from the HexChat input bar:

```
/server localhost 6667 mypassword
```

### Connecting with WeeChat

```
/server add ft_irc localhost/6667 -password=mypassword
/connect ft_irc
```

### Quick test with netcat

```bash
nc -C 127.0.0.1 6667
PASS mypassword
NICK testnick
USER testuser 0 * :Real Name
JOIN #general
PRIVMSG #general :Hello world!
```

### Makefile rules

| Rule | Description |
|------|-------------|
| `make` | Build the server |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and binary |
| `make re` | Full rebuild |

## Resources

### IRC Protocol references

- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812 — IRC: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [Modern IRC documentation](https://modern.ircdocs.horse/)
- [IRC numeric replies reference](https://www.alien.net.au/irc/irc2numerics.html)
