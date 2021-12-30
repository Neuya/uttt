#ifndef VIEW_H
#define VIEW_H
#include <ncurses.h>
#include "common.h"
#include "model.h"
#include "ia.h"

typedef struct view * p_view; 

/*!
 * This function allocates dynamically a struct view in order to handle the corresponding u_tictactoe.
 *
 * \param p_uttt a pointer on a s_utictactoe.
 * \return a reference to the s_utictactoe memory space allocated, NULL in case
 * of allocation problem.
 */
p_view  create_view(s_utictactoe * p_uttt);

/*!
 * This function retrieves a valid move proposal from the user using the corresponding view.
 *
 * \param p_move a pointer on a s_move to be modified.
 * \param v a pointer on the view to use.
 */
void set_next_player_move(s_move * p_move, p_view v,e_tactic tactic);

/*!
 * This function free all the memory used by a given view which
 * reference is given.
 *
 * \param v a pointer on a view to be freed.
 */
void free_view(p_view v);

/*
* This function get char (e_player) of cell in a tictactoe
* @params : s_tictactoe : the tictactoe, 
* pos : the position in the tictactoe (0<=position<=TICTACTOE_SIZE)
* @return char : the correponding char ('.' if NOBODY setted)
*/
char get_cell(s_tictactoe *t,int pos);

/* 
* This function prints the u_tictactoe in a current view 
* (usefull for refreshing)
* @params : p_view : the view where printing the tictactoe
*/
void print_utictactoe(p_view v);

/*
* This functions prints the tictactoe in a position specified
* (usefull for refreshing)
* NB : unused for inception level 1
* @params : p_view : the view where printing the tictactoe
* int : the position of (inner) tictactoe to refresh
*/
void print_tictactoe(p_view v,int t_pos);

/*!
* This function display on the window the winner 
* at the end of the utictactoe
*
* \param v the pointer on struct view where display
*/
void display_winner(p_view v);

/*
* Function used to test evaluate tictactoe position fonction
* @params : p_view the view where printing evaluate value
*/
void display_evaluate(p_view v);
/*
* Function used to display a simple string under a window
* @params : 
* p_view the view where to print
* char* the string
*/
void display_string(p_view,char*);

#endif /* VIEW_H */