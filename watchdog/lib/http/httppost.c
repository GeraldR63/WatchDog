#include <httppost.h>
#include <stdlib.h>
#include <string.h>


int num;

char * httpPostSearch(struct httppost * httppost, const char *key)
{
int idx;

  for (idx=0;idx<num;idx++)
  {
  }

return httppost[idx].value;
}

void httpPostPut(struct httppost * httppost, const char *post)
/*
 * This function converts a HTTP POST string to an array of 
 * key=value 
 * pairs in plain ASCII
 *
 */
{
  int n=0;
  char *value;   /* Temp variables */
  char *key;

  for (;;)   /* Iterate over complete POST string */
  {
  if (httppost == NULL )
  {
     httppost=(struct httppost *) malloc( sizeof(struct httppost) );
     if (httppost)
     {    /* Allocate memory to save the original post string! */
          httppost[0].post=(char *) malloc(strlen(post)+1);
          strcpy(httppost[0].post,post);  /* All elements of the array will point to substrings of this structure */
     }
  }
  else
  {
    httppost=(struct httppost *) malloc( sizeof(struct httppost) * (httppost[0].num+1) );
    if (httppost) httppost[httppost[0].num].post=httppost[0].post;   /* All post members poins to first element of array ! */


  }
 
  httppost[httppost[0].num].key=key;         
  httppost[httppost[0].num].value=value;
  httppost[0].num++; /* The first element of the array holds the number of elements in the array */

   break; /* Remove if not required anymore! */ 

  }
}
