#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdio>

bool affScores(SDL_Surface *screen, int const W, int const H)
{
    SDL_Surface *text = NULL;
    TTF_Font *police = NULL;
    TTF_Font *police2 = NULL;
    police = TTF_OpenFont("polices/Ubuntu-MediumItalic.ttf",((46 * H) / 1200));
    police2 = TTF_OpenFont("polices/LiberationSerif-Regular.ttf",((46 * H) / 1200));
    SDL_Color couleur = {153,176,0};
    SDL_Color couleur2 = {76,88,0};
    text = rotozoomSurfaceXY(TTF_RenderText_Blended(police,"Records", couleur),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Rect pos;
    pos.x = (W / 2) - (text->w / 2);
    pos.y = ((100 * H) / 840);
    SDL_BlitSurface(text,NULL,screen,&pos);
    std::string fichier = "data/joueurs.txt";
    std::ifstream flux(fichier.c_str());
    pos.x = ((200 * W) / 1200);
    std::string arg;
    int i = 1;
    while(std::getline(flux,arg))
    {
        std::string arg2 = std::to_string(i) + ") " + arg;
        pos.y = ((125 * H) / 840) + (i * ((50 * H) / 840));
        SDL_BlitSurface(rotozoomSurfaceXY(TTF_RenderText_Blended(police2,arg2.c_str(),couleur2),0,double(W) / 1200, double(H) / 840 , 0),NULL,screen,&pos);
        i += 1;
    }
    pos.x = ((900 * W) / 1200);
    fichier = "data/scores.txt";
    flux.close();
    flux = std::ifstream(fichier.c_str());
    i = 1;
    while(std::getline(flux,arg))
    {
        pos.y = ((125 * H) / 840) + (i * ((50 * H) / 840));
        SDL_BlitSurface(rotozoomSurfaceXY(TTF_RenderText_Blended(police2,arg.c_str(),couleur2),0,double(W) / 1200, double(H) / 840 , 0),NULL,screen,&pos);
        i += 1;
    }
    flux.close();
    SDL_Surface *Quit = NULL;
    Quit = rotozoomSurfaceXY(IMG_Load("images/boutonQuit.png"),0,double(W) / 1200, double(H) / 840,0);
    pos.y = ((710 * H) / 840);
    SDL_Surface *QuitOn = NULL;
    QuitOn = rotozoomSurfaceXY(IMG_Load("images/boutonQuitOn.png"),0,double(W) / 1200, double(H) / 840,0);
    pos.x = (W / 2) - (Quit->w / 2);
    SDL_BlitSurface(Quit,NULL,screen,&pos);
    SDL_Flip(screen);
    SDL_Event event;
    bool continuer = true;
    bool h = true;
    bool On = false;
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION :
                if((((event.motion.x >= ((W / 2) - (Quit->w / 2)))&&(event.motion.x <= ((W / 2) - (Quit->w / 2)) + ((235 * W) / 1200)))&&((event.motion.y >= ((710 * H) / 840))&&(event.motion.y <= ((710 * H) / 840) + ((87 * H) / 840))))&&(!On))
                {
                    SDL_BlitSurface(QuitOn,NULL,screen,&pos);
                    On = true;
                }
                else if((((event.motion.x < ((W / 2) - (Quit->w / 2)))||(event.motion.x > ((W / 2) - (Quit->w / 2)) + ((235 * W) / 1200)))||((event.motion.y < ((710 * H) / 840))||(event.motion.y > ((710 * H) / 840) + ((87 * H) / 840))))&&(On))
                {
                    SDL_BlitSurface(Quit,NULL,screen,&pos);
                    On = false;
                }
                break;
            case SDL_QUIT :
                continuer = false;
                h = false;
                break;
            case SDL_KEYDOWN :
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE :
                        continuer = false;
                        h = false;

                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN :
                if(On)
                    continuer = false;
                break;
        }
        SDL_Flip(screen);
    }
    SDL_FreeSurface(Quit);
    SDL_FreeSurface(QuitOn);
    SDL_FreeSurface(text);
    TTF_CloseFont(police);
    TTF_CloseFont(police2);
    return h;
}

void pushBackIntToString(std::string &u, int y)
{
    if(y != 10)
    {
        while(y > 0)
        {
            u.push_back(char((y % 10) + 48));
            y -= y % 10;
            y /= 10;
        }
    }
    else
        u += "10";
}

bool affTutoriel(SDL_Surface *screen, int const W, int const H)
{
    bool h = true;
    SDL_Surface *baldi = NULL;
    baldi = rotozoomSurfaceXY(IMG_Load("images/tutoriel.png"),0,(double(W) / 1200) * 0.75,(double(H) / 840) * 0.75,0);
    bool continuer = true;
    SDL_Event event;
    SDL_Surface *backup = rotozoomSurfaceXY(IMG_Load("images/backup.png"),0,double(W) / 1200, double(H) / 840, 0);
    int i = 1;
    SDL_Rect pos;
    int TPrecedent = SDL_GetTicks();
    int TActuel;
    TTF_Font *police = NULL;
    police = TTF_OpenFont("polices/Ubuntu-MediumItalic.ttf",((46 * H) / 1200));
    TTF_Font *police2 = NULL;
    police2 = TTF_OpenFont("polices/LiberationSerif-Regular.ttf",((39 * H) / 1200));
    SDL_Color couleur = {153,176,0};
    SDL_Color couleur2 = {76,88,0};
    SDL_Surface *text;
    text = rotozoomSurfaceXY(TTF_RenderText_Blended(police,"Tutoriel", couleur),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *body1;
    body1 = rotozoomSurfaceXY(TTF_RenderText_Blended(police2,"Utilise les fleches directionelles pour te diriger. Ton but est de manger les spheres.",couleur2),0,double(W) / 1200, double(H) / 840,0);
    SDL_Surface *body2;
    body2 = rotozoomSurfaceXY(TTF_RenderText_Blended(police2,"Tu meurs si tu touches un mur ou toi-meme. Appuie sur ENTER pour mettre le jeu en",couleur2),0,double(W) / 1200, double(H) / 840,0);
    SDL_Surface *body3 = NULL;
    body3 = rotozoomSurfaceXY(TTF_RenderText_Blended(police2,"pause.",couleur2),0,double(W) / 1200, double(H) / 840, 0);
    SDL_Surface *Quit = NULL;
    Quit = rotozoomSurfaceXY(IMG_Load("images/boutonQuit.png"),0,double(W) / 1200, double(H) / 840,0);
    SDL_Surface *QuitOn = NULL;
    QuitOn = rotozoomSurfaceXY(IMG_Load("images/boutonQuitOn.png"),0,double(W) / 1200, double(H) / 840,0);
    bool On = false;
    pos.x = ((20 * W) / 1200);
    pos.y = ((200 * H) / 840);
    SDL_BlitSurface(body1,NULL,screen,&pos);
    pos.y = ((200 * H) / 840) + ((40 * H) / 840);
    SDL_BlitSurface(body2,NULL,screen,&pos);
    pos.y = ((200 * H) / 840) + ((80 * H) / 840);
    SDL_BlitSurface(body3,NULL,screen,&pos);
    pos.x = (W / 2) - (baldi->w / 2);
    pos.y = ((H / 2) - (baldi->h / 2)) + ((65 * H) / 840);
    SDL_BlitSurface(baldi,NULL,screen,&pos);
    pos.x = (W / 2) - (Quit->w / 2);
    pos.y = ((710 * H) / 840);
    SDL_BlitSurface(Quit,NULL,screen,&pos);
    int x(0), y(0);
    while(continuer)
    {
        pos.x = (W / 2) - (text->w / 2);
        pos.y = ((100 * H) / 840);
        SDL_BlitSurface(text,NULL,screen,&pos);
        pos.x = (W / 2) - (Quit->w / 2);
        pos.y = ((710 * H) / 840);
        SDL_Flip(screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT :
                continuer = false;
                h = false;
                break;
            case SDL_MOUSEMOTION :
                x = event.motion.x;
                y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN :
                continuer = false;
                break;
            case SDL_KEYDOWN :
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE :
                        continuer = false;
                        h = false;
                        break;
                }
                break;
        }
        if((((x >= ((W / 2) - (Quit->w / 2)))&&(x <= ((W / 2) - (Quit->w / 2)) + ((235 * W) / 1200)))&&((y >= ((710 * H) / 840))&&(y <= ((710 * H) / 840) + ((87 * H) / 840))))&&(!On))
        {
            pos.x = (W / 2) - (Quit->w / 2);
            pos.y = ((710 * H) / 840);
            SDL_BlitSurface(QuitOn,NULL,screen,&pos);
            On = true;
        }
        else if((((x < ((W / 2) - (Quit->w / 2)))||(x > ((W / 2) - (Quit->w / 2)) + ((235 * W) / 1200)))||((y < ((710 * H) / 840))||(y > ((710 * H) / 840) + ((87 * H) / 840))))&&(On))
        {
            pos.x = (W / 2) - (Quit->w / 2);
            pos.y = ((710 * H) / 840);
            SDL_BlitSurface(Quit,NULL,screen,&pos);
            On = false;
        }
        SDL_Flip(screen);
    }
    TTF_CloseFont(police);
    TTF_CloseFont(police2);
    SDL_FreeSurface(backup);
    SDL_FreeSurface(text);
    SDL_FreeSurface(body1);
    SDL_FreeSurface(body2);
    SDL_FreeSurface(body3);
    SDL_FreeSurface(Quit);
    SDL_FreeSurface(QuitOn);
    SDL_FreeSurface(baldi);
    return h;
}


