#include <cstdio>

struct Pair 
{
   int first, second;
   Pair(int first=0, int second=0):first(first), second(second){};
   friend bool operator < ( const Pair &, const Pair &);
   friend bool operator == ( const Pair &, const Pair &);
};

class Queue 
{
   private:
       int tail, head;
       int a[10000];
   public:
       Queue();
       int size();
       void push( int v );
       void pop();
       int first();
};

/*******************
******Queue BLOCK***
*******************/

Queue :: Queue()
{
   tail = head = 0;
}

int Queue :: size()
{
   return head - tail;
}

void Queue :: push( int v )
{
   a[head++] = v;
}

void Queue :: pop()
{
   tail++;
}

int Queue :: first()
{
   return a[tail];
}

/*******************
******Pair BLOCK****
*******************/

bool operator < ( const Pair &p1, const Pair & p2 )
{
   return p1.first < p2.first;
}

bool operator == ( const Pair &p1, const Pair & p2 )
{
   return p1.first == p2.first;
}

/****************************
******RED BLACK TREE BLOCK***
****************************/

typedef enum { RED, BLACK } nodeColor;

typedef struct Node {
   Node *right;
   Node *left;
   Node *parent;
   nodeColor color;
   Pair data;
} *PNode;

#define NIL &leaf

Node leaf = { NIL, NIL, 0, BLACK, Pair(0,0)};

class Tree {
protected:
 PNode root;
 void rotateLeft ( PNode );
 void rotateRight ( PNode );
 void restoreAfterInsert ( PNode );
 void restoreAfterDelete ( PNode );

public:
 Tree ()
 {
   root = NIL;
 }
 void deleteNode ( PNode );
 PNode findNode ( Pair );
 PNode findMin ();
 void outTree ( PNode );
 void insertNode ( Pair data );
 PNode getRoot();
};

void Tree :: rotateLeft ( PNode x )
{
 PNode y = x->right;
 x->right = y->left;
 if ( y->left != NIL ) y->left->parent = x;
 y->parent = x->parent;
 if ( x->parent != NIL )
 {
   if ( x == x->parent->left )
     x->parent->left = y;
   else
     x->parent->right = y;
 }
 else
   root = y;
 y->left = x;
 x->parent = y;
}

void Tree ::  rotateRight ( PNode x )
{
 PNode y = x->left;
 x->left = y->right;
 if ( y->right != NIL ) y->right->parent = x;
 y->parent = x->parent;
 if ( x->parent != NIL )
 {
   if ( x == x->parent->right )
     x->parent->right = y;
   else
     x->parent->left = y;
 }
 else
   root = y;
 y->right = x;
 x->parent = y;
}

void Tree ::  restoreAfterInsert ( PNode z )
{
 while ( z->parent->color == RED )
 {
   //!VIOLATION
   if ( z->parent == z->parent->parent->left )
   {
     PNode y = z->parent->parent->right;
     if ( y->color == RED ) {
       //uncle is RED
       z->parent->color = BLACK;
       y->color = BLACK;
       z->parent->parent->color = RED;
       z = z->parent->parent;
     }
     else
     {
       //uncle is BLACK
       if ( z == z->parent->right )
       {
         z = z->parent;
         rotateLeft(z);
       }
       z->parent->color = BLACK;
       z->parent->parent->color = RED;
       rotateRight( z->parent->parent );
     }
   }
   else
   {
     PNode y = z->parent->parent->left;
     if ( y->color == RED )
     {
       //uncle is RED
       z->parent->color = BLACK;
       y->color = BLACK;
       z->parent->parent->color = RED;
       z = z->parent->parent;
     }
     else
     {
       //uncle is BLACK
       if ( z == z->parent->left )
       {
         z = z->parent;
         rotateRight(z);
       }
       z->parent->color = BLACK;
       z->parent->parent->color = RED;
       rotateLeft( z->parent->parent );
     }
   }
 }
 root->color = BLACK;
}

void Tree :: insertNode ( Pair data )
{
 PNode x, y;
 x = root;
 y = NIL;
 while ( x != NIL )
 {
   if ( data == x->data ) return;
   y = x;
   x = ( data < x->data ) ? x->left : x->right;
 }
 PNode z = new Node();
 z->data = data;
 z->parent = y;
 z->left = NIL;
 z->right = NIL;
 z->color = RED;
 if( y != NIL )
 {
   if( data < y->data )
     y->left = z;
   else
     y->right = z;
 }
 else
   root = z;
 restoreAfterInsert( z );
}

