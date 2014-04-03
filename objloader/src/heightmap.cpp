#include "heightmap.h"

heightmap::heightmap(const char* dir,float r)
{
    loadHeightmap(dir);
    _scaling_varible = r;
}


void heightmap::loadHeightmap(const char* dir)
{
    _data = read_ppm(dir,&_width,&_width,&_max);    
}

void heightmap::unloadHeightmap()
{
    free(_data);
}

void heightmap::saveHeightmap(char* dir)
{
    write_ppm(dir,_width,_width,_max,(int*)_data);
}

