#include "model.h"

s_move *create_empty_move()
{
	s_move *ret = malloc(sizeof(s_move));

	ret->inner_position = FREE;
	ret->outer_position = FREE;
	ret->player = NOBODY;

	return ret;

}

s_tictactoe *create_empty_tictactoe()
{
	//Allocate memory for cells
	s_tictactoe *tic = malloc(sizeof(s_tictactoe));
	tic->content = malloc(sizeof(e_player)*(TICTACTOE_SIZE));

	for(int i=0;i<TICTACTOE_SIZE;i++)
		(tic->content)[i] = NOBODY;

	tic->winner = NOBODY;

	return tic;
}

s_utictactoe *create_empty_utictactoe(uint inception_level)
{
	s_utictactoe *ret = malloc(sizeof(s_utictactoe));

	ret->inception_level = inception_level;
	
	
	if(inception_level > 1) //For UTTTs
	{
		ret->inner_tictactoes= malloc(sizeof(s_tictactoe)*(TICTACTOE_SIZE));
		for(int i=0;i<TICTACTOE_SIZE;i++)
			ret->inner_tictactoes[i]=create_empty_tictactoe();
	}
	else //For a simple TTT ie:inception level 1
		ret->inner_tictactoes=NULL;
	ret->outer_tictactoe = create_empty_tictactoe();

	ret->history=NULL;
	return ret;
}

void free_move(s_move *move)
{
	free(move);
}

void free_tictactoe(s_tictactoe *p_ttt)
{
	free(p_ttt->content);
	free(p_ttt);
}

void free_utictactoe(s_utictactoe *p_uttt)
{
	if(p_uttt->inception_level>1)
	{
		for(int i=0;i<TICTACTOE_SIZE;i++)
		{
			free_tictactoe(p_uttt->inner_tictactoes[i]);//Free each "subtictactoe"
		}
		free(p_uttt->inner_tictactoes);
	}

	free_tictactoe(p_uttt->outer_tictactoe);
	//Freeing move history
	list_element_s_move *tmp;
	while(p_uttt->history!=NULL)
	{
		tmp=p_uttt->history;
		p_uttt->history = p_uttt->history->next;
		free_move(tmp->last_move); //Free move of history node
		free(tmp); //Free pointer
	}

	free(p_uttt);
}

e_player get_next_player_to_play(s_utictactoe *p_uttt)
{
	if(p_uttt->history!=NULL) //If no move has been played
	{
		list_element_s_move *tmp=p_uttt->history;
		return p_uttt->outer_tictactoe->winner == NOBODY? //If utictactoe has no winner
		(tmp->last_move->player==PLAYER_X?PLAYER_O:PLAYER_X)
		:NOBODY;
	}
	return PLAYER_X; //First player to play is X
}

bool is_empty_location_ttt(e_location loca,s_utictactoe *p_uttt)
{
	int empty = 1;
	empty = loca==FREE?
	1
	:p_uttt->inner_tictactoes[loca]->winner==NOBODY;
	return empty;
}


e_location get_next_outer_position(s_utictactoe *p_uttt)
{
	if(p_uttt->history!=NULL && p_uttt->inception_level>1)
	{
		list_element_s_move *tmp=p_uttt->history;
		return is_empty_location_ttt(tmp->last_move->inner_position,p_uttt)? //Is next combination of inner/outer position playable
		tmp->last_move->inner_position //Taking last inner position played
		:FREE;
	}
	return FREE;
}

e_status is_move_valid(s_utictactoe *p_uttt,s_move *move)
{
	e_status ret;
	ret = move->player !=get_next_player_to_play(p_uttt)?NO: //Check if next player is adequate
	((move->outer_position==NONE || move->inner_position == NONE 
	|| move->outer_position==FREE || move->inner_position==FREE)?NO: //Check if move positions are corrects (!=FREE or NONE)
		p_uttt->outer_tictactoe->winner==NOBODY?YES:NO); //Check if the ultimate TT has already a winner

	if(ret==NO) return ret;
	
	if(p_uttt->inception_level==1) // For a simple TTT
	{
		ret = (p_uttt->outer_tictactoe->content[move->inner_position]==NOBODY?YES:NO); //Check if cell is empty
		return ret;
	}
	else //For UTTTs
	{
		ret = (move->outer_position == get_next_outer_position(p_uttt))?YES
		:(get_next_outer_position(p_uttt)==FREE?YES:NO); //Check if outer positions are corresponding or next outer position is FREE
		if(ret==YES) //Check if the tictactoe[outer position] has no winner already
			ret = p_uttt->inner_tictactoes[move->outer_position]->winner == NOBODY?YES:NO; 
		if(ret==YES) //Check if tictactoe[outer position][inner position] cell is empty
			ret = p_uttt->inner_tictactoes[move->outer_position]->content[move->inner_position]==NOBODY?YES:NO;
		return ret;
	}
}

