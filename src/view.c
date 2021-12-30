#include "common.h"
#include "model.h"
#include "view.h"

struct view{
    s_utictactoe * p_uttt;
    WINDOW* w_uttt;
    WINDOW* w_ttt;
}; 

char get_cell(s_tictactoe *t,int pos)
{
    return t->content[pos]==NOBODY || pos==FREE?'.':t->content[pos];
}

p_view create_view(s_utictactoe * u)
{
    
    initscr();
    raw();
    noecho();
    start_color();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_BLACK,COLOR_WHITE);
    init_pair(2, COLOR_BLACK,COLOR_RED);
    curs_set( 0 );
    p_view p = malloc(sizeof(struct view));
    WINDOW * w_uttt =  subwin(stdscr, 9, 11, 2, 1);
    WINDOW * w_ttt = NULL;
     
    box(w_uttt, ACS_VLINE, ACS_HLINE);
   
    int position = 2;
    for(int i=0;i<TICTACTOE_SIZE;i+=3)
    {
        mvwprintw(w_uttt,position,3,"%c|%c|%c",get_cell(u->outer_tictactoe,i),get_cell(u->outer_tictactoe,i+1),get_cell(u->outer_tictactoe,i+2));
        if(i!=6)
        mvwprintw(w_uttt,position+1,3,"-----");
        position+=2;
    }
    if(u->inception_level > 1)
    {
        w_ttt =  subwin(stdscr, 9, 11, 2, 15);
        box(w_ttt, ACS_VLINE, ACS_HLINE);
        position = 2;
        for(int i=0;i<TICTACTOE_SIZE;i+=3)
        {
            mvwprintw(w_ttt,position,3,"%c|%c|%c",get_cell(u->outer_tictactoe,i),get_cell(u->outer_tictactoe,i+1),get_cell(u->outer_tictactoe,i+2));
            if(i!=6)
            mvwprintw(w_ttt,position+1,3,"-----");
            position+=2;
        }
    }

    wrefresh(w_uttt);
    wrefresh(stdscr);    
 
    p->p_uttt=u;
    p->w_uttt = w_uttt;
    p->w_ttt=w_ttt; 
    return p;
}

void print_utictactoe(p_view v)
{
    int position = 2;
    for(int i=0;i<TICTACTOE_SIZE;i+=3)
    {
        mvwprintw(v->w_uttt,position,3,"%c|%c|%c",get_cell(v->p_uttt->outer_tictactoe,i),get_cell(v->p_uttt->outer_tictactoe,i+1),get_cell(v->p_uttt->outer_tictactoe,i+2));
        if(i!=6)
        mvwprintw(v->w_uttt,position+1,3,"-----");
        position+=2;
    }
}

void print_tictactoe(p_view v,int t_pos)
{
    int position = 2;
    for(int i=0;i<TICTACTOE_SIZE;i+=3)
    {
        mvwprintw(v->w_ttt,position,3,"%c|%c|%c",get_cell(v->p_uttt->inner_tictactoes[t_pos],i)
            ,get_cell(v->p_uttt->inner_tictactoes[t_pos],i+1),get_cell(v->p_uttt->inner_tictactoes[t_pos],i+2));
        if(i!=6)
        mvwprintw(v->w_ttt,position+1,3,"-----");
        position+=2;
    }
}

