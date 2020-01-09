#include "gimage.h"
#include <string.h>

/****
  giflib/gifout.c
  kirk johnson
  may 1990

  RCS $Id: gimagewritegif.c,v 1.4 2001/02/08 01:09:33 gww Exp $

  routines for writing GIF files

  Copyright 1989, 1990 by Kirk L. Johnson (see the included
  file "kljcpyrt.h" for complete copyright information)
  ****/

/* $Header: /home/gww/HtmlPress/gdraw/RCS/gimagewritegif.c,v 1.4 2001/02/08 01:09:33 gww Exp $ */

#include <stdio.h>
/****
  giflib/giflib.h
  kirk johnson
  may 1990

  RCS $Id: gimagewritegif.c,v 1.4 2001/02/08 01:09:33 gww Exp $

  external interface to gifin.c and gifout.c

  Copyright 1989, 1990 by Kirk L. Johnson (see the included
  file "kljcpyrt.h" for complete copyright information)
  ****/
/* $Header: /home/gww/HtmlPress/gdraw/RCS/gimagewritegif.c,v 1.4 2001/02/08 01:09:33 gww Exp $ */

#ifndef _GIFLIB_H_
#define _GIFLIB_H_

/*
 * giflib return codes
 */

#define GIFLIB_SUCCESS       0  /* success */
#define GIFLIB_DONE          1  /* no more images */

#define GIFLIB_ERR_BAD_SD   -1  /* bad screen descriptor */
#define GIFLIB_ERR_BAD_SEP  -2  /* bad image separator */
#define GIFLIB_ERR_BAD_SIG  -3  /* bad signature */
#define GIFLIB_ERR_EOD      -4  /* unexpected end of raster data */
#define GIFLIB_ERR_EOF      -5  /* unexpected end of input stream */
#define GIFLIB_ERR_OUT      -6  /* i/o output error */
#define GIFLIB_ERR_FAO      -7  /* file already open */
#define GIFLIB_ERR_IAO      -8  /* image already open */
#define GIFLIB_ERR_ISO      -9  /* image still open */
#define GIFLIB_ERR_NFO     -10  /* no file open */
#define GIFLIB_ERR_NIO     -11  /* no image open */

/*
 * colormap indices
 */

#define GIFLIB_RED  0
#define GIFLIB_GRN  1
#define GIFLIB_BLU  2

/*
 * typedef BYTE for convenience
 */

typedef unsigned char BYTE;

/*
 * procedures exported by gifin.c
 */

extern int gifin_open_file();
extern int gifin_open_image();
extern int gifin_get_pixel();
extern int gifin_get_row();
extern int gifin_close_image();
extern int gifin_close_file();

/*
 * variables exported by gifin.c
 */

extern int  gifin_rast_width;       /* raster width */
extern int  gifin_rast_height;      /* raster height */
extern BYTE gifin_g_cmap_flag;      /* global colormap flag */
extern int  gifin_g_pixel_bits;     /* bits per pixel, global colormap */
extern int  gifin_g_ncolors;        /* number of colors, global colormap */
extern BYTE gifin_g_cmap[3][256];   /* global colormap */
extern int  gifin_bg_color;         /* background color index */
extern int  gifin_color_bits;       /* bits of color resolution */

extern int  gifin_img_left;         /* image position on raster */
extern int  gifin_img_top;          /* image position on raster */
extern int  gifin_img_width;        /* image width */
extern int  gifin_img_height;       /* image height */
extern BYTE gifin_l_cmap_flag;      /* local colormap flag */
extern int  gifin_l_pixel_bits;     /* bits per pixel, local colormap */
extern int  gifin_l_ncolors;        /* number of colors, local colormap */
extern BYTE gifin_l_cmap[3][256];   /* local colormap */
extern BYTE gifin_interlace_flag;   /* interlace image format flag */

#endif
/****
  giflib/gifint.h
  kirk johnson
  november 1989

  RCS $Id: gimagewritegif.c,v 1.4 2001/02/08 01:09:33 gww Exp $

  #defines, typedefs, and such internal to gifin.c and gifout.c

  Copyright 1989, 1990 by Kirk L. Johnson (see the included
  file "kljcpyrt.h" for complete copyright information)
  ****/
/* $Header: /home/gww/HtmlPress/gdraw/RCS/gimagewritegif.c,v 1.4 2001/02/08 01:09:33 gww Exp $ */

#ifndef _GIFINT_H_
#define _GIFINT_H_