e_player tester_lignes(e_player *content)
{
	for(int i=0;i<TICTACTOE_SIZE-TICTACTOE_WIDTH+1;i+=TICTACTOE_WIDTH)
	{
		if(content[i]==content[i+1] && content[i]==content[i+2])
			if(content[i]!=NOBODY)
				return content[i];
	}

	return NOBODY;
}

e_player tester_colonnes(e_player *content)
{
	for(int i=0;i<TICTACTOE_WIDTH;i++)
	{
		if(content[i]==content[i+TICTACTOE_WIDTH] && content[i]==content[i+((TICTACTOE_WIDTH)*2)])
		{
			if(content[i]!=NOBODY)
			{
				return content[i];
			}
		}
	}
	return NOBODY;
}

void print_all_winner(s_utictactoe *p_uttt)
{
	printf("\nWINNER EN COURS : %c\n",p_uttt->outer_tictactoe->winner);
	for(int i=0;i<9;i++)
	{
		printf("WINNER DU TICTACTOE : %d : %c\n\n",i,p_uttt->inner_tictactoes[i]->winner);
	}
}

e_player tester_diags(e_player *content)
{
	if(content[0]==content[4] && content[4]==content[8] && content[0]!=NOBODY)
		return content[0];
	if(content[6]==content[4] && content[2]==content[4] && content[6]!=NOBODY)
		return content[4];
	return NOBODY;
}

bool check_all_set(e_player *content)
{
	for(int i=0;i<TICTACTOE_SIZE;i++)
	{
		if(content[i]==NOBODY)
			return 0;
	}
	return 1;
}

void set_tictactoe_winner(s_tictactoe *p_ttt)
{
	e_player winner = tester_colonnes(p_ttt->content); //Checking for columns
	if(winner == NOBODY)
		winner=tester_lignes(p_ttt->content); //Checking for lines 
	if(winner == NOBODY)
		winner=tester_diags(p_ttt->content); //Checking for Diagonals
	if(winner == NOBODY && check_all_set(p_ttt->content)) //Checking for all cells setted or not, then winner = BOTH
		p_ttt->winner=BOTH;
	else
		p_ttt->winner=winner;
}

e_status play_move(s_utictactoe *p_uttt,s_move *p_move)
{
	//printf("MOVEP: %d %d %c\n",p_move->inner_position,p_move->outer_position,p_move->player);
	if(is_move_valid(p_uttt,p_move))
	{
		//printf("MOVE VALIDE\n");
		if(p_uttt->inception_level==1)
		{
			p_uttt->outer_tictactoe->content[p_move->inner_position]=p_move->player;
			set_tictactoe_winner(p_uttt->outer_tictactoe);
			if(p_uttt->outer_tictactoe->winner==BOTH && !check_all_set(p_uttt->outer_tictactoe->content))
				p_uttt->outer_tictactoe->winner = NOBODY;
		}
		else
		{
		//Adding move to content
		p_uttt->inner_tictactoes[p_move->outer_position]->content[p_move->inner_position]=p_move->player;
		//Setting winners if there is a new one
		set_tictactoe_winner(p_uttt->inner_tictactoes[p_move->outer_position]);
		
		if(p_uttt->inner_tictactoes[p_move->outer_position]->winner!=NOBODY) //if there is effectively a winner of this subtictactoe
		{
			//Adding winner to utictactoe
			p_uttt->outer_tictactoe->content[p_move->outer_position]=p_uttt->inner_tictactoes[p_move->outer_position]->winner;

			//Setting winner of utictactoe if it exists
			set_tictactoe_winner(p_uttt->outer_tictactoe);

			//Checking is winner has been set to BOTH and other cells of utictactoe are all empties
			if(p_uttt->outer_tictactoe->winner==BOTH && !check_all_set(p_uttt->outer_tictactoe->content))
				p_uttt->outer_tictactoe->winner = NOBODY;

		}
		}
		//Creating new node of history
		list_element_s_move *add = malloc(sizeof(list_element_s_move));
		add->last_move=create_empty_move();
		add->last_move->player = p_move->player;
		add->last_move->inner_position=p_move->inner_position;
		add->last_move->outer_position =p_move->outer_position;
		
		//adding it to the list (ie new node becomes the first (FIFO list))
		add->next = p_uttt->history;
		p_uttt->history=add;

		return YES;
	}
	return NO;
}

