#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<dirent.h>

#define MAXREVIEWTEXTLENGHT 50000
//Ali Ýpekyüz--150112006


struct Score_Binary_Tree{
	double score_number;
	int totalNumberOfReviews;
	struct review_Binary_Tree* reviewPtr;
	struct Score_Binary_Tree* right;
	struct Score_Binary_Tree* left;
};
typedef struct Score_Binary_Tree node; //global variables...


struct review_Binary_Tree{
	int review_id;
   	char text[MAXREVIEWTEXTLENGHT];
	struct review_Binary_Tree* right;
	struct review_Binary_Tree* left;
	
};
typedef struct review_Binary_Tree nodeReview;

FILE *inpf;
DIR *dir;
struct dirent *ent;


//prototypes of methods
void insert(node ** root, double val);
void insertReview(nodeReview ** root ,int val , char text[]);
void init(node ** root);
void calculateAverageScore(node ** root, nodeReview * review_id);
void removeReviewsBetweenAspecificScope(double starting_scope_period,double end_scope_period);
void print_inorder_review(nodeReview * tree);
void print_inorder(node * tree);
node * search(node ** tree, double val);
void searchReviewWord(node ** root,nodeReview * review_id , char word []);
void searchReview(node ** root,char word []);
void totalReview(nodeReview * tree,int total);
double average();
int isScoreNumberExist(double scoreNumber);
void FindTotalNumberOfReviewsAndTotalReviewScore(node * tree);
int deleteNode( node **root, double nodeValue );
void deleteFromTree(node **nodeToDelete );


int selection;
int totalNumberOfReviews;
double totalReviewScore;
node *temp2 =NULL;

int main()
{
	double add_score_number ;
	int add_review_id;
	double retVal=0;
	double nodeValue;
	char text[MAXREVIEWTEXTLENGHT],word[MAXREVIEWTEXTLENGHT];
	node *root,*tempNodeScore,*insertedScore;
	root = NULL;
	
	init(&root);
	
	
		
	 while(selection!=7){//until 7 it continues asking
	   printf("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n","          MENU\n"
	   "1:Add a new review-score",
	   "2:Add some reviews into a review-score",
	   "3:Calculate the average score of all of the reviews",
	   "4:List the id s of reviews which have a particular word",
	   "5:Remove the reviews between in a specific score-scope",
	   "6:Display",
	   "7:EXIT");
	
	    scanf("%d",&selection);  
		   switch(selection){
		
		      case 1:
		       printf("%s\n","Enter the score_number: ");
		       scanf("%lf",&add_score_number);
		       
		       while(add_score_number>5){
				    printf("Please again enter the score number (0/0.5/..../4/4.5/5) :");
			    	scanf("%lf",&add_score_number);
			   }
		       
			       if(isScoreNumberExist(add_score_number) == 0)
					{ 
					  printf("Score was added.....");
				      insert((&root),add_score_number);   			        	
			        }
			        
			        else
					{
			          printf("The score number already exist.\n");	
			        }
				  		       
		       break;
		       
		      case 2:
		      	
				printf("Enter the score number of review: ");
			    scanf("%lf",&add_score_number);
			   	printf("Enter the your's id of review : ");
			   	scanf("%d",&(add_review_id));
			   	printf("Enter the your review text :  ");
				gets(text);
				gets(text); 
				insertedScore = search(&root,add_score_number); 
				insertReview(&(insertedScore->reviewPtr),add_review_id,text);        		
		       	
		       	break;
		      
		      case 3:
		      	//calculateAverageScore(&root,review_id);
		        FindTotalNumberOfReviewsAndTotalReviewScore(root);
		       printf("Average = %2.1lf",average(root));
		      	break;
		       
		      case 4:
		       printf("Enter your search word: ");
			   gets(word);
	     	   gets(word);
	     	   printf("\n***************************************\n");
               printf("\nSearch Word: %s\n\n",word);
			   searchReview(&root,word);	
				break;
				
			  case 5:
			  	printf("Enter the element to delete : "); 
                scanf_s("%lf", &nodeValue);
                retVal = deleteNode( &root, nodeValue );
                if(retVal == 1)
                    printf("\nDeleted!!\n");
                else
                    printf("\nNot Found!!\n");
			  	break;
		      
			  case 6:
			   print_inorder(root);
		      	break;
		    
		     case 7:
               printf("Ending the program...");
			   exit(0);		     	   	
		   }
	 }
	
}


