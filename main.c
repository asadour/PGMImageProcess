#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

typedef struct pgm
{
	char *comments;
	char *headline;
	int width;
	int height;
	int grayscale;
}PGMImage;

/***********/
int ROW;
int COL;
/***********/
PGMImage image_info;
char *output_name;



void vertical_reflection(char* img[ROW]);
void horizontal_reflection(char* img1[ROW]);
char** rotate_left(char* img[ROW]);
void reverseColumns(char* arr[ROW]) ;
void transpose(char* arr[ROW]) ;
char** rotate_right(char* arr[ROW]) ;
void import_noise(char* img[]);
PGMImage getImgInfo(char filename[]);
void PrintImageInfo(PGMImage img);
char** allocate_2d_img();
char* allocate_1d_arr(int size);
int valid_command(char comm[]);
char* img_to_1d_array(char filename[]);
char** img_to_2d_array(char* img);
char** free_2d_array(char* arr[]);


int valid_scale(int scale)
{
	double i;
	for(i=0.0;i<=8.0;i=i+1.0)
	{
		if((int)pow(2.0,i)-1 == scale)
			return 1;
	}
	return 0;
}

char *strremove(char *str, const char *sub) 
{
    size_t len = strlen(sub);
    if (len > 0) 
    {
        char *p = str;
        size_t size = 0;
        while ((p = strstr(p, sub)) != NULL) 
	{
            size = (size == 0) ? (p - str) + strlen(p + len) + 1 : size - len;
            memmove(p, p + len, size - (p - str));
        }
    }
    return str;
}

int main(int argc, char* argv[])
{
	srand(time(NULL)); 

	unsigned char letter;
	unsigned char tmp;
	int i,j;
	int count = 0;
	char copy;
	int img_counter = 0;
	int row = 0;
	int col = 0;
	char *img;
	char **imgplus;
	
	/************************/
	
	
	 
	printf("argv %d\n",argc);	
	if(argc == 4)
	{
		output_name = allocate_1d_arr(strlen(argv[3]));
		strcpy(output_name,argv[3]);
		printf("Command : %s\n",argv[1]);
		if(strcmp(argv[1],"-fH")==0)
		{
			printf("Correct command\n");
			image_info = getImgInfo(argv[2]);
	 		PrintImageInfo(image_info);
	 		ROW = image_info.width;
	 		COL = image_info.height;
	 		 img = 	img_to_1d_array("temporary.pgm");
	 		 for(i=0;i<ROW*COL;i++)
	 		 printf("%c",img[i]);
			 imgplus = img_to_2d_array(img);
			 for(i=0;i<ROW;i++)
			 {
			 	for(j=0;j<COL;j++)
			 	printf("%d",imgplus[i][j]);
			 	printf("\n");
			 }
			 horizontal_reflection(imgplus);
		}
		else if(strcmp(argv[1],"-fV")==0)
		{
			printf("Correct command\n");
			image_info = getImgInfo(argv[2]);
	 		PrintImageInfo(image_info);
	 		ROW = image_info.width;
	 		COL = image_info.height;
	 		img = 	img_to_1d_array("temporary.pgm");
	 		imgplus = img_to_2d_array(img);
	   		vertical_reflection(imgplus);
		}
		else if(strncmp(argv[1],"-rR",3)==0 && valid_command(argv[1]) == 1)
		{
			printf("Correct command\n");
			image_info = getImgInfo(argv[2]);
	 		PrintImageInfo(image_info);
	 		ROW = image_info.width;
	 		COL = image_info.height;
	 		if(ROW == COL)
	 		{
	 		 img = 	img_to_1d_array("temporary.pgm");
			 imgplus = img_to_2d_array(img);
			 char *ptr;
			 ptr = strremove(argv[1],"-rR");
			 int times = atoi(ptr);
			   
			for(i=0;i<times;i++)
			 	imgplus = rotate_right(imgplus) ;
		}
		else
		{
				printf("Image must be quadratic\n");
		}
			
	}
		else if(strncmp(argv[1],"-rL",3)==0 && valid_command(argv[1]) == 1)
		{
			printf("Correct command\n");
			image_info = getImgInfo(argv[2]);
	 		PrintImageInfo(image_info);
	 		ROW = image_info.width;
	 		COL = image_info.height;
	 		if(ROW == COL)
	 		{
	 		   img = img_to_1d_array("temporary.pgm");
			   imgplus = img_to_2d_array(img);
			   char *ptr;
			   int times;
			   ptr = strremove(argv[1],"-rL");
			   times = atoi(ptr);
			   puts(ptr);
				for(i=0;i<times;i++)
			 		imgplus = rotate_left(imgplus);
			}
			else
			{
				printf("Image must be quadratic\n");
			}
	 		
		}
		else if(strcmp(argv[1],"-n")==0)
		{
			printf("Correct command\n");
			image_info = getImgInfo(argv[2]);
	 		PrintImageInfo(image_info);
	 		ROW = image_info.width;
	 		COL = image_info.height;
	 		img = img_to_1d_array("temporary.pgm");
	 		imgplus = img_to_2d_array(img);
	   		import_noise(imgplus);
		}
		else
		{
			printf("Please insert a valid command!\n");
			printf("Valid commands : \n(Flip Horizontally)-fH input_name output_name\n(Flip Vertically)-fV input_name output_name\n(Right Rotation n*90 degrees) -rR(n) input_name output_name\n(Left Rotation n*90 degrees)-rL(n) input_name output_name\n(Insert noise in image)-n input_name output_name\n");
			return 0;
		}
	}
	else
	{
		printf("Try again ... Missing arguments!!\n");
		return 0;
	}
	 
	free_2d_array(imgplus);
	free(img);
	
	return 0;
}

