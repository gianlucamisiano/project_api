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
	int sclen; // Quantita di celle allocate (<=size) per persone
    	score **persone; // Array di puntatori dei destinatari di "nameRel"
} report;


entities* entita;
report* output;
int entSize=0; // Tiene conto della dimensione dell'array "entita"
int entAmount=0;
int relSize=0;
int relAmount=0;

report **puntRel;

void inserisciEnt(char *input)
{
	int i;
	if (entAmount==0)
	{
		strcpy(entita[0].nameEnt,input);
		entAmount++;
		return;
	}
	for (i=entAmount-1; (i>=0 && strcmp(entita[i].nameEnt,input)>0);i--)
	{
		strcpy(entita[i+1].nameEnt,entita[i].nameEnt);
	}
	strcpy(entita[i+1].nameEnt,input);
	entAmount++;
	return;
}

void inserisciRel(char *r)
{
	int i;
	for (i=relAmount-1; (i>=0 && strncmp(puntRel[i]->nameRel,r,strlen(r))>0);i--)
	{
		puntRel[i+1]=puntRel[i];
	}
	puntRel[i+1]=&(output[relAmount]);
	relAmount++;
	return;
}

void sortDest(char *r, char *d, int posRel, int posOri)
{
	int i;
	for (i=puntRel[posRel]->size-1; (i>=0 && strncmp((puntRel[posRel]->persone[i])->relazionati,r,strlen(r))>0);i--)
	{
		puntRel[posRel]->persone[i+1]=puntRel[posRel]->persone[i];
	}
	strcpy((puntRel[posRel]->persone[i+1])->relazionati,d);
	(puntRel[posRel]->persone[i+1])->posOrig[posOri]=1;
	(puntRel[posRel]->persone[i+1])->amount++;
	//for (int j=0; j<puntRel[posRel]->size+1; j++) printf("SORTDEST: relazionato %s \n",puntRel[posRel]->persone[j]->relazionati);
	puntRel[posRel]->size++;
}

