#include "ia.h"

void best_move(s_utictactoe* p_uttt, s_move* p_move, e_tactic tactic)
{
	if(tactic == RANDOM)
	{
		srand(time(NULL));
		do
		{
			p_move -> outer_position = get_next_outer_position(p_uttt);

			p_move -> inner_position = TOPLEFT + rand() % TICTACTOE_SIZE;

			if(p_move -> outer_position == FREE)
				p_move->outer_position = TOPLEFT + rand() % TICTACTOE_SIZE;

		}while(!is_move_valid(p_uttt,p_move));
	}
	if(tactic==CLEVER)
	{
		s_move *s = best_move_clever(p_uttt,get_next_player_to_play(p_uttt));
		p_move -> outer_position = s->outer_position;
		p_move -> inner_position = s->inner_position;
		p_move -> player = s->player;
		free_move(s);
	}
}



void free_list_tree(list_tree* list)
{
	list_tree *tmp = list;
	while(list!=NULL)
	{
		tmp = list;
		list = list->next;
		free_tree(tmp->node);
		free(tmp);
	}
}

int is_empty_list(list_tree* list)
{
	return list->node == NULL && list->next == NULL;
}


void free_tree(tree *t)
{
	if(!is_empty_list(t->child_list))
		free_list_tree(t->child_list);
	else
		free(t->child_list);
	free_move(t->move);
	free(t);
}


list_tree* create_empty_list_tree()
{
	list_tree *ret = malloc(sizeof(list_tree));
	ret->node = NULL;
	ret->next = NULL;
	return ret;
}

tree* create_empty_tree()
{
	tree *ret = malloc(sizeof(tree));
	ret->root = NULL;
	ret->child_list = create_empty_list_tree();
	return ret;
}

int min_of_childs(tree* root)
{
	int min=INFINITE;
	if(!is_leaf(root))
	{
		list_tree *tmp = root->child_list;
		min = tmp->node->value;

		//Browsing list
		while(tmp!=NULL)
		{
			//Checking for min value
			min = tmp->node->value < min?tmp->node->value:min;
			tmp = tmp->next;
		}
	}

	return min;
}

int max_of_childs(tree* root)
{
	int max=INFINITE*(-1);
	if(!is_leaf(root))
	{
		list_tree *tmp = root->child_list;
		max = tmp->node->value;

		//Browsing list
		while(tmp!=NULL)
		{
			//Checking for max value
			max = tmp->node->value > max?tmp->node->value:max;
			tmp = tmp->next;
		}
	}

	return max;
}

void print_list_childs(tree* root)
{
	list_tree *tmp = root->child_list;
	int i=0;
	while(tmp!=NULL)
	{
		i++;
		tmp=tmp->next;
	}
}

void print_list_childs_file(tree *root)
{
	list_tree *tmp = root->child_list;
	while(tmp!=NULL)
	{
		fprintf(stderr,"Value : %d \n",tmp->node->value);
		tmp=tmp->next;
	}
}

int add_child(tree* root,tree* child)
{
	//If there is no child yet 
	if(root->child_list->node == NULL)
		root->child_list->node = child; //root Child_list first node becomes child
	else
	{
		list_tree *tmp = root->child_list;
		//Browsing list of children
		while(tmp->next!=NULL)
			tmp = tmp->next;
		//Adding last node
		list_tree *new = create_empty_list_tree();
		new->node = child;
		tmp->next = new;
	}
	//Return value of child effectively added
	return child->value;
}


void print_tree_to_file(FILE *fd,tree *root,int n)
{
	
	for(int i=0;i<n+1;i++)
	{
		fprintf(fd,"|");
	}
	fprintf(fd,"%d MOVE : o:%d i:%d %c\n",root->value,root->move->outer_position,root->move->inner_position,root->move->player);
	if(!is_leaf(root))
	{
		list_tree* tmp = root->child_list;
		while(tmp!=NULL)
		{
			print_tree_to_file(fd,tmp->node,n+1);
			tmp = tmp->next;
		}
	
	}
}

