# FASE-1: compilar
FROM alpine:3.20 AS compiler
RUN apk add --no-cache build-base ncurses ncurses-dev ncurses-static make

WORKDIR /root
COPY . .

RUN make
RUN mkdir -p /out/ti && infocmp xterm-256color > /tmp/xterm.ti && tic -x -o /out/ti /tmp/xterm.ti

# FASE-2: executar
FROM scratch
WORKDIR /pacman

COPY --from=compiler /root/bin/game /pacman/game
COPY --from=compiler /root/map /pacman/map
COPY --from=compiler /out/ti /pacman/ti

ENV TERM=xterm-256color
ENV TERMINFO=/pacman/ti

ENTRYPOINT ["/pacman/game"]
