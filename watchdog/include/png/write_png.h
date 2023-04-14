/*
 * (c) 2007 Gerald Roehrbein
 *
 * This defines a set of primitives to create PNG images
 * for statistics.
 *
 * Most of this code was found in pngwriter project written
 * by
 * Paul Blackburn
 *      individual61@users.sourceforge.net
 *
 *  Migrated to C language and modified by Gerald Roehrbein.
 *  I need only a subset of pngwriter and I've written my own
 *  primitives for drawing!
 *
 *  This library is able to deal with multithreading!
 *
 *
 */


#ifndef __WRITE_PNG_H
#define __WRITE_PNG_H

#include <png.h>

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

#define PNG_BYTES_TO_CHECK (4)
#define PNGWRITER_DEFAULT_COMPRESSION (6)

#ifndef NO_PNG_FREETEXT
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

struct color{
          int red;
          int green;
          int blue;
};

#define  BLACK 0
#define  BLUE  1
#define  GREEN 2
#define  CYAN  3
#define  RED   4
#define  MAGENTA 5
#define  BROWN 6
#define  LIGHTGREY 7
#define  DARKGREY 8
#define  LIGHTBLUE 9
#define  LIGHTGREEN 10
#define  LIGHTCYAN 11
#define  LIGHTRED 12
#define  LIGHTMAGENTA 13
#define  YELLOW 14
#define  WHITE 15

extern struct color color[16];


struct pngwriter
{
   int socket;       /*
                      * If socket is not -1 than we will write the image to the socket!
                      */

   char * filename;  /*
                      *  If this pointer is not NULL the we will write the image to
                      *  this file!
                      */

   char * author;
   char * description;
   char * title;
   char * software;


   int height;
   int width;

   double x_scale;
   double y_scale;

   int  backgroundcolour;
   int bit_depth;
   int rowbytes;
   int colortype;
   int compressionlevel;
   unsigned char * * graph;
   double filegamma;
   double screengamma;
#ifndef NO_PNG_FREETEXT
   FT_Bitmap * bitmap;
   char * face_path;
   int fontsize;

#endif

} pngwriter;


   /* The algorithms HSVtoRGB and RGBtoHSV were found at http://www.cs.rit.edu/~ncs/
    * which is a page that belongs to Nan C. Schaller, though
    * these algorithms appear to be the work of Eugene Vishnevsky.
    *
    */

   void HSVtoRGB(  double *r, double *g, double *b, double h, double s, double v );
   void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );



   /* void plotHSV(( struct pngwriter )*p,int x, int y, double hue, double saturation, double value); */

   void plotHSV(struct pngwriter*,int x, int y, int hue, int saturation, int value);
   /*
   void RGBtoHSV( (struct pngwriter ), float r, float g, float b, float *h, float *s, float *v );
   void HSVtoRGB( (struct pngwriter ), double *r, double *g, double *b, double h, double s, double v );
   */


/* Initialize PNG image. Allocates memory  and initializes image 
 * If you want to write to a file set socket == -1
 * If you want to write to a socket than you have to use httpcomm from the Watchdog project
 * because its htmlprintf function is used to write to a socket!
 */

void write_png_init(struct pngwriter * ,int x, int y, int backgroundcolour, char * filename, int socket);

/* Initialize some comments in the PNG file
 * Required to call if you want to change image comments!
 * Attention: Use constants as parameters or free the variable as required!
 *
 */
void write_png_set_info(struct pngwriter *,char * title, char * author, char * description, char * software);

/*
 * General plot function !
 * This plots a point in the image!
 *
 */
void write_png_plot(struct pngwriter *,int x, int y, int red, int green, int blue);


void write_png_line(struct pngwriter *png,int xfrom, int yfrom, int xto, int yto, int red, int green,int  blue);
void write_png_ellipse(struct pngwriter *png, int mx, int my, int a, int b, int red, int green, int blue );


/*
 *
 * Write the image to disk
 * This function  writes the image to disk or to a given open socket!
 *
 */


void write_png_finish(struct pngwriter *);



/*
 *
 *  Demo to creates a Kaleidoscope image
 *
 */

void write_png_caleidoscope(struct pngwriter *png);
void write_png_plot_text( struct pngwriter *, int x_start, int y_start, double angle, char * text, int red, int green, int blue);
void write_png_draw_bitmap( struct pngwriter *, FT_Bitmap *,  int x, int y, int char_height, int red, int green, int blue);


#endif
