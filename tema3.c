#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_header.h"

#define strlength 100


void header_write (FILE *output,bmp_fileheader FH, bmp_infoheader IH)
{
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
}

void write_pixel (FILE *output,unsigned char b,unsigned char g,unsigned char r)
{
	fwrite(&b,sizeof(char),1,output);			//Write pixels.
	fwrite(&g,sizeof(char),1,output);
	fwrite(&r,sizeof(char),1,output);
}

int main()
{	int i,j,a,k,l;
	unsigned char zero=0x00, full=0xff;
	bmp_fileheader FH;
	bmp_infoheader IH;
	pixel **img;

	FILE *input=fopen("input.txt","rt");
	char line[strlength], copie[strlength];
	fgets(line,sizeof(line),input);			// Reads image name.					
	strcpy(copie,line);						// Adds ./input/images at beginning
	strcpy(line,"./input/images/");			//	of string.
	strcat(line,copie);
	strcpy(line+strlen(line)-1,"\0");		// Erases last \n (from fgets).

	FILE *imagine=fopen(line,"rb");			
	fread(&FH.fileMarker1,sizeof(char),1,imagine);	//FileHeader reading.
	fread(&FH.fileMarker2,sizeof(char),1,imagine);
	fread(&FH.bfSize,sizeof(int),1,imagine);
	fread(&FH.unused1,sizeof(short),1,imagine);
	fread(&FH.unused2,sizeof(short),1,imagine);
	fread(&FH.imageDataOffset,sizeof(int),1,imagine);

	fread(&IH.biSize,sizeof(int),1,imagine);		//InfoHeader reading.
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

	int padding;									// Calculate padding.
	padding=(4-(IH.width*3)%4)%4;

	img=malloc(IH.height*sizeof(pixel *));			// Memory allocation
	for(i=0;i<IH.height;i++)
		img[i]=malloc(IH.width*sizeof(pixel)+padding);
	
	for(i=0;i<IH.height;i++)
		{
		for(j=0;j<IH.width;j++)
		{
			fread(&img[i][j].b,sizeof(char),1,imagine);		// Read every pixel
			fread(&img[i][j].g,sizeof(char),1,imagine);
			fread(&img[i][j].r,sizeof(char),1,imagine);
		}
		if(i!=IH.height-1)
			fseek(imagine,padding,SEEK_CUR);				//Jump over padding
		}
///////////////////BLACK&WHITE////////////

	char bw[strlength], *p;
	p=strtok(copie,".");
	strcpy(bw,p);
	strcat(bw,"_black_white.bmp");				// Create image name string

	FILE *output1=fopen(bw,"wb");				// Open output with previously 
	header_write(output1,FH,IH);				//	created string.

	int medie;
	for(i=0;i<IH.height;i++)
	{
		for(j=0;j<IH.width;j++)
		{	
			medie=(img[i][j].b+img[i][j].g+img[i][j].r)/3;	//Calculate average.
			write_pixel(output1,medie,medie,medie);
		}
		
		for(k=0;k<padding;k++)
			fwrite(&zero,sizeof(char),1,output1);
	}
	
	fclose(output1);

///////NOCROP///////////pana aici 10h au trecut////////

	char nc[strlength];
	int format;			//if format==0, image is portrait
	int before,after;

	strcpy(nc,p);
	strcat(nc,"_nocrop.bmp");
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

	int padding1;			//calculate before/after lines and new padding
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
		padding1=padding;
	}

	if(format)
	{
		for(i=0;i<before;i++)
		{
			for(j=0;j<IH.width;j++)
				write_pixel(output2,full,full,full);
		
			for(k=0;k<padding;k++)
			fwrite(&zero,sizeof(char),1,output2);
		}

		for(i=0;i<IH.height;i++)
		{
			for(j=0;j<IH.width;j++)
				write_pixel(output2,img[i][j].b,img[i][j].g,img[i][j].r);
			
			for(k=0;k<padding;k++)
			fwrite(&zero,sizeof(char),1,output2);
		}

		for(i=0;i<after;i++)
		{
			for(j=0;j<IH.width;j++)	
				write_pixel(output2,full,full,full);

			for(k=0;k<padding1;k++)
			fwrite(&zero,sizeof(char),1,output2);
		}

	}
	else
	{
		for(i=0;i<IH.height;i++)
		{
			for(j=0;j<before;j++)
			write_pixel(output2,full,full,full);

			for(j=0;j<IH.width;j++)
			write_pixel(output2,img[i][j].b,img[i][j].g,img[i][j].r);

			for(j=0;j<after;j++)
			write_pixel(output2,full,full,full);

			for(k=0;k<padding1;k++)
			fwrite(&zero,sizeof(char),1,output2);

		}
	}
	fclose(output2);
///////CONVOLAYER///////////surprisingly small time on nocrop////////
	char cl[strlength],filt[strlength];
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

	int aux;
	for(i=0;i<n/2;i++)
		for(j=0;j<n;j++)
		{
			aux=filter[n-i-1][j];
			filter[n-i-1][j]=filter[i][j];
			filter[i][j]=aux;
		}

	strcpy(cl,p);
	strcat(cl,"_filter.bmp");
	FILE *output3=fopen(cl,"wb");

	header_write(output3, FH,IH);

	int b,g,r,b1,g1,r1;

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
			write_pixel(output3,b,g,r);
		}
		for(k=0;k<padding1;k++)
			fwrite(&zero,sizeof(char),1,output3);
	}	
	fclose(output3);