char* img_to_1d_array(char filename[])
{
	char* img = allocate_1d_arr(ROW*COL);
	FILE* imagein = fopen(filename,"rb+");
	if(imagein == NULL)
    	{
    		printf("Error in file !\n");
		exit(0);		
	}
	unsigned char letter;
	int count = 0;
	if(img == NULL)
	 return 0;
	if(imagein==NULL)
	{
	    perror("Error");
	    exit(1);
	}
	else
	{
	  	while(!feof(imagein))
		{
			fread(&letter, sizeof(char), 1, imagein);
			img[count] = letter;
			count++;
		}
	}
	  fclose(imagein);
	  return img;
}

char** img_to_2d_array(char* img)
{
	int i,j;
	unsigned char tmp;
	char** imgplus = allocate_2d_img();
	FILE* imageout = fopen("exampleout.pgm","wb+");
	if(imageout == NULL)
    	{
    		printf("Error in file !\n");
		exit(0);		
	}
	int img_counter = 0;
	if(imgplus == NULL)
	{
	    perror("Error");
	    exit(1);
	}
	 for(i=0;i<ROW;i++)
	 {
	 	for(j=0;j<COL;j++)
	 	{
	 		imgplus[i][j] = img[img_counter];
	 		img_counter = img_counter +1;
	 		tmp = imgplus[i][j];
	 		fwrite(&tmp, sizeof(char), 1, imageout);
		 } 
	 }
fclose(imageout);
return imgplus;
}

int valid_command(char comm[])
{
	int i;
	int counter_alpha = 0;
	int counter_num = 0;
	for(i=0;i<strlen(comm);i++)
	{
		if (isalpha(comm[i]) || comm[i]=='-') 
		{
			counter_alpha ++;
			if(counter_alpha > 3 )
			 return 0;
		}
		if(isdigit(comm[i]))
		{
			if(counter_alpha<3 || counter_alpha>3)
			return 0;
		}
	}
	if(counter_alpha == 3)
	return 1;
}
char** allocate_2d_img()
{
	char **img_arr;
	int i;
	img_arr = (char**)malloc(ROW*sizeof(char*));
	for(i=0;i<ROW;i++)
		img_arr[i] = (char*)malloc(COL*sizeof(char));
	return img_arr;
}

char* allocate_1d_arr(int size)
{
	char* img;
	img = (char*)malloc(sizeof(char)*size);
	return img;
}

