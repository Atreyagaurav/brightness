#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h> 

int read_brightness(char fname[]);
void write_brightness(char fname[], int brightness);
void brightness_calculation(int *perc, int *val, int *maxv, int p2v);
void clean_percentage(int *perc);
void non_linear(int* perc, int reverse);


int main(int argc, char **argv){
  int opt,linear;
  int brightness_p,brightness,max_brightness;
  char filename[55],max_filename[55];
  
  strcpy(filename,"/sys/class/backlight/intel_backlight/brightness");
  strcpy(max_filename,"/sys/class/backlight/intel_backlight/max_brightness");
  /* strcpy(filename,"test/brightness"); */
  /* strcpy(max_filename,"test/max_brightness"); */
  
  brightness = read_brightness(filename);
  max_brightness = read_brightness(max_filename);

  non_linear(&brightness_p, 0);
  brightness_calculation(&brightness_p, &brightness, &max_brightness, 0);
  linear = 0;
  
  while ((opt = getopt(argc,argv,":u:d:p:hl")) != -1){
    switch(opt){
    case 'l':
      linear = 1;
      break;
    case 'p':
      brightness_p = atoi(optarg);
      break;
    case 'u':
      brightness_p += atoi(optarg);
      break;
    case 'd':
      brightness_p -= atoi(optarg);
      break;
    case 'h':
      printf("Brightness Controller -by atreyagaurav\nOptions:\n");
      printf("\t-p val\t percentage brightness, val=(0-100)\n");
      printf("\t-u val\t increse by percentage, val=(0-100)\n");
      printf("\t-d val\t decrease by percentage, val=(0-100)\n");
    }
  }

  
  non_linear(&brightness_p, 1);
  brightness_calculation(&brightness_p, &brightness, &max_brightness, 1);
  write_brightness(filename,brightness);
  return 0;
}


void brightness_calculation(int* perc, int* val,int* maxv, int p2v){
  if(p2v){
    clean_percentage(perc);
    *val = *perc * *maxv /100;
  }else{
    *perc = *val * 100 / *maxv;
  }
}

void non_linear(int* perc,int reverse){
  /*this function maps 0-100 percentage with a function which gives 0-100 with non-linearity*/
  if (reverse){
    *perc = *perc * *perc /100;
  }else {
    int i;
    double r;
    r = (*perc * 100.0)/2;
    for(i=0;i<4;i++){
      r = ((*perc * 100)/r + r) / 2;
    }
    *perc = r;
  }
}

void clean_percentage(int *perc){
  if (*perc < 1){
    *perc = 1;
  }
  else if(*perc > 100){
    *perc = 100;
  }
}


int read_brightness(char filename[]){
  int b;
  FILE * fp;
  fp = fopen(filename,"r");
  fscanf(fp,"%d",&b);
  fclose(fp);
  return b;
}

void write_brightness(char filename[],int brightness){
  FILE * fp;
  fp = fopen(filename,"w");
  fprintf(fp,"%d",brightness);
  fclose(fp);
}
