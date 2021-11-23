#include <iostream>
#include <thread>
#include <array>
#include "networking/connect.hpp"
#include "nogui/board.hpp"
#include <curses.h>

void connected(Connection* connect, bool* finished)
{
    connect->receive_data();
    *finished = true;
}

void print_board(Board* board, int selected)
{
    for(int y = 0;y < 8;y++)
    {
        for(int x = 0;x < 8;x++)
        {
            int test = board->getBoard(y*8+x);
            if(test < 8 && test >= 0)
                attron(COLOR_PAIR(1+(y*8+x == selected ? 3 : 0)));
            else if(test >= 8)
                attron(COLOR_PAIR(2+(y*8+x == selected ? 3 : 0)));
            else if(y*8+x == selected)
                attron(COLOR_PAIR(3));
            move(y,x);
            switch (test)
            {
            case -1:
                addch('.'); break;
            case 0:
            case 8:
                addch(ACS_UARROW); break;
            case 1:
            case 9:
                addch(ACS_URCORNER); break;
            case 2:
            case 10:
                addch(ACS_RARROW); break;
            case 3:
            case 11:
                addch(ACS_LRCORNER); break;
            case 4:
            case 12:
                addch(ACS_DARROW); break;
            case 5:
            case 13:
                addch(ACS_LLCORNER); break;
            case 6:
            case 14:
                addch(ACS_LARROW); break;
            case 7:
            case 15:
                addch(ACS_ULCORNER); break;
            }
            if(test < 8 && test >= 0)
                attroff(COLOR_PAIR(1+(y*8+x == selected ? 3 : 0)));
            else if(test >= 8)
                attroff(COLOR_PAIR(2+(y*8+x == selected ? 3 : 0)));
            else if(y*8+x == selected)
                attroff(COLOR_PAIR(3));
        }
    }
    refresh();
}

void start_curses()
{
    initscr();
    noecho();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(3+1, COLOR_BLACK, COLOR_BLUE);
    init_pair(3+2, COLOR_BLACK, COLOR_RED);
}

void moveHandle(Board* board, Connection* connect, int& selected)
{
    int input = getch();
    int moveTable[9] = {5,4,3,6,-1,2,7,0,1};
    switch (input)
    {
    case 65:
    case KEY_UP:
        selected-=8;
        break;
    case 66:
    case KEY_DOWN:
        selected+=8;
        break;
    case 68:
    case KEY_LEFT:
        selected--;
        break;
    case 67:
    case KEY_RIGHT:
        selected++;
        break;
    case 49:
    case 50:
    case 51:
    case 52:
    case 54:
    case 55:
    case 56:
    case 57:
        connect->send_data({selected,moveTable[input-49]});
        mvprintw(10,10,"%d",selected);
        board->set(selected,moveTable[input-49]);
        break;
    default:
        break;
    }
    if(selected<0) {selected+=64;}
    else if(selected>=64) {selected-=64;}
    connect->send_data({selected,-1});
}

int main()
{
    int menu;
    std::cout << "1. Host game" << std::endl << "2. Join game" << std::endl;
    for(menu = 0;;)
    {
        menu = std::cin.get();
        std::cout << menu << std::endl;
        switch (menu)
        {
        case 49:
        case 50:
            break;
        default:
            continue;
        }
        break;
    }
    Connection *connect;
    int selected = 0;
    Board board;
    if(menu == 49)
    {
        std::cout << "Waiting for other person to join..." << std::endl;
        connect = new Server();
        std::cout << "Person joined!" << std::endl;

        start_curses();
        while(board.check() == 0)
        {
            mvprintw(0,9,"It is ");
            if(board.getTurn() == 0)
            {
                attron(COLOR_PAIR(3+1));
                printw("YOUR");
                attroff(COLOR_PAIR(3+1));
            }
            else
            {
                attron(COLOR_PAIR(2));
                printw("opponent's");
                attroff(COLOR_PAIR(2));
            }
            printw(" turn.");
            print_board(&board,selected);
            if(board.getTurn() == 0)
                moveHandle(&board,connect,selected);
            else
            {
                std::array<int,2> data = connect->receive_data();
                if(data[1] == -1)
                    selected = data[0];
                else
                    board.set(data[0],data[1]);
            }
        }
    }
    else if(menu == 50)
    {
        std::cout << "Enter other player's ip" << std::endl;
        std::string ip;
        std::cin >> ip;
        connect = new Client(ip);
        std::cout << "You joined!" << std::endl;

        start_curses();
        while(board.check() == 0)
        {
            mvprintw(0,9,"It is ");
            if(board.getTurn() == 1)
            {
                attron(COLOR_PAIR(3+2));
                printw("YOUR");
                attroff(COLOR_PAIR(3+2));
            }
            else
            {
                attron(COLOR_PAIR(1));
                printw("opponent's");
                attroff(COLOR_PAIR(1));
            }
            printw(" turn.");
            print_board(&board, selected);
            mvprintw(11,10,"%d",selected);
            if(board.getTurn() == 1)
                moveHandle(&board,connect,selected);
            else
            {
                std::array<int,2> data = connect->receive_data();
                if(data[1] == -1)
                    selected = data[0];
                else
                    board.set(data[0],data[1]);
            }
        }
    }
    clear();
    if(board.check()>0)
        attron(COLOR_PAIR(3+1));
    else
        attron(COLOR_PAIR(3+2));
    mvprintw(1,0,"%d",menu-49);
    if((board.check()>0?0:1) == menu-49)
        mvprintw(0,0,"YOU");
    else
        mvprintw(0,0,"OPPONENT");
    if(board.check()>0)
        attron(COLOR_PAIR(3+1));
    else
        attron(COLOR_PAIR(3+2));
    printw(" won with a score of %d",(board.check()>0?board.check():-board.check()));
    refresh();
    getch();
    endwin();
    return 0;
}