void PrintImageInfo(PGMImage img)
{
	printf("\nInfo for your image !\n");
	printf("Headline %s\nComments %s\nDimensions %d X %d\nGrayscale %d\n",img.headline,img.comments,img.width,img.height,img.grayscale);
}
PGMImage getImgInfo(char filename[])
{
	FILE *trav;
	FILE *tmp_file;
	PGMImage tmp;
	char fname[strlen(filename)];
	char end[]=".pgm";
	char *line;
	char chrcter;
	strcpy(fname,filename);
	trav = fopen(strcat(fname,end),"rb");
	if(trav == NULL)
	{
		printf("File does not exist !!\n");
		exit(0);
	}
	tmp_file = fopen("temporary.pgm","wb+");
	if(tmp_file == NULL)
    	{
    		printf("Error in file !\n");
		exit(0);		
	}
	
	line = allocate_1d_arr(1000);
	
	int line_counter = 0;
	
	while(!feof(trav))
	{
		fgets (line, 1000, trav);
		if(line_counter == 0)
		{
			line[strlen(line)-1] = '\0';
			tmp.headline = (char*)malloc(sizeof(char)*strlen(line));
			strcpy(tmp.headline,line);
		}
		if(line_counter == 1)
		{
			if(line[0] == '#')
			{
				line[strlen(line)-1] = '\0';
				tmp.comments = (char*)malloc(sizeof(char)*strlen(line));
				strcpy(tmp.comments,line);
			}
			else
			 {
			 	line_counter = line_counter + 1;
			 	tmp.comments = (char*)malloc(sizeof(char)*strlen(line));
			 	strcpy(tmp.comments,"empty");
			 }

		}
		
		if(line_counter == 2)
		{
			line[strlen(line)-1] = '\0';
			int c_dimen = 0;
			char *token;
			token = strtok(line, " ");
			while( token != NULL ) 
			{
			      printf( " %s\n", token );
				  if(c_dimen == 0)
				  {
				  	sscanf (token,"%d",&tmp.height);
				  	c_dimen++;
				  }
				  else
				  {
				  	sscanf (token,"%d",&tmp.width);
				  }
			      token = strtok(NULL, " ");
		    	}

		}
		
		if(line_counter == 3)
		{
			line[strlen(line)-1] = '\0';
			sscanf (line,"%d",&tmp.grayscale);
			if(valid_scale(tmp.grayscale) == 0)
			{
				printf("Invalid gray scale...\n");
				
			}
		}
		if(line_counter>3)
		{
			
			while(!feof(trav))
			{
				fread(&chrcter, sizeof(char), 1, trav);
				fwrite(&chrcter, sizeof(char),1,tmp_file);
			}
			break;
		}
		line_counter++;
	}

	fclose(tmp_file);
	fclose(trav);
	free(line);
	
	return tmp;

}

void import_noise(char* img[])
{
	int previous;
	int random_perc;
	int prbty;
	int position;
	int i,j;
	char temp;
	int percentage;
	FILE *noise;
	int row_rand;
	int col_rand;
	noise = fopen(strcat(output_name,"noise.pgm"),"wb+");
	if(noise == NULL)
    	{
    		printf("Error in file !\n");
		exit(0);		
	}
	random_perc = rand()%9 + 1;
	percentage = (random_perc*ROW*COL)/100;
	for(i=0;i<percentage;i++)
	{
		row_rand = rand()%ROW;
		col_rand = rand()%COL;
		prbty = rand()%2;
		if(prbty == 0)
		{
			img[row_rand][col_rand] = image_info.grayscale+'0';
		}
		else
		{
			img[row_rand][col_rand] = '0';
		}	 
	}
	fprintf(noise,"%s\n",image_info.headline);
	if(strcmp(image_info.comments,"empty")!=0)
		fprintf(noise,"%s\n",image_info.headline);
	
	fprintf(noise,"%d %d\n",image_info.height,image_info.width);
	fprintf(noise,"%d\n",image_info.grayscale);
		for(i=0;i<ROW;i++)
	 	{
	 		for(j=0;j<COL;j++)
	 		{
	 			temp = img[i][j];
	 			fwrite(&temp, sizeof(char), 1, noise);
			}
	 	}	
	 fclose(noise);
	printf("%d",random_perc);	
}


void horizontal_reflection(char* img1[ROW])
{
	
	int l,r;
	FILE *fp_H;
	char temp;
	int i,j;
	fp_H= fopen(strcat(output_name,"horizontal.pgm"),"wb+");
	if(fp_H == NULL)
    	{
    		printf("Error in file !\n");
		exit(0);		
	}
	for(l = 0;l<ROW;l++)
	{
		for(r = 0 ; r<COL/2; r++)
		{
			temp = img1[l][r];
			img1[l][r] = img1[l][COL-1-r];
			img1[l][COL-1-r] = temp;
		}
	}
	
	fprintf(fp_H,"%s\n",image_info.headline);
	
	if(strcmp(image_info.comments,"empty")!=0)
		fprintf(fp_H,"%s\n",image_info.headline);
	
	fprintf(fp_H,"%d %d\n",image_info.height,image_info.width);
	fprintf(fp_H,"%d\n",image_info.grayscale);
	for(i=0;i<ROW;i++)
	 {
	 	for(j=0;j<COL;j++)
	 	{

	 		temp = img1[i][j];
	 		fwrite(&temp, sizeof(char), 1, fp_H);	
		 }
		 
	 }	
		 fclose(fp_H);

}

