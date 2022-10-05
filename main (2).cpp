#include <simplecpp>
#include "shooter.h"
#include "bubble.h"
#include <string>
/* Simulation Vars */
const double STEP_TIME = 0.03;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);
const int RIGHT_MARGIN = 400;
int level = 1;
int win_or_lose;   //0 for loss, 1 for win
int c = 0;   //0 if game has just begun, 1 otherwise

/* function to check the relative position of bullets and bubbles*/
void ifshoot(vector<Bubble> &bubbles,vector<Bullet> &bullets)
{
    for (unsigned int i=0;i < bullets.size();i++)
    {
        int p=0;
        double bulletx = bullets[i].get_center_x();
        double bullety = bullets[i].get_center_y();
        for (unsigned int j=0;j < bubbles.size();j++)
        {
            double bubblex = bubbles[j].get_center_x();
            double bubbley = bubbles[j].get_center_y();

            if (bubbles[j].get_radius()<=13)p=15;
            else p=30;   //bigger margin for bigger bubbles

            if (abs(bulletx - bubblex)<p && abs(bullety - bubbley)<p && (bubbles[j].get_radius()<=13))
            {
                bubbles.erase(bubbles.begin()+j);   //erase bullets and bubbles if position matches
                bullets.erase(bullets.begin()+i);
            }

            else if (abs(bulletx - bubblex)<p && abs(bullety - bubbley)<p && (  //spiltting of larger bubbles
            bubbles[j].get_radius()>13)){
                double x = bubbles[j].get_center_x();
                double y = bubbles[j].get_center_y();
                double vx = bubbles[j].get_vx();
                double vy = bubbles[j].get_vy();
                double radius = bubbles[j].get_radius()/2.0;

                bubbles.erase(bubbles.begin()+j);   //erase the previous bubble, create two new ones
                bullets.erase(bullets.begin()+i);

                bubbles.push_back(Bubble(x, y, radius , -vx, vy, COLOR(255/level,0,255)));
                bubbles.push_back(Bubble(x, y, radius , vx, vy, COLOR(255/level,0,255)));

                j++;
                continue;
            }
        }
    }
}

void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles,vector<Bullet> &bullets,Shooter shooter){
    // move all bubbles
    //int y = 0;
    //if (bubbles.size==0)nextlevel(bubbles,bullets,shooter);
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
        ifshoot(bubbles,bullets);//check for posn wrt bullets every time bubbles move
    }
}

/*level implementation*/
void next_level(int lvl,vector<Bubble> &bubbles)
{
    if (level == 2){
        string msg_level("~MEDIUM LEVEL~");
        Text charPressed(RIGHT_MARGIN-175, BOTTOM_MARGIN-250, msg_level);
        charPressed.setMessage(msg_level);
        wait(1);     //display the message for 1 second
        msg_level = " ";
        charPressed.setMessage(msg_level);
    }
    else if (level == 3){
        string msg_level("~HARD LEVEL~");
        Text charPressed(RIGHT_MARGIN-175, BOTTOM_MARGIN-250, msg_level);
        charPressed.setMessage(msg_level);
        wait(1);
        msg_level = " ";
        charPressed.setMessage(msg_level);
    }

    //create new bubbles depending on level
    int x = 1;
    for (int i = 1; i<=level; i++)
    {
        bubbles.push_back(Bubble(WINDOW_X/(i+x)+50, BUBBLE_START_Y+50*i, BUBBLE_DEFAULT_RADIUS + 5*i,
         -x*(BUBBLE_DEFAULT_VX+50*level), 0, COLOR(255/level,0,255)));
        x *= -1;
    }

}

/*relative posn of bubbles and the shooter*/
int bub_shoot(vector<Bubble> &bubbles,vector<Bullet> &bullets,Shooter shooter)
{
    for (unsigned int i = 0; i<bubbles.size();i++)
    {
        double headx = shooter.get_head_center_x();
        double heady = shooter.get_head_center_y();
        double bodyx = shooter.get_body_center_x();
        double bodyy = shooter.get_body_center_y();
        double bubblex = bubbles[i].get_center_x();
        double bubbley = bubbles[i].get_center_y();

        /*shooter is dead if  the bubbles touch either head or body*/

        if ((abs(bodyx - bubblex) < 9+bubbles[i].get_radius() && abs(bodyy - bubbley) < 15+bubbles[i].get_radius())||(
        abs(headx - bubblex) < 6 + bubbles[i].get_radius() && abs(heady - bubbley) < 6 + bubbles[i].get_radius()))
        {
            return -1;
        }
    }
    return 0;

}


vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0+100, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,0,255)));
    //bubbles.push_back(Bubble(WINDOW_X/4.0+200, BUBBLE_START_Y+100, 10, -BUBBLE_DEFAULT_VX, 0, COLOR(255,0,255)));
    bubbles.push_back(Bubble(WINDOW_X/8.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,0,255)));
    //bubbles.push_back(Bubble(-WINDOW_X/16.0, BUBBLE_START_Y+200, 20, BUBBLE_DEFAULT_VX, 0, COLOR(255,0,255)));
    //bubbles.push_back(Bubble(-WINDOW_X/8.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,0,255)));
    //bubbles.push_back(Bubble(-WINDOW_X/4.0, BUBBLE_START_Y+10, 20, BUBBLE_DEFAULT_VX, 0, COLOR(255,0,255)));
    return bubbles;
}


int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);


    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);


            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        //print easy level if the game has just begun
        if (level == 1 && c == 0){
            string msg_easy("EASY LEVEL!");
            Text charPressed1(RIGHT_MARGIN-175, BOTTOM_MARGIN-250, msg_easy);
            charPressed1.setMessage(msg_easy);
            wait(1);
            charPressed1.setMessage("");
            c++;
        }

        //check if the game is over
        if (bub_shoot(bubbles,bullets,shooter) == -1){
            win_or_lose = 0;
            break;
            }
        // Update the bubbles
        move_bubbles(bubbles,bullets,shooter);

        // Update the bullets
        move_bullets(bullets);

        //check if there are no bubbles left on the screen
        //call new level
        if (bubbles.size()==0 && level!=3){
            for (unsigned int i = 0; i<bullets.size();i++)
            {
                bullets.erase(bullets.begin()+i);
            }
            level++;
            next_level(level,bubbles);
        }

        //check if all the levels are over
        if (level == 3 && bubbles.size()==0){
            win_or_lose = 1;
            break;
        }

        wait(STEP_TIME);
    }

//display outcome of the game
if (win_or_lose == 1){
    string msg_won("CONGRATULATIONS! YOU WON!");
    Text charPressed1(RIGHT_MARGIN-175, BOTTOM_MARGIN-250, msg_won);
    charPressed1.setMessage(msg_won);
    getClick();}
else if (win_or_lose == 0){
    string msg_lost("GAME OVER!");
    Text charPressed2(RIGHT_MARGIN-175, BOTTOM_MARGIN-250, msg_lost);
    charPressed2.setMessage(msg_lost);
    getClick();}

}
