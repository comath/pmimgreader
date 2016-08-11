#include <stdio.h>
#include <stdlib.h>
#include "pgmreader.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))


char nextch(FILE *fp)
{
	char ch;
	fread(&ch,sizeof(ch),1,fp);
	return ch;
}
/*
char ** pgm_read(char *filename, PGMheader *header)
{
	FILE *imgfile;
	log_info("Opening file %s\n",filename);
	imgfile = fopen(filename,"r");
	check(imgfile,"Unable to open %s", filename);
	int i=0;
	int j=0;
	check(nextch(imgfile) == 'P', "Invalid PGM file.");
	check(nextch(imgfile)-48 ==5, "Invalid Greyscale PGM file.");
	while(nextch(imgfile) != '\n');	
	while(nextch(imgfile) == '#'){
		while(nextch(imgfile) != '\n');
	}
	fseek(imgfile, -1, SEEK_CUR);
	/*while(!feof(imgfile)){
		printf("%c", nextch(imgfile));
	}

	fscanf(imgfile,"%d", &((*header).width));
	fscanf(imgfile,"%d", &((*header).height));
	fscanf(imgfile,"%d", &((*header).maxval));
	while(nextch(imgfile) != '\n');
	while(!feof(imgfile)){
		printf("%c", nextch(imgfile));
	}

	check(((*header).maxval) == 255, "Not valid maxval for this program, needs to be 255");

	char **pixarr;
	pixarr = (char **)malloc(sizeof(char *)*((*header).height));
	check(pixarr,"Memory Error");
	for(i=0; i < ((*header).height); i++){
		pixarr[i] = (char *)malloc(sizeof(char)*((*header).width));
		check(pixarr[i],"Memory Error");
	}

	for(i=0;i<((*header).height);i++){
		for(j=0;j<((*header).width);j++){
			pixarr[i][j]=nextch(imgfile);
		}
	}

	fclose(imgfile);
	return pixarr;
error:
	return NULL;
}

void pgm_write(char *filename,PGMheader *header, char **pixarr)
{
	FILE *fp;
	fp = fopen(filename,"wb");
	check(fp,"Unable to create %s",filename);
	char magicnum[3] = {'P','5','\n'};
	fwrite(magicnum,sizeof(char)*3,1,fp);
	int rc = fprintf(fp, "%d",header->width);
	check(rc>0,"File write error");
	fputc(' ',fp);
	rc = fprintf(fp, "%d", header->height);
	check(rc>0,"File write error");
	fputc('\n',fp);
	rc = fprintf(fp, "%d", header->maxval);
	check(rc>0,"File write error");
	fputc('\n',fp);

	int i,j =0;
	for(i=0;i<(header->height);i++){
		for(j=0;j<(header->width);j++){
			fputc(pixarr[i][j],fp);
		}
	}

	//fclose(fp);
error:
	if(fp) fclose(fp);
}

void pgm_free(PGMheader *header,char ** pixarr)
{
	int i =0;
	for(i=0; i < ((*header).height); i++){
		free(pixarr[i]);
	}
	free(pixarr);
}
*/

pm_img * pm_read(char *filename)
{
	FILE *imgfile;
	//log_info("Opening file %s\n",filename);
	imgfile = fopen(filename,"r");
	//check(imgfile,"Unable to open %s", filename);
	int i=0;
	int j=0;
	//check(nextch(imgfile) == 'P', "Invalid PGM file.");
	int type = nextch(imgfile)-48;
	//check(type == 6 || type == 5, "Invalid Greyscale PGM or PPM file.");
	


	while(nextch(imgfile) != '\n');	
	while(nextch(imgfile) == '#'){
		while(nextch(imgfile) != '\n');
	}
	fseek(imgfile, -1, SEEK_CUR);
	/*while(!feof(imgfile)){
		printf("%c", nextch(imgfile));
	}*/
	int width,height,maxval;
	fscanf(imgfile,"%d", width);
	fscanf(imgfile,"%d", height);
	fscanf(imgfile,"%d", maxval);
	while(nextch(imgfile) != '\n');
	/*while(!feof(imgfile)){
		printf("%c", nextch(imgfile));
	}*/

	pm_img *img = pm_new(height,width,maxval,type);

	//check(img->maxval == 255, "Not valid maxval for this program, needs to be 255");
	if(type == 6) {
		for(i=0;i<img->height;i++){
			for(j=0;j<img->width;j++){
				img->pixarr[i][j].r=nextch(imgfile);
				img->pixarr[i][j].g=nextch(imgfile);
				img->pixarr[i][j].b=nextch(imgfile);
			}
		}
	} else {
		for(i=0;i<img->height;i++){
			for(j=0;j<img->width;j++){
				img->pixarr[i][j].r=nextch(imgfile);
			}
		}
	}

	fclose(imgfile);
	return img;
error:
	return NULL;
}

pm_img * pm_new(int height, int width, int maxval, int type)
{
	img = Malloc(pm_img, 1);
	img->height = height;
	img->width = width;
	img->maxval = maxval;
	img->type = type;
	rgbpix **pixarr;
	pixarr = (rgbpix **)malloc(sizeof(rgbpix *)*(img->height));
	//check(pixarr,"Memory Error");
	for(i=0; i < img->height; i++){
		pixarr[i] = (rgbpix *)malloc(sizeof(rgbpix)*(img->width));
		//check(pixarr[i],"Memory Error");
	}
}


void pm_write(const char *filename,pm_img *img)
{
	FILE *fp;
	fp = fopen(filename,"wb");
	//check(fp,"Unable to create %s",filename);
	char magicnum[3] = {'P',(char)(img->type + 48),'\n'};
	fwrite(magicnum,sizeof(char)*3,1,fp);
	int rc = fprintf(fp, "%d",img->width);
	//check(rc>0,"File write error");
	fputc(' ',fp);
	rc = fprintf(fp, "%d", img->height);
	//check(rc>0,"File write error");
	fputc('\n',fp);
	rc = fprintf(fp, "%d", img->maxval);
	//check(rc>0,"File write error");
	fputc('\n',fp);

	int i,j =0;
	if(img->type == 6) {
		for(i=0;i<(img->height);i++){
			for(j=0;j<(img->width);j++){
				fputc(img->pixarr[i][j].r,fp);
				fputc(img->pixarr[i][j].g,fp);
				fputc(img->pixarr[i][j].b,fp);
			}
		}
	} else {
		for(i=0;i<(img->height);i++){
			for(j=0;j<(img->width);j++){
				fputc(img->pixarr[i][j].r,fp);
			}
		}
	}

	//fclose(fp);
error:
	if(fp) fclose(fp);
}

void pm_free(pm_img *img)
{
	int i,j =0;
	for(i=0; i < (img->height); i++){
		free(img->pixarr[i]);
	}
	free(img);
}