void vertical_reflection(char* img1[ROW])
{
	int l,r;
	FILE *fp_V;
	char temp;
	int i,j;
	fp_V = fopen(strcat(output_name,"vertical.pgm"),"wb+");
	if(fp_V == NULL)
    	{
    		printf("Error in file !\n");
		exit(0);		
	}
	for(l = 0;l<COL;l++)
	{
		for(r = 0 ; r<ROW/2; r++)
		{
			temp = img1[r][l];
			img1[r][l] = img1[ROW-1-r][l];
			img1[ROW-1-r][l] = temp;
		}
	}
	
	fprintf(fp_V,"%s\n",image_info.headline);
	if(strcmp(image_info.comments,"empty")!=0)
		fprintf(fp_V,"%s\n",image_info.headline);
	
	fprintf(fp_V,"%d %d\n",image_info.height,image_info.width);
	fprintf(fp_V,"%d\n",image_info.grayscale);
	for(i=0;i<ROW;i++)
	 {
	 	for(j=0;j<COL;j++)
	 	{
	 		temp = img1[i][j];
	 		fwrite(&temp, sizeof(char), 1, fp_V);	
		 }
		 
	 }	
	 fclose(fp_V);
}


char** rotate_right(char* img[ROW])
{
	FILE *right_rot;
	int i,j;
	char **img_new;
	char temp;
	char *name;
	name = (char*)malloc(strlen(output_name)*sizeof(char));
	strcpy(name,output_name);
	
	right_rot = fopen(strcat(name,"rightrot.pgm"),"wb+");
	if(right_rot == NULL)
    	{
    		printf("Error in file !\n");
		exit(0);		
	}
	img_new = (char**)malloc(COL*sizeof(char*));
	for(i=0;i<ROW;i++)
		img_new[i] = (char*)malloc(ROW*sizeof(char));
	for (i=0; i<COL; i++)
	{
		for (j=0;j<ROW; j++)
		img_new[j][COL-1-i] = img[i][j];
	}
	fprintf(right_rot,"%s\n",image_info.headline);
	if(strcmp(image_info.comments,"empty")!=0)
		fprintf(right_rot,"%s\n",image_info.headline);
	fprintf(right_rot,"%d %d\n",image_info.height,image_info.width);
	fprintf(right_rot,"%d\n",image_info.grayscale);
	for(i=0;i<ROW;i++)
	 {
	 	for(j=0;j<COL;j++)
	 	{
	 		temp = img_new[i][j];
	 		fwrite(&temp, sizeof(char), 1, right_rot);	
		 }
		 
	 }	
	 fclose(right_rot);
	 return img_new;
}
/****************************************/

void reverseColumns(char* arr[ROW]) 
{ 
int i,j,k;
char temp;
    for (i = 0; i < COL; i++) 
        for (j = 0, k = COL - 1; j < k; j++, k--) 
        {
        	temp = arr[j][i];
        	arr[j][i]= arr[k][i];
        	arr[k][i] = temp;
	} 
} 
  
void transpose(char* arr[ROW]) 
{ 
 int i,j;
 char temp;
    for (i = 0; i < ROW; i++) 
        for (j = i; j < COL; j++) 
        {
        	temp = arr[i][j];
        	arr[i][j] = arr[j][i];
        	arr[j][i] = temp;
	}
} 
  

  
char** rotate_left(char* arr[ROW]) 
{ 
    int i,j;
    char temp;
    FILE *left_rot;
    char *name;
    name = (char*)malloc(strlen(output_name)*sizeof(char));
    strcpy(name,output_name);	
    transpose(arr); 
    reverseColumns(arr); 
    left_rot = fopen(strcat(name,"leftrot.pgm"),"wb+");
    if(left_rot == NULL)
    {
    	printf("Error in file !\n");
		exit(0);		
    }
    fprintf(left_rot,"%s\n",image_info.headline);
	if(strcmp(image_info.comments,"empty")!=0)
		fprintf(left_rot,"%s\n",image_info.headline);
	fprintf(left_rot,"%d %d\n",image_info.height,image_info.width);
	fprintf(left_rot,"%d\n",image_info.grayscale);
   
    for(i=0;i<ROW;i++)
    {
	 	for(j=0;j<COL;j++)
	 	{
	 		temp = arr[i][j];
	 		fwrite(&temp, sizeof(char), 1, left_rot);
		} 
    }
	 fclose(left_rot);	
	 return arr;
} 

char** free_2d_array(char* arr[])
{
	int i;
	for(i=0;i<ROW;i++)
		free(arr[i]);
	free(arr);
	return NULL;
}
/****************************************/
