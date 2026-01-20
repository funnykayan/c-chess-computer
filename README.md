# c-chess-computer
c chess computer (super good and super fast)

special moves like castling, check, checkmate, the pawn moving 2 at the first move and en passant arent added yet

# compile:
```gcc -O2 -std=c11 -Wall -Wextra -o build/chess src/chess.c```

# run:
```./build/chess```

# making moves
the moving syntax is as following, if you wanted to move from e2 to e3 this would be the input (this takes if there is a piece present on the new position):
```e2e3```

