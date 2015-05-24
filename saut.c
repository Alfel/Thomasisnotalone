#include <sdl/sdl.h>

#ifdef WIN32
#pragma comment(lib,"sdl.lib")
#pragma comment(lib,"sdlmain.lib")
#endif

#define STAT_SOL 0
#define STAT_AIR 1

typedef struct
{
	int status;
	float x,y;
	float vx,vy;
} Sprite;

void InitSprite(Sprite* Sp)
{
	Sp->x = 150.0f;
	Sp->y = 250.0f;
	Sp->status = STAT_SOL;
	Sp->vx = Sp->vy = 0.0f;
}

void Render(Sprite* Sp,SDL_Surface* screen)
{
	SDL_Rect R;
	R.x = (Sint16)Sp->x;
	R.y = (Sint16)Sp->y;
	R.w = 20;
	R.h = 40;
	SDL_FillRect(screen,&R,SDL_MapRGB(screen->format,0,255,0));
}

void Saute(Sprite* Sp,float impulsion)
{
	Sp->vy = -impulsion;
	Sp->status = STAT_AIR;
}

void ControleSol(Sprite* Sp)
{
	if (Sp->y>250.0f)
	{
		Sp->y = 250.0f;
		if (Sp->vy>0)
			Sp->vy = 0.0f;
		Sp->status = STAT_SOL;
	}
}

void Gravite(Sprite* Sp,float factgravite,float factsautmaintenu,Uint8* keys)
{
	if (Sp->status == STAT_AIR && keys[SDLK_SPACE])
		factgravite/=factsautmaintenu;
	Sp->vy += factgravite;
}

void Evolue(Sprite* Sp,Uint8* keys)
{
	float lateralspeed = 0.5f;
	float airlateralspeedfacteur = 0.5f;
	float maxhspeed = 3.0f;
	float adherance = 1.5f;
	float impulsion = 6.0f;
	float factgravite = 0.5f;
	float factsautmaintenu = 3.0f;
// controle lateral
	if (Sp->status == STAT_AIR) // (*2)
		lateralspeed*= airlateralspeedfacteur;
	if (keys[SDLK_LEFT]) // (*1)
		Sp->vx-=lateralspeed;
	if (keys[SDLK_RIGHT])
		Sp->vx+=lateralspeed;
	if (Sp->status == STAT_SOL && !keys[SDLK_LEFT] && !keys[SDLK_RIGHT]) // (*3)
		Sp->vx/=adherance;
// limite vitesse
	if (Sp->vx>maxhspeed) // (*4)
		Sp->vx = maxhspeed;
	if (Sp->vx<-maxhspeed)
		Sp->vx = -maxhspeed;
// saut
	if (keys[SDLK_SPACE] && Sp->status == STAT_SOL)
		Saute(Sp,impulsion);
	Gravite(Sp,factgravite,factsautmaintenu,keys);
	ControleSol(Sp);
// application du vecteur à la position.
	Sp->x +=Sp->vx;
	Sp->y +=Sp->vy;
}

int main(int argc,char** argv)
{
	SDL_Surface* screen;
	int numkeys;
	Uint8 * keys;
	Sprite S;
	Uint32 timer,elapsed;
	SDL_Init(SDL_INIT_VIDEO);
	screen=SDL_SetVideoMode(400,320,32,SDL_SWSURFACE|SDL_DOUBLEBUF);
	InitSprite(&S);
	do 
	{
		timer = SDL_GetTicks();
		SDL_FillRect(screen,NULL,0);
		SDL_PumpEvents();
		keys = SDL_GetKeyState(&numkeys);
		Evolue(&S,keys);
		Render(&S,screen);
		SDL_Flip(screen);
		elapsed = SDL_GetTicks() - timer;
		if (elapsed<20)
			SDL_Delay(20-elapsed);
	} while (!keys[SDLK_ESCAPE]);
	SDL_Quit();
	return 0;
}


