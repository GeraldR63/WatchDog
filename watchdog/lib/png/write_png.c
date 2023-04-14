/*
 * (c) 2007 Gerald Roehrbein
 * Most ideas of this code are from the pngwriter project written by Paul Blackburn
 *
 * My job was:
 *
 * migration from C++ to C
 * add functions to write an image directly to HTTP port
 * changed orientation from 0,0 beeing in the lower left to the upper right as usual
 * I've also added a function to write a line and an ellipses implemented  by myself.
 *
 * I've also added some features to avoid kernel dumps. For example it is not possible
 * to plot a pixel outside the border of the image. 
 *
 *
 */

#include <write_png.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define PNG_WRITER_FONT "/etc/watchdog/fonts/FreeMonoBold.ttf"


/*
 * Some default colors!
 */

struct color color[16]=
{
{0x00,0x00,0x00}, /* BLACK */
{0x00,0x00,0xc0}, /* BLUE  */
{0x00,0xc0,0x00}, /* GREEN */
{0x00,0xc0,0xc0}, /* CYAN  */
{0xc0,0x00,0x00}, /* RED   */
{0xc0,0x00,0xc0}, /* MAGENTA */
{0xc0,0xc0,0x00}, /* BROWN */
{0xc0,0xc0,0xc0}, /* LIGHTGREY */
{0x30,0x30,0x30}, /* DARKGREY */
{0x00,0x00,0xFF}, /* LIGHTBLUE */
{0x00,0xFF,0x00}, /* LIGHTGREEN */
{0x00,0xFF,0xFF}, /* LIGHTCYAN */
{0xFF,0x00,0x00}, /* LIGHTRED */
{0xFF,0x00,0xFF}, /* LIGHMAGENTA */
{0xFF,0xFF,0x00}, /* YELLOW */
{0xFF,0xFF,0xFF}  /* WHITE */
};

#ifdef WATCHDOG

#include <httpcomm.h>
#include <logwriter.h>


/* This is a user define write function for LIBPNG
 * This function will write to an open socket! 
 */
void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
 voidp write_io_ptr = png_get_io_ptr(png_ptr);
 struct pngwriter * png=write_io_ptr;

htmlprintf( png->socket , length, data);
}
void user_flush_data(png_structp png_ptr)
{
}




#endif

 void HSVtoRGB( double *r, double *g, double *b, double h, double s, double v )
 {
    int i;
    double f, p, q, t;

    h = h*360.0;
    if( s == 0 )
      {
 	*r = *g = *b = v;
 	return;
      }

    h /= 60;                        /* sector 0 to 5 */
    i = (int)floor( h );
    f = h - i;                      /* factorial part of h */
    p = v * ( 1 - s );
    q = v * ( 1 - s * f );
    t = v * ( 1 - s * ( 1 - f ) );

    switch( i )
      {
       case 0:
 	*r = v;
 	*g = t;
 	*b = p;
 	break;
       case 1:
 	*r = q;
 	*g = v;
 	*b = p;
 	break;
       case 2:
 	*r = p;
 	*g = v;
 	*b = t;
 	break;
       case 3:
 	*r = p;
 	*g = q;
 	*b = v;
 	break;
       case 4:
 	*r = t;
 	*g = p;
 	*b = v;
 	break;
       default:                
 	*r = v;
 	*g = p;
 	*b = q;
 	break;
      }
 }

 void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
 {

    float min=0.0; /* These values are not used. */
    float max=1.0;
    float delta;

    if( (r>=g)&&(r>=b) )
      {
 	max = r;
      }
    if( (g>=r)&&(g>=b) )
      {
 	max = g;
      }
    if( (b>=g)&&(b>=r) )
      {
 	max = b;
      }

    if( (r<=g)&&(r<=b) )
      {
 	min = r;
      }
    if( (g<=r)&&(g<=b) )
      {
 	min = g;
      }
    if( (b<=g)&&(b<=r) )
      {
 	min = b;
      }

    *v = max;                               

    delta = max - min;

    if( max != 0 )
      *s = delta / max;               
    else
      {

 	r = g = b = 0;               
 	*s = 0;
 	*h = -1;
 	return;
      }

    if( r == max )
      *h = ( g - b ) / delta;         
    else if( g == max )
      *h = 2 + ( b - r ) / delta;    
    else
      *h = 4 + ( r - g ) / delta;    

    *h *= 60;                         
    if( *h < 0 )
      *h += 360;

 }

 /*
  *
  */

 void plotHSV2(struct pngwriter * png,int x, int y, double hue, double saturation, double value)
 {
    double red,green,blue;
    double *redp;
    double *greenp;
    double *bluep;

    redp = &red;
    greenp = &green;
    bluep = &blue;

    HSVtoRGB(redp,greenp,bluep,hue,saturation,value);
    write_png_plot(png,x,y,red,green,blue);
 }

 void plotHSV(struct pngwriter* png,int x, int y, int hue, int saturation, int value)
 {
    plotHSV2(png, x, y, (double)hue/65535.0, (double)saturation/65535.0,  (double)value/65535.0);
 }