void init(node ** root)//This method merge master linked list and review link list.
{
	double nodeScoreNumber;
	int reviewId = 0;
  	FILE *openFolder;
  	char orderTxt [MAXREVIEWTEXTLENGHT];
  	char temp;
	char *scoreNumberToken,*reviewIdToken;
	node *tempInsertedNodeScore;
	nodeReview *nodeReviewRoot = NULL;
	char file[MAXREVIEWTEXTLENGHT];
	char riddenString[MAXREVIEWTEXTLENGHT];
	char fileString[MAXREVIEWTEXTLENGHT];
	
	
	openFolder = fopen("InputForProject2\\Order.txt ","r");
    while (fgets(orderTxt,1000,openFolder)!= NULL){
    	sscanf(orderTxt, "%lf_%d", &nodeScoreNumber,&reviewId);
    	
    	//Read review text start
      	strcpy(file, "");
      	strcpy(file, "InputForProject2\\");       // copies "one" into str_output
	  	strcat(file, orderTxt);    //  attaches str_two to str_output
	  	strtok(file,"\r\n");
	  	strcat(file, ".txt"); 
	  	inpf=fopen(file,"r");
      
      	if(inpf == NULL)
		{//read string
        	printf("file not found\n");
	  	}
		else
		{

			strcpy(fileString,"");
		    strcpy(riddenString,"");
				while(fgets(riddenString, sizeof(riddenString), inpf) != NULL){
				    strcat(fileString,riddenString);
				
	      		}
			  
      		insert(root,nodeScoreNumber);
      		tempInsertedNodeScore = search(root,nodeScoreNumber);
      		insertReview(&(tempInsertedNodeScore->reviewPtr),reviewId , fileString);   
			tempInsertedNodeScore->totalNumberOfReviews += 1;			  	
      		
	  	}
		  fclose(inpf);    		
    	// Read review Text end
	}
	  	fclose(openFolder);

}

void insert(node ** root, double val)
{
	node *temp = NULL;
    if(!(*root))
    {
        temp = (node *)malloc(sizeof(node));
        temp->left = temp->right = NULL;
        temp->reviewPtr=NULL;
        temp->score_number = val;
        temp->totalNumberOfReviews = 0;
        *root = temp;
        return;
    }

    if(val < (*root)->score_number)
    {
        insert(&(*root)->left, val);
    }
    else if(val > (*root)->score_number)
    {
        insert(&(*root)->right, val);
    }
}


void insertReview(nodeReview ** root, int val , char text[])
{
	nodeReview *temp = NULL;
    if(!(*root))
    {	  
        temp = (nodeReview *)malloc(sizeof(nodeReview));
        temp->left = temp->right = NULL;
        temp->review_id = val;
        strcpy(temp->text,text);
        *root = temp;	
        return;
    }


    if(val < (*root)->review_id)
    { 
        insertReview(&(*root)->left, val ,text);
    }
    else if(val > (*root)->review_id)
    {
        insertReview(&(*root)->right, val , text);
    }
}

//This method find score_number...
node * search(node ** root, double val)
{
    if(!(*root))
    {
        return NULL;
    }

    if(val < (*root)->score_number)
    {
        search(&((*root)->left), val);
    }
    else if(val > (*root)->score_number)
    {
        search(&((*root)->right), val);
    }
    else if(val == (*root)->score_number)
    {
        return *root;
    }
}


