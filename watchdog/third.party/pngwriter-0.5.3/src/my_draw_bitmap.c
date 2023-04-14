void pngwriter::my_draw_bitmap( FT_Bitmap * bitmap, int x, int y, double red, double green, double blue)
{
   double temp;
   for(int j=1; j<bitmap->rows+1; j++)
     {
	for(int i=1; i< bitmap->width + 1; i++)
	  {
	     temp = (double)(bitmap->buffer[(j-1)*bitmap->width + (i-1)] )/255.0;

	     if(temp)
	       {
		  this->plot(x + i,
			     y  - j,
			     temp*red + (1-temp)*(this->dread(x+i,y-j,1)),
			     temp*green + (1-temp)*(this->dread(x+i,y-j,2)),
			     temp*blue + (1-temp)*(this->dread(x+i,y-j,3))
			     );
	       }
	  }
     }
}