enum direc{DROITE, GAUCHE, BAS, HAUT};

class VOLUME
{
    public:
        static int VOLUME_SONORE;
};

class serpent
{
public :
    serpent(int const x, int const y, int const H, int const W) : m_H(H), m_W(W)
    {
        sandvich = rotozoomSurfaceXY(IMG_Load("images/sandvich.png"),0,1.5,1.5,0);
        positions.push_back(std::pair<int,int>(x,y));
        positions.push_back(std::pair<int,int>(x - 1,y));
        positions.push_back(std::pair<int,int>(x - 2,y));
        positions.push_back(std::pair<int,int>(x - 3,y));
        bool h = true;
        while(h)
        {
            food.first = (rand() % 29);
            food.second = (rand() % 26);
            bool j = false;
            for(int i = 0; i < positions.size(); ++i)
            {
                if(food == positions[i])
                    j = true;
            }
            h = j;
        }
        corps[0] = rotozoomSurfaceXY(IMG_Load("images/teteBas.png"),0,1.5,1.5,0); //bas
        corps[1] = rotozoomSurfaceXY(IMG_Load("images/teteHaut.png"),0,1.5,1.5,0); //haut
        corps[2] = rotozoomSurfaceXY(IMG_Load("images/teteDroite.png"),0,1.5,1.5,0); //droite
        corps[3] = rotozoomSurfaceXY(IMG_Load("images/teteGauche.png"),0,1.5,1.5,0); //gauche
        corps[4] = rotozoomSurfaceXY(IMG_Load("images/queueBas.png"),0,1.5,1.5,0);
        corps[5] = rotozoomSurfaceXY(IMG_Load("images/queueHaut.png"),0,1.5,1.5,0);
        corps[6] = rotozoomSurfaceXY(IMG_Load("images/queueDroite.png"),0,1.5,1.5,0);
        corps[7] = rotozoomSurfaceXY(IMG_Load("images/queueGauche.png"),0,1.5,1.5,0);
        corps[8] = rotozoomSurfaceXY(IMG_Load("images/virageHG.png"),0,1.5,1.5,0);
        corps[9] = rotozoomSurfaceXY(IMG_Load("images/virageHD.png"),0,1.5,1.5,0);
        corps[10] = rotozoomSurfaceXY(IMG_Load("images/virageBG.png"),0,1.5,1.5,0);
        corps[11] = rotozoomSurfaceXY(IMG_Load("images/virageBD.png"),0,1.5,1.5,0);
        corps[12] = rotozoomSurfaceXY(IMG_Load("images/corpsV.png"),0,1.5,1.5,0);
        corps[13] = rotozoomSurfaceXY(IMG_Load("images/corpsH.png"),0,1.5,1.5,0);
    }
//https://www.youtube.com/watch?v=2MtOpB5LlUA
    ~serpent()
    {
        SDL_FreeSurface(sandvich);
        for(unsigned int i = 0; i < 14; ++i)
        {
            SDL_FreeSurface(corps[i]);
        }
    }
    static int speed;
    void bouge(direc &d)
    {
        switch(d)
        {
            case HAUT :
                if(std::pair<int,int>(positions[0].first,positions[0].second - 1) == food)
                {
                    positions.push_back(positions[positions.size() - 1]);
                    for(int i = positions.size() - 2; i > 0; --i)
                    {
                        positions[i] = positions[i - 1];
                    }
                    positions[0] = std::pair<int,int>(positions[0].first , positions[0].second - 1);
                    TPFood();
                }
                else
                {
                    for(int i = positions.size() - 1; i > 0; --i)
                    {
                        positions[i] = positions[i - 1];
                    }
                    positions[0] = std::pair<int,int>(positions[0].first , positions[0].second - 1);
                }
                break;
            case BAS :
                if(std::pair<int,int>(positions[0].first,positions[0].second + 1) == food)
                {
                    positions.push_back(positions[positions.size() - 1]);
                    for(int i = positions.size() - 2; i > 0; --i)
                    {
                        positions[i] = positions[i - 1];
                    }
                    positions[0] = std::pair<int,int>(positions[0].first , positions[0].second + 1);
                    TPFood();
                }
                else
                {
                    for(int i = positions.size() - 1; i > 0; --i)
                    {
                        positions[i] = positions[i - 1];
                    }
                    positions[0] = std::pair<int,int>(positions[0].first , positions[0].second + 1);
                }
                break;
            case DROITE :
                if(std::pair<int,int>(positions[0].first + 1,positions[0].second) == food)
                {
                    positions.push_back(positions[positions.size() - 1]);
                    for(int i = positions.size() - 2; i > 0; --i)
                    {
                        positions[i] = positions[i - 1];
                    }
                    positions[0] = std::pair<int,int>(positions[0].first + 1 , positions[0].second);
                    TPFood();
                }
                else
                {
                    for(int i = positions.size() - 1; i > 0; --i)
                    {
                        positions[i] = positions[i - 1];
                    }
                    positions[0] = std::pair<int,int>(positions[0].first + 1 , positions[0].second);
                }
                break;
            case GAUCHE :
                if(std::pair<int,int>(positions[0].first - 1,positions[0].second) == food)
                {
                    positions.push_back(positions[positions.size() - 1]);
                    for(int i = positions.size() - 2; i > 0; --i)
                    {
                        positions[i] = positions[i - 1];
                    }
                    positions[0] = std::pair<int,int>(positions[0].first - 1 , positions[0].second);
                    TPFood();
                }
                else
                {
                    for(int i = positions.size() - 1; i > 0; --i)
                    {
                        positions[i] = positions[i - 1];
                    }
                    positions[0] = std::pair<int,int>(positions[0].first - 1 , positions[0].second);
                }
                break;
        }
    }
    void aff(SDL_Surface *screen, SDL_Rect &POS) const
    {
        SDL_Surface *fond = NULL;
        fond = IMG_Load("images/fondJeu.png");
        SDL_Rect pos;
        for(int i = 0; positions.size() > i; ++i)
        {
            pos.x = (30*(positions[i].first - 1)) + 30;
            pos.y = (30*(positions[i].second - 1)) + 30;
            if(i == 0)
            {
                if(positions[0] == std::pair<int,int>(positions[1].first,positions[1].second + 1))
                    SDL_BlitSurface(corps[0],NULL,fond,&pos); //tete = teteBas
                else if(positions[0] == std::pair<int,int>(positions[1].first,positions[1].second - 1))
                    SDL_BlitSurface(corps[1],NULL,fond,&pos);//tete = teteHaut;
                else if(positions[0] == std::pair<int,int>(positions[1].first + 1,positions[1].second))
                    SDL_BlitSurface(corps[2],NULL,fond,&pos);//tete = teteDroite
                else if(positions[0] == std::pair<int,int>(positions[1].first - 1,positions[1].second))
                    SDL_BlitSurface(corps[3],NULL,fond,&pos);//tete = teteGauche
            }
            else if(i == positions.size() - 1)
            {
                if(positions[i] == std::pair<int,int>(positions[i - 1].first,positions[i - 1].second + 1))
                    SDL_BlitSurface(corps[4],NULL,fond,&pos);
                else if(positions[i] == std::pair<int,int>(positions[i - 1].first,positions[i - 1].second - 1))
                    SDL_BlitSurface(corps[5],NULL,fond,&pos);
                else if(positions[i] == std::pair<int,int>(positions[i - 1].first + 1,positions[i - 1].second))
                    SDL_BlitSurface(corps[6],NULL,fond,&pos);
                else if(positions[i] == std::pair<int,int>(positions[i - 1].first - 1,positions[i - 1].second))
                    SDL_BlitSurface(corps[7],NULL,fond,&pos);
            }
            else
            {
                if((positions[i].first == positions[i - 1].first)&&(positions[i].first == positions[i + 1].first))
                {
                    SDL_BlitSurface(corps[12],NULL,fond,&pos);
                }
                else if((positions[i].second == positions[i - 1].second)&&(positions[i].second == positions[i + 1].second))
                {
                    SDL_BlitSurface(corps[13],NULL,fond,&pos);
                }
                else
                {
                    if((positions[i].first == positions[i - 1].first)&&(positions[i].second == positions[i - 1].second + 1))
                    {
                        if(positions[i].first - positions[i + 1].first == 1)
                            SDL_BlitSurface(corps[8],NULL,fond,&pos);
                        else if(positions[i].first - positions[i + 1].first == -1)
                            SDL_BlitSurface(corps[9],NULL,fond,&pos);
                    }
                    else if((positions[i].first == positions[i - 1].first)&&(positions[i].second == positions[i - 1].second - 1))
                    {
                        if(positions[i].first - positions[i + 1].first == 1)
                            SDL_BlitSurface(corps[10],NULL,fond,&pos);
                        else if(positions[i].first - positions[i + 1].first == -1)
                            SDL_BlitSurface(corps[11],NULL,fond,&pos);
                    }
                    else if((positions[i].second == positions[i - 1].second)&&(positions[i].first == positions[i - 1].first + 1)) //D
                    {
                        if(positions[i].second - positions[i + 1].second == 1)
                            SDL_BlitSurface(corps[8],NULL,fond,&pos);
                        else if(positions[i].second - positions[i + 1].second == -1)
                            SDL_BlitSurface(corps[10],NULL,fond,&pos);
                    }
                    else if((positions[i].second == positions[i - 1].second)&&(positions[i].first == positions[i - 1].first - 1)) //G
                    {
                        if(positions[i].second - positions[i + 1].second == 1)
                            SDL_BlitSurface(corps[9],NULL,fond,&pos);
                        else if(positions[i].second - positions[i + 1].second == -1)
                            SDL_BlitSurface(corps[11],NULL,fond,&pos);
                    }
                }
            }
        }
        pos.x = (30*(food.first - 1)) + 30;
        pos.y = (30*(food.second - 1)) + 30;
        SDL_BlitSurface(sandvich,NULL,fond,&pos);
        SDL_Surface *sub = rotozoomSurfaceXY(fond,0,double(m_W) / 1200, (double(m_H) + 3) / 840,0);
        SDL_BlitSurface(sub,NULL,screen,&POS);
        SDL_FreeSurface(fond);
        SDL_FreeSurface(sub);
    }
    bool vivant() const
    {
        if((positions[0].first < 0)||(positions[0].first > 28))
            return false;
        else if((positions[0].second < 0)||(positions[0].second > 25))
            return false;
        else
        {
            for(unsigned int i = 1; i < positions.size(); ++i)
            {
                if(positions[i] == positions[0])
                    return false;
            }
        }
        return true;
    }
    int taille() const
    {
        return positions.size();
    }
private :
    void TPFood()
    {
        bool h = true;
        while(h)
        {
            food.first = (rand() % 29);
            food.second = (rand() % 26);
            bool j = false;
            for(int i = 0; i < positions.size(); ++i)
            {
                if(food == positions[i])
                    j = true;
            }
            h = j;
        }
    }
    std::vector<std::pair<int,int>> positions;
    std::pair<int,int> food;
    int const m_H;
    int const m_W;
    SDL_Surface *sandvich;
    SDL_Surface* corps[14];
};

