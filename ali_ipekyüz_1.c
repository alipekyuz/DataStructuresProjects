/**
*Ali Ýpekyüz - 150112006
*/

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<dirent.h>
#include <string.h>


#define MAXREVIEWTEXTLENGHT 50000

struct nodeScore {
	double score_number;
    struct nodeScore *nextScoreNodePtr; 
    struct nodeReview *reviewPtr;
	
};

typedef struct nodeScore nodeScore;
typedef struct nodeScore *nodeScorePtr;
struct nodeScore *header,*temp2=NULL;

struct nodeReview {
	double score_number;
	int id;
	int length;
	char text[MAXREVIEWTEXTLENGHT];
	int polarity;	
	struct nodeReview *nextReview;	

};
typedef struct nodeReview *nodeReviewPtr;


FILE *inpf;


DIR *dir;
struct dirent *ent;


int addScore(double score_number);
void addReview(double score_number,int review_id,int length,char text[],int polarity);
void calculateAverageScore();
void listReviewsInASpecificPolarity(int polarity);
void listReviewsContainsAParticularWord(char word[]);
void removeReviewsBetweenAspecificScope(double starting_scope_period,double end_scope_period);
int isScoreNumberExist(double scoreNumber);
void init();
void listAll();


int main(int argc, char** argv) {
	
	
	int selection=0,review_id,length,polarity;
	double score_number;
	double start=0,end=0;
	char text[MAXREVIEWTEXTLENGHT],word[MAXREVIEWTEXTLENGHT];
	nodeScorePtr hdr, p, q, r;
	nodeScorePtr newNode, temp, prev;
	nodeScore *temp2;
	hdr=NULL;	
	init();

	while(selection!=8)
	{	
		printf("\n1:Add a new review-score\n");
    	printf("2:Add some reviews into a review-score\n");
    	printf("3:Calculate the average score of all of the reviews\n");
    	printf("4:Show the review id s and the total number of them in a specific polarity\n");
    	printf("5:List the id s of reviews which have a particular word\n");
    	printf("6:Remove the reviews between in a specific score-scope\n");
    	printf("7:List all text\n");
    	printf("8:Exit\n\n");
    	scanf ("%d", &selection);
    	
     		switch(selection){
    			//Add new review-score..
				case 1:
    				{
    			    	printf("%s\n","Enter the score_number: ");
				        scanf("%lf",&score_number);
				        
				        while(score_number>5){
				        	printf("Please again enter the score number (0/0.5/..../4/4.5/5) :");
				        	scanf("%lf",&score_number);
				        }
				        
				        // if score number for review is not exist.
				        if(isScoreNumberExist(score_number) == 0)
				        { 
					        addScore(score_number);
					        p=hdr;
					            
								while(p!=NULL){
					            	printf("%1.1lf\n",p->score_number);
					            	p=p->nextScoreNodePtr;
					            }				        	
				        }
				        else {
				        	p=hdr;
							printf("The score number for review is already exist.\n");  
							while(p!=NULL){
					            	printf("%1.1lf\n",p->score_number);
					            	p=p->nextScoreNodePtr;
					            }  
				        }		
    				}
    				break;
            	//Add new review and show all of them in screen.
				case 2:
		            printf("Enter the score number of review: ");
			   	    scanf("%lf",&(score_number));
			   	    printf("Enter the your's id of review : ");
			   	    scanf("%d",&(review_id));
			   	    printf("Enter the your review text :  ");
					gets(text);
					gets(text);            		
            		addReview(score_number,review_id,length,text,polarity);
            		listAll();
            		break;
            	
				//Calculate the average score of all of the reviews. 	
            	case 3:
				   calculateAverageScore();
				   break;
				
				//Show reviews's polarity and total review.   
				case 4:
				   printf("Enter the polarity of reviews(-1-0-1): ");
				   scanf("%d",&polarity);
				   temp2=header;
				   char text;
				   int counter=0;
				   while(temp2!=NULL){
						struct nodeReview *tmp=temp2->reviewPtr;
						while(tmp!=NULL){
						   if(polarity==-1){
						     if(strstr (tmp->text,"Cons")!=NULL && strstr (tmp->text,"Pros")==NULL){
						     	counter++;
						   	    printf("\n>>>>Review Id: %d\n%s\n",tmp->id,tmp->text);
						     }
						   }
						   if(polarity==0){
						     if(strstr (tmp->text,"Pros")!=NULL && strstr (tmp->text,"Cons")!=NULL){
						     	counter++;
						        printf("\n>>>>Review Id: %d\n%s\n",tmp->id,tmp->text);
						     }
					       }
						   if(polarity==1){
						     if(strstr (tmp->text,"Pros")!=NULL && strstr (tmp->text,"Cons")==NULL){
						     	counter++;
						     	printf("\n>>>>Review Id: %d\n%s\n",tmp->id,tmp->text);
						     }
						   }
						tmp=tmp->nextReview;
						   }
						temp2=temp2->nextScoreNodePtr;
				   }
				   printf("\nTotal Reviews: %d\n",counter);
				   break;
				
				//Search particular word.
				case 5:
				   printf("Enter your search word: ");
				   gets(word);
				   gets(word);
				   listReviewsContainsAParticularWord(word);	
				   break;
				
				//Deleting in linked list.
				case 6:
				   printf("Starting_scope_period: ");
				   scanf("%lf",&start);
				   printf("End_scope_period: ");
				   scanf("%lf",&end);
				   removeReviewsBetweenAspecificScope(start,end);
				   break;
    			
				//Print all of reviews on screen.
				case 7:
    				listAll();
    				break;
    			
				//Closing the program.	
    			case 8:
    				printf("Ending the program...");
					exit(0);
     		}
   } 
}

  int addScore(double score_number){//This method is my creating my master linked list.
   	
   	nodeScorePtr newNode, temp, prev;

	// create node to insert and assign values to its fields
	newNode=malloc(sizeof(nodeScorePtr));
	newNode->score_number=score_number;
	newNode->nextScoreNodePtr=NULL;
	newNode->reviewPtr=NULL;

	// if LL empty
	if (header == NULL) 
		header=newNode;
	// if LL not empty
	else {
		temp=header;
		while (temp != NULL && temp->score_number <newNode->score_number) {
			prev=temp;
			temp=temp->nextScoreNodePtr;
		}

		if (temp!=NULL && temp->score_number==newNode->score_number) {
			return 0;
		}

		// insert node in a sorted fashion
		if (temp!=NULL)
			newNode->nextScoreNodePtr=temp;

		// if value to insert is the least in LL then have header point to node
		if (temp==header)
			header=newNode;
		// otherwise insert node in correct position
		else 
			prev->nextScoreNodePtr=newNode;
			
			
	}
	return 1; 
	
}

