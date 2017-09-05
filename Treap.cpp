#include <cstdio>
#include <cstdlib>

const int mod = 1000000000;

typedef struct item {
  int key, prior;
  item * l, * r;
  item( int key = 0, int prior = 0 ) : key(key), prior(prior), l(NULL), r(NULL) {}
} * pItem;

pItem merge( pItem l, pItem r, pItem & t ) {
  if( !l || !r ) {
      t = l ? l : r;
  } else {
      if( l->prior > r->prior ) {
          merge(l->r, r, l->r);
          t = l;
      } else {
          merge(l, r->l, r->l);
          t = r;
      }
  }
}

pItem split( pItem t, int key, pItem &l, pItem &r ) {
  if( !t ) {
      l = r = NULL;
  } else {
      if( key <= t->key ) {
          split(t->l, key, l, t->l);
          r = t;
      } else {
          split(t->r, key, t->r, r);
          l = t;
      }
  }
}

void insert( pItem &p, pItem t ) {
  if(!p) {
      p = t;
  } else {
      if( t->prior >= p->prior ) {
          split( p, t->key, t->l, t->r );
          p = t;
      } else {
          if( t->key < p->key ) {
              insert( p->l, t );
          } else {
              insert( p->r, t );
          }
      }
  }
}

int findMin( pItem p ) {
  if( !p ) {
      return -1;
  } else {
      if( p->l == NULL ) {
          return p->key;
      } else {
          return findMin( p->l );
      }
  }
}

int main() {
  //freopen("in", "r", stdin);
  pItem root = NULL;
  int n, t, y=0;
  char ch;
  scanf("%d", &n);
  srand(123*n);
  for(int i=0; i<n; i++) {
      scanf("\n%c %d", &ch, &t);
      if( ch == '+' ) {
          insert(root, new item((t + y)%mod, rand() % mod));
          y = 0;
      } else {
          pItem r=NULL, l=NULL;
          split(root, t, l, r);
          printf("%d\n", y = findMin(r));
          merge(l, r, root);
      }
  }
  return 0;
} 