e_status undo_last_move(s_utictactoe *p_uttt)
{
	if(p_uttt->history == NULL)
		return NO;

	//Creating pointers to current history
	s_move *last = p_uttt->history->last_move;
	list_element_s_move *list = p_uttt->history;

	if(p_uttt->inception_level == 1)
	{
		//Last move played -> NOBODY
		p_uttt->outer_tictactoe->content[last->inner_position] = NOBODY;
		//resetting tictactoe winner
		set_tictactoe_winner(p_uttt->outer_tictactoe);
	}
	else
	{
		//Last move played -> NOBODY
		p_uttt->inner_tictactoes[list->last_move->outer_position]->content[list->last_move->inner_position] = NOBODY;
		//resetting last inner tictactoe winner
		set_tictactoe_winner(p_uttt->inner_tictactoes[list->last_move->outer_position]);

		//If outer position in outer tictactoe is setted (ie !=NOBODY)
		if(p_uttt->outer_tictactoe->content[list->last_move->outer_position]!=NOBODY)
		{
			//If corresponding inner tictactoe has no winner
			if(p_uttt->inner_tictactoes[list->last_move->outer_position]->winner == NOBODY)
				p_uttt->outer_tictactoe->content[list->last_move->outer_position]=NOBODY;
		}
		//Setting outer tictactoe winner
		set_tictactoe_winner(p_uttt->outer_tictactoe);
	}

	//freeing pointers and setting history to his previous move
	free_move(last);
	p_uttt->history = p_uttt->history->next;
	free(list);

	return YES;
}

void draw_ith_line_of_ttt(s_tictactoe *p_ttt, uint line) {
  assert(p_ttt);
  printf("%c%c%c", p_ttt->content[line * TICTACTOE_WIDTH],
         p_ttt->content[line * TICTACTOE_WIDTH + 1],
         p_ttt->content[line * TICTACTOE_WIDTH + 2]);
}

void draw_tictactoe(s_tictactoe *p_ttt) {
  assert(p_ttt);
  for (uint line = 0; line < TICTACTOE_WIDTH; line++) {
    draw_ith_line_of_ttt(p_ttt, line);
    printf("\n");
  }
}

void draw_utictactoe_history(s_utictactoe *p_uttt) {
  assert(p_uttt);
  char *e_location_name[] = {"TL", "TC", "TR", "ML",   "MC",  "MR",
                             "BL", "BC", "BR", "FREE", "NONE"};
  list_element_s_move *tmp = p_uttt->history;
  while (tmp != NULL) {
    if (tmp->last_move) {
      printf(" - %s %s %c \n", e_location_name[tmp->last_move->inner_position],
             e_location_name[tmp->last_move->outer_position],
             tmp->last_move->player);
    }
    tmp = tmp->next;
  }
}

void draw_utictactoe(s_utictactoe *p_uttt) {
  assert(p_uttt);
  if (p_uttt->inception_level == 1) {
    draw_tictactoe(p_uttt->outer_tictactoe);
  } else {
    for (uint id_ttt = 0; id_ttt < TICTACTOE_SIZE;
         id_ttt = id_ttt + TICTACTOE_WIDTH) {
      for (uint line = 0; line < TICTACTOE_WIDTH; line++) {
        draw_ith_line_of_ttt(p_uttt->inner_tictactoes[id_ttt], line);
        printf("|");
        draw_ith_line_of_ttt(p_uttt->inner_tictactoes[id_ttt + 1], line);
        printf("|");
        draw_ith_line_of_ttt(p_uttt->inner_tictactoes[id_ttt + 2], line);
        printf("\n");
      }
      if (id_ttt + TICTACTOE_WIDTH < TICTACTOE_SIZE) {
        printf("-----------");
        printf("\n");
      }
    }
  }
  printf("\n####\n");
}

void save_a_utictactoe_to_file(FILE *fd,s_utictactoe *p_uttt)
{
	fprintf(fd,"%d\n",p_uttt->inception_level);
	char *e_location_name[] = {"TL", "TC", "TR", "ML",   "MC",  "MR",
                             "BL", "BC", "BR", "FREE", "NONE"};
  list_element_s_move *tmp = p_uttt->history;
  while (tmp != NULL)
	{
    if (tmp->last_move)
		{
      fprintf(fd,"%s %s %c\n", e_location_name[tmp->last_move->inner_position],
             e_location_name[tmp->last_move->outer_position],
             tmp->last_move->player);
    }
    tmp = tmp->next;
	}
}