void set_next_player_move(s_move * m,p_view v,e_tactic tactic)
{
    if(tactic != HUMAN)
    {
        print_utictactoe(v);
        best_move(v->p_uttt,m,tactic);
        wrefresh(v->w_uttt);
        wrefresh(stdscr);
    }
   
    if(tactic == HUMAN)
    {  
        int ch=-1;
        char *s=malloc(sizeof(char)*20);
        sprintf(s,"Next player : %c",m->player);
        int current_pos_X=3;
        int current_pos_Y=2;
        int position=0;
        int inner_position=0;
        int inner = 0;
        int played = 0;
        int current_outer_position_X=0,current_outer_position_Y=0;
        int position_to_play = get_next_outer_position(v->p_uttt);
        int y = position_to_play/3*2+2;
        int x = position_to_play%3*2+3;
        int inception1 = v->p_uttt->inception_level == 1;
        while(ch!=' ' && !played)
        {
            mvwprintw(stdscr,0,0,s);
            print_utictactoe(v);
            if(!inception1)
                print_tictactoe(v,position);
            if(position_to_play!=FREE)
            {
                wattron(v->w_uttt, COLOR_PAIR(2));
                mvwaddch(v->w_uttt,y,x,'.');
                wattroff(v->w_uttt, COLOR_PAIR(2));
            }
            if(!inner || inception1)
            {
                wattron(v->w_uttt, COLOR_PAIR(1));
                mvwaddch(v->w_uttt,current_pos_Y,current_pos_X, get_cell(v->p_uttt->outer_tictactoe,position));
                wattroff(v->w_uttt, COLOR_PAIR(1));
            }
            else
            {
                wattron(v->w_uttt, COLOR_PAIR(1));
                mvwaddch(v->w_uttt,current_outer_position_Y,current_outer_position_X, get_cell(v->p_uttt->outer_tictactoe,position));
                wattroff(v->w_uttt, COLOR_PAIR(1)); 
                wattron(v->w_ttt, COLOR_PAIR(1));
                mvwaddch(v->w_ttt,current_pos_Y,current_pos_X,get_cell(v->p_uttt->inner_tictactoes[position],inner_position));
                wattroff(v->w_ttt, COLOR_PAIR(1));  
            }

                wrefresh(v->w_uttt);
                if(!inception1)
                    wrefresh(v->w_ttt);
                wrefresh(stdscr);
                ch = wgetch(stdscr);
                switch(ch)
    	       {	
                    case KEY_BACKSPACE:
                    case 127:
                    case '\b':
                        inner = 0;
                        current_pos_X=current_outer_position_X;
                        current_pos_Y=current_outer_position_Y;
                        break;
                    case KEY_RIGHT: 
                        if(current_pos_X+2<8)
                        {
                            current_pos_X=current_pos_X+2;
                            
                            if(!inner)
                            {
                                position+=1;
                                if(!inception1)
                                {
                                    print_tictactoe(v,position);
                                    wrefresh(v->w_ttt);
                                }
                                
                            }
                            else
                                inner_position+=1;
                        }
                       
                        break;
                    case KEY_LEFT: 
                        if(current_pos_X-2>2){
                            current_pos_X=current_pos_X-2;
                           
                            if(!inner)
                            {
                                position-=1;
                                if(!inception1)
                                {
                                    print_tictactoe(v,position);
                                    wrefresh(v->w_ttt);
                                }
                             
                            }
                            else
                                inner_position-=1;
                        }
                        
                        break;
                    case KEY_DOWN:
                        if(current_pos_Y+2<8)
                        {
                            current_pos_Y+=2;
                            
                            if(!inner)
                            {
                                position+=3;
                                if(!inception1)
                                {
                                    print_tictactoe(v,position);
                                    wrefresh(v->w_ttt);
                                }
                            }
                            else
                                inner_position+=3;
                        }
    
                        break;
                    case KEY_UP:
                        if(current_pos_Y-2>1)
                        {
                            current_pos_Y-=2;
                            
                            if(!inner)
                            {
                                position-=3;
                                if(!inception1)
                                {
                                    print_tictactoe(v,position);
                                    wrefresh(v->w_ttt);
                                }
                            }
                            else
                                inner_position-=3;
                        }
    
                        break;
                    case 'u':
                        undo_last_move(v->p_uttt);
                        played = 1;
                        m->outer_position = NONE;
                        m->inner_position = NONE;
                        break;
                    case KEY_ENTER:
                    case '\n':
                        if(inception1)
                        {
                            m->inner_position = position;
                            m->outer_position = position;
                            played = 1;
                        }
                        else
                        {
                            m->outer_position = position;
                            if(inner)
                            {
                                m->inner_position = inner_position;
                                played = 1;
                            }
                            inner=1;
                            current_outer_position_X=current_pos_X;
                            current_outer_position_Y=current_pos_Y;
                            current_pos_X=3;
                            current_pos_Y=2;
                        }
                        break;
                    case 'q':
                    endwin();
                    exit(1);
                    break;
	       }
        }
        free(s);
    }
    
    
}

void display_evaluate(p_view v)
{
    mvwprintw(stdscr, 13 , 1 , "EVALUTE : %d",evaluate_position(v->p_uttt->outer_tictactoe));
}

void display_winner(p_view v)
{
    mvwprintw(stdscr, 13 , 1 , "WINNER : %c",v->p_uttt->outer_tictactoe->winner);
    print_utictactoe(v);
    wrefresh(v->w_uttt);
    if(v->p_uttt->inception_level > 1)
    {
        print_tictactoe(v,v->p_uttt->history->last_move->outer_position);
        wrefresh(v->w_ttt);
    }
    wrefresh(stdscr);
}

void display_string(p_view v,char* c)
{
    mvwprintw(stdscr, 15 , 1 , c);
}

void free_view(p_view s){

    delwin(s->w_uttt);
    delwin(s->w_ttt);
    endwin();

    free(s);
}