int VOLUME::VOLUME_SONORE = 128;

int serpent::speed = 1;

bool affSpeed(SDL_Surface *screen, int const W, int const H)
{
    Mix_Music *mus = NULL;
    SDL_Surface *text = NULL;
    TTF_Font *police = NULL;
    police = TTF_OpenFont("polices/Ubuntu-MediumItalic.ttf",((46 * H) / 1200));
    SDL_Color couleur = {153,176,0};
    text = rotozoomSurfaceXY(TTF_RenderText_Blended(police,"Sn4ke Sp33d", couleur),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Rect pos;
    pos.x = (W / 2) - (text->w / 2);
    pos.y = ((100 * H) / 840);
    SDL_BlitSurface(text,NULL,screen,&pos);
    TTF_CloseFont(police);
    police = TTF_OpenFont("polices/LiberationSerif-Regular.ttf",((39 * H) / 1200));
    SDL_Color couleur2 = {76,88,0};
    pos.x = ((20 * W) / 1200);
    pos.y = ((200 * H) / 840);
    SDL_BlitSurface(rotozoomSurfaceXY(TTF_RenderText_Blended(police,"Mets la barre de difficulte a ta convenance. Les points sont multiplies par la",couleur2),0,double(W) / 1200, double(H) / 840 , 0),NULL,screen,&pos);
    pos.y = ((200 * H) / 840) + ((40 * H) / 840);
    SDL_BlitSurface(rotozoomSurfaceXY(TTF_RenderText_Blended(police,"difficulte, donc plus c'est dur, plus tu as de points.",couleur2),0,double(W) / 1200, double(H) / 840,0),NULL,screen,&pos);
    SDL_Surface *Quit = NULL;
    Quit = rotozoomSurfaceXY(IMG_Load("images/boutonQuit.png"),0,double(W) / 1200, double(H) / 840,0);
    pos.y = ((710 * H) / 840);
    SDL_Surface *QuitOn = NULL;
    QuitOn = rotozoomSurfaceXY(IMG_Load("images/boutonQuitOn.png"),0,double(W) / 1200, double(H) / 840,0);
    pos.x = (W / 2) - (Quit->w / 2);
    SDL_BlitSurface(Quit,NULL,screen,&pos);
    SDL_Surface *corde = NULL;
    corde = SDL_CreateRGBSurface(SDL_HWSURFACE,((685 * W) / 1200),((144 * H) / 840),32,0,0,0,0);
    SDL_FillRect(corde,NULL,SDL_MapRGB(corde->format,0,0,0));
    pos.x = (W / 2) - (corde->w / 2);
    pos.y = ((H / 2) - (corde->h / 2)) + ((50 * H) / 840);
    SDL_BlitSurface(corde,NULL,screen,&pos);
    SDL_Surface *rouge = NULL;
    rouge = SDL_CreateRGBSurface(SDL_HWSURFACE,((80 * W) / 1200),((134 * H) / 840),32,0,0,0,0);
    SDL_FillRect(rouge,NULL,SDL_MapRGB(rouge->format,255,0,0));
    SDL_Surface *gris;
    gris = SDL_CreateRGBSurface(SDL_HWSURFACE,((80 * W) / 1200),((134 * H) / 840),32,0,0,0,0);
    SDL_FillRect(gris,NULL,SDL_MapRGB(gris->format,220,220,220));
    pos.y = ((H / 2) - (corde->h / 2)) + ((55 * H) / 840);
    for(int i = 1; i <= 8; ++ i)
    {
        if(i <= serpent::speed)
        {
           pos.x = ((W / 2) - (corde->w / 2) + ((5 * W) / 1200))  + ((rouge->w + ((5 * W) / 1200)) * (i -1));
           SDL_BlitSurface(rouge,NULL,screen,&pos);
        }
        else
        {
            pos.x = ((W / 2) - (corde->w / 2) + ((5 * W) / 1200))  + ((rouge->w + ((5 * W) / 1200)) * (i -1));
            SDL_BlitSurface(gris,NULL,screen,&pos);
        }
    }
    bool continuer = true;
    bool h = true;
    bool On = false;
    SDL_Event event;
    SDL_Flip(screen);
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT :
                continuer = false;
                h = false;
                break;
            case SDL_MOUSEMOTION :
                if((((event.motion.x >= ((W / 2) - (Quit->w / 2)))&&(event.motion.x <= ((W / 2) - (Quit->w / 2)) + ((235 * W) / 1200)))&&((event.motion.y >= ((710 * H) / 840))&&(event.motion.y <= ((710 * H) / 840) + ((87 * H) / 840))))&&(!On))
                {
                    pos.x = (W / 2) - (Quit->w / 2);
                    pos.y = ((710 * H) / 840);
                    SDL_BlitSurface(QuitOn,NULL,screen,&pos);
                    On = true;
                }
                else if((((event.motion.x < ((W / 2) - (Quit->w / 2)))||(event.motion.x > ((W / 2) - (Quit->w / 2)) + ((235 * W) / 1200)))||((event.motion.y < ((710 * H) / 840))||(event.motion.y > ((710 * H) / 840) + ((87 * H) / 840))))&&(On))
                {
                    pos.x = (W / 2) - (Quit->w / 2);
                    pos.y = ((710 * H) / 840);
                    SDL_BlitSurface(Quit,NULL,screen,&pos);
                    On = false;
                }
                break;
            case SDL_KEYDOWN :
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE :
                        h = false;
                        continuer = false;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN :
                if(On)
                    continuer = false;
                else
                {
                    if((event.motion.y >= ((H / 2) - (corde->h / 2)) + ((50 * H) / 840))&&(event.motion.y <= (((H / 2) - (corde->h / 2)) + ((50 * H) / 840)) + corde->h))
                    {
                        if((event.motion.x >= (W / 2) - (corde->w / 2))&&(event.motion.x <=((W / 2) - (corde->w / 2)) + corde->w))
                        {
                            int d = event.motion.x - ((W / 2) - (corde->w / 2));
                            if(d <= ((5 * W) / 1200))
                            {
                                serpent::speed = 1;
                            }
                            else
                            {
                                d -= ((5 * W) / 1200);
                                d -= d % ((85 * W) / 1200);
                                d /= ((85 * W) / 1200);
                                d += 1;
                                serpent::speed = d;
                            }
                            for(int i = 1; i <= 8; ++ i)
                            {
                                if(i <= serpent::speed)
                                {
                                    pos.y = ((H / 2) - (corde->h / 2)) + ((55 * H) / 840);
                                    pos.x = ((W / 2) - (corde->w / 2) + ((5 * W) / 1200))  + ((rouge->w + ((5 * W) / 1200)) * (i -1));
                                    SDL_BlitSurface(rouge,NULL,screen,&pos);
                                }
                                else
                                {
                                    pos.y = ((H / 2) - (corde->h / 2)) + ((55 * H) / 840);
                                    pos.x = ((W / 2) - (corde->w / 2) + ((5 * W) / 1200))  + ((rouge->w + ((5 * W) / 1200)) * (i -1));
                                    SDL_BlitSurface(gris,NULL,screen,&pos);
                                }
                            }
                        }
                    }
                }
                break;
        }
        SDL_Flip(screen);
    }
    TTF_CloseFont(police);
    SDL_FreeSurface(text);
    SDL_FreeSurface(Quit);
    SDL_FreeSurface(corde);
    SDL_FreeSurface(rouge);
    SDL_FreeSurface(gris);
    std::string arg = "data/diff.txt";
    std::ofstream flux(arg.c_str());
    flux << serpent::speed;
    return h;
}

