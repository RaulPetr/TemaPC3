#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_header.h"

int main()
{	int i,j,a;
	unsigned char zero=0x00, full=0xff;
	bmp_fileheader FH;
	bmp_infoheader IH;
	pixel **img;
	FILE *input=fopen("input.txt","rt");
	char line[100], copie[100];
	fgets(line,sizeof(line),input);			//citeste numele imaginii					
	strcpy(copie,line);						//adauga ./input/images/
	strcpy(line,"./input/images/");
	strcat(line,copie);
	strcpy(line+strlen(line)-1,"\0");		//sterge un \n de la final
	FILE *imagine=fopen(line,"rb");			
	fread(&FH.fileMarker1,sizeof(char),1,imagine);	//citire fileheader
	fread(&FH.fileMarker2,sizeof(char),1,imagine);
	fread(&FH.bfSize,sizeof(int),1,imagine);
	fread(&FH.unused1,sizeof(short),1,imagine);
	fread(&FH.unused2,sizeof(short),1,imagine);
	fread(&FH.imageDataOffset,sizeof(int),1,imagine);

	fread(&IH.biSize,sizeof(int),1,imagine);		//citire infoheader
	fread(&IH.width,sizeof(int),1,imagine);
	fread(&IH.height,sizeof(int),1,imagine);
	fread(&IH.planes,sizeof(short),1,imagine);
	fread(&IH.bitPix,sizeof(short),1,imagine);
	fread(&IH.biCompression,sizeof(int),1,imagine);
	fread(&IH.biSizeImage,sizeof(int),1,imagine);
	fread(&IH.biXPelsPerMeter,sizeof(int),1,imagine);
	fread(&IH.biYPelsPerMeter,sizeof(int),1,imagine);
	fread(&IH.biClrUsed,sizeof(int),1,imagine);
	fread(&IH.biClrImportant,sizeof(int),1,imagine);

	img=malloc(IH.height*sizeof(pixel *));			//alocare memorie
	for(i=0;i<IH.height;i++)
		img[i]=malloc(IH.width*sizeof(pixel)+4);
	int padding;									//calculare padding
	padding=(4-(IH.width*3)%4)%4;

	for(i=0;i<IH.height;i++)
		{
		for(j=0;j<IH.width;j++)
		{
			fread(&img[i][j].b,sizeof(char),1,imagine);		//citire pixeli
			fread(&img[i][j].g,sizeof(char),1,imagine);
			fread(&img[i][j].r,sizeof(char),1,imagine);
		}
		if(i!=IH.height-1)
			fseek(imagine,padding,SEEK_CUR);				//omitere padding
		}
//////////////////////////////////////////////////////////////////////////////
	FILE *output=fopen("imagine.bmp","wb");
	fwrite(&FH.fileMarker1,sizeof(char),1,output);
	fwrite(&FH.fileMarker2,sizeof(char),1,output);
	fwrite(&FH.bfSize,sizeof(int),1,output);
	fwrite(&FH.unused1,sizeof(short),1,output);
	fwrite(&FH.unused2,sizeof(short),1,output);
	fwrite(&FH.imageDataOffset,sizeof(int),1,output);

	fwrite(&IH.biSize,sizeof(int),1,output);
	fwrite(&IH.width,sizeof(int),1,output);
	fwrite(&IH.height,sizeof(int),1,output);
	fwrite(&IH.planes,sizeof(short),1,output);
	fwrite(&IH.bitPix,sizeof(short),1,output);
	fwrite(&IH.biCompression,sizeof(int),1,output);
	fwrite(&IH.biSizeImage,sizeof(int),1,output);
	fwrite(&IH.biXPelsPerMeter,sizeof(int),1,output);
	fwrite(&IH.biYPelsPerMeter,sizeof(int),1,output);
	fwrite(&IH.biClrUsed,sizeof(int),1,output);
	fwrite(&IH.biClrImportant,sizeof(int),1,output);
	
	for(i=0;i<IH.height;i++)
	{
		for(j=0;j<IH.width;j++)
		{	
			fwrite(&img[i][j].b,sizeof(char),1,output);
			fwrite(&img[i][j].g,sizeof(char),1,output);
			fwrite(&img[i][j].r,sizeof(char),1,output);
		}
		
		
		{	if(padding==3)
			{fwrite(&zero,sizeof(char),1,output);
			fwrite(&zero,sizeof(char),1,output);
			fwrite(&zero,sizeof(char),1,output);
			}
			if(padding==1)
			{
				fwrite(&zero,sizeof(char),1,output);
			}
		}
	}
	
			
		
	fclose(output);	
///////////////////BLACK&WHITE////////////

	char bw[100], *p;
	p=strtok(copie,".");
	strcpy(bw,p);
	strcat(bw,"_black_white.bmp");
	printf("%s\n",bw );
	FILE *output1=fopen(bw,"wb");
	fwrite(&FH.fileMarker1,sizeof(char),1,output1);
	fwrite(&FH.fileMarker2,sizeof(char),1,output1);
	fwrite(&FH.bfSize,sizeof(int),1,output1);
	fwrite(&FH.unused1,sizeof(short),1,output1);
	fwrite(&FH.unused2,sizeof(short),1,output1);
	fwrite(&FH.imageDataOffset,sizeof(int),1,output1);

	fwrite(&IH.biSize,sizeof(int),1,output1);
	fwrite(&IH.width,sizeof(int),1,output1);
	fwrite(&IH.height,sizeof(int),1,output1);
	fwrite(&IH.planes,sizeof(short),1,output1);
	fwrite(&IH.bitPix,sizeof(short),1,output1);
	fwrite(&IH.biCompression,sizeof(int),1,output1);
	fwrite(&IH.biSizeImage,sizeof(int),1,output1);
	fwrite(&IH.biXPelsPerMeter,sizeof(int),1,output1);
	fwrite(&IH.biYPelsPerMeter,sizeof(int),1,output1);
	fwrite(&IH.biClrUsed,sizeof(int),1,output1);
	fwrite(&IH.biClrImportant,sizeof(int),1,output1);

	fseek(output1,FH.imageDataOffset,SEEK_SET);
	int medie;
	for(i=0;i<IH.height;i++)
	{
		for(j=0;j<IH.width;j++)
		{	
			medie=(img[i][j].b+img[i][j].g+img[i][j].r)/3;
			//printf("%d: %d %d %d\n",k,img[i][j].b, img[i][j].g, img[i][j].r );
			//k+=3;
			fwrite(&medie,sizeof(char),1,output1);
			fwrite(&medie,sizeof(char),1,output1);
			fwrite(&medie,sizeof(char),1,output1);
		}
		
		if(padding==3)
		{
		fwrite(&zero,sizeof(char),1,output1);
		fwrite(&zero,sizeof(char),1,output1);
		fwrite(&zero,sizeof(char),1,output1);
		}
		if(padding==1)
		{
			fwrite(&zero,sizeof(char),1,output1);
		}
		
	}
	
	
	fclose(output1);

///////NOCROP///////////pana aici 10h au trecut////////

	char nc[100];
	int format;//format=0 inseamna ca img e portrait
	int before,after;

	strcpy(nc,p);
	strcat(nc,"_nocrop.bmp");
	printf("%s\n",bw );
	FILE *output2=fopen(nc,"wb");
	if(IH.height>IH.width)
		format=0;
	else
		format=1;

	fwrite(&FH.fileMarker1,sizeof(char),1,output2);
	fwrite(&FH.fileMarker2,sizeof(char),1,output2);
	fwrite(&FH.bfSize,sizeof(int),1,output2);
	fwrite(&FH.unused1,sizeof(short),1,output2);
	fwrite(&FH.unused2,sizeof(short),1,output2);
	fwrite(&FH.imageDataOffset,sizeof(int),1,output2);
	fwrite(&IH.biSize,sizeof(int),1,output2);
	if(format)
	{
		fwrite(&IH.width,sizeof(int),1,output2);
		fwrite(&IH.width,sizeof(int),1,output2);
	}
	else
	{
		fwrite(&IH.height,sizeof(int),1,output2);
		fwrite(&IH.height,sizeof(int),1,output2);
	}
	fwrite(&IH.planes,sizeof(short),1,output2);
	fwrite(&IH.bitPix,sizeof(short),1,output2);
	fwrite(&IH.biCompression,sizeof(int),1,output2);
	fwrite(&IH.biSizeImage,sizeof(int),1,output2);
	fwrite(&IH.biXPelsPerMeter,sizeof(int),1,output2);
	fwrite(&IH.biYPelsPerMeter,sizeof(int),1,output2);
	fwrite(&IH.biClrUsed,sizeof(int),1,output2);
	fwrite(&IH.biClrImportant,sizeof(int),1,output2);
	int padding1;
	if(format==0)
	{
		before=(IH.height-IH.width)/2+(IH.height-IH.width)%2;
		after=(IH.height-IH.width)/2;
		padding1=(4-(IH.height*3)%4)%4;
	}
	else
	{
		before=(IH.width-IH.height)/2+(IH.width-IH.height)%2;
		after=(IH.width-IH.height)/2;
		padding1=(4-(IH.width*3)%4)%4;
	}
	printf("%d %d %d\n",format, before, after);

	
	if(format)
	{
		for(i=0;i<before;i++)
		{
			for(j=0;j<IH.width;j++)
			{	
				fwrite(&full,sizeof(char),1,output2);
				fwrite(&full,sizeof(char),1,output2);
				fwrite(&full,sizeof(char),1,output2);
			}
			
			{	if(padding1==3)
				{
					fwrite(&zero,sizeof(char),1,output2);
					fwrite(&zero,sizeof(char),1,output2);
					fwrite(&zero,sizeof(char),1,output2);
				}
				if(padding1==1)
				{
					fwrite(&zero,sizeof(char),1,output2);
				}
			}
		}

		for(i=0;i<IH.height;i++)
		{
			for(j=0;j<IH.width;j++)
			{	
				fwrite(&img[i][j].b,sizeof(char),1,output2);
				fwrite(&img[i][j].g,sizeof(char),1,output2);
				fwrite(&img[i][j].r,sizeof(char),1,output2);
			}
			
			
			{	if(padding1==3)
				{fwrite(&zero,sizeof(char),1,output2);
				fwrite(&zero,sizeof(char),1,output2);
				fwrite(&zero,sizeof(char),1,output2);
				}
				if(padding1==1)
				{
					fwrite(&zero,sizeof(char),1,output2);
				}
			}
		}

		for(i=0;i<after;i++)
		{
			for(j=0;j<IH.width;j++)
			{	
				fwrite(&full,sizeof(char),1,output2);
				fwrite(&full,sizeof(char),1,output2);
				fwrite(&full,sizeof(char),1,output2);
			}
			
			{	if(padding1==3)
				{
					fwrite(&zero,sizeof(char),1,output2);
					fwrite(&zero,sizeof(char),1,output2);
					fwrite(&zero,sizeof(char),1,output2);
				}
				if(padding1==1)
				{
					fwrite(&zero,sizeof(char),1,output2);
				}
			}
		}

	}
	else
	{
		for(i=0;i<IH.height;i++)
		{
			for(j=0;j<before;j++)
			{
				fwrite(&full,sizeof(char),1,output2);
				fwrite(&full,sizeof(char),1,output2);
				fwrite(&full,sizeof(char),1,output2);
			}
			for(j=0;j<IH.width;j++)
			{
				fwrite(&img[i][j].b,sizeof(char),1,output2);
				fwrite(&img[i][j].g,sizeof(char),1,output2);
				fwrite(&img[i][j].r,sizeof(char),1,output2);
			}
			for(j=0;j<after;j++)
			{
				fwrite(&full,sizeof(char),1,output2);
				fwrite(&full,sizeof(char),1,output2);
				fwrite(&full,sizeof(char),1,output2);
			}
			if(padding1==3)
				{
					fwrite(&zero,sizeof(char),1,output2);
					fwrite(&zero,sizeof(char),1,output2);
					fwrite(&zero,sizeof(char),1,output2);
				}
			if(padding1==1)
			{
				fwrite(&zero,sizeof(char),1,output2);
			}

		}
	}
	fclose(output2);
///////CONVOLAYER///////////surprisingly small time on nocrop////////
	char cl[100],filt[100];
	int n, **filter;
	fgets(filt,sizeof(filt),input);
	strcpy(filt+strlen(filt)-1,"\0");
	FILE *filter_in=fopen(filt,"rt");
	fscanf(filter_in,"%d",&n);
	filter=malloc(n*sizeof(int *));
	for(i=0;i<n;i++)
		filter[i]=malloc(n*sizeof(int));
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			fscanf(filter_in,"%d",&filter[i][j]);

//rasturnare matrice
	int aux;
//mai intai liniile
	/*for(i=0;i<n;i++)
		for(j=0;j<n/2;j++)
		{
			aux=filter[i][j];
			filter[i][j]=filter[i][n-j-1];
			filter[i][n-j-1]=aux;
		}*/
//apoi coloanele
	for(i=0;i<n/2;i++)
		for(j=0;j<n;j++)
		{
			aux=filter[n-i-1][j];
			filter[n-i-1][j]=filter[i][j];
			filter[i][j]=aux;
		}

	for(i=0;i<n;i++)
		{for(j=0;j<n;j++)
			printf("%d",filter[i][j]);
		printf("\n");}
	strcpy(cl,p);
	strcat(cl,"_filter.bmp");
	FILE *output3=fopen(cl,"wb");
	fwrite(&FH.fileMarker1,sizeof(char),1,output3);
	fwrite(&FH.fileMarker2,sizeof(char),1,output3);
	fwrite(&FH.bfSize,sizeof(int),1,output3);
	fwrite(&FH.unused1,sizeof(short),1,output3);
	fwrite(&FH.unused2,sizeof(short),1,output3);
	fwrite(&FH.imageDataOffset,sizeof(int),1,output3);

	fwrite(&IH.biSize,sizeof(int),1,output3);
	fwrite(&IH.width,sizeof(int),1,output3);
	fwrite(&IH.height,sizeof(int),1,output3);
	fwrite(&IH.planes,sizeof(short),1,output3);
	fwrite(&IH.bitPix,sizeof(short),1,output3);
	fwrite(&IH.biCompression,sizeof(int),1,output3);
	fwrite(&IH.biSizeImage,sizeof(int),1,output3);
	fwrite(&IH.biXPelsPerMeter,sizeof(int),1,output3);
	fwrite(&IH.biYPelsPerMeter,sizeof(int),1,output3);
	fwrite(&IH.biClrUsed,sizeof(int),1,output3);
	fwrite(&IH.biClrImportant,sizeof(int),1,output3);

	int b,g,r,k,l,b1,g1,r1;

	for(i=0;i<IH.height;i++)
	{
		for(j=0;j<IH.width;j++)
		{	
			b=g=r=0;
			for(k=0;k<n;k++)
				for(l=0;l<n;l++)
				{
					if((i+k-n/2)<0||(i+k-n/2)>=IH.height)
						b1=g1=r1=0;
					else
					if((j+l-n/2)<0||(j+l-n/2)>=IH.width)
						b1=g1=r1=0;
					else
					{
						b1=img[i+k-n/2][j+l-n/2].b;
						g1=img[i+k-n/2][j+l-n/2].g;
						r1=img[i+k-n/2][j+l-n/2].r;
					}


					b=b+b1*filter[k][l];
					g=g+g1*filter[k][l];
					r=r+r1*filter[k][l];
				}
			if(b>255)
				b=255;
			if(r>255)
				r=255;
			if(g>255)
				g=255;
			if(b<0)
				b=0;
			if(g<0)
				g=0;
			if(r<0)
				r=0;
			fwrite(&b,sizeof(char),1,output3);
			fwrite(&g,sizeof(char),1,output3);
			fwrite(&r,sizeof(char),1,output3);
		}
		
		if(padding==3)
		{
			fwrite(&zero,sizeof(char),1,output3);
			fwrite(&zero,sizeof(char),1,output3);
			fwrite(&zero,sizeof(char),1,output3);
		}
		if(padding==1)
		{
			fwrite(&zero,sizeof(char),1,output3);
		}
	}	






	for(i=0;i<IH.height;i++)			//eliberare memorie
		free(img[i]);
	free(img);
	return 0;}
