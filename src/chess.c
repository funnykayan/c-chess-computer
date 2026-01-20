#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// shitty 
#define EMPTY 0
#define PAWN   1
#define KNIGHT 2
#define BISHOP 3
#define ROOK   4
#define QUEEN  5
#define KING   6

int board[8][8];

typedef struct {
    int from_rank, from_file;
    int to_rank, to_file;
    int captured_piece;
    int moved_piece;
} Move;

void generate_moves_for_color(int color, Move* moves, int* count);

#define MAX_MOVES 200

int rook_dirs[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
int bishop_dirs[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}};
int queen_dirs[8][2] = {
    {-1,0}, {1,0}, {0,-1}, {0,1},
    {-1,-1}, {-1,1}, {1,-1}, {1,1}
};

int is_valid_square(int r, int f) {
    return r >= 0 && r < 8 && f >= 0 && f < 8;
}

void add_move(Move* moves, int* count, int fr, int ff, int tr, int tf) {
    if (*count < MAX_MOVES) {
        moves[*count] = (Move){fr, ff, tr, tf, board[tr][tf], board[fr][ff]};
        (*count)++;
    }
}

void gen_sliding_moves(Move* moves, int* count, int rank, int file, int dr[][2], int num_dirs) {
    int piece = board[rank][file];
    if (piece == EMPTY) return;
    int color = (piece > 0) ? 1 : -1;

    for (int d = 0; d < num_dirs; d++) {
        int r = rank + dr[d][0];
        int f = file + dr[d][1];
        while (is_valid_square(r, f)) {
            int target = board[r][f];
            if (target == EMPTY) {
                add_move(moves, count, rank, file, r, f);
            } else {
                if ((target > 0 ? 1 : -1) != color) {
                    add_move(moves, count, rank, file, r, f);
                }
                break;
            }
            r += dr[d][0];
            f += dr[d][1];
        }
    }
}

void gen_knight_moves(Move* moves, int* count, int rank, int file) {
    int piece = board[rank][file];
    if (piece == EMPTY) return;
    int color = (piece > 0) ? 1 : -1;
    int knight_offsets[8][2] = {
        {-2,-1}, {-2,1}, {-1,-2}, {-1,2},
        {1,-2}, {1,2}, {2,-1}, {2,1}
    };

    for (int i = 0; i < 8; i++) {
        int r = rank + knight_offsets[i][0];
        int f = file + knight_offsets[i][1];
        if (is_valid_square(r, f)) {
            int target = board[r][f];
            if (target == EMPTY || (target > 0 ? 1 : -1) != color) {
                add_move(moves, count, rank, file, r, f);
            }
        }
    }
}

void gen_king_moves(Move* moves, int* count, int rank, int file) {
    int piece = board[rank][file];
    if (piece == EMPTY) return;
    int color = (piece > 0) ? 1 : -1;
    int king_offsets[8][2] = {
        {-1,-1}, {-1,0}, {-1,1},
        {0,-1},           {0,1},
        {1,-1},  {1,0},   {1,1}
    };

    for (int i = 0; i < 8; i++) {
        int r = rank + king_offsets[i][0];
        int f = file + king_offsets[i][1];
        if (is_valid_square(r, f)) {
            int target = board[r][f];
            if (target == EMPTY || (target > 0 ? 1 : -1) != color) {
                add_move(moves, count, rank, file, r, f);
            }
        }
    }
}

void gen_pawn_moves(Move* moves, int* count, int rank, int file) {
    int piece = board[rank][file];
    if (piece == EMPTY) return;
    int color = (piece > 0) ? 1 : -1;
    int dir = (color == 1) ? -1 : 1;

    int r1 = rank + dir;
    if (is_valid_square(r1, file) && board[r1][file] == EMPTY) {
        add_move(moves, count, rank, file, r1, file);
        if (((color == 1 && rank == 6) || (color == -1 && rank == 1))) {
            int r2 = rank + 2 * dir;
            if (is_valid_square(r2, file) && board[r2][file] == EMPTY) {
                add_move(moves, count, rank, file, r2, file);
            }
        }
    }

    for (int offset = -1; offset <= 1; offset += 2) {
        int f = file + offset;
        int r = rank + dir;
        if (is_valid_square(r, f)) {
            int target = board[r][f];
            if (target != EMPTY && (target > 0 ? 1 : -1) != color) {
                add_move(moves, count, rank, file, r, f);
            }
        }
    }
}