//////POOLING///////////////////////////////////////////////////////////////
	char pl[strlength],type;
	int size;
	strcpy(pl,p);
	strcat(pl,"_pooling.bmp");
	FILE *output4=fopen(pl,"wb");
	fgets(filt,sizeof(filt),input);
	strcpy(filt+strlen(filt)-1,"\0");
	FILE *pooling=fopen(filt,"rt");
	fscanf(pooling,"%c %d",&type,&size);

	header_write(output4,FH,IH);

	int minb, ming, minr, maxb, maxg, maxr;

	for(i=0;i<IH.height;i++)
	{
		for(j=0;j<IH.width;j++)
		{	
			minb=ming=minr=0xff;
			maxb=maxg=maxr=0x00;
			for(k=0;k<size;k++)
				for(l=0;l<size;l++)
				{	
					
					if((i+k-size/2)<0||(i+k-size/2)>=IH.height)
						b1=g1=r1=0;
					else
					if((j+l-size/2)<0||(j+l-size/2)>=IH.width)
						b1=g1=r1=0;
					else
					{
						b1=img[i+k-size/2][j+l-size/2].b;
						g1=img[i+k-size/2][j+l-size/2].g;
						r1=img[i+k-size/2][j+l-size/2].r;
					}

					if(b1>maxb)
						maxb=b1;
					if(b1<minb)
						minb=b1;
					if(g1>maxg)
						maxg=g1;
					if(g1<ming)
						ming=g1;
					if(r1>maxr)
						maxr=r1;
					if(r1<minr)
						minr=r1;
				}
			if(type=='M')
				write_pixel(output4,maxb,maxg,maxr);
			else
				write_pixel(output4,minb,ming,minr);
		}
		
		for(k=0;k<padding1;k++)
			fwrite(&zero,sizeof(char),1,output4);
	}
	fclose(output4);
/////////////CLUSTERING/////////////////////////////////////////////////////////
//NOT_WORKING//
	int **zone,zonenr=0; //matricea ce contine zonele pixelilor si contorul
	int zones=256; //se aloca spatiu pentru cate 256 de culori de zona
	Zone_Color *zc; //vector de pixeli cu culoarea fiecarei zone
	int thr; //thr=threshhold
	int ok,count;
	zone=malloc(IH.height*sizeof(int *));			//alocare memorie
	for(i=0;i<IH.height;i++)
		zone[i]=malloc(IH.width*sizeof(zone));
	for(i=0;i<IH.height;i++)
		for(j=0;j<IH.width;j++)
			zone[i][j]=0;
	zc=malloc(zones*sizeof(Zone_Color));

	char clus[strlength];
	strcpy(clus,p);
	strcat(clus,"_clustered.bmp");
	FILE *output5=fopen(clus,"wb");
	fgets(filt,sizeof(filt),input);
	strcpy(filt+strlen(filt)-1,"\0");
	FILE *clustering=fopen(filt,"rt");
	fscanf(clustering,"%d",&thr);

	for(i=IH.height-1;i>=0;i--)
		for(j=0;j<IH.width;j++)
		{
			if(zone[i][j]==0)
			{	
				zonenr++;	
				b=g=r=0;
				count=1;
				zone[i][j]=zonenr;

				b1=img[i][j].b;
				g1=img[i][j].g;
				r1=img[i][j].r;

				b=b+img[i][j].b;
				g=g+img[i][j].g;
				r=r+img[i][j].r;

				ok=0;
				for(k=i;k>=0;k--)
					for(l=0;l<IH.width;l++)
					{
						if(ok)
						{
							l=j+1;
							ok=0;
						}
						if(k>i||l<IH.width) //in caz ca pixelul gasit liber e la
						// capat de rand, nu se iese din spatiul de memorie
						{
							if(( abs(b1-img[k][l].b) + abs(g1-img[k][l].g)
								+abs(r1-img[k][l].r) ) <= thr)
								if(zone[k][l]==0)
								{
									zone[k][l]=zonenr;
									b=b+img[k][l].b;
									g=g+img[k][l].g;
									r=r+img[k][l].r;
									count++;
								}
						}
					}

				b=b/count;
				g=g/count;
				r=r/count;

				if(zonenr+3>zones)	//realocare pt. vectorul pixelilor zonelor
				{	
					zones*=2;
					zc=realloc(zc,zones*sizeof(Zone_Color));
				}

				zc[zonenr].b=b;
				zc[zonenr].g=g;
				zc[zonenr].r=r;
				}


			}

	header_write(output5,FH,IH);
	for(i=0;i<IH.height;i++)
	{
		for(j=0;j<IH.width;j++)
			write_pixel(output5 , zc[zone[i][j]].b , zc[zone[i][j]].g,
				zc[zone[i][j]].r);
		
		for(k=0;k<padding1;k++)
			fwrite(&zero,sizeof(char),1,output5);	
	}
	fclose(output5);	

	for(i=0;i<IH.height;i++)			//Memory freeing.
		free(img[i]);
	free(img);

	for(i=0;i<IH.height;i++)		
		free(zone[i]);
	free(zone);

	for(i=0;i<n;i++)
		free(filter[i]);
	free(filter);

	free(zc);

	return 0;
}
