#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define PI 3.14159265

void playgame(int index, char name[], int level);
void attack(int level, char* creature, int *curenhealpoint);
void heal(int playerhealth, int *curplayhealpoint); 
void enemymove(int enlevel, char* creature, int playerhealth, int enemyhealth, int *curplayhealpoint, int *curenhealpoint); 
void battlewon(int index, char* name, int level);
void battlelost(int index, char* name, int level);
struct enemy genenemy(int charlevel);
int userintput(int digitlength);

typedef struct character{
    int filenumber;                
    char *name;
    int level;          
    struct character *next;
} charData;

struct enemy{
    int enlevel;
    char* creature;
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
    char username[32];
	
	switch(userintput(1)){
	case 1:
		do {printf("Please enter your name: ");
			fgets(username, 32, stdin);
       	} while (!strcspn(username, "\n"));
	   	username[strcspn(username, "\n")] = 0;
        
        char ch;
        int linecount = 0;
        FILE *f = fopen("savefiles.txt", "r");
        if(f==0)
        {
            FILE *f = fopen("savefiles.txt", "ab+");
        }
        else{
            while ((ch=getc(f)) != EOF) {
		       if (ch == '\n') { ++linecount; }
            }
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
        FILE* file = fopen("savefiles.txt", "r");         
        if(file==0){
            printf("Save files not found.\n\n");
            break;
        }
        printf("Please select a user:\n");
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
                printf("\nPrepare for battle...\n\n");
                struct enemy curenemy = genenemy(level);
                int playerhealth = (level/2)+7;
                int enemyhealth = (curenemy.enlevel/2)+7;
                int curplayerhealth = playerhealth;
                int curenemyhealth = enemyhealth;
            printf("A level %d %s approaches!\n\n", curenemy.enlevel,curenemy.creature);
    do{            

            printf("%s (lvl %d) HP:%d/%d\n", name,level,curplayerhealth,playerhealth);
            printf("%s (lvl %d) HP:%d/%d\n\n", curenemy.creature,curenemy.enlevel,curenemyhealth,enemyhealth);

            if(curplayerhealth <= 0){
                battlelost(index, name, level);
            }

            printf("1: Attack\n2: Heal\n3: Skip Turn\n\n");

	            switch(userintput(1)){
	                case 1: attack(level, curenemy.creature, &curenemyhealth); break;
	                case 2: heal(playerhealth,&curplayerhealth);break;
	                case 3: printf("You skip your turn.\n\n");break;
                    default: printf("Invalid selection\n");
                }
            if(curenemyhealth<=0){
                printf("The %s was slain!\n\n",curenemy.creature);
                battlewon(index, name, level);
                curenemy = genenemy(level);
                playerhealth = (level/2)+7;
                enemyhealth = (curenemy.enlevel/2)+7;
                curplayerhealth = playerhealth;
                curenemyhealth = enemyhealth;
                printf("A level %d %s approaches!\n\n", curenemy.enlevel,curenemy.creature);
            } 
            else{
                enemymove(curenemy.enlevel, curenemy.creature, playerhealth, enemyhealth, &curplayerhealth, &curenemyhealth);
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
        
        int animalno = (rand() % (animalmax + 1));
        fopen("animal-list.txt", "r");
        char line[256];
        char* creature = (char *)malloc(strlen(line)+1);;
        int i = 0;
        while (fgets(line, sizeof(line), (FILE*)anfile)) {
            i++; if(i == animalno ){strcpy(creature,line);}
        }

        creature[strcspn(creature, "\n")] = 0;
        fclose(anfile);
        struct enemy curenemy = {enlevel,creature};
        return curenemy;
}

void attack(int level, char* creature, int *curenhealpoint){

        printf("You swing your sword at the %s...\n\n",creature);

        float damage = 0.0;
        int hitsuccess = 0;
        float hittest = (double)rand() / (double)3000000000;
        float hitchance = 0.33333+(2/PI*atan(level))/2;
        hitchance =  hitchance-0.125+((double)rand() / (double)RAND_MAX)/4;
        //printf("hittest:%f hitchance:%f\n",hittest,hitchance);
        if (hitchance>hittest){
            hitsuccess = 1;
            damage = 1 + level*10*pow((2/PI*atan(level)/2),2);
            damage = ((damage + ((double)rand() / (double)RAND_MAX)/level)*pow((1-(2/PI*atan(level)/2)),5)+1);
            damage = damage * ((double)rand() / (double)RAND_MAX)+1;
        }
        if (hitsuccess){
            printf("You hit the %s for %d points of damage.\n\n",creature,(int)damage); 
            *curenhealpoint = *curenhealpoint - (int)damage;
        }
        else{
            switch((int)rand()%6){
	                case 1: printf("...but the %s dodges your attack!\n\n",creature); break;
	                case 2: printf("...but your sword barely grazes its head.\n\n"); break;
	                case 3: printf("...but your sword bounces off its skin.\n\n"); break;
	                case 4: printf("...but your attack is deflected by a counter attack!\n\n"); break;
	                case 5: printf("...but your swing was too weak to cause any damage.\n\n"); break;
	                case 6: printf("...but you lose grip of the sword and botch your attack.\n\n"); break;
                    default : printf("...and miss.\n\n");}
        }
        
}

void heal(int playerhealth, int *curplayhealpoint){
        printf("You attempt to heal your wounds...\n\n");
        if((int)rand()/(int)(RAND_MAX/100)>50){
            int healamount = (int)rand()/(int)(RAND_MAX/playerhealth)/2;
            printf("You manage to heal %d points of health.\n\n",healamount);
            *curplayhealpoint = *curplayhealpoint + healamount;
            if (*curplayhealpoint>playerhealth){
                *curplayhealpoint = playerhealth;
            }
        }
        else{
            printf("You fail to make a difference.\n\n");
        }
}

void battlewon(int index, char* name, int level){
        printf("You won the battle.\n\n");
        printf("You have gained a level.\n\n");
        printf("Please press Enter to continue.\n\n");
        getchar();
        printf("Prepare for the next battle...\n\n");
}

void battlelost(int index, char* name, int level){
        printf("You have been killed!\n\n");
        printf("%s's adventure has come to an end.\n",name);
        FILE* file = fopen("grave.txt", "r");
        char line[256]; int i=0;
        name[strcspn(name, "\n")] = 0;
        int namelength = strlen(name);
        name[13] = 0;
        int leveldigits = floor(log10(abs(level))) + 1;
        int borderno = (13-namelength)/2;
        int borderno2 = (13-leveldigits-7)/2;
        while (fgets(line, sizeof(line), file)) {
                i++;
            	printf("%s", line);
                if(i==7){
                    printf("           |");
                    for (int i = 0; i < borderno; i++) { printf(" "); }
                    printf("%s",name);
                    for (int i = 0; i < borderno; i++) { printf(" "); }
                    if(namelength%2==0&&namelength<13){ printf(" "); }
                }
                if(i==8){
                    printf("           |");
                    for (int i = 0; i < borderno2; i++) { printf(" "); }
                    printf("Level: %d",level);
                    for (int i = 0; i < borderno2; i++) { printf(" "); }
                    if(leveldigits%2==1){ printf(" "); }
                }
        }
        fclose(file);
        printf("\nPress Enter to exit.\n");
        getchar();
        exit(0);
}

void enemymove(int enlevel, char* creature, int playerhealth, int enemyhealth, int *curplayhealpoint, int *curenhealpoint){
    if(*curenhealpoint<enemyhealth/2)
    {
        printf("The %s attempts to heal its wounds...\n\n",creature);
        if((int)rand()/(int)(RAND_MAX/100)>50){
            int healamount = (int)rand()/(int)(RAND_MAX/enemyhealth)/2;
            printf("It manages to heal %d points of health.\n\n",healamount);
            *curenhealpoint = *curenhealpoint + healamount;
            if (*curenhealpoint>enemyhealth){
                *curenhealpoint = enemyhealth;
            }
        }
        else{
            printf("It fails to make a difference.\n\n");
        }
    }
    else
    {
        printf("The %s attempts to attack you...\n\n",creature);
        if((int)rand()/(int)(RAND_MAX/100)>20){
            int damage = ((int)rand()/(int)(RAND_MAX/playerhealth))/(2+(int)rand()/(int)(RAND_MAX/2)); 
            printf("It strikes you for %d points of damage!\n\n",damage);
            *curplayhealpoint = *curplayhealpoint - damage;
        }
        else
        {
            printf("...and misses.\n\n");
        }
    }
}

int userintput(int digitlength){
    char selection[digitlength+1];
	do {
		fgets(selection, digitlength+1, stdin);
    } while (!strcspn(selection, "\n"));
    int intselection = strtol(selection, NULL, 10);
	int c; while ( (c = getchar()) != '\n' && c != EOF ) { }
    return intselection;

}
     
