void pngwriter::plot_text( char * face_path, int fontsize, int x_start, int y_start, double angle, char * text, double red, double green, double blue)
{
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
   if (error) { std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Could not init Library."<< std::endl; return;}

   /* Initialize FT face object */
   error = FT_New_Face( library,face_path,0,&face );
   if ( error == FT_Err_Unknown_File_Format ) { std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Font was opened, but type not supported."<< std::endl; return; } else if (error){ std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Could not find or load font file."<< std::endl; return; }

   /* Set the Char size */
   error = FT_Set_Char_Size( face,          /* handle to face object           */
			     0,             /* char_width in 1/64th of points  */
			     fontsize*64,   /* char_height in 1/64th of points */
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

	if (error) { std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Set char size error." << std::endl; return;};

	/* Retrieve glyph index from character code */
	glyph_index = FT_Get_Char_Index( face, text[n] );

	/* Load glyph image into the slot (erase previous one) */
	error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
	if (error) { std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Could not load glyph (in loop). (FreeType error "<< std::hex << error <<")." << std::endl; return;}

	/* Convert to an anti-aliased bitmap */
	//	error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
	error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
	if (error) { std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Render glyph error." << std::endl; return;}

	/* Now, draw to our target surface */
	my_draw_bitmap( &slot->bitmap,
			slot->bitmap_left,
			y_start + slot->bitmap_top,
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
}