#define GIF_SIG      "GIF89a"
#define GIF_SIG_LEN  6          /* GIF signature length */
#define GIF_SD_SIZE  7          /* GIF screen descriptor size */
#define GIF_ID_SIZE  9          /* GIF image descriptor size */

#define GIF_SEPARATOR   ','     /* GIF image separator */
#define GIF_EXTENSION   '!'     /* GIF extension block marker */
#define GIF_TERMINATOR  ';'     /* GIF terminator */

#define STAB_SIZE  4096         /* string table size */
#define PSTK_SIZE  4096         /* pixel stack size */

#define NULL_CODE  -1           /* string table null code */

#endif
#ifndef _KLJ_COPYRIGHT_
#define _KLJ_COPYRIGHT_

/*
 * RCS $Id: gimagewritegif.c,v 1.4 2001/02/08 01:09:33 gww Exp $
 *
 * Copyright 1989, 1990, 1993 Kirk Lauritz Johnson
 *
 * Permission to use, copy, modify, distribute, and sell this
 * software and its documentation for any purpose is hereby granted
 * without fee, provided that the above copyright notice appear in
 * all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation. The author
 * makes no representations about the suitability of this software
 * for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

static char *Copyright = "Copyright 1989, 1990, 1993 Kirk Lauritz Johnson";

#endif

static int giferror;

/****
 **
 ** local #defines
 **
 ****/

#define HASHSZ     1024
#define HASH(p, e) (((p)&(HASHSZ-1))^(e))

#define PUT_CODE(val)                       \
{                                           \
  work_data |= ((long) (val) << work_bits); \
  work_bits += code_size;                   \
  while (work_bits >= 8)                    \
  {                                         \
    PUT_BYTE(work_data & 0xFF);             \
    work_data >>= 8;                        \
    work_bits  -= 8;                        \
  }                                         \
}

#define PUT_BYTE(val)                 \
{                                     \
  buf[buf_idx++] = (val);             \
  if (buf_idx == 255)                 \
  {                                   \
    write_data_block(255, buf, outs); \
    buf_idx = 0;                      \
  }                                   \
}


/****
 **
 ** local variables
 **
 ****/

static int  cmap_bits();
static int  root_bits();
static void put_clr_code();
static void write_data_block();
static void reset_string_out();
static void add_string_out();
static int  find_string_out();
/*static void gifout_fatal();*/


static BYTE file_open  = 0;     /* status flags */
static BYTE image_open = 0;

static int rast_width;          /* raster width */
static int rast_height;         /* raster height */
static int ncolors;             /* number of colors */
static int img_width;           /* image width */
static int img_height;          /* image height */

static FILE *outs;              /* output file */

static int root_size;           /* root code size */
static int clr_code;            /* clear code */
static int eoi_code;            /* end of info code */
static int code_size;           /* current code size */
static int code_mask;           /* current code mask */
static int old_code;            /* previous code */

static long work_data;          /* working bits */
static int  work_bits;          /* working bit count */

static BYTE buf[256];           /* byte buffer */
static int  buf_idx;            /* buffer index */

static int table_size;          /* string table size */
static int htable[HASHSZ];
static int *prefix;
static int *extnsn;
static int *next;
/*static int prefix[STAB_SIZE];*/
/*static int extnsn[STAB_SIZE];*/
/*static int next[STAB_SIZE];*/


/****
 **
 ** exported procedures
 **
 ****/

/*
 * open a GIF file for writing on stream s
 */
