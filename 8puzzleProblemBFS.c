#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct node
{
	int puzzle[9];            //Stores the current state of the node/puzzle
	struct node *parent;      //stores the parent of the the current node in the tree
	struct node *childRight;  //Child obtained after moving empty piece right
	struct node *childUp;     //Child obtained after moving empty piece up
	struct node *childDown;   //Child obtained after moving empty piece down
	struct node *childLeft;   //Child obtained after moving empty piece left
	struct node *Qnext;       //pointer for the next node in a queue
	int index;               // tells the index of '0' in the current node/puzzle
}Node;

int nodecount=0;            //Counts the number of nodes that will be generated 
int Qcount=0;               //counts the number of nodes added in the queue 
double runtime;
int finalState[9];
int initial[9];
int nodesExpanded = 0;
int Depth = 0;
	
//creating children for a particular node/puzzle
void childrenNodes(Node *root)
{
	nodesExpanded++;
	int index1;
	//first find the location of the empty space in the puzzle node
	for(int i=0;i<9;i++)
	{
		if(root->puzzle[i]==0)
		{
			root->index=i;
			index1=i;
			break;
			
		}
	}
	
	int temp;
	int childNew[9];
	if(index1%3<2) // means we can move right
	{
		//first create a copy of the original puzzle node obtained
		
		for(int i=0;i<9;i++)
		{
			childNew[i]=root->puzzle[i];
		}
		
		//now form the new childRight and assign it to root->childRight
		root->childRight=malloc(sizeof(Node));
		
		temp=childNew[index1];
		childNew[index1]=childNew[index1+1];
		childNew[index1+1]=temp;
		
		root->childRight->parent=root;
		root->childRight->childDown=NULL;
		root->childRight->childUp=NULL;
		root->childRight->childRight=NULL;
		root->childRight->childLeft=NULL;
		root->childRight->Qnext=NULL;
		root->childRight->index=-1;
		
		for(int i=0;i<9;i++)
		{
			root->childRight->puzzle[i]=childNew[i];
		}
		nodecount++;
		//printf("%d\n",nodecount);
		//printf("child found\n");
	}
	
	if(index1%3>0)  //means we can move left
	{
		//first create a copy of the original puzzle node obtained
		
		for(int i=0;i<9;i++)
		{
			childNew[i]=root->puzzle[i];
		}
		
		//now form the new childRight and assign it to root->childLeft
		root->childLeft=malloc(sizeof(Node));
		
		temp=childNew[index1];
		childNew[index1]=childNew[index1-1];
		childNew[index1-1]=temp;
		
		root->childLeft->parent=root;
		root->childLeft->childDown=NULL;
		root->childLeft->childUp=NULL;
		root->childLeft->childRight=NULL;
		root->childLeft->childLeft=NULL;
		root->childLeft->Qnext=NULL;
		root->childLeft->index=-1;
		
		for(int i=0;i<9;i++)
		{
			root->childLeft->puzzle[i]=childNew[i];
		}
		nodecount++;
		//printf("%d\n",nodecount);
		//printf("child found\n");
	}
	if(index1>2)  //means we can move up
	{
		//first create a copy of the original puzzle node obtained
		
		for(int i=0;i<9;i++)
		{
			childNew[i]=root->puzzle[i];
		}
		
		//now form the new childRight and assign it to root->childUp
		root->childUp=malloc(sizeof(Node));
		
		temp=childNew[index1];
		childNew[index1]=childNew[index1-3];
		childNew[index1-3]=temp;
		
		root->childUp->parent=root;
		root->childUp->childDown=NULL;
		root->childUp->childUp=NULL;
		root->childUp->childRight=NULL;
		root->childUp->childLeft=NULL;
		root->childUp->Qnext=NULL;
		root->childUp->index=-1;
		
		for(int i=0;i<9;i++)
		{
			root->childUp->puzzle[i]=childNew[i];
		}
		nodecount++;
		//printf("%d\n",nodecount);
		//printf("child found\n");
	}
	if(index1<6) //means we can move down
	{
		//first create a copy of the original puzzle node obtained
		
		for(int i=0;i<9;i++)
		{
			childNew[i]=root->puzzle[i];
		}
		
		//now form the new childRight and assign it to root->childDown
		root->childDown=malloc(sizeof(Node));
		
		temp=childNew[index1];
		childNew[index1]=childNew[index1+3];
		childNew[index1+3]=temp;
		
		root->childDown->parent=root;
		root->childDown->childDown=NULL;
		root->childDown->childUp=NULL;
		root->childDown->childRight=NULL;
		root->childDown->childLeft=NULL;
		root->childDown->Qnext=NULL;
		root->childDown->index=-1;
		
		for(int i=0;i<9;i++)
		{
			root->childDown->puzzle[i]=childNew[i];
		}
		nodecount++;
		//printf("%d\n",nodecount);
		//printf("child found\n");
	}
}
int checkIfpresentinQueue(Node *head,Node *check)
{
	int result=0;
	int flag;
	Node *current=head;
	while(current!=NULL)
	{
		flag=0;
		for(int j=0;j<9;j++)
		{
			if(current->puzzle[j]!=check->puzzle[j])
			{
				flag=1;
				break;
			}
			
		}
		if(flag==0)
		{
			result=1;
			return result;
		}
		current=current->Qnext;
			
	}
	return result;
}