void add_value(tree* t,int n)
{
	t->value = n;
}

int is_root(tree* t)
{
	return t->root == NULL;
}

int is_leaf(tree* t)
{
	return t->child_list->node ==NULL;
}

e_player* sub_tab(e_player i,e_player j,e_player k)
{
	e_player* ret = malloc(sizeof(e_player)*3);
	
	ret[0]=i;
	ret[1]=j;
	ret[2]=k;

	return ret;
}

int count_occurence(e_player *tab)
{
	int x=0,o=0,n=0;
	
	//counting occurences of PLAYER_X, PLAYER_O And Blanks
	for(int i=0;i<TICTACTOE_WIDTH;i++)
	{
		tab[i]==PLAYER_X?x++:(tab[i]==PLAYER_O?o++:n++);
	}

	//Freeing tab
	free(tab);

	/*
	* 1 if there is one X and two NOBODY,
	* 2 if there is 2 X and 1 NOBODY,
	* -1 if there is one O and two NOBODY,
	* -2 if there is two O and one NOBODY
	* else 0 (equal)
	*/
	int ret =  n==2?(x==0?o*(-1):x):(n==1?(x==0?(-1)*o:(o==0?x:0)):0);

	return ret;
}

int evaluate_position(s_tictactoe *p_ttt)
{
	//If there is a winner, return WINVALUE
	if(p_ttt->winner == PLAYER_X)
	{
		return WINVALUE;
	}
	else if(p_ttt->winner==PLAYER_O)
	{
		return WINVALUE*(-1);
	}

	int x1=0,x2=0,o1=0,o2=0;
	e_player *tab = p_ttt->content;

	int count;

	for(int i=0;i<TICTACTOE_WIDTH;i++)
	{
		//Lines
		count=count_occurence(sub_tab(tab[TICTACTOE_SIZE-i*TICTACTOE_WIDTH-3],tab[TICTACTOE_SIZE-i*TICTACTOE_WIDTH-2],tab[TICTACTOE_SIZE-i*TICTACTOE_WIDTH-1]));
		if(count!=0)
			count==1?x1++:(count==2?x2++:(count==-1?o1++:o2++)); //Adjusting variables with result of count_occurences

		//Columns
		count = count_occurence(sub_tab(tab[i],tab[i+TICTACTOE_WIDTH],tab[i+TICTACTOE_WIDTH*2]));
		if(count!=0)
			count==1?x1++:(count==2?x2++:(count==-1?o1++:o2++)); //Adjusting variables with result of count_occurences
	}

	//Diags
	count = count_occurence(sub_tab(tab[0],tab[4],tab[8]));
	if(count!=0)
		count==1?x1++:(count==2?x2++:(count==-1?o1++:o2++)); //Adjusting variables with result of count_occurences


	count = count_occurence(sub_tab(tab[2],tab[4],tab[6]));
	if(count!=0)
		count==1?x1++:(count==2?x2++:(count==-1?o1++:o2++)); //Adjusting variables with result of count_occurences

	/*
	Return : number of lines/diags/col where there is
	- (2 X 1 Nobody) * 3 (close to win) + 1 X 1 Nobody
	- minus (2 O 1 nobody) * 3 + 1 O 2 nobody
	*/
	return x2*3 + x1 - (o2*3 + o1);
}

int count_childs(tree* root)
{
	int i=0;
	list_tree* tmp = root->child_list;
	while(tmp!=NULL)
	{
		i++;
		tmp = tmp->next;
	}
	return i;
}

