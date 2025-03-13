#include <iostream>
#include <stdio.h>
#include "JSONFile.h"

int main(){
    JSONFile json("TestUser","TEST.json");
    for(int i=0;i<10;i++)json.AjouterDonneesJSON("", 123, 456, 789);
    json.CloreJSON();
}