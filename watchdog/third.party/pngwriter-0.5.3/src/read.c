int pngwriter::read(int x, int y, int colour)
{
   int temp1,temp2;

   if((colour !=1)&&(colour !=2)&&(colour !=3))
     {
	std::cerr << " PNGwriter::read - WARNING **: Invalid argument: should be 1, 2 or 3, is " << colour << std::endl;
	return 0;
     }

   if( ( x>0 ) && ( x <= (this->width_) ) && ( y>0 ) && ( y <= (this->height_) ) )
     {

	if(bit_depth_ == 16)
	  {
	     temp2=6*(x-1);
	     if(colour == 1)
	       {
		  temp1 = (graph_[(height_-y)][temp2])*256 + graph_[height_-y][temp2+1];
		  return temp1;
	       }

	     if(colour == 2)
	       {
		  temp1 = (graph_[height_-y][temp2+2])*256 + graph_[height_-y][temp2+3];
		  return temp1;
	       }

	     if(colour == 3)
	       {
		  temp1 = (graph_[height_-y][temp2+4])*256 + graph_[height_-y][temp2+5];
		  return temp1;
	       }
	  }

	if(bit_depth_ == 8)
	  {
	     temp2=3*(x-1);
	     if(colour == 1)
	       {
		  temp1 = graph_[height_-y][temp2];
		  return temp1*256;
	       }

	     if(colour == 2)
	       {
		  temp1 =  graph_[height_-y][temp2+1];
		  return temp1*256;
	       }

	     if(colour == 3)
	       {
		  temp1 =  graph_[height_-y][temp2+2];
		  return temp1*256;
	       }
	  }
     }
   else
     {
	return 0;
     }

   std::cerr << " PNGwriter::read - WARNING **: Returning 0 because of bitdepth/colour type mismatch."<< std::endl;
   return 0;
}