class u
{
    public :
        bool operator()(std::pair<int,std::string> const a, std::pair<int,std::string> const b)
        {
            return a.first > b.first;
        }
};

void ecriture(std::string &entree, int s)
{
    std::string const fichierJ = "data/joueurs.txt";
    std::string const fichierS = "data/scores.txt";
    std::ifstream flux(fichierS.c_str());
    std::ifstream flux2(fichierJ.c_str());
    std::string bol;
    std::vector<int> a;
    while(std::getline(flux,bol))
    {
        a.push_back(std::stoi(bol));
    }
    std::vector<int> a2;
    if(a.size() > 9)
    {
        for(unsigned int i = 0; i < a.size() - 1; ++i)
        {
            a2.push_back(a[i]);
        }
    }
    else
    {
        for(unsigned int i = 0; i < a.size(); ++i)
        {
            a2.push_back(a[i]);
        }
    }
    a2.push_back(s);
    std::vector<std::string> b;
    while(std::getline(flux2,bol))
    {
        b.push_back(bol);
    }
    std::vector<std::string> b2;
    if(b.size() > 9)
    {
        for(unsigned int i = 0; i < b.size() - 1; ++i)
        {
            b2.push_back(b[i]);
        }
    }
    else
    {
        for(unsigned int i = 0; i < b.size(); ++i)
        {
            b2.push_back(b[i]);
        }
    }
    b2.push_back(entree);
    flux.close();
    flux2.close();
    std::vector<std::pair<int,std::string>> ab;
    for(unsigned int i = 0; i < a2.size(); ++i)
    {
        ab.push_back(std::pair<int,std::string>(a2[i],b2[i]));
    }
    std::sort(ab.begin(),ab.end(),u());
    std::ofstream flux3(fichierS.c_str());
    std::ofstream flux4(fichierJ.c_str());
    for(unsigned int i = 0; i < ab.size(); ++i)
    {
        if(i != 0)
        {
            flux3 << std::endl;
            flux4 << std::endl;
        }
        flux3 << ab[i].first;
        flux4 << ab[i].second;
    }
    flux3.close();
    flux4.close();
}