//Printing nodes of tree with inorder..
void print_inorder_review(nodeReview * tree)
{
    if (tree)
    {
    	
        print_inorder_review(tree->left);
        printf("\n---->Review Id: %d\n %s\n",tree->review_id,tree->text);
        print_inorder_review(tree->right);   
    }
}

void print_inorder(node * tree){
	if (tree)
    {
        
        print_inorder(tree->left);
    	printf("\n*****************************************\n");
    	printf("            Score Number: %2.1lf \n\n",tree->score_number);
        print_inorder_review(tree->reviewPtr);        
        print_inorder(tree->right);
    }
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
	
	if ((dir = opendir ("InputForProject2")) != NULL) {
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

//Find the specific word in binary search tree...
void searchReviewWord(node ** root,nodeReview * review_id , char word [])
{
    if((review_id)!=NULL){
    	searchReviewWord(root,review_id->left,word);
    	if(strstr (review_id->text,word)!=NULL)
         {
	        printf("Score Number: %2.1lf    Review Id: %d\n",(*root)->score_number,((*root)->reviewPtr)->review_id);
         } 
         searchReviewWord(root,review_id->right,word);
    }
}

void searchReview(node ** root , char word [])
{
		if((*root)!=NULL){
		     searchReview(&(*root)->left,word);
			 if((*root)->reviewPtr != NULL){
		     	searchReviewWord(root,(*root)->reviewPtr,word);
			 }   
		     searchReview(&(*root)->right,word);
	    }    	    
}


//Find to total number of reviews and review-score number...
void FindTotalNumberOfReviewsAndTotalReviewScore(node * tree)
{
	if (&(*tree)!=NULL)
    { 
        FindTotalNumberOfReviewsAndTotalReviewScore((tree)->left);
            
        
        totalReviewScore += (tree->score_number) * (tree->totalNumberOfReviews);
        totalNumberOfReviews += tree->totalNumberOfReviews;
        
        FindTotalNumberOfReviewsAndTotalReviewScore(tree->right);
    }
}

//This method find the average score of all of the reviews.
double average()
{
	return totalReviewScore / totalNumberOfReviews;
}


//Deleted Method (deleteNode---deleteFromTree)..
int deleteNode( node **root, double nodeValue )
{
    node * currentNode = *root;
    node* trailNode = NULL; 
    int found = 0;
 
    while(!found && currentNode != NULL)
    {
        if( currentNode->score_number == nodeValue )
            found = 1;
        else
        {
            trailNode = currentNode;
            if( currentNode->score_number > nodeValue )
                currentNode = currentNode->left;
            else
                currentNode = currentNode->right;
        }
    }
 
    if(found == 1)
    {
        if( trailNode == NULL )
            deleteFromTree( &currentNode );
        else if( trailNode->score_number > nodeValue )
            deleteFromTree( &trailNode->left);
        else
            deleteFromTree( &trailNode->right);
    }
 
    return found;
}
 
void deleteFromTree( node **nodeToDelete )
{
     node* tempNode = NULL;
     node* currentNode = *nodeToDelete;
     node* trailNode = NULL;
 
    if( (*nodeToDelete)->left == NULL && (*nodeToDelete)->right == NULL )
    {
        tempNode = *nodeToDelete;
        *nodeToDelete = NULL;
        free( tempNode );
    }
    else if( (*nodeToDelete)->left == NULL )
    {
        tempNode = *nodeToDelete;
        *nodeToDelete = (*nodeToDelete)->right;
        free( tempNode );
    }
    else if( (*nodeToDelete)->right == NULL )
    {
        tempNode = *nodeToDelete;
        *nodeToDelete = (*nodeToDelete)->left;
        free( tempNode );
    }
    else
    {
        currentNode = currentNode->left;
 
        while( currentNode != NULL )
        {
            trailNode = currentNode;
            currentNode = currentNode->right;
        }
 
        (*nodeToDelete)->score_number = trailNode->score_number;
        tempNode = trailNode;
        trailNode = trailNode->left;
        free(tempNode);
    }
}
