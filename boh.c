#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct entities
{
	char *nameEnt; // Nome dell'entità
} entities;


typedef struct score
{
	char *relazionati; // Stringa destinataria della relazione
	int *posOrig; // Vettore di "entAmount" interi in cui se l'iesimo bit è 1, allora esiste relazione "nameRel" fra "relazionati" e l'iesima entita 
	int amount; // Numero di volte in cui "relazionati" è destinataria di "nameRel" di report (numero di 1 in posOrig)
} score;

typedef struct report
{
	char *nameRel; // Nome della relazione
	int size; // Numero di entita che sono destinatarie di "nameRel" (quindi size di "persone")
    	score *persone; // Array di destinatari di "nameRel"
} report;


entities* entita;
report* output;
int entSize=0; // Tiene conto della dimensione dell'array "entita"
int entAmount=0;
int relSize=0;
int relAmount=0;

int searchRelInRep(report* rep, char *s, int sx, int dx){
	if(sx>dx) return -1;
	int cx=(sx+dx)/2;
	int cmp=strcmp(s,rep[cx].nameRel);
	if(cmp==0) return cx;
	if(cmp>0) return searchRelInRep(rep,s,cx+1,dx);
	if(cmp<0) return searchRelInRep(rep,s,sx,cx-1);
	return -1;
}

int searchOrig(report* rep, char *s, int sx, int dx){
	if(sx>dx) return -1;
	int cx=(sx+dx)/2;
	int cmp=strcmp(s,rep[cx].nameRel);
	if(cmp==0) return cx;
	if(cmp>0) return searchOrig(rep,s,cx+1,dx);
	if(cmp<0) return searchOrig(rep,s,sx,cx-1);
	return -1;
}
int searchEnt(entities *a,char *s, int sx,int dx){
	if(sx>dx) return -1;
	int cx=(sx+dx)/2;
	int cmp=strcmp(s,a[cx].nameEnt);
	if(cmp==0) return cx;
	if(cmp>0) return searchEnt(a,s,cx+1,dx);
	if(cmp<0) return searchEnt(a,s,sx,cx-1);
	return -1;
}

int searchDest(score *a,char *s, int sx,int dx){
	if(sx>dx) return -1;
	int cx=(sx+dx)/2;
	int cmp=strcmp(s,a[cx].relazionati);
	if(cmp==0) return cx;
	if(cmp>0) return searchDest(a,s,cx+1,dx);
	if(cmp<0) return searchDest(a,s,sx,cx-1);
	return -1;
}

void sortEnt(entities *entita){

    int i, j;
    entities temp;

    for(i=0; i<entAmount-1; ++i)
        for(j=i+1; j<entAmount ; ++j)
        {
            if(strcmp(entita[i].nameEnt, entita[j].nameEnt)>0)
            {
                temp=entita[i];
                entita[i]=entita[j];
                entita[j]=temp;
            }
        }
    return;
}
void sortReport(report *a){

    int i, j;
    report temp;

    for(i=0; i<entAmount-1; ++i)
        for(j=i+1; j<entAmount ; ++j)
        {
            if(strcmp(a[i].nameRel, a[j].nameRel)>0)
            {
                temp=a[i];
                a[i]=a[j];
                a[j]=temp;
            }
        }
    return;
}

void sortScore(score *a){

    int i, j;
    score temp;

    for(i=0; i<entAmount-1; ++i)
        for(j=i+1; j<entAmount ; ++j)
        {
            if(strcmp(a[i].relazionati, a[j].relazionati)>0)
            {
                temp=a[i];
                a[i]=a[j];
                a[j]=temp;
            }
        }
    return;
}


