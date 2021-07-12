#ifndef DATA_H
#define DATA_H

#include  "MemAlloc.h"

typedef struct  Data  Data;
struct  Data{
  int val1;
  double  val2;
};

Data  *createData();
void  freeData(Data *data);

#endif // DATA_H