void Tree :: restoreAfterDelete ( PNode x )
{
 while( x != root && x->color == BLACK )
 {
   if ( x == x->parent->left )
   {
     PNode w = x->parent->right;//brother
     if ( w->color == RED )
     {
       w->color = BLACK;
       x->parent->color = RED;
       rotateLeft( x->parent );
       w = x->parent->right;
     }
     if ( w->left->color == BLACK && w->right->color == BLACK )
     {
       w->color = RED;
       x = x->parent;
     }
     else
     {
       if ( w->right->color == BLACK )
       {
         w->left->color = BLACK;
         w->color = RED;
         rotateRight ( w );
         w = x->parent->right;
       }
       w->color = x->parent->color;
       x->parent->color = BLACK;
       w->right->color = BLACK;
       rotateLeft ( x->parent );
       x = root;
     }
   }
   else {
     PNode w = x->parent->left;
     if ( w->color == RED )
     {
       w->color = BLACK;
       x->parent->color = RED;
       rotateRight( x->parent );
       w = x->parent->left;
     }
     if ( w->left->color == BLACK && w->right->color == BLACK )
     {
       w->color = RED;
       x = x->parent;
     }
     else
     {
       if ( w->left->color == BLACK )
       {
         w->right->color = BLACK;
         w->color = RED;
         rotateLeft ( w );
         w = x->parent->left;
       }
       w->color = x->parent->color;
       x->parent->color = BLACK;
       w->left->color = BLACK;
       rotateRight ( x->parent );
       x = root;
     }
   }
 }
 x->color = BLACK;
}

void Tree :: deleteNode ( PNode z )
{
 PNode x, y;
 if ( z == NIL ) return;
 if ( z->left == NIL || z->right == NIL )
   y = z;
 else
 {
   y = z->right;
   while( y->left != NIL ) y = y->left;
 }
 if (y->left != NIL)
   x = y->left;
 else
   x = y->right;
 x->parent = y->parent;
 if ( y->parent == NIL )
   root = x;
 else
 {
   if ( y == y->parent->left )
     y->parent->left = x;
   else
     y->parent->right = x;
 }
 if ( y != z ) z->data = y->data;
 if ( y->color == BLACK ) restoreAfterDelete(x);
 delete y;
}

PNode Tree :: findNode ( Pair data)
{
 PNode current = root;
 do {
    if( data == current->data ) return current;
 } while( ( current = ( data < current->data ) ? current->left : current->right ) != NIL );
 return NIL;
}

PNode Tree :: findMin ()
{
 PNode current = root;
 if ( current == NIL ) return NIL;
 while( current->left != NIL ) current = current->left;
 return current;
}

void Tree :: outTree ( PNode t )
{
 if ( t == NIL ) return;
 printf("%d(%6d,%1d,{%1d,%1d}):[%d(%6d,%1d,{%1d,%1d}), %d(%6d,%1d,{%1d,%1d})]\n",
       t,        t->parent,         t->color,        t->data.first,          t->data.second,
       t->left,  t->left->parent,   t->left->color,  t->left->data.first,    t->left->data.second,
       t->right, t->right->parent,  t->right->color, t->right->data.first,   t->right->data.second
 );
 outTree( t->left );
 outTree( t->right );
}

PNode Tree :: getRoot ()
{
 return root;
}

int main()
{
 int n, m;
 int cost[110];
 int car[2100];
 int where[2100];
 scanf("%d %d", &n, &m);
 Queue que;
 Tree* tree = new Tree();
 for( int i=0; i<n; i++ )
 {
     scanf("%d", &cost[i]);
     tree->insertNode( Pair( i+1, cost[i] ) );
 }
// tree->outTree( tree->getRoot() );
 //printf("\n***\n");
 for( int i=0; i<m; i++ ) scanf("%d", &car[i]);
 int qw;
 int sum = 0;
 for( int i=0; i<2*m; i++ )
 {
      scanf("%d", &qw);
      if( qw > 0 )
      {
          if( tree->getRoot() == NIL )
          {
               que.push( qw );
          }
          else
          {
              PNode min = tree->findMin();
              Pair v = min->data;
              tree->deleteNode( min );
              where[qw] = v.first;
              sum += v.second * car[qw-1];
          }
      }
      else
      {
         int v = where[-qw];
         if( que.size() != 0 )
         {
             int vv = que.first();
             que.pop();
             where[vv] = v;
             sum += cost[v-1] * car[vv-1];
         }
         else
         {
             tree->insertNode( Pair(v, cost[v-1]) );
         }
      }
      //printf("%d\n", sum);
      //tree->outTree( tree->getRoot() );
      //printf("\n\n");
 }
 printf("%d\n", sum);
 return 0;
} 