bool play(SDL_Surface *screen, int const W, int const H)
{
    SDL_ShowCursor(0);
    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
    int TA = SDL_GetTicks();
    SDL_Flip(screen);
    Mix_Volume(1, VOLUME::VOLUME_SONORE);
    Mix_Chunk *son;
    son = Mix_LoadWAV("musiques/mange.wav");
    Mix_VolumeChunk(son, VOLUME::VOLUME_SONORE);
    Mix_Music *mus;
    mus = Mix_LoadMUS("musiques/piste2.mp3");
    bool h = true;
    direc d = DROITE;
    serpent snake(10,10,H,W);
    bool continuer = true;
    SDL_Event event;
    SDL_Rect pos;
    SDL_Surface *PP = NULL;
    PP = SDL_CreateRGBSurface(SDL_HWSURFACE,W,H,32,0,0,0,0);
    SDL_FillRect(PP,NULL,SDL_MapRGB(PP->format,0,0,0));
    SDL_Surface *fond = rotozoomSurfaceXY(IMG_Load("images/fond.png"),0,double(W) / 1200, double(H) / 840,0);
    bool u = true;
    SDL_Color c = {76,88,0};
    int k = snake.taille();
    if(SDL_GetTicks() - TA < 500)
    {
        SDL_Delay(500 - (SDL_GetTicks() - TA));
    }
    Mix_PlayMusic(mus,-1);
    while(continuer)
    {
        Mix_VolumeMusic(VOLUME::VOLUME_SONORE);
        if(snake.taille() > k)
        {
            k = snake.taille();
            Mix_PlayChannel(1,son,0);
        }
        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
        SDL_FillRect(PP,NULL,SDL_MapRGB(PP->format,0,0,0));
        pos.x = 0;
        pos.y = 0;
        SDL_BlitSurface(fond,NULL,PP,&pos);
        pos.x = ((31 * W) / 1200);
        pos.y = ((29 * H) / 840);
        snake.aff(PP,pos);
        TTF_Font *pol = NULL;
        pol = TTF_OpenFont("polices/ComicSans.ttf",50);
        SDL_Surface *score = NULL;
        std::string p = std::to_string(serpent::speed * snake.taille());
        score = rotozoomSurfaceXY(TTF_RenderText_Blended(pol,p.c_str(),c),0,double(W) / 1200, double(H) / 840,0);
        pos.x = ((925 * W) / 1200);
        pos.y = ((190 * H) / 840);
        SDL_BlitSurface(score,NULL,PP,&pos);
        SDL_FreeSurface(score);
        SDL_Surface *speed = NULL;
        p = std::to_string(serpent::speed);
        speed = rotozoomSurfaceXY(TTF_RenderText_Blended(pol,p.c_str(),c),0,double(W) / 1200, double(H) / 840,0);
        pos.y = ((375 * H) / 840);
        SDL_BlitSurface(speed,NULL,PP,&pos);
        SDL_FreeSurface(speed);
        TTF_CloseFont(pol);
        pos.x = 0;
        pos.y = 0;
        SDL_BlitSurface(PP,NULL,screen,&pos);
        SDL_Flip(screen);
        event.type = NULL;
        int h = 0;
        while((h < 50 - (serpent::speed*5))&&(event.type == NULL))
        {
            SDL_PollEvent(&event);
            h++;
            SDL_Delay(1);
        }
        if(event.type != NULL)
        {
            switch(event.type)
            {
                case SDL_KEYDOWN :
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE :
                            h = false;
                            break;
                        case SDLK_UP :
                            if(d != BAS)
                                d = HAUT;
                            break;
                        case SDLK_DOWN :
                            if(d != HAUT)
                                d = BAS;
                            break;
                        case SDLK_RIGHT :
                            if(d != GAUCHE)
                                d = DROITE;
                            break;
                        case SDLK_LEFT :
                            if(d != DROITE)
                                d = GAUCHE;
                            break;
                        case SDLK_RETURN :
                            SDL_ShowCursor(1);
                            SDL_Surface *cache = NULL;
                            cache = SDL_CreateRGBSurface(SDL_HWSURFACE,W,H,32,0,0,0,0);
                            SDL_FillRect(cache,NULL,SDL_MapRGB(cache->format,255,255,255));
                            SDL_SetAlpha(cache,SDL_SRCALPHA,192);
                            SDL_BlitSurface(cache,NULL,screen,&pos);
                            SDL_Surface *sampleText = NULL;
                            TTF_Font *police = NULL;
                            police = TTF_OpenFont("polices/Ubuntu-MediumItalic.ttf",80);
                            SDL_Color couleur =  {153,176,0};
                            sampleText = rotozoomSurfaceXY(TTF_RenderText_Blended(police,"Pause",couleur),0,double(W) / 1200, double(H) / 840,0);
                            pos.x = (W / 2) - (sampleText->w / 2);
                            pos.y = ((200 * H) / 840);
                            SDL_BlitSurface(sampleText,NULL,screen,&pos);
                            SDL_Surface *Quit = NULL;
                            Quit = rotozoomSurfaceXY(IMG_Load("images/boutonQuit.png"),0,double(W) / 1200, double(H) / 840,0);
                            SDL_Surface *QuitOn = NULL;
                            QuitOn = rotozoomSurfaceXY(IMG_Load("images/boutonQuitOn.png"),0,double(W) / 1200, double(H) / 840,0);
                            SDL_Surface *Play = NULL;
                            Play = rotozoomSurfaceXY(IMG_Load("images/boutonPlay.png"),0,double(W) / 1200, double(H) / 840,0);
                            SDL_Surface *PlayOn = NULL;
                            PlayOn = rotozoomSurfaceXY(IMG_Load("images/boutonPlayOn.png"),0,double(W) / 1200, double(H) / 840,0);
                            pos.y = ((600 * H) / 840);
                            pos.x = (W / 4) - (Play->w / 2);
                            SDL_BlitSurface(Play,NULL,screen,&pos);
                            pos.x = ((W * 3) / 4) - (Quit->w / 2);
                            SDL_BlitSurface(Quit,NULL,screen,&pos);
                            SDL_Flip(screen);
                            bool continuer2 = true;
                            SDL_Event event2;
                            bool OnQuit = false;
                            bool OnPlay = false;
                            while(continuer2)
                            {
                                SDL_WaitEvent(&event2);
                                switch(event2.type)
                                {
                                    case SDL_KEYDOWN :
                                        switch(event2.key.keysym.sym)
                                        {
                                            case SDLK_ESCAPE :
                                                continuer2 = false;
                                                continuer = false;
                                                h = false;
                                                break;
                                        }
                                        break;
                                    case SDL_MOUSEMOTION :
                                        if((((event2.motion.x <= ((W / 4) - (Play->w / 2)) + ((235 * W) / 1200))&&(event2.motion.x >= ((W / 4) - (Play->w / 2)))&&(event2.motion.y <= ((600 * H) / 840) + ((87 * H) / 840))&&(event2.motion.y >= ((600 * H) / 840))))&&(!OnPlay))
                                        {
                                            pos.x = (W / 4) - (Play->w / 2);
                                            pos.y = ((600 * H) / 840);
                                            SDL_BlitSurface(PlayOn,NULL,screen,&pos);
                                            OnPlay = true;
                                        }
                                        else if(((event2.motion.x >= ((W / 4) - (Play->w / 2)) + ((235 * W) / 1200))||(event2.motion.x <= ((W / 4) - (Play->w / 2)))||((event2.motion.y >= ((600 * H) / 840) + ((87 * H) / 840))||(event2.motion.y <= ((600 * H) / 840))))&&(OnPlay))
                                        {
                                            pos.x = (W / 4) - (Play->w / 2);
                                            pos.y = ((600 * H) / 840);
                                            SDL_BlitSurface(Play,NULL,screen,&pos);
                                            OnPlay = false;
                                        }
                                        if((((event2.motion.x <= (((W * 3) / 4) - (Quit->w / 2)) + ((235 * W) / 1200))&&(event2.motion.x >= ((W * 3) / 4) - (Quit->w / 2))&&(event2.motion.y <= ((600 * H) / 840) + ((87 * H) / 840))&&(event2.motion.y >= ((600 * H) / 840))))&&(!OnQuit))
                                        {
                                            pos.x = ((W * 3) / 4) - (Quit->w / 2);
                                            pos.y = ((600 * H) / 840);
                                            SDL_BlitSurface(QuitOn,NULL,screen,&pos);
                                            OnQuit = true;
                                        }
                                        else if(((event2.motion.x >= (((W * 3) / 4) - (Quit->w / 2)) + ((235 * W) / 1200))||(event2.motion.x <= ((W * 3) / 4) - (Quit->w / 2))||((event2.motion.y >= ((600 * H) / 840) + ((87 * H) / 840))||(event2.motion.y <= ((600 * H) / 840))))&&(OnQuit))
                                        {
                                            pos.x = ((W * 3) / 4) - (Quit->w / 2);
                                            pos.y = ((600 * H) / 840);
                                            SDL_BlitSurface(Quit,NULL,screen,&pos);
                                            OnQuit = false;
                                        }
                                        break;
                                        case SDL_MOUSEBUTTONDOWN :
                                            if(OnPlay)
                                                continuer2 = false;
                                            else if(OnQuit)
                                            {
                                                continuer2 = false;
                                                u = false;
                                            }

                                            break;
                                }
                                SDL_Flip(screen);
                            }
                            TTF_CloseFont(police);
                            SDL_FreeSurface(cache);
                            SDL_FreeSurface(sampleText);
                            SDL_FreeSurface(Quit);
                            SDL_FreeSurface(QuitOn);
                            SDL_FreeSurface(Play);
                            SDL_FreeSurface(PlayOn);
                            SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
                            pos.x = 0;
                            pos.y = 0;
                            SDL_BlitSurface(PP,NULL,screen,&pos);
                            SDL_ShowCursor(0);
                            SDL_Flip(screen);
                            break;
                    }
                    break;
            }
            SDL_Delay((100 - (serpent::speed*10)) - h);
        }
        snake.bouge(d);
        if(!snake.vivant())
        {
            SDL_Surface *cache = NULL;
            cache = SDL_CreateRGBSurface(SDL_HWSURFACE,W,H,32,0,0,0,0);
            SDL_FillRect(cache,NULL,SDL_MapRGB(cache->format,255,255,255));
            SDL_SetAlpha(cache,SDL_SRCALPHA,192);
            pos.x = 0;
            pos.y = 0;
            SDL_BlitSurface(cache,NULL,screen,&pos);
            std::string arg = "data/scores.txt";
            std::ifstream flux(arg.c_str());
            std::string bol;
            int y, n(0);
            while(std::getline(flux,bol))
            {
                y = std::stoi(bol);
                ++n;
            }
            if((serpent::speed * snake.taille() > y)||(n < 10))
            {
                SDL_Color couleur = {153,176,0};
                std::string entree = " ";
                TTF_Font *pol3 = NULL;
                pol3 = TTF_OpenFont("polices/Ubuntu-MediumItalic.ttf",50);
                TTF_Font *pol4 = NULL;
                pol4 = TTF_OpenFont("polices/LiberationSerif-Regular.ttf",35);
                SDL_Surface *sampletext = NULL;
                sampletext = rotozoomSurfaceXY(TTF_RenderText_Blended(pol3,"Wow. Nouveau Record!",couleur),0,double(W) / 1200,double(H) / 840,0);
                SDL_Surface *sampletext2 = NULL;
                sampletext2 = rotozoomSurfaceXY(TTF_RenderText_Blended(pol4,"Enregistre ton pseudo et appuie sur enter pour terminer.",c),0,double(W) / 1200,double(H) / 840,0);
                SDL_Flip(screen);
                bool continuer4 = true;
                SDL_Event event4;
                bool maj = false;
                SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
                pos.x = 0;
                pos.y = 0;
                SDL_BlitSurface(PP,NULL,screen,&pos);
                SDL_BlitSurface(cache,NULL,screen,&pos);
                pos.x = (W / 2) - (sampletext->w / 2);
                pos.y = ((100 * H) / 840);
                SDL_BlitSurface(sampletext,NULL,screen,&pos);
                pos.x = (W / 2) - (sampletext2->w / 2);
                pos.y = ((110 * H) / 840) + sampletext->h;
                SDL_BlitSurface(sampletext2,NULL,screen,&pos);
                SDL_Surface *text;
                SDL_Flip(screen);
                while(continuer4)
                {

                    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
                    pos.x = 0;
                    pos.y = 0;
                    SDL_BlitSurface(PP,NULL,screen,&pos);
                    SDL_BlitSurface(cache,NULL,screen,&pos);
                    pos.x = (W / 2) - (sampletext->w / 2);
                    pos.y = ((100 * H) / 840);
                    SDL_BlitSurface(sampletext,NULL,screen,&pos);
                    pos.x = (W / 2) - (sampletext2->w / 2);
                    pos.y = ((110 * H) / 840) + sampletext->h;
                    SDL_BlitSurface(sampletext2,NULL,screen,&pos);
                    SDL_Surface *text;
                    text = rotozoomSurfaceXY(TTF_RenderText_Blended(pol3,"",couleur),0,double(W) / 1200, double(H) / 840,0);
                    SDL_WaitEvent(&event4);
                    switch(event4.type)
                    {
                        case SDL_KEYDOWN :
                            if(event4.key.keysym.sym == SDLK_ESCAPE)
                            {
                                continuer4 = false;
                                h = false;
                            }
                            else if((event4.key.keysym.sym == SDLK_RSHIFT)||(event4.key.keysym.sym == SDLK_LSHIFT))
                                maj = true;
                            else if(entree.size() <= 10)
                            {
                                int t = event4.key.keysym.sym;
                                if((t >= 256)&&(t <= 265))
                                {
                                    t -= 208;
                                    entree += char(t);
                                }
                                else if(t == SDLK_RETURN)
                                {
                                    ecriture(entree,int(snake.taille() * serpent::speed));
                                    continuer4 = false;
                                }
                                else if((maj)&&(t == SDLK_MINUS))
                                {
                                    entree += char(95);
                                }
                                else
                                {
                                    if((t >= 97)&&(t <= 122))
                                    {
                                        if(maj)
                                            t -= 32;
                                        entree += char(t);
                                    }
                                    else if((t == SDLK_BACKSPACE)&&(entree.size() > 1))
                                    {
                                        entree.erase(entree.size() - 1,1);
                                    }

                                }

                            }
                            else if(entree.size() <= 11)
                            {
                                int t = event4.key.keysym.sym;
                                if(t == SDLK_BACKSPACE)
                                    entree.erase(entree.size() - 1,1);
                                else if(t == SDLK_RETURN)
                                {
                                    ecriture(entree,int(snake.taille() * serpent::speed));
                                    continuer4 = false;
                                }
                            }
                            break;
                        case SDL_KEYUP :
                            if((event4.key.keysym.sym == SDLK_RSHIFT)||(event4.key.keysym.sym == SDLK_LSHIFT))
                                maj = false;
                            break;
                    }
                    SDL_FreeSurface(text);
                    text = rotozoomSurfaceXY(TTF_RenderText_Blended(pol3,entree.c_str(),couleur),0,double(W) / 1200, double(H) / 840,0);
                    pos.y = ((400 * H) / 840);
                    pos.x = (W / 2) - (text->w / 2);
                    SDL_BlitSurface(text,NULL,screen,&pos);
                    SDL_Flip(screen);
                }
                SDL_FreeSurface(sampletext);
                SDL_FreeSurface(sampletext2);
                TTF_CloseFont(pol3);
                TTF_CloseFont(pol4);
            }
            else
            {
                SDL_ShowCursor(1);
                TTF_Font *pol2 = NULL;
                pol2 = TTF_OpenFont("polices/Ubuntu-MediumItalic.ttf",50);
                SDL_Surface *text = NULL;
                SDL_Color couleur =  {153,176,0};
                text = TTF_RenderText_Blended(pol2,"Dommage. Vous avez perdu.",couleur);
                pos.y = ((175 * H) / 840);
                pos.x = (W / 2) - (text->w / 2);
                SDL_BlitSurface(text,NULL,screen,&pos);
                SDL_Surface *bouton = NULL;
                bouton = rotozoomSurfaceXY(IMG_Load("images/boutonQuit.png"),0,double(W) / 1200,double(H) / 840,0);
                SDL_Surface *boutonOn = NULL;
                boutonOn = rotozoomSurfaceXY(IMG_Load("images/boutonQuitOn.png"),0,double(W) / 1200,double(H) / 840,0);
                pos.y = ((650 * H) / 840);
                pos.x = (W / 2) - (bouton->w / 2);
                SDL_BlitSurface(bouton,NULL,screen,&pos);
                bool continuer3 = true;
                SDL_Event event3;
                SDL_Flip(screen);
                bool OnQuit = false;
                while(continuer3)
                {
                    SDL_WaitEvent(&event3);
                    switch(event3.type)
                    {
                        case SDL_KEYDOWN :
                            switch(event3.key.keysym.sym)
                            {
                                case SDLK_ESCAPE :
                                    continuer3 = false;
                                    h = false;
                                    break;
                            }
                            break;
                        case SDL_MOUSEMOTION :
                            if((((event3.motion.x <= ((W / 2) - (bouton->w / 2)) + ((235 * W) / 1200))&&(event3.motion.x >= ((W / 2) - (bouton->w / 2)))&&(event3.motion.y <= ((650 * H) / 840) + ((87 * H) / 840))&&(event3.motion.y >= ((650 * H) / 840))))&&(!OnQuit))
                            {
                                SDL_BlitSurface(boutonOn,NULL,screen,&pos);
                                OnQuit = true;
                            }
                            else if((((event3.motion.x > ((W / 2) - (bouton->w / 2)) + ((235 * W) / 1200))||(event3.motion.x < ((W / 2) - (bouton->w / 2)))||(event3.motion.y > ((650 * H) / 840) + ((87 * H) / 840))||(event3.motion.y < ((650 * H) / 840))))&&(OnQuit))
                            {
                                SDL_BlitSurface(bouton,NULL,screen,&pos);
                                OnQuit = false;
                            }
                            break;
                        case SDL_MOUSEBUTTONDOWN :
                            if(OnQuit)
                                continuer3 = false;
                            break;
                    }
                    SDL_Flip(screen);
                }
                SDL_FreeSurface(bouton);
                SDL_FreeSurface(boutonOn);
                SDL_FreeSurface(text);
                TTF_CloseFont(pol2);
                SDL_ShowCursor(0);
            }
            SDL_FreeSurface(cache);
        }
        continuer =  u;
        if(continuer)
        {
            continuer = h;
            if(continuer)
                continuer = snake.vivant();
            else
            {
                std::cout << h << std::endl;
            }
        }
    }
    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
    TA = SDL_GetTicks();
    SDL_Flip(screen);
    SDL_FreeSurface(fond);
    SDL_FreeSurface(PP);
    Mix_FreeMusic(mus);
    Mix_FreeChunk(son);
    if(SDL_GetTicks() - TA < 500)
    {
        SDL_Delay(500 - (SDL_GetTicks() - TA));
    }
    SDL_ShowCursor(1);
    return h;
}