s_move* get_move_associated(int value,list_tree* list)
{
	s_move* new = create_empty_move();
	list_tree *tmp=list;
	//Browsing list to find value corresponding
	while(tmp->node->value!=value && tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	new->outer_position = tmp->node->move->outer_position;
	new->inner_position = tmp->node->move->inner_position;
	new->player = tmp->node->move->player;
	//return move associated
	return new;
}

s_move* best_move_clever(s_utictactoe *p_uttt,e_player player)
{
	s_move* s = create_empty_move();
	//Construct search_tree
	tree* root = my_min_max(DEEPNESS,p_uttt,s,INFINITE*(-1),INFINITE,player);
	/*FILE *fd = fopen("debug.txt","w");
	if(fd)
		print_tree_to_file(fd,root,0);
	fclose(fd);*/

	//Get move associated to max value stored in search_tree root
	s_move *ret = get_move_associated(root->value,root->child_list);
	free_tree(root);

	return ret;
}

int maxi(int a,int b)
{
	return a>=b?a:b;
}

int mini(int a,int b)
{
	return a<=b?a:b;
}

tree* my_min_max(int deep,s_utictactoe *p_uttt,s_move* move,int alpha,int beta, e_player player_to_max)
{
	tree *root = create_empty_tree();
	root->move = move;
	int eval;
	if(deep==0)
	{
		int val = evaluate_position(p_uttt->outer_tictactoe);
		root->value = player_to_max == PLAYER_O? val*(-1):val;
	}
	else if(p_uttt->outer_tictactoe->winner!=NOBODY) //Checking if position is winned by someone
	{
		if(p_uttt->outer_tictactoe->winner == BOTH) //Equals if BOTH
			root->value = 0;
		else
			root->value = p_uttt->outer_tictactoe->winner == player_to_max?WINVALUE:WINVALUE*(-1);
	}
	else
	{
		s_move* move = create_empty_move();
		int stop = 0;  //Boolean for pruning
		int position_free = 0; //Boolean for knowing if next outer position is free
		int out_pos = 0;
		if(p_uttt->inception_level > 1)
		{
			//Recovering next outer position according to last_move played
			move->outer_position = get_next_outer_position(p_uttt);
			position_free = move->outer_position == FREE;
		}

		do{
			//If next outer position is free		
			if(position_free)
			{
				do
				{
					move->outer_position = out_pos;
					out_pos++;
				}while(p_uttt->outer_tictactoe->content[move->outer_position]!=NOBODY && out_pos < TICTACTOE_SIZE);
				
				if(out_pos == TICTACTOE_SIZE)
					position_free = 0;
			}

			for(int i=0;(i<9 && !stop);i++)
			{
				
				move->player = get_next_player_to_play(p_uttt);
				move->inner_position = i;

				if(p_uttt->inception_level == 1)
					move->outer_position = i;

				if(play_move(p_uttt,move))
				{
					s_move* move_to_add = create_empty_move();
					move_to_add->player = move->player;
					move_to_add->inner_position = move->inner_position;
					move_to_add->outer_position = move->outer_position;
					
					//recovering last value
					eval = add_child(root,my_min_max(deep-1,p_uttt,move_to_add,alpha,beta,player_to_max));

					//Undoing last_move in utictactoe
					undo_last_move(p_uttt);
					
					//Pruning
					if(move->player == player_to_max)
						alpha = maxi(alpha,eval);
					else
						beta = mini(beta,eval);
					stop = beta <= alpha;
				}
					
			}
			
			root->value = (move->player == player_to_max)?max_of_childs(root):min_of_childs(root);		
		
		}while(position_free);
		
		free_move(move);	
	}

	return root;
}


void print_tree(tree* root,int n)
{
	for(int i=0;i<n+1;i++)
	{
		printf("|");
	}
	printf("%d MOVE : %d %c\n",root->value,root->move->inner_position,root->move->player);
	if(!is_leaf(root))
	{
		list_tree* tmp = root->child_list;
		while(tmp!=NULL)
		{
			print_tree(tmp->node,n+1);
			tmp = tmp->next;
		}
	}
}

