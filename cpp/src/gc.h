#ifndef gc_
#define gc_

#ifdef __linux__
#define _GNU_SOURCE 
#endif

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <wchar.h>
#include <assert.h>
#include <stdint.h>

// ................................................................... enum pointer type

typedef enum gc_pointer_e
{
    gc_generic  ,
    gc_file

} gc_pointer_t ;

// ................................................................... main struct 
 
typedef struct gcNode_s 
{ 
    void*               pointer             ; 
    struct gcNode_s *   left                ; 
    struct gcNode_s *   right               ;
    gc_pointer_t        type                ;
    void (*destructor)(struct gcNode_s *p)  ; 
       
} gcNode_t ; 
 
struct gcNode_s *   gcNewNode       (void*  item                                        ) ;

void                gcPrint_        (struct gcNode_s *  root                            ) ;
void                gcWPrint_       (struct gcNode_s *  root                            ) ;

struct gcNode_s *   gcPushBack      (struct gcNode_s *  node    ,   void* pointer       ) ;
void                gcPopBack_      (struct gcNode_s *  node    ,   void *pointer       ) ;
void                gcStop_         (struct gcNode_s *  node                            ) ;
void*               gcPush_         (struct gcNode_s *  gc_root ,   void*P              ) ;
void                gcPop_          (struct gcNode_s *  gc_root ,   void*P              ) ;
void*               gcRealloc_      (void*              P       ,   size_t dim          ) ;


extern struct gcNode_s *gc_root ;

// push : insert node and pointer
// free : delete node and pointer
// pop  : delete node -----------

#define gcPrint()               gcPrint_(gc_root)
#define gcWPrint()              gcWPrint_(gc_root)

#define gcStart()               gc_root = NULL;

#if defined(__MINGW32__) || defined(__MINGW64__)
#define fcloseall _fcloseall
#define flushall _flushall
#endif 

//extern void cdEraseTemp(void) ;

#define gcStop()\
do {\
    gcStop_(gc_root);\
    /*fcloseall();*/\
    /*cdEraseTemp()*/;\
}while(0);
 
#define gcPush(P)               gcPush_(gc_root,(P))
#define gcPop(P)                gcPop_(gc_root,(P))

#define gcFree(P)               gcPopBack_(gc_root,(P))  
#define gcMalloc(...)           gcPush( malloc(  __VA_ARGS__ ) ) 
#define gcRealloc( P, ... )     gcRealloc_((P),__VA_ARGS__)
#define gcCalloc(N,...)         gcPush( calloc(  N,__VA_ARGS__ ) ) 

/**/

// ................................................................... wrapper strdup
char*    gcStrDup( char *s ) ;
#define strdup gcStrDup

// ................................................................... wrapper wcsdup
wchar_t* gcWcsDup( wchar_t *s ) ;
#define wcsdup gcWcsDup

// ................................................................... int dup
int* gcIntDup(int val) ;

// ................................................................... double dup
double* gcDoubleDup ( double val ) ;

/**/

// ......................................... [] compare functioon

int gcCompareInt        ( const void * a, const void * b ) ;

int gcCompareFloat      ( const void * a, const void * b ) ;

int gcCompareDouble     ( const void * a, const void * b ) ;

int gcCompareFloatAsInt ( const void * a, const void * b ) ;

int gcCompareDoubleAsInt( const void * a, const void * b ) ;

int gcCompareStrC       ( const void * a, const void * b ) ;

int gcCompareWStrC      ( const void * a, const void * b ) ;

int gcComparepStrC      ( const void * a, const void * b ) ;

int gcComparepWStrC     ( const void * a, const void * b ) ;

/**/

#endif  /* gc_ */



/**/