enum next{PLAY,SCORES,SPEED,TUTORIEL,NONE};

int main(int argc, char *argv[])
{
    Mix_AllocateChannels(1);
    Mix_Volume(1,VOLUME::VOLUME_SONORE * 2);
    Mix_Chunk *son = NULL;
    son = Mix_LoadWAV("musiques/mange.wav");
    Mix_PlayChannel(1,son,30);
    Mix_FreeChunk(son);
    std::srand(std::time(0));
    std::string const arg("data/diff.txt");
    std::ifstream flux(arg.c_str());
    std::string bol;
    while(std::getline(flux,bol))
    {
        serpent::speed = std::stoi(bol);
    }
    flux.close();
    std::string const a = "data/volume.txt";
    std::ifstream flux2(a.c_str());
    while(std::getline(flux2,bol))
    {
        VOLUME::VOLUME_SONORE = std::stoi(bol);
    }
    flux2.close();
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024);
    const SDL_VideoInfo *resolution = NULL;
    resolution = SDL_GetVideoInfo();
    int const W = resolution->current_w;
    int const H = resolution->current_h;
    SDL_Surface *screen = NULL;
    screen = SDL_SetVideoMode(W,H,32,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    Mix_Music *musique = NULL;
    musique = Mix_LoadMUS("musiques/piste1.mp3");
    Mix_PlayMusic(musique,-1);
    SDL_Surface *fond = NULL;
    fond = rotozoomSurfaceXY(IMG_Load("images/acceuil.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonPlay = NULL;
    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    SDL_BlitSurface(fond,NULL,screen,&pos);
    boutonPlay = rotozoomSurfaceXY(IMG_Load("images/boutonPlay.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonPlayOn = NULL;
    boutonPlayOn =  rotozoomSurfaceXY(IMG_Load("images/boutonPlayOn.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonScores = NULL;
    boutonScores = rotozoomSurfaceXY(IMG_Load("images/boutonScores.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonScoresOn = NULL;
    boutonScoresOn =  rotozoomSurfaceXY(IMG_Load("images/boutonScoresOn.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonSpeed = NULL;
    boutonSpeed =  rotozoomSurfaceXY(IMG_Load("images/boutonSpeed.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonSpeedOn = NULL;
    boutonSpeedOn =  rotozoomSurfaceXY(IMG_Load("images/boutonSpeedOn.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonTutoriel = NULL;
    boutonTutoriel=  rotozoomSurfaceXY(IMG_Load("images/boutonTutoriel.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonTutorielOn = NULL;
    boutonTutorielOn =  rotozoomSurfaceXY(IMG_Load("images/boutonTutorielOn.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonQuit = NULL;
    boutonQuit =  rotozoomSurfaceXY(IMG_Load("images/boutonQuit.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonQuitOn = NULL;
    boutonQuitOn =  rotozoomSurfaceXY(IMG_Load("images/boutonQuitOn.png"),0,double(W) / 1200, double(H) / 840 , 0);
    SDL_Surface *boutonSon = NULL;
    boutonSon = rotozoomSurfaceXY(IMG_Load("images/son.png"),0,(double(W) / 1200) / 4, (double(H) / 840) / 4 , 0);
    SDL_Surface *volume = NULL;
    volume = SDL_CreateRGBSurface(SDL_HWSURFACE,((VOLUME::VOLUME_SONORE * W) / 1200),((25 * H) / 840),32,0,0,0,0);
    SDL_FillRect(volume,NULL,SDL_MapRGB(volume->format,0,0,0));
    pos.x = (225 * W) / 1200;
    pos.y = (285 * H) / 840;
    SDL_BlitSurface(boutonPlay,NULL,screen,&pos);
    pos.y = (385 * H) / 840;
    SDL_BlitSurface(boutonScores,NULL,screen,&pos);
    pos.y = (485 * H) / 840;
    SDL_BlitSurface(boutonSpeed,NULL,screen,&pos);
    pos.y = (585 * H) / 840;
    SDL_BlitSurface(boutonTutoriel,NULL,screen,&pos);
    pos.y = (685 * H) / 840;
    SDL_BlitSurface(boutonQuit,NULL,screen,&pos);
    pos.x = (900 * W) / 1200;
    pos.y = (715 * H) / 840;
    SDL_BlitSurface(boutonSon,NULL,screen,&pos);
    pos.y = (732 * H) / 840;
    pos.x = (960 * W) / 1200;
    SDL_BlitSurface(volume,NULL,screen,&pos);
    SDL_Flip(screen);
    Mix_VolumeMusic(VOLUME::VOLUME_SONORE);
    next func = NONE;
    bool OnBoutonPlay = false;
    bool OnBoutonScores = false;
    bool OnBoutonSpeed = false;
    bool OnBoutonTutoriel = false;
    bool OnBoutonQuit = false;
    bool continuer = true;
    while(continuer)
    {
        if(func != NONE)
        {
            SDL_Surface *cache = NULL;
            cache = SDL_CreateRGBSurface(SDL_HWSURFACE,W,H,32,0,0,0,0);
            SDL_FillRect(cache,NULL,SDL_MapRGB(cache->format,255,255,255));
            SDL_SetAlpha(cache,SDL_SRCALPHA,192);
            pos.x = 0;
            pos.y = 0;
            SDL_BlitSurface(cache,NULL,screen,&pos);
            SDL_Flip(screen);
            if(func == PLAY)
            {
                Mix_HaltMusic();
                Mix_FreeMusic(musique);
                bool p = play(screen,W,H);
                musique = Mix_LoadMUS("musiques/piste1.mp3");
                Mix_PlayMusic(musique,-1);
                continuer = p;
            }
            else if(func == SCORES)
            {
                continuer = affScores(screen,W,H);
            }
            else if(func == SPEED)
            {
                continuer = affSpeed(screen,W,H);
            }
            else if(func == TUTORIEL)
            {
                continuer = affTutoriel(screen,W,H);
            }
            SDL_SetAlpha(cache,SDL_SRCALPHA,255);
            SDL_FillRect(screen,&pos,SDL_MapRGB(screen->format,255,255,255));
            SDL_BlitSurface(fond,NULL,screen,&pos);
            pos.x = (225 * W) / 1200;
            pos.y = (285 * H) / 840;
            SDL_BlitSurface(boutonPlay,NULL,screen,&pos);
            pos.y = (385 * H) / 840;
            SDL_BlitSurface(boutonScores,NULL,screen,&pos);
            pos.y = (485 * H) / 840;
            SDL_BlitSurface(boutonSpeed,NULL,screen,&pos);
            pos.y = (585 * H) / 840;
            SDL_BlitSurface(boutonTutoriel,NULL,screen,&pos);
            pos.y = (685 * H) / 840;
            SDL_BlitSurface(boutonQuit,NULL,screen,&pos);
            pos.x = (900 * W) / 1200;
            pos.y = (715 * H) / 840;
            SDL_BlitSurface(boutonSon,NULL,screen,&pos);
            pos.y = (732 * H) / 840;
            pos.x = (960 * W) / 1200;
            SDL_BlitSurface(volume,NULL,screen,&pos);
            SDL_FreeSurface(cache);
        }
        func = NONE;
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT :
                continuer = false;
                break;
            case SDL_MOUSEMOTION :
                if(((event.motion.x <= ((225 * W) / 1200) + ((235 * W) / 1200))&&(event.motion.x >= ((225 * W) / 1200))&&(event.motion.y <= ((275 * H) / 840) + ((87 * H) / 840))&&(event.motion.y >= ((275 * H) / 840)))&&(!OnBoutonPlay))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (285 * H) / 840;
                    SDL_BlitSurface(boutonPlayOn,NULL,screen,&pos);
                    OnBoutonPlay = true;
                }
                else if(((event.motion.x >= ((225 * W) / 1200) + ((235 * W) / 1200))||(event.motion.x <= ((225 * W) / 1200)))||((event.motion.y >= ((275 * H) / 840) + ((87 * H) / 840))||(event.motion.y <= ((275 * H) / 840)))&&(OnBoutonPlay))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (285 * H) / 840;
                    SDL_BlitSurface(boutonPlay,NULL,screen,&pos);
                    OnBoutonPlay = false;
                }
                if(((event.motion.x <= ((225 * W) / 1200) + ((235 * W) / 1200))&&(event.motion.x >= ((225 * W) / 1200))&&(event.motion.y <= ((375 * H) / 840) + ((87 * H) / 840))&&(event.motion.y >= ((375 * H) / 840)))&&(!OnBoutonScores))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (385 * H) / 840;
                    SDL_BlitSurface(boutonScoresOn,NULL,screen,&pos);
                    OnBoutonScores = true;
                }
                else if(((event.motion.x >= ((225 * W) / 1200) + ((235 * W) / 1200))||(event.motion.x <= ((225 * W) / 1200)))||((event.motion.y >= ((375 * H) / 840) + ((87 * H) / 840))||(event.motion.y <= ((375 * H) / 840)))&&(OnBoutonScores))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (385 * H) / 840;
                    SDL_BlitSurface(boutonScores,NULL,screen,&pos);
                    OnBoutonScores = false;
                }
                if(((event.motion.x <= ((225 * W) / 1200) + ((235 * W) / 1200))&&(event.motion.x >= ((225 * W) / 1200))&&(event.motion.y <= ((475 * H) / 840) + ((87 * H) / 840))&&(event.motion.y >= ((475 * H) / 840)))&&(!OnBoutonSpeed))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (485 * H) / 840;
                    SDL_BlitSurface(boutonSpeedOn,NULL,screen,&pos);
                    OnBoutonSpeed = true;
                }
                else if(((event.motion.x >= ((225 * W) / 1200) + ((235 * W) / 1200))||(event.motion.x <= ((225 * W) / 1200)))||((event.motion.y >= ((475 * H) / 840) + ((87 * H) / 840))||(event.motion.y <= ((475 * H) / 840)))&&(OnBoutonSpeed))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (485 * H) / 840;
                    SDL_BlitSurface(boutonSpeed,NULL,screen,&pos);
                    OnBoutonSpeed = false;
                }
                if(((event.motion.x <= ((225 * W) / 1200) + ((235 * W) / 1200))&&(event.motion.x >= ((225 * W) / 1200))&&(event.motion.y <= ((575 * H) / 840) + ((87 * H) / 840))&&(event.motion.y >= ((575 * H) / 840)))&&(!OnBoutonTutoriel))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (585 * H) / 840;
                    SDL_BlitSurface(boutonTutorielOn,NULL,screen,&pos);
                    OnBoutonTutoriel = true;
                }
                else if(((event.motion.x >= ((225 * W) / 1200) + ((235 * W) / 1200))||(event.motion.x <= ((225 * W) / 1200)))||((event.motion.y >= ((575 * H) / 840) + ((87 * H) / 840))||(event.motion.y <= ((575 * H) / 840)))&&(OnBoutonTutoriel))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (585 * H) / 840;
                    SDL_BlitSurface(boutonTutoriel,NULL,screen,&pos);
                    OnBoutonTutoriel = false;
                }
                if(((event.motion.x <= ((225 * W) / 1200) + ((235 * W) / 1200))&&(event.motion.x >= ((225 * W) / 1200))&&(event.motion.y <= ((675 * H) / 840) + ((87 * H) / 840))&&(event.motion.y >= ((675 * H) / 840)))&&(!OnBoutonQuit))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (685 * H) / 840;
                    SDL_BlitSurface(boutonQuitOn,NULL,screen,&pos);
                    OnBoutonQuit = true;
                }
                else if(((event.motion.x >= ((225 * W) / 1200) + ((235 * W) / 1200))||(event.motion.x <= ((225 * W) / 1200)))||((event.motion.y >= ((675 * H) / 840) + ((87 * H) / 840))||(event.motion.y <= ((675 * H) / 840)))&&(OnBoutonQuit))
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (685 * H) / 840;
                    SDL_BlitSurface(boutonQuit,NULL,screen,&pos);
                    OnBoutonQuit = false;
                }
                break;
            case SDL_MOUSEBUTTONDOWN :
                SDL_Event event2;
                SDL_WaitEvent(&event2);
                while(event2.type != SDL_MOUSEBUTTONUP)
                {
                    SDL_WaitEvent(&event2);
                }
                if(OnBoutonPlay)
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (285 * H) / 840;
                    SDL_BlitSurface(boutonPlay,NULL,screen,&pos);
                    OnBoutonPlay = false;
                    func = PLAY;
                }
                else if(OnBoutonScores)
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (385 * H) / 840;
                    SDL_BlitSurface(boutonScores,NULL,screen,&pos);
                    OnBoutonScores = false;
                    func = SCORES;
                }
                else if(OnBoutonSpeed)
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (485 * H) / 840;
                    SDL_BlitSurface(boutonSpeed,NULL,screen,&pos);
                    OnBoutonSpeed = false;
                    func = SPEED;
                }
                else if(OnBoutonTutoriel)
                {
                    pos.x = (225 * W) / 1200;
                    pos.y = (585 * H) / 840;
                    SDL_BlitSurface(boutonTutoriel,NULL,screen,&pos);
                    OnBoutonTutoriel = false;
                    func = TUTORIEL;
                }
                else if(OnBoutonQuit)
                    continuer = false;
                else
                {
                    if(((event.motion.x > ((960 * W) / 1200))&&(true)&&((event.motion.y >= ((732 * H) / 840))&&(event.motion.y <= (((732 * H) / 840) + volume->h)))))
                    {
                        SDL_FreeSurface(volume);
                        int p = event.motion.x - ((960 * W) / 1200);
                        if(p > ((128 * W) / 1200))
                        {
                            p = ((128 * W) / 1200);
                        }
                        volume = SDL_CreateRGBSurface(SDL_HWSURFACE,p,((25 * H) / 840),32,0,0,0,0);
                        SDL_FillRect(volume,NULL,SDL_MapRGB(volume->format,0,0,0));
                        SDL_FillRect(screen,&pos,SDL_MapRGB(screen->format,0,0,0));
                        pos.x = 0;
                        pos.y = 0;
                        SDL_BlitSurface(fond,NULL,screen,&pos);
                        pos.x = (225 * W) / 1200;
                        pos.y = (285 * H) / 840;
                        SDL_BlitSurface(boutonPlay,NULL,screen,&pos);
                        pos.y = (385 * H) / 840;
                        SDL_BlitSurface(boutonScores,NULL,screen,&pos);
                        pos.y = (485 * H) / 840;
                        SDL_BlitSurface(boutonSpeed,NULL,screen,&pos);
                        pos.y = (585 * H) / 840;
                        SDL_BlitSurface(boutonTutoriel,NULL,screen,&pos);
                        pos.y = (685 * H) / 840;
                        SDL_BlitSurface(boutonQuit,NULL,screen,&pos);
                        pos.x = (900 * W) / 1200;
                        pos.y = (715 * H) / 840;
                        SDL_BlitSurface(boutonSon,NULL,screen,&pos);
                        pos.y = (732 * H) / 840;
                        pos.x = (960 * W) / 1200;
                        SDL_BlitSurface(volume,NULL,screen,&pos);
                        VOLUME::VOLUME_SONORE = ((volume->w * 1200) / W);
                        Mix_VolumeMusic(VOLUME::VOLUME_SONORE);
                    }
                }
                break;
            case SDL_KEYDOWN :
                SDL_Event event3;
                bool continuer2 = true;
                while(continuer2)
                {
                    SDL_WaitEvent(&event3);
                    if(event3.type == SDL_KEYUP)
                        continuer2 = false;
                }
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE :
                        continuer = false;
                        break;
                    default :
                        break;
                }
                break;
        }
        SDL_Flip(screen);
    }
    std::ofstream f(a.c_str());
    f << VOLUME::VOLUME_SONORE;
    f.close();
    SDL_FreeSurface(fond);
    SDL_FreeSurface(boutonScores);
    SDL_FreeSurface(boutonQuit);
    SDL_FreeSurface(boutonSpeed);
    SDL_FreeSurface(boutonTutoriel);
    SDL_FreeSurface(boutonPlay);
    SDL_FreeSurface(boutonScoresOn);
    SDL_FreeSurface(boutonQuitOn);
    SDL_FreeSurface(boutonSpeedOn);
    SDL_FreeSurface(boutonTutorielOn);
    SDL_FreeSurface(boutonPlayOn);
    SDL_FreeSurface(boutonSon);
    SDL_FreeSurface(volume);
    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
