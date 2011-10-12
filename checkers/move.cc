#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "move.h"

#define Clear 0x1f
#define Empty 0x00
#define Piece 0x20
#define King 0x60
#define Red 0x00
#define White 0x80
#define number(x) ((x)&0x1f)
#define empty(x) ((((x)>>5)&0x03)==0?1:0)
#define piece(x) ((((x)>>5)&0x03)==1?1:0)
#define king(x) ((((x)>>5)&0x03)==3?1:0)
#define color(x) ((((x)>>7)&1)+1)

Move *move_from_string(const char *buf) {
    int i=0;
	const char *ind = buf;
	Move *move = (Move*)malloc(sizeof(Move));
	bzero(move, sizeof(Move));

    while(*ind != '\0') {
        *move[i] = (char) atoi(ind);
		i++;
		while (isdigit(*ind)) ind++;
		ind++;
    }
    return move;
}

void move_to_string(Move *move, char *buf) {
    int i;
	char temp[8];

    buf[0] = '\0';
    for(i=0; move[i] != '\0'; i++) {
		sprintf(temp, "%d", *move[i]);
		strcat(buf, temp);
		strcat(buf, "-");
    }
    buf[strlen(buf)-1] = '\0';
}

void move_list_add(MoveList *moves, Move *move) {
	memcpy(&moves->moves[moves->length], move, sizeof(Move));
	moves->length++;
}

/* Finds legal non-jump moves for the King at position x,y */
void move_list_add_king_moves(MoveList *moves, Board *board, int x, int y) {
    int i,j,x1,y1;
    Move move;
    memset(move,0,sizeof(Move));

    /* Check the four adjacent squares */
    for(j=-1; j<2; j+=2)
    for(i=-1; i<2; i+=2)
    {
        y1 = y+j; x1 = x+i;
        /* Make sure we're not off the edge of the board */
        if(y1<0 || y1>7 || x1<0 || x1>7) continue; 
//        if(empty(*board[y1][x1])) {  /* The square is empty, so we can move there */
//            move[0] = number(*board[y][x])+1;
//            move[1] = number(*board[y1][x1])+1;    
//            move_list_add(moves, &move);
//        }
    }
}

/* Finds legal non-jump moves for the Piece at position x,y */
void move_list_add_moves(MoveList *moves, Board *board, Player player, int x, int y) 
{
    int i,j,x1,y1;
    Move move;

    memset(move,0,sizeof(Move));

    /* Check the two adjacent squares in the forward direction */
    if(player == 0) j = 1; else j = -1;
    for(i=-1; i<2; i+=2)
    {
        y1 = y+j; x1 = x+i;
        /* Make sure we're not off the edge of the board */
        if(y1<0 || y1>7 || x1<0 || x1>7) continue; 
//        if(empty(*board[y1][x1])) {  /* The square is empty, so we can move there */
//            move[0] = number(*board[y][x])+1;
//            move[1] = number(*board[y1][x1])+1;    
//            move_list_add(moves, &move);
//        }
    }
}

/* Determines all of the legal moves possible for a given state */
void FindLegalMoves(MoveList *moves, Board *b, Player player) {
/*
    int x,y;
    char move[12], board[8][8];

    memset(move,0,sizeof(Move));

    for(y=0; y<8; y++)
    for(x=0; x<8; x++)
    {
        if(x%2 != y%2 && color(board[y][x]) == player && !empty(board[y][x])) {
            if(king(board[y][x])) {
                move[0] = number(*board[y][x])+1;
                FindKingJump(player,board,move,1,x,y);
                if(!jumpptr) move_list_add_king_moves(board,x,y);
            } 
            else if(piece(*board[y][x])) {
                move[0] = number(*board[y][x])+1;
                FindJump(player,board,move,1,x,y);
                if(!jumpptr) move_list_add_moves(moves, board, player, x, y);
            }
        }    
    }
    if(jumpptr) {
        for(x=0; x<jumpptr; x++) 
        for(y=0; y<12; y++) 
        //state->movelist[x][y] = jumplist[x][y];
        //state->numLegalMoves = jumpptr;
    } 
    else {
        for(x=0; x<numLegalMoves; x++) 
        for(y=0; y<12; y++) 
        //state->movelist[x][y] = movelist[x][y];
        //state->numLegalMoves = numLegalMoves;
    }
    return (jumpptr+numLegalMoves);
*/
}

MoveList *move_list_create(Board *board, Player player) {
	MoveList *moves = (MoveList *)malloc(sizeof(MoveList));
	moves->length = 0;
	FindLegalMoves(moves, board, player);
	return moves;
}

