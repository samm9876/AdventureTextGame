#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define PI 3.14159265

void playgame(int index, char name[], int level);
void attack(int index, char name[], int level, int playerhealth, int curplayerhealth, char creature, int enlevel, int enemyhealth, int curenemyhealth);
struct enemy genenemy(int charlevel);

typedef struct character{
    int filenumber;                
    char *name;
    int level;          
    struct character *next;
} charData;

struct enemy{
    int enlevel;
    char *creature;
};

int main(){
    srand(time(NULL));
	printf("\033[2J\033[1;1H");
	FILE* file = fopen("title.txt", "r");
	char line[256];
    while (fgets(line, sizeof(line), file)) {

        	printf("%s", line); 
    }
    fclose(file);
    printf("\n");

	printf("1: New Game\n2: Load Game\n3: Quit\n\n");	
	char selection[2];
	do {
        printf("\033[F");
		fgets(selection, 2, stdin);
    } while (!strcspn(selection, "\n"));
    int intselection = strtol(selection, NULL, 10);
	int c; while ( (c = getchar()) != '\n' && c != EOF ) { }
    char username[32];
	
	switch(intselection){
	case 1:
		do {printf("Please enter your name: ");
			fgets(username, 32, stdin);
       	} while (!strcspn(username, "\n"));
	   	username[strcspn(username, "\n")] = 0;
        
        char ch;
        int linecount = 0;
        FILE *f = fopen("savefiles.txt", "r");
        while ((ch=getc(f)) != EOF) {
		   if (ch == '\n') { ++linecount; }
        }
        fclose(f);

        fopen("savefiles.txt", "a");
        fprintf(f,"%d %s 1\n",linecount+1,username);
        fclose(f);

        printf("Welcome warrior %s!\n", username);

        playgame(linecount+1,username,1);        

		break;
	case 2:
        printf("\n");
        printf("Please select a user:\n");
        FILE* file = fopen("savefiles.txt", "r"); 
        int filenumber, level;                
        char name[100];
        charData *first = NULL; charData *last = NULL;
        charData *node;
	    char line[100];
        int numofchars = 0;
        while (fgets (line, sizeof(line), file) != NULL) {
            numofchars++;
            line[strlen (line)-1] = '\0';
            sscanf (line, "%d %s %d", &filenumber, name, &level);
            node = malloc (sizeof (charData));

            node->filenumber = filenumber;
            node->name = strdup(name);
            node->level = level;
            node->next = NULL;

            if (first != NULL) {
                last->next = node;
                last = node;
            } 
            else {
                first = node;
                last = node;
            }
        }

        printf("(Number of Characters: %d)\n", numofchars);

        fclose (file);
        node = first;
        printf ("\n   Name (Level):\n\n");
        while (node != NULL) {
            printf ("%d: %s (%d)\n", node->filenumber, node->name, node->level);
            node = node->next;
        }

    	printf("\n0: Return to main menu\n\n");
        
        int intselection = numofchars+1;
        do{
            char *selection = malloc(1);
            int i = 0; int c;
            while((c = getchar()) != '\n' && c != EOF)
            {
                selection[i++] = c;
                selection = realloc(selection, i+1); /* must be i+1 not i++*/
            }
            selection[i] = '\0';

            intselection = strtol(selection, NULL, 10);
            free(selection);
        }while(intselection > numofchars);

        if (intselection == 0) break;
        else {
            node = first;
            for (int i = 0; i < intselection-1; i++) {
                node = node->next;
            }
        }
        printf ("Level %d character %s loaded.\n", node->level, node->name);

        playgame(intselection,node->name,node->level);           

		break;
	case 3:
		break;
	default:
		printf("Invalid input\n");
	}
	return 0;
}





void playgame(int index, char name[], int level){
    printf("index: %d, name: %s, level: %d\n",index,name,level);

do{
        printf("\nPrepare for battle...\n\n");
        struct enemy curenemy = genenemy(level);

        printf("A level %d %s approaches!\n\n", curenemy.enlevel,curenemy.creature);
        int playerhealth = (level/2)+7;
        int enemyhealth = (curenemy.enlevel/2)+7;
        int curplayerhealth = playerhealth;
        int curenemyhealth = enemyhealth;
        printf("%s (lvl %d) HP:%d/%d\n", name,level,curplayerhealth,playerhealth);
        printf("%s (lvl %d) HP:%d/%d\n\n", curenemy.creature,curenemy.enlevel,curenemyhealth,enemyhealth);
        printf("1: Attack\n2: Defend\n3: Run\n\n");
	
        char selection[2];
        do {
            printf("\033[F");
		    fgets(selection, 2, stdin);
        } while (!strcspn(selection, "\n"));
        int intselection = strtol(selection, NULL, 10);
	    int c; while ( (c = getchar()) != '\n' && c != EOF ) { }
        char username[32];
	
	    switch(intselection){
	    case 1: attack(index, name, level, playerhealth, curplayerhealth, *curenemy.creature, curenemy.enlevel, enemyhealth, curenemyhealth); break;
	    case 2: printf("Number two!\n");break;
	    case 3: printf("Number three!\n");break;
        default: printf("Invalid selection\n");
        }
    }while(1);
}

struct enemy genenemy(int charlevel){
        int enlevel = charlevel+(rand()%2);
        char ch;
        int animalmax = 0;
        FILE *anfile = fopen("animal-list.txt", "r");
        while ((ch=getc(anfile)) != EOF) {
		   if (ch == '\n') { ++animalmax; }
        }
        fclose(anfile);
        
        char creature[32];
        int animalno = (rand() % (animalmax + 1));
        fopen("animal-list.txt", "r");
        anfile = fopen("animal-list.txt", "r");
        fscanf(anfile, "%s", creature);
        for(int i = 0; i < animalno; i++ ){
            fgets(creature, 255, (FILE*)anfile);
        }
        creature[strcspn(creature, "\n")] = 0;
        fclose(anfile);
        struct enemy curenemy = {enlevel,creature};
        return curenemy;
}

void attack(int index, char name[], int level, int playerhealth, int curplayerhealth, char creature, int enlevel, int enemyhealth, int curenemyhealth){
        float hitchance = 0.33333+(2/PI*atan(level))/2;
        hitchance =  hitchance-0.125+((double)rand() / (double)RAND_MAX)/4 ;
        printf("\nHitchance: %f",hitchance);
}