void write_png_init(struct pngwriter * png,int x, int y, int backgroundcolour, char * filename, int socket)
{
   int kkkk;
   int tempindex;
   int hhh,vhhh;

   png->width = x;
   png->height = y;

   png->x_scale=1.0;
   png->y_scale=1.0;

   png->backgroundcolour = backgroundcolour;
   png->compressionlevel = -2;
   png->filegamma = 0.6;


   png->author="write_png.c Author: Paul Blackburn migrated to C by Gerald Roehrbein ";
   png->description="http://www.OraForecast.com/";
   png->software="Watchdog. A HA monitoring and performance forecast utility";
   png->title=filename;
   png->filename=filename;  /*  If we have a filename and socket == -1 than we will write to a file !*/ 
   png->socket=socket;      /*  If socket is set to a value <> -1 than everything will be written to a socket! */


   png->face_path=PNG_WRITER_FONT;
   png->fontsize=24;

   if((png->width<0) | (png->height<0))
     {

	   png->height = 1;
	   png->width = 1;
     }

   if(png->backgroundcolour >65535)
     {

	   png->backgroundcolour = 65535;
     }

   if(png->backgroundcolour <0)
     {
	   png->backgroundcolour = 0;
     }


   png->bit_depth = 16; /* Default bit depth for new images */
   png->colortype=2;
   png->screengamma = 2.2;

   png->graph = (png_bytepp)malloc(png->height * sizeof(png_bytep));
   if(png->graph == NULL)
     {
#ifdef WATCHDOG
	   syslogprintf(__FILE__,__LINE__,"watchdog",2," PNGwriter::pngwriter - ERROR **:  Not able to allocate memory for image.\n");
#else
	   fprintf(stderr," PNGwriter::pngwriter - ERROR **:  Not able to allocate memory for image.\n");
#endif
     }

   for (kkkk = 0; kkkk < png->height; kkkk++)
     {
        png->graph[kkkk] = (png_bytep)malloc(6*png->width * sizeof(png_byte));
	if(png->graph[kkkk] == NULL)
	  {
#ifdef WATCHDOG
	     syslogprintf(__FILE__,__LINE__,"watchdog",2, " PNGwriter::pngwriter - ERROR **:  Not able to allocate memory for image.\n");
#else
	     fprintf(stderr, " PNGwriter::pngwriter - ERROR **:  Not able to allocate memory for image.\n");
#endif
	  }
     }

   if(png->graph == NULL)
     {
#ifdef WATCHDOG
	  syslogprintf(__FILE__,__LINE__,"watchdog",2," PNGwriter::pngwriter - ERROR **:  Not able to allocate memory for image.\n");
#else
	  fprintf(stderr," PNGwriter::pngwriter - ERROR **:  Not able to allocate memory for image.\n");

#endif
     }

   for(hhh = 0; hhh<png->width;hhh++)
     {
	for(vhhh = 0; vhhh<png->height;vhhh++)
	  {
	     tempindex=6*hhh;
	     png->graph[vhhh][tempindex] = (char) floor(((double)png->backgroundcolour)/256);
	     png->graph[vhhh][tempindex+1] = (char)(png->backgroundcolour%256);
	     png->graph[vhhh][tempindex+2] = (char) floor(((double)png->backgroundcolour)/256);
	     png->graph[vhhh][tempindex+3] = (char)(png->backgroundcolour%256);
	     png->graph[vhhh][tempindex+4] = (char) floor(((double)png->backgroundcolour)/256);
	     png->graph[vhhh][tempindex+5] = (char)(png->backgroundcolour%256);
	  }
     }

}