void addReview(double score_number,int review_id,int length,char text[],int polarity){//This method of adding review.
	nodeScore *temp2;
	nodeReviewPtr newReviewNode,tempNodeReviewPtr;
	newReviewNode=malloc(sizeof(nodeReviewPtr));
	
	temp2=header;
	   while(temp2!=NULL){
         
         if(temp2->score_number==score_number){
         	if(temp2->reviewPtr==NULL){
         		temp2->reviewPtr=(struct nodeReview *)malloc(sizeof(struct nodeReview));
	    		temp2->reviewPtr->id=review_id;
	    		strcpy(temp2->reviewPtr->text,text);
	    		temp2->reviewPtr->polarity=polarity;
	    		temp2->reviewPtr->nextReview=NULL;
	    		
	    		
	    		return;
         	}
         	tempNodeReviewPtr=temp2->reviewPtr;
          	while(tempNodeReviewPtr->nextReview!=NULL) 
    	    tempNodeReviewPtr=tempNodeReviewPtr->nextReview;
    	    
    	    tempNodeReviewPtr->nextReview= (struct nodeReview *)malloc(sizeof(struct nodeReview));
			tempNodeReviewPtr->nextReview->id=review_id;
			strcpy(tempNodeReviewPtr->nextReview->text,text);
			tempNodeReviewPtr->nextReview->polarity=polarity;
			tempNodeReviewPtr->nextReview->nextReview=NULL;
		
    	return ;
         }
         
         temp2=temp2->nextScoreNodePtr;
       }
       if(temp2 != NULL)
       {
	       if(temp2->reviewPtr == NULL) temp2->reviewPtr = newReviewNode;
	       else
	       {
	       	 tempNodeReviewPtr = temp2->reviewPtr;
	       	 while(tempNodeReviewPtr->nextReview != NULL)
	       	 {

	       	 	if(tempNodeReviewPtr->id == review_id)
	       	 	{
	       	 		printf("hata");
	       	 	}
	       	 	
	      	 	tempNodeReviewPtr = tempNodeReviewPtr->nextReview;
	       	 }
	       	 
	       	 tempNodeReviewPtr=tempNodeReviewPtr->nextReview;
	       	 
	       }       	
       }
       else printf("\nScore number category for review is not exist.");
	   
}



/**
* Determine whether the given score number is exist for review, or not.
* if exist return 1, else return 0.
*/
int isScoreNumberExist(double scoreNumber)
{
	char *scoreNumberPtr;
	double aa;
	int isExist = 0;
	
	if ((dir = opendir ("ReviewsAboutVideoAndTelevisions")) != NULL) {
	/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
		   scoreNumberPtr = strtok(ent->d_name,"_");
		   aa=atof(scoreNumberPtr);
		   
		   if(aa==scoreNumber){
		   	isExist = 1;
		   	break;
		   }
		}
	closedir (dir);
	}
	
	return isExist;	
}

