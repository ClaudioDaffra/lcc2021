
#include "gc.h"

static int gcPopButNotFree=0 ;  // flag elimina ma non dealloca

struct gcNode_s *gc_root ;

// ................................................................... new
struct gcNode_s *gcNewNode(void* item) 
{ 
    struct gcNode_s *temp =  (struct gcNode_s *)malloc(sizeof(struct gcNode_s )); 

    temp->pointer   = item; 
    temp->left      = NULL;
    temp->right     = NULL; 
    temp->type      = gc_generic;
    temp->destructor= NULL;

    return temp; 
} 
  
// ................................................................... print inorder
static int gcLevel=0;

void gcPrint_(struct gcNode_s *root) 
{ 
    gcLevel=0;
    if (root != NULL) 
    {
        gcPrint_(root->left); 

        printf ( "\n%10p[%10p]\n----LEFT/%10p ... RIGHT/%10p type(%03d)."
            ,(void*)root
            ,(void*)root->pointer
            ,(void*)root->left
            ,(void*)root->right 
            ,(int)root->type
        ) ;

        gcPrint_(root->right);
    } 
 
} 

void gcWPrint_(struct gcNode_s *root) 
{ 
    gcLevel=0;
    if (root != NULL) 
    {
        gcWPrint_(root->left); 

        wprintf ( L"\n%10p[%10p]\n----LEFT/%10p ... RIGHT/%10p type(%03d)."
            ,(void*)root
            ,(void*)root->pointer
            ,(void*)root->left
            ,(void*)root->right 
            ,(int)root->type
        ) ;

        gcWPrint_(root->right);
    } 
 
} 

// ................................................................... insert  
 
struct gcNode_s * gcPushBack(struct gcNode_s * node, void* pointer) 
{ 
    /* If the tree is empty, return a new node */
    if (node == NULL) return gcNewNode(pointer); 
  
    /* Otherwise, recur down the tree */
    if (pointer < node->pointer) 
        node->left  = gcPushBack(node->left, pointer); 
    else
        node->right = gcPushBack(node->right, pointer); 
  
    /* return the (unchanged) node pointer */
    return node; 
} 

// ................................................................... gcPopBack_

void gcPopBack_(struct gcNode_s * node,void *pointer)  
{  
    if (node == NULL) return;  

    gcPopBack_(node->left,pointer);  

    if ( node->pointer == pointer ) 
    {
        if ( gcPopButNotFree==0 )
            free(node->pointer);

        node->pointer=NULL;
    }

    gcPopBack_(node->right,pointer);  
} 

// ................................................................... gcStop_

void gcStop_(struct gcNode_s * node)  
{  
    if (node == NULL) return;  

    /* first delete both subtrees */
    gcStop_(node->left);  
    gcStop_(node->right);  
      
    /* then pointer & delete the node */

    if ( node->pointer != NULL ) 
    {
        free(node->pointer);
        node->pointer=NULL;
    }
 
    if ( node != NULL ) 
    {
        free(node);
        node=NULL;  
    }

}  

// ................................................................... push(hook)
void* gcPush_ ( struct gcNode_s *gc_root2,void*P )
{
    gc_root=gcPushBack(gc_root2,P);
    
    assert ( gc_root != NULL ) ;
    
    return P ;
}
// ................................................................... pop
void gcPop_ ( struct gcNode_s *gc_root,void*P )
{
    gcPopButNotFree=1;
    
    gcPopBack_(gc_root,P);
    
    gcPopButNotFree=0;
}

// ................................................................... wrapper realloc
void* gcRealloc_( void* P , size_t N )
{
	if (P==NULL) return gcMalloc(N);
	
    void* old=P;
    P = (void*) realloc ( P,N ) ;

    gcPop(old);
    gcPush(P) ;
    
    assert ( P != NULL ) ; 
    
    return P ;
}

/**/

// ................................................................... wrapper strdup
char* gcStrDup( char *s )
{
    if ( s == NULL ) return (char*)NULL ;
    #undef strdup
    return (char*)gcPush( strdup(s) ) ;
    #define strdup gcStrDup
}

// ................................................................... wrapper wcsdup
wchar_t* gcWcsDup( wchar_t *s)
{
    if ( s == NULL ) return (wchar_t*)NULL ;
    #undef wcsdup
    return (wchar_t*)gcPush( wcsdup(s) ) ;
    #define wcs gcWcsDup
}

// ................................................................... intDup

int* gcIntDup(int val)
{
    int* p=(int*)  gcMalloc ( sizeof(int) );
    *p=val;
    return p ;
}

// ................................................................... doubleDup
double* gcDoubleDup ( double val )  
{
    double* p = (double*) gcMalloc ( sizeof(double) );
    *p=val;
    return p ;
}

/**/

// ......................................... [] compare function

int gcCompareInt(const void* a, const void* b)
{
  int va = *(const int*) a;
  int vb = *(const int*) b;
  return (va > vb) - (va < vb);
}

int gcCompareFloat (const void * a, const void * b)
{
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa > fb) - (fa < fb);
}

int gcCompareDouble (const void * a, const void * b)
{
  double fa = *(const double*) a;
  double fb = *(const double*) b;
  return (fa > fb) - (fa < fb);
}

int gcCompareFloatAsInt (const void * a, const void * b)
{
  float _fa = *(const float*) a;
  float _fb = *(const float*) b;
  int fa = (int)_fa;
  int fb = (int)_fb;
  return (fa > fb) - (fa < fb);
}
int gcCompareDoubleAsInt (const void * a, const void * b)
{
  double _fa = *(const double*) a;
  double _fb = *(const double*) b;
  int fa = (int)_fa;
  int fb = (int)_fb;  
  return (fa > fb) - (fa < fb);
}
int gcCompareStrC ( const void * a, const void * b ) 
{
    const char **pa = (const char **)a;
    const char **pb = (const char **)b;
    return strcmp(*pa, *pb);   
    
}
int gcCompareWStrC ( const void * a, const void * b ) 
{
    const wchar_t **pa = (const wchar_t **)a;
    const wchar_t **pb = (const wchar_t **)b;
    return wcscmp(*pa, *pb);  
}

int gcComparepStrC ( const void * a, const void * b ) 
{
    const char *pa = (const char *)a;
    const char *pb = (const char *)b;
    return strcmp(pa, pb);   
    
}
int gcComparepWStrC ( const void * a, const void * b ) 
{
    const wchar_t *pa = (const wchar_t *)a;
    const wchar_t *pb = (const wchar_t *)b;
    return wcscmp(pa, pb);  
}

/**/