static int gifout_open_file(s, wdth, hght, sz, cmap, bg)
     FILE *s;
     int   wdth;                /* raster width (in pixels) */
     int   hght;                /* raster height (in pixels) */
     int   sz;                  /* number of colors */
     BYTE  cmap[3][256];        /* global colormap */
     int   bg;                  /* background color index */
{
  int i;
  int pixel_bits;

  /* make sure there isn't already a file open */
  if (file_open)
    return GIFLIB_ERR_FAO;

  /* remember that we've got this file open */
  file_open   = 1;
  outs        = s;
  rast_width  = wdth;
  rast_height = hght;

  /* write GIF signature */
  if (fwrite(GIF_SIG, sizeof(char), GIF_SIG_LEN, outs) != GIF_SIG_LEN)
    return GIFLIB_ERR_OUT;

  /* write screen descriptor */
  pixel_bits = cmap_bits(sz);
  ncolors    = 1 << pixel_bits;

  buf[0] = (wdth & 0x00FF);
  buf[1] = (wdth & 0xFF00) >> 8;
  buf[2] = (hght & 0x00FF);
  buf[3] = (hght & 0xFF00) >> 8;
  buf[4] = (pixel_bits - 1) | 0x80;
  buf[5] = bg;
  buf[6] = 0;

  if (fwrite(buf, sizeof(char), GIF_SD_SIZE, outs) != GIF_SD_SIZE)
    return GIFLIB_ERR_OUT;

  /* write (global) color map */
  for (i=0; i<sz; i++)
  {
    buf[GIFLIB_RED] = cmap[GIFLIB_RED][i];
    buf[GIFLIB_GRN] = cmap[GIFLIB_GRN][i];
    buf[GIFLIB_BLU] = cmap[GIFLIB_BLU][i];

    if (fwrite(buf, sizeof(BYTE), 3, outs) != 3)
      return GIFLIB_ERR_OUT;
  }

  for (i=sz; i<ncolors; i++)
  {
    buf[GIFLIB_RED] = 0;
    buf[GIFLIB_GRN] = 0;
    buf[GIFLIB_BLU] = 0;

    if (fwrite(buf, sizeof(BYTE), 3, outs) != 3)
      return GIFLIB_ERR_OUT;
  }

  /* done! */
  return GIFLIB_SUCCESS;
}


/*
 * open a new GIF image for writing in the current GIF file
 */
static int gifout_open_image(left, top, wdth, hght, progressive)
     int left;                  /* column index for left edge */
     int top;                   /* row index for top edge */
     int wdth;                  /* image width (in pixels) */
     int hght;                  /* image height (in pixels) */
     int progressive;		/* save as a progressive gif */
{
  /* make sure there's a file open */
  if (!file_open)
    return GIFLIB_ERR_NFO;

  /* make sure there isn't already an image open */
  if (image_open)
    return GIFLIB_ERR_IAO;

  /* remember that we've got this image open */
  image_open = 1;
  img_width  = wdth;
  img_height = hght;

  /* write image separator */
  putc(GIF_SEPARATOR, outs);

  /* write image descriptor */
  buf[0] = (left & 0x00FF);
  buf[1] = (left & 0xFF00) >> 8;
  buf[2] = (top  & 0x00FF);
  buf[3] = (top  & 0xFF00) >> 8;
  buf[4] = (wdth & 0x00FF);
  buf[5] = (wdth & 0xFF00) >> 8;
  buf[6] = (hght & 0x00FF);
  buf[7] = (hght & 0xFF00) >> 8;
  /* I never write out a local colour map, if I did depth info would go here */
  /* as well as later on */
  buf[8] = progressive?0x40:0;

  if (fwrite(buf, sizeof(BYTE), GIF_ID_SIZE, outs) != GIF_ID_SIZE)
    return GIFLIB_ERR_OUT;

  /* initialize raster data stream */
  root_size = root_bits(ncolors);
  putc(root_size, outs);

  clr_code  = 1 << root_size;
  eoi_code  = clr_code + 1;
  code_size = root_size + 1;
  code_mask = (1 << code_size) - 1;
  old_code  = NULL_CODE;

  work_bits = 0;
  work_data = 0;

  buf_idx = 0;

  /* initialize string table */
  reset_string_out();

  /* output initial clear code */
  put_clr_code();

  /* done! */
  return GIFLIB_SUCCESS;
}

#if 0
/*
 * write a pixel into the current image
 */
static void gifout_put_pixel(val)
     int val;                   /* pixel color index */
{
  int idx;

  /* see if string is in table already */
  idx = find_string_out(old_code, val);

  if (idx != NULL_CODE)
  {
    /* found a match */
    old_code = idx;
  }
  else
  {
    /* no match */
    PUT_CODE(old_code);
    add_string_out(old_code, val);
    old_code = val;

    /* check for full string table */
    if (table_size == STAB_SIZE)
    {
      /* output remaining code */
      PUT_CODE(old_code);

      /* reset encoder */
      put_clr_code();
    }
  }
}
#endif

/*
 * write a row of pixels into the current image
 */
static void gifout_put_row(row)
     int *row;                  /* array of size img_width */
{
  int col;
  int idx;

  for (col=0; col<img_width; col++)
  {
    /* see if string is in table already */
    idx = find_string_out(old_code, row[col]);

    if (idx != NULL_CODE)
    {
      /* found a match */
      old_code = idx;
    }
    else
    {
      /* no match */
      PUT_CODE(old_code);
      add_string_out(old_code, row[col]);
      old_code = row[col];

      /* check for full string table */
      if (table_size == STAB_SIZE)
      {
        /* output remaining code */
        PUT_CODE(old_code);

        /* reset encoder */
        put_clr_code();
      }
    }
  }
}


