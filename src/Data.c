#include  <stdio.h>
#include  "MemAlloc.h"
#include "Data.h"

Data  *createData(){
  Data  *data = memAlloc(sizeof(Data));
  data->val1 = 0;
  data->val2 = 0;
}

void  freeData(Data *data){
  if(data)
    memFree(data);
}