/*!F

	Voici un petit programme qui illustre quelques contrôles pour les sauts.
	Essayez le tout d'abord :

	Votre personnage est ici un rectangle vert. Déplacez le latéralement avec les flèches.
	Sautez avec la barre d'espace.
	- vous pouvez vous diriger en l'air
	- vous pouvez sauter plus ou moins haut en maintenant espace.

	Voyons un peu comment ça marche.

	Le main :
	---------

	Les intialisations, je vous laisse regarder.
	Dans le while, la boucle principale, d'abord le peins l'écran en noir avec SDL_FillRect

	La gestion des events, je la fais ici avec SDL_GetKeyState : j'obtiens le statut des touches.
	Je quitte si on appuie sur Echap.

	Afin de contrôler le framerate, je calcule le temps d'une frame. S'il est inférieur à 20 millisecondes, 
	j'attends de façon à ce que chaque frame fasse 20 millisecondes. Je bloque donc à 50 FPS l'affichage (parce que 50*20 = 1000)

	La structure :
	--------------

	J'ai créé une structure Sprite au dessus. Elle identifiera mon "personnage" carré et vert.

	La structure sprite contient :
	- position x,y du sprite
	- vecteur vitesse instantannée du sprite
	- un statut.

	Le statut, ici je n'en ai que deux : par terre et en l'air. Gardez en tête qu'on ne peut pas faire la même chose par terre qu'en l'air.
	C'est pour ça que le statut est important. Il faut que je sache à tout moment dans quel état je suis, pour savoir ce que je peux faire ou pas.

	Je stocke tout en float, et non en int. En effet, pour tous mes calculs, j'aurais plusieurs difficultés d'arrondis si je ne restais qu'en int.

	Render
	------

	Pour la fonction Render, c'est l'affichage. Rappelez vous, une fonction de rendue doit être courte, car elle ne fait finalement qu'illustrer
	ce qu'il y a dans la structure...

	Or, ma fonction de Rendu marche avec SDL, qui attend des coordonnées sous forme de nombres entiers. 
	Moi je stocke des nombres flottants, donc on va tout simplement caster pour le rendu, mais sans modifier les valeurs flottantes.
	La fonction de Rendu ne fait que lire la structure, elle ne la modifie pas.

	Evolue
	------
	
	L'idée va être, à tout moment, de calculer le vecteur de déplacement de mon sprite, en fonction de la gravité, du sol, et de ma volonté de "pousser"
	le personnage vers la droite ou la gauche pour le déplacer. 

	D'ailleurs, comme vous le voyez à la fin de la fonction Evolue, les deux dernières lignes appliquent le vecteur à la position, et c'est le seul endroit ou 
	je touche à la position.

	La fonction Evolue définit d'abord des constantes, dont je parlerai au fur et à mesure (et que vous pourrez modifier, pour voir !)

	Voyons les différentes parties :
	
	partie (*1)

	Si j'appuie sur gauche ou droite, on ajoute (ou retranche) au vecteur vx la constante lateralspeed
	Si vous la changez, vous pousserez plus ou moins fort votre personnage. Essayez ! Il aura donc un démarrage plus ou moins lent. Il sera mou ou vivace.

	partie (*2)

	Je dis que si on est en l'air, alors on n'aura pas la même impacte en faisant bouger notre personnage. En effet, on pourra le bouger en l'air, mais pas à la même vitesse.
	A vous d'ajuster la constante airlateralspeedfacteur pour changer ça. Une valeur de 1.0 fera que vous aurez la même impacte au sol qu'en l'air.

	partie (*3)

	Important si vous ne voulez pas être sur une patinoire : l'adhérence. 
	Si on relâche gauche et droite, et qu'on est au sol (important), alors on va s'arrêter rapidement.
	Le facteur adherance va jouer la dessus. Si vous mettez adherence = 1.0 par exemple, vous êtes sur la glace ! Essayez !
	Plus vous montez l'adherence, plus l'arrêt est brusque. Une valeur en dessous de 1.0 est un non sens.
	Notez que je ne teste l'adhérence qu'au sol : on ne freine pas en l'air :)

	partie (*4)

	La vitesse maximale est importante : sinon, vous pouvez accélérer jusqu'à devenir une fusée. Simple test ici.

	Il nous reste les fonctions Saute, Gravite, et ControleSol. Nous gardons le saut pour la fin.

	Gravite
	-------

	Quoiqu'il arrive, dans tous les cas, la gravité vous pousse vers le bas.
	Le paramètre factgravite va définir cette force, elle s'ajoute simplement au vecteur vy à chaque instant.
	Jouez avec ce paramètre pour être sur la Terre ou sur la Lune.

	Nous verrons pendant le saut le if de cette fonction.

	ControleSol
	-----------

	Si la gravité nous pousse toujours vers le bas, en revanche, un sol compense toujours cela.
	Ici, j'ai dit que le sol invisible est au dela de la coordonnées 250.

	Donc si on dépasse cette valeur, on se remet dessus, et, si la composante du vecteur y était vers le bas, on l'annule.
	Si on programme avec des sons, on pourra, à ce moment la, faire un bruit de pieds qui tape au sol.

	Et dans ce cas, on remet le statut à "AU_SOL"

	Saut
	----

	Cette fonction ne peut être appelée que si le statut est au sol : on ne saute pas si on est déjà en l'air. 
	(si vous voulez gérer un double saut, il faudra alors rajouter un compteur)

	Elle est très simple : elle donne une valeur "impulsion" à vy, et surtout, elle met le statut à "EN L'AIR"

	Voila, avec tout ça, on a à peu près quelque chose de réaliste (Newton serait content, sauf s'il lit la suite)

	Revenons maintenant sur le if de la fonction gravité : c'est TOTALEMENT ARTIFICIEL et contraire aux lois de la physique :
	En effet, ce if permet de contrôler des sauts plus ou moins longs.

	Dans la réalité, pour sauter plus ou moins haut, il faut jouer sur l'impulsion : soit on pousse fort sur son pied, soit peu.
	Mais une fois qu'on a commencé le saut, les dés sont jetés, on le subit.

	Mais dans les jeux vidéos, on contrôle la longueur d'un saut alors qu'on est déjà en l'air : si on est en train de monter et qu'on maintient
	le bouton de saut, alors on continue à monter, ce qui n'est pas comme dans la réalité.

	Alors on est obligé de tricher.
	Pour cela, une solution est d'atténuer la gravité pendant le saut. Mais c'est UNE solution, et non LA solution ultime.

!*/

/*!E
	No explanations yet.
!*/

/*
titre: Saut pour jeu de plateforme.
titreE: Jump for a platform game.
meta: C;SDL;jump
metaE: C;SDL;jump
desc: Un exemple pour montrer comment on saute dans les jeux de plateforme.
descE: A sample showing how to jump with SDL for a platform game
cle  : 
*/
