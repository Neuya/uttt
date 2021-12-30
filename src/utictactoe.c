#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include "common.h"
#include "utictactoe.h"
#include "model.h"
#include "view.h"
#include "ia.h"

int main(int argc,char** argv)
{


  long optc;
  int verbose=0,contest_mode=0; //boolean
  size_t board_level=2;
  int tactic_to_useX=-1,tactic_to_useO=-1;
  char* file;

  const struct option long_opts[]=
  {
    {"help", no_argument, 0, 'h'},
    {"version",no_argument,0,'V'},
    {"verbose",no_argument,0,'v'},
    {"inception-level",required_argument,0,'i'},
    {"cross-ai",required_argument,0,'x'},
    {"round-ai",required_argument,0,'o'},
    {"contest",required_argument,0,'c'},
  };


  while ((optc = getopt_long (argc, argv,"hVvx:o:c:i:",long_opts, NULL)) != -1){
    switch (optc) {
      case 'h':
                printf("Usage: utictactoe ([-i LEVEL|-x N|-o N|-v|-V|-h] | -c FILE)\n");
                printf("Play a utictactoe game with human or program players.\n");
                printf("-i, --inception-level LEVEL deepness of the game (min=1, max=3 (default=2))\n");
                printf("-x, --cross-ai N set tactic of cross player 'X'\n");
                printf("-o, --round-ai N set tactic of round player 'O'\n");
                printf("-c, --contest enable 'contest' mode\n");
                printf("-v, --verbose verbose output\n");
                printf("-V, --version display version and exit\n");
                printf("-h, --help display this help and exit\n");
                printf("Tactics list: random (0), clever (1)\n");
                exit(EXIT_SUCCESS);
                break;
      case 'V':
                printf("Utictactoe V %d.%d.%d\n",VERSION,SUBVERSION,REVISION);
                printf("This software allows to play to ultimate tictactoe game\n");
                exit(EXIT_SUCCESS);
                break;
      case 'v':
                verbose = 1;
                printf("Verbose : %d\n",verbose);
                break;
      case 'i':
                board_level = atoi(optarg);
                printf("Board_size : %lu\n",board_level);
                break;
      case 'x':
                tactic_to_useX = atoi(optarg);
                if(tactic_to_useX!=0 && tactic_to_useX!=1)
                  err(EXIT_FAILURE,"Tactics should be 1 or 0, not %d\nExiting..",tactic_to_useX);
                printf("Cross AI (tactic : %d)\n",tactic_to_useX);
                break;
      case 'o':
                tactic_to_useO = atoi(optarg);
                if(tactic_to_useO!=0 && tactic_to_useO!=1)
                  err(EXIT_FAILURE,"Tactics should be 1 or 0, not %d\nExiting..",tactic_to_useO);
                printf("Round AI (tactic : %d)\n",tactic_to_useO);
                break;
      case 'c':
                contest_mode = 1;
                if((file = optarg)==NULL)
                  err(1,"Contest mode needs a file as argument!");
                if(access(file,R_OK)==-1)
                  err(1,"Contest file '%s' unreachable or unreadable",file);
                break;

      default:
                printf("\nFailure to execute : try ./utictactoe -h or --help for more informations\n");
                exit (EXIT_FAILURE);
    }
  }

  printf("\n\nChosen options : \n\n");
  printf("Verbose : %d\n",verbose);
  printf("Board level : %lu\n",board_level);
  if(tactic_to_useX!=-1)
    printf("Tactic for cross player : %d   (0=Random)|(1=Clever)\n",tactic_to_useX);
  if(tactic_to_useO!=-1)
    printf("Tactic for round player : %d   (0=Random)|(1=Clever)\n",tactic_to_useO);
  if(contest_mode)
    printf("Contest mode on, file : %s\n",file);
  else
    printf("Contest mode off\n");



  /*s_move *test_move = create_empty_move();
  free_move(test_move);
  s_tictactoe *test_ttt = create_empty_tictactoe();
  free_tictactoe(test_ttt);
  s_utictactoe *test_uttt;
  test_uttt = create_empty_utictactoe(1);
  free_utictactoe(test_uttt);
  test_uttt = create_empty_utictactoe(2);
  free_utictactoe(test_uttt);
  test_uttt = create_empty_utictactoe(2);
  printf("Next player to play : %c\n",get_next_player_to_play(test_uttt));
  draw_utictactoe(test_uttt);
  test_move = create_empty_move();
  test_move->outer_position = TOPLEFT;
  test_move->inner_position = TOPLEFT;
  test_move->player = PLAYER_X;
  //play_move(test_uttt,test_move);
  free_move(test_move);
  draw_utictactoe(test_uttt);
  free_utictactoe(test_uttt);*/

  /*srand(time(NULL));
        s_utictactoe *s = create_empty_utictactoe(1); //replace by 2 for uttt play
        s_move *m = create_empty_move();
        m->outer_position = TOPLEFT + rand() % TICTACTOE_SIZE;
        int i=0;
        while (s->outer_tictactoe->winner == NOBODY) {
            printf("\nINNER TICTACTOE : \n");
            draw_utictactoe(s);
            m->player = get_next_player_to_play(s);
            if(m->player == PLAYER_O)
            {
              m->inner_position = TOPLEFT + rand() % TICTACTOE_SIZE;
              m->outer_position = get_next_outer_position(s)==FREE?m->inner_position:get_next_outer_position(s);
            }
            else
            {
              m = best_move_clever(s);
            }
            //m = best_move_clever(s);
            play_move(s, m);
            printf("\nOUTER TICTACTOE : \n");
            draw_tictactoe(s->outer_tictactoe);
            printf("\n");
            i++;
            printf("EVALUATE : %d\n",evaluate_position(s->outer_tictactoe));
            getchar();
            //print_tree(test,0);
           // getchar();
            //scanf("%c",&c);
        }
        draw_utictactoe(s);
        draw_utictactoe_history(s);
        printf("The winner is : %c\n", s->outer_tictactoe->winner);
        free_move(m);
        free_utictactoe(s);*/
       /* FILE *fd = fopen("./test.txt","w");
        if(fd){
          save_a_utictactoe_to_file(fd,s);
          fclose(fd);
        }
        free_move(m);
        free_utictactoe(s);*/

  /*s_utictactoe *s = create_empty_utictactoe(1);
  s_move *m = create_empty_move();
  p_view v = create_view(s);
  while (s->outer_tictactoe->winner == NOBODY) {
    m->player = get_next_player_to_play(s);
    m->outer_position=get_next_outer_position(s);
    set_next_player_move(m,v); 
    //printf("MOVE %d %d %c",m->inner_position,m->outer_position,m->player);
    play_move(s, m);
  }
  free_view(v);
  draw_utictactoe(s);
  draw_utictactoe_history(s);
  printf("The winner is : %c\n", s->outer_tictactoe->winner);
  free_move(m);
  free_utictactoe(s);*/
  

  e_tactic tactic_cross=HUMAN;
  e_tactic tactic_round=CLEVER;

  s_utictactoe *s = create_empty_utictactoe(board_level);
  s_move *m = create_empty_move();
  p_view v = create_view(s);
  while (s->outer_tictactoe->winner == NOBODY) {
    m->player = get_next_player_to_play(s);
    m->outer_position=get_next_outer_position(s);
    if(m->player == PLAYER_X)
      set_next_player_move(m,v,tactic_cross);
    else
      set_next_player_move(m,v,tactic_round);
    play_move(s, m);  
    //display_evaluate(v);
    //display_winner(v);
    //m->outer_position = m->inner_position;    
  }
  display_winner(v);
  getchar();//Little sleep before quitting
  free_view(v);
  draw_utictactoe_history(s);
  printf("The winner is : %c\n", s->outer_tictactoe->winner);
  free_move(m);
  free_utictactoe(s);






  return 1;
}
