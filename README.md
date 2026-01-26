# Chess v.1[.1,.2,.3]

## Scope [as of right now]
A two-player chess game based in the console with legal moves

## Excluded [for now]:
- No AI [only for 2 players]
- No GUI
- No timers

## What does 'done' look like?
Being able to play chess on the console; each player provides the square and the piece that they want to move to [and from].

### Planning
- Each piece will keep track of it's "own data".
    - Pieces will not keep track of where they are.
    - Essentially they will only know HOW they move and they will implement that; the board will take care of the rest. 
- Every piece will basically know how to move.
    - e.g. a rook will know it can move back and forth and side to side but not know which squares it can land on. 
- The board will keep track of essentially where the pieces are. 
    - This is done so that the board can "validate" if the piece's moves are feasible as each piece doesn't keep tabs on the others
    - So items such as pins and castling violations, the board will keep track of, even though a piece might think it's legal to make those moves. 

### Plans for 3 Versions [General Overview]

- Version 1: Console Based Chess 'Engine'
    - v 1.1: Implement Board and Pieces [Kings, Queens, Bishops, Knights, Rooks, and Pawns]
    - v 1.2: Castling, En Passant, Promotions, Checks [includes pins], and Checkmate!
    - v 1.3: Better Board Graphics, Players, Switching Board Views

- Version 2: GUI Based Chess 'Engine' 
- Version 3: Client-Server [Chess Engine Online]