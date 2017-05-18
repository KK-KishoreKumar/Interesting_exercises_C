Chess in C programming language - Author: Danilo Novakoviæ

1. GENERAL INFORMATION
Chess is a two-player strategy board game played on a chessboard, a checkered gameboard with 64 squares arranged in an 8×8 
grid. The game is played by millions of people worldwide.
Each player begins with 16 pieces: one king, one queen, two rooks, two knights, two bishops, and eight pawns. 
Each of the six piece types moves differently, with the most powerful being the queen and the least powerful the pawn. 
The objective is to checkmate the opponent's king by placing it under an inescapable threat of capture. 
To this end, a player's pieces are used to attack and capture the opponent's pieces, while supporting each other. 
In addition to checkmate, the game can be won by voluntary resignation of the opponent, 
which typically occurs when too much material is lost, or checkmate appears unavoidable. 
A game can also in several ways end in a draw.
							(Wikipedia)

More on rules of the chess, figure movement and notation used in the program can be found in chess_manual.txt
aswell as, in any time, inside of programm by typing command /help or by selection option 2. from the main menu.

2. REQUIREMENTS
This game uses C programming language standard libraries (ANSII C Standard)
(No additional library is needed, you only need terminal and some kind of compiler like GCC)

3. COMPILING

Linux: Open terminal, navigate to directory containing the source files and execute the following command:
gcc ch_board.c ch_cmd.c ch_danger.c checkmate.c chess.c list.c menu.c redo_undo.c stalemate.c


For more detailed information, see documentation file.