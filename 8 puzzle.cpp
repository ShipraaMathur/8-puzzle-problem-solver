#include <iostream>
#include<bits/stdc++.h>
using namespace std;
struct Node 
{ 
	// stores the parent node of the current node 
	// helps in tracing path when the answer is found 
	Node* parent; 

	// stores matrix 
	int mat[3][3]; 

	// stores blank tile coordinates 
	int x, y; 

	// stores the number of misplaced tiles 
	int heur; 

	// stores the number of moves so far 
	int level; 
}; 

int printMatrix(int mat[3][3]) 
{ 
  printf("\n");
	for (int i = 0; i < 3; i++) 
	{ 
	    printf("+---+---+---+\n");
		for (int j = 0; j < 3; j++) 
		{	
			printf("| %d ", mat[i][j]); 
    }
		printf("|\n"); 
	} 
	printf("+---+---+---+\n");
} 

int isValid(int k, int i, int j, int Matrix[3][3])
{
  if(k<9 && k>=0)
  {
    int flag = 1;
    for( int a = 0 ; a <= i ; a++)
    {
      for( int b = 0 ; b <= j ; b++)
      {
        if(k == Matrix[a][b])
        {
          flag = 0;
        }
      }
    }
    if(flag == 0)
    { 
       return 0;
    }
    else 
    {
      return 1;
    }  
  }
  else
  {  return 0;}
}
int heur(int Initial[3][3],int Final[3][3])
{
  int count =0;

  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
    {
      if(Initial[i][j]!=Final[i][j])
      {
        count++;
      }
    }
  }
  return count;
}
bool check(int a,int b)
{
  if(a<3 && a>=0 && b<3 && b>=0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}


void swap(int temp[3][3],int x,int y, int newX, int newY)
{
  int t = temp[x][y];
  temp[x][y] = temp[newX][newY];
  temp[newX][newY] = temp[x][y];
}

Node* newNode(int mat[3][3], int x, int y, int newX, 
			int newY, int level, Node* parent) 
{ 
	Node* node = new Node; 
	// set pointer for path to root 
	node->parent = parent; 

	// copy data from parent node to current node 
	memcpy(node->mat, mat, sizeof node->mat); 

	// move tile by 1 position 
	swap(node->mat[x][y], node->mat[newX][newY]); 

	// set number of misplaced tiles 
	node->heur = INT_MAX; 

	// set number of moves so far 
	node->level = level; 

	// update new blank tile cordinates 
	node->x = newX; 
	node->y = newY; 

	return node; 
} 
int row[] = { 1, 0, -1, 0 }; 
int col[] = { 0, -1, 0, 1 }; 

void printPath(Node* root) 
{ 
	if (root == NULL) 
		return; 
	printPath(root->parent); 
	printMatrix(root->mat); 

	printf("\n"); 
} 
bool compareCost(Node *i1, Node *i2) 
{ 
    return (i1->heur < i2->heur); 
} 
  
void Solve(int Initial[3][3],int Final[3][3],int x, int y)
{
  Node *root = newNode(Initial, x, y , x , y , 0 , NULL );
  
  root->heur = heur(Initial,Final);
  Node *temp = root;

  vector <Node*> Pq;
  Pq.emplace_back(root);
  while(!Pq.empty())
  {
    Node *Minimum = Pq[0];
    Pq.erase (Pq.begin());
    if(Minimum -> heur == 0)
    {
      printPath(Minimum);
      return;
    }
    Node *child=NULL;
    for(int i=0;i<4;i++)
    {
      int newX = x + row[i];
      int newY = y + col[i];
      if( check ( newX , newY ) )
      {
        child= newNode(temp->mat, x, y, newX, newY, temp->level+1,temp);
        child->heur = heur(temp->mat, Final);
        Pq.emplace_back(child);
      }
    }
    sort(Pq.begin(),Pq.end(),compareCost);
  }

}

int main() {
  int Initial[3][3], Final[3][3],x,y; 
  for(int i = 0 ; i< 3; i++)
  {
    for(int j = 0 ; j < 3 ; j++)
    {
      label:
      printf("\nEnter Input Matrix[%d][%d]", i,j);
      int k; 
      scanf("%d", &k);
      if(isValid(k,i,j,Initial))
      {
        Initial[i][j] = k;
      }
      else{
        cout<<"Invalid Input, please try again...";
        goto label;
      }
      if(k==0){x=i;y=j;}
    }
  }
  cout<<"\nThe Input Matrix Is :\n";
  printMatrix(Initial);
  cout<<"\n The position of the blank space is : "<<x<<"  :  "<<y<<endl;
  for(int i = 0 ; i< 3; i++)
  {
    for(int j = 0 ; j < 3 ; j++)
    {
      label1:
      printf("\nEnter Goal Matrix[%d][%d]", i,j);
      int k; 
      scanf("%d", &k);
      if(isValid(k,i,j,Final))
      {
        Final[i][j] = k;
      }
      else{
        cout<<"Invalid Input, please try again...";
        goto label1;
      }
    }
  }
  cout<<"\nThe Goal Matrix Is :\n";
  //printMatrix(Final);

  printf("____________________");
  Solve(Initial, Final, x,y);

  return 0;
}