int searchRel(char *s, int sx, int dx){
	if(sx>dx) return -1;
	int cx=(sx+dx)/2;
	int cmp=strncmp(s,puntRel[cx]->nameRel,strlen(s));
	if(cmp==0) return cx;
	if(cmp>0) return searchRel(s,cx+1,dx);
	if(cmp<0) return searchRel(s,sx,cx-1);
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

int searchDest(score **a,char *s, int sx,int dx){
	if(sx>dx) return -1;
	int cx=(sx+dx)/2;
	int cmp=strcmp(s,a[cx]->relazionati);
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
	int posOri=searchEnt(entita,orig,0,entAmount-1);
	if (posOri==-1) return;
	char* dest=strtok(NULL,del);
	char* nameRel=strtok(NULL,del);
	int posizioneRelazione=searchRel(nameRel,0,relAmount-1);
	if (relSize==0)
	{
		output=(report*)malloc(100*sizeof(report));
		puntRel=(report**)malloc(100*sizeof(report*));
		for (int c=0;c<100;c++)
		{
			puntRel[c]=&(output[c]);
			output[c].nameRel=(char*)malloc(50*sizeof(char));
			output[c].size=0;
			output[c].sclen=50;
			output[c].persone=(score**)malloc(50*sizeof(score*));
			for (int d=0;d<50;d++)
			{
				output[c].persone[d]=(score*)malloc(sizeof(score));
				for (int e=0;e<50;e++)
				{
					(output[c].persone[d])->relazionati=(char*)malloc(50*sizeof(char));
					(output[c].persone[d])->posOrig=(int*)malloc(entAmount*sizeof(int));
					for (int f=0;f<entAmount;f++) (output[c].persone[d])->posOrig[f]=0;
				}
			}
		}
		strcpy(output[0].nameRel,nameRel);
		output[0].size++;
		strcpy((output[0].persone[0])->relazionati,dest);
		(output[0].persone[0])->posOrig[posOri]=1;
		(output[0].persone[0])->amount++;
		relAmount++;
		relSize=100;
		return;
	}
	if (posizioneRelazione==-1) // Se non esiste relazione con questo nome in output (MA CE NE SONO ALTRE GIA IN OUTPUT)
	{ 
		if (relAmount==relSize) // TODO Check se va bene ignorare che sclen e size siano uguali (forse va bene ma non so)
		{
			relSize+=10;
			output=realloc(output,(relSize)*sizeof(report));
			puntRel=realloc(puntRel,(relSize)*sizeof(report*));
			for (int i=relAmount; i<relSize;i++) 
			{
				puntRel[i]=&(output[i]); //TODO Check se i tipi sono compatibili
				output[i].nameRel=(char*)malloc(50*sizeof(char));
				output[i].persone=(score**)malloc(50*sizeof(score*));
				output[i].size=0;
				output[i].sclen=50;
				for (int j=0;j<50;j++) 
				{
					output[i].persone[j]=(score*)malloc(sizeof(score));
					for (int a=0;a<50;a++) 
					{
						(output[i].persone[j])->relazionati=(char*)malloc(50*sizeof(char));
						(output[i].persone[j])->posOrig=(int*)malloc(entAmount*sizeof(int));
						for (int b=0; b<entAmount; b++) (output[i].persone[j])->posOrig[b]=0;
					}
				}
			}
		//printf("La relazione non esiste e relAmount = relSize\n");
			// TODO strcpy(output[relAmount].nameRel,nameRel);
			//strcpy(output[relAmount].persone[0].relazionati,dest);
			//output[relAmount].size=1;
			//sortReport(output); // switch con inserimento ordinato	
		}
		//printf("Sono qua\n");
		strcpy(output[relAmount].nameRel,nameRel);
		output[relAmount].size++;
		strcpy((output[relAmount].persone[0])->relazionati,dest);
		(output[relAmount].persone[0])->posOrig[posOri]=1;
		(output[relAmount].persone[0])->amount++;
		//relAmount++;
		inserisciRel(nameRel);
		return;
		//inserisciRel(orig,dest,nameRel); // TODO Implementa ordinato e ricordati il bit e aggiorna relAmount
	}
	else // Se esiste già una relazione che si chiama così in output
	{
		int posDest=searchDest(puntRel[posizioneRelazione]->persone,dest,0,puntRel[posizioneRelazione]->size-1); //TODO edit searchDest
		if (posDest==-1) // Se non esiste il destinatario (fra i "relazionati") 
		{
			if (puntRel[posizioneRelazione]->size==puntRel[posizioneRelazione]->sclen)
			{
				//printf("Il dest non esiste e rialloco spazio\n");
				puntRel[posizioneRelazione]->persone=realloc(puntRel[posizioneRelazione]->persone,(puntRel[posizioneRelazione]->size+10)*sizeof(score));
				puntRel[posizioneRelazione]->sclen+=10;
				for (int a=puntRel[posizioneRelazione]->size; a< puntRel[posizioneRelazione]->sclen;a++)
				{
					(puntRel[posizioneRelazione]->persone[a])->relazionati=(char*)malloc(50*sizeof(char));
					(puntRel[posizioneRelazione]->persone[a])->posOrig=(int*)malloc(entAmount*sizeof(int));
					for (int b=0; b<entAmount; b++) (puntRel[posizioneRelazione]->persone[a])->posOrig[b]=0;
				}
			}
			//printf("Il destinatario non esiste\n");
			sortDest(nameRel, dest, posizioneRelazione, posOri); // TODO Ordine dei relazionati?
			//output[posizioneRelazione].persone=realloc(output[posizioneRelazione].persone,(output[posizioneRelazione].size+1)*sizeof(score));		
			//strcpy(output[posizioneRelazione].persone[output[posizioneRelazione].size].relazionati,dest);
			//if (1==output[posizioneRelazione].persone[output[posizioneRelazione].size].posOrig[posOri]) return;
			//else output[posizioneRelazione].persone[output[posizioneRelazione].size].posOrig[posOri]=1;
			//output[posizioneRelazione].size+=10;
			//sortScore(output[posizioneRelazione].persone);
		}
		else if (posDest!=-1) // Se esiste sia l'origine (in entita) che il destinatario (in persone, dentro output)
		{
			if ((puntRel[posizioneRelazione]->persone[posDest])->posOrig[posOri]==1) return;
			//if (output[posizioneRelazione].persone[posDest].posOrig[posOri]==1) return;
			(puntRel[posizioneRelazione]->persone[posDest])->posOrig[posOri]=1;
			(puntRel[posizioneRelazione]->persone[posDest])->amount++;
			//output[posizioneRelazione].persone[posDest].posOrig[posOri]=1;
			//sortScore(output[posizioneRelazione].persone[posDest]);
		}
	}
	//inserisciRel(nameRel);
	return;
	//relAmount++;
}
void addent (char *ent){
	char del[] = " ";
	char *ptr=strtok(ent,del);
	ptr=strtok(NULL,del);
	if (entSize==0) {
		entSize=50;
		entita=(entities*)malloc(50*sizeof(entities));
		for (int k=0;k<50;k++)
		{
			entita[k].nameEnt=(char*)malloc(50*sizeof(char));
			strcpy(entita[k].nameEnt,"\0");
		}
	}
	else { // Se l'array è non vuoto
		for (int i=0; i<entAmount; i++){ // Controllo se c'è già un'entità con lo stesso nome
			if (strcmp(entita[i].nameEnt,ptr)==0) return;
		}
	}
	if (entSize==entAmount){ // Se ci sono tante entità quanto spazio allocato allora realloca
		entita=realloc(entita,(entAmount+50)*sizeof(entities));
		entSize+=50;
		// TODO inizializzare ciò che è nuovo
	}
//	printf("in entita[entAmount] c'e': %s",entita[entAmount].nameEnt);
	inserisciEnt(ptr);
	//strcpy(entita[entAmount].nameEnt,ptr);	// Copia l'entità (derivante da strtok di ptr) nel primo spazio disponibile
	//entAmount++;
	//for (int j=0;j<entAmount;j++) printf("ADDENT: Entita: %s , entAmount: %d\n",entita[j].nameEnt,entAmount); // Per visualizzare
	return;
}

void delent(char *input) // TODO Tomorrow
{
	char del[] = " ";
	char *ent=strtok(input,del);
	ent=strtok(NULL,del);
	//printf("%s\n",ent);
	int posEnt=searchEnt(entita,ent,0,entAmount-1);
	if (posEnt==-1) return;
	//printf("L'entità %s è al posto %d di entita\n",ent,posEnt);
	int i;
	for (i=posEnt; i<entAmount-1; i++)
	{
		//entita[i]=entita[i+1];
		strcpy(entita[i].nameEnt,entita[i+1].nameEnt);
	}
	//entita[entAmount-1].nameEnt="\0";
	//entita=realloc(entita,(entAmount-1)*sizeof(entita));
	
	for (i=0;i<relAmount;i++)
	{
		for (int j=0; j<puntRel[i]->size;j++)
		{
			if ((puntRel[i]->persone[j])->posOrig[posEnt]==1) 
			{
				(puntRel[i]->persone[j])->amount--; //TODO entita in rel con se stesse
				(puntRel[i]->persone[j])->posOrig[posEnt]=0;
			}		
			int related=searchDest(puntRel[i]->persone,ent,0,puntRel[i]->size-1);
			if(related != -1) 
			{
				for (int b=related;b<puntRel[i]->size-1;b++) 
				{
					puntRel[i]->persone[b]=puntRel[i]->persone[b+1];
				}
				puntRel[i]->size--;
			}
			for (int a=posEnt;a<entAmount-1;a++) 
			{
				(puntRel[i]->persone[j])->posOrig[a] = (puntRel[i]->persone[j])->posOrig[a+1]; 
			}
		}
	}
	entAmount--;
	//entSize--;
	//for (i=0;i<entAmount;i++) printf("DELENT: Entita al posto %d: %s, entAmount: %d\n",i,entita[i].nameEnt,entAmount); //Per visualizzare 
}

void delrel(char *input)  //TODO Check se l'unica cosa da fare nel delrel è eliminare il relazionato e la relazione (eventualmente)
{
	char del[] = " ";
	char *orig=strtok(input,del);
	orig=strtok(NULL,del);
	char* dest=strtok(NULL,del);
	char* nameRel=strtok(NULL,del);
	int posRel=searchRel(nameRel,0,relAmount-1);
	if (posRel==-1) return;
	int posOri=searchEnt(entita,orig,0,entAmount-1);
	if (posOri==-1) return;
	int sizeOfDests=puntRel[posRel]->size;
	int posDest=searchDest(puntRel[posRel]->persone,dest,0,sizeOfDests-1);
	if (posDest==-1) return;
	for (int i=posDest; i<sizeOfDests-1;i++)
	{
		puntRel[posRel]->persone[posDest]=puntRel[posRel]->persone[posDest+1];
	}
	puntRel[posRel]->size--;
	if (puntRel[posRel]->size==0)
	{
		for (int j=posRel; j<relAmount-1; j++)
		{
			puntRel[posRel]=puntRel[posRel+1];
		}
		relAmount--;
	}
	return;
}
//TODO Ask quando usare il FREE
void reportOut()
{	
	if (relAmount==0)
	{
		printf("none\n"); // TODO Check se si debba andare a capo
		return;
	}
	int len[entAmount];
	int max;
	for (int i=0; i<relAmount; i++)
	{
		max=-1;
		for (int a=0; a<entAmount; a++) len[a]=0;
		printf("%s",puntRel[i]->nameRel);
		for (int j=0; j<puntRel[i]->size; j++)		
		{
			if ((puntRel[i]->persone[j])->amount>max)
			{
				max=(puntRel[i]->persone[j])-> amount;
				//printf(" %s ", (puntRel[i]->persone[j])->relazionati);
				for (int a=0; a<entAmount; a++) len[a]=0;
			}
			len[searchEnt(entita,(puntRel[i]->persone[j])->relazionati,0,entAmount-1)]=1; // TODO EDIT for output
		}
		for (int b=0;b<entAmount;b++)
		{
			if (len[b]==1) printf(" %s ", entita[b].nameEnt);
		}
		printf("%d; ",max);
	}
	printf("\n");
	return;
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
		if (0==strncmp("delrel",input,6)) delrel(input);
		if (0==strncmp("report",input,6)) reportOut();
        }      
	return 0;
}




