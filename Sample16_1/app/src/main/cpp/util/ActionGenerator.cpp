#include "ActionGenerator.h"
vector<Action*> ActionGenerator::acVector;
void ActionGenerator::genData()
{
    int count = 20;
    acVector.push_back(new Action());
    acVector.push_back(new Action());
    acVector.push_back(new Action());
    acVector.push_back(new Action());
    acVector[0]->totalStep = count;
    acVector[0]->data[0]=new float[8]{ 1,0, 0, 0, 0, 0, 0 ,0};
    acVector[0]->data[1]=new float[7]{ 1, 1, 10, 10, 1, 0, 0 };   
    acVector[0]->data[2]=new float[7]{ 3, 1, -70, 0, 0.948f, 0, 0.316f };    
    acVector[0]->data[3]=new float[7]{ 5, 1, -70, 0, -0.948f, 0, 0.316f };    
    acVector[0]->data[4]=new float[7]{ 4, 1, -80, -80, 0.948f, 0, 0.316f };
    acVector[0]->data[5]=new float[7]{ 6, 1, 80, 80, -0.948f, 0, 0.316f };
    acVector[0]->data[6]=new float[7]{ 7, 1, -50, 0, 1, 0, 0 };    
    acVector[0]->data[7]=new float[7]{ 9, 1, 20, 0, 1, 0, 0 };    
    acVector[0]->data[8]=new float[7]{ 10, 1, 0, 90, 1, 0, 0 };    
    acVector[0]->data[9]=new float[7]{ 11, 1, 10, 0, 1, 0, 0 };    
    acVector[1]->totalStep=count;
    acVector[1]->data[0]=new float[8]{1,0,0,0,0,0,0,0};
    acVector[1]->data[1]=new float[7]{1,1,10,10,1,0,0};		
    acVector[1]->data[2]=new float[7]{3,1,0,70,0.948f,0,0.316f};		
    acVector[1]->data[3]=new float[7]{5,1,0,70,-0.948f,0,0.316f};	
    acVector[1]->data[4]=new float[7]{4,1,-80,-80,0.948f,0,0.316f};	
    acVector[1]->data[5]=new float[7]{6,1,80,80,-0.948f,0,0.316f};	
    acVector[1]->data[6]=new float[7]{7,1,0,20,1,0,0};		
    acVector[1]->data[7]=new float[7]{9,1,0,-50,1,0,0};		
    acVector[1]->data[8]=new float[7]{10,1,90,0,1,0,0};		
    acVector[1]->data[9]=new float[7]{12,1,0,10,1,0,0};
    acVector[2]->totalStep=count;
    acVector[2]->data[0]=new float[8]{1,0,0,0,0,0,0,0};
    acVector[2]->data[1]=new float[7]{1,1,10,10,1,0,0};		
    acVector[2]->data[2]=new float[7]{3,1,70,0,0.948f,0,0.316f};		
    acVector[2]->data[3]=new float[7]{5,1,70,0,-0.948f,0,0.316f};		
    acVector[2]->data[4]=new float[7]{4,1,-80,-80,0.948f,0,0.316f};	
    acVector[2]->data[5]=new float[7]{6,1,80,80,-0.948f,0,0.316f};	
    acVector[2]->data[6]=new float[7]{7,1,20,0,1,0,0};		
    acVector[2]->data[7]=new float[7]{9,1,-50,0,1,0,0};		
    acVector[2]->data[8]=new float[7]{8,1,0,90,1,0,0};		
    acVector[2]->data[9]=new float[7]{12,1,10,0,1,0,0};	
    acVector[3]->totalStep=count;
    acVector[3]->data[0]=new float[8]{1,0,0,0,0,0,0,0};
    acVector[3]->data[1]=new float[7]{1,1,10,10,1,0,0};	
    acVector[3]->data[2]=new float[7]{3,1,0,-70,0.948f,0,0.316f};	
    acVector[3]->data[3]=new float[7]{5,1,0,-70,-0.948f,0,0.316f};	
    acVector[3]->data[4]=new float[7]{4,1,-80,-80,0.948f,0,0.316f};
    acVector[3]->data[5]=new float[7]{6,1,80,80,-0.948f,0,0.316f};
    acVector[3]->data[6]=new float[7]{7,1,0,-50,1,0,0};	
    acVector[3]->data[7]=new float[7]{9,1,0,20,1,0,0};	
    acVector[3]->data[8]=new float[7]{8,1,90,0,1,0,0};	
    acVector[3]->data[9]=new float[7]{11,1,0,10,1,0,0};
}
ActionGenerator::~ActionGenerator()
{
    for (Action *a : acVector)
    {
        delete a;
    }
}