void init()//This method merge master linked list and review link list.
{
	char c;
	char file[MAXREVIEWTEXTLENGHT];
	char riddenString[MAXREVIEWTEXTLENGHT];
	char fileString[MAXREVIEWTEXTLENGHT];
	double nodeScoreNumber;
	int nodeReviewId,polarity=0;
  	DIR *openFolder;
	struct dirent *dir;
	openFolder = opendir("ReviewsAboutVideoAndTelevisions");
  if (openFolder){
  		readdir (openFolder) ;
	    readdir (openFolder) ;
    while ((dir = readdir(openFolder)) != NULL){
      	sscanf(dir->d_name, "%lf_%d", &nodeScoreNumber,&nodeReviewId);
      	strcpy(file, "");
      	strcpy(file, "ReviewsAboutVideoAndTelevisions\\");       // copies "one" into str_output
	  	strcat(file, dir->d_name);    //  attaches str_two to str_output
	  	inpf=fopen(file,"r");
      
      	if(inpf == NULL){//read string
        	printf("file not found\n");
	  	}else{

			strcpy(fileString,"");
		    strcpy(riddenString,"");
		
			//Polarity control.
			while(fgets(riddenString, sizeof(riddenString), inpf) != NULL){
			    strcat(fileString,riddenString);
      		}	
      		
      		if(strstr (fileString,"Pros")!=NULL){
      			polarity=1;
      		}
      		if(strstr (fileString,"Cons")!=NULL){
      			polarity=-1;
      		}
      		if(strstr (fileString,"Pros")!=NULL && strstr (fileString,"Cons")!=NULL){
      			polarity=0;
      		}
      		
      		addScore(nodeScoreNumber);
      		addReview(nodeScoreNumber,nodeReviewId,strlen(fileString),fileString,polarity);	
      		
	  	}
		  fclose(inpf);
		  	
    }

    closedir(openFolder);
  }	
}


void listAll(){//This method is print all of reviews on screen.
	
	temp2=header;
	char text;
	while(temp2!=NULL){
		struct nodeReview *tmp=temp2->reviewPtr;
			printf("*****************************************");
			printf("\n>>>>>>>Score Number: %1.1lf\n",temp2->score_number);
		while(tmp!=NULL){
			printf("\nReview Id: %d\n",tmp->id);		
			printf("Text: %s",tmp->text);
			printf("\nPolarity: %d\n\n",tmp->polarity);
			tmp=tmp->nextReview;
		}
		temp2=temp2->nextScoreNodePtr;
	}
}


void calculateAverageScore(){//This method calculates the average. 
	int totalNumberOfReviews=0;
	double sum=0;
	int totalNumberOfReviewsPerNodeScore=0;
	temp2 = header;
      while(temp2 != NULL)
      {
      	totalNumberOfReviewsPerNodeScore = 0;
			if(temp2->reviewPtr!=NULL){
				struct nodeReview *tmp=temp2->reviewPtr;
				while(tmp!=NULL){
					totalNumberOfReviewsPerNodeScore++;
					totalNumberOfReviews++;
					tmp=tmp->nextReview;
				}
				sum+=totalNumberOfReviewsPerNodeScore*(temp2->score_number);
			}	
    		temp2=temp2->nextScoreNodePtr;
 }
 	printf("-->The average of all reviews: %2.1f\n",sum / totalNumberOfReviews);
}

void listReviewsContainsAParticularWord(char word[]){//This method is searching specific word.
	struct nodeReview *tmp;
    temp2=header;
    int isFound = 0;

    printf("\n***************************************\n");
    printf("\nSearch Word: %s\n\n",word);
    
	while(temp2!=NULL){
		
		tmp=temp2->reviewPtr;
		while(tmp!=NULL){
			if(strstr (tmp->text,word)!=NULL)
			{
				printf("\nReview Id: %d",tmp->id);
				printf("\nScore Number: %2.1f\n\n",temp2->score_number);
					
				isFound = 1;				
			}	
			tmp=tmp->nextReview;			
		}			
			temp2=temp2->nextScoreNodePtr;
	}
	
	if(isFound == 0) printf("\nWORD IS NOT FOUND.");
	
 printf("\n***********\n");	
}

void removeReviewsBetweenAspecificScope(double starting_scope_period,double end_scope_period){//Deleting in linked list.
	struct nodeScore *currentNodeScore,*nextNodeScore,*tempNodeScore,*previousNodeScore;	
	tempNodeScore = header;
	
	
	if(tempNodeScore != NULL) 
	{
		if(tempNodeScore->score_number == starting_scope_period)
		{
			header = tempNodeScore->nextScoreNodePtr;
			//free(tempNodeScore);
					
			while(header != NULL && header->score_number <=end_scope_period)
			{
				tempNodeScore = header;
				//free(tempNodeScore);
				header = header->nextScoreNodePtr;
			}
		} 
		else 
		{ 
		    currentNodeScore = tempNodeScore->nextScoreNodePtr;
		    
		    while(currentNodeScore != NULL && currentNodeScore->score_number < starting_scope_period)
		    {
		    	previousNodeScore = currentNodeScore;
		    	currentNodeScore = currentNodeScore->nextScoreNodePtr;
		    	
		    }
		
		    tempNodeScore = previousNodeScore;
		    
			while(currentNodeScore != NULL && currentNodeScore->score_number<=end_scope_period)
			{
				nextNodeScore = currentNodeScore->nextScoreNodePtr;
			
			//	free(currentNodeScore);
				
				currentNodeScore = nextNodeScore;
				
			}
			
			tempNodeScore->nextScoreNodePtr = currentNodeScore;
		}
	}
}


