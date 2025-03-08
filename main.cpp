#include "Bank.h"

int main() {
    Bank myBank;
    myBank.cityCoordinates=generateRandomCoordinates(7);
    myBank.distanceMatrix=createDistanceMatrix(myBank.cityCoordinates);
    myBank.showHomePage();
    return 0;
}