//Displaying the current node
void display(int puzzle[])
{
	printf ("\n");
  for (int i = 0; i < 3; i++)
    {
      printf ("+---+---+---+\n");
      for (int j = 0; j < 3; j++)
	{
	  printf ("| %d ", puzzle[3* i + j]);
	}
      printf ("|\n");
    }
  printf ("+---+---+---+\n");
}

//Function to test if the goal state has been found
int goalTest(Node *current)
{
	//comparing the current state to the goal state
	int goal=1;
	for(int i=0;i<9;i++)
	{
		if(current->puzzle[i]!=finalState[i])
		{
			goal=0;
			return goal;
		}
	}
	return goal;
}

//function to print the moves to be made by tracing back the algorithm using recursion
void tracePath(Node *root)
{
	if(root!=NULL)
	{
		tracePath(root->parent);
		display(root->puzzle);
		Depth ++;
	}
	return;
}

void enqueue(Node **head,Node *insert)
{
	Node *current,*new;
	new=malloc(sizeof(Node));
	new=insert;
	current=*head;

	if(current==NULL)
	{
		*head=new;
		Qcount++;
		return;
	}
	while(current->Qnext!=NULL)
		current=current->Qnext;
	current->Qnext=new;
	Qcount++;
	return;
}

//Function to dequeue from the head of the list
Node *dequeue(Node **head)
{
	Node *p;
	p=*head;
	if(p==NULL)
	{
		printf("Underflow has occurred in the Queue ,no items available \n");
		return NULL;
	}
	*head=p->Qnext;
	Qcount--;
	return p;
}
		

int main()
{
	Node *Queuehead;       //Forming a Queue for BFS algorithm

	Queuehead=malloc(sizeof(Node));
	Queuehead=NULL;
	
	// Creating the initial State and final state 
	

	printf("\nEnter the Initial Matrix...\n");
	for(int i = 0 ; i < 9 ; i ++)
	{
	    int r = i/3;
	    int c = i%3;
	    printf("Enter Initial[%d][%d]", r, c);
	    scanf("%d", &initial[r*3 + c]);
	}
	
	printf("\nEnter the Final Matrix...\n");
	for(int i = 0 ; i < 9 ; i ++)
	{
	    int r = i/3;
	    int c = i%3;
	    printf("Enter Final[%d][%d]", r, c);
	    scanf("%d", &finalState[r*3 + c]);
	}

	Node *root=NULL;               //Assigning the inital state to the root of the tree
	root=malloc(sizeof(Node));
	for(int i=0;i<9;i++)
	{
		root->puzzle[i]=initial[i];
	}
	root->parent=NULL;
	root->childDown=NULL;
	root->childUp=NULL;
	root->childRight=NULL;
	root->childLeft=NULL;
	root->Qnext=NULL;
	root->index=-1;

	printf("The initial state of the puzzle is -\n");
	display(root->puzzle);
	
	enqueue(&Queuehead,root);      //Adding the first node to the queue
	 
	printf("The goal state of the puzzle is -\n");
	display(finalState);

	//BREADTH FIRST SEARCH
	int goalFound=0;
	clock_t start = clock();

	while(Qcount>0 && goalFound==0)
	{
		Node *current=dequeue(&Queuehead);
        		
		
		childrenNodes(current);      //generating the children nodes of the current node
		if(current->childDown!=NULL)
		{
			Node *currentChild1=current->childDown;
			if(goalTest(currentChild1))   //Test if the goal state has been found 
			{
				printf("Goal State Found,now tracing back and displaying the moves to be made \n");
				
				goalFound=1;
				tracePath(currentChild1);
				printf("\nNo. of nodes generated for the BFS search were %d\n",nodecount);
				break;
			}
			if(!checkIfpresentinQueue(Queuehead,currentChild1))  //Before adding a node to the queue ,we check if the node has already been achieved and added to the queue
				enqueue(&Queuehead,currentChild1);
			
		}
		if(current->childUp!=NULL)
		{
			Node *currentChild2=current->childUp;
			if(goalTest(currentChild2))
			{
				printf("Goal State Found,now tracing back and displaying the moves to be made \n");
				goalFound=1;
				tracePath(currentChild2);
				printf("\nNo. of nodes generated for the BFS search were %d\n",nodecount);
				break;
			}
			if(!checkIfpresentinQueue(Queuehead,currentChild2))
				enqueue(&Queuehead,currentChild2);
		}
		
		if(current->childRight!=NULL)
		{
			Node *currentChild3=current->childRight;
			if(goalTest(currentChild3))
			{
				printf("Goal State Found,now tracing back and displaying the moves to be made \n");
				goalFound=1;
				tracePath(currentChild3);
				printf("\nNo. of nodes generated for the BFS search were %d\n",nodecount);
				break;
			}
			if(!checkIfpresentinQueue(Queuehead,currentChild3))
				enqueue(&Queuehead,currentChild3);
		}
		if(current->childLeft!=NULL)
		{
			Node *currentChild4=current->childLeft;
			if(goalTest(currentChild4))
			{
				printf("Goal State Found,now tracing back and displaying the moves to be made \n");
				goalFound=1;
				tracePath(currentChild4);
				printf("\nNo. of nodes generated for the BFS search were %d\n",nodecount);
				break;
			}
			if(!checkIfpresentinQueue(Queuehead,currentChild4))
				enqueue(&Queuehead,currentChild4);
		}
	}
	runtime = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("\nThe number of Nodes Expanded were : %d", nodesExpanded);
    printf("\nThe Depth of the Program is : %d", Depth-1);
    printf("\n\nThe time taken by the program was :  %lf" ,1.1 * runtime );
}
	
			
	
	
	
	



				
	
	
		
		