void addrel(char *input)
{
	char del[] = " ";
	char *orig=strtok(input,del);
	orig=strtok(NULL,del);
	char* dest=strtok(NULL,del);
	char* nameRel=strtok(NULL,del);
	int posizioneRelazione=searchRelInRep(output,nameRel,0,relSize-1);
	int posOri=searchEnt(entita,orig,0,entAmount-1); //TODO 9?????
	if ((relSize==0)&&(posOri!=-1))
	{
		output=(report*)malloc(100*sizeof(report));
		relSize=100;
	}
	else if ((posizioneRelazione==-1)&&(posOri!=-1)) // Se non esiste relazione con questo nome in output
	{ 
		if (relAmount==relSize)
		{
			output=(report*)realloc(output,(relSize+10)*sizeof(report));
			output[relAmount].nameRel=(char*)malloc(50*sizeof(char));
			strcpy(output[relAmount].nameRel,nameRel);
			output[relAmount].size=1;
			output[relAmount].persone=(score*)malloc(50*sizeof(score));
			output[relAmount].persone[0].relazionati=(char*)malloc(50*sizeof(char));
			strcpy(output[relAmount].persone[0].relazionati,dest);
			output[relAmount].persone[0].posOrig=(int*)malloc(entAmount*sizeof(int));
			for (int i=0; i<entAmount; i++) output[relAmount].persone[0].posOrig=0;
			output[relAmount].persone[0].posOrig[posOri]=1; // Se il bit di posOrig in una posizione è 1 allora addRel ritorna.
			sortReport(output); // TODO switch con inserimento ordinato
			
		}
		//inserisciRel(orig,dest,nameRel); // TODO Implementa ordinato e ricordati il bit e aggiorna relAmount
	}
	else if ((posizioneRelazione!=-1)&&(posOri!=-1)) // Se esiste già una relazione che si chiama così in output
	{
		int posDest=searchDest(output[posizioneRelazione].persone,dest,0,output[posizioneRelazione].size-1); // TODO Implement (restituisce la posizione del destinatario nell'array "persone")
		if ((posDest==-1)&&(posOri!=-1)) // Se esiste l'origine (in entita) ma non il destinatario (in persone, dentro output)
		{
			output[posizioneRelazione].persone=realloc(output[posizioneRelazione].persone,(output[posizioneRelazione].size+10)*sizeof(score));
			strcpy(output[posizioneRelazione].persone[output[posizioneRelazione].size].relazionati,dest);
			if (1==output[posizioneRelazione].persone[output[posizioneRelazione].size].posOrig[posOri]) return;
			else output[posizioneRelazione].persone[output[posizioneRelazione].size].posOrig[posOri]=1;
			output[posizioneRelazione].size+=10;
			sortScore(output[posizioneRelazione].persone);
		}
		else if ((posDest!=-1)&&(posOri!=-1)) // Se esiste sia l'origine (in entita) che il destinatario (in persone, dentro output)
		{
			if (output[posizioneRelazione].persone[posDest].posOrig[posOri]==1) return;
			output[posizioneRelazione].persone[posDest].posOrig[posOri]=1;
			//sortScore(output[posizioneRelazione].persone[posDest]);
		}
	}
	relAmount++;
	/*if (relSize==0)
	{
		relSize=10;
		output=(report*)malloc(10*sizeof(report));
		for (int i=0;i<10;i++)
		{
			output[i].nameRel=(char*)malloc(30*sizeof(char));
			*output[i].nameRel='\0';
			output[i].amountOfPeople=0;
		}
	}
	else
	{
		int posRel=searchRelInRep(output,nameRel,0,relAmount-1);
		if (posRel!=-1)
		{
				output[posRel].persone
		}
	}
	if (relSize=relAmount)
	{
		output=realloc(output,(relAmount+5)*sizeof(report));
		relSize+=5;
	}
	strcpy(report[relAmount].nameRel,nameRel);
	if((searchRelInRep(output,nameRel,0,relSize-1)!=-1)&&(searchOrig!=-1)&&(searchDest!=-1))
	{
		output[relAmount].persone=(score*)realloc(sizeof(score));
		output[relAmount].persone[output[relAmount].amountOfPeople].amount++;
	}
	relAmount++;
	report[relAmount].persone.relazionati*/
}
void addent (char *ent){
	char del[] = " ";
	char *ptr=strtok(ent,del);
	ptr=strtok(NULL,del);
	if (entSize==0) {
		printf("Sono 1\n");
		entSize=10;
		entita=(entities*)malloc(10*sizeof(entities));
		for (int k=0;k<10;k++)
		{
			entita[k].nameEnt=(char*)malloc(30*sizeof(char));
			*entita[k].nameEnt='\0';
		}
	}
	else { // Se l'array è non vuoto
		printf("Sono 2\n");
		for (int i=0; i<entAmount; i++){ // Controllo se c'è già un'entità con lo stesso nome
			if (strcmp(entita[i].nameEnt,ptr)==0) return;
		}
	}
	if (entSize==entAmount){ // Se ci sono tante entità quanto spazio allocato allora realloca
		printf("Sono 3\n");
		entita=realloc(entita,(entAmount+5)*sizeof(entities));
		entSize+=5;
		// TODO inizializzare ciò che è nuovo
	}
	printf("Metto %s in entita[%d]\n",ptr,entAmount);
	strcpy(entita[entAmount].nameEnt,ptr);	// Copia l'entità (derivante da strtok di ptr) nel primo spazio disponibile
	entAmount++;
	//sortEnt(entita); // TODO Inserimento ordinato
	for (int j=0;j<entAmount;j++) printf("ADDENT: Entita: %s , entAmount: %d\n",entita[j].nameEnt,entAmount); // Per visualizzare
	
	return;
}

void delent(char *input)
{
	char del[] = " ";
	char *ent=strtok(input,del);
	ent=strtok(NULL,del);
	printf("%s\n",ent);
	int posEnt=searchEnt(entita,ent,0,entAmount-1);
	if (posEnt==-1) return;
	printf("L'entità %s è al posto %d di entita\n",ent,posEnt);
	int i;
	for (i=posEnt; i<entAmount-1; i++)
	{
		entita[i]=entita[i+1];
	}
	for (i=0;i<entAmount-1;i++) printf("DELENT: Entita: %s, entAmount: %d\n",entita[i].nameEnt,entAmount-1); //Per visualizzare 
	//entita=realloc(entita,(entAmount-1)*sizeof(entita));
	
	for (i=0;i<relAmount;i++)
	{
		for (int j=0; j<output[i].size;j++)
		{
			if (output[i].persone[j].posOrig[posEnt]==1) output[i].persone[j].amount--;
			int related=searchDest(output[i].persone,ent,0,output[i].size-1);
			if(related != -1) 
			{
				for (int b=related;b<output[i].size-1;b++){ output[i].persone[b]=output[i].persone[b+1]; }
				output[i].size--;
			}
			for (int a=posEnt;a<entAmount-1;a++) { output[i].persone[j].posOrig[a] = output[i].persone[j].posOrig[a+1]; }
		}
	}
	entAmount--;
	//entSize--; Commentato come realloc di entita -1
}

int main() {
    	char input[200];
	while (1)
        {
        	fgets(input,200,stdin);
		input[strlen(input)-1]='\0';
		if (strncmp(input,"end",3) == 0) {
			return 0;
		}
		if (strncmp("addent",input,6) == 0) {
			addent(input);
		}
		if (0==strncmp("delent",input,6)) delent(input);
		if (0==strncmp("addrel",input,6)) addrel(input);
		/*if (0==strncmp("delrel",input,6)) delrel(input);
		if (0==strncmp("report",input,6)) addent(input);*/
        }      
	return 0;
}




