
#include <write_png.h>
#include <statistics.h>

#define BORDER 20

void statistics_grid(struct pngwriter *png, struct color *c)
{

  int x,y;
  int mid_x=png->width/2;
  int mid_y=png->height/2;

  x=mid_x;
  y=mid_y;

  printf("(%02d,%02d,%02d,%02d)\n",x,y,mid_x,mid_y);

  for (;x<=png->width-BORDER; x+=(10*png->x_scale))
  {
    write_png_line(png,x-mid_x +(20*png->x_scale) ,BORDER,x-mid_x +(20*png->x_scale),png->height-BORDER,c->red,c->green,65535);
    write_png_line(png,x,BORDER,x,png->height-BORDER,c->red,c->green,c->blue);
  }


  for (;y<=png->height-BORDER; y+=(10*png->y_scale))
  {
    write_png_line(png,BORDER,y-mid_y+(20*png->y_scale),png->width-BORDER,y-mid_y+(20*png->y_scale),c->red,c->green,65535);
    write_png_line(png,BORDER,y,png->width-BORDER,y,c->red,c->green,c->blue);
  }

  /* Center cross */

    write_png_line(png,mid_x,0,mid_x,png->height,65535,c->green,0);
    write_png_line(png,0,mid_y,png->width,mid_y,65535,c->green,0);


}