void write_png_finish(struct pngwriter * png)
{
   FILE            *fp;
   png_structp     png_ptr;
   png_infop       info_ptr;

   if(png->socket == -1 )  /* If socket is -1 than write to a file! */
   {
   fp = fopen(png->filename, "wb");
   if( fp == NULL)
     {
#ifdef WATCHDOG
	    syslogprintf(__FILE__,__LINE__,"watchdog",1," PNGwriter::close - ERROR **\n");
#else
	    fprintf(stderr," PNGwriter::close - ERROR **\n");
#endif
	    return;
     }
   }

   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   info_ptr = png_create_info_struct(png_ptr);

   if(png->socket == -1 ) /* Init IO system to write to a file! */
   {
     png_init_io(png_ptr, fp);
   }
#ifdef WATCHDOG
   else
   {
      png_set_write_fn(png_ptr, (void *) png, user_write_data, user_flush_data);
   }
#endif



   if(png->compressionlevel != -2)
     {
	png_set_compression_level(png_ptr, png->compressionlevel);
     }
   else
     {
	png_set_compression_level(png_ptr, PNGWRITER_DEFAULT_COMPRESSION);
     }

   png_set_IHDR(png_ptr, info_ptr, png->width, png->height,
		png->bit_depth, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

   if(png->filegamma < 1.0e-1)
     {
	   png->filegamma = 0.7;
     }

   png_set_gAMA(png_ptr, info_ptr, png->filegamma);

   time_t          gmt;
   png_time        mod_time;
   png_text        text_ptr[5];
   time(&gmt);
   png_convert_from_time_t(&mod_time, gmt);
   png_set_tIME(png_ptr, info_ptr, &mod_time);
   text_ptr[0].key = "Title";
   text_ptr[0].text = png->title;
   text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr[1].key = "Author";
   text_ptr[1].text = png->author;
   text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr[2].key = "Description";
   text_ptr[2].text = png->description;
   text_ptr[2].compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr[3].key = "Creation Time";
   text_ptr[3].text = png_convert_to_rfc1123(png_ptr, &mod_time);
   text_ptr[3].compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr[4].key = "Software";
   text_ptr[4].text = png->software;
   text_ptr[4].compression = PNG_TEXT_COMPRESSION_NONE;
   png_set_text(png_ptr, info_ptr, text_ptr, 5);

   png_write_info(png_ptr, info_ptr);
   png_write_image(png_ptr, png->graph);
   png_write_end(png_ptr, info_ptr);
   png_destroy_write_struct(&png_ptr, &info_ptr); 
   /* png_destroy_info_struct(&png_ptr, &info_ptr); */
   int jjj;

   for (jjj = 0; jjj < png->height; jjj++) free(png->graph[jjj]);
   free(png->graph);

   if(png->socket == -1 )  /*  Wrote to a file! Close the file! */
   {
    if (fp) fclose(fp);
   }
}



void write_png_set_info(struct pngwriter * png,char * title, char * author, char * description, char * software)
{

   png->author = author;
   png->description =  description;
   png->software = software;
   png->title = title;

}



void write_png_plot(struct pngwriter * png,int x, int y, int red, int green, int blue)
{
   int tempindex;
   if ((x<1) | (y<1) | (x>png->width-1) | (y>png->height-1)) return;
   
   /* Set color to 0 if color is out of range! Between 0 and 65535 */ 
   red*= (red<65536 && red>-1);
   green*= (green<65536 && green>-1);
   blue*= (blue<65536 && blue>-1);

   if((png->bit_depth == 16))
     {
	     tempindex= 6*x-6;
	     png->graph[y ][tempindex] = (char) floor(((double)red)/256);
	     png->graph[y ][tempindex+1] = (char)(red%256);
	     png->graph[y ][tempindex+2] = (char) floor(((double)green)/256);
	     png->graph[y ][tempindex+3] = (char)(green%256);
	     png->graph[y ][tempindex+4] = (char) floor(((double)blue)/256);
	     png->graph[y ][tempindex+5] = (char)(blue%256);
     }

   if((png->bit_depth == 8))
     {
	     tempindex = 3*x-3;
	     png->graph[y][tempindex] = (char)(floor(((double)red)/257.0));
	     png->graph[y][tempindex+1] = (char)(floor(((double)green)/257.0));
	     png->graph[y][tempindex+2] = (char)(floor(((double)blue)/257.0));
     }
}


void write_png_line(struct pngwriter *png,int x1, int y1, int x2, int y2, int red, int green,int  blue)
{
     int x,y,deltax,deltay;
          deltax=x1-x2;
          deltay=y1-y2;

    /*
     *  printf("(%d,%d,%d,%d)\n",x1,y1,x2,y2);
     *  return;
     */

          if ((deltax==0) && (deltay==0))
           {
                 write_png_plot(png,x1,y1,red,green,blue);
                 return;
          }
          if (((abs(deltay)<abs(deltax)) & (deltax != 0)) | (deltay==0))
                 if (x1<x2)
                 for (x=x1; x<= x2;x++)
                       write_png_plot(png, x,y1+round((double)((x-x1)*deltay/deltax)),red,green,blue);
                 else for (x=x1; x>=x2;x--)
                       write_png_plot(png,x,y1+round((double)((x-x1)*deltay/deltax)),red,green,blue);

          else             if (y1<y2)
                  for (y=y1;y<=y2;y++)
                      write_png_plot(png,x1+round((double)((y-y1)*deltax/deltay)),y,red,green,blue);
                 else for (y=y1;y>=y2;y--)
                      write_png_plot(png,x1+round((double)((y-y1)*deltax/deltay)),y,red,green,blue);
}



void write_png_caleidoscope(struct pngwriter *png)
/*
 * This function creates random lines
 * Call this function a few times to create a caleidoscope
 * 
 * Originally written by Hans Hamers modified for PNG library by Gerald Roehrbein
 *
 */
{

   int red,green,blue;
   int x1,y1,x2,y2,xv1,yv1,xv2,yv2,xa,ya,xb,yb;
   int mitte_x=png->width/2, mitte_y=png->height/2,x_faktor=8, y_faktor=5;

    {
       x1=(random() % mitte_y)+1;
       x2=(random() % mitte_y)+1;
       y1=(random() % x1)+1;
       y2=(random() % x2)+1;


       xv1=(random() % y_faktor -2);
       xv2=(random() % y_faktor -2);
       yv1=(random() % y_faktor -2);
       yv2=(random() % y_faktor -2);


       red=random() % 65535;
       green=random() % 65535;
       blue=random() % 65535;

       xa=(div(x1*x_faktor ,  y_faktor).quot);
       xb=(div(x2*x_faktor ,  y_faktor).quot);
       ya=(div(y1*x_faktor ,  y_faktor).quot);
       yb=(div(y2*x_faktor ,  y_faktor).quot);


       write_png_line(png,(mitte_x+xa),(mitte_y-y1),(mitte_x+xb),(mitte_y-y2),red,green,blue);
       write_png_line(png,(mitte_x-ya),(mitte_y+x1),(mitte_x-yb),(mitte_y+x2),red,green,blue);
       write_png_line(png,(mitte_x-xa),(mitte_y-y1),(mitte_x-xb),(mitte_y-y2),red,green,blue);
       write_png_line(png,(mitte_x-ya),(mitte_y-x1),(mitte_x-yb),(mitte_y-x2),red,green,blue);
       write_png_line(png,(mitte_x-xa),(mitte_y+y1),(mitte_x-xb),(mitte_y+y2),red,green,blue);
       write_png_line(png,(mitte_x+ya),(mitte_y-x1),(mitte_x+yb),(mitte_y-x2),red,green,blue);
       write_png_line(png,(mitte_x+xa),(mitte_y+y1),(mitte_x+xb),(mitte_y+y2),red,green,blue);
       write_png_line(png,(mitte_x+ya),(mitte_y+x1),(mitte_x+yb),(mitte_y+x2),red,green,blue);


       x1=div((x1+xv1) ,  mitte_y).quot;
       y1=div((y1+yv1) ,  mitte_y).quot;
       x2=div((x2+xv2) ,  mitte_y).quot;
       y2=div((y2+yv2) ,  mitte_y).quot;
   }

}

void write_png_ellipse(struct pngwriter *png, int mx, int my, int a, int b, int red, int green, int blue )
/*
 * Writes an ellipse at xm,ym with width a and heightb in color into memory structure located at img
 *
 * Originally written by Gerald Roehrbein in 1990
 * using Turbo Pascal language
 * 
 * Migrated to Java language in 1996
 * Migrated in C language in 2007
 *
 */


{
int x, y;
    float qr1, qr2, dx, dy, da;

    x = 0;
    y = b;
    qr1 =(float) (2.0f * a * a);
    qr2 =(float) (2.0f * b * b);
    dx = 1.0f;
    dy = qr1 * (float)b - 1.0f;
    da = (int) (dy / 2);
     do {
      write_png_plot(png,mx + x, my + y,red,green,blue);
      write_png_plot(png,mx + x, my - y,red,green,blue);
      write_png_plot(png,mx - x, my + y,red,green,blue);
      write_png_plot(png,mx - x, my - y,red,green,blue);
      if (da >= 0.0f) {
        da -=  dx - 1.0f;
        dx +=  qr2;
        x++;
      }
      else {
        da +=  dy - 1.0f;
        dy -=  qr1;
        y--;
      }
    }while (y > 0);
}


void write_png_plot_text( struct pngwriter *png, int x_start, int y_start, double angle, char * text, int red, int green, int blue)
{

#ifndef NO_PNG_FREE_TEXT

   FT_Library  library;
   FT_Face     face;
   FT_Matrix   matrix;      // transformation matrix
   FT_Vector   pen;

   FT_UInt glyph_index;
   FT_Error error;

   FT_Bool use_kerning;
   FT_UInt previous = 0;

   /* Set up transformation Matrix */
   matrix.xx = (FT_Fixed)( cos(angle)*0x10000);   /* It would make more sense to do this (below), but, bizzarely, */
   matrix.xy = (FT_Fixed)(-sin(angle)*0x10000);   /* if one does, FT_Load_Glyph fails consistently.               */
   matrix.yx = (FT_Fixed)( sin(angle)*0x10000);  //   matrix.yx = - matrix.xy;
   matrix.yy = (FT_Fixed)( cos(angle)*0x10000);  //   matrix.yy = matrix.xx;

   /* Place starting coordinates in adequate form. */
   pen.x = x_start*64 ;
   pen.y =   (int)(y_start/64.0);

   /*Count the length of the string */
   int num_chars = strlen(text);

   /* Initialize FT Library object */
   error = FT_Init_FreeType( &library );
   if (error) 
   { 
#ifdef WATCHDOG
     syslogprintf(__FILE__,__LINE__,"watchdog",1, "plot_text - FreeType: Could not init Library."); 
#else
     fprintf(stderr, "plot_text - FreeType: Could not init Library."); 
#endif
     return;
   }

   /* Initialize FT face object */
   error = FT_New_Face( library,png->face_path,0,&face );
   if ( error == FT_Err_Unknown_File_Format ) 
   { 
#ifdef WATCHDOG
      syslogprintf(__FILE__,__LINE__,"watchdog",1, "plot_text - FreeType: Font not supported."); 
#else
      fprintf(stderr, "plot_text - FreeType: Font not supported."); 
#endif
      return; 
   } else if (error)
   { 
#ifdef WATCHDOG
     syslogprintf(__FILE__,__LINE__,"watchdog",1, "plot_text - FreeType: Could not find or load font."); 
#else
     fprintf(stderr, "plot_text - FreeType: Could not find or load font."); 
#endif
     return; 
   }

   /* Set the Char size */
   error = FT_Set_Char_Size( face,          /* handle to face object           */
			     0,             /* char_width in 1/64th of points  */
			     png->fontsize*64,   /* char_height in 1/64th of points */
			     100,           /* horizontal device resolution    */
			     100 );         /* vertical device resolution      */

   /* A way of accesing the glyph directly */
   FT_GlyphSlot  slot = face->glyph;  // a small shortcut


   /* Does the font file support kerning? */
   use_kerning = FT_HAS_KERNING( face );

   int n;
   for ( n = 0; n < num_chars; n++ )
     {
	/* Convert character code to glyph index */
	glyph_index = FT_Get_Char_Index( face, text[n] );

	/* Retrieve kerning distance and move pen position */
	if ( use_kerning && previous&& glyph_index )
	  {
	     FT_Vector  delta;
	     FT_Get_Kerning( face,
			     previous,
			     glyph_index,
			     ft_kerning_default, //FT_KERNING_DEFAULT,
			     &delta );

	     /* Transform this kerning distance into rotated space */
	     pen.x += (int) (((double) delta.x)*cos(angle));
	     pen.y +=  (int) (((double) delta.x)*( sin(angle)));
	  }

	/* Set transform */
	FT_Set_Transform( face, &matrix, &pen );

/*set char size*/

	if (error) { 
#ifdef WATCHDOG
           syslogprintf(__FILE__,__LINE__,"watchdog",1, "plot_text - FreeType: Set char size error.") ;
#else
           fprintf(stderr, "plot_text - FreeType: Set char size stderr.") ;
#endif
           return;
        };

	/* Retrieve glyph index from character code */
	glyph_index = FT_Get_Char_Index( face, text[n] );

	/* Load glyph image into the slot (erase previous one) */
	error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
	if (error) 
        { 
#ifdef WATCHDOG
            syslogprintf(__FILE__,__LINE__,"watchdog",1,"plot_text - FreeType: Could not load glyph (in loop).") ; 
#else
            fprintf(stderr,"plot_text - FreeType: Could not load glyph (in loop).") ; 
#endif
         return;
        }

	/* Convert to an anti-aliased bitmap */
	//	stderr = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
	error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
	if (error) 
        { 
#ifdef WATCHDOG
         syslogprintf(__FILE__,__LINE__,"watchdog",1," plot_text - FreeType: Render glyph stderr.") ; 
#else
         fprintf(stderr," plot_text - FreeType: Render glyph stderr.") ; 
#endif

         return;
        }

	/* Now, draw to our target surface */
	write_png_draw_bitmap(png, 
                        &slot->bitmap,
			slot->bitmap_left,
                        y_start,
			slot->bitmap_top,
			red,
			green,
			blue );

	/* Advance to the next position */
	pen.x += slot->advance.x;
	pen.y += slot->advance.y;

	/* record current glyph index */
	previous = glyph_index;
     }

   /* Free the face and the library objects */
   FT_Done_Face    ( face );
   FT_Done_FreeType( library );
#endif
}


int png_read(struct pngwriter *png, int x, int y, int colour)
{
   int temp1,temp2;

   if((colour !=1)&&(colour !=2)&&(colour !=3))
     {
#ifdef WATCHDOG
	syslogprintf(__FILE__,__LINE__,"watchdog",1,"Color should be 1, 2 or 3, is %d", colour );
#else
	fprintf(stderr,"Color should be 1, 2 or 3, is %d", colour );
#endif
	return 0;
     }

   if( ( x>0 ) && ( x <= (png->width) ) && ( y>0 ) && ( y <= (png->height) ) )
     {

	if(png->bit_depth == 16)
	  {
	     temp2=6*(x-1);
	     if(colour == 1)
	       {
		  temp1 = (png->graph[(png->height-y)][temp2])*256 + png->graph[png->height-y][temp2+1];
		  return temp1;
	       }

	     if(colour == 2)
	       {
		  temp1 = (png->graph[png->height-y][temp2+2])*256 + png->graph[png->height-y][temp2+3];
		  return temp1;
	       }

	     if(colour == 3)
	       {
		  temp1 = (png->graph[png->height-y][temp2+4])*256 + png->graph[png->height-y][temp2+5];
		  return temp1;
	       }
	  }

	if(png->bit_depth == 8)
	  {
	     temp2=3*(x-1);
	     if(colour == 1)
	       {
		  temp1 = png->graph[png->height-y][temp2];
		  return temp1*256;
	       }

	     if(colour == 2)
	       {
		  temp1 =  png->graph[png->height-y][temp2+1];
		  return temp1*256;
	       }

	     if(colour == 3)
	       {
		  temp1 =  png->graph[png->height-y][temp2+2];
		  return temp1*256;
	       }
	  }
     }
   else
     {
	return 0;
     }

#ifdef WATCHDOG
   syslogprintf(__FILE__,__LINE__,"watchdog",1, "Returning 0 because of bitdepth/colour type mismatch.");
#else
  fprintf(stderr,"watchdog",1, "Returning 0 because of bitdepth/colour type mismatch.");
#endif
   return 0;
}

double  dread(struct pngwriter *png, int x, int y, int colour)
{
   return (double)png_read(png,x,y,colour)/65535.0;
}

void write_png_draw_bitmap( struct pngwriter *png, FT_Bitmap *bitmap,  int x, int y, int char_height, int red, int green, int blue)
{

#ifndef NO_PNG_FREETEXT
   double temp;
   int j,i;

   for(j=1; j< bitmap->rows+1; j++)
     {
	for( i=1; i< bitmap->width + 1; i++)
	  {
	     temp = (double)(bitmap->buffer[(j-1)*bitmap->width + (i-1)] )/255.0;

	     if(temp)
	       {
                 
		  write_png_plot(png,x + i,
			     (png->height-(char_height-j))  - (png->height-(y+png->fontsize))    ,
			     temp*red + (1-temp)*(dread(png,x+i,y-j,1)),
			     temp*green + (1-temp)*(dread(png,x+i,y-j,2)),
			     temp*blue + (1-temp)*(dread(png,x+i,y-j,3))
			     );
                 
	       }
	  }
     }
#endif
}