/*
 * close an open GIF image
 */
static int gifout_close_image()
{
  /* make sure there's an image open */
  if (!image_open)
    return GIFLIB_ERR_NIO;

  /* flush any remaining code */
  if (old_code != NULL_CODE)
    PUT_CODE(old_code);

  /* output end of info code */
  PUT_CODE(eoi_code);

  /* flush any extra bits */
  while (work_bits > 0)
  {
    PUT_BYTE(work_data & 0xFF);
    work_data >>= 8;
    work_bits  -= 8;
  }

  /* flush any extra bytes */
  if (buf_idx > 0)
    write_data_block(buf_idx, buf, outs);

  /* trailing zero byte */
  putc(0, outs);

  /* mark image as closed */
  image_open = 0;

  /* done! */
  return GIFLIB_SUCCESS;
}


/*
 * close an open GIF file
 */
static int gifout_close_file()
{
  /* make sure there's a file open */
  if (!file_open)
    return GIFLIB_ERR_NFO;

  /* make sure there isn't an image open */
  if (image_open)
    return GIFLIB_ERR_ISO;

  /* write gif terminator */
  putc(GIF_TERMINATOR, outs);

  /* mark file as closed */
  file_open = 0;

  /* done! */
  return GIFLIB_SUCCESS;
}


/****
 **
 ** internal procedures
 **
 ****/

static int cmap_bits(n)
     int n;
{
  int nbits;

  if (n < 2) {
	fprintf(stderr,"cmap_bits(): argument out of range\n");
return( 0 );
  }

  n    -= 1;
  nbits = 0;

  while (n != 0)
  {
    n    >>= 1;
    nbits += 1;
  }

  return nbits;
}


static int root_bits(n)
     int n;
{
  int rslt;

  rslt = cmap_bits(n);
  if (rslt < 2)
    rslt = 2;

  return rslt;
}


static void put_clr_code()
{
  /* output clear code */
  PUT_CODE(clr_code);

  /* reset raster data stream */
  code_size = root_size + 1;
  code_mask = (1 << code_size) - 1;
  old_code  = NULL_CODE;

  /* clear the string table */
  reset_string_out();
}


static void write_data_block(cnt, buf, s)
     int   cnt;
     BYTE *buf;
     FILE *s;
{
  putc(cnt, s);

  if (fwrite(buf, sizeof(BYTE), cnt, s) != cnt) {
    fprintf(stderr,"No disk space\n");
    giferror = 1;
  }
}


static void reset_string_out()
{
  int i;

  for (i=0; i<HASHSZ; i++)
    htable[i] = NULL_CODE;

  table_size = eoi_code + 1;
}


static void add_string_out(p, e)
     int p;
     int e;
{
  int idx;

  idx = HASH(p, e);

  prefix[table_size] = p;
  extnsn[table_size] = e;

  next[table_size] = htable[idx];
  htable[idx]      = table_size;

  if ((table_size > code_mask) && (code_size < 12))
  {
    code_size += 1;
    code_mask  = (1 << code_size) - 1;
  }

  table_size += 1;
}


static int find_string_out(p, e)
     int p;
     int e;
{
  int idx;
  int rslt;

  if (p == NULL_CODE)
  {
    /* a lone symbol is always in table */
    rslt = e;
  }
  else
  {
    rslt = NULL_CODE;

    /* search the hash table */
    idx = htable[HASH(p, e)];
    while (idx != NULL_CODE)
      if ((prefix[idx] == p) && (extnsn[idx] == e))
      {
        rslt = idx;
        break;
      }
      else
      {
        idx = next[idx];
      }
  }

  return rslt;
}

static int gifout_write_graphiccontrol(int trans,int delay) {
    char buf[8];

    buf[0] = GIF_EXTENSION;
    buf[1] = 0xf9;		/* transparent */
    buf[2] = 4;
    buf[3] = trans!=-1;		/* trans flag */
    buf[4] = delay&0xff;
    buf[5] = delay>>8;		/* Bytes 4,5 form a short containing delay time in 100ths of sec */
    buf[6] = trans==-1?0:trans;
    buf[7] = 0;
  if (fwrite(buf, sizeof(char), 8, outs) != 8) {
    giferror = 1;
    return GIFLIB_ERR_OUT;
  }
  return GIFLIB_SUCCESS;
}

