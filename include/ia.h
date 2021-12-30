#ifndef IA_H
#define IA_H
#include "common.h"
#include "model.h"
#include <time.h>

/*!
 * This function modify the move in parameter in order to get a valid move (1) randomly if tactic is 0 or 
 * (2) in a clever way if tactic is 1 in the corresponding utictactoe u.
 *
 * \param p_uttt a pointer on a s_utictactoe.
 * \param p_move a pointer on a s_move to be modified.
 * \param tactic an e_tactic set to 0 for random strategy and 1 for clever one.
 */
void best_move(s_utictactoe* p_uttt, s_move* p_move, e_tactic tactic);

//Infinite is a value unreachable with evaluating positions
#define INFINITE 20
//Winvalue is a value correponding to a winning position (must be != infinite)
#define WINVALUE 15
//Deepness is defined to adjust the Deepness wanted for min_max algorithm
#define DEEPNESS 7

/*
* Struct representing a search tree
* root : the "current" root
* listT* child_list : the list of direct children of root
* int value: the value stored in root
* s_move* move : the move stored in root
*/
typedef struct search_tree
{
	struct search_tree* root;
	struct listT* child_list;
	int value;
	s_move* move;
}tree;

/*
* Struct representing a list of trees 
* search_tree* node : the current node
* listT *next : the next listT of node
*/
typedef struct listT
{
	struct search_tree* node;
	struct listT *next;
}list_tree;

/*	
* Function determining if a tree is a leaf
* @params : tree* 
* @return : boolean true if the tree has no childs
*/
int is_leaf(tree*);

/*
* Function that adds a child to another tree
* @params : tree root the root tree, tree child the child to add
* @return : int the value attribute of added child 
*/
int add_child(tree*,tree*);

/*
* Function used to print a tree in std output
* @params : tree* : the root, int : the deepness (starting at 0)
*/
void print_tree(tree*,int);

/*
* Function used to print a tree in file
* @params : FILE : the file specified, tree*: the root, int : the deepness
*/
void print_tree_to_file(FILE *fd,tree *root,int n);

/*
* Function used to free a list of child in a tree
* @params : list_tree* the list to free
*/
void free_list_tree(list_tree* list);

/*
* Function used to free a tree
* it's also freeing all his children
* @params : tree* the tree to free
*/
void free_tree(tree *t);

/*
* Function used to count number of children of tree
* @params : tree* the tree
* @return int : the number of children
*/
int count_childs(tree* root);

/*
* Function used to return the min of a tree children
* @params: tree* the root
* @return: int : the minimum value of children
*/
int min_of_childs(tree* root);

/*
* Function used to return the max of a tree children
* @params: tree* the root
* @return: int : the maximum value of children
*/
int max_of_childs(tree* root);

/*
* Function used to print all children of a tree
* (usefull for debugging)
* @params : tree* the root
*/
void print_list_childs(tree* root);

/*
* Function used to create a sub_tab of three e_player
* NB : used and freed in count_occurence function
* @params : e_players : the three e_players
* @return : e_player* the tab created
*/
e_player* sub_tab(e_player i,e_player j,e_player k);

/*
* Function used to count occurences of different e_player in a sub tab
* NB : free the tab at end of function
* @params : e_player* the sub tab
* @return : 
* 1 if there is one X and two NOBODY,
* 2 if there is 2 X and 1 NOBODY,
* -1 if there is one O and two NOBODY,
* -2 if there is two O and one NOBODY
* else 0 (equal)
*/
int count_occurence(e_player *tab);

/*
* Function used to get the first move correponding 
* to a value in a list of tree
* @params : int : the value we want to correspond,
* list_tree* the list to browse
* @return : s_move* the first move (stored in a tree)
* where value if corresponding
*/ 
s_move* get_move_associated(int value,list_tree* list);

/*
* This recursive function is adapted to the famous min_max algorithm
* it construct a tree where all move possibles are played
* then adjust different values to determine what is the best move
* @params : int : the current deepness (firstly called with DEEPNESS)
* s_utictactoe : the current u_tictactoe
* int, int : alpha & beta (used to pruning tree)
* e_player : the player to maximize
* @return : the tree effectively created
*/
tree* my_min_max(int,s_utictactoe*,s_move*,int ,int ,e_player);

/*
* This function create a tree
* apply min_max on current position of s_utictactoe
* then return the best move correponding
* @params: s_utictactoe the ultimate tictactoe
* e_player : the player we want to maximize
* @return : s_move* the best_move corresponding
*/
s_move* best_move_clever(s_utictactoe*,e_player);

/*
* This function evaluates a position 
* of a tictactoe
* it is positive if Player_X is winning
* negative if Player_O is winning
* @params : s_tictactoe the tictactoe to evaluates
* @return int : 
*  number of lines/diags/col where there is
	- (2 X 1 Nobody) * 3 (close to win) + 1 X 1 Nobody
	- minus (2 O 1 nobody) * 3 + 1 O 2 nobody
*/
int evaluate_position(s_tictactoe *p_ttt);

#endif