void generate_moves_for_color(int color, Move* moves, int* count) {
    *count = 0;
    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            int piece = board[r][f];
            if (piece == EMPTY) continue;
            if ((piece > 0 ? 1 : -1) != color) continue;

            switch (piece > 0 ? piece : -piece) {
                case PAWN:   gen_pawn_moves(moves, count, r, f); break;
                case KNIGHT: gen_knight_moves(moves, count, r, f); break;
                case BISHOP: gen_sliding_moves(moves, count, r, f, bishop_dirs, 4); break;
                case ROOK:   gen_sliding_moves(moves, count, r, f, rook_dirs, 4); break;
                case QUEEN:  gen_sliding_moves(moves, count, r, f, queen_dirs, 8); break;
                case KING:   gen_king_moves(moves, count, r, f); break;
            }
        }
    }
}

void make_move(Move m) {
    board[m.to_rank][m.to_file] = board[m.from_rank][m.from_file];
    board[m.from_rank][m.from_file] = EMPTY;
}

void unmake_move(Move m) {
    board[m.from_rank][m.from_file] = m.moved_piece;
    board[m.to_rank][m.to_file] = m.captured_piece;
}


int evaluate(void) {
    int score = 0;
    int piece_values[7] = {0, 100, 320, 330, 500, 900, 20000}; 

    int center[4][2] = {{3,3}, {3,4}, {4,3}, {4,4}};

    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            int piece = board[r][f];
            if (piece == EMPTY) continue;

            int value = piece_values[(piece > 0 ? piece : -piece)];
            int is_white = (piece > 0);
            int sign = is_white ? 1 : -1;

            score += sign * value;

            switch (abs(piece)) {
                case PAWN:
                    // central pawns
                    if (f >= 2 && f <= 5) score += sign * 10;
                    break;

                case KNIGHT:
                    // knigts go center
                    if (r >= 2 && r <= 5 && f >= 2 && f <= 5) {
                        score += sign * 30;
                    }
                    // bonus
                    for (int i = 0; i < 4; i++) {
                        if (r == center[i][0] && f == center[i][1]) {
                            score += sign * 20;
                        }
                    }
                    break;

                case BISHOP:
                    // bishop stuff
                    if (r >= 2 && r <= 5 && f >= 2 && f <= 5) {
                        score += sign * 20;
                    }
                    break;

                case ROOK:
                    // rook stuff
                    if (is_white && r == 7) score -= 10;
                    if (!is_white && r == 0) score += 10;
                    break;

                case QUEEN:
                    // centralization 
                    if (r >= 2 && r <= 5 && f >= 2 && f <= 5) {
                        score += sign * 20;
                    }
                    break;

                case KING:
                    // keep king safe
                    if (is_white) {
                        if (r == 7 && (f == 4 || f == 5 || f == 6)) score += 20;
                    } else {
                        if (r == 0 && (f == 4 || f == 5 || f == 6)) score -= 20;
                    }
                    break;
            }
        }
    }
    return score;
}

int negamax(int depth, int alpha, int beta, int color, int ply) {
    if (depth == 0) {
        return color * evaluate();
    }

    Move moves[MAX_MOVES];
    int count = 0;
    generate_moves_for_color(color, moves, &count);

    if (count == 0) {
        return -10000 * color; // crude mate score
    }

    // move ordering

    for (int i = 0; i < count; i++) {
        make_move(moves[i]);
        int score = -negamax(depth - 1, -beta, -alpha, -color, ply + 1);
        unmake_move(moves[i]);

        if (score > alpha) {
            alpha = score;
            if (alpha >= beta) {
                break;
            }
        }
    }
    return alpha;
}

// calculates the best move so 
Move get_best_move(int depth, int color) {
    Move moves[MAX_MOVES];
    int count = 0;
    generate_moves_for_color(color, moves, &count);

    if (count == 0) {
        printf("ERROR: No moves\n");
        exit(1);
    }

    Move best_move = moves[0];
    int best_score = -1000000;

    for (int i = 0; i < count; i++) {
        make_move(moves[i]);
        int score = -negamax(depth - 1, -1000000, 1000000, -color, 0);
        unmake_move(moves[i]);

        if (score > best_score) {
            best_score = score;
            best_move = moves[i];
        }
    }
    return best_move;
}