static int gifout_write_animated_appexten(void) {
    char buf[19];

    buf[0] = GIF_EXTENSION;
    buf[1] = 0xff;		/* Application extension */
    buf[2] = 11;
    strcpy(buf+3,"NETSCAPE2.0");
    buf[14]= 3;
    buf[15]= 1;
    buf[16]= 232;
    buf[17]= 3;
    buf[18]= 0;
  if (fwrite(buf, sizeof(char), 19, outs) != 19) {
    giferror = 1;
    return GIFLIB_ERR_OUT;
  }
  return GIFLIB_SUCCESS;
}

static int gifout_write_image(struct _GImage *base, int width, int height,
	int progressive) {
    int i,k,j;
    static int yincr[] = { 8, 8, 4, 2 };
    static int ybase[] = { 0, 4, 2, 1 };
    int *irow;
    char *row;

    gifout_open_image(0,0, width,height,progressive);
    irow = (int *) galloc(width*sizeof(int));
    if ( progressive ) {
	i = 0;
	for ( k=0; k<4; ) {
	    row = (char *) (base->data + i*base->bytes_per_line);
	    for ( j=0; j<width; ++j )
		irow[j] = ((unsigned char *) row)[j];
	    gifout_put_row(irow);
	    i += yincr[k];
	    while ( i>= height )
		i = (++k==4)? -1 : ybase[k];
	}
    } else {
	for ( i=0; i<height; ++i ) {
	    row = (char *) (base->data + i*base->bytes_per_line);
	    for ( j=0; j<width; ++j )
		irow[j] = ((unsigned char *) row)[j];
	    gifout_put_row(irow);
	}
    }
    gifout_close_image();
    gfree(irow);
  return GIFLIB_SUCCESS;
}

int GImageWriteGif(GImage *gi,char *filename,int progressive) {
    unsigned char cmap[3][256];
    int i,k;
    short usedcols;
    FILE *file;
    struct _GImage *base = gi->list_len==0?gi->u.image:gi->u.images[0];

    /* check that all the images have the same color table */
    if ( gi->list_len==0 ) {
	if ( gi->u.image->image_type==it_true ) {
	    fprintf(stderr,"Gif save failed, attempt to save a true color image\n");
return( false );
	}
    } else {
	for ( i=0; i<gi->list_len; ++i ) {
	    if ( gi->u.images[i]->image_type!=base->image_type ||
		    ( gi->u.images[i]->clut==NULL ) != (base->clut==NULL)) {
		fprintf(stderr,"Gif save failed, mismatched image types\n");
return( false );
	    }
	    if ( base->clut!=NULL ) {
		if ( gi->u.images[i]->clut->clut_len != base->clut->clut_len )
return( false );
		for ( k=0; k<base->clut->clut_len; ++k )
		    if ( gi->u.images[i]->clut->clut[k]==base->clut->clut[k] )
return( false );
	    }
	}
    }

    giferror = 0;

    if ( base->clut==NULL ) {
	usedcols = 2;
	cmap[0][0] = cmap[1][0] = cmap[2][0] = 0;
	cmap[0][1] = cmap[1][1] = cmap[2][1] = 0xff;
     } else {
	usedcols = base->clut->clut_len;
	for ( i=0; i<usedcols; ++i ) {
	    cmap[0][i] = COLOR_RED(base->clut->clut[i]);
	    cmap[1][i] = COLOR_GREEN(base->clut->clut[i]);
	    cmap[2][i] = COLOR_BLUE(base->clut->clut[i]);
	}
    }

    prefix = (int *) galloc(sizeof(int)*STAB_SIZE);
    extnsn = (int *) galloc(sizeof(int)*STAB_SIZE);
    next = (int *) galloc(sizeof(int)*STAB_SIZE);

    file = fopen(filename,"wb");
    if ( file==NULL ) {
	fprintf(stderr,"File create failed\n");
return( 0 );
    }
    gifout_open_file(file,base->width,base->height,usedcols,cmap,0);
    if ( gi->list_len==0 ) {
	if ( base->trans!=-1 )
	    gifout_write_graphiccontrol(base->trans,0);
	gifout_write_image(base,base->width,base->height,progressive);
    } else {
	gifout_write_animated_appexten();	/* Marks it as animated gif???*/
	for ( k=0; k<gi->list_len; ++k ) {
	    base = gi->u.images[k];
	    gifout_write_graphiccontrol(base->trans,base->delay);
	    gifout_write_image(base,base->width,base->height,progressive);
	}
    }
    gifout_close_file();
    fclose(file);

    gfree(prefix); gfree(extnsn); gfree(next);
return( !giferror );
}
