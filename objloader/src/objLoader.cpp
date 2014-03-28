#include "objLoader.h"

vector<int> objLoader::extractface(string temp)
{
    vector<int> face;
    int num=0;
    unsigned int index=2;
    while(index < temp.size())
    {
        if(temp[index]!=' ' && temp[index]!='/')
        {
            num = num * 10 + (temp[index]-'0');
        }
        else
        {
            if(num>0)
                face.push_back(num-1);
            num=0;
            if(temp[index]=='/' && temp[index+1]=='/')
                face.push_back(0);
        }
        index++;
    }
    
    face.push_back(num-1);
    
    return face;
}


objLoader::objLoader(const char* dir)
{
    ifstream file(dir,ios::in);
    if(!file)
    {
        cerr << "can not opem" << dir<<endl;
        exit(1);

    }
    
    string line;

    while( getline(file,line))
    {
        if(line.substr(0,2)== "v ")
        {
            
            istringstream v(line.substr(2));
            float x,y,z;
            v>>x;v>>y;v>>z;
            vertices.push_back(vec4(x,y,z,1.0f));
        }
        if(line.substr(0,2)=="vn")
        {
            istringstream v(line.substr(3));
            float x,y,z;
            v>>x;v>>y;v>>z;
            normals.push_back(vec4(x,y,z,1.0f));        
        }

        if(line.substr(0,2)=="f ")
        {
            face.push_back(extractface(line));
        }
    }
    reOrganizeVertices();
    file.close();
}

int objLoader::sizeVert()
{
    return vertices.size();
}
int objLoader::sizeNorm()
{
    return normals.size();
}
int objLoader::sizeFace()
{
    return face.size();
}

int objLoader::sizeMesh()
{
    return mesh.size();
}

vec4* objLoader::Vert()
{
    return (vec4*)(&(*vertices.begin()));
}

vec4* objLoader::Mesh()
{
    return (vec4*)(&(*mesh.begin()));
}
vec4* objLoader::MeshNorm()
{
    return (vec4*)(&(*meshnorm.begin()));
}

void objLoader::reOrganizeVertices()
{
    for(unsigned int i=0; i < face.size(); i++)
    {
        if(face[i].size()<12)
        {
            mesh.push_back(vertices[face[i][0]]);
            mesh.push_back(vertices[face[i][3]]);
            mesh.push_back(vertices[face[i][6]]);    
            meshnorm.push_back(normals[face[i][2]]);
            meshnorm.push_back(normals[face[i][5]]);        
            meshnorm.push_back(normals[face[i][8]]);
        }
        else
        {
        
            mesh.push_back(vertices[face[i][0]]);
            mesh.push_back(vertices[face[i][3]]);
            mesh.push_back(vertices[face[i][6]]);    
            mesh.push_back(vertices[face[i][6]]);    
            mesh.push_back(vertices[face[i][9]]);    
            mesh.push_back(vertices[face[i][0]]);    
            meshnorm.push_back(normals[face[i][2]]);
            meshnorm.push_back(normals[face[i][5]]);
            meshnorm.push_back(normals[face[i][8]]);
            meshnorm.push_back(normals[face[i][8]]);
            meshnorm.push_back(normals[face[i][11]]);
            meshnorm.push_back(normals[face[i][2]]);        
        }
    }
}