void init_board(void) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = EMPTY;

    for (int i = 0; i < 8; i++) {
        board[1][i] = -PAWN;
        board[6][i] =  PAWN;
    }

    board[0][0] = board[0][7] = -ROOK;
    board[0][1] = board[0][6] = -KNIGHT;
    board[0][2] = board[0][5] = -BISHOP;
    board[0][3] = -QUEEN;
    board[0][4] = -KING;

    board[7][0] = board[7][7] =  ROOK;
    board[7][1] = board[7][6] =  KNIGHT;
    board[7][2] = board[7][5] =  BISHOP;
    board[7][3] =  QUEEN;
    board[7][4] =  KING;
}

char piece_to_char(int piece) {
    if (piece == EMPTY) return '.';
    const char* white = " PNBRQK";
    const char* black = " pnbrqk";
    int idx = (piece > 0) ? piece : -piece;
    return (piece > 0) ? white[idx] : black[idx];
}

void print_board(void) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            putchar(piece_to_char(board[rank][file]));
            if (file < 7) putchar(' ');
        }
        putchar('\n');
    }
}

int parse_move(const char* input, Move* m) {
    if (strlen(input) != 4) return 0;
    if (!islower(input[0]) || !isdigit(input[1]) ||
        !islower(input[2]) || !isdigit(input[3])) return 0;

    int from_file = input[0] - 'a';
    int from_rank = 8 - (input[1] - '0');
    int to_file = input[2] - 'a';
    int to_rank = 8 - (input[3] - '0');

    if (!is_valid_square(from_rank, from_file) || !is_valid_square(to_rank, to_file))
        return 0;

    m->from_rank = from_rank;
    m->from_file = from_file;
    m->to_rank = to_rank;
    m->to_file = to_file;
    m->moved_piece = board[from_rank][from_file];
    m->captured_piece = board[to_rank][to_file];
    return 1;
}

int is_move_legal(Move m, int color) {
    if (m.moved_piece == EMPTY) return 0;
    if ((m.moved_piece > 0 ? 1 : -1) != color) return 0;

    Move moves[MAX_MOVES];
    int count = 0;
    generate_moves_for_color(color, moves, &count);

    for (int i = 0; i < count; i++) {
        if (moves[i].from_rank == m.from_rank &&
            moves[i].from_file == m.from_file &&
            moves[i].to_rank == m.to_rank &&
            moves[i].to_file == m.to_file) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    init_board();
    printf("Cursed Chess Engine v0.3 (SMARTER)\n");
    printf("You play BLACK. Bot plays WHITE.\n");
    printf("Enter moves as 'e7e5', 'g8f6', etc.\n\n");

    print_board();
    printf("\n");

    // bot plays
    Move bot_move = get_best_move(4, 1);
    printf("Bot plays: %c%d%c%d\n",
        'a' + bot_move.from_file, 8 - bot_move.from_rank,
        'a' + bot_move.to_file,   8 - bot_move.to_rank);
    make_move(bot_move);
    printf("\n");
    print_board();
    printf("\n");

    while (1) {
        char input[10];
        Move player_move;

        printf("Your move (black): ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;

        if (!parse_move(input, &player_move)) {
            printf("Invalid format. Use e.g. 'e7e5'.\n");
            continue;
        }

        if (!is_move_legal(player_move, -1)) {
            printf("Illegal move. Try again.\n");
            continue;
        }

        make_move(player_move);
        printf("\n");
        print_board();
        printf("\n");

        // check for end
        Move temp_moves[MAX_MOVES];
        int temp_count = 0;
        generate_moves_for_color(1, temp_moves, &temp_count);
        if (temp_count == 0) {
            printf("Game over. Bot has no moves.\n");
            break;
        }

        // bot respond
        bot_move = get_best_move(4, 1);
        printf("Bot plays: %c%d%c%d\n",
            'a' + bot_move.from_file, 8 - bot_move.from_rank,
            'a' + bot_move.to_file,   8 - bot_move.to_rank);
        make_move(bot_move);
        printf("\n");
        print_board();
        printf("\n");

        generate_moves_for_color(-1, temp_moves, &temp_count);
        if (temp_count == 0) {
            printf("Game over. You have no moves.\n");
            break;
        }
    }

    